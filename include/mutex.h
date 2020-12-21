#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <list.h>

struct mutex_t {
  int ID;  /*ID*/
  int locked; /*Variable booleana*/
  int owner; /*Variable que indicia que TID es el owner del mutex*/
  struct list_head blocked; /*Cola de bloqueados*/
};

int sys_mutex_init(int n);

int sys_mutex_lock(int n);

int sys_mutex_unlock(int n);

int sys_mutex_destroy(int n);

#endif  /* __MUTEX_H__ */
