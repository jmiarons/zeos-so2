#include <libc.h>

char buff[24];

int pid;

char* b = "Hola, soy init y tengo un pid = ";
char* a = "Hola, no soy init y tengo un pid = ";
char* c = "Hola\n";
char* d = "Adeu\n";
char* n = "\n";

void hola() {
    write(1, c, strlen(c));
    while(1);
    pthread_exit(NULL);
}

void adeu() {
    write(1, d, strlen(d));
    pthread_exit(NULL);
}



int __attribute__ ((__section__(".text.main")))
main(void) {
    struct thread_struct* t = NULL;
    struct thread_struct* t1 = NULL;
    pid = fork(); //Comprobar que el planificador de 1r nivel funciona*/
    if (pid == 0) {
        write(1, a, strlen(a));
        int aux2 = getpid();
        itoa(aux2, buff);
        write(1, buff, strlen(buff)); write(1, n, 1);
        pthread_create(t, &hola, NULL); /*Comprobar que el planificador de segundo nivel funciona, además del pthread_create*/
    }
    else if (pid > 0) {
        write(1, b, strlen(b));
        int aux = getpid();
        itoa(aux, buff);
        write(1, buff, strlen(buff)); write(1, n, 1);
        //pthread_create(t1, &adeu, NULL); /*Comprobar que se crea un thread y se destruye adecuadamente*/
    }
    while(1) { }
}
