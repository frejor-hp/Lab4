//
// Created by felipe on 02/03/2021.
//

#ifndef LAB4_EXECUTA_H
#define LAB4_EXECUTA_H

#endif //LAB4_EXECUTA_H

void execute(char **args);
void launch(char  **args);
int num_builtins();
void cd(char **args);
void bg(char **args);
void jobs(char **args);
void fg(char **args);
void intHandler(sig_t s);
void stpHandler(sig_t s);
void extHandler(int signo, siginfo_t *si, void *data);
void handleFinishedPID(pid_t pid);
