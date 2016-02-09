/*
 * long_type.c
 */



#include "Funcs.h"




#define DEFINE_LONG_TYPE_1D(TYPE_NAME,TYPE)\
static void long_type_##TYPE(dest,src)\
     image dest;\
     image src;\
{\
  image temp;\
  register TYPE *src_ras;\
  register long *dest_ras;\
  register long i;\
  long n;\
\
  image__type_check("image__long_type",src,TYPE_NAME);\
\
  if (__TYPE( src ) == Long )\
    {\
      if ( src != dest ) Image.copy( dest, src );\
      return;\
    }\
\
  if (src == dest) temp = Image.create("dest"); else temp = dest;\
\
  Image.make(temp,Long,Image.xsize(src),Image.ysize(src));\
\
  src_ras = (TYPE *)Image.raster(src);\
  dest_ras = (long *)Image.raster(temp);\
\
  n = Image.area(src);\
  for (i = 0; i < n; i++)\
    dest_ras[i] = src_ras[i];\
\
  if (src == dest) {Image.copy(dest,temp);Image.destroy(temp);}\
}



#define DEFINE_LONG_TYPE_2D(TYPE_NAME,TYPE)\
static void long_type_##TYPE(dest,src)\
     image dest;\
     image src;\
{\
  image temp;\
  register TYPE *src_ras;\
  register long2 *dest_ras;\
  register long i;\
  long n;\
\
  if (Image.type(src) == Long2) { Image.copy(dest,src); return; }\
  image__type_check("image__long_type",src,TYPE_NAME);\
\
  if (src == dest) temp = Image.create("dest"); else temp = dest;\
\
  Image.make(temp,Long2,Image.xsize(src),Image.ysize(src));\
\
  src_ras = (TYPE *)Image.raster(src);\
  dest_ras = (long2 *)Image.raster(temp);\
\
  n = Image.area(src);\
  for (i = 0; i < n; i++)\
    {\
      dest_ras[i].at[0] = src_ras[i].at[0];\
      dest_ras[i].at[1] = src_ras[i].at[1];\
    }\
\
  if (src == dest) {Image.copy(dest,temp);Image.destroy(temp);}\
}



#define DEFINE_LONG_TYPE_3D(TYPE_NAME,TYPE)\
static void long_type_##TYPE(dest,src)\
     image dest;\
     image src;\
{\
  image temp;\
  register TYPE *src_ras;\
  register long3 *dest_ras;\
  register long i;\
  long n;\
\
  if (Image.type(src) == Long3) { Image.copy(dest,src); return; }\
  image__type_check("image__long_type",src,TYPE_NAME);\
\
  if (src == dest) temp = Image.create("dest"); else temp = dest;\
\
  Image.make(temp,Long3,Image.xsize(src),Image.ysize(src));\
\
  src_ras = (TYPE *)Image.raster(src);\
  dest_ras = (long3 *)Image.raster(temp);\
  n = Image.area(src);\
  for (i = 0; i < n; i++)\
    {\
      dest_ras[i].at[0] = src_ras[i].at[0];\
      dest_ras[i].at[1] = src_ras[i].at[1];\
      dest_ras[i].at[2] = src_ras[i].at[2];\
    }\
\
  if (src == dest) {Image.copy(dest,temp);Image.destroy(temp);}\
}



DEFINE_LONG_TYPE_1D(Bit1,bit1)
DEFINE_LONG_TYPE_1D(Bit4,bit4)

DEFINE_LONG_TYPE_1D(Char,char)
DEFINE_LONG_TYPE_1D(UChar,uchar)
DEFINE_LONG_TYPE_1D(Short,short)
DEFINE_LONG_TYPE_1D(UShort,ushort)
DEFINE_LONG_TYPE_1D(Long,long)
DEFINE_LONG_TYPE_1D(ULong,ulong)

DEFINE_LONG_TYPE_2D(Char2,char2)
DEFINE_LONG_TYPE_2D(UChar2,uchar2)
DEFINE_LONG_TYPE_2D(Short2,short2)
DEFINE_LONG_TYPE_2D(UShort2,ushort2)
DEFINE_LONG_TYPE_2D(Long2,long2)
DEFINE_LONG_TYPE_2D(ULong2,ulong2)

DEFINE_LONG_TYPE_3D(Char3,char3)
DEFINE_LONG_TYPE_3D(UChar3,uchar3)
DEFINE_LONG_TYPE_3D(Short3,short3)
DEFINE_LONG_TYPE_3D(UShort3,ushort3)
DEFINE_LONG_TYPE_3D(Long3,long3)
DEFINE_LONG_TYPE_3D(ULong3,ulong3)



typedef void (*f_caller) P ((image dest,image src));



static caller_table table[] = {
  {Bit1, long_type_bit1},
  {Bit4, long_type_bit4},

  {Char, long_type_char},
  {UChar, long_type_uchar},
  {Short, long_type_short},
  {UShort, long_type_ushort},
  {Long, long_type_long},
  {ULong, long_type_ulong},

  {Char2, long_type_char2},
  {UChar2, long_type_uchar2},
  {Short2, long_type_short2},
  {UShort2, long_type_ushort2},
  {Long2, long_type_long2},
  {ULong2, long_type_ulong2},

  {Char3, long_type_char3},
  {UChar3, long_type_uchar3},
  {Short3, long_type_short3},
  {UShort3, long_type_ushort3},
  {Long3, long_type_long3},
  {ULong3, long_type_ulong3}
};



void image__long_type(dst,src)
     image dst;
     image src;
{
  f_caller caller;

  caller = (f_caller)image__caller("image__long_type",src,Number(table),table);

  if (caller) caller(dst,src);
}
