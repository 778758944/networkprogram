//
//  echo_udp_client.c
//  netprogram
//
//  Created by Tom Xing on 3/2/19.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "unp.h"

#define NDG 2000
#define DGLEN 1400

void echo_cli(FILE * fp, int sockfd, SA * addr, socklen_t addrlen) {
    char buf[MAXLINE];
    long n;
    SA * cliaddr;
    cliaddr = Malloc(addrlen);
    socklen_t clilen;
    Connect(sockfd, addr, addrlen);
    while (Fgets(buf, MAXLINE, fp) != NULL) {
        sendto(sockfd, buf, strlen(buf), 0, addr, addrlen);
        clilen = addrlen;
        n = Recvfrom(sockfd, buf, MAXLINE, 0, cliaddr, &clilen);
        
        printf("reply from %s\n", Sock_ntop(cliaddr, clilen));
        if (clilen != addrlen) {
            printf("address length is not equal\n");
            continue;
        }
        
        if (memcmp(addr, cliaddr, clilen) != 0) {
            printf("address is not equal\n");
            continue;
        }
        buf[n] = 0;
        Fputs(buf, stdout);
    }
}

void echo_cli2(FILE * fp, int sockfd, SA * addr, socklen_t addrlen) {
    char buf[MAXLINE];
    Connect(sockfd, addr, addrlen);
    size_t n;
    
    while(Fgets(buf, MAXLINE, fp) != NULL) {
        Write(sockfd, buf, strlen(buf));
        n = Read(sockfd, buf, MAXLINE);
        buf[n] = 0;
        Fputs(buf, stdout);
    }
}

void echo_cli3(FILE * fp, int sockfd, SA * addr, socklen_t addrlen) {
    int i;
    char buf[DGLEN];
    for (i = 0; i < NDG; i++) {
        sendto(sockfd, buf, DGLEN, 0, addr, addrlen);
    }
}


int main(int argc, char **argv) {
    struct sockaddr_in servaddr;
    int sockfd;
    if (argc < 2) {
        err_quit("./echo_udp_client <IP ADDRESS>");
    }
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
    echo_cli3(stdin, sockfd, (SA*) &servaddr, sizeof(servaddr));
    return 0;
}
