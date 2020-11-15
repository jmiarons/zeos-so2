/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>

#include <utils.h>

#include <io.h>

#include <mm.h>

#include <mm_address.h>

#include <sched.h>

#include <errno.h>

#include <system.h>

#define LECTURA 0
#define ESCRIPTURA 1


int check_fd(int fd, int permissions)
{
  if (fd!=1) return -9; /*EBADF*/
  if (permissions!=ESCRIPTURA) return -13; /*EACCES*/
  return 0;
}

int sys_ni_syscall()
{
	return -38; /*ENOSYS*/
}

int sys_getpid()
{
	return current()->PID;
}

int ret_from_fork() {
  return 0;
}

int sys_fork() {
  int PID = -1;
  /*PCB y tp del proceso padre*/
  struct task_struct* father_task_s = current();
  union task_union* father_task_u = (union task_union *) father_task_s;
  page_table_entry* father_pt = get_PT(father_task_s);

  /*Si no hay espacio devolvemos un error*/
  if (list_empty(&freequeue)) return -EAGAIN;

  /*PCB nuevo para el proceso hijo*/
  struct list_head* first = list_first(&freequeue);
  list_del(first);
  struct task_struct* task_s = list_head_to_task_struct(first);
  union task_union* task_u = (union task_union *) task_s;

  /*Copiamos el contexto del padre al hijo*/
  copy_data((void *) father_task_u,(void *) task_u, sizeof(union task_union));

  /*Asignamos memoria al hijo*/
  int frames_assigned[NUM_PAG_DATA];
  int i;
  for (i = 0; i < NUM_PAG_DATA; ++i) {
    frames_assigned[i] = alloc_frame();
    if (frames_assigned[i] == -1) {
      free(i, frames_assigned);
      return -ENOMEM;
    }
  }

  allocate_DIR(task_s);
  page_table_entry* pt = get_PT(task_s);

  for (i = 0; i < NUM_PAG_CODE; ++i) {
    pt[PAG_LOG_INIT_CODE + i].entry = father_pt[PAG_LOG_INIT_CODE +i].entry;
  }

  for (i = 0; i < NUM_PAG_DATA; ++i) {
    set_ss_pag(father_pt, PAG_LOG_INIT_DATA+NUM_PAG_DATA, frames_assigned[i]);
    set_ss_pag(pt, PAG_LOG_INIT_DATA + i, frames_assigned[i]);
    copy_data((int *)((PAG_LOG_INIT_DATA + i) << 12), (int *) ((PAG_LOG_INIT_DATA + NUM_PAG_DATA) << 12),PAGE_SIZE);
    del_ss_pag(father_pt, PAG_LOG_INIT_DATA + NUM_PAG_DATA);
    set_cr3(get_DIR(father_task_s)); //flush TLB
  }
  task_u->task.PID = next_pid;
  PID = task_u->task.PID;
  ++next_pid;
  task_u->task.state = ST_READY;

  task_u->stack[KERNEL_STACK_SIZE-18] = (unsigned long)&ret_from_fork;
  task_u->stack[KERNEL_STACK_SIZE-19] = 0; //fake ebp
  task_u->task.kernel_esp = (unsigned long *) &task_u->stack[KERNEL_STACK_SIZE - 19];

  list_add_tail(first, &readyqueue);
  return PID;
}

void sys_exit() {
  struct task_struct * t = current();
  page_table_entry * pgt = get_PT(t);

  for (int i = 0; i < NUM_PAG_DATA; ++i) {
    free_frame(get_frame(pgt, PAG_LOG_INIT_DATA+i));
    del_ss_pag(pgt, PAG_LOG_INIT_DATA+i);
  }
  t -> PID = -1;
  set_cr3(get_DIR(t));
  update_process_state_rr(t, &freequeue);
  sched_next_rr();

}

int sys_write(int fd, char * buffer, int size) {
	int ok = check_fd(fd, ESCRIPTURA);
	if (ok < 0) return -EBADFD;
	if (buffer == NULL) return -EFAULT;
	if (size < 0) return -EINVAL;

	char buff[64];
	int num_b = 0;
	while (size > 64) {
		copy_from_user(buffer, buff, 64);

		num_b += sys_write_console(buff, 64);

		buffer += 64;
		size -= 64;
	}

	copy_from_user(buffer, buff, size);
	num_b += sys_write_console(buff, size);
	size = 0;
	return num_b;
}

int sys_gettime() {
	return zeos_ticks;
}
