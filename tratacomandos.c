//
// Created by felipe on 02/03/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tratacomandos.h"

char *get_line(){
    char *line = NULL;
    size_t size = 0;

    if(getline(&line, &size, stdin) == -1){
        free(line);
        exit(EXIT_FAILURE);
    }

    return line;
}
char **get_args(char *line){

    int size = 64;
    int index = 0;
    char *arg;
    char **args = malloc(size * sizeof(char *));

    while((arg = strtok(line, " \n")) != NULL){
        args[index] = arg;
        index++;

        if(index == size){
            size += size;
            args = realloc(args, size * sizeof(char *));
            if(!args){
                printf("allocation error");
                free(line);
                exit(EXIT_FAILURE);
            }
        }
    }
    return args;
}