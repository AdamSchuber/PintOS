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

static void syscall_handler(struct intr_frame *);

void handle_halt(void)
{
  power_off();
}

void handle_exit(int status)
{
  printf("%s%d\n", "THREAD_STATUS: ", status);
  thread_exit();
}

// Return -1 if filedescriptor is incorrect, or chars read if correct
int handle_read(int32_t fd, int32_t *buffer, unsigned size)
{
  // Checks if file descriptor is correct for read
  if (fd == STDIN_FILENO)
  {
    int num_of_chars = 0;
    // Loops til size is reached, gets characters from input_getc
    // and puts them in buffer and putbuf() to display in program
    for (int i = 0; i < (int)(size); ++i)
    {
      char input = input_getc();
      // Changes \r to \n so that ENTER button works as intended
      if (input == '\r')
        input = '\n';
      buffer[i] = input;
      putbuf((const char *)(&input), 1);
      ++num_of_chars;
    }
    return num_of_chars;
  }
  return -1;
}

// Return -1 if filedescriptor is incorrect, or chars written if correct
int handle_write(int32_t fd, int32_t *buffer, unsigned size)
{
  // Checks if file descriptor is correct for write
  if (fd == STDOUT_FILENO)
  {
    putbuf((const char *)(buffer), size);
    return size;
  }
  return -1;
}

int handle_open(int32_t filename)
{
  // Check if filename exit in filesystem
  if (filesys_open(filename) != -1)
  {
  
  }

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

  switch (esp[0])
  {
  case SYS_HALT:
  {
    handle_halt();
    break;
  }

  case SYS_EXIT:
  {
    handle_exit(esp[1]);
    break;
  }

  case SYS_READ:
  {
    // printf("%s\n", "ARRIVES AT SYS_READ!");
    f->eax = handle_read((int32_t)(esp[1]), (int32_t *)(esp[2]), (int32_t)(esp[3]));
    break;
  }

  case SYS_WRITE:
  {
    // printf("%s\n", "ARRIVES AT SYS_WRITE!");
    f->eax = handle_write((int32_t)(esp[1]), (int32_t *)(esp[2]), (int32_t)(esp[3]));
    break;
  }

  case SYS_OPEN:
  {
    f->eax = handle_open((int32_t)(esp[1]));
    break;
  }

  case SYS_CLOSE:
  {
    break;
  }

  case SYS_REMOVE:
  {
    break;
  }

  case SYS_CREATE:
  {
    break;
  }

  case SYS_SEEK:
  {
    break;
  }

  case SYS_TELL:
  {
    break;
  }

  case SYS_FILESIZE:
  {
    break;
  }

  default:
  {
    printf("Executed an unknown system call!\n");

    printf("Stack top + 0: %d\n", esp[0]);
    printf("Stack top + 1: %d\n", esp[1]);

    thread_exit();
  }
  }
}
