#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#include <lib/stdbool.h>
#include "filesys/file.h"

void syscall_init (void);
void handle_halt(void);
void handle_exit(int status);

int handle_read(int32_t fd, char * buffer, unsigned size);
int handle_write(int32_t fd, int32_t *buffer, unsigned size);
int handle_open(int32_t* filename);
void handle_close(int fd);

bool handle_create(const char *file, unsigned initial_size);
bool handle_remove(const char *file);

void handle_seek(int fd, unsigned position);
unsigned handle_tell (int fd);
int handle_filesize(int fd);

#endif /* userprog/syscall.h */
