#
# Makefile
#
# Author : Takahiro Sugiyama
# Date   : 1994/04/27
#

OBJS = main.o weight.o liner.o gauss.o arithmetic.o
EXEC = scaling
LIBS = -lcil -lcilext -lm

#

CFLAGS = -O  -I${INC}

#
# COMMON SETUP
#

LIB = ${HOME}/Source/CIL/lib
INC = ${HOME}/Source/CIL/include
BIN = /usr/local/bin

###

CC = cc

${EXEC} : ${OBJS}
	$(CC) -o $@ ${OBJS} -L${LIB} ${LIBS}

clean:
	rm -f *~ a.out core

aclean:
	rm -f *~ ${EXEC}

allclean:
	rm -f *~ *.o ${EXEC}


install: ${EXEC}
	install ${EXEC} ${BIN}
