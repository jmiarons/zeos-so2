#include <libc.h>

char buff[24];

int pid;

char* b = "Hola, soy init\n";
char* a = "Hola, no soy init\n";
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

void prueba1() { /*Crear un thread y ejecutar un pthread_exit*/
    int id;
    pthread_create(&id, &hola, NULL);
}

void prueba2() { /*Crear un thread y ver como el planificador de nivel 1 funciona*/
    int id;
    pthread_create(&id, &adeu, NULL);
    while(1) {
        write(1, e, strlen(e));
    }
}

void prueba3() { /*Crear un thread y ver como el main thread de init se bloquea hasta que el nuevo thread acaba de ejecutarse*/
    int id;
    pthread_create(&id, &hola, NULL);
    pthread_join(&id, NULL);
    write(1, e, strlen(e));
}

void prueba4() { /*ver como el planificador de 2 niveles funciona*/
    pid = fork();
    if (pid == 0) {
        while (1) {
            write (1, a, strlen(a));
        }
    }
    if (pid > 0) {
        while (1) {
            write(1, b, strlen(b));
        }
    }
}

void prueba5() { /*Planificador completo*/
    pid = fork();
    if (pid == 0) {
        while (1) {
            write (1, a, strlen(a));
        }
    }
    if (pid > 0) {
        int id;
        pthread_create(&id, &adeu, NULL);
        while (1) {
            write(1, b, strlen(b));
        }
    }
}


int __attribute__ ((__section__(".text.main")))
main(void) {
    //prueba1();
    //prueba2();
    //prueba3();
    //prueba4();
    //prueba5();
    while(1) { 
    }
}
