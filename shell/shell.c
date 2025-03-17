#include <arpa/inet.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include "pthread.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"
#include "sys/wait.h"
#include "unistd.h"

#define DEFAULT_BUFFER_SIZE 2048

typedef struct {
    int connfd;
    struct sockaddr_in clientAddr;
} clientInfo_t;

void removeTrailingLine(char *command) {
    size_t len = strlen(command);
    if (len && command[len - 1] == '\n') {
        command[len - 1] = '\0';
    }

    char *cr = strchr(command, '\r');
    if (cr) {
        *cr = '\0';
    }
}

void parseCommand(char command[], char cmd[][1000], int *cmdCount) {
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

    if (strcmp(args[0], "cd") == 0) {
        if (tot == 1) {
            chdir(getenv("HOME"));
        } else {
            if (chdir(args[1]) < 0) {
                fprintf(stderr, "cd failed\n");
                strcat(buffer_of_last_command, "cd failed\n");
            }
        }

        for (int i = 0; i < tot; i++) {
            free(args[i]);
        }
        return NULL;
    }  // handle cd command

    args[tot] = NULL;
    tot++;

    pid_t pid;
    int fd_in[2];   // this one is for the child process get input data from parent process
    int fd_out[2];  // this is for execlvp to write output to parent process
    if (pipe((fd_in)) < 0) {
        fprintf(stderr, "pipe failed\n");
        exit(EXIT_FAILURE);
    }
    if (pipe(fd_out) < 0) {
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
        // note : fd[0] = read end of pipe, fd[1] = write end of pipe

        if (dup2(fd_out[1], STDOUT_FILENO) < 0 || dup2(fd_out[1], STDERR_FILENO) < 0) {
            fprintf(stderr, "dup2 failed\n");
            exit(EXIT_FAILURE);
        }

        if (dup2(fd_in[0], STDIN_FILENO) < 0) {
            fprintf(stderr, "dup2 failed\n");
            exit(EXIT_FAILURE);
        }

        close(fd_out[0]);  // close the unused read end and original write end
        close(fd_out[1]);
        close(fd_in[0]);
        close(fd_in[1]);

        execvp(args[0], args);  // execute the command

        perror("COMMAND EXECUTION failed ");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // parent process
        close(fd_in[0]);   // close the read end of input pipe
        close(fd_out[1]);  // close the write end of output pipe

        if (!isFirst) {
            if (write(fd_in[1], buffer_of_last_command, strlen(buffer_of_last_command)) < 0) {
                fprintf(stderr, "write failed\n");
                exit(EXIT_FAILURE);
            }
        }
        close(fd_in[1]);  //// Important: close the write end after writing so the child sees EOF.

        buffer_of_last_command[0] = '\0';
        // clear cache of last command

        size_t bytesRead = 0;
        size_t totalBytesRead = 0;
        while (bytesRead =
                   read(fd_out[0], buffer_of_last_command + totalBytesRead, DEFAULT_BUFFER_SIZE - totalBytesRead - 1)) {
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

void *shellCore(char command[], char ret[]) {
    // printf("message received from client: %s\n", command);
    if (strcmp(command, "exit\n") == 0) {
        strcpy(ret, "exit");
        return NULL;
        // exit the shell
    }
    removeTrailingLine(command);

    char cmd[100][1000];
    int cmdCount = 0;
    parseCommand(command, cmd, &cmdCount);
    // command => original command input , which is in a line
    // cmd => the command after parsing, which is separated by "|"
    // cmdCount => the number of commands
    char buffer_of_last_command[DEFAULT_BUFFER_SIZE] = "";
    for (int i = 0; i < cmdCount; i++) {
        excecuteCommand(cmd[i], buffer_of_last_command, (i == 0));
        printf("%s\n", buffer_of_last_command);  // the output of each command is printed in server side
    }
    strcpy(ret, buffer_of_last_command);
    // only the output of last command will be sent to client
    return NULL;
}

static pthread_mutex_t glock = PTHREAD_MUTEX_INITIALIZER;

void *serveClient(const int connfd, const struct sockaddr_in clientAddr) {
    char recv_buffer[DEFAULT_BUFFER_SIZE];
    char send_buffer[DEFAULT_BUFFER_SIZE];

    char working_dir[DEFAULT_BUFFER_SIZE];
    strcpy(working_dir, getenv("HOME"));
    while (true) {
        memset(recv_buffer, 0, sizeof(recv_buffer));
        memset(send_buffer, 0, sizeof(send_buffer));

        int byteRead = read(connfd, recv_buffer, sizeof(recv_buffer) - 1);
        if (byteRead < 0) {
            fprintf(stderr, "read from client side failed, client IP: %d:\n", clientAddr.sin_addr, clientAddr.sin_port);
            close(connfd);
            break;
        }  // handle exception

        recv_buffer[byteRead] = '\0';  // null terminate the input

        if (strcmp(recv_buffer, "exit\n") == 0 || strcmp(recv_buffer, "exit") == 0 || byteRead == 0) {
            break;
        }  // handle exit condition

        printf("commmand from %d:%d :%s\n", clientAddr.sin_addr, clientAddr.sin_port, recv_buffer);

        pthread_mutex_lock(&glock);  // lock the mutex, here is the critical section
        chdir(working_dir);          // change the working dir to the thread distinct working dir
        shellCore(recv_buffer, send_buffer);
        if (getcwd(working_dir, sizeof(working_dir)) == NULL) {
            perror("getcwd failed");
            break;
        }  // update the working dir
        pthread_mutex_unlock(&glock);

        if (send(connfd, send_buffer, strlen(send_buffer), 0) < 0) {
            perror("send failed");
            break;
        }
    }

    printf("Connection from %s:%d closed\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
    close(connfd);

    return NULL;
}

void *serveClientThread(void *arg) {
    clientInfo_t *clientInfo = (clientInfo_t *)arg;
    serveClient(clientInfo->connfd, clientInfo->clientAddr);
    free(clientInfo);
    return NULL;
}

void *severCore(char ip_addr[], const int port) {
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);  // create a socket
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Optional: Set socket options for address reuse.
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(ip_addr);  // initialize server's IP
    serverAddr.sin_port = htons(port);                // initialize port (network byte order)

    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 20) < 0) {
        perror("listen failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on %s:%d\n", ip_addr, port);

    // Main server loop
    while (true) {
        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int connfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clientLen);
        // wait until client  connect
        if (connfd < 0) {
            perror("accept failed");
            continue;  // skip to next iteration instead of exiting
        }
        printf("Connection from %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

        clientInfo_t *INFO = (clientInfo_t *)malloc(sizeof(clientInfo_t));
        if (INFO == NULL) {
            fprintf(stderr, "malloc failed\n");
            close(connfd);
            continue;
        }

        INFO->connfd = connfd;
        INFO->clientAddr = clientAddr;

        pthread_t clientThread;
        pthread_attr_t attr;
        pthread_attr_init(&attr);

        int state = pthread_create(&clientThread, &attr, serveClientThread, (void *)INFO);
        if (state != 0) {
            fprintf(stderr, "pthread_create failed\n");
            close(connfd);
            free(INFO);
        }
        pthread_detach(clientThread);
    }

    close(sockfd);
    return NULL;
}
int main(int argc, char *argv[]) {
    char ip_addr[64];
    int port;
    if (argc == 3) {
        strcpy(ip_addr, argv[1]);
        port = atoi(argv[2]);
    } else if (argc == 2) {
        strcpy(ip_addr, "127.0.0.1");
        port = atoi(argv[1]);
    }
    severCore(ip_addr, port);
    return 0;
}
