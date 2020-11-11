/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>
#include <entry.h>

union task_union task[NR_TASKS]
  __attribute__((__section__(".data.task")));

struct task_struct *list_head_to_task_struct(struct list_head *l)
{
  return list_entry( l, struct task_struct, list);
}

int next_PID = 1000;

extern struct list_head blocked;
struct list_head freequeue;
struct list_head readyqueue;
int quantum_left;

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

int allocate_DIR(struct task_struct *t)
{
	int pos;

	pos = ((int)t-(int)task)/sizeof(union task_union);

	t->dir_pages_baseAddr = (page_table_entry*) &dir_pages[pos];

	return 1;
}

void cpu_idle(void)
{
	__asm__ __volatile__("sti": : :"memory");
  printk("IDLE\n");
	while(1)
	{
	;
	}
}

void init_idle (void) {
  struct list_head* first = list_first(&freequeue);
  list_del(first);
  struct task_struct* task_s = list_head_to_task_struct(first);
  union task_union* task_u = (union task_union *) task_s;

  task_s -> PID = 0;

  allocate_DIR(task_s);

  task_u -> stack[KERNEL_STACK_SIZE-1] = (unsigned long)&cpu_idle;
  task_u -> stack[KERNEL_STACK_SIZE-2] = 0;

  task_u -> task.kernel_esp = (unsigned long)&task_u -> stack[KERNEL_STACK_SIZE-2];

  idle_task = task_u;
}

void init_task1(void) {
  struct list_head *first = list_first(&freequeue);
  list_del(first);
  struct task_struct *task_s = list_head_to_task_struct(first);
  union task_union *task_u = (union task_union *) task_s;

  task_s->PID = next_PID;
  ++next_PID;

  allocate_DIR(task_s);
  set_user_pages(task_s);

  tss.esp0 = (unsigned int) &task_u->stack[KERNEL_STACK_SIZE];
  writeMSR(0x175, tss.esp0);

  set_cr3(task_s->dir_pages_baseAddr);

}


void init_queues() {
  INIT_LIST_HEAD(&freequeue);
  for (int i = 0; i < NR_TASKS; ++i) {
    task[i].task.PID = INVALID;
    list_add_tail(&task[i].task.list, &freequeue);
  }

  INIT_LIST_HEAD(&readyqueue);
}


void init_sched() {
  init_queues();
}

struct task_struct* current()
{
  int ret_value;

  __asm__ __volatile__(
  	"movl %%esp, %0"
	: "=g" (ret_value)
  );
  return (struct task_struct*)(ret_value&0xfffff000);
}


void inner_task_switch(union task_union *t) {

  writeMSR(0x175, (unsigned int) KERNEL_ESP(t)); /*Usamos sysenter*/
  set_cr3(t->task.dir_pages_baseAddr);
  unsigned long current_kesp = current()->kernel_esp;
  unsigned long new_kesp = t->task.kernel_esp;
	__asm__ __volatile__ (
		"movl %%ebp,%0;"
		"movl %1,%%esp;"
		"popl %%ebp;"
		"ret;"
		:
		: "g" (current_kesp), "g" (new_kesp)
	);
}

void schedule() {
  
}

int get_quantum (struct task_struct *t) {
  return t->quantum;
}

void set_quantum (struct task_struct *t, int new_quantum) {
  t->quantum = new_quantum;
}
void update_sched_data_rr () {
  quantum_left--;
}

int needs_sched_rr () {
  return (current() -> PID == 0 || quantum_left <= 0) && !list_empty(&readyqueue); //si estoy en idle o no queda quantum y hay mas procesos devuelvo un 1
}

void update_process_state_rr(struct task_struct *t, struct list_head *dest) {
  if (t->state != ST_RUN) list_del(&t->list);
  if (dest == NULL) 
    t->state = ST_RUN;
  else {
    list_add_tail(&t->list, dest);
    if (dest == &readyqueue)
      t->state = ST_READY;
    else
      t->state = ST_BLOCKED;
  }
}

void sched_next_rr() {
  if (!list_empty(&readyqueue)) {
    task_switch(list_first(&readyqueue));
  } else 
    task_switch((union task_union *) &idle_task);
}

void enqueue_process () {
  update_process_state_rr(current(), &readyqueue);
}

