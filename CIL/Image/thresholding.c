/*
 * thresholding.c
 */



#include "Funcs.h"



#define DEFINE_THRESHOLDING_1D(TYPE_NAME,TYPE)\
static void thresholding_##TYPE \
  _P3 (( image , out       ),\
       ( image , in        ),\
       ( double, threshold ))\
{\
  register TYPE **in_ptr;\
  register bit1 **out_ptr;\
  register long x,y;\
  register long xsize,ysize;\
\
  image__type_check("image__thresholding",in,TYPE_NAME);\
\
  xsize = Image.xsize(in);\
  ysize = Image.ysize(in);\
\
  Image.make(out,Bit1,xsize,ysize);\
\
  in_ptr  = (TYPE **)Image.data(in);\
  out_ptr = (bit1 **)Image.data(out);\
\
  for (y = 0; y < ysize; y++)\
    for (x = 0; x < xsize; x++)\
      out_ptr[y][x] = ( (double)in_ptr[y][x] > threshold ) ? 1 : 0; \
}



DEFINE_THRESHOLDING_1D(Char,char)
DEFINE_THRESHOLDING_1D(UChar,uchar)
DEFINE_THRESHOLDING_1D(Int,int)
DEFINE_THRESHOLDING_1D(UInt,uint)
DEFINE_THRESHOLDING_1D(Short,short)
DEFINE_THRESHOLDING_1D(UShort,ushort)
DEFINE_THRESHOLDING_1D(Long,long)
DEFINE_THRESHOLDING_1D(ULong,ulong)
DEFINE_THRESHOLDING_1D(Float,float)
DEFINE_THRESHOLDING_1D(Double,double)



typedef void (*f_caller)
     P3 (( image , out       ),
	 ( image , in        ),
	 ( double, threshold ))



static caller_table table[] = {
  {Char,thresholding_char},
  {UChar,thresholding_uchar},
  {Int,thresholding_int},
  {UInt,thresholding_uint},
  {Short,thresholding_short},
  {UShort,thresholding_ushort},
  {Long,thresholding_long},
  {ULong,thresholding_ulong},
  {Float,thresholding_float},
  {Double,thresholding_double},
};



void image__thresholding
  _P3 (( image , out       ),
       ( image , in        ),
       ( double, threshold ))
{
  f_caller caller;

  caller = (f_caller)image__caller("image__thresholding",
				   in,Number(table),table);

  if (caller) caller(out,in,threshold);
}
