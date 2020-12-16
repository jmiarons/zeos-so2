/*
 * libc.h - macros per fer els traps amb diferents arguments
 *          definició de les crides a sistema
 */
 
#ifndef __LIBC_H__
#define __LIBC_H__

#include <stats.h>
#include <sched.h>

extern int errno;

int write(int fd, char *buffer, int size);

void itoa(int a, char *b);

int strlen(char *a);

void perror();

int getpid();

int fork();

void exit();

int yield();

int pthread_create(struct thread_struct* t , void *(* start_routine) (void *), void* arg);

int pthread_join(struct thread_struct *thread, void **value_ptr);

int pthread_exit(void *value_ptr); 

int mutex_init(struct mutex_t *mutex);

int mutex_destroy(struct mutex_t *mutex);

#endif  /* __LIBC_H__ */
