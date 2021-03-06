/*
 * laplacian4.c
 */



#include "Funcs.h"


#define DEFINE_LAPLACIAN8_1D(TYPE_NAME,TYPE)\
static void laplacian8_##TYPE(dest,src,th)\
     image dest;\
     image src;\
     long th;\
{\
  static long laplacian8[3][3] = {{-1,-1,-1},\
				  {-1, 8,-1},\
				  {-1,-1,-1}};\
  register TYPE **src_data;\
  register short **dest_data;\
  register long x,y;\
\
  image__type_check("image__laplacian8",src,TYPE_NAME);\
\
  Image.make(dest,Short,Image.xsize(src),Image.ysize(src));\
\
  src_data = (TYPE **)Image.data(src);\
  dest_data = (short **)Image.data(dest);\
\
  for (y = 1; y < Image.ysize(src) - 1; y++)\
    for (x = 1; x < Image.xsize(src) - 1; x++)\
      {\
	long lx,ly;\
	long v = 0;\
\
	for (ly = -1; ly <= 1; ly++)\
	  for (lx = -1; lx <= 1; lx++)\
	    v += src_data[y+ly][x+lx] * laplacian8[ly+1][lx+1];\
\
	dest_data[y][x] = (abs(v) > th) ? v : 0;\
      }\
}



#define DEFINE_LAPLACIAN8_3D(TYPE_NAME,TYPE)\
static void laplacian8_uchar3(dest,src,th)\
     image dest;\
     image src;\
     long th;\
{\
  static long laplacian8[3][3] = {{-1,-1,-1},\
				  {-1, 8,-1},\
				  {-1,-1,-1}};\
  register TYPE **src_data;\
  register short **dest_data;\
  register long x,y;\
\
  image__type_check("image__laplacian8",src,TYPE_NAME);\
\
  Image.make(dest,Short,Image.xsize(src),Image.ysize(src));\
\
  src_data = (TYPE **)Image.data(src);\
  dest_data = (short **)Image.data(dest);\
\
  for (y = 1; y < Image.ysize(src) - 1; y++)\
    for (x = 1; x < Image.xsize(src) - 1; x++)\
      {\
	long lx,ly;\
	long v0 = 0,v1 = 0,v2 = 0;\
	long a0 = 0,a1 = 0,a2 = 0;\
\
	for (ly = -1; ly <= 1; ly++)\
	  for (lx = -1; lx <= 1; lx++)\
	    {\
	      v0 += src_data[y+ly][x+lx].at[0] * laplacian8[ly+1][lx+1];\
	      v1 += src_data[y+ly][x+lx].at[1] * laplacian8[ly+1][lx+1];\
	      v2 += src_data[y+ly][x+lx].at[2] * laplacian8[ly+1][lx+1];\
	    }\
	a0 = abs(v0);\
	a1 = abs(v1);\
	a2 = abs(v2);\
	if (a0 < a1) a0 = a1;\
	if (a0 < a2) a0 = a2;\
	if ((v0 + v1 + v2) < 0) a0 = -a0;\
	dest_data[y][x] = (abs(a0) > th) ? a0 : 0;\
      }\
}



DEFINE_LAPLACIAN8_1D(UChar,uchar)
DEFINE_LAPLACIAN8_3D(UChar3,uchar3)



typedef void (*f_caller) P ((image dest,image src,long th));



static caller_table table[] = {
  {UChar, laplacian8_uchar},
  {UChar3,laplacian8_uchar3}
};



void image__laplacian8(dst,src,th)
     image dst;
     image src;
     long th;
{
  f_caller caller;

  caller = (f_caller)image__caller("image__laplacian8",src,Number(table),table);

  if (caller) caller(dst,src,th);
}
