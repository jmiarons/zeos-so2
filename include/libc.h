/*
 * libc.h - macros per fer els traps amb diferents arguments
 *          definició de les crides a sistema
 */
 
#ifndef __LIBC_H__
#define __LIBC_H__

#include <stats.h>

#define NULL 0

extern int errno;

int write(int fd, char *buffer, int size);

void itoa(int a, char *b);

int strlen(char *a);

void perror();

int getpid();

int fork();

void exit();

int yield();

int pthread_create(int* id, void *(* start_routine) (void *), void* arg);

int pthread_join(int* id, void **value_ptr);

int pthread_exit(void *value_ptr); 

int mutex_init(int n);

int mutex_lock(int n);

int mutex_unlock(int n);

int mutex_destroy(int n);

#endif  /* __LIBC_H__ */
