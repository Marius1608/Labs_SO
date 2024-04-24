#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 75 

int *v; 
int *nrDiv; 
int n,nr_threads; 

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 


int calc(int nr) {

    int counter = 0;
    for (int i = 1; i <= nr; i++) {
        if (nr % i == 0) {
            counter++;
        }
    }
    return counter;
}


void *calcNr(void *arg) {

    int tids = *(int *)arg;
    int start = tids * (n / nr_threads);
    int end;

    if (tids == nr_threads - 1) {
        end = n;
    } else {
        end = (tids + 1) * (n / nr_threads);
     }


    for (int i = start; i < end; i++) {
        nrDiv[i] = calc(v[i]);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    nr_threads = atoi(argv[1]);

    if (nr_threads <= 0 || nr_threads > THREADS) {
        printf("Invalid number of threads\n");
        return -1;
    }

    
    n = 100;
    v = (int *)malloc(n * sizeof(int));
    nrDiv = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        v[i] = i + 1;
    }

   

    pthread_t threads[nr_threads];
    int tids[nr_threads];
    for (int i = 0; i < nr_threads; i++) {
        tids[i] = i;
    }

    
    for (int i = 0; i < nr_threads; i++) {
        pthread_create(&threads[i], NULL, calcNr, &tids[i]);
    }

  
    for (int i = 0; i < nr_threads; i++) {
        pthread_join(threads[i], NULL);
    }

   
    printf("Numarul de divizori:\n");
    for (int i = 0; i < n; i++) {
        printf("%d: %d\n", v[i], nrDiv[i]);
    }

    
    free(v);
    free(nrDiv);

    return 0;
}
