#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#define TEST_TIMES 20
int main() {
    char path[] = "/home/xyma/MarxFiles/Project1/copy/ForkCopy";
    chdir("/home/xyma/MarxFiles/Project1/copy");

    size_t bufferSize = 1;
    for (int i = 0; i < 20; i++) {
        bufferSize *= 2;

        double sum = 0;
        for (int j = 0; j < TEST_TIMES; j++) {
            clock_t start, end;
            double elapsed;
            start = clock();

            pid_t pid = fork();
            if (pid < 0) {
                fprintf(stderr, "Fork failed\n");
                continue;
            }
            if (pid == 0) {  // the child process
                char buffer[42];
                sprintf(buffer, "%zu", bufferSize);
                if (execl(path, "ForkCopy", "input.txt", "output.txt", buffer, (char *)NULL) == -1) {
                    perror("execl failed in benchmark");
                    exit(EXIT_FAILURE);
                }
            } else {
                wait(NULL);
                end = clock();
                elapsed = (double)(end - start) / CLOCKS_PER_SEC;
                sum += elapsed;
            }
        }
        printf("%zu:%lf\n", bufferSize, sum / TEST_TIMES);
    }
    return 0;
}
