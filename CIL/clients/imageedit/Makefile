CC = /opt/lang/acc
#CFLAGS = -g -I${INC} -I/usr/openwin/include -Wall #-O -fforce-mem -fforce-addr -fstrength-reduce -finline-functions -fomit-frame-pointer
CFLAGS = -I${INC} -I/usr/openwin/include -O

LIB  = /home/abe/common/lib/sun4
INC  = /home/abe/common/include
DEST = /home/abe/common/bin/sun4

OBJS = \
	Edit.o main.o init.o open.o exec.o close.o exit.o  \
	panel.o view.o \
	info.o pen.o pen_loc.o draw.o edit.o buffer.o \
	Bitmap.o imageItem.o local.o draw_obj.o imagefunc.o

PROG = imageedit

${PROG}: $(OBJS)
	${CC} -o ${PROG} ${OBJS} -L${LIB} -lcil -lcilext -lXcil -lXw -lX11 -lm

static: $(OBJS)
	${CC} -o ${PROG} ${OBJS} -Bstatic -L${LIB} -lcil -lcilext -lXw -Bdynamic -lX11 -lm

clean :
	rm -f *~ core

allclean :
	rm -f ${PROG} *~ *.o core

install : imageedit
	install ${PROG} ${DEST}

compress :
	lha a IMGED.LZH *.c *.h Makefile Bitmap
