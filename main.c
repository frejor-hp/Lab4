#include <stdio.h>
#include <stdlib.h>
#include "tratacomandos.h"
#include "executacomandos.h"

void loop();
void print(char **arg, char *line);

int main() {

    loop();

    return 1;
}

void loop(){
    char *line;
    char **args;

    line = get_line();
    args = get_args(line);
    lsh_execute(args);

    free(line);
    free(args);
}

void print(char **args, char *line){
    char *arg;
    int index = 0;

    printf("%s", line);

    while((arg = args[index]) != NULL){
        printf("%s", arg);
        index++;
    }
}
