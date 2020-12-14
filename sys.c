/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>

#include <utils.h>

#include <io.h>

#include <mm.h>

#include <mm_address.h>

#include <sched.h>

#include <p_stats.h>

#include <errno.h>

#define LECTURA 0
#define ESCRIPTURA 1

void * get_ebp();

int check_fd(int fd, int permissions)
{
  if (fd!=1) return -EBADF;
  if (permissions!=ESCRIPTURA) return -EACCES;
  return 0;
}


int sys_ni_syscall()
{
	return -ENOSYS;
}

int sys_getpid()
{
	return current_t()->p->PID;
}

int global_PID=1000;

int ret_from_fork()
{
  return 0;
}

int sys_fork(void)
{
  struct list_head *lhcurrent = NULL;
  union task_union *uchild;

  struct list_head *t_lh;
  union thread_union *uthread;

  /* Any free task_struct? */
  if (list_empty(&freequeue)) return -ENOMEM;
  if (list_empty(&free_threadqueue)) return -ENOMEM;

  lhcurrent=list_first(&freequeue);
  t_lh = list_first(&free_threadqueue);

  list_del(lhcurrent);
  list_del(t_lh);

  uchild=(union task_union*)list_head_to_task_struct(lhcurrent);
  uthread=(union thread_union*)list_head_to_thread_struct(t_lh);


  /* Copy the parent's task struct to child's */

  /*Current crítico*/
  copy_data(current_t(), uthread, sizeof(union thread_union));

  /* new pages dir */
  allocate_DIR((struct task_struct*)uchild);

  /* Allocate pages for DATA+STACK */
  int new_ph_pag, pag, i;
  page_table_entry *process_PT = get_PT(&uchild->task);
  for (pag=0; pag<NUM_PAG_DATA; pag++)
  {
    new_ph_pag=alloc_frame();
    if (new_ph_pag!=-1) /* One page allocated */
    {
      set_ss_pag(process_PT, PAG_LOG_INIT_DATA+pag, new_ph_pag);
    }
    else /* No more free pages left. Deallocate everything */
    {
      /* Deallocate allocated pages. Up to pag. */
      for (i=0; i<pag; i++)
      {
        free_frame(get_frame(process_PT, PAG_LOG_INIT_DATA+i));
        del_ss_pag(process_PT, PAG_LOG_INIT_DATA+i);
      }
      /* Deallocate task_struct */
      list_add_tail(lhcurrent, &freequeue);

      /* Return error */
      return -EAGAIN;
    }
  }

  /* Copy parent's SYSTEM and CODE to child. */
  page_table_entry *parent_PT = get_PT(current_p());
  for (pag=0; pag<NUM_PAG_KERNEL; pag++)
  {
    set_ss_pag(process_PT, pag, get_frame(parent_PT, pag));
  }
  for (pag=0; pag<NUM_PAG_CODE; pag++)
  {
    set_ss_pag(process_PT, PAG_LOG_INIT_CODE+pag, get_frame(parent_PT, PAG_LOG_INIT_CODE+pag));
  }
  /* Copy parent's DATA to child. We will use TOTAL_PAGES-1 as a temp logical page to map to */
  for (pag=NUM_PAG_KERNEL+NUM_PAG_CODE; pag<NUM_PAG_KERNEL+NUM_PAG_CODE+NUM_PAG_DATA; pag++)
  {
    /* Map one child page to parent's address space. */
    set_ss_pag(parent_PT, pag+NUM_PAG_DATA, get_frame(process_PT, pag));
    copy_data((void*)(pag<<12), (void*)((pag+NUM_PAG_DATA)<<12), PAGE_SIZE);
    del_ss_pag(parent_PT, pag+NUM_PAG_DATA);
  }
  /* Deny access to the child's memory space */
  set_cr3(get_DIR(current_p()));


  uchild->task.PID=++global_PID;
  uchild->task.state=ST_READY;
  uchild->task.total_quantum = DEFAULT_QUANTUM_P;
  uchild->task.nthread = 1;
  
  uthread->task.TID = (uchild->task.nthread)++;
  uthread->task.state = ST_READY;
  uthread->task.dir_pages_baseAddr = get_DIR((struct task_struct*)uchild);
  uthread->task.p = (struct task_struct*)uchild;

  //printk("Llego hasta la assignacion de mem\n");

  int register_ebp;		/* frame pointer */
  /* Map Parent's ebp to child's stack */
  register_ebp = (int) get_ebp();
  register_ebp=(register_ebp - (int)current_t()) + (int)(uthread);//Al 99 % seguro de que es thread ya que es aritmetica de pila

  uthread->task.register_esp=register_ebp + sizeof(DWord);

  DWord temp_ebp=*(DWord*)register_ebp;
  /* Prepare child stack for context switch */
  uthread->task.register_esp-=sizeof(DWord);
  *(DWord*)(uthread->task.register_esp)=(DWord)&ret_from_fork;
  uthread->task.register_esp-=sizeof(DWord);
  *(DWord*)(uthread->task.register_esp)=temp_ebp;

  //printk("Llego hasta el manejo de la pila\n");

  /* Set stats to 0 */
  //init_stats(&(uchild->task.p_stats));

  /* Queue child process into readyqueue */
  uchild->task.state=ST_READY;
  list_add_tail(&(uchild->task.list), &readyqueue);
  list_add_tail(&(uthread->task.list), &(uchild->task.ready_threads));

  //printk("Llego hasta el final\n");

  return uchild->task.PID;
}

#define TAM_BUFFER 512

int sys_write(int fd, char *buffer, int nbytes) {
char localbuffer [TAM_BUFFER];
int bytes_left;
int ret;

	if ((ret = check_fd(fd, ESCRIPTURA)))
		return ret;
	if (nbytes < 0)
		return -EINVAL;
	if (!access_ok(VERIFY_READ, buffer, nbytes))
		return -EFAULT;

	bytes_left = nbytes;
	while (bytes_left > TAM_BUFFER) {
		copy_from_user(buffer, localbuffer, TAM_BUFFER);
		ret = sys_write_console(localbuffer, TAM_BUFFER);
		bytes_left-=ret;
		buffer+=ret;
	}
	if (bytes_left > 0) {
		copy_from_user(buffer, localbuffer,bytes_left);
		ret = sys_write_console(localbuffer, bytes_left);
		bytes_left-=ret;
	}
	return (nbytes-bytes_left);
}


extern int zeos_ticks;

int sys_gettime()
{
  return zeos_ticks;
}

int sys_pthread_create(struct thread_struct* t, void *(* start_routine) (void *), void* arg) {
    if (!access_ok(VERIFY_READ, start_routine, sizeof(void*))) return -EFAULT;
    if (list_empty(&free_threadqueue)) return -ENOMEM;
    
    struct list_head *t_lh;
    union thread_union *uthread;

    t_lh = list_first(&free_threadqueue);
    uthread = (union thread_union *)list_head_to_thread_struct(t_lh);
    list_del(t_lh);

    copy_data(current_t(), uthread, (unsigned int) sizeof(union thread_union));
    
    int index  = ((int) get_ebp() - (int) current_t())/sizeof(int);
    uthread->task.register_esp = (int) &(uthread->stack[index]);
    uthread->stack[KERNEL_STACK_SIZE - 5]=(int)start_routine;
    
    uthread->task.TID = (current_p()->nthread)++;
    uthread->task.quantum = DEFAULT_QUANTUM_T;
    uthread->task.state = ST_READY;
    t = (struct thread_struct*)uthread;
    list_add_tail(&(uthread->task.list), &(current_p()->ready_threads));
    return 0;
}





void sys_exit()
{
  int i;

  page_table_entry *process_PT = get_PT(current_p());

  // Deallocate all the propietary physical pages
  for (i=0; i<NUM_PAG_DATA; i++)
  {
    free_frame(get_frame(process_PT, PAG_LOG_INIT_DATA+i));
    del_ss_pag(process_PT, PAG_LOG_INIT_DATA+i);
  }

  /* Free task_struct */
  list_add_tail(&(current_p()->list), &freequeue);

  current_p()->PID=-1;

  while (!list_empty(&(current_p()->ready_threads))) {
    struct thread_struct *t;
    struct list_head *l;
    l = list_first(&(current_p()->ready_threads));
    t = list_head_to_thread_struct(l);
    list_del(l);

    list_add_tail(&(t->list), &free_threadqueue);

    t->TID = -1;
  }

  while (!list_empty(&(current_p()->ready_threads))) {
    struct thread_struct *t;
    struct list_head *l;
    l = list_first(&(current_p()->ready_threads));
    t = list_head_to_thread_struct(l);
    list_del(l);

    list_add_tail(&(t->list), &free_threadqueue);

    t->TID = -1;
  }


  /* Restarts execution of the next process */
  sched_next_process_rr();
}

/* System call to force a task switch */
int sys_yield()
{
  force_task_switch();
  return 0;
}

int sys_pthread_join(struct thread_struct *thread, void **value_ptr) 
{
  //recorrer vector de threads buscando si existe
  if (current_p()->nthread < 2) return -1; //TODO buscar el error que toca
  current_t()->blocked_by = thread->TID;
  update_thread_state_rr(current_t(), &(current_p()->blocked_threads));
  sched_next_thread_rr();
  //descubrir que hace value_ptr
  return 0;
}

int sys_pthread_exit(void *value_ptr) {

  /*page_table_entry *thread_PT = get_PT_thread(current_t());

  for (i=0; i<NUM_PAG_DATA; ++i)
  {
    del_ss_pag(thread_PT, PAG_LOG_INIT_DATA+i);
  }*/

  list_add_tail(&(current_t()->list), &free_threadqueue);

  current_t()->TID = -1;

  sched_next_thread_rr();

  //TODO Usar value_ptr o hacer una funcion para controlar los blockeds que se han hecho con el join

  return 0;
}
