#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/wait.h>

int main()
{
    int *shared_var;
    sem_t *sem_parent, *sem_child;

    shared_var = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_parent = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_child = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (shared_var == MAP_FAILED || sem_parent == MAP_FAILED || sem_child == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    *shared_var = 0;
    sem_init(sem_parent, 1, 1); 
    sem_init(sem_child, 1, 0);  

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        for (int i = 0; i < 10; i++) {
            sem_wait(sem_child);
            (*shared_var)++;
            printf("Copil:%d\n", *shared_var);
            sem_post(sem_parent);
        }
    } else {
        for (int i = 0; i < 10; i++) {
            sem_wait(sem_parent);
            (*shared_var)++;
            printf("Parinte:%d\n", *shared_var);
            sem_post(sem_child);
        }
        wait(NULL);

        sem_destroy(sem_parent);
        sem_destroy(sem_child);
        munmap(shared_var, sizeof(int));
        munmap(sem_parent, sizeof(sem_t));
        munmap(sem_child, sizeof(sem_t));
    }

    return 0;
}
