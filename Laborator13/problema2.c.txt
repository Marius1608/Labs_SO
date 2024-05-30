#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main()
{
    pid_t p2, p3, p4;

    printf("[orig]P1[%d]: Created\n", getpid());
    p2 = fork();
    if (p2 < 0) {
        perror("fork failed");
        exit(1);
    }

    if (p2 == 0) {
    	int num;
    	int fd[2];
    	
    	printf("[%d]P2[%d]: Created\n", getppid(), getpid());
    	
    	if (pipe(fd) == -1) {
            perror("pipe failed");
	    exit(1);
	}
    	
        p3 = fork();
        if (p3 < 0) {
            perror("fork failed");
            exit(1);
        }

        if (p3 == 0) {
            printf("[%d]P3[%d]: Created\n", getppid(), getpid());
        
            close(fd[0]);
            
            printf("[%d]P3[%d]: Introduceți un număr:\n", getppid(), getpid());
            scanf("%d", &num);
            
            write(fd[1], &num, sizeof(int));
            close(fd[1]);
            
            printf("[%d]P3[%d]: Ending\n", getppid(), getpid());
        } else {
            p4 = fork();
            if (p4 < 0) {
                perror("fork failed");
                exit(1);
            }

            if (p4 == 0) {
            	printf("[%d]P4[%d]: Created\n", getppid(), getpid());
            
                close(fd[1]);
                
                read(fd[0], &num, sizeof(int));
                printf("[%d]P4[%d]: Numărul primit este %d\n", getppid(), getpid(), num);
               
                close(fd[0]);
                
                printf("[%d]P4[%d]: Ending\n", getppid(), getpid());
            } else {
                close(fd[0]);
                close(fd[1]);
                
                waitpid(p3, NULL, 0);
                waitpid(p4, NULL, 0);
                
                printf("[%d]P2[%d]: Ending\n", getppid(), getpid());
            }
        }
    } else {
        waitpid(p2, NULL, 0);
        printf("[orig]P1[%d]: Ending\n", getpid());
    }

    return 0;
}
