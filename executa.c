//
// Created by felipe on 02/03/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "string.h"
#include "executa.h"

char *builtin_str[] = {
        "cd"
};

void (*builtin_func[]) (char **) = {
        &cd
};

void execute(char **args){

    int i;

    if (args[0] == NULL) {
        return;
    }

    for (i = 0; i < num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            (*builtin_func[i])(args);
        }
    }

    launch(args);
}

void launch(char  **args){
}

int num_builtins(){
    return sizeof(builtin_str) / sizeof(char *);
}

void cd(char **args){
    if(args[1] == NULL) chdir(getenv("HOME"));
    else {
        if(chdir(args[1]) != 0) printf("%s\n", strerror(errno));
    }
}