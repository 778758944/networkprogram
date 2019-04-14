//
//  mycat.c
//  netprogram
//
//  Created by WENTAO XING on 2019/3/18.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "unp.h"

int my_open(char * pathname, int mode);

ssize_t read_fd(int fd, void * ptr, size_t nbytes, int * recvfd);
int main(int argc, char ** argv) {
    int fd;
    ssize_t n;
    char buf[BUFFSIZE];
    
    if (argc != 2) err_quit("Usage:");
    
    if ((fd = my_open(argv[1], O_RDONLY)) < 0) {
        err_sys("cannot open %s", argv[1]);
    }
    
    while ((n = Read(fd, buf, BUFFSIZE)) > 0) {
        Write(STDOUT_FILENO, buf, n);
    }
}

int my_open(char * pathname, int mode) {
    int fd, sockfd[2], status;
    pid_t childpid;
    char c, argsockfd[10], argmode[10];
    Socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd);
    
    if ((childpid = Fork()) == 0) {
        // child process
        Close(sockfd[0]);
        snprintf(argsockfd, sizeof(argsockfd), "%d", sockfd[1]);
        snprintf(argmode, sizeof(argmode), "%d", mode);
        execl("./openfile", "openfile", argsockfd, pathname, argmode, NULL);
        err_sys("execl error");
    }
    
    Close(sockfd[1]);
    
    Waitpid(childpid, &status, 0);
    if (WIFEXITED(status) == 0) {
        err_quit("child did terminate");
    }
    
    if ((status = WEXITSTATUS(status)) == 0) {
        Read_fd(sockfd[0], &c, 1, &fd);
    } else {
        errno = status;
        fd = -1;
    }
    
    Close(sockfd[0]);
    return fd;
}


ssize_t read_fd(int fd, void * ptr, size_t nbytes, int * recvfd) {
    struct msghdr msg;
    struct iovec iov[1];
    ssize_t n;
    
#ifdef HAVE_MSGHDR_MSG_CONTROL
    union {
        struct cmsghdr cm;
        
    };
#endif
    return 1;
}

