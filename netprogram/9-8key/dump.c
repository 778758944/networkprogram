//
//  dump.c
//  netprogram
//
//  Created by WENTAO XING on 2019/4/8.
//  Copyright © 2019 Tom Xing. All rights reserved.
//
#include "printsadbmsg.h"
#include "name.h"

void sadb_dump(int type) {
    int s;
    char buf[4096];
    struct sadb_msg msg;
    int goteof;
    
    s = Socket(PF_KEY, SOCK_RAW, PF_KEY_V2);
    
    bzero(&msg, sizeof(msg));
    msg.sadb_msg_version = PF_KEY_V2;
    msg.sadb_msg_type = SADB_DUMP;
    msg.sadb_msg_satype = type;
    msg.sadb_msg_len = sizeof(msg) / 8;
    msg.sadb_msg_pid = getpid();
    
    printf("sending dump message: \n");
    print_sadb_msg(&msg, sizeof(msg));
    Write(s, &msg, sizeof(msg));
    printf("\nMessage returned: \n");
    
    goteof = 0;
    while (goteof == 0) {
        int msglen;
        struct  sadb_msg * msgp;
        msglen = (int) Read(s, &buf, sizeof(buf));
        msgp = (struct sadb_msg *) &buf;
        print_sadb_msg(msgp, msglen);
        if (msgp->sadb_msg_seq == 0) goteof = 1;
    }
    
    close(s);
}

int main(int argc, char **argv) {
    int satype = SADB_SATYPE_UNSPEC;
    int c;
    opterr = 0;
    while ((c = getopt(argc, argv, "t: ")) != -1) {
        switch (c) {
            case 't':
                if ((satype = getsatypebyname(optarg)) == -1) {
                    err_quit("invalid -t option %s\n", optarg);
                }
                break;
                
            default:
                err_quit("unrecognized option: %c", c);
        }
    }
    
    sadb_dump(satype);
}
