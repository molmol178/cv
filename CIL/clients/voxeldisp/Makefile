CC = cc
CFLAGS = -O -I../../include -I/Users/sugi/Source/CIL/include  -I/usr/X11R6/include #-Wall 

BINDIR = ../../bin
LIBDIR = /Users/sugi/Source/CIL/lib

INC = ../../include

PROG1 = voxeldisp
OBJS1 = \
	main.o initialize.o calculate.o xevent.o\
	open_dialog.o ps.o




all : ${PROG1}

${PROG1}: ${OBJS1}
	${CC} -o ${PROG1} ${OBJS1} -L${LIBDIR} -lcil -lXcil -lcilext -L../../lib -L/usr/X11R6/lib -lX11 -lm -lcil

clean :
	rm -f *~ a.out core

allclean :
	rm -f ${PROG1}
	rm -f *~ *.o a.out core

aclean :
	rm -f ${PROG1}

install : ${PROG1}
	install ${PROG1} ${BINDIR}

