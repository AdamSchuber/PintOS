#ifndef _PLIST_H_
#define _PLIST_H_

#include <stdbool.h>
#include <stdlib.h>
#include "threads/synch.h"

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
    struct semaphore sema;
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

pid_t plist_insert(struct plist *m, tid_t tid, pid_t parent_pid, char* name);

void plist_remove_process(struct plist *m, tid_t curr);

void plist_print(struct plist *m, pid_t pid);

#endif /* PLIST_H */
