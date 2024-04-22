#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define THREADS 10000

void *Thread_Fn(void *arg) {
    sleep(60);
}

int main() {
    
    int counter= 0;
    pthread_t tid[THREADS];
    
    while (counter < THREADS) {
        int ok = pthread_create(&tid[counter], NULL, Thread_Fn, NULL);
        if (ok != 0) {
            break;
        }
        counter++;
    }
    
    printf("Nr==%d\n", counter);
    for (int i = 0; i < counter; i++) {
        pthread_join(tid[i], NULL);
    }
    
    return 0;
}
