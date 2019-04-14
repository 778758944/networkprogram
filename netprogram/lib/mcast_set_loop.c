//
//  mcast_set_loop.c
//  netprogram
//
//  Created by WENTAO XING on 2019/4/11.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "./libnet.h"

int mcast_set_loop2(int sockfd, int onoff) {
    switch (sockfd_to_family(sockfd)) {
        case AF_INET: {
            unsigned char flag;
            flag = onoff;
            return setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP, &flag, sizeof(flag));
        }
#ifdef IPV6
        case AF_INET6: {
            unsigned flag;
            flag = onoff;
            return setsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &flag, sizeof(flag));
        }
#endif
        default:
            errno = EAFNOSUPPORT;
            return -1;
    }
}
