CFLAGS = -I/Users/wentaoxing/Documents/unixnetworkprogram/lib -g -O2 -D_REENTRANT -Wall
LIBS = /Users/wentaoxing/Documents/unixnetworkprogram/libunp.a -lresolv -lpthread


echo_udp_server: echo_udp_server.c
	${CC} ${CFLAGS} -o $@ echo_udp_server.c ${LIBS}

echo_udp_client: echo_udp_client.c
	${CC} ${CFLAGS} -o $@ echo_udp_client.c ${LIBS}

echo_select_server: echo_select_server.c
	${CC} ${CFLAGS} -o $@ echo_select_server.c ${LIBS}

echo_poll_server: echo_poll_server.c
	${CC} ${CFLAGS} -o $@ echo_poll_server.c ${LIBS}
