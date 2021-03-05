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
        "jobs",
        "fg"
};

void (*builtin_func[]) (char **) = {
        &cd,
        &bg, 
        &jobs,
        &fg
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
            argCount = argCount - 1;
            args[argCount] = NULL; 
            background = 1;
        
    } else{
        printf("Asked to run on foreground thread\n");
    }

    if((pid = fork()) == 0) {
        if(execvp(args[0], args) == -1){
            printf("%s\n", strerror(errno));
            exit(EXIT_FAILURE);
        };
    } else if(pid > 0){
        pid_list_index = pid_list_index + 1;
        pid_list[pid_list_index] = pid;
        if(background == 0){
            do {
                wpid = waitpid(pid, &status, WUNTRACED);
            } while(!WIFEXITED(status) && !WIFSIGNALED(status));
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
    if(args[1] == NULL){
        for(int i=0; i<=pid_list_index; i++){
            if(pid_list[i] != 0){
                printf("[%d] Processo %d\n", i, pid_list[i]);
            }
        }
    } else {
        int processIndex = (int)strtol(args[1], (char **)NULL, 10);
        if(processIndex > pid_list_index){
            printf("Você digitou um valor acima do disponível\n");
        } else {
            printf("[%d] Processo %d\n", processIndex, pid_list[processIndex]);
        }
    }
}

void bg(char **args){

}

void fg(char **args){
    int i = 0;
    pid_t pid, wpid;
    int status;

    while(args[1][i+1] != '\n'){
        args[1][i] = args[1][i+1];
        i++;
    }

    pid = (int)strtol(args[1], (char **)NULL, 10);

    int found = 0;  

    for(int i=0; i<=pid_list_index; i++){
        if(pid_list[i] == pid) {
            found = 1;
        }
    }

    if(found == 0){
        printf("Processo %d não encontrado.\n", pid);
    } else {
         if(waitpid(pid, &status, WUNTRACED) == -1){
                printf("%s\n", strerror(errno));
         }
    }
}

void handleFinishedPID(pid_t pid){
    int index = -1;
    
    for(int i = 0; i <= pid_list_index; i++){
        if(pid == pid_list[i]){
            index = i;
        }
    }

    if(index == pid_list_index){
        pid_list_index = pid_list_index - 1;
    } else {
        pid_list[index] = 0;
    }
}

