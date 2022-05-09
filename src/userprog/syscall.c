#include <stdio.h>
#include <syscall-nr.h>
#include "userprog/syscall.h"
#include "threads/interrupt.h"
#include "threads/thread.h"

/* header files you probably need, they are not used yet */
#include <string.h>
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "threads/vaddr.h"
#include "threads/init.h"
#include "userprog/pagedir.h"
#include "userprog/process.h"
#include "devices/input.h"
#include <lib/stdio.h>
#include "devices/timer.h"

/* Verify all addresses from and including 'start' up to but excluding
 * (start+length). */
bool verify_fix_length(void *start, unsigned length)
{
  if ((unsigned int*)start >= PHYS_BASE || (unsigned int*)start + length >= PHYS_BASE)
    return false;

  // Check first if start is even in the page directory
  if (pagedir_get_page(thread_current()->pagedir, start) == NULL)
    return false;

  // Get the page number for the start addresses page
  unsigned int prev_pg = pg_no(start);

  // Iterate through all addresses from start to length
  // and if the page has changed since the previous iteration,
  // check once again whether the page is in the directory
  for (unsigned int i = 0; i < length; ++i)
  {
    unsigned int curr_pg = pg_no(start + i);
    if (curr_pg != prev_pg)
    {
      prev_pg = curr_pg;
      void *check_addr = start + i;
      if (pagedir_get_page(thread_current()->pagedir, check_addr) == NULL)
        return false;
    }
  }
  return true;
}

/* Verify all addresses from and including 'start' up to and including
 * the address first containg a null-character ('\0'). (The way
 * C-strings are stored.)
 */
bool verify_variable_length(const char *start)
{
  if ((unsigned int*)start >= PHYS_BASE)
    return false;

  // Check first if start is even in the page directory
  if (pagedir_get_page(thread_current()->pagedir, start) == NULL)
    return false;

  unsigned int prev_pg = 0, curr_pg = pg_no(start);
  void *check_addr = start;

  // Iterate as long as check_addr is not NULL terminated end of string,
  // and if the page has changed since the previous iteration,
  // check once again whether the page is in the directory
  do
  {
    curr_pg = pg_no(check_addr);
    if (curr_pg != prev_pg)
    {
      prev_pg = curr_pg;
      if (pagedir_get_page(thread_current()->pagedir, check_addr) == NULL)
        return false;
    }

    if (*((char*)check_addr) == '\0')
      return true;

  } while (++check_addr);
  return false;
}

static void syscall_handler(struct intr_frame *);

void handle_plist(void)
{
  process_print_list();
}

void handle_sleep(int millis)
{
  timer_msleep(millis);
}

int handle_exec(const char* command_line)
{
  if (!(verify_variable_length(command_line)))
    process_exit(-1);
    
  return process_execute(command_line);
}

int handle_wait(int pid)
{
  if (pid < 0 || PLIST_SIZE < pid)
    process_exit(-1);

  return process_wait(pid);
}

void handle_halt(void)
{
  power_off();
}

void handle_exit(int status)
{
  process_exit(status);
}

// Return -1 if filedescriptor is incorrect, or chars read if correct
int handle_read(int fd, char *buffer, unsigned size)
{
  if (!(verify_fix_length(buffer, size)) || size < 0)
    process_exit(-1);
    

  // Checks if file descriptor is correct for read
  if (fd != STDOUT_FILENO)
  {
    int num_of_chars = 0;
    // Enters if-block if fd is is standard input, else it is read
    // as a regular file.
    
    // Loops til size is reached, gets characters from input_getc
    // and puts them in buffer and putbuf() to display in program
    if (fd == STDIN_FILENO)
    {
      for (int i = 0; i < (int)(size); ++i)
      {
        char input = input_getc();
        *(buffer + i) = input;
        putbuf((const char *)(&input), 1);
        ++num_of_chars;
      }
      return num_of_chars;
    }
    else
    {
      struct file *file_ptr = map_find(&thread_current()->open_file_table, fd);
      if (file_ptr != NULL)
      {
        return file_read(file_ptr, buffer, size);
      }
    }
  }
  return -1;
}

// Return -1 if filedescriptor is incorrect, or chars written if correct
int handle_write(int fd, char *buffer, unsigned size)
{
  if (!(verify_fix_length(buffer, size)))
    handle_exit(-1);


  // Checks if file descriptor is correct for write
  if (fd != STDIN_FILENO)
  {
    // Enters if-block if fd is is standard output so it can write to screen,
    // else it is written to as if it is a regular file.
    if (fd == STDOUT_FILENO)
    {
      putbuf((const char *)(buffer), size);
    }
    else
    {
      struct file *file_ptr = map_find(&thread_current()->open_file_table, fd);
      if (file_ptr != NULL)
      {
        file_write(file_ptr, (char *)(buffer), size);
      }
      else
      {
        return -1;
      }
    }
    return size;
  }
  return -1;
}

int handle_open(const char *filename)
{
  if (!(verify_variable_length(filename)))
    process_exit(-1);

  // Check if filename exists in filesystem, returns null if not
  struct file *file_ptr = filesys_open((char *)filename);
  if (file_ptr != NULL)
  {
    // Inserts file into the threads open filetable and is given a
    // file descriptor
    int fd = map_insert(&thread_current()->open_file_table, file_ptr);

    // If file table fails to insert, remove file from file system
    if (fd == -1)
      filesys_close(file_ptr);
    return fd;
  }
  return -1;
}

void handle_close(int fd)
{
  struct file *file_ptr = map_find(&thread_current()->open_file_table, fd);
  if (file_ptr != NULL)
  {
    // Removes the file and descriptor from the table
    map_remove(&thread_current()->open_file_table, fd);
    // Cloeses the file such that it does no longer exit in the
    // file system
    filesys_close(file_ptr);
  }
}

bool handle_create(const char *file, unsigned initial_size)
{
  if (!(verify_variable_length(file)) || file == NULL)
    process_exit(-1);

  return filesys_create(file, initial_size);
}

bool handle_remove(const char *file)
{
  if (!(verify_variable_length(file)))
    process_exit(-1);

  return filesys_remove(file);
}

void handle_seek(int fd, unsigned position)
{
  struct file *file_ptr = map_find(&thread_current()->open_file_table, fd);
  // Seek action will be carryed out as long as the seek position is
  // not outside the given bounds of the file
  if (file_ptr != NULL)
    if (position <= (unsigned int)file_length(file_ptr))
      file_seek(file_ptr, position);
}

unsigned handle_tell(int fd)
{
  struct file *file_ptr = map_find(&thread_current()->open_file_table, fd);
  if (file_ptr != NULL)
    return file_tell(file_ptr);
  return -1;
}

int handle_filesize(int fd)
{
  struct file *file_ptr = map_find(&thread_current()->open_file_table, fd);
  if (file_ptr != NULL)
    return file_length(file_ptr);
  return -1;
} 

/*########################################################################*/

void syscall_init(void)
{
  intr_register_int(0x30, 3, INTR_ON, syscall_handler, "syscall");
}

/* This array defined the number of arguments each syscall expects.
   For example, if you want to find out the number of arguments for
   the read system call you shall write:

   int sys_read_arg_count = argc[ SYS_READ ];

   All system calls have a name such as SYS_READ defined as an enum
   type, see `lib/syscall-nr.h'. Use them instead of numbers.
 */
const int argc[] = {
    /* basic calls */
    0, 1, 1, 1, 2, 1, 1, 1, 3, 3, 2, 1, 1,
    /* not implemented */
    2, 1, 1, 1, 2, 1, 1,
    /* extended, you may need to change the order of these two (plist, sleep) */
    0, 1};

static void
syscall_handler(struct intr_frame *f)
{
  int32_t *esp = (int32_t *)f->esp;

  int required_args = argc[esp[0]];

    // Checks all that all arguments passed thorugh to the sys_call is correct
  if (!(verify_fix_length(esp + 1, sizeof(int) * required_args)))
    return process_exit(-1);

  if (esp[0] >= SYS_NUMBER_OF_CALLS || esp[0] < 0)
    return process_exit(-1);

  switch (esp[0])
  {
  case SYS_HALT:
    handle_halt();
    break;
  case SYS_EXIT:
    handle_exit(esp[1]);
    break;
  case SYS_EXEC:
    f->eax = handle_exec((const char*)(esp[1]));
    break;
  case SYS_WAIT:
    f->eax = handle_wait((int)(esp[1]));
    break;
  case SYS_SLEEP:
    handle_sleep((int)(esp[1]));
    break;
  case SYS_PLIST:
    handle_plist();
    break;
  case SYS_READ:
    f->eax = handle_read((int32_t)(esp[1]), (char *)(esp[2]), (int32_t)(esp[3]));
    break;
  case SYS_WRITE:
    f->eax = handle_write((int32_t)(esp[1]), (char *)(esp[2]), (int32_t)(esp[3]));
    break;
  case SYS_OPEN:
    f->eax = handle_open((char *)(esp[1]));
    break;
  case SYS_CLOSE:
    handle_close((int32_t)(esp[1]));
    break;
  case SYS_CREATE:
    f->eax = handle_create((char *)(esp[1]), (int32_t)(esp[2]));
    break;
  case SYS_REMOVE:
    f->eax = handle_remove((char *)(esp[1]));
    break;
  case SYS_SEEK:
    handle_seek((int32_t)(esp[1]), (int32_t)(esp[2]));
    break;
  case SYS_TELL:
    f->eax = handle_tell((int32_t)(esp[1]));
    break;
  case SYS_FILESIZE:
    f->eax = handle_filesize((int32_t)(esp[1]));
    break;
  default:
  {
    printf("Executed an unknown system call!\n");

    printf("Stack top + 0: %d\n", esp[0]);
    printf("Stack top + 1: %d\n", esp[1]);

    thread_exit();
  }
  }
}
