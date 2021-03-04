#include <sys/types.h> 
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>


int lsh_launch(char **args)
{	
	pid_t pid, wpid;
	int status;
	int background = 0;
	int argCount = 0;
	
	while(1){
		if(args[argCount] != NULL){
			argCount = argCount + 1;
		} else{
			break;
		}
	}
	if(strcmp(args[argCount - 1], "&") == 0){
		printf("Asked to run on background thread");
		pid = fork();
		background = 1;
		if(pid == 0){
			printf("Running on background thread");
			execvp(args[0], args);
		} else if(pid > 0){
			do {
				wpid = waitpid(pid, &status, WUNTRACED);
			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		} else {
			perror("Error when forking");
		}
	} else {
		printf("Asked to run on foreground thread");
		execvp(args[0], args);
	}


  return 1;
}

int lsh_execute(char **args)
{
	if (args[0] == NULL) {
		return 1;
	}
  return lsh_launch(args);
}


