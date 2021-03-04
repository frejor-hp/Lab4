#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "tratacomandos.h"
#include "executa.h"

void loop();

int main() {

    loop();

    return 0;
}

void loop(){
    char *line;
    char **args;
    pid_t pid, wpid;
    int status;

    while(1){
        printf("mabshell> ");
        line = get_line();
        args = get_args(line);

        if(args[0] == (void *)EOF) return;

        if((pid = fork()) == 0) {
            execute(args);
        } else if(pid > 0){
            do {
                wpid = waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        } else{
            perror("Error when forking");
        }

        free(line);
        free(args);
    }
}
