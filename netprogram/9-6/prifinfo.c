//
//  prifinfo.c
//  netprogram
//
//  Created by WENTAO XING on 2019/4/2.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "unpifi.h"

int main(int argc, char ** argv) {
    struct ifi_info *ifi, *ifihead;
    struct sockaddr * sa;
    u_char *ptr;
    int i, family = 0, doaliases;
    if (argc != 3) err_quit("usage:");
    
    if (strcmp(argv[1], "inet4") == 0) {
        family = AF_INET;
    } else if (strcmp(argv[1], "inet6") == 0) {
        family = AF_INET6;
    } else {
        err_quit("invalid <address-family>");
    }
    
    doaliases = atoi(argv[2]);
    
    for (ifihead = ifi = get_ifi_info2(family, doaliases); ifi != NULL; ifi = ifi->ifi_next ) {
        printf("%s: < ", ifi->ifi_name);
        if (ifi->ifi_flags & IFF_UP) printf("UP ");
        if (ifi->ifi_flags & IFF_BROADCAST) printf("BCAST ");
        if (ifi->ifi_flags & IFF_MULTICAST) printf("MCAST ");
        if (ifi->ifi_flags & IFF_LOOPBACK) printf("LOOP ");
        if (ifi->ifi_flags & IFF_POINTOPOINT) printf("P2P ");
        printf(">\n");
        
        if ((i = ifi->ifi_hlen) > 0) {
            ptr = ifi->ifi_haddr;
            do {
                printf("%s%x", (i == ifi->ifi_hlen) ? " " : ":", *ptr++);
                
            } while (--i != 0);
            printf("\n");
        }
        
        if (ifi->ifi_mtu != 0) {
            printf("  MTU: %d\n", ifi->ifi_mtu);
        }
        
        if ((sa = ifi->ifi_addr) != NULL)
            printf("  IP addr: %s\n", Sock_ntop_host(sa, sizeof(*sa)));
        
        if ((sa = ifi->ifi_brdaddr) != NULL)
            printf("  broadcast addr: %s\n", Sock_ntop_host(sa, sizeof(*sa)));
        
        if ((sa = ifi->ifi_dstaddr) != NULL)
            printf("  destination addr: %s\n", Sock_ntop_host(sa, sizeof(*sa)));
    }
    
    free_ifi_info2(ifihead);
}