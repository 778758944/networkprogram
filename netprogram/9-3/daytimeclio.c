//
//  daytimeclio.c
//  netprogram
//
//  Created by Tom Xing on 3/9/19.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "../lib/libnet.h"

int main(int argc, char ** argv) {
    int sockfd;
    ssize_t n;
    struct sockaddr * addr;
    socklen_t len;
    char buf[MAXLINE];
    struct timeval tv;
    
    sockfd = Udp_client2(argv[1], argv[2], &addr, &len);
    sendto(sockfd, NULL, 0, 0, addr, len);
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    Setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    
    if ((n = recvfrom(sockfd, buf, MAXLINE, 0, addr, &len)) > 0) {
        buf[n] = 0;
        printf("%s\n", buf);
    } else {
        if (errno == EWOULDBLOCK) {
            printf("sock timeout");
        } else {
            err_sys("recvfrom error");
        }
    }
    
    return 0;
    
    
    
}
