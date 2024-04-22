#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main(void)
{
	
	printf("[CHILD] My PID1 is %d. My parent's PID is %d.\n", getpid(), getppid());
	pid_t P2,P3;
	P2=fork();
	if(P2==0){

		printf("[CHILD] My PID2 is %d. My parent's PID1 is %d.\n", getpid(), getppid());
		pid_t P4;
		P4=fork();
		if(P4==0){
			printf("[CHILD] My PID4 is %d. My parent's PID2 is %d.\n", getpid(), getppid());
			sleep(60);
		}
		wait(&P4);
	}
	wait(&P2);

	P3=fork();
	if(P3==0){

		printf("[CHILD] My PID3 is %d. My parent's PID2 is %d.\n", getpid(), getppid());
		pid_t P5;
		P5=fork();
		if(P5==0){
			printf("[CHILD] My PID5 is %d. My parent's PID3 is %d.\n", getpid(), getppid());
			sleep(60);
		}
		wait(&P5);
	}

	wait(&P3);
	

	return 0;
}