//
//  unpifi.c
//  netprogram
//
//  Created by WENTAO XING on 2019/4/2.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#ifndef __unp_ifi_h
#define __unp_ifi_h

#include "unp.h"
#include <net/if.h>

#define IFI_NAME 16
#define IFI_HADDR 8

struct ifi_info {
    char ifi_name[IFI_NAME];
    short ifi_index;
    short ifi_mtu;
    u_char ifi_haddr[IFI_HADDR];
    u_short ifi_hlen;
    short ifi_flags;
    short ifi_myflags;
    struct sockaddr *ifi_addr; /*primary address*/
    struct sockaddr *ifi_brdaddr; /*broadcast address*/
    struct sockaddr *ifi_dstaddr; /*destination address*/
    struct ifi_info * ifi_next; /*next*/
};

#define IFI_ALIAS 1
struct ifi_info *get_ifi_info2(int, int);
struct ifi_info *Get_ifi_info2(int, int);

void free_ifi_info2(struct ifi_info *);

#endif
