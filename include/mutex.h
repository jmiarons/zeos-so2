#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <list.h>

struct mutex_t {
  int ID;  /*ID*/
  int locked; /*Variable booleana*/
  struct list_head blocked; /*Cola de bloqueados*/
  struct list_head list; 
};

int sys_mutex_init(struct mutex_t *mutex);

void sys_mutex_destroy(struct mutex_t *mutex);

#endif  /* __MUTEX_H__ */
