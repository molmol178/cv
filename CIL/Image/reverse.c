/*
 * reverse.c
 */



#include "Funcs.h"




static void reverse_packedBit1(dest,src)
     image dest;
     image src;
{
  register uchar *src_ras;
  register uchar *dest_ras;
  register long i;
  long n,xbyte;

  image__type_check("image__reverse",src,PackedBit1);

  if (dest != src)
    Image.make(dest,src->type,src->xsize,src->ysize);

  src_ras = (uchar *)Image.raster(src);
  dest_ras = (uchar *)Image.raster(dest);
  n = Image.byte(src);
  for (i = 0; i < n; i++)
    dest_ras[i] = ~src_ras[i];
}



#define DEFINE_REVERSE_BIT(TYPE_NAME,TYPE,MASK)\
static void reverse_##TYPE(dest,src)\
     image dest;\
     image src;\
{\
  register TYPE *src_ras;\
  register TYPE *dest_ras;\
  register long i;\
  long n;\
\
  image__type_check("image__reverse",src,TYPE_NAME);\
\
  if (dest != src)\
    Image.make(dest,Image.type(src),Image.xsize(src),Image.ysize(src));\
\
  src_ras = (TYPE *)Image.raster(src);\
  dest_ras = (TYPE *)Image.raster(dest);\
  n = Image.area(src);\
  for (i = 0; i < n; i++)\
    dest_ras[i] = (~src_ras[i]) & MASK;\
}



#define DEFINE_REVERSE_1D(TYPE_NAME,TYPE)\
static void reverse_##TYPE(dest,src)\
     image dest;\
     image src;\
{\
  register TYPE *src_ras;\
  register TYPE *dest_ras;\
  register long i;\
  long n;\
\
  image__type_check("image__reverse",src,TYPE_NAME);\
\
  if (dest != src)\
    Image.make(dest,Image.type(src),Image.xsize(src),Image.ysize(src));\
\
  src_ras = (TYPE *)Image.raster(src);\
  dest_ras = (TYPE *)Image.raster(dest);\
\
  n = Image.area(src);\
  for (i = 0; i < n; i++)\
    dest_ras[i] = ~src_ras[i];\
}



#define DEFINE_REVERSE_1D_F(TYPE_NAME,TYPE)\
static void reverse_##TYPE(dest,src)\
     image dest;\
     image src;\
{\
  register TYPE *src_ras;\
  register TYPE *dest_ras;\
  register long i;\
  double max,min;\
  long n;\
\
  image__type_check("image__reverse",src,TYPE_NAME);\
\
  if (dest != src)\
    Image.make(dest,Image.type(src),Image.xsize(src),Image.ysize(src));\
\
  src_ras = (TYPE *)Image.raster(src);\
  dest_ras = (TYPE *)Image.raster(dest);\
\
  n = Image.area(src);\
  max = min = src_ras[0];\
  for (i = 1; i < n; i++)\
    {\
      if (src_ras[i] > max) max = src_ras[i];\
      if (src_ras[i] < min) min = src_ras[i];\
    }\
\
  n = Image.area(src);\
  for (i = 0; i < n; i++)\
    dest_ras[i] = max - src_ras[i] + min;\
}



#define DEFINE_REVERSE_2D(TYPE_NAME,TYPE)\
static void reverse_##TYPE(dest,src)\
     image dest;\
     image src;\
{\
  register TYPE *src_ras;\
  register TYPE *dest_ras;\
  register long i;\
  long n;\
\
  image__type_check("image__reverse",src,TYPE_NAME);\
\
  if (dest != src)\
    Image.make(dest,Image.type(src),Image.xsize(src),Image.ysize(src));\
\
  src_ras = (TYPE *)Image.raster(src);\
  dest_ras = (TYPE *)Image.raster(dest);\
\
  n = Image.area(src);\
  for (i = 0; i < n; i++)\
    {\
      dest_ras[i].at[0] = ~(src_ras[i].at[0]);\
      dest_ras[i].at[1] = ~(src_ras[i].at[1]);\
    }\
}



#define DEFINE_REVERSE_3D(TYPE_NAME,TYPE)\
static void reverse_##TYPE(dest,src)\
     image dest;\
     image src;\
{\
  register TYPE *src_ras;\
  register TYPE *dest_ras;\
  register long i;\
  long n;\
\
  image__type_check("image__reverse",src,TYPE_NAME);\
\
  if (dest != src)\
    Image.make(dest,Image.type(src),Image.xsize(src),Image.ysize(src));\
\
  src_ras = (TYPE *)Image.raster(src);\
  dest_ras = (TYPE *)Image.raster(dest);\
  n = Image.area(src);\
  for (i = 0; i < n; i++)\
    {\
      dest_ras[i].at[0] = ~(src_ras[i].at[0]);\
      dest_ras[i].at[1] = ~(src_ras[i].at[1]);\
      dest_ras[i].at[2] = ~(src_ras[i].at[2]);\
    }\
}



DEFINE_REVERSE_BIT(Bit1,bit1,1)
DEFINE_REVERSE_BIT(Bit4,bit4,0x0f)

DEFINE_REVERSE_1D(Char,char)
DEFINE_REVERSE_1D(UChar,uchar)
DEFINE_REVERSE_1D(Short,short)
DEFINE_REVERSE_1D(UShort,ushort)
DEFINE_REVERSE_1D(Long,long)
DEFINE_REVERSE_1D(ULong,ulong)
DEFINE_REVERSE_1D_F(Float,float)
DEFINE_REVERSE_1D_F(Double,double)

DEFINE_REVERSE_2D(Char2,char2)
DEFINE_REVERSE_2D(UChar2,uchar2)
DEFINE_REVERSE_2D(Short2,short2)
DEFINE_REVERSE_2D(UShort2,ushort2)
DEFINE_REVERSE_2D(Long2,long2)
DEFINE_REVERSE_2D(ULong2,ulong2)

DEFINE_REVERSE_3D(Char3,char3)
DEFINE_REVERSE_3D(UChar3,uchar3)
DEFINE_REVERSE_3D(Short3,short3)
DEFINE_REVERSE_3D(UShort3,ushort3)
DEFINE_REVERSE_3D(Long3,long3)
DEFINE_REVERSE_3D(ULong3,ulong3)



typedef void (*f_caller) P ((image dest,image src));



static caller_table table[] = {
  {Bit1, reverse_bit1},
  {Bit4, reverse_bit4},

  {Char, reverse_char},
  {UChar, reverse_uchar},
  {Short, reverse_short},
  {UShort, reverse_ushort},
  {Long, reverse_long},
  {ULong, reverse_ulong},
  {Float, reverse_float},
  {Double, reverse_double},

  {Char2, reverse_char2},
  {UChar2, reverse_uchar2},
  {Short2, reverse_short2},
  {UShort2, reverse_ushort2},
  {Long2, reverse_long2},
  {ULong2, reverse_ulong2},

  {Char3, reverse_char3},
  {UChar3, reverse_uchar3},
  {Short3, reverse_short3},
  {UShort3, reverse_ushort3},
  {Long3, reverse_long3},
  {ULong3, reverse_ulong3},
  {PackedBit1,reverse_packedBit1}
};



void image__reverse(dst,src)
     image dst;
     image src;
{
  f_caller caller;

  caller = (f_caller)image__caller("image__reverse",src,Number(table),table);

  if (caller) caller(dst,src);
}
