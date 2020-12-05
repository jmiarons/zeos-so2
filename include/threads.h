#include <sched.h>

struct thread_struct {
  int TID;
  page_table_entry * dir_pages_baseAddr;
  struct task_struct* p;
  int register_esp;
  enum state_t state;
  int quantum;
  struct list_head list;
};


union thread_union {
  struct thread_struct task;
  unsigned long stack[KERNEL_STACK_SIZE];
};
