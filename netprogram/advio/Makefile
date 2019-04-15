CFLAGS = -I/Users/tomxing/Documents/unixnetworkprogram/lib -g -O2 -D_REENTRANT -Wall
LIBS = /Users/tomxing/Documents/unixnetworkprogram/libunp.a -lresolv -lpthread

main: main.c dgechoaddr.c recvfromflags.c
	${CC} ${CFLAGS} -o $@ main.c dgechoaddr.c recvfromflags.c ../lib/libnet.c ${LIBS}
