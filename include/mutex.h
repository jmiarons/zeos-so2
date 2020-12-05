


struct mutex_t {
  int PID;
  int TID;  /*ID*/
  int locked; /*Variable booleana*/
  struct list_head blocked; /*Cola de bloqueados*/
}
