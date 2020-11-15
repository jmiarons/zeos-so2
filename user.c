#include <libc.h>
#include <stats.h>

char buff[24];

char *a = "Hola soy el hijo\n";
char *b = "Hola soy el padre\n";
int pid;
struct stats info;

int addASM(int, int);

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

     int p1 = fork();
     if (p1 == 0) {
       while (1) {
         write(1, a, strlen(a));
       }
     }
     else if (p1 > 0) {
       while (1) {
         //get_stats(p1, &info); 
         write(1, b, strlen(b));
       }
     }

  while(1) { }
}
