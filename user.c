#include <libc.h>

char buff[24];

char *a = "asdf\n";

int pid;

int addASM(int, int);

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

	int local = addASM(0x43, 0x666);
	int aux = write(1, a, strlen(a));
	if (aux < 0) {
		write(1, "error\n", 6);
		itoa(errno, buff);
		write(1, buff, strlen(buff));
	}
	else {
		itoa(aux, buff);
		write(1, buff, strlen(buff));		
	}
	
	int i = 0; 	
	
	while (1) {
		if (i % 1000 == 0) {
			int f = gettime();
			itoa(f, buff);
			write(1, buff, strlen(buff));
		}
		++i;
	}
  while(1) { }
}
