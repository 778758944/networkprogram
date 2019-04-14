//
//  dgcli.c
//  netprogram
//
//  Created by WENTAO XING on 2019/4/11.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "../lib/libnet.h"

int main(int argc, char ** argv) {
    int sockfd;
    struct sockaddr_in addr;
    
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &addr.sin_addr);
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
//    mcast_set_loop(sockfd, 0);
    
    dg_cli(stdin, sockfd, (SA*) &addr, sizeof(addr));
    
    return 0;
}
