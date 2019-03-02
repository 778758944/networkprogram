
//
//  echo_server.c
//  netprogram
//
//  Created by WENTAO XING on 2019/2/25.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "unp.h"
#include "./sum.h"

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



void str_echo(int sockfd) {
    char buf[MAXLINE];
    ssize_t n;
    
again:
    while ((n = read(sockfd, buf, MAXLINE)) > 0) {
        Writen(sockfd, buf, n);
    }
    
    printf("n = %zd\n", n);
    
    if (n < 0 && errno == EINTR) {
        goto again;
    } else if (n < 0) {
        err_sys("str_echo: read_error");
    }
}

void str_echo2(int sockfd) {
    char buf[MAXLINE];
    ssize_t n;
    long arg1, arg2;
    for (;;) {
        if ((n = Readline(sockfd, buf,  MAXLINE)) == 0) {
            return;
        }
        
        printf("buf: %s\n", buf);
        
        if (sscanf(buf, "%ld%ld", &arg1, &arg2) == 2) {
            snprintf(buf, MAXLINE, "%ld\n", arg1+arg2);
        } else {
            snprintf(buf, MAXLINE, "input error\n");
        }
        
        Writen(sockfd, buf, strlen(buf));
    }
}

void str_echo3(int sockfd) {
    struct args arg;
    struct result res;
    
    for(;;) {
        if (Readn(sockfd, &arg, sizeof(arg)) == 0) {
            err_quit("Failed to read from client");
        }
        res.sum = arg.arg2 + arg.arg1;
        Writen(sockfd, &res, sizeof(res));
    }
}

int main() {
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    
    // the third param means transport layer protocol
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    // bind a network interface for for server
    Bind(listenfd, (SA*) &servaddr, sizeof(servaddr));
    
    Listen(listenfd, LISTENQ);
    Signal(SIGCHLD, sig_chld);
    
    for(;;) {
        clilen = sizeof(cliaddr);
        if ((connfd = accept(listenfd, (SA*) &cliaddr, &clilen)) < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                err_sys("accept error");
            }
        }
        if ((childpid = fork()) == 0) {
            Close(listenfd);
            str_echo(connfd);
            exit(0);
        }
        Close(connfd);
    }
}
