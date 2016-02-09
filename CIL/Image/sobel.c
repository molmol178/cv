/*
 * sobel.c
 */



#include "Funcs.h"



#define DEFINE_SOBEL_1D(TYPE_NAME,TYPE)\
static void sobel_##TYPE(dest,src,th)\
     register image dest;\
     register image src;\
     long th;\
{\
  static long sobel[3][3] = {{-1,0,1},\
			     {-2,0,2},\
			     {-1,0,1}};\
  register long x,y;\
\
  image__type_check("image__sobel",src,TYPE_NAME);\
\
  Image.make(dest,Short2,src->xsize,src->ysize);\
\
  for (y = 1; y < src->ysize - 1; y++)\
    for (x = 1; x < src->xsize - 1; x++)\
      {\
	long lx,ly;\
	long xv = 0,yv = 0;\
\
	for (ly = -1; ly <= 1; ly++)\
	  for (lx = -1; lx <= 1; lx++)\
	    {\
	      xv += __PIXEL( src, x + lx, y + ly, TYPE ) * sobel[ly+1][lx+1];\
	      yv += __PIXEL( src, x + lx, y + ly, TYPE ) * sobel[lx+1][ly+1];\
	    }\
\
	__PIXEL0( dest, x, y, short2 ) = (abs(xv) > th) ? xv : 0;\
	__PIXEL1( dest, x, y, short2 ) = (abs(yv) > th) ? yv : 0;\
      }\
}



#define DEFINE_SOBEL_2D(TYPE_NAME,TYPE)\
static void sobel_short2(dest,src,th)\
     register image dest;\
     register image src;\
     long th;\
{\
  static long sobel[3][3] = {{-1,0,1},\
			     {-2,0,2},\
			     {-1,0,1}};\
  register long x,y;\
\
  image__type_check("image__sobel",src,TYPE_NAME);\
\
  Image.make(dest,Short2,src->xsize,src->ysize);\
\
  for (y = 1; y < src->ysize - 1; y++)\
    for (x = 1; x < src->xsize - 1; x++)\
      {\
	long lx,ly;\
	long xv = 0,yv = 0;\
\
	for (ly = -1; ly <= 1; ly++)\
	  for (lx = -1; lx <= 1; lx++)\
	    {\
	      xv += __PIXEL0(src,x+lx,y+ly,TYPE) * sobel[ly+1][lx+1];\
	      yv += __PIXEL1(src,x+lx,y+ly,TYPE) * sobel[lx+1][ly+1];\
	    }\
\
	__PIXEL0(dest,x,y,short2) = (abs(xv) > th) ? xv : 0;\
	__PIXEL1(dest,x,y,short2) = (abs(yv) > th) ? yv : 0;\
      }\
}



static long abs_max(a,b,c)
     long a,b,c;
{
  long ab,ac;
  long mm = 0;

  ab = abs(b);
  ac = abs(c);

  mm = abs(a);
  if (mm < ab) mm = ab;
  if (mm < ac) mm = ac;

  return mm;
}

#define DEFINE_SOBEL_3D(TYPE_NAME,TYPE)\
static void sobel_##TYPE(dest,src,th)\
     register image dest;\
     register image src;\
     long th;\
{\
  static long sobel[3][3] = {{-1,0,1},\
			     {-2,0,2},\
			     {-1,0,1}};\
  register long x,y;\
\
  image__type_check("image__sobel",src,TYPE_NAME);\
\
  Image.make(dest,Short2,src->xsize,src->ysize);\
\
  for (y = 1; y < Image.ysize(src) - 1; y++)\
    for (x = 1; x < Image.xsize(src) - 1; x++)\
      {\
	long lx,ly;\
	long xv0 = 0,yv0 = 0;\
	long xv1 = 0,yv1 = 0;\
	long xv2 = 0,yv2 = 0;\
	long xvn = 0,yvn = 0;\
	long xv = 0,yv = 0;\
\
	for (ly = -1; ly <= 1; ly++)\
	  for (lx = -1; lx <= 1; lx++)\
	    {\
	      TYPE *p = __PIXELPTR(src,x+lx,y+ly,TYPE);\
	      xv0 += p->at[0] * sobel[ly+1][lx+1];\
	      yv0 += p->at[0] * sobel[lx+1][ly+1];\
	      xv1 += p->at[1] * sobel[ly+1][lx+1];\
	      yv1 += p->at[1] * sobel[lx+1][ly+1];\
	      xv2 += p->at[2] * sobel[ly+1][lx+1];\
	      yv2 += p->at[2] * sobel[lx+1][ly+1];\
	      xvn += (xv0 + xv1 + xv2);\
	      yvn += (yv0 + yv1 + yv2);\
	    }\
\
	xv = abs_max(xv0,xv1,xv2);\
	yv = abs_max(yv0,yv1,yv2);\
\
	if (xvn < 0) xv = - xv;\
	if (yvn < 0) yv = - yv;\
\
	__PIXEL0(dest,x,y,short2) = (abs(xv) > th) ? xv : 0;\
	__PIXEL1(dest,x,y,short2) = (abs(yv) > th) ? yv : 0;\
      }\
}



DEFINE_SOBEL_1D(UChar,uchar)
DEFINE_SOBEL_2D(Short2,short2)
DEFINE_SOBEL_3D(UChar3,uchar3)



typedef void (*f_caller) P ((image dest,image src,long th));



static caller_table table[] = {
  {UChar, sobel_uchar},
  {UChar3,sobel_uchar3},
  {Short2,sobel_short2}
};



void image__sobel(dst,src,th)
     image dst;
     image src;
     long th;
{
  f_caller caller;

  caller = (f_caller)image__caller("image__sobel",src,Number(table),table);

  if (caller) caller(dst,src,th);
}
