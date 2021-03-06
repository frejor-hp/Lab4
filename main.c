#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "tratacomandos.h"
#include "executa.h"
#include <signal.h>

void extHandler(sig_t s){
    int pid = wait(NULL);
    handleFinishedPID(pid);
}

void loop();

int main() {

    loop();

    return 0;
}

void loop(){
    char *line;
    char **args;
		
    while(1){

        if(signal(SIGINT, SIG_IGN) == SIG_ERR)
		        printf("Erro no SIGINT\n");
        if(signal(SIGCHLD, extHandler) == SIG_ERR)
		        printf("Erro no SIGCHLD\n");
        if(signal(SIGTSTP, SIG_IGN) == SIG_ERR)
		        printf("Erro no SIGTSTP\n");

        char cwd[4096];
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd() error");
            return;
        }

        printf("%s> ", cwd);
        line = get_line();
        args = get_args(line);

        if(args[0] == (void *)EOF) return;

        execute(args);

        free(line);
        free(args);
    }
}
