CFLAGS = -I/Users/wentaoxing/Documents/unixnetworkprogram/lib -g -O2 -D_REENTRANT -Wall
LIBS = /Users/wentaoxing/Documents/unixnetworkprogram/libunp.a -lresolv -lpthread


#CFLAGS = -I/Users/tomxing/Documents/unixnetworkprogram/lib -g -O2 -D_REENTRANT -Wall
#LIBS = /Users/tomxing/Documents/unixnetworkprogram/libunp.a -lresolv -lpthread

daytimeserv: daytimeserv.c libnet.c
	${CC} ${CFLAGS} -o $@ daytimeserv.c libnet.c ${LIBS}

daytimeudpserv: daytimeudpserv.c libnet.c
	${CC} ${CFLAGS} -o $@ daytimeudpserv.c libnet.c ${LIBS}


daytimeudpcli: daytimeudpcli.c libnet.c
	${CC} ${CFLAGS} -o $@ daytimeudpcli.c libnet.c ${LIBS}

daytimecli: daytimecli.c libnet.c
	${CC} ${CFLAGS} -o $@ daytimecli.c libnet.c ${LIBS}

echo_poll_server: echo_poll_server.c
	${CC} ${CFLAGS} -o $@ echo_poll_server.c ${LIBS}
