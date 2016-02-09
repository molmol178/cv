/*
 * separate2.c
 */



#include "Funcs.h"



#define DEFINE_SEPARATE2_2D(TYPE_NAME,TYPE)\
static void separate2_##TYPE(dst1,dst2,src)\
     image dst1;\
     image dst2;\
     image src;\
{\
  register long x,y;\
  register TYPE **dst1_data,**dst2_data;\
  register TYPE##2 **src_data;\
  long xsize,ysize;\
\
  image__type_check("image__separate2",src,TYPE_NAME##2);\
\
  xsize = Image.xsize(src);\
  ysize = Image.ysize(src);\
  Image.make(dst1,TYPE_NAME,xsize,ysize);\
  Image.make(dst2,TYPE_NAME,xsize,ysize);\
\
  src_data  = (TYPE##2 **)Image.data(src);\
  dst1_data = (TYPE **)Image.data(dst1);\
  dst2_data = (TYPE **)Image.data(dst2);\
\
  for (y = 0; y < ysize; y++)\
    for (x = 0; x < xsize; x++)\
      {\
	dst1_data[y][x] = src_data[y][x].at[0];\
	dst2_data[y][x] = src_data[y][x].at[1];\
      }\
}



DEFINE_SEPARATE2_2D(Short,short)



typedef void (*f_caller) P ((image dest1,image dest2,image src));



static caller_table table[] = {
  {Short2, separate2_short}
};



void image__separate2(dst1,dst2,src)
     image dst1;
     image dst2;
     image src;
{
  f_caller caller;

  caller = (f_caller)image__caller("image__separate2",src,Number(table),table);

  if (caller) caller(dst1,dst2,src);
}
