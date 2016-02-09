CC = cc
CFLAGS = -O -I${INC} -I/usr/openwin/include
LIB = /home/abe/common/lib/sun4
INC = /home/abe/common/include

OBJS = \
	xrgb.o \
	open.o \
	disp.o \
	exec.o \
	close.o

PROG = imagergb

${PROG}: $(OBJS)
	cc -o ${PROG} ${OBJS} -L${LIB} -lcil -lXcil -lX11 -lm

static: $(OBJS)
	cc -o ${PROG} ${OBJS} -Bstatic -L${LIB} -lcil -lXw -Bdynamic -lX11 -lm

clean :
	rm -f *~ core

allclean :
	rm -f ${PROG} *~ *.o core
