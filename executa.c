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
#include <signal.h>


pid_t pid_list[1000];
int pid_list_index = -1;
int bg_priority_list[1000];
int bg_priority_index = -1;

void intHandler(sig_t s) {
    int pid = wait(NULL);
    kill(pid, SIGINT);
    fflush(stdout);
}

void stpHandler(sig_t s) {
    int pid = wait(NULL);
    kill(pid, SIGSTOP);
    fflush(stdout);
}

void extHandler(int signo, siginfo_t *si, void *data){
    int status;
    unsigned long pid =(unsigned long)si->si_pid;
    printf("Caí no extHandler com pid %d\n", pid);
    if(pid > 0 ){
        handleFinishedPID(pid);
    }
}

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
        if(signal(SIGINT, SIG_DFL) == SIG_ERR)
		        printf("Erro no SIGINT\n");
        if(signal(SIGTSTP, SIG_DFL) == SIG_ERR)
		        printf("Erro no SIGTSTP\n");
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
            } while(!WIFEXITED(status) && !WIFSIGNALED(status) && !WIFSTOPPED(status));
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
        int i = 0;
        while(pid_list[i] != NULL){
            if(pid_list[i] != 0){
                printf("[%d] Processo %d\n", i, pid_list[i]);
            }
            i++;
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
    int i = 1;
    int pid;
    while(args[i] != NULL){
        int j = 0;
        while(args[i][j+1] != '\n'){
            args[i][j] = args[i][j+1];
            j++;
        }
        pid = (int)strtol(args[i], (char **)NULL, 10);
        bg_priority_index = i-1; //To keep array starting from 0
        bg_priority_list[bg_priority_index] = pid;
        i++;
    }
}

void fg(char **args){
    pid_t pid, wpid;
    int status;

    if(args[1] != NULL){   
        int index = (int)strtol(args[1], (char **)NULL, 10);
        if(index <= pid_list_index){
            pid = pid_list[index];
            if(waitpid(pid, &status, WUNTRACED) == -1){
                printf("%s\n", strerror(errno));
            }
        } else {
            printf("O valor %d é inválido\n", index);
        }
    } else if(bg_priority_list[0] != NULL) {
        pid = bg_priority_list[bg_priority_index];
        bg_priority_list[bg_priority_index] = NULL;     
        bg_priority_index--;
        if(waitpid(pid, &status, WUNTRACED) == -1){
                printf("%s\n", strerror(errno));
        }
    }
}

void handleFinishedPID(pid_t pid){
    int index = -1;
    
    for(int i = 0; i <= pid_list_index; i++){
        if(pid == pid_list[i]){
            printf("Achei o pid %d\n", pid);
            index = i;
        }
    }

    pid_list[index] = 0;
}


