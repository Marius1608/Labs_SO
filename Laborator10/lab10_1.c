#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define NR_CUSTOMERS 20
#define NR_CHAIRS 3

sem_t lock;
sem_t customer;
sem_t barber;
//sem_t served;
int waitingCustomers = 0;
int barberChair = 0; 
int servedCustomers = 0;


void *th_barber(void *arg)
{
    for(;;) {
        sem_post(&customer);
        sem_wait(&barber);
        printf("[B] Serving customer %d\n", barberChair);
        usleep(1000);
        servedCustomers++;
        //sem_post(&served);
        if (servedCustomers == NR_CUSTOMERS) {
            printf("[B  ] All customers served. Barber is going home.\n");
            pthread_exit(NULL);
        }
    }
    return NULL;
}

void *th_customer(void *arg)
{
    int myId = (int)(ssize_t)arg;
    int tooBusy = 0;

    usleep(1000 * (rand() % 20));
    printf("[C%02d] Entering the barber shop\n", myId);
    
    sem_wait(&lock);
    if(waitingCustomers < NR_CHAIRS) {
        ++waitingCustomers;
        printf("[C%02d] %d customer(s) waiting\n", myId, waitingCustomers);
    } else {
        tooBusy = 1;
        printf("[C%02d] Too busy, will come back another day.\n", myId);
    }
    sem_post(&lock);

    if(!tooBusy) {
        sem_wait(&customer);
        //sem_wait(&served);

        sem_wait(&lock);
        --waitingCustomers;
        sem_post(&lock);

        barberChair = myId;
        sem_post(&barber);
        printf("[C%02d] being served\n", myId);
        usleep(1000);
    }

    return NULL;
}

int main()
{
    int i;
    pthread_t tidC[NR_CUSTOMERS], tidB;
    srand(time(NULL));

    sem_init(&lock, 0, 1);
    sem_init(&customer, 0, 0);
    sem_init(&barber, 0, 0);
    //sem_init(&served, 0, 0);

    pthread_create(&tidB, NULL, th_barber, NULL);
    for(i=0; i<NR_CUSTOMERS; i++) {
        pthread_create(&tidC[i], NULL, th_customer, (void*)(ssize_t)(i+1));
    }
    for(i=0; i<NR_CUSTOMERS; i++) {
        pthread_join(tidC[i], NULL);
    }
    pthread_join(tidB, NULL);

    sem_destroy(&lock);
    sem_destroy(&customer);
    sem_destroy(&barber);
    //sem_destroy(&served);

    return 0;
}
