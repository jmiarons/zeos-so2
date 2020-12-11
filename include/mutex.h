#ifndef __MUTEX_H__
#define __MUTEX_H__

struct mutex_t {
  int PID;
  int TID;  /*ID*/
  int locked; /*Variable booleana*/
  struct list_head blocked; /*Cola de bloqueados*/
  struct list_head list; 
};

#endif  /* __MUTEX_H__ */
