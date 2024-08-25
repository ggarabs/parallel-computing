#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char *argv[]){
	pid_t childpid = 0;
	
	if(argc != 2){
		fprintf(stderr, "Usage: %s processes\n", argv[0]);
		return 1;
	}

	int n = atoi(argv[1]), status;

	for(int i = 0; i < n; i++){
		childpid = fork();

		if(childpid == 0){
			srand(time(NULL) + getpid());
			int sleep_time = rand() % 10 + 1;
			sleep(sleep_time);
			return sleep_time;
		}else{
			wait(&status);
			printf("No processo pai: processo filho: %ld, status de saída do filho: %ld\n", (long)childpid, (long)status >> 8);	
		}
	}

	return 0;
}
