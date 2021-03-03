#include <sys/types.h> 
#include <sys/wait.h>
#include <stdlib.h>

int lsh_launch(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("lsh");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int lsh_execute(char **args)
{
	int i;

	if (args[0] == NULL) {
		return 1;
	}
  //tratar o built in
  
  
  int ret = execv(args[0], args);

  return lsh_launch(args);
}


