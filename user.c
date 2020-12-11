#include <libc.h>

char buff[24];

int pid;

char* b = "Hola soy init y tengo un pid = ";
char* a = "Hola soy otro proceso y tengo un pid = ";
char* c = "Hola\n";
char* n = "\n";
void hola() {
    write(1, c, strlen(c));
    pthread_exit(NULL);
}


int __attribute__ ((__section__(".text.main")))
main(void) {
    struct thread_struct* t;
    pid = fork();
    if (pid == 0) {
        write(1, a, strlen(a));
        int aux2 = getpid();
        itoa(aux2, buff);
        write(1, buff, strlen(buff)); write(1, n, 1);
        pthread_create(t, &hola, NULL);
        while (1) {
            write(1, "efgh\n", 5);
        }
    }
    else if (pid > 0) {
        write(1, b, strlen(b));
        int aux = getpid();
        itoa(aux, buff);
        write(1, buff, strlen(buff)); write(1, n, 1);
        while (1) {
            write(1, "abcd\n", 5);
        }
    }
    while(1) { }
}
