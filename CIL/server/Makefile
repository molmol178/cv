CC = gcc
CFLAGS = -O -I../include  -I/usr/X11R6/include #-Wall 



BINDIR = /common/bin
LIBDIR = ../lib



PROG1 = cilserver
OBJS1 = cilserver.o

PROG2 = cills
OBJS2 = cills.o

PROG3 = ciladd
OBJS3 = ciladd.o

PROG4 = cilrm
OBJS4 = cilrm.o

PROG5 = cilcp
OBJS5 = cilcp.o

PROG6 = cilview
OBJS6 = cilview.o



PROGS = ${PROG1} ${PROG2} ${PROG3} ${PROG4} ${PROG5} ${PROG6}



all : ${PROGS}



${PROG1}: ${OBJS1} ${LIBDIR}/libcil.a
	gcc -o ${PROG1} ${OBJS1} -L${LIBDIR} -lcil -L/usr/X11R6/lib -lX11 #-lsocket

${PROG2}: ${OBJS2} ${LIBDIR}/libcil.a
	gcc -o ${PROG2} ${OBJS2} -L${LIBDIR} -lcil #-lsocket

${PROG3}: ${OBJS3} ${LIBDIR}/libcil.a
	gcc -o ${PROG3} ${OBJS3} -L${LIBDIR} -lcil #-lsocket

${PROG4}: ${OBJS4} ${LIBDIR}/libcil.a
	gcc -o ${PROG4} ${OBJS4} -L${LIBDIR} -lcil #-lsocket

${PROG5}: ${OBJS5} ${LIBDIR}/libcil.a
	gcc -o ${PROG5} ${OBJS5} -L${LIBDIR} -lcil #-lsocket

${PROG6}: ${OBJS6} ${LIBDIR}/libcil.a
	gcc -o ${PROG6} ${OBJS6} -L${LIBDIR} -lcil #-lsocket



clean :
	/bin/rm -f *~ a.out core

allclean :
	/bin/rm -f ${PROGS}
	/bin/rm -f *~ *.o a.out core

aclean :
	rm -f ${PROGS}

install : ${PROGS}
	install ${PROGS} ${BINDIR}
