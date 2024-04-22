#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define THREADS 5 

typedef struct {
    int from;
    int to;
    int result;
} TH_STRUCT;

void *Thread_FN(void * param) {

    TH_STRUCT *s = (TH_STRUCT *) param;
    int nr1 = 0;
    for (int i = s->from; i <= s->to; i++) {
        int cpy_nr = i;
        while (cpy_nr > 0) {
            if (cpy_nr % 10 == 1) {
                nr1++;
            }
            cpy_nr /= 10;
        }
    }
    s->result = nr1;
    pthread_exit(NULL);
}

int main() {

    pthread_t tid[THREADS];
    TH_STRUCT params[THREADS];
    int n = 5000;   
    int interval = n / THREADS;
    int from = 1;
    int to = interval;
    int nr1_total = 0;

    for (int j = 0; j < THREADS; j++) {
        params[j].from = from;
        params[j].to = to;
        pthread_create(&tid[j], NULL, Thread_FN, (void *)&params[j]);
        from = to + 1;
        to += interval;

        //penultimul fir de executie(pentru a pierde vreun numar din interval)
        if (j == THREADS - 2) {
            to = n; 
        }
    }


    for (int j = 0; j < THREADS; j++) {
        pthread_join(tid[j], NULL);
        nr1_total += params[j].result;
    }

 
    printf("Nr de '1' de la 1 la %d este: %d\n", n, nr1_total);
    pthread_exit(NULL);
    return 0;
}
