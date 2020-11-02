/*
 * entry.h - Definició del punt d'entrada de les crides al sistema
 */

#ifndef __ENTRY_H__
#define __ENTRY_H__

void clock_handler();

void writeMSR(int reg, int val);

void task_switch(union task_union *new);

void syscall_handler_sysenter();

#endif  /* __ENTRY_H__ */
