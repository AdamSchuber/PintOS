#ifndef _PLIST_H_
#define _PLIST_H_

#include <stdbool.h>
#include <stdlib.h>

#define PLIST_SIZE 512

typedef int pid_t;
typedef int tid_t;

struct process_info
{
    tid_t   tid;
    pid_t   parent_pid;
    char*   name;
    int     exit_status;
    bool    parent_is_running;
    bool    is_running;
    bool    valid_row;
};

typedef struct process_info* process_ptr;
typedef int key_t;

struct plist
{
    process_ptr content[PLIST_SIZE];
};

bool plist_parent_running(struct plist *m, pid_t parent_pid);

pid_t plist_get_pid(struct plist *m, tid_t tid);

void plist_init(struct plist *m);

pid_t plist_insert(struct plist *m, process_ptr v);

process_ptr plist_process_info_init(tid_t tid, pid_t parent_pid, char* name);

process_ptr plist_find(struct plist *m, key_t k);

process_ptr plist_remove(struct plist *m, key_t k);

void plist_for_each(struct plist *m,
                  void (*exec)(process_ptr v));

void plist_remove_process(struct plist *m, tid_t curr);

void plist_print(struct plist *m, pid_t pid);

#endif /* PLIST_H */

/* Place functions to handle a running process here (process list).
   
//    plist.h : Your function declarations and documentation.
//    plist.c : Your implementation.

//    The following is strongly recommended:

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
