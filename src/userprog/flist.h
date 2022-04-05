#ifndef _MAP_H_
#define _MAP_H_

#ifndef _PLIST_H_
#define _PLIST_H_

#include <stdbool.h>
#include <stdlib.h>

#define MAP_SIZE 128

typedef struct file* value_t;
typedef int key_t;

struct map
{
    value_t content[MAP_SIZE];
};

void map_init(struct map *m);

key_t map_insert(struct map *m, value_t v);

value_t map_find(struct map *m, key_t k);

value_t map_remove(struct map *m, key_t k);

void map_for_each(struct map *m,
                  void (*exec)(value_t v));

void map_remove_if(struct map *m,
                   bool (*cond)(key_t k, value_t v, int aux),
                   int aux);

#endif /* PLIST_H */

/* Place functions to handle a running process here (process list).
   
//    plist.h : Your function declarations and documentation.
//    plist.c : Your implementation.

//    The following is strongly recommended:

//    - A function that given process inforamtion (up to you to create)
//      inserts this in a list of running processes and return an integer
//      that can be used to find the information later on.

//    - A function that given an integer (obtained from above function)
//      FIND the process information in the list. Should return some
//      failure code if no process matching the integer is in the list.
//      Or, optionally, several functions to access any information of a
//      particular process that you currently need.

//    - A function that given an integer REMOVE the process information
//      from the list. Should only remove the information when no process
//      or thread need it anymore, but must guarantee it is always
//      removed EVENTUALLY.
     
//    - A function that print the entire content of the list in a nice,
//      clean, readable format.
     
 */


/* Place code to keep track of your per-process open file table here.
 *
 * (The system-wide open file table exist as part of filesys/inode.c )
 *
 * User-mode code use a file by first opening it to retrieve a file
 * descriptor (integer) that uniquely identifies the open file for the
 * operation system. This file descriptor is then passed to read or
 * write to use the file, and finally to close to led the operating
 * system release any resources associated with the file.
 *
 * The kernel use a file in the same way, but use pointer to a file
 * structure to uniquely identify a file instead of an integer. If we
 * do not care for security we could pass this pointer directly to
 * user-mode code when a file is opened and expect the same pointer
 * back when the file is used in read, write or close.
 *
 * But we do care for security, we want to:
 *
 * - Hide kernel addresses and data from (untrusted) user-mode code
 *
 * - Perform validity checks that a file descriptor was indeed
 *   obtained from a call to open by the same process
 *
 * - Verify that a file descriptor was not closed
 *
 * - Make sure the kernel can close all files associated to a process
 *   as soon as it terminates
 *
 * This is best done by shielding kernel data from user code. Now the
 * kernel must keep track of which file descriptors a certain process
 * have open, and which kernel file pointer that are associated to
 * each file descriptor. This mapping is for you to solve, and the
 * data structure you need may be placed in this file.
 *
 *
 * User-mode sequence                 Kernel sequence
 * ------------------                 ---------------
 *
 * char buffer[5];                    struct file* fp;
 *
 * int   fd = open("example.txt");    fp = filesys_open(...)
 *       |                            \_________
 *       |                                      \
 *       V                                       V
 * read( fd, buffer, 5);              file_read( fp, ...)
 *       |                                       |
 *       V                                       V
 * write(fd, buffer, 5);              file_write(fp, ...)
 *       |                                       |
 *       V                                       V
 * close(fd);                         file_close(fp);
 *
 *
 * A (very) simple implementation data structure equivalent to a C++
 * std::map is recommended.
 *
 * This structure can be placed either globally or locally for each
 * process. If you go for a global map, consider how to remember which
 * process that opened each file. If you go for a local map, consider
 * where to declare and initialize it correctly. In both cases, consider
 * what size limit that may be appropriate.
 */

#endif
