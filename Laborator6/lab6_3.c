#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int i, n = 7;
    int count=0;
    int total_copii=0;
    int status=0;
    
    for (i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == 0) {
		count=0;
        } else {
        	count++;
        }
    }
    
    for(int i=0; i<count; i++){

	wait(&status);
	total_copii+=WEXITSTATUS(status);
	
    }
    printf("%d ",count);
    exit(total_copii);			
   	
    return 0;
}
