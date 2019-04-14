//
//  ntp.h
//  netprogram
//
//  Created by WENTAO XING on 2019/4/12.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#ifndef ntp_h
#define ntp_h

#include <stdint.h>

#define JAN_1970 2208988800UL

struct l_fixedpt {
    uint32_t int_part;
    uint32_t fraction;
};

struct s_fixedpt {
    uint16_t int_part;
    uint16_t fraction;
};

struct ntpdata {
    unsigned char status;
    unsigned char stratum;
    unsigned char ppoll;
    int precision: 8;
    struct s_fixedpt distance;
    struct s_fixedpt dispersion;
    uint32_t refid;
    struct l_fixedpt reftime;
    struct l_fixedpt org;
    struct l_fixedpt rec;
    struct l_fixedpt xmt;
};

#define VERSION_MASK 0x38
#define MODE_MASK 0x07
#define MODE_CLIENT 3
#define MODE_SERVER 4
#define MODE_BROADCAST 5






#endif /* ntp_h */
