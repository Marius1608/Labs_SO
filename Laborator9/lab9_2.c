#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define NR_THREADS 7
#define AMOUNT_DEPOSIT 11
#define AMOUNT_WITHDRAW 7

int balance=50;
int turn = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *thread_withdraw(void *arg)
{
    int id = *((int *) arg);
    //usleep(10000 * id);

    pthread_mutex_lock(&lock);
    while (turn != id) {
        printf("[Th%d] Waiting for my turn...\n", id);
        pthread_cond_wait(&cond, &lock);
    }
    
    if (turn == id) {
        if (balance < AMOUNT_WITHDRAW) {
            printf("[Th%d] Not enough money (%d). Will wait...\n", id, balance);
            pthread_mutex_unlock(&lock);
            return NULL;
        }
        balance -= AMOUNT_WITHDRAW;
        printf("[Th%d] Withdrawn %d. Current balance is %d.\n", id, AMOUNT_WITHDRAW, balance);
        turn = (turn + 1) % NR_THREADS;
    }
    pthread_cond_broadcast(&cond); 
    pthread_mutex_unlock(&lock);

    return NULL;
}

void *thread_deposit(void *arg)
{
    int id = *((int *) arg);
    //usleep(10000 * id);

    pthread_mutex_lock(&lock);
    while (turn != id) {
        printf("[Th%d] Waiting for my turn...\n", id);
        pthread_cond_wait(&cond, &lock);
    }
    
    if (turn == id) {
        balance += AMOUNT_DEPOSIT;
        printf("[Th%d] Deposited %d. Current balance is %d.\n", id, AMOUNT_DEPOSIT, balance);
        turn = (turn + 1) % NR_THREADS; 
    }
    pthread_cond_broadcast(&cond); 
    pthread_mutex_unlock(&lock);

    return NULL;
}

int main()
{
    int i;
    pthread_t tids[NR_THREADS];
    int thread_ids[NR_THREADS];
    
    for (i = 0; i < NR_THREADS; i++) {
        thread_ids[i] = i;
        if (i == 0 || i == 2 || i == 3 || i == 4) {
            pthread_create(&tids[i], NULL, thread_withdraw, &thread_ids[i]);
        } else {
            pthread_create(&tids[i], NULL, thread_deposit, &thread_ids[i]);
        }
    }
    for (i = 0; i < NR_THREADS; i++) {
        pthread_join(tids[i], NULL);
    }

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&lock);

    return 0;
}
