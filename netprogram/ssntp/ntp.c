//
//  ntp.c
//  netprogram
//
//  Created by WENTAO XING on 2019/4/12.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "ntp.h"
#include "../lib/libnet.h"
#include "../9-6/unpifi.h"

void sntp_proc(char *, ssize_t, struct timeval *);

int main(int argc, char ** argv) {
    int sockfd;
    char buf[MAXLINE];
    ssize_t n;
    socklen_t salen, len;
    struct ifi_info *ifi;
    struct sockaddr *mcastsa, *wild, *from;
    struct timeval now;
    
    if (argc != 2) {
        err_quit("usage: ssntp <IPADDRESS>");
    }
    
    sockfd = Udp_client2(argv[1], "ntp", &mcastsa, &salen);
    
    wild = Malloc(salen);
    memcpy(wild, mcastsa, salen);
    sock_set_wild(wild, salen);
    
    bind(sockfd, wild, salen);
    
#ifdef MCAST
    for (ifi = get_ifi_info2(mcastsa->sa_family, 1); ifi != NULL; ifi = ifi->ifi_next) {
        if (ifi->ifi_flags & IFF_MULTICAST) {
            mcast_join2(sockfd, mcastsa, salen, ifi->ifi_name, 0);
            printf("joined %s on %s\n", Sock_ntop(mcastsa, salen), ifi->ifi_name);
        }
    }
#endif
    from = malloc(salen);
    for (;;) {
        len = salen;
        n = recvfrom(sockfd, buf, sizeof(buf), 0, from, &len);
        Gettimeofday(&now, NULL);
        sntp_proc(buf, n, &now);
    }
}
