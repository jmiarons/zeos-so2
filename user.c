#include <libc.h>

char buff[24];

int pid;

char* b = "Hola soy el padre y tengo un pid = \n";
char* a = "Hola soy un thread\n";

int __attribute__ ((__section__(".text.main")))
main(void) {
  write(1, b, strlen(b));
  while(1) { }
}
