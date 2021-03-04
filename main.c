#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
