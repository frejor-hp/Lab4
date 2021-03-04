//
// Created by felipe on 02/03/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "executa.h"

char *builtin_str[] = {
        "cd",
        "bg"
};

void (*builtin_func[]) (char **) = {
        &cd,
        &bg
};

void execute(char **args){

    int i;
    pid_t pid, wpid;
    int status;

    if (args[0] == NULL) {
        return;
    }

    for (i = 0; i < num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            (*builtin_func[i])(args);
        }
    }

    if((pid = fork()) == 0) {
        launch(args);
    } else if(pid > 0){
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    } else{
        perror("Error when forking");
    }
}

void launch(char  **args){
    int background = 0;
    int argCount = 0;

    while((args[argCount] != NULL)){
            argCount = argCount + 1;
    }

    if(strcmp(args[argCount - 1], "&") == 0){
            printf("Asked to run on background thread\n");
            background = 1;
            printf("Running on background thread\n");
    } else{
        printf("Asked to run on foreground thread\n");
    }
    execvp(args[0], args);
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

void bg(char **args){

}