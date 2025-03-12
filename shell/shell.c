#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "stdbool.h"
#include "stdlib.h"
#include "string.h"
#include "sys/wait.h"
#include "unistd.h"
#define DEFAULT_BUFFER_SIZE 2048
char command[1000];

void removeTrailingLine() {
    size_t len = strlen(command);
    if (len && command[len - 1] == '\n') {
        command[len - 1] = '\0';
    }
}

void parseCommand(char cmd[][1000], int *cmdCount) {
    size_t len = strlen(command);
    char *single = strtok(command, "|");
    size_t cnt = 0;
    while (single != NULL) {
        strcpy(cmd[cnt], single);
        cnt++;
        single = strtok(NULL, "|");
    }
    *cmdCount = cnt;

    for (int i = 0; i < cnt; i++) {
        size_t len = strlen(cmd[i]);
        for (int j = len - 1; j >= 0; j--) {
            if (cmd[i][j] == ' ') {
                cmd[i][j] = '\0';
            } else {
                break;
            }
        }  // remove the trailing space

        size_t pivot;
        len = strlen(cmd[i]);
        for (int j = 0; j < len; j++) {
            if (cmd[i][j] != ' ') {
                pivot = j;
                break;
            }
        }
        for (int j = pivot; j < len; j++) {
            cmd[i][j - pivot] = cmd[i][j];
        }
        cmd[i][len - pivot] = '\0';  // remove leading space
    }
}

void *excecuteCommand(char *cmd, char *buffer_of_last_command, bool isFirst) {
    char *args[100];
    size_t tot = 0;

    char *buffer = strtok(cmd, " ");
    while (buffer != NULL) {
        size_t len = strlen(buffer);
        args[tot] = (char *)malloc(len + 1);
        if (args[tot] == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);  // allocate memory failed
        }

        strcpy(args[tot], buffer);
        tot++;
        buffer = strtok(NULL, " ");
    }

    if(strcmp(args[0],"cd") == 0){
        if(tot == 1){
            chdir(getenv("HOME"));
        }else{
            if(chdir(args[1]) < 0)
                fprintf(stderr, "cd failed\n");
        }

        for (int i = 0; i < tot; i++) {
            free(args[i]);
        }
        return NULL;
    } // handle cd command

    args[tot] = NULL;
    tot++;

    pid_t pid;
    int fd_in[2]; // this one is for the child process get input data from parent process
    int fd_out[2]; // this is for execlvp to write output to parent process
    if (pipe((fd_in)) < 0) {
        fprintf(stderr, "pipe failed\n");
        exit(EXIT_FAILURE);
    }
    if(pipe(fd_out) < 0){
        fprintf(stderr, "pipe failed\n");
        exit(EXIT_FAILURE);
    }
    // define and initailize pipe

    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // child process
        //note : fd[0] = read end of pipe, fd[1] = write end of pipe

        if (dup2(fd_out[1], STDOUT_FILENO) < 0 || dup2(fd_out[1], STDERR_FILENO) < 0) {
            fprintf(stderr, "dup2 failed\n");
            exit(EXIT_FAILURE);
        }

        if(dup2(fd_in[0], STDIN_FILENO) < 0){
            fprintf(stderr, "dup2 failed\n");
            exit(EXIT_FAILURE);
        }

        close(fd_out[0]); //close the unused read end and original write end
        close(fd_out[1]);
        close(fd_in[0]);
        close(fd_in[1]);


        execvp(args[0], args);

        perror("COMMAND EXECUTION failed");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // parent process
        close(fd_in[0]); // close the read end of input pipe
        close(fd_out[1]); // close the write end of output pipe

        if(!isFirst){
            if(write(fd_in[1],buffer_of_last_command,strlen(buffer_of_last_command)) < 0){
                fprintf(stderr, "write failed\n");
                exit(EXIT_FAILURE);
            }            
        }
        close(fd_in[1]); //// Important: close the write end after writing so the child sees EOF.

        buffer_of_last_command[0] = '\0'; 
        //clear cache of last command

        size_t bytesRead = 0;
        size_t totalBytesRead = 0;
        while (bytesRead = read(fd_out[0], buffer_of_last_command + totalBytesRead,
                                DEFAULT_BUFFER_SIZE - totalBytesRead - 1 )) {
            totalBytesRead += bytesRead;
            // accumulate the reading end of pipe until the pipe gets empty
        }
        // update the output buffer
        if (bytesRead < 0) {
            fprintf(stderr, "read failed when obtaining info from pipe\n");
            buffer_of_last_command[0] = '\0';
            exit((EXIT_FAILURE));
        } else {
            buffer_of_last_command[totalBytesRead] = '\0';
        }
        close(fd_out[0]);
        wait(NULL);
        // parent process
    }

    for (int i = 0; i < tot - 1; i++) {
        free(args[i]);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    
    while (true) {
        printf(">> ");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            continue;
        }
        if (strcmp(command, "exit\n") == 0) {
            break;
            // exit the shell
        }
        removeTrailingLine();

        char cmd[100][1000];
        int cmdCount = 0;
        parseCommand(cmd, &cmdCount);

        char buffer_of_last_command[DEFAULT_BUFFER_SIZE]= "";
        for (int i = 0; i < cmdCount;i++) {
            excecuteCommand(cmd[i], buffer_of_last_command, (i == 0));
            printf("%s\n", buffer_of_last_command);
        }
    }

    return 0;
}
