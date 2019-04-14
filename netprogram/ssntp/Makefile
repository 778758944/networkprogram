CFLAGS = -I/Users/wentaoxing/Documents/unixnetworkprogram/lib -g -O2 -D_REENTRANT -Wall
LIBS = /Users/wentaoxing/Documents/unixnetworkprogram/libunp.a -lresolv -lpthread

ntp: ntp.c ../lib/mcast_join.c ../lib/mcast_set_loop.c sntp_proc.c
	${CC} ${CFLAGS} -o $@ ntp.c sntp_proc.c ../lib/libnet.c ../lib/mcast_join.c ../lib/mcast_set_loop.c ../9-6/get_ifi_info.c  ${LIBS}
