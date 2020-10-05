#include <asm.h>

ENTRY(write)
	/*link Dinamico*/
	pushl %ebp;
	movl %esp, %ebp;	
	
	/*ID*/	
	movl $4, %eax;

	/*Salvamos los regs que machacamos*/
	pushl %ecx;
	pushl %edx;

	/*Paso de parametros*/	
	movl 8(%ebp), %ebx;
	movl 12(%ebp), %ecx;
	movl 16(%ebp), %edx;
	
	/*@ de retorno*/
	leal dir_ret, %edi;
	pushl %edi;
	
	/*Fake dynamic link*/
	pushl %ebp
	movl %esp, %ebp;
	
	sysenter

dir_ret:
	movl %ebp, %esp;
	popl %ebp;

	cmpl $0, %eax;
	jge end_write;
	negl %eax;	
	movl %eax, errno;
	movl $-1, %eax; 
	
end_write:
	popl %edi;
	popl %edx;
	popl %ecx;
	movl %ebp, %esp;
	popl %ebp;
	ret;
