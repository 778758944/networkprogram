CFLAGS = -I/Users/wentaoxing/Documents/unixnetworkprogram/lib -g -O2 -D_REENTRANT -Wall
LIBS = /Users/wentaoxing/Documents/unixnetworkprogram/libunp.a -lresolv -lpthread


echo_server: echo_server.c
	${CC} ${CFLAGS} -o $@ echo_server.c ${LIBS}

echo_client: echo_client.c
	${CC} ${CFLAGS} -o $@ echo_client.c ${LIBS}

echo_select_server: echo_select_server.c
	${CC} ${CFLAGS} -o $@ echo_select_server.c ${LIBS}

echo_poll_server: echo_poll_server.c
	${CC} ${CFLAGS} -o $@ echo_poll_server.c ${LIBS}