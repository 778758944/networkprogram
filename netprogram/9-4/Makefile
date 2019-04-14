CFLAGS = -I/Users/wentaoxing/Documents/unixnetworkprogram/lib -g -O2 -D_REENTRANT -Wall
LIBS = /Users/wentaoxing/Documents/unixnetworkprogram/libunp.a -lresolv -lpthread


unixserv: unixserv.c
	${CC} ${CFLAGS} -o $@ unixserv.c ${LIBS}

unixcli: unixcli.c
	${CC} ${CFLAGS} -o $@ unixcli.c ${LIBS}

unixudpserv: unixudpserv.c
	${CC} ${CFLAGS} -o $@ unixudpserv.c ${LIBS}

unixudpcli: unixudpcli.c
	${CC} ${CFLAGS} -o $@ unixudpcli.c ${LIBS}
