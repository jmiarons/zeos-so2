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
    write(1, c, strlen(c));
    pthread_exit(NULL);
}

void adeu() {
    while (1) {
        write(1, d, strlen(d));
    }
    pthread_exit(NULL);
}



int __attribute__ ((__section__(".text.main")))
main(void) {
    struct thread_struct* t = NULL;
    struct thread_struct* t1 = NULL;
    
    pthread_create(t, &hola, NULL);
    //pthread_create(t1, &adeu, NULL);

    pthread_join(t, NULL);

    while(1) { 
        write(1, e, strlen(e));
    }
}
