#include <mutex.h>
#include <sched.h>
#include <list.h>
#include <utils.h>
#include <errno.h>

int sys_mutex_init(int n)
{
  struct mutex_t *m;
  if (n >= 20 || n < 0) return -EINVAL;
  m = &mutex_vector[n];
  if (m->owner != 0) return -EBUSY;
  m -> owner = current_t() -> TID; 
  m -> locked = 0;
  INIT_LIST_HEAD(&(m -> blocked));

  return 0;
}

int sys_mutex_lock(int n)
{
  if (n >= 20 || n < 0) return -EINVAL;
  struct mutex_t *m = &mutex_vector[n];
  if (m->owner < 0) return -EINVAL;
  if (m->locked) {
    current_t() -> state = ST_BLOCKED;
    list_add_tail(&current_t()->list, &(m->blocked));
    sched_next_thread_rr();
  }
  else m->locked = 1;
  return 0;
}

int sys_mutex_unlock(int n)
{
  if (n >= 20 || n < 0) return -EINVAL;
  struct mutex_t *m = &mutex_vector[n];
  if (m->owner < 0) return -EINVAL;
  if (!(m -> locked)) return 0;
  struct list_head *l = list_first(&(m->blocked));
  struct thread_struct *t = list_head_to_thread_struct(l);
  update_thread_state_rr(t, &(current_p()->ready_threads));
  if (list_empty(&(m->blocked))) m -> locked = 0;
  return 0;
}

int sys_mutex_destroy(int n)
{
  if (n >= 20 || n < 0) return -EINVAL;
  struct mutex_t *m = &mutex_vector[n];
  if (m -> owner <= 0) return -EINVAL;
  if (current_t()->TID != m->owner) return -EPERM;
  struct thread_struct *t;
  struct list_head *l;
  while (!list_empty(&(m->blocked))) {
    l = list_first(&(m->blocked));
    t = list_head_to_thread_struct(l);
    list_del(l);
    update_thread_state_rr(t, &(t->p->ready_threads));
  }
  m -> locked = 0;
  m -> owner = 0;
  return 0;
}
