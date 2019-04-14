//
//  get_ifi_info.c
//  netprogram
//
//  Created by WENTAO XING on 2019/4/3.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "../9-6/unpifi.h"
#include "unproute.h"

struct ifi_info * get_ifi_info3(int family, int doaliases) {
    int flags;
    char *buf, *next, *lim;
    size_t len;
    struct if_msghdr * ifm;
    struct ifa_msghdr * ifam;
    struct sockaddr * sa, *rti_info[RTAX_MAX];
    struct sockaddr_dl *sdl;
    
    struct ifi_info *ifi, *ifisave, *ifihead, **ifipnext;
    
    buf = net_rt_iflist2(family, 0, &len);
    
    ifihead = NULL;
    ifipnext = &ifihead;
    
    lim = buf + len;
    
    for (next = buf; next < lim; next += ifm->ifm_msglen) {
        ifm = (struct if_msghdr *) next;
        if (ifm->ifm_type == RTM_IFINFO) {
            if (((flags = ifm->ifm_flags) & IFF_UP) == 0) {
                continue;
            }
            
            sa = (struct sockaddr *) ifm + 1;
            get_rtaddrs2(ifm->ifm_addrs, sa, rti_info);
            if ((sa = rti_info[RTAX_IFP]) != NULL) {
                ifi = Calloc(1, sizeof(struct ifi_info));
                *ifipnext = ifi;
                ifipnext = &ifi->ifi_next;
                
                ifi->ifi_flags = flags;
                if (sa->sa_family == AF_LINK) {
                    sdl = (struct sockaddr_dl *) sa;
                    ifi->ifi_index = sdl->sdl_index;
                    if (sdl->sdl_nlen > 0) {
                        snprintf(ifi->ifi_name, IFI_NAME, "%*s", sdl->sdl_nlen, &sdl->sdl_data[0]);
                    } else {
                        snprintf(ifi->ifi_name, IFI_NAME, "index %d", sdl->sdl_index);
                    }
                    
                    if ((ifi->ifi_hlen = sdl->sdl_alen) > 0) {
                        memcpy(ifi->ifi_haddr, LLADDR(sdl), min(IFI_HADDR, sdl->sdl_alen));
                    }
                }
            }
        } else if (ifm->ifm_type == RTM_NEWADDR) {
            if (ifi->ifi_addr) {
//                if (doal)
            }
        }
    }
    
    return ifihead;
}
