/*
 * liner.c
 */



#include "Funcs.h"
#include <stdio.h>



#define DEFINE_LINER_1D(TYPE_NAME,TYPE,tTYPE,MAX)\
static void liner_##TYPE(dest,src)\
     image dest;\
     image src;\
{\
  register TYPE *src_ras;\
  register TYPE *dest_ras;\
  register long i;\
  tTYPE max,min,range;\
  long n;\
\
  image__type_check("image__liner",src,TYPE_NAME);\
\
  if (dest != src)\
    Image.make(dest,Image.type(src),Image.xsize(src),Image.ysize(src));\
\
  src_ras = (TYPE *)Image.raster(src);\
  dest_ras = (TYPE *)Image.raster(dest);\
  n = Image.area(src);\
  if (n == 0) return;\
\
  max = min = src_ras[0];\
  for (i = 1; i < n; i++)\
    if (src_ras[i] > max) max = src_ras[i];\
    else\
    if (src_ras[i] < min) min = src_ras[i];\
  range = max - min;\
  if (range == 0) range = 1;\
  fprintf(stderr,"original:max = %.2f, min = %.2f, range = %.2f\n",\
	  (double)max,(double)min,(double)range);\
  for (i = 0; i < n; i++)\
    dest_ras[i] = MAX * (src_ras[i] - min) / range;\
}



#define DEFINE_LINER_2D(TYPE_NAME,TYPE,MAX)\
static void liner_##TYPE(dest,src)\
     image dest;\
     image src;\
{\
  register TYPE *src_ras;\
  register TYPE *dest_ras;\
  register long i;\
  TYPE max,min,range;\
  long n;\
\
  image__type_check("image__liner",src,TYPE_NAME);\
\
  if (dest != src)\
    Image.make(dest,Image.type(src),Image.xsize(src),Image.ysize(src));\
\
  src_ras = (TYPE *)Image.raster(src);\
  dest_ras = (TYPE *)Image.raster(dest);\
  n = Image.area(src);\
  if (n == 0) return;\
\
  max = min = src_ras[0];\
  for (i = 1; i < n; i++)\
    {\
      if (src_ras[i].at[0] > max.at[0]) max.at[0] = src_ras[i].at[0];\
      else\
      if (src_ras[i].at[0] < min.at[0]) min.at[0] = src_ras[i].at[0];\
\
      if (src_ras[i].at[1] > max.at[1]) max.at[1] = src_ras[i].at[1];\
      else\
      if (src_ras[i].at[1] < min.at[1]) min.at[1] = src_ras[i].at[1];\
    }\
  range.at[0] = max.at[0] - min.at[0] + 1;\
  range.at[1] = max.at[1] - min.at[1] + 1;\
  if (range.at[0] == 0) range.at[0] = 1;\
  if (range.at[1] == 0) range.at[1] = 1;\
  for (i = 0; i < n; i++)\
    {\
      dest_ras[i].at[0] = MAX * (src_ras[i].at[0] - min.at[0]) / range.at[0];\
      dest_ras[i].at[1] = MAX * (src_ras[i].at[1] - min.at[1]) / range.at[1];\
    }\
}



#define DEFINE_LINER_3D(TYPE_NAME,TYPE,MAX)\
static void liner_##TYPE(dest,src)\
     image dest;\
     image src;\
{\
  register TYPE *src_ras;\
  register TYPE *dest_ras;\
  register long i;\
  TYPE max,min,range;\
  long n;\
\
  image__type_check("image__liner",src,TYPE_NAME);\
\
  if (dest != src)\
    Image.make(dest,Image.type(src),Image.xsize(src),Image.ysize(src));\
\
  src_ras = (TYPE *)Image.raster(src);\
  dest_ras = (TYPE *)Image.raster(dest);\
  n = Image.area(src);\
  if (n == 0) return;\
\
  max = min = src_ras[0];\
  for (i = 1; i < n; i++)\
    {\
      if (src_ras[i].at[0] > max.at[0]) max.at[0] = src_ras[i].at[0];\
      else\
      if (src_ras[i].at[0] < min.at[0]) min.at[0] = src_ras[i].at[0];\
\
      if (src_ras[i].at[1] > max.at[1]) max.at[1] = src_ras[i].at[1];\
      else\
      if (src_ras[i].at[1] < min.at[1]) min.at[1] = src_ras[i].at[1];\
\
      if (src_ras[i].at[2] > max.at[2]) max.at[2] = src_ras[i].at[2];\
      else\
      if (src_ras[i].at[2] < min.at[2]) min.at[2] = src_ras[i].at[2];\
    }\
  range.at[0] = max.at[0] - min.at[0];\
  range.at[1] = max.at[1] - min.at[1];\
  range.at[2] = max.at[2] - min.at[2];\
  if (range.at[0] == 0) range.at[0] = 1;\
  if (range.at[1] == 0) range.at[1] = 1;\
  if (range.at[2] == 0) range.at[2] = 1;\
  for (i = 0; i < n; i++)\
    {\
      dest_ras[i].at[0] = MAX * (src_ras[i].at[0] - min.at[0]) / range.at[0];\
      dest_ras[i].at[1] = MAX * (src_ras[i].at[1] - min.at[1]) / range.at[1];\
      dest_ras[i].at[2] = MAX * (src_ras[i].at[2] - min.at[2]) / range.at[2];\
    }\
}



DEFINE_LINER_1D(Bit1,bit1,long,1)
DEFINE_LINER_1D(Bit4,bit4,long,15)

DEFINE_LINER_1D(Char,char,long,127)
DEFINE_LINER_1D(UChar,uchar,long,255)
DEFINE_LINER_1D(Short,short,long,255)
DEFINE_LINER_1D(UShort,ushort,long,65535)
DEFINE_LINER_1D(Long,long,long,255)
DEFINE_LINER_1D(ULong,ulong,long,255)
DEFINE_LINER_1D(Float,float,double,1)
DEFINE_LINER_1D(Double,double,double,1)

DEFINE_LINER_2D(Char2,char2,128)
DEFINE_LINER_2D(UChar2,uchar2,256)
DEFINE_LINER_2D(Short2,short2,256)
DEFINE_LINER_2D(UShort2,ushort2,256)
DEFINE_LINER_2D(Long2,long2,256)
DEFINE_LINER_2D(ULong2,ulong2,256)
DEFINE_LINER_2D(Float2,float2,1)
DEFINE_LINER_2D(Double2,double2,1)

DEFINE_LINER_3D(Char3,char3,127)
DEFINE_LINER_3D(UChar3,uchar3,255)
DEFINE_LINER_3D(Short3,short3,255)
DEFINE_LINER_3D(UShort3,ushort3,255)
DEFINE_LINER_3D(Long3,long3,255)
DEFINE_LINER_3D(ULong3,ulong3,255)
DEFINE_LINER_3D(Float3,float3,1)
DEFINE_LINER_3D(Double3,double3,1)



typedef void (*f_caller) P ((image dest,image src));



static caller_table table[] = {
  {Bit1, liner_bit1},
  {Bit4, liner_bit4},

  {Char, liner_char},
  {UChar, liner_uchar},
  {Short, liner_short},
  {UShort, liner_ushort},
  {Long, liner_long},
  {ULong, liner_ulong},
  {Float, liner_float},
  {Double, liner_double},

  {Char2, liner_char2},
  {UChar2, liner_uchar2},
  {Short2, liner_short2},
  {UShort2, liner_ushort2},
  {Long2, liner_long2},
  {ULong2, liner_ulong2},
  {Float2, liner_float2},
  {Double2, liner_double2},

  {Char3, liner_char3},
  {UChar3, liner_uchar3},
  {Short3, liner_short3},
  {UShort3, liner_ushort3},
  {Long3, liner_long3},
  {ULong3, liner_ulong3},
  {Float3, liner_float3},
  {Double3, liner_double3},
};



void image__liner(dst,src)
     image dst;
     image src;
{
  f_caller caller;

  caller = (f_caller)image__caller("image__liner",src,Number(table),table);

  if (caller) caller(dst,src);
}
