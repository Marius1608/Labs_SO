#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_THREADS 32

typedef struct {
    int from;
    int to;
    int result;
} TH_STRUCT;

void *Thread_FN(void *param) {
    TH_STRUCT *s = (TH_STRUCT *)param;
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
    int n = 1000000; 
    int nr_threads;
    double exec_times[MAX_THREADS];
    clock_t start, end;
    FILE *fptr;
    
    fptr = fopen("exec_times.txt", "w");
    if (fptr == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    
    for (nr_threads = 1; nr_threads <= MAX_THREADS; nr_threads++) {

        pthread_t tid[nr_threads];
        TH_STRUCT params[nr_threads];
        int interval = n / nr_threads;
        int remainder = n % nr_threads;
        int from = 1;
        int to = interval;
        int nr1_total = 0;

        start = clock();
        for (int j = 0; j < nr_threads; j++) {
            params[j].from = from;
            params[j].to = to + (remainder > 0 ? 1 : 0); 
            from = to + (remainder > 0 ? 1 : 0) + 1; 
            to += interval;
            if (remainder > 0) {
                remainder--;
            }
            pthread_create(&tid[j], NULL, Thread_FN, (void *)&params[j]);
        }

        for (int j = 0; j < nr_threads; j++) {
            pthread_join(tid[j], NULL);
            nr1_total += params[j].result;
        }
        end = clock();
        exec_times[nr_threads - 1] = ((double)(end - start)) / CLOCKS_PER_SEC;
        fprintf(fptr, "Numar thread-uri: %d, Timp de executie: %lf secunde\n", nr_threads, exec_times[nr_threads - 1]);
        printf("Nr de '1' de la 1 la %d este: %d\n", n, nr1_total);
    }
    
    fclose(fptr);
    return 0;
}
