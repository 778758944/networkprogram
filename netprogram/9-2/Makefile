CFLAGS = -I/Users/wentaoxing/Documents/unixnetworkprogram/lib -g -O2 -D_REENTRANT -Wall
LIBS = /Users/wentaoxing/Documents/unixnetworkprogram/libunp.a -lresolv -lpthread


daytimeserv: daytimeserv.c
	${CC} ${CFLAGS} -o $@ daytimeserv.c ../lib/libnet.c ${LIBS}

daytimeudpserv: daytimeudpserv.c libnet.c
	${CC} ${CFLAGS} -o $@ daytimeudpserv.c libnet.c ${LIBS}

echo_select_server: echo_select_server.c
	${CC} ${CFLAGS} -o $@ echo_select_server.c ${LIBS}

echo_poll_server: echo_poll_server.c
	${CC} ${CFLAGS} -o $@ echo_poll_server.c ${LIBS}