#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
# define CLOCK_MONOTONIC 1
int main() {
    char path[] = "./PipeCopy";

    size_t bufferSize=1;
    for (int i = 1; i <= 20; i++) {
        bufferSize *= 2;
        struct timespec start, end;
        double elapsed;
        
        clock_gettime(CLOCK_MONOTONIC, &start);  // 记录开始时间

        pid_t pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Fork failed\n");
            continue;  // 继续下一个迭代
        } else if (pid == 0) {
            // 子进程
            char buffer[42];
            sprintf(buffer, "%d", bufferSize);
            execl(path, path, "input.txt", "output.txt", buffer, NULL);

            perror("execl() failure");
            exit(1);
        } else {
            // 父进程等待子进程完成
            wait(NULL);
            clock_gettime(CLOCK_MONOTONIC, &end);  // 记录结束时间

            elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
            printf("%zu: %f \n", bufferSize, elapsed);
        }
    }
    return 0;
}
