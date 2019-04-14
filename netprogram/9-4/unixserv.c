//
//  unixserv.c
//  netprogram
//
//  Created by WENTAO XING on 2019/3/15.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "unp.h"

void sig_chld(int signo) {
    pid_t pid;
    int stat;
    // wait will block the function and when it is block, the new signal will be ignore, so it result the child process to zombia
    //    pid = wait(&stat);
    //    printf("child %d terminated\n", pid);
    //
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        printf("child %d terminated\n", pid);
    }
    return;
}

void str_echo(int connfd) {
    ssize_t n;
    char buf[MAXLINE];
    for (;;) {
        if ((n  = read(connfd, buf, MAXLINE)) < 0) {
            if (errno == EINTR) continue;
            err_sys("read error:");
        } else {
            write(connfd, buf, n);
        }
    }
}



int main() {
    int listenfd, connfd, pid;
    struct sockaddr_un addr, cliaddr;
    socklen_t len;
    
    unlink(UNIXSTR_PATH);
    bzero(&addr, sizeof(addr));
    
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, UNIXSTR_PATH);
    listenfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    Bind(listenfd, (SA *) &addr, sizeof(addr));
    
    Listen(listenfd, LISTENQ);
    
    Signal(SIGCHLD, sig_chld);
    
    for (;;) {
        len = sizeof(cliaddr);
        if ((connfd = Accept(listenfd, (SA*) &cliaddr, &len)) < 0) {
            if (errno == EINTR) {
                printf("signal occured");
                continue;
            } else {
                err_sys("accept error");
            }
        }
        
        if ((pid = fork()) == 0) {
            Close(listenfd);
            str_echo(connfd);
            exit(0);
        }
        Close(connfd);
        
    }
    
    
    
}
