#include <stddef.h>

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "plist.h"

void plist_init(struct plist *m)
{
    for (int i = 0; i < PLIST_SIZE; ++i)
    {
        m->content[i] = NULL;
    }
}

process_ptr process_info_init( tid_t tid, pid_t parent_pid)
{
    process_ptr this;
    this->tid = tid;
    this->parent_pid = parent_pid;
    return this;
}

// return -key_t if full to indicate that 
// file is already in open file table
key_t plist_insert(struct plist *m, process_ptr v)
{
    for (int i = 0; i < PLIST_SIZE; ++i)
    {
        if (m->content[i] == NULL)
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

void plist_remove_if(struct plist *m,
                   bool (*cond)(key_t k, process_ptr v, int aux),
                   int aux)
{
    for (int i = 0; i < PLIST_SIZE; ++i)
    {
        if (m->content[i] != NULL)
        {
            if ((*cond)(i, m->content[i], aux))
                plist_remove(m, i);
        }
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