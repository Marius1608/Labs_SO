#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(void)
{
    int fd[2];
    char string[] = "Lab11";
    int length = strlen(string) + 1; 

    if(pipe(fd) != 0) {
        perror("Could not create pipe");
        return 1;
    }

    if(fork() != 0) {
        //parent
        close(fd[0]); 
        write(fd[1], &length, sizeof(length)); 
        write(fd[1], string, length); 
        printf("Parent: wrote \"%s\" to pipe\n", string);
        close(fd[1]);
        wait(NULL);
    } else {
        //child
        close(fd[1]); 
        read(fd[0], &length, sizeof(length)); 
        char string_2[length]; 
        read(fd[0], string_2, length); 
        printf("Child: read \"%s\" from pipe\n", string_2);
        close(fd[0]);
    }

    return 0;
}
