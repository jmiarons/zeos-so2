#include <libc.h>

char buff[24];

int pid;

char* b = "Hola, soy init y tengo un pid = ";
char* a = "Hola, no soy init y tengo un pid = ";
char* c = "Hola\n";
char* d = "Adeu\n";
char* e = "Hey\n";
char* n = "\n";

void hola() {
    while (1);
    pthread_exit(NULL);
}

void adeu() {
    write(1, d, strlen(d));
    while(1);
    pthread_exit(NULL);
}



int __attribute__ ((__section__(".text.main")))
main(void) {
    struct thread_struct* t = NULL;
    pthread_create(t, &hola, NULL);


    while(1) { 
        //write(1, d, strlen(d));
    }
}
