#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define N 4
#define M 1000

long count = 0;
sem_t block;

void *thread_function(void *unused) {
    //long aux;
    for (int i = 0; i < M; i++) {
        
        //Prin blocarea semaforului înainte de a accesa variabila count 
        //și eliberarea semaforului după actualizarea acesteia, 
        //putem asigura că fiecare thread accesează count în mod sigur, fără a interfere cu alte thread-uri.

        sem_wait(&block);
        //aux=count;
        count++;
        //aux++;
        sem_post(&block);
        usleep(random() % 10);
        //count=aux;
    }
    return NULL;
}

int main() {

    pthread_t threads[N];
    sem_init(&block, 0, 1);

    for (int i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, thread_function, NULL);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Count : %ld\n", count);
    sem_destroy(&block);

    return 0;
}
