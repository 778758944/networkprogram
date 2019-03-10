#CFLAGS = -I/Users/wentaoxing/Documents/unixnetworkprogram/lib -g -O2 -D_REENTRANT -Wall
#LIBS = /Users/wentaoxing/Documents/unixnetworkprogram/libunp.a -lresolv -lpthread

CFLAGS = -I/Users/tomxing/Documents/unixnetworkprogram/lib -g -O2 -D_REENTRANT -Wall
LIBS = /Users/tomxing/Documents/unixnetworkprogram/libunp.a -lresolv -lpthread

testconnect: testconnect.c ../lib/libnet.c
	${CC} ${CFLAGS} -o $@ testconnect.c ../lib/libnet.c ${LIBS}

daytimeclio: daytimeclio.c ../lib/libnet.c
	${CC} ${CFLAGS} -o $@ daytimeclio.c ../lib/libnet.c ${LIBS}

echo_server: echo_server.c
	${CC} ${CFLAGS} -o $@ echo_server.c ../lib/libnet.c ${LIBS}

echo_client: echo_client.c
	${CC} ${CFLAGS} -o $@ echo_client.c ../lib/libnet.c ${LIBS}