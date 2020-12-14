/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <types.h>
#include <hardware.h>
#include <segment.h>
#include <sched.h>
#include <mm.h>
#include <io.h>
#include <utils.h>
#include <p_stats.h>


/**
 * Container for the Task array and 2 additional pages (the first and the last one)
 * to protect against out of bound accesses.
 */
union task_union protected_tasks[NR_TASKS+2]
  __attribute__((__section__(".data.task")));

union thread_union thread_tasks[NR_THREADS + 2]
  __attribute__((__section__(".data.task")));

struct mutex_t mutex_vector[NR_MUTEX + 2]
  __attribute__((__section__(".data.mutex_t")));

union task_union *task = &protected_tasks[1]; /* == union task_union task[NR_TASKS] */



#if 0
struct task_struct *list_head_to_task_struct(struct list_head *l)
{
  return list_entry( l, struct task_struct, list);
}
#endif

extern struct list_head blocked;

// Free task structs
struct list_head freequeue;
// Ready queue
struct list_head readyqueue;

struct list_head mutexqueue;

struct list_head free_threadqueue;

void init_stats(struct stats *s)
{
	s->user_ticks = 0;
	s->system_ticks = 0;
	s->blocked_ticks = 0;
	s->ready_ticks = 0;
	s->elapsed_total_ticks = get_ticks();
	s->total_trans = 0;
	s->remaining_ticks = get_ticks();
}

/* get_DIR - Returns the Page Directory address for task 't' */
page_table_entry * get_DIR (struct task_struct *t)
{
	return t->dir_pages_baseAddr;
}

/* get_PT - Returns the Page Table address for task 't' */
page_table_entry * get_PT (struct task_struct *t)
{
	return (page_table_entry *)(((unsigned int)(t->dir_pages_baseAddr->bits.pbase_addr))<<12);
}

/* get_PT - Returns the Page Table address for thread 't' */
page_table_entry * get_PT_thread (struct thread_struct *t)
{
	return (page_table_entry *)(((unsigned int)(t->dir_pages_baseAddr->bits.pbase_addr))<<12);
}

int allocate_DIR(struct task_struct *t)
{
	int pos;

	pos = ((int)t-(int)task)/sizeof(union task_union);

	t->dir_pages_baseAddr = (page_table_entry*) &dir_pages[pos];

	return 1;
}

void cpu_idle(void)
{
    printk("IDLE\n");
    __asm__ __volatile__("sti": : :"memory");

	while(1)
	{
	;
	}
}


int remaining_quantum_t=0;
int remaining_quantum_p=0;

int get_quantum_p(struct task_struct *t)
{
  return t->total_quantum;
}

int get_quantum_t(struct thread_struct *t)
{
  return t->quantum;
}

void set_quantum(struct task_struct *t, int new_quantum)
{
  t->total_quantum=new_quantum;
}

struct thread_struct *idle_task=NULL;

void update_sched_data_rr(void)
{
  remaining_quantum_t--;
  remaining_quantum_p--;
}

int needs_sched_rr(void)
{
  //printk("thread update\n");
  if ((remaining_quantum_p==0)&&(!list_empty(&readyqueue))) {
          //printk("el need es igual a 1 \n");
          return 1;
  }

  if ((remaining_quantum_t==0)&&(!list_empty(&(current_p()->ready_threads)))) {
          //printk("el need es igual a 2\n");
          return 2;
  }

  if (remaining_quantum_p==0) remaining_quantum_p=get_quantum_p(current_p());
  if (remaining_quantum_t==0) remaining_quantum_t=get_quantum_t(current_t());
  return 0;
}


void update_process_state_rr(struct task_struct *t, struct list_head *dst_queue)
{
  if (t->state!=ST_RUN) list_del(&(t->list));
  if (dst_queue!=NULL)
  {
    list_add_tail(&(t->list), dst_queue);
    if (dst_queue!=&readyqueue) t->state=ST_BLOCKED;
    else
    {
      t->state=ST_READY;
    }
  }
  else t->state=ST_RUN;
}

void update_thread_state_rr(struct thread_struct *t, struct list_head *dst_queue)
{
  if (t->state!=ST_RUN) list_del(&(t->list));
  if (dst_queue!=NULL)
  {
    list_add_tail(&(t->list), dst_queue);
    if (dst_queue!=&(current_p()-> ready_threads)) t->state=ST_BLOCKED;
    else t->state=ST_READY;
  }
  else t->state=ST_RUN;
}

void sched_next_process_rr(void)
{
  struct list_head *e;
  struct list_head *a;
  struct task_struct *p;
  struct thread_struct *t;

  if (!list_empty(&readyqueue)) {
    e = list_first(&readyqueue);
    list_del(e);

    p=list_head_to_task_struct(e);
     //no creo que haga falta comprovar que un proceso tenga un thread en la readyqueue ya que por defecto siempre tiene
    a = list_first(&(p->ready_threads));
    t = list_head_to_thread_struct(a);
    list_del(a);
    p->state=ST_RUN;
    remaining_quantum_p=get_quantum_p(p);
   }
   else
    t=idle_task;

  t->state=ST_RUN;
  remaining_quantum_t=get_quantum_t(t);

  task_switch((union thread_union*)t);
}


void sched_next_thread_rr(void)
{
  struct list_head *e;
  struct thread_struct *t;

  struct task_struct *p = current_p();

  e = list_first(&(p->ready_threads));
  t = list_head_to_thread_struct(e);
  list_del(e);

  t->state=ST_RUN;
  remaining_quantum_t=get_quantum_t(t);

  task_switch((union thread_union*)t);
}

void schedule()
{
  update_sched_data_rr();
  int scheduler = needs_sched_rr();
  if (scheduler == 1)
  {
    update_process_state_rr(current_p(), &readyqueue);
    
    update_thread_state_rr(current_t(), &(current_p()->ready_threads));

    sched_next_process_rr();
  }
  else if (scheduler == 2)
  {
    //printk("He entrado aqui\n");
    update_thread_state_rr(current_t(), &(current_p()->ready_threads));
    sched_next_thread_rr();
  }
}

void init_idle (void)
{
  struct list_head *l = list_first(&freequeue);
  list_del(l);
  struct task_struct *c = list_head_to_task_struct(l);
  union task_union *uc = (union task_union*)c;

  struct list_head *lh_t = list_first(&free_threadqueue);
  list_del(lh_t);
  struct thread_struct *ts = list_head_to_thread_struct(lh_t);
  union thread_union *tu = (union thread_union*)ts;

  c->PID=0;
  ts->TID = 0;

  c->total_quantum=DEFAULT_QUANTUM_P;

  //init_stats(&c->p_stats);

  allocate_DIR(c);

  tu->stack[KERNEL_STACK_SIZE-1]=(unsigned long)&cpu_idle; /* Return address */
  tu->stack[KERNEL_STACK_SIZE-2]=0; /* register ebp */

  ts->register_esp=(int)&(uc->stack[KERNEL_STACK_SIZE-2]); /* top of the stack */

  idle_task=ts;
}

void setMSR(unsigned long msr_number, unsigned long high, unsigned long low);

void init_task1(void) {
  struct list_head *l = list_first(&freequeue);
  list_del(l);
  struct task_struct *c = list_head_to_task_struct(l);

  struct list_head *lh_t = list_first(&free_threadqueue);
  list_del(lh_t);
  struct thread_struct *ts = list_head_to_thread_struct(lh_t);
  union thread_union *tu = (union thread_union*)ts;

  c->PID=1;

  c->total_quantum=DEFAULT_QUANTUM_P;

  c->state=ST_RUN;

  c->nthread = 1;
  remaining_quantum_p=c->total_quantum;

  //init_stats(&c->p_stats);

  allocate_DIR(c);

  set_user_pages(c);

  ts->TID = (c->nthread)++;
  ts->dir_pages_baseAddr = c->dir_pages_baseAddr;
  ts->p = c;
  ts->state = ST_RUN;
  ts->quantum = DEFAULT_QUANTUM_T;
  remaining_quantum_t = ts->quantum;
  tss.esp0=(DWord)&(tu->stack[KERNEL_STACK_SIZE]);
  setMSR(0x175, 0, (unsigned long)&(tu->stack[KERNEL_STACK_SIZE]));

  set_cr3(ts->dir_pages_baseAddr);
}

void init_freequeue()
{
  int i;

  INIT_LIST_HEAD(&freequeue);

  /* Insert all task structs in the freequeue */
  for (i=0; i<NR_TASKS; i++)
  {
    task[i].task.PID=-1;
    INIT_LIST_HEAD(&(task[i].task.ready_threads));
    INIT_LIST_HEAD(&(task[i].task.blocked_threads));
    list_add_tail(&(task[i].task.list), &freequeue);
  }
}

void init_freethreadqueue() {
  INIT_LIST_HEAD(&free_threadqueue);

  for (int i = 0; i < NR_TASKS; i++) {
    thread_tasks[i].task.TID = -1;
    list_add_tail(&(thread_tasks[i].task.list),&free_threadqueue);
  }
}

void init_mutexqueue() {
  INIT_LIST_HEAD(&mutexqueue);

  for (int i = 0; i < NR_MUTEX; i++) {
    mutex_vector[i].ID = i;
    mutex_vector[i].locked = 0;
    INIT_LIST_HEAD(&(mutex_vector[i].blocked));
    list_add_tail(&(mutex_vector[i].list), &mutexqueue);
  }
}

void init_sched()
{
  init_freequeue();
  INIT_LIST_HEAD(&readyqueue);
  init_freethreadqueue();
  init_mutexqueue();
}

struct task_struct* current_p() {
  return (struct task_struct*)(current_t()->p);
}

struct thread_struct* current_t() {
  int ret_value;
  return (struct thread_struct*)( ((unsigned int)&ret_value) & 0xfffff000);
}


struct task_struct* list_head_to_task_struct(struct list_head *l)
{
  return (struct task_struct*)((int)l&0xfffff000);
}

struct thread_struct* list_head_to_thread_struct(struct list_head * l) {
    return (struct thread_struct*)((int)l&0xfffff000);
}

struct mutex_t* list_head_to_mutex_struct(struct list_head * l) {
    return (struct mutex_t*)((int)l&0xfffff000);
}

/* Do the magic of a task switch */
void inner_task_switch(union thread_union *new)
{
  page_table_entry *new_DIR = get_DIR(new->task.p);

  /* Update TSS and MSR to make it point to the new stack */
  tss.esp0=(int)&(new->stack[KERNEL_STACK_SIZE]);
  setMSR(0x175, 0, (unsigned long)&(new->stack[KERNEL_STACK_SIZE]));

  /* TLB flush. New address space */
  set_cr3(new_DIR);

  switch_stack(&current_t()->register_esp, new->task.register_esp);
}


/* Force a task switch assuming that the scheduler does not work with priorities */
void force_task_switch()
{
  update_process_state_rr(current_p(), &readyqueue);

  sched_next_process_rr();
}
