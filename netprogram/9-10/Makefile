#CFLAGS = -I/Users/tomxing/Documents/unixnetworkprogram/lib -g -O2 -D_REENTRANT -Wall
#LIBS = /Users/tomxing/Documents/unixnetworkprogram/libunp.a -lresolv -lpthread
CFLAGS = -I/Users/wentaoxing/Documents/unixnetworkprogram/lib -g -O2 -D_REENTRANT -Wall
LIBS = /Users/wentaoxing/Documents/unixnetworkprogram/libunp.a -lresolv -lpthread

myifconfig: prifinfo.c get_ifi_info.c
	${CC} ${CFLAGS} -o $@ prifinfo.c get_ifi_info.c ${LIBS}

web: web.c home_page.c start_connect.c write_get_cmd.c ../lib/libnet.c
	${CC} ${CFLAGS} -o $@ web.c home_page.c start_connect.c write_get_cmd.c ../lib/libnet.c ${LIBS}

dgserv: dgserv.c
	${CC} ${CFLAGS} -o $@ dgserv.c ../lib/libnet.c ${LIBS}

echo_client: tcpcli03.c
	${CC} ${CFLAGS} -o $@ tcpcli03.c ../lib/libnet.c ${LIBS}
