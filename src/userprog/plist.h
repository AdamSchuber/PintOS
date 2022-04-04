#ifndef _PLIST_H_
#define _PLIST_H_

#include <stdbool.h>
#include <stdlib.h>

#define MAP_SIZE 128
#define PANIC(message) do { printf("PANIC: %s\n", message); exit(1); } while (0)            // Varför inuti do-while?

typedef char *value_t;
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
                  void (*exec)(key_t k, value_t v, int aux),
                  int aux);

void map_remove_if(struct map *m,
                   bool (*cond)(key_t k, value_t v, int aux),
                   int aux);

#endif /* PLIST_H */

/* Place functions to handle a running process here (process list).
   
   plist.h : Your function declarations and documentation.
   plist.c : Your implementation.

   The following is strongly recommended:

   - A function that given process inforamtion (up to you to create)
     inserts this in a list of running processes and return an integer
     that can be used to find the information later on.

   - A function that given an integer (obtained from above function)
     FIND the process information in the list. Should return some
     failure code if no process matching the integer is in the list.
     Or, optionally, several functions to access any information of a
     particular process that you currently need.

   - A function that given an integer REMOVE the process information
     from the list. Should only remove the information when no process
     or thread need it anymore, but must guarantee it is always
     removed EVENTUALLY.
     
   - A function that print the entire content of the list in a nice,
     clean, readable format.
     
 */
