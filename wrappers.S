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

ENTRY(gettime)
	/*link Dinamico*/
	pushl %ebp;
	movl %esp, %ebp;

	/*ID*/
	movl $10, %eax;

	/*Salvamos los regs que machacamos*/
	pushl %ecx;
	pushl %edx;

	/*@ de retorno*/
	leal time_ret, %edi;
	pushl %edi;

	/*Fake dynamic link*/
	pushl %ebp
	movl %esp, %ebp;


	sysenter

time_ret:

	movl %ebp, %esp;
	popl %ebp;

	cmpl $0, %eax;
	jge end_time;
	negl %eax;
	movl %eax, errno;
	movl $-1, %eax;

end_time:
	popl %edi;
	popl %edx;
	popl %ecx;
	movl %ebp, %esp;
	popl %ebp;
	ret;

ENTRY(getpid)
	/*link Dinamico*/
	pushl %ebp;
	movl %esp, %ebp;

	/*ID*/
	movl $20, %eax

	/*Salvamos los regs que machacamos*/
	pushl %ecx;
	pushl %edx;

	/*@ de retorno*/
	leal getpid_ret, %edi;
	pushl %edi;

	/*Fake dynamic link*/
	pushl %ebp;
	movl %esp, %ebp;

	sysenter;

getpid_ret:
	popl %edi;
	popl %edx;
	popl %ecx;
	movl %ebp, %esp;
	popl %ebp;
	ret;
