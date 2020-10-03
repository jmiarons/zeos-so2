#include <asm.h>

ENTRY(write)
	/*link Dinamico*/
	pushl %ebp;
	movl %esp, %ebp;	
	
	/*Paso de parametros*/	
	movl 8(%ebp), %ebx;
	movl 12(%ebp), %ecx;
	movl 16(%ebp), %edx;
	
    pushl %edx;
    pushl %ecx;

	/*@ de retorno*/
	leal dir_ret, %edi;
	pushl %edi;
	
	/*ID*/	
	movl $4, %eax;
	
	/*Fake dynamic link*/
	pushl %ebp
	movl %esp, %ebp;
	
	sysenter

dir_ret:
	
    popl %ebp;
    popl %edi;
    popl %ecx;
    popl %edx;


    cmpl $0, %eax;
    jge end;
    negl %eax;
    movl %eax, %eax; //mover a errno
   
end:
    movl %ebp, %esp;
    popl %ebp;
    ret; 
		
