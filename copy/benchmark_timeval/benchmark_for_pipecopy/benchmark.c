#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#define TEST_TIMES 20
#define CLOCK_MONOTONIC 1

int main() {
    char path[] = "/home/xyma/MarxFiles/Project1/copy/PipeCopy";
    chdir("/home/xyma/MarxFiles/Project1/copy");
    size_t bufferSize = 1;
    for (int i = 1; i <= 20; i++) {
        bufferSize *= 2;
        double sum = 0;
        for (int j = 0; j < TEST_TIMES; j++) {
            struct timeval start, end;
            long seconds, microseconds;
            double elapsed;

            gettimeofday(&start, NULL);
            pid_t pid = fork();
            if (pid < 0) {
                fprintf(stderr, "Fork failed\n");
                continue;
            } else if (pid == 0) {
                // 子进程
                char buffer[42];
                sprintf(buffer, "%zu", bufferSize);
                if (execl(path, "PipeCopy", "input.txt", "output.txt", buffer, (char *)NULL) == -1) {
                    perror("execl failed in benchmark");
                    exit(EXIT_FAILURE);
                }
            } else {
                wait(NULL);
                gettimeofday(&end, NULL);
                seconds = end.tv_sec - start.tv_sec;
                microseconds = end.tv_usec - start.tv_usec;
                elapsed = seconds + microseconds / 1e6;
                sum += elapsed;
            }
        }
        printf("%zu:%lf\n", bufferSize, sum / TEST_TIMES);
    }
    return 0;
}