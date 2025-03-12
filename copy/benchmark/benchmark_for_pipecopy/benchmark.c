#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main() {
    char path[] = "./PipeCopy";

    size_t bufferSize;
    for (int i = 1; i < 150; i++) {
        bufferSize = i * 500;
        double elapsed;
        clock_t start, end;
        start = clock();  // 记录开始时间

        pid_t pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Fork failed\n");
            continue;  // 继续下一个迭代
        } else if (pid == 0) {
            // 子进程
            char buffer[42];
            sprintf(buffer, "%ld", bufferSize);
            execl(path, path, "input.txt", "output.txt", buffer, NULL);

            perror("execl() failure");
            exit(1);
        } else {
            wait(NULL);
            end = clock();
            elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
            printf("%zu:%lf \n", bufferSize, elapsed);

            // output format :
            // BufferSize:Time
        }
    }
    return 0;
}
