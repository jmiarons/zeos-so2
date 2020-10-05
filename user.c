#include <libc.h>

char buff[24];

char *a;

int pid;

int addASM(int, int);

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

	int local = addASM(0x43, 0x666);

	for(int i = 0; i < 10000; i +=10) {
	int aux = gettime();
	itoa(aux, buff);
	write(1, buff, strlen(buff));
	}
	
  while(1) { }
}
