//
//  printsadbmsg.h
//  netprogram
//
//  Created by WENTAO XING on 2019/4/8.
//  Copyright © 2019 Tom Xing. All rights reserved.
//

#ifndef printsadbmsg_h
#define printsadbmsg_h

#include "unp.h"
#include <net/pfkeyv2.h>

void print_sadb_msg(struct sadb_msg *msg, int msglen);

#endif /* printsadbmsg_h */
