#include <mutex.h>
#include <sched.h>
#include <list.h>
#include <utils.h>

int sys_mutex_init(struct mutex_t *mutex)
{
  struct list_head *l;
  struct mutex_t *m;
  l = list_first(&mutexqueue);
  m = list_head_to_mutex_struct(l);
  list_del(l);

  copy_to_user(m, mutex, sizeof(struct mutex_t));

  return m->ID;
}

int sys_mutex_lock(struct mutex_t *mutex)
{
  if (mutex->locked) {
  update_thread_state_rr(current_t(), &(mutex->blocked));
  sched_next_thread_rr();
  }
  else mutex->locked = 1;
  return 0;
}

int sys_mutex_unlock(struct mutex_t *mutex)
{
  struct list_head *l;
  struct thread_struct *t;
  if (!(mutex -> locked)) return -1;
  l = list_first(&(mutex->blocked));
  t = list_head_to_thread_struct(l);
  update_thread_state_rr(t, &(current_p()->ready_threads));
  if (list_empty(&(mutex->blocked))) mutex -> locked = 0;
  return 0;
}

void sys_mutex_destroy(struct mutex_t *mutex)
{
  mutex -> locked = 0;
  struct thread_struct *t;
  struct list_head *l;
  while (!list_empty(&(mutex->blocked))) {
    l = list_first(&(mutex->blocked));
    t = list_head_to_thread_struct(l);
    list_del(l);
    update_thread_state_rr(t, &(t->p->ready_threads));
  }

  list_add_tail(&(mutex->list), &mutexqueue);
}
