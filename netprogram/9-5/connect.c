//
//  connect.c
//  netprogram
//
//  Created by WENTAO XING on 2019/3/29.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "unp.h"

int connect_nonb(int sockfd, const SA * saptr, socklen_t salen, int nsec) {
    int flags, n, error;
    socklen_t len;
    fd_set rset, wset;
    struct timeval tval;
    
    flags = Fcntl(sockfd, F_GETFL, 0);
    Fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
    error = 0;
    if ((n = connect(sockfd, saptr, salen)) < 0) {
        printf("not connect immidiately\n");
        if (errno != EINPROGRESS)
            return -1;
    }
    
    // Do whatever we want when connect is taking place
    
    
    if (n == 0) {
        printf("connected immidiately\n");
        goto done;
    }
    
    FD_ZERO(&rset);
    FD_SET(sockfd, &rset);
    wset = rset;
    tval.tv_usec = 0;
    tval.tv_sec = nsec;
    
    if ((n = select(sockfd + 1, &rset, &wset, NULL, nsec ? &tval : NULL)) == 0) {
        close(sockfd);
        errno = ETIMEDOUT;
        return -1;
    }
    
    if (FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset)) {
        len = sizeof(error);
        if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0) {
            return -1;
        }
    } else {
        err_quit("select error:");
    }
    
done:
    Fcntl(sockfd, F_SETFL, flags);
    if (error) {
        close(sockfd);
        errno = error;
        return -1;
    }
    
    return 0;
}

int main(int argc, char ** argv) {
    int sockfd, n;
    ssize_t st;
    struct sockaddr_in addr;
    char buf[MAXLINE];
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(13);
    inet_pton(AF_INET, argv[1], &addr.sin_addr);
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    n = connect_nonb(sockfd, (SA*) &addr, sizeof(addr), 0);
    
    if (n == 0) {
        if ((st = read(sockfd, buf, MAXLINE)) > 0) {
            buf[st] = '\0';
            printf("%s", buf);
        }
    }
    
    return 0;
}
