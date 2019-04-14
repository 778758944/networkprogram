//
//  main.c
//  netprogram
//
//  Created by WENTAO XING on 2019/4/12.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "../lib/libnet.h"

#define SAP_NAME "sap.mcast.net"
#define SAP_PORT "9875"

void loop(int, socklen_t);

int main(int argc, char **argv) {
    int sockfd;
    const int on = 1;
    socklen_t salen;
    struct sockaddr *sa;
    
    if (argc == 1) {
        sockfd = Udp_client2(SAP_NAME, SAP_PORT, &sa, &salen);
    } else if (argc == 4) {
        sockfd = Udp_client2(argv[1], argv[2], &sa, &salen);
    } else {
        err_quit("Usage: \n");
    }
    
    Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    Bind(sockfd, sa, salen);
    
    mcast_join2(sockfd, sa, salen, NULL, 0);
    loop(sockfd, salen);
    exit(0);
}





