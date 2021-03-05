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

pid_t pid_list[1000];
int pid_list_index = -1;

char *builtin_str[] = {
        "cd",
        "bg",
        "jobs"
};

void (*builtin_func[]) (char **) = {
        &cd,
        &bg, 
        &jobs
};

void execute(char **args){

    int i;

    if (args[0] == NULL) {
        return;
    }

    for (i = 0; i < num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            (*builtin_func[i])(args);
            return;
        }
    }

    launch(args);
}

void launch(char  **args){
    int background = 0;
    int argCount = 0;
    pid_t pid, wpid;
    int status;

    while((args[argCount] != NULL)){
            argCount = argCount + 1;
    }

    if(strcmp(args[argCount - 1], "&") == 0){
            printf("Asked to run on background thread\n");
            background = 1;
        
    } else{
        printf("Asked to run on foreground thread\n");
    }

    if((pid = fork()) == 0) {
        execvp(args[0], args);
    } else if(pid > 0){
        pid_list_index = pid_list_index + 1;
        pid_list[pid_list_index] = pid;
        if(background == 0){
            wpid = waitpid(pid, &status, WUNTRACED);
        }
    } else{
        perror("Error when forking");
    }   
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

void jobs(char **args){
    for(int i=0; i<=pid_list_index; i++){
        printf("[%d] Processo %d\n", i, pid_list[pid_list_index]);
    }
}

void bg(char **args){

}

