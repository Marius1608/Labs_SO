#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

sem_t sem[3];
void *th_smoker(void *arg)
{
        int ind = (int)(size_t)arg;
        if(ind == 0) {
                for (int i = 0; i >= 0; ++i) {
                        sem_wait(sem + 1);
                        if(sem_trywait(sem + 2) == 0) {
                                printf("0 got 1 and 2\n");
                                fflush(stdout);
                        } else {
                                sem_post(sem + 1);
                        }
                        usleep((random() % 20));
                }
        } else if(ind == 1) {
                for (int i = 0; i >= 0; ++i) {
                        sem_wait(sem + 0);
                        if(sem_trywait(sem + 2) == 0) {
                                printf("1 got 0 and 2\n");
                                fflush(stdout);
                        } else {
                                sem_post(sem + 0);
                        }
                        usleep((random() % 20));
                }
        } else {
                for (int i = 0; i >= 0; ++i) {
                        sem_wait(sem + 0);
                        if(sem_trywait(sem + 1) == 0) {
                                printf("2 got 0 and 1\n");
                                fflush(stdout);
                        } else {
                                sem_post(sem + 0);
                        }
                        usleep((random() % 20));

                }
        }
        return NULL;
}

void *th_dealer(void *arg)
{
        long i1, i2;
        for(int i = 0; i >= 0; ++i) {
                usleep( 3000 + (random() % 20));
                i1 = random() % 3;
                while((i2 = random() % 3) == i1);
                printf("Dealer is giving: %ld %ld\n", i1, i2);
                fflush(stdout);
                sem_post(sem + i1);
                sem_post(sem + i2);
        }
        return NULL;
}

int main()
{
        int i;
        pthread_t tidC[3], dealer;
        srandom(time(NULL));
        for(i = 0; i < 3; ++i) {
                sem_init(sem + i, 0, 1);
                sem_wait(sem + i);
        }

        pthread_create(&dealer, NULL, th_dealer, NULL);
        for(i = 0; i < 3; i++) {
                pthread_create(&tidC[i], NULL, th_smoker, (void*)(size_t)(i));
        }

        scanf("%*c");
        for(i = 0; i < 3; i++) {
                pthread_cancel(tidC[i]);
                pthread_join(tidC[i], NULL);
        }
        pthread_cancel(dealer);
        pthread_join(dealer, NULL);

        printf("Finished\n");
        for(i = 0; i < 3; ++i)
                sem_destroy(sem + i);
        return 0;
}