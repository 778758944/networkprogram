#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void handler(int sig) {
    pid_t pid;
    pid = wait(0);
    printf("%d is terminated\n", pid);
}

int main() {
    int status;
    pid_t pid;
    if ((pid = fork()) == 0) {
        // child process
        sleep(15);
        printf("child process\n");
        exit(0);
    }
    printf("main process\n");
    signal(SIGCHLD, handler);
    return 0;
}

