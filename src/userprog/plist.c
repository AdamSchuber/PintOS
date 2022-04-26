#include <stddef.h>

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "plist.h"
#include "threads/malloc.h"

void plist_init(struct plist *m)
{
    for (int i = 0; i < PLIST_SIZE; ++i)
    {
        m->content[i] = NULL;
    }
}

process_ptr plist_process_info_init(tid_t tid, pid_t parent_pid, char *name)
{
    process_ptr this = malloc(sizeof(struct process_info));
    this->tid = tid;
    this->parent_pid = parent_pid;
    this->exit_status = 999;
    this->parent_is_running = true;
    this->valid_row = false;
    memcpy(this->name, name, sizeof(*name));
    return this;
}

bool plist_parent_running(struct plist *m, pid_t parent_pid)
{
    return m->content[parent_pid]->is_running;
}

pid_t plist_get_pid(struct plist *m, tid_t tid)
{
    for (int i = 0; i < PLIST_SIZE; ++i)
    {
        if (tid == m->content[i]->tid)
            return i;
    }
    return -1; //<--- No such process
}

void plist_print(struct plist *m, pid_t pid)
{
    if (m->content[pid]->valid_row != true)
    {
        printf("| %d    | %d       | %d       | %s |\n", pid, m->content[pid]->parent_pid, m->content[pid]->exit_status, m->content[pid]->name);
    }
}

// return -key_t if full to indicate that
// file is already in global plist
pid_t plist_insert(struct plist *m, process_ptr v)
{
    for (int i = 0; i < PLIST_SIZE; ++i)
    {
        if (m->content[i] == NULL || m->content[i]->valid_row == true)
        {
            m->content[i] = v;
            return i;
        }
    }
    return -1;
}

// return char* = NULL if no such element was find
process_ptr plist_find(struct plist *m, key_t k)
{
    if (k > PLIST_SIZE - 1 || k < 0)
    {

        return NULL;
    }
    return m->content[k];
}

// return process_ptr = NULL if no such element was find
process_ptr plist_remove(struct plist *m, key_t k)
{
    if (k > PLIST_SIZE - 1 || k < 0)
    {
        process_ptr null_ptr = NULL;
        return null_ptr;
    }

    process_ptr tmp = m->content[k];
    m->content[k] = NULL;
    return tmp;
}

void plist_for_each(struct plist *m,
                    void (*exec)(process_ptr v))
{
    for (int i = 0; i < PLIST_SIZE; ++i)
    {
        if (m->content[i] != NULL)
            (*exec)(m->content[i]);
    }
}

void plist_remove_process(struct plist *m, tid_t curr_t)
{
    // uppdatera parent_running_status
    pid_t curr_p = plist_get_pid(m, curr_t);
    if (curr_p != -1)
    {
        bool parent_status = plist_parent_running(m, m->content[curr_p]->parent_pid);
        m->content[curr_p]->parent_is_running = parent_status;

        // sätt valid_row till true om is_running och parent_is_running är false
        if (m->content[curr_p]->is_running == false && m->content[curr_p]->parent_is_running == false)
            m->content[curr_p]->valid_row = true;
    }
}

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