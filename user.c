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
    
    int id;
    pthread_create(&id, &hola, NULL);
    
    itoa(id, buff);
    write(1, buff, strlen(buff));


    pthread_join(&id, NULL);

    write(1, e, strlen(e));
    while(1) { 
    }
}
