/*
 * sched.h - Estructures i macros pel tractament de processos
 */

#ifndef __SCHED_H__
#define __SCHED_H__

#include <list.h>
#include <types.h>
#include <mm_address.h>
#include <stats.h>
#include <mutex.h>

#define NR_TASKS      10
#define NR_THREADS    20
#define NR_MUTEX      20
#define KERNEL_STACK_SIZE	1024

#define DEFAULT_QUANTUM_P 20
#define DEFAULT_QUANTUM_T 10


enum state_t { ST_RUN, ST_READY, ST_BLOCKED };

struct task_struct {
  int PID;			/* Process ID. This MUST be the first field of the struct. */
  page_table_entry * dir_pages_baseAddr;
  struct list_head list;	/* Task struct enqueuing */
  enum state_t state;		/* State of the process */
  int total_quantum;		/* Total quantum of the process */
  struct list_head ready_threads;
  struct list_head blocked_threads;
  unsigned int nthread;
};

union task_union {
  struct task_struct task;
  unsigned long stack[KERNEL_STACK_SIZE];    /* pila de sistema, per procés */
};

/*A eliminar la pila de task_union*/

struct thread_struct {
  int TID;
  page_table_entry * dir_pages_baseAddr;
  struct task_struct* p;
  int register_esp;
  enum state_t state;
  int quantum;
  struct list_head list;
  unsigned int blocked_by;
};


union thread_union {
  struct thread_struct task;
  unsigned long stack[KERNEL_STACK_SIZE];
};

extern union task_union protected_tasks[NR_TASKS+2];
extern union thread_union thread_tasks[NR_THREADS + 2];
extern struct mutex_t mutex_vector[NR_MUTEX+2];


extern union task_union *task; /* Vector de tasques */
extern struct thread_struct *idle_task;


#define KERNEL_ESP(t) (DWord) &(t)->stack[KERNEL_STACK_SIZE]

#define INITIAL_ESP KERNEL_ESP(&task[1])

extern struct list_head freequeue;
extern struct list_head readyqueue;
extern struct list_head free_threadqueue;
extern struct list_head mutexqueue;

/* Inicialitza les dades del proces inicial */
void init_task1(void);

void init_idle(void);

void init_sched(void);

void schedule(void);

struct task_struct * current_p();

struct thread_struct* current_t();

void task_switch(union thread_union*t);
void switch_stack(int * save_sp, int new_sp);

void sched_next_rr(void);

void force_task_switch(void);

struct task_struct *list_head_to_task_struct(struct list_head *l);
struct thread_struct* list_head_to_thread_struct(struct list_head * l);
struct mutex_t* list_head_to_mutex_struct(struct list_head * l);

int allocate_DIR(struct task_struct *t);

page_table_entry * get_PT (struct task_struct *t) ;
page_table_entry * get_PT_thread (struct thread_struct *t) ;

page_table_entry * get_DIR (struct task_struct *t) ;

/* Headers for the scheduling policy */
void sched_next_process_rr();
void sched_next_thread_rr();
void update_process_state_rr(struct task_struct *t, struct list_head *dest);
void update_thread_state_rr(struct thread_struct *t, struct list_head *dest);
int needs_sched_rr();
void update_sched_data_rr();

void init_stats(struct stats *s);

#endif  /* __SCHED_H__ */
