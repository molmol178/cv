/*
 * median.c
 */



#include "Funcs.h"



#define DEFINE_MEDIAN_1D(TYPE_NAME,TYPE)\
static int cmp_##TYPE(a,b) TYPE *a,*b; {return *a - *b;}\
static void median_##TYPE(dest,src,median)\
     image dest;\
     image src;\
     register long median;\
{\
  register TYPE **src_data;\
  register TYPE **dest_data;\
  register long x,y;\
  register long xsize,ysize;\
  TYPE pix[9];\
\
  image__type_check("image__median",src,TYPE_NAME);\
\
  xsize = Image.xsize(src);\
  ysize = Image.ysize(src);\
\
  Image.make(dest,TYPE_NAME,xsize,ysize);\
\
  src_data = (TYPE **)Image.data(src);\
  dest_data = (TYPE **)Image.data(dest);\
\
  for (y = 1; y < ysize - 1; y++)\
    for (x = 1; x < xsize - 1; x++)\
      {\
	pix[0] = src_data[y-1][x-1];\
	pix[1] = src_data[y-1][x];\
	pix[2] = src_data[y-1][x+1];\
	pix[3] = src_data[y][x-1];\
	pix[4] = src_data[y][x];\
	pix[5] = src_data[y][x+1];\
	pix[6] = src_data[y+1][x-1];\
	pix[7] = src_data[y+1][x];\
	pix[8] = src_data[y+1][x+1];\
	qsort(pix,9,sizeof(TYPE),cmp_##TYPE);\
	dest_data[y][x] = pix[median];\
      }\
\
  for (x = 0; x < xsize; x++)\
    {\
      dest_data[0][x] = dest_data[1][x];\
      dest_data[ysize-1][x] = dest_data[ysize-2][x];\
    }\
  for (y = 0; y < ysize; y++)\
    {\
      dest_data[y][0] = src_data[y][1];\
      dest_data[y][xsize-1] = src_data[y][xsize-2];\
    }\
  dest_data[0][0] = dest_data[1][1];\
  dest_data[ysize-1][0] = dest_data[ysize-2][1];\
  dest_data[ysize-1][xsize-1] = dest_data[ysize-2][xsize-1];\
  dest_data[0][xsize-1] = dest_data[1][xsize-1];\
}



#define DEFINE_MEDIAN_3D(TYPE_NAME,TYPE)\
static int cmp0_##TYPE(a,b) TYPE *a,*b; {return a->at[0] - b->at[0];}\
static int cmp1_##TYPE(a,b) TYPE *a,*b; {return a->at[1] - b->at[1];}\
static int cmp2_##TYPE(a,b) TYPE *a,*b; {return a->at[2] - b->at[2];}\
static void median_##TYPE(dest,src,median)\
     image dest;\
     image src;\
     long median;\
{\
  register TYPE **src_data;\
  register TYPE **dest_data;\
  register long x,y;\
  TYPE pix[9];\
  long xsize,ysize;\
\
  image__type_check("image__median",src,TYPE_NAME);\
\
  xsize = Image.xsize(src);\
  ysize = Image.ysize(src);\
\
  Image.make(dest,TYPE_NAME,xsize,ysize);\
\
  src_data = (TYPE **)Image.data(src);\
  dest_data = (TYPE **)Image.data(dest);\
\
  for (y = 1; y < ysize - 1; y++)\
    for (x = 1; x < xsize - 1; x++)\
      {\
	pix[0] = src_data[y-1][x-1];\
	pix[1] = src_data[y-1][x];\
	pix[2] = src_data[y-1][x+1];\
	pix[3] = src_data[y][x-1];\
	pix[4] = src_data[y][x];\
	pix[5] = src_data[y][x+1];\
	pix[6] = src_data[y+1][x-1];\
	pix[7] = src_data[y+1][x];\
	pix[8] = src_data[y+1][x+1];\
\
	qsort(pix,9,sizeof(TYPE),cmp0_##TYPE);\
	dest_data[y][x].at[0] = pix[median].at[0];\
\
	qsort(pix,9,sizeof(TYPE),cmp1_##TYPE);\
	dest_data[y][x].at[1] = pix[median].at[1];\
\
	qsort(pix,9,sizeof(TYPE),cmp2_##TYPE);\
	dest_data[y][x].at[2] = pix[median].at[2];\
      }\
\
  for (x = 0; x < xsize; x++)\
    {\
      dest_data[0][x] = dest_data[1][x];\
      dest_data[ysize-1][x] = dest_data[ysize-2][x];\
    }\
  for (y = 0; y < ysize; y++)\
    {\
      dest_data[y][0] = src_data[y][1];\
      dest_data[y][xsize-1] = src_data[y][xsize-2];\
    }\
  dest_data[0][0] = dest_data[1][1];\
  dest_data[ysize-1][0] = dest_data[ysize-2][1];\
  dest_data[ysize-1][xsize-1] = dest_data[ysize-2][xsize-1];\
  dest_data[0][xsize-1] = dest_data[1][xsize-1];\
}



DEFINE_MEDIAN_1D(UChar,uchar)
DEFINE_MEDIAN_1D(Short,short)
DEFINE_MEDIAN_1D(Long,long)
DEFINE_MEDIAN_1D(Float,float)
DEFINE_MEDIAN_3D(UChar3,uchar3)



typedef void (*f_caller) P ((image dest,image src,long median));



static caller_table table[] = {
  {UChar,  median_uchar},
  {Short,  median_short},
  {Long,   median_long},
  {Float,  median_float},
  {UChar3, median_uchar3}
};



void image__median(dst,src,median)
     image dst;
     image src;
     long median;
{
  f_caller caller;

  caller = (f_caller)image__caller("image__median",src,Number(table),table);

  if (caller) caller(dst,src,median);
}
