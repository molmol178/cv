/*
 * filename : or.c
 * author   : Takahiro Sugiyama
 * date     : Wednesday, June 1 1994
 * describe : image operator of OR
 */



#include "Funcs.h"



#define DEFINE_OR_1D(TYPE_NAME,TYPE) \
static void or_##TYPE\
  _P3 (( image, output ),\
       ( image, input1 ),\
       ( image, input2 ))\
{\
  register uchar *src1_ras;\
  register uchar *src2_ras;\
  register uchar *dest_ras;\
  register long i;\
  long n, xbyte;\
\
  image__type_check( "image__or", input1, TYPE_NAME );\
  image__type_check( "image__or", input2, TYPE_NAME );\
\
  if (( output != input1 ) && ( output != input2 ))\
    Image.make( output, __TYPE(input1), __XSIZE(input1), __YSIZE(input1) );\
\
  src1_ras = __RASTER( input1, uchar );\
  src2_ras = __RASTER( input2, uchar );\
  dest_ras = __RASTER( output, uchar );\
  n = __BYTE( input1 );\
  for ( i = 0; i < n; i++ )\
    *dest_ras++ = *src1_ras++ | *src2_ras++;\
}



DEFINE_OR_1D(PackedBit1,packedbit1)
DEFINE_OR_1D(Bit1,bit1)
DEFINE_OR_1D(Bit4,bit4)

DEFINE_OR_1D(Char,char)
DEFINE_OR_1D(UChar,uchar)
DEFINE_OR_1D(Short,short)
DEFINE_OR_1D(UShort,ushort)
DEFINE_OR_1D(Long,long)
DEFINE_OR_1D(ULong,ulong)

DEFINE_OR_1D(UChar3,uchar3)



typedef void (*f_caller)
     P3 (( image, output ),
	 ( image, input1 ),
	 ( image, input2 ))



static caller_table table[] = {
  {PackedBit1, or_packedbit1},
  {Bit1, or_bit1},
  {Bit4, or_bit4},

  {Char, or_char},
  {UChar, or_uchar},
  {Short, or_short},
  {UShort, or_ushort},
  {Long, or_long},
  {ULong, or_ulong},

  {UChar3, or_uchar3},
};



void image__or
  _P3 (( image, output ),
       ( image, input1 ),
       ( image, input2 ))
{
  f_caller caller;

  caller = (f_caller)image__caller( "image__or", input1, Number(table), table );

  if ( caller ) caller( output, input1, input2 );
}
