#include <libc.h>

char buff[24];

int pid;

char* b = "Hola soy el padre y tengo un pid = \n";
char* a = "Hola soy un thread\n";

int __attribute__ ((__section__(".text.main")))
main(void) {
  pid = fork();
  if (pid == 0) {
    write(1, a, strlen(a));
    int aux2 = getpid();
    itoa(aux2, buff);
    write(1, buff, strlen(buff));
    while (1) {
        write(1, "padre\n", strlen("padre\n"));
    }
  }
  else if (pid > 0) {
    write(1, b, strlen(b));
    int aux = getpid();
    itoa(aux, buff);
    write(1, buff, strlen(buff));
    while (1) {
        write(1, "hijo\n", strlen("hijo\n"));
    }
  }
  while(1) { }
}
