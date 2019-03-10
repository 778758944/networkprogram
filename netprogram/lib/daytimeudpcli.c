//
//  daytimeudpcli.c
//  netprogram
//
//  Created by WENTAO XING on 2019/3/6.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "./libnet.h"

int main(int argc, char** argv) {
    int sockfd = 0;
    socklen_t len;
    struct sockaddr * addr;
    char buf[MAXLINE];
    size_t readn;
    
    if (argc != 3) {
        err_quit("Usage error");
    }
    sockfd = Udp_client2(argv[1], argv[2], &addr, &len);
    printf("send to %s\n", Sock_ntop(addr, len));
    sendto(sockfd, "", 0, 0, addr, len);
    
    // test select timeout
    if (Readable_timeo2(sockfd, 5) == 0) {
        printf("timeout\n");
    } else {
        readn = recvfrom(sockfd, buf, MAXLINE, 0, NULL, NULL);
        buf[readn] = 0;
        printf("%s", buf);
    }
    
    /*
    readn = recvfrom(sockfd, buf, MAXLINE, 0, NULL, NULL);
    buf[readn] = 0;
    printf("%s", buf);
     */
    /*
    sockfd = Udp_connect2(argv[1], argv[2]);
    Write(sockfd, "", 0);
    readn = Read(sockfd, buf, MAXLINE);
    buf[readn] = 0;
    printf("%s", buf);
     */
    return 0;
}
