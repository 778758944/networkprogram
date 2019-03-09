//
//  gethostbyname.c
//  netprogram
//
//  Created by WENTAO XING on 2019/3/5.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#include "unp.h"

int main(int argc, char ** argv) {
    char *ptr, **pptr;
    char str[INET_ADDRSTRLEN];
    
    struct hostent *hptr;
    
    while (--argc > 0) {
        ptr = *(++argv);
        if ((hptr = gethostbyname(ptr)) == NULL) {
            err_msg("gethostbyname error for host: %s: %s\n", ptr, hstrerror(h_errno));
        }
        
        printf("offical hostname: %s\n", hptr->h_name);
        
        for (pptr = hptr->h_aliases; *pptr != NULL; pptr++) {
            printf("talias: %s\n", *pptr);
        }
        
        switch(hptr->h_addrtype) {
                case AF_INET:
                    for (pptr = hptr->h_addr_list; *pptr != NULL; ++pptr) {
                        inet_ntop(AF_INET, *pptr, str, sizeof(str));
                        printf("addr: %s\n", str);
                    }
                    break;
                
                default:
                    err_ret("unknown address type");
                    break;
        }
    }
    exit(0);
}
