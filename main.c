#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "tratacomandos.h"
#include "executa.h"
#include <signal.h>

void loop();

int main() {

    loop();

    return 0;
}

void intHandler(sig_t s) {
    printf("Peguei o ctrl c\n");
}

void stpHandler(sig_t s) {
    printf("Peguei o ctrl z\n");
}

void extHandler(sig_t s) {
    int pid = wait(NULL);
    handleFinishedPID(pid);
    printf("Removendo o processo com PID %d da minha listagem\n", pid);
}

void loop(){
    char *line;
    char **args;

	if(signal(SIGINT, intHandler) == SIG_ERR)
		printf("Erro no SIGINT\n");
	if(signal(SIGTSTP, stpHandler) == SIG_ERR)
		printf("Erro no SIGTSTP\n");
    if(signal(SIGCHLD, extHandler) == SIG_ERR)
		printf("Erro no SIGCHLD\n");
		
    while(1){

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
