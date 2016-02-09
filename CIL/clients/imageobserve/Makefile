CC = cc
CFLAGS = -O -I../../include -I/common/include  -I/usr/X11R6/include #-Wall 

BINDIR = /usr/local/bin
LIBDIR = ../../lib

INC = /usr/local/include

PROG1 = imageobserve
OBJS1 = main.o init.o exec.o item.o text.o



all : ${PROG1}

${PROG1}: ${OBJS1}
	${CC} -o ${PROG1} ${OBJS1} -L${LIBDIR} -lcil -lXcil -lcilext -L/usr/X11R6/lib -lX11 -lm

clean :
	rm -f *~ a.out core

allclean :
	rm -f ${PROG1}
	rm -f *~ *.o a.out core

aclean :
	rm -f ${PROG1}

install : ${PROG1}
	install ${PROG1} ${BINDIR}
