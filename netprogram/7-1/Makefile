CFLAGS = -I/Users/tomxing/Documents/unixnetworkprogram/lib -g -O2 -D_REENTRANT -Wall
LIBS = /Users/tomxing/Documents/unixnetworkprogram/libunp.a -lresolv -lpthread


daytimeserver:	daytimeserver.c
	${CC} ${CFLAGS} -o $@ daytimeserver.c ${LIBS}

daytimeclient: daytimeclient.c
	${CC} ${CFLAGS} -o $@ daytimeclient.c ${LIBS}