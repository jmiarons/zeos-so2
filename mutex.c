#include <mutex.h>
#include <sched.h>
#include <list.h>

int sys_mutex_init(struct mutex_t *mutex)
{
  struct list_head *l;
  struct mutex_t *m;
  l = list_first(&mutexqueue);
  m = list_head_to_mutex_struct(l);
  list_del(l);

  mutex -> ID = m -> ID;
  mutex -> locked = m -> locked;
  mutex -> blocked = m -> blocked;

  return mutex->ID;
}

void sys_mutex_destroy(struct mutex_t *mutex)
{
  mutex -> locked = 0;
  while (!list_empty(&(mutex->blocked))) 
    list_del(list_first(&(mutex->blocked)));

  list_add_tail(&(mutex->list), &mutexqueue);
}
