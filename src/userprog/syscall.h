#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);
void handle_halt(void);
void handle_exit(int status);
int handle_read(int32_t fd, int32_t *buffer, unsigned size);
int handle_write(int32_t fd, int32_t *buffer, unsigned size);
int handle_open(int32_t* filename);
void handle_close(int fd);

#endif /* userprog/syscall.h */
