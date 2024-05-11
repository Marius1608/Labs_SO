#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    int fd[2];
    char c = 'a';

    if (pipe(fd) != 0) {
        perror("Could not create pipe");
        return 1;
    }

    if (fork() != 0) {
        // parent
        close(fd[0]); 
        for (int i = 0; i < 10; i++) {
            write(fd[1], &c, sizeof(c)); 
            read(fd[0], &c, sizeof(c)); 
            printf("Parent: %c\n", c);
            c++;
        }
        close(fd[1]); 
        wait(NULL);
    } else {
        // child
        close(fd[1]); 
        for (int i = 0; i < 10; i++) {
            read(fd[0], &c, sizeof(c)); 
            printf("Child: %c\n", c);
            c++;
            write(fd[1], &c, sizeof(c)); 
        }
        close(fd[0]);
    }

    return 0;
}
