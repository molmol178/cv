/*
 * filename : and.c
 * author   : Takahiro Sugiyama
 * date     : Wednesday, June 1 1994
 * describe : image operator of AND
 */



#include "Funcs.h"



#define DEFINE_AND_1D(TYPE_NAME,TYPE) \
static void and_##TYPE\
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
  image__type_check( "image__and", input1, TYPE_NAME );\
  image__type_check( "image__and", input2, TYPE_NAME );\
\
  if (( output != input1 ) && ( output != input2 ))\
    Image.make( output, __TYPE(input1), __XSIZE(input1), __YSIZE(input1) );\
\
  src1_ras = __RASTER( input1, uchar );\
  src2_ras = __RASTER( input2, uchar );\
  dest_ras = __RASTER( output, uchar );\
  n = __BYTE( input1 );\
  for ( i = 0; i < n; i++ )\
    *dest_ras++ = *src1_ras++ & *src2_ras++;\
}



DEFINE_AND_1D(PackedBit1,packedbit1)
DEFINE_AND_1D(Bit1,bit1)
DEFINE_AND_1D(Bit4,bit4)

DEFINE_AND_1D(Char,char)
DEFINE_AND_1D(UChar,uchar)
DEFINE_AND_1D(Short,short)
DEFINE_AND_1D(UShort,ushort)
DEFINE_AND_1D(Long,long)
DEFINE_AND_1D(ULong,ulong)

DEFINE_AND_1D(UChar3,uchar3)



typedef void (*f_caller)
     P3 (( image, output ),
	 ( image, input1 ),
	 ( image, input2 ))



static caller_table table[] = {
  {PackedBit1, and_packedbit1},
  {Bit1, and_bit1},
  {Bit4, and_bit4},

  {Char, and_char},
  {UChar, and_uchar},
  {Short, and_short},
  {UShort, and_ushort},
  {Long, and_long},
  {ULong, and_ulong},

  {UChar3, and_uchar3},
};



void image__and
  _P3 (( image, output ),
       ( image, input1 ),
       ( image, input2 ))
{
  f_caller caller;

  caller = (f_caller)image__caller( "image__and", input1, Number(table), table );

  if ( caller ) caller( output, input1, input2 );
}
