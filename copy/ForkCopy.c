#include "stdio.h"
#include "stdlib.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "unistd.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <src> <dest>\n", argv[0]);
        exit(1);
    } else if (argc > 4) {
        fprintf(stderr, "Usage %s : <src> <dest> <buffer size>\n", argv[0]);
        exit(1);
    }

    pid_t pid;
    pid = fork();
    if (pid < 0) {
        printf("Fork failed\n");
        exit(1);
    }
    if (pid == 0) {
        // child process
        if (argc == 4) {
            execl("./MyCopy", argv[0], argv[1], argv[2], argv[3], NULL);
        } else {
            execl("./MyCopy", argv[0], argv[1], argv[2], NULL);
        }
        perror("execl Failed");
        exit(1);
    } else {  // parent process
        wait(NULL);
    }
    return 0;
}