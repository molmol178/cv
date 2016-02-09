#============================================================
# CIL LIBRARIES MAKEING MAKEFILE
#	author	: Takahiro Sugiyama
#	date	: Project Start : 1992/04/01
#	date	: from 1993/04/01
#	date	: 1993/10/14
#	date	: 1995/12/01 for display property
#	date	: 1997/02/06 for labelling routines
#	date	: 2000/10/09 for Vine Linux PPC
#	date	: 2000/11/04 for MacOS X
#============================================================



#============================================================
# BOOTS DEFINE
#============================================================
CC = cc

CFLAGS = -g -Wall  -Iinclude -I/usr/X11R6/include

RM	= rm -f
AR	= ar cr
ARSA	= ar clq
RANLIB	= ranlib
LD	= ld

.c.o :
	${CC} ${CFLAGS} -o $*.o -c $*.c
# for shared library
#	${CC} -pic -c ${CFLAGS} $*.c -o shared/$*.o

#============================================================
# NAMES DEFINE
#============================================================
LIBNAME = cil
CILREV = 1.0
DEST = /usr/local/lib

CILLIBFN	= lib${LIBNAME}.a
CILLIB		= lib/${CILLIBFN}
CILLIBS		= ${CILLIB}

XCILLIBFN	= libX${LIBNAME}.a
XCILLIB		= lib/${XCILLIBFN}
XCILLIBS	= ${XCILLIB}

CILEXTLIBFN	= lib${LIBNAME}ext.a
CILEXTLIB	= lib/${CILEXTLIBFN}
CILEXTLIBS	= ${CILEXTLIB}

all: ${CILLIBS} ${XCILLIBS} ${CILEXTLIBS}
#all: ${CILLIBS} ${CILEXTLIBS}

#============================================================
# CIL LIBRARY MAKEING
#============================================================
CILOBJS = \
	ImageFile.o Image.o Voxel.o \
	ImageFile/FImage.o \
	ImageFile/pnm.o ImageFile/c2d.o ImageFile/gif.o ImageFile/j4.o \
	ImageFile/jpeg.o ImageFile/ps.o ImageFile/s3d.o ImageFile/tiff.o \
	ImageFile/viff.o ImageFile/xbm.o ImageFile/xwbitmap.o \
	ImageFile/xwd.o ImageFile/dib.o ImageFile/bmp.o \
	misc/optlib.o misc/memlib.o misc/shmemlib.o \
	misc/filelib.o misc/fileslib.o misc/debug.o misc/strlib.o \
	misc/typelib.o misc/timelib.o misc/socketlib.o 
#	ImageDisp.o server/cilclient.o
# 
CILSAOBJS = misc/debug.o
CILSOOBJS = ${CILOBJS}

${CILLIB} : ${CILOBJS}
	${RM} $@
	${AR} $@ ${CILOBJS}
	${RANLIB} $@

#============================================================
# X CIL LIBRARY MAKEING
#============================================================
XCILOBJS = \
	ImageShowing.o \
	Xcil/Xcil_lib.o Xcil/Xcil_event.o Xcil/Xcil_panel.o \
	Xcil/Xcil_text.o Xcil/Xcil_misc.o Xcil/Xcil_confirm.o Xcil/Xcil_hdr.o \
	Xcil/Xcil_file_dialog.o Xcil/Xcil_dialog.o Xcil/Xcil_menu.o\
	Xcil/Xcil_scroll.o Xcil/Xcil_popup.o Xcil/Xcil_string.o \
	Xcil/Xcil_text_loc.o \
	Xcil/XImage.o Xcil/XImageCG.o Xcil/ImageCG.o \
	Xcil/imgfont.o Xcil/xfont.o \
	Xcil/XImage1p.o Xcil/XImage8p.o Xcil/XImage16p.o Xcil/XImage15p.o Xcil/XImage24p.o  Xcil/XImage32p.o \
	Xcil/bit1.o Xcil/char.o Xcil/long.o Xcil/uchar.o Xcil/uchar3.o \
	Xcil/fontlib.o Xcil/font.o Xcil/cglib.o

${XCILLIB} : ${XCILOBJS}
	${RM} $@
	${AR} $@ ${XCILOBJS}
	${RANLIB} $@

#============================================================
# CIL EXTENTION LIBRARY MAKEING
#============================================================
CILEXTOBJS = \
	Image/nosupport.o Image/type-check.o Image/caller.o \
	Image/average.o Image/copyarea.o Image/median.o Image/labeling.o \
	Image/sobel.o Image/laplacian4.o Image/laplacian8.o \
	Image/separate2.o Image/fillmoat.o \
	Image/reverse.o Image/liner.o Image/longtype.o \
	Image/dilation.o Image/erosion.o Image/opening.o Image/closing.o \
	Image/and.o Image/or.o Image/eor.o \
	Image/gaussian.o Image/dgaussian.o Image/dgaussabs.o \
	Image/thresholding.o Image/histflat2.o \
	Image/fillin.o Image/binddata.o \
	Image/roundpix.o \
	Image/anytopnm.o \
	Image/filter.o Image/gauss_loc.o Image/filtering.o \
	Image/labelling.o Image/labelling4.o Image/labelling8.o \
	ColorImage/Abe.o ColorImage/Joblove.o ColorImage/Lab.o \
	ColorImage/Luv.o ColorImage/Ohta.o ColorImage/RGB_to.o \
	ColorImage/Smith.o ColorImage/Yagi.o ColorImage/distance.o

${CILEXTLIB} : ${CILEXTOBJS}
	${RM} $@
	${AR} $@ ${CILEXTOBJS}
	${RANLIB} $@

#============================================================
# CLEAN
#============================================================
allclean :
	${RM} ${CILLIBS}
	${RM} ${XCILLIBS}
	${RM} ${CILEXTLIBS}
	${RM} *~ */*~
	${RM} core a.out
	${RM} *.o */*.o

clean :
	${RM} *~ */*~
	${RM} core a.out

aclean :
	${RM} ${CILLIBS}
	${RM} ${XCILLIBS}
	${RM} ${CILEXTLIBS}
	${RM} *~ */*~
	${RM} core a.out



#============================================================
# INSTALL
#============================================================
install : all
	install ${CILLIBS} ${DEST}
	install ${XCILLIBS} ${DEST}
	install ${CILEXTLIBS} ${DEST}
	${RANLIB} ${DEST}/${CILLIBFN}
	${RANLIB} ${DEST}/${XCILLIBFN}
	${RANLIB} ${DEST}/${CILEXTLIBFN}
