//
//  dgserv.c
//  netprogram
//
//  Created by WENTAO XING on 2019/4/9.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "unp.h"
#include <time.h>

int main(int argc, char ** argv) {
    int sockfd;
    struct sockaddr_in addr;
    struct sockaddr_in cliaddr;
    ssize_t n;
    char buf[MAXLINE];
    socklen_t len;
    time_t t;
    
    bzero(&addr, sizeof(addr));
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(13);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bind(sockfd, (SA *) &addr, sizeof(addr));
    
    for (;;) {
        len = sizeof(cliaddr);
        n = recvfrom(sockfd, buf, MAXLINE, 0, (SA*) &cliaddr, &len);
        inet_ntop(cliaddr.sin_family, &cliaddr.sin_addr, buf, INET_ADDRSTRLEN);
        
        printf("new client: %s:%d\n", buf, ntohs(cliaddr.sin_port));
        if (n < 0) err_sys("error:");
        t = time(NULL);
        snprintf(buf, MAXLINE, "%.24s\r\n", ctime(&t));
        sendto(sockfd, buf, strlen(buf), 0, (SA*) &cliaddr, len);
    }
    
}
