



pthread_create(struct task_union* t, void *(* start_routine) (void *), void* arg) {
  if (!access_ok(VERIFY_READ, start_routine, sizeof(void*))) return -EFAULT;

  copy_data(current(), t, (unsigned int) sizeof(union task_union));

  int reg_ebp = (int) get_ebp();
  reg_ebp = (reg_ebp - (int) current()) + (int)(t);

  t->task.register_esp = reg_ebp + sizeof(DWord);
  DWord temp_ebp=*(DWord*)reg_ebp;




  t->task.state = ST_READY;
  list_add_tail(&(t->task.list), &readyqueue);
  return 0;
}
