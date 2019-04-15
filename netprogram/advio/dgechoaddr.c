//
//  dgechoaddr.c
//  netprogram
//
//  Created by Tom Xing on 4/14/19.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "../lib/libnet.h"
#include "recvfromflags.h"

#undef MAXLINE
#define MAXLINE 20

void dg_echo5(int sockfd, SA * pciaddr, socklen_t clilen) {
    int flags;
    const int on = 1;
    socklen_t len;
    ssize_t n;
    char mesg[MAXLINE], str[INET6_ADDRSTRLEN];
    struct in_addr in_zero;
    struct unp_in_pktinfo pktinfo;
    
#ifdef IP_RECVDSTADDR
    if (setsockopt(sockfd, IPPROTO_IP, IP_RECVDSTADDR, &on, sizeof(on)) < 0) {
        err_ret("setsockopt of IP_RECVDSTADDR");
    }
#endif
    
#ifdef IP_RECVIF
    if (setsockopt(sockfd, IPPROTO_IP, IP_RECVIF, &on, sizeof(on)) < 0) {
        err_ret("setsockopt of IP_RECVIF");
    }
#endif
    
    bzero(&in_zero, sizeof(struct in_addr));
    for (;;) {
        len = clilen;
        flags = 0;
        n = recvfrom_flags2(sockfd, mesg, MAXLINE, &flags, pciaddr, &len, &pktinfo);
        printf("%zd-byte datagram from %s", n, Sock_ntop(pciaddr, len));
        if (memcmp(&pktinfo.ipi_addr, &in_zero, sizeof(in_zero)) != 0) {
            printf(", to %s", Inet_ntop(AF_INET, &pktinfo.ipi_addr, str, sizeof(str)));
        }
        if (pktinfo.ipi_ifindex > 0) {
            printf(", recv %d\n", pktinfo.ipi_ifindex);
        }
        
#ifdef MSG_TRUNC
        if (flags & MSG_TRUNC)
            printf("datagram truncated");
#endif
#ifdef MSG_CTRUNC
        if (flags & MSG_CTRUNC)
            printf("control info truncated");
#endif
#ifdef MSG_BCAST
        if (flags && MSG_TRUNC)
            printf("datagram truncated");
#endif
#ifdef MSG_MCAST
        if (flags& MCAST)
            printf("multicast");
#endif
        printf("\n");
        sendto(sockfd, mesg, n, 0, pciaddr, len);
    }
    
}
