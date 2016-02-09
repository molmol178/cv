CC = cc
CFLAGS = -O -I../../include -I/common/include  -I/usr/X11R6/include #-Wall 

BINDIR = ../../bin
LIBDIR = ../../lib

INC = ../..//include

PROG1 = imagedisp
OBJS1 = \
	main.o \
	initialize.o \
	load.o \
	open.o \
	disp.o \
	exec.o \
	destrize.o \
	server.o



all : ${PROG1}

${PROG1}: ${OBJS1} ${LIBDIR}/libcil.a ${LIBDIR}/libXcil.a ${LIBDIR}/libcilext.a
	cc -o ${PROG1} ${OBJS1} -L${LIBDIR} -lcil -lXcil -lcilext -L/usr/X11R6/lib -lX11 -lcil

clean :
	rm -f *~ a.out core

allclean :
	rm -f ${PROG1}
	rm -f *~ *.o a.out core

aclean :
	rm -f ${PROG1}

install : ${PROG1}
	install ${PROG1} ${BINDIR}
