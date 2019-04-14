CFLAGS = -I/Users/wentaoxing/Documents/unixnetworkprogram/lib -g -O2 -D_REENTRANT -Wall
LIBS = /Users/wentaoxing/Documents/unixnetworkprogram/libunp.a -lresolv -lpthread


daytimeserver:	daytimeserver.c
	${CC} ${CFLAGS} -o $@ daytimeserver.c ${LIBS}

daytimeclient: daytimeclient.c
	${CC} ${CFLAGS} -o $@ daytimeclient.c ${LIBS}
