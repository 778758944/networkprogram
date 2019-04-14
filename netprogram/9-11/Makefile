CFLAGS = -I/Users/wentaoxing/Documents/unixnetworkprogram/lib -g -O2 -D_REENTRANT -Wall
LIBS = /Users/wentaoxing/Documents/unixnetworkprogram/libunp.a -lresolv -lpthread

dgserv: dgserv.c ../lib/mcast_join.c
	${CC} ${CFLAGS} -o $@ dgserv.c ../lib/mcast_join.c ../lib/mcast_set_loop.c ${LIBS}
