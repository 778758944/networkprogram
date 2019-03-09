//
//  daytimeudpserv.c
//  netprogram
//
//  Created by WENTAO XING on 2019/3/6.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "./libnet.h"

int main(int argc, char ** argv) {
    int sockfd = 0;
    struct sockaddr addr;
    socklen_t addrlen;
    time_t t;
    char buf[MAXLINE];
    
    if (argc == 2) {
        sockfd = Udp_server2(NULL, argv[1], NULL);
    } else if(argc != 2) {
        sockfd = Udp_server2(argv[1], argv[2], NULL);
    } else {
        err_quit("Usage error");
    }
    
    
    for(;;) {
        addrlen = sizeof(addr);
        recvfrom(sockfd, buf, MAXLINE, 0, &addr, &addrlen);
        printf("data from %s\n", Sock_ntop(&addr, addrlen));
        t = time(NULL);
        snprintf(buf, MAXLINE, "%.24s\r\n", ctime(&t));
        sendto(sockfd, buf, strlen(buf), 0, &addr, addrlen);
    }
}
