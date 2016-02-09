PROG = dataplot
CC = cc
CFLAGS = -O -I/home/abe/common/include -I/usr/openwin/include

LIB = /home/abe/common/src/CIL/lib
DEST = /home/abe/common/bin/sun4

OBJS = main.o init.o load.o open.o disp.o exec.o close.o exit.o


${PROG}:${OBJS}
	cc -o ${PROG} ${OBJS} -L${LIB} -lcil -L/home/abe/common/lib/sun4 -lXw -lX11 -lm

static:${OBJS}
	cc -o ${PROG} ${OBJS} -Bstatic -L${LIB} -lcil -L/home/abe/common/lib/sun4 -lXw -Bdynamic -lX11 -lm

clean:
	rm -f *~ core

allclean:
	rm -f *~ *.o dataplot core

install:
	install ${PROG} ${DEST}

