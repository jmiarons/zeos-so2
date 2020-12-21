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

void mutex_test_1() {
    mutex_lock(3);
    write(1,e,strlen(e));
    yield();
    mutex_unlock(3);
    pthread_exit(NULL);
}


void mutex_test_2() {
    mutex_lock(3);
    write(1,c,strlen(c));
    mutex_unlock(3);
    pthread_exit(NULL);
}

void mutex_test_3() {
    mutex_lock(5);
    write(1,d,strlen(d));
    yield();
    mutex_unlock(5);
    while(1) 
      write(1,d,strlen(d));
    pthread_exit(NULL);
}

void mutex_test_4() {
    mutex_lock(5);
    write(1,c,strlen(c));
    mutex_unlock(5);
    while(1) 
      write(1,c,strlen(c));
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

void prueba6() { /*Mutex bloquea y desbloquea y acaban ambos*/
    int id;
    int id2;
    mutex_init(3);
    pthread_create(&id, &mutex_test_1, NULL);
    pthread_create(&id2, &mutex_test_2, NULL);
    mutex_destroy(3);
}

void prueba7() { /*Mutex bloquea y desbloquea y siguen ejecutandose ambos threads correctamente*/
    int id;
    int id2;
    mutex_init(5);
    pthread_create(&id, &mutex_test_3, NULL);
    pthread_create(&id2, &mutex_test_4, NULL);
    mutex_destroy(5);
}

int __attribute__ ((__section__(".text.main")))
main(void) {
    //prueba1();
    //prueba2();
    //prueba3();
    //prueba4();
    //prueba5();
    //prueba6();
    //prueba7();
    while(1) { 
    }
}
