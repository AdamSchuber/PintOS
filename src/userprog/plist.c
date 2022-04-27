#include <stddef.h>

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "plist.h"
#include "threads/malloc.h"
#include "threads/synch.h"

void plist_init(struct plist *m)
{
    for (int i = 0; i < PLIST_SIZE; ++i)
    {
        m->content[i] = NULL;
    }
}

bool plist_parent_running(struct plist *m, pid_t parent_pid)
{
    return m->content[parent_pid]->is_running;
}

pid_t plist_get_pid(struct plist *m, tid_t tid)
{
    for (int i = 0; i < PLIST_SIZE; ++i)
    {
        if (m->content[i] == NULL)
        {
            continue;
        }
        else if (tid == m->content[i]->tid)
        {
            return i;
        }
    }
    return -1; //<--- No such process
}

void plist_print(struct plist *m, pid_t pid)
{
    if (m->content[pid] == NULL)
    {
        return;
    }
    else if (m->content[pid]->valid_row != true)
    {
        printf("| %d    | %d         | %d         | %s  |\n", pid, m->content[pid]->parent_pid, m->content[pid]->exit_status, m->content[pid]->name);
    }
}

// return -key_t if full to indicate that
// file is already in global plist
pid_t plist_insert(struct plist *m, tid_t tid, pid_t parent_pid, char* name)
{
    for (int i = 0; i < PLIST_SIZE; ++i)
    {
        if (m->content[i] == NULL)
        {
            printf("Name: %s ID: %d| PLIST_INSERT ENTERED\n", name, i);
            process_ptr new_ptr = malloc(sizeof(struct process_info));
            new_ptr->tid = tid;
            new_ptr->parent_pid = parent_pid;
            new_ptr->is_running = true;
            new_ptr->parent_is_running = true;
            new_ptr->valid_row = false;
            sema_init(&new_ptr->sema, 0);
            new_ptr->name = malloc(sizeof(*name));
            strlcpy(new_ptr->name, name, strlen(name));  
            m->content[i] = new_ptr;
            return i;
        }
        else if (m->content[i]->valid_row == true)
        {
            free(m->content[i]);
            process_ptr new_ptr = malloc(sizeof(struct process_info));
            new_ptr->tid = tid;
            new_ptr->parent_pid = parent_pid;
            new_ptr->is_running = true;
            new_ptr->parent_is_running = true;
            new_ptr->valid_row = false;
            sema_init(&new_ptr->sema, 0);
            new_ptr->name = malloc(sizeof(*name));
            strlcpy(new_ptr->name, name, strlen(name));  
            return i;
        }
    }
    return -1;
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
