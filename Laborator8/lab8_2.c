#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define THREADS 16

int nrThreads = 0;
sem_t block;
sem_t limited_area;

//Semaforul block este folosit pentru a asigura accesul exclusiv la variabila nrThreads.
//Semaforul limited_area este folosit pentru a limita numărul maxim de fire de execuție care pot intra în zona limitată.

void *limited_area_function(void *unused) {

    sem_wait(&limited_area);

    sem_wait(&block);
    printf("The number of threads in the limited area is: %d\n", nrThreads);
    nrThreads++;
    sem_post(&block);

    usleep(1000);
    
    
    sem_wait(&block);
    printf("The number of threads in the limited area is: %d\n", nrThreads);
    nrThreads--;
    sem_post(&block);

    sem_post(&limited_area);
    
    return NULL;
}

int main(int argc, char *argv[]) {

    int max_threads = 16; 
    pthread_t threads[THREADS];

    sem_init(&block, 0, 1);
    sem_init(&limited_area, 0, max_threads);

    for (int i = 0; i < THREADS; i++) {
        pthread_create(&threads[i], NULL, limited_area_function, NULL);
    }

    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&block);
    sem_destroy(&limited_area);

    return 0;
}
