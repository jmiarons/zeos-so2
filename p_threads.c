



int pthread_create(struct thread_struct* t, void *(* start_routine) (void *), void* arg) {
  if (!access_ok(VERIFY_READ, start_routine, sizeof(void*))) return -EFAULT;

  union thread_union* ut = (union thread_union*) t;

  copy_data(current_t(), ut, (unsigned int) sizeof(union thread_union));

  int register_ebp = (int) get_ebp();
  register_ebp = (register_ebp - (int)current_t()) + (int)(ut);

  ut->task.register_esp = register_ebp + sizeof(DWord);
  DWord temp_ebp=*(DWord*)reg_ebp;
  ut>task.register_esp-=sizeof(DWord);
  *(DWord*)(ut->task.register_esp)=(DWord)start_routine;
  ut->task.register_esp-=sizeof(DWord);
  *(DWord*)(ut->task.register_esp)=temp_ebp;

  t->TID = 2; //Numero random;
  t->state = ST_READY;
  list_add_tail(&(t->list), &(current()->ready_threads));
  return 0;
}

pthread_exit(void* statuts) {
  ;
}
