#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "plist.h"

void map_init(struct map *m)
{
    for (int i = 0; i < MAP_SIZE; ++i)
    {
        m->content[i] = NULL;
    }
}

// return -1 if full
key_t map_insert(struct map *m, value_t v)
{
    for (int i = 0; i < MAP_SIZE; ++i)
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
value_t map_find(struct map *m, key_t k)
{
    if (k > MAP_SIZE - 1 || k < 0)
    {
        value_t null_ptr = NULL;
        return null_ptr;
    }
    return m->content[k];
}

// return char* = NULL if no such element was find
value_t map_remove(struct map *m, key_t k)
{
    if (k > MAP_SIZE - 1 || k < 0)
    {
        value_t null_ptr = NULL;
        return null_ptr;
    }

    value_t tmp = m->content[k];
    m->content[k] = NULL;
    return tmp;
}

void map_for_each(struct map *m,
                  void (*exec)(key_t k, value_t v, int aux),
                  int aux)
{
    for (int i = 0; i < MAP_SIZE; ++i)
    {
        if (m->content[i] != NULL)
            (*exec)(i, m->content[i], aux);
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