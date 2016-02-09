/*
 * fillmoat.c
 */



#include "Funcs.h"



#define DEFINE_FILLMOAT_1D(TYPE_NAME,TYPE)\
static void fillmoat_##TYPE(src,width)\
     image src;\
     long width;\
{\
  register TYPE **pp;\
  register long x,y;\
  long xsize,ysize;\
\
  image__type_check("image__fillmoat",src,TYPE_NAME);\
\
  pp = (TYPE **)Image.data(src);\
  xsize = Image.xsize(src);\
  ysize = Image.ysize(src);\
\
  for (y = width - 1; y >= 0 ; y--)\
    {\
      for (x = width+1; x < xsize - width - 1; x++)\
	pp[y][x] = (pp[y+1][x-1] + pp[y+1][x]+ pp[y+1][x+1]) / 3;\
      pp[y][width] = (pp[y+1][width+1] + pp[y+1][width]+ pp[y][width+1]) / 3;\
      pp[y][x] = (pp[y+1][x-1] + pp[y+1][x]+ pp[y][x-1]) / 3;\
    }\
\
  for (y = ysize - width; y < ysize ; y++)\
    {\
      for (x = width+1; x < xsize - width-1; x++)\
	pp[y][x] = (pp[y-1][x-1] + pp[y-1][x]+ pp[y-1][x+1]) / 3;\
      pp[y][width] = (pp[y-1][width+1] + pp[y-1][width]+ pp[y][width+1]) / 3;\
      pp[y][x] = (pp[y-1][x-1] + pp[y-1][x]+ pp[y][x-1]) / 3;\
    }\
\
  for (x = width - 1; x >= 0; x--)\
    {\
      for (y = 1; y < ysize - 1; y++)\
	pp[y][x] = (pp[y-1][x+1] + pp[y][x+1] + pp[y+1][x+1]) / 3;\
      pp[0][x] = (pp[0][x+1] + pp[1][x+1] + pp[1][x]) / 3;\
      pp[y][x] = (pp[y][x+1] + pp[y-1][x+1] + pp[y-1][x]) / 3;\
    }\
\
  for (x = xsize - width; x < xsize; x++)\
    {\
      for (y = 1; y < ysize-1; y++)\
	pp[y][x] = (pp[y-1][x-1] + pp[y][x-1] + pp[y+1][x-1]) / 3;\
      pp[0][x] = (pp[0][x-1] + pp[1][x-1] + pp[1][x]) / 3;\
      pp[y][x] = (pp[y][x-1] + pp[y-1][x-1] + pp[y-1][x]) / 3;\
    }\
}



#define DEFINE_FILLMOAT_3D(TYPE_NAME,TYPE) \
static void fillmoat_##TYPE(src,n) \
     image src; \
     long n; \
{ \
  register TYPE **p; \
  register long x,y; \
  long xsize,ysize; \
 \
  image__type_check("image__fillmoat",src,TYPE_NAME); \
 \
  p = (TYPE **)Image.data(src); \
  xsize = Image.xsize(src); \
  ysize = Image.ysize(src); \
 \
  for (y = n - 1; y >= 0 ; y--) { \
    for (x = n + 1; x < xsize - n - 1; x++) { \
      p[y][x].at[0]=(p[y+1][x-1].at[0]+p[y+1][x].at[0]+p[y+1][x+1].at[0])/3; \
      p[y][x].at[1]=(p[y+1][x-1].at[1]+p[y+1][x].at[1]+p[y+1][x+1].at[1])/3; \
      p[y][x].at[2]=(p[y+1][x-1].at[2]+p[y+1][x].at[2]+p[y+1][x+1].at[2])/3; \
    } \
    p[y][n].at[0]=(p[y+1][n+1].at[0]+p[y+1][n].at[0]+p[y][n+1].at[0])/3; \
    p[y][n].at[1]=(p[y+1][n+1].at[1]+p[y+1][n].at[1]+p[y][n+1].at[1])/3; \
    p[y][n].at[2]=(p[y+1][n+1].at[2]+p[y+1][n].at[2]+p[y][n+1].at[2])/3; \
    p[y][x].at[0]=(p[y+1][x-1].at[0]+p[y+1][x].at[0]+p[y][x-1].at[0])/3; \
    p[y][x].at[1]=(p[y+1][x-1].at[1]+p[y+1][x].at[1]+p[y][x-1].at[1])/3; \
    p[y][x].at[2]=(p[y+1][x-1].at[2]+p[y+1][x].at[2]+p[y][x-1].at[2])/3; \
  } \
 \
  for (y=ysize - n; y < ysize ; y++) { \
    for (x=n+1; x < xsize - n-1; x++) { \
      p[y][x].at[0]=(p[y-1][x-1].at[0]+p[y-1][x].at[0]+p[y-1][x+1].at[0])/3; \
      p[y][x].at[1]=(p[y-1][x-1].at[1]+p[y-1][x].at[1]+p[y-1][x+1].at[1])/3; \
      p[y][x].at[2]=(p[y-1][x-1].at[2]+p[y-1][x].at[2]+p[y-1][x+1].at[2])/3; \
    } \
    p[y][n].at[0]=(p[y-1][n+1].at[0]+p[y-1][n].at[0]+p[y][n+1].at[0])/3; \
    p[y][n].at[1]=(p[y-1][n+1].at[1]+p[y-1][n].at[1]+p[y][n+1].at[1])/3; \
    p[y][n].at[2]=(p[y-1][n+1].at[2]+p[y-1][n].at[2]+p[y][n+1].at[2])/3; \
    p[y][x].at[0]=(p[y-1][x-1].at[0]+p[y-1][x].at[0]+p[y][x-1].at[0])/3; \
    p[y][x].at[1]=(p[y-1][x-1].at[1]+p[y-1][x].at[1]+p[y][x-1].at[1])/3; \
    p[y][x].at[2]=(p[y-1][x-1].at[2]+p[y-1][x].at[2]+p[y][x-1].at[2])/3; \
  } \
 \
  for (x=n - 1; x >= 0; x--) { \
    for (y=1; y < ysize - 1; y++) { \
      p[y][x].at[0]=(p[y-1][x+1].at[0]+p[y][x+1].at[0]+p[y+1][x+1].at[0])/3; \
      p[y][x].at[1]=(p[y-1][x+1].at[1]+p[y][x+1].at[1]+p[y+1][x+1].at[1])/3; \
      p[y][x].at[2]=(p[y-1][x+1].at[2]+p[y][x+1].at[2]+p[y+1][x+1].at[2])/3; \
    } \
    p[0][x].at[0]=(p[0][x+1].at[0]+p[1][x+1].at[0]+p[1][x].at[0])/3; \
    p[0][x].at[1]=(p[0][x+1].at[1]+p[1][x+1].at[1]+p[1][x].at[1])/3; \
    p[0][x].at[2]=(p[0][x+1].at[2]+p[1][x+1].at[2]+p[1][x].at[2])/3; \
    p[y][x].at[0]=(p[y][x+1].at[0]+p[y-1][x+1].at[0]+p[y-1][x].at[0])/3; \
    p[y][x].at[1]=(p[y][x+1].at[1]+p[y-1][x+1].at[1]+p[y-1][x].at[1])/3; \
    p[y][x].at[2]=(p[y][x+1].at[2]+p[y-1][x+1].at[2]+p[y-1][x].at[2])/3; \
  } \
 \
  for (x=xsize - n; x < xsize; x++) { \
    for (y=1; y < ysize-1; y++) { \
      p[y][x].at[0]=(p[y-1][x-1].at[0]+p[y][x-1].at[0]+p[y+1][x-1].at[0])/3; \
      p[y][x].at[1]=(p[y-1][x-1].at[1]+p[y][x-1].at[1]+p[y+1][x-1].at[1])/3; \
      p[y][x].at[2]=(p[y-1][x-1].at[2]+p[y][x-1].at[2]+p[y+1][x-1].at[2])/3; \
    } \
    p[0][x].at[0]=(p[0][x-1].at[0]+p[1][x-1].at[0]+p[1][x].at[0])/3; \
    p[0][x].at[1]=(p[0][x-1].at[1]+p[1][x-1].at[1]+p[1][x].at[1])/3; \
    p[0][x].at[2]=(p[0][x-1].at[2]+p[1][x-1].at[2]+p[1][x].at[2])/3; \
    p[y][x].at[0]=(p[y][x-1].at[0]+p[y-1][x-1].at[0]+p[y-1][x].at[0])/3; \
    p[y][x].at[1]=(p[y][x-1].at[1]+p[y-1][x-1].at[1]+p[y-1][x].at[1])/3; \
    p[y][x].at[2]=(p[y][x-1].at[2]+p[y-1][x-1].at[2]+p[y-1][x].at[2])/3; \
  } \
}



DEFINE_FILLMOAT_1D(UChar,uchar)
DEFINE_FILLMOAT_3D(UChar3,uchar3)



typedef void (*f_caller) P ((image dest,long width));



static caller_table table[] = {
  {UChar, fillmoat_uchar},
  {UChar3,fillmoat_uchar3}
};



void image__fillmoat(src,width)
     image src;
     long width;
{
  f_caller caller;

  caller = (f_caller)image__caller("image__fillmoat",src,Number(table),table);

  if (caller) caller(src,width);
}
