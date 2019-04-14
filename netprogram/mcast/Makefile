CFLAGS = -I/Users/wentaoxing/Documents/unixnetworkprogram/lib -g -O2 -D_REENTRANT -Wall
LIBS = /Users/wentaoxing/Documents/unixnetworkprogram/libunp.a -lresolv -lpthread

main: main.c ../lib/mcast_join.c ../lib/mcast_set_loop.c 
	${CC} ${CFLAGS} -o $@ main.c send.c recv.c ../lib/libnet.c ../lib/mcast_join.c ../lib/mcast_set_loop.c ${LIBS}
