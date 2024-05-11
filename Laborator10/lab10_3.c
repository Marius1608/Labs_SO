#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define NR_SEATS 25

sem_t table; //daca masa e completa 
sem_t seat; //pentru locuri 
int occupied = 0;

void *customer(void *arg)
{
    int myId = (int)(ssize_t)arg;
    usleep(1000 * (rand() % 20)); 

    printf("[C%02d] Arrived at the restaurant.\n", myId);

    sem_wait(&seat); 
    if (occupied < NR_SEATS) {
        occupied++;
        if (occupied == NR_SEATS) {
            sem_post(&table); 
        }
        sem_post(&seat); 
        printf("[C%02d] Seated at the table.\n", myId);
    } else {
        sem_post(&seat); 
        sem_wait(&table); 
        sem_wait(&seat); 
        occupied = 1; 
        sem_post(&seat); 
        printf("[C%02d] Seated at the table.\n", myId);
    }

    usleep(1000 * (rand() % 20)); 

    sem_wait(&seat); 
    occupied--; 
    if (occupied == 0) {
        sem_post(&table); 
    }
    sem_post(&seat); 

    printf("[C%02d] Left the restaurant.\n", myId);

    return NULL;
}

int main()
{
    int i;
    pthread_t tid[NR_SEATS];

    srand(time(NULL));

    sem_init(&table, 0, 1); 
    sem_init(&seat, 0, 1); 

    for(i = 0; i < NR_SEATS; i++) {
        pthread_create(&tid[i], NULL, customer, (void*)(ssize_t)(i + 1));
    }

    for(i = 0; i < NR_SEATS; i++) {
        pthread_join(tid[i], NULL);
    }

    sem_destroy(&table);
    sem_destroy(&seat);

    return 0;
}
