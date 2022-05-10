#include <stddef.h>

#include "flist.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

void map_init(struct map *m)
{
    for (int i = 2; i < MAP_SIZE; ++i)
    {
        m->content[i] = NULL;
    }
}

// return -key_t if full to indicate that 
// file is already in open file table
key_t map_insert(struct map *m, value_t v)
{
    for (int i = 2; i < MAP_SIZE; ++i)
    {
        if (v == m->content[i])
        {
            return -i;      
        }
        else if (m->content[i] == NULL)
        {
            m->content[i] = v;
            return i;
        }
    }
    return -1;
}

// return char* = NULL if no such element was find
value_t map_find(struct map *m, key_t k)
{
    if (k > MAP_SIZE - 1 || k < 2)
        return NULL;
    
    return m->content[k];
}

// return value_t = NULL if no such element was find
value_t map_remove(struct map *m, key_t k)
{
    if (k > MAP_SIZE - 1 || k < 2)
    {
        return NULL;
    }

    value_t tmp = m->content[k];
    m->content[k] = NULL;
    return tmp;
}

void map_for_each(struct map *m,
                  void (*exec)(value_t v))
{
    for (int i = 0; i < MAP_SIZE; ++i)
    {
        if (m->content[i] != NULL)
            (*exec)(m->content[i]);
    }
}

void map_remove_if(struct map *m,
                   bool (*cond)(key_t k, value_t v, int aux),
                   int aux)
{
    for (int i = 0; i < MAP_SIZE; ++i)
    {
        if (m->content[i] != NULL)
        {
            if ((*cond)(i, m->content[i], aux))
                map_remove(m, i);
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
