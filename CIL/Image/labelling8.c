/*
 * filename : labelling8.c
 * author   : Takahiro Sugiyama
 * date     : Wednesday, February 5 1997
 * describe : labelling with eight connection
 */


#include <stdio.h>
#include "Image/Funcs.h"
#include "Image.h"
#include "misc/memlib.h"


/*========================================================================*
 * ラベリング８連結
 * １からラベルをつける。
 *========================================================================*/
#define DEFINE_LABELLING_N8(_Dst_Type,_dst_type,_src_type) \
long image__N8_labelling_##_dst_type##_##_src_type(image dst, image src)\
{\
  register long x, y, xsize, ysize;\
  register _src_type **src_data, val;\
  register _dst_type **dst_data, *ptr;\
  register long ai, bi;\
  register long *table, *label;\
  long next_label, index;\
\
  register long s1, s2, s3, s4;\
\
  xsize = __XSIZE(src);\
  ysize = __YSIZE(src);\
  Image.make(dst, _Dst_Type, xsize, ysize);\
\
  /* ラベルテーブルの初期化 */\
  next_label = 1;\
  table = typenew1(__AREA(src), long);\
  for (x = 0; x < __AREA(src); x++) table[x] = x;\
\
  src_data = __DATA(src, _src_type);\
  dst_data = __DATA(dst, _dst_type);\
\
  for (y = 0; y < ysize; y++)\
    for (x = 0; x < xsize; x++)\
      {\
	val = src_data[y][x];\
	ptr = &dst_data[y][x];\
\
	if (x == 0 && y == 0)\
	  {\
	    *ptr = next_label++;\
	    continue;\
	  }\
\
	if (y == 0)\
	  {\
	    if (val == src_data[y][x-1])\
	      *ptr = dst_data[y][x-1];\
	    else\
	      *ptr = next_label++;\
	    continue;\
	  }\
\
	if (x == 0)\
	  {\
	    if (val == src_data[y-1][x])\
	      *ptr = dst_data[y-1][x];\
	    else\
	    if (val == src_data[y-1][x+1])\
	      *ptr = dst_data[y-1][x+1];\
	    else\
	      *ptr = next_label++;\
	    continue;\
	  }\
\
	if (x == (xsize-1))\
	  {\
	    if (val == src_data[y-1][x])\
	      *ptr = dst_data[y-1][x];\
	    else\
	    if (val == src_data[y][x-1])\
	      *ptr = dst_data[y][x-1];\
	    else\
	    if (val == src_data[y-1][x-1])\
	      *ptr = dst_data[y-1][x-1];\
	    else\
	      *ptr = next_label++;\
	    continue;\
	  }\
\
	s1 = src_data[y][x-1];\
	s2 = src_data[y-1][x];\
	s3 = src_data[y-1][x-1];\
	s4 = src_data[y-1][x+1];\
\
	if ((val == s2) && (val == s1))\
	  {\
	    ai = dst_data[y][x-1];\
	    bi = dst_data[y-1][x];\
	    if (ai != bi) {\
	      while (table[ai] != ai) ai = table[ai];\
	      while (table[bi] != bi) bi = table[bi];\
	      if (ai != bi) table[bi] = ai;\
	    }\
	    *ptr = (ai < bi) ? ai : bi;\
	    continue;\
	  }\
\
	if ((val == s3) && (val != s2) && (val == s4))\
	  {\
	    ai = dst_data[y-1][x-1];\
	    bi = dst_data[y-1][x+1];\
	    if (ai != bi) {\
	      while (table[ai] != ai) ai = table[ai];\
	      while (table[bi] != bi) bi = table[bi];\
	      if (ai != bi) table[bi] = ai;\
	    }\
	    *ptr = (ai < bi) ? ai : bi;\
	    continue;\
	  }\
\
	if ((val == s1) && (val != s2) && (val != s3) && (val == s4))\
	  {\
	    ai = dst_data[y][x-1];\
	    bi = dst_data[y-1][x+1];\
	    if (ai != bi) {\
	      while (table[ai] != ai) ai = table[ai];\
	      while (table[bi] != bi) bi = table[bi];\
	      if (ai != bi) table[bi] = ai;\
	    }\
	    *ptr = (ai < bi) ? ai : bi;\
	    continue;\
	  }\
\
	if (val == s2)\
	  {\
	    *ptr = dst_data[y-1][x];\
	    continue;\
	  }\
\
	if (val == s1)\
	  {\
	    *ptr = dst_data[y][x-1];\
	    continue;\
	  }\
\
	if (val == s3)\
	  {\
	    *ptr = dst_data[y-1][x-1];\
	    continue;\
	  }\
\
	if (val == s4)\
	  {\
	    *ptr = dst_data[y-1][x+1];\
	    continue;\
	  }\
\
	*ptr = next_label++;\
      }\
\
  /* ラベルの再割当 */\
  for (x = 0; x < next_label; x++)\
    {\
      index = x;\
      while (table[index] != index) index = table[index];\
      table[x] = index;\
    }\
  label = typenew1(next_label, long);\
  index = 1;\
  for (x = 0; x < next_label; x++)\
    if (table[x] == x) label[x] = index++;\
  for (y = 0; y < ysize; y++)\
    for (x = 0; x < xsize; x++)\
      dst_data[y][x] = label[table[dst_data[y][x]]];\
\
  typefree1(table);\
  typefree1(label);\
\
  return index-1;\
}



/*========================================================================*
 * ラベリング８連結
 * ０ラベルは無視する。残りは１からラベルをつける。
 *========================================================================*/
#define DEFINE_LABELLING_N8_FG(_Dst_Type,_dst_type,_src_type) \
long image__N8_labelling_foreground_##_dst_type##_##_src_type(image dst, image src)\
{\
  register long x, y, xsize, ysize;\
  register _src_type **src_data, val;\
  register _dst_type **dst_data, *ptr;\
  register long ai, bi;\
  register long *table, *label;\
  long next_label, index;\
\
  register long s1, s2, s3, s4;\
\
  xsize = __XSIZE(src);\
  ysize = __YSIZE(src);\
  Image.make(dst, _Dst_Type, xsize, ysize);\
\
  /* ラベルテーブルの初期化 */\
  next_label = 1;\
  table = typenew1(__AREA(src), long);\
  for (x = 0; x < __AREA(src); x++) table[x] = x;\
\
  src_data = __DATA(src, _src_type);\
  dst_data = __DATA(dst, _dst_type);\
\
  for (y = 0; y < ysize; y++)\
    for (x = 0; x < xsize; x++)\
      {\
	val = src_data[y][x];\
	ptr = &dst_data[y][x];\
\
	if (val == 0) { *ptr = 0; continue; }\
\
	if (x == 0 && y == 0)\
	  {\
	    *ptr = next_label++;\
	    continue;\
	  }\
\
	if (y == 0)\
	  {\
	    if (val == src_data[y][x-1])\
	      *ptr = dst_data[y][x-1];\
	    else\
	      *ptr = next_label++;\
	    continue;\
	  }\
\
	if (x == 0)\
	  {\
	    if (val == src_data[y-1][x])\
	      *ptr = dst_data[y-1][x];\
	    else\
	    if (val == src_data[y-1][x+1])\
	      *ptr = dst_data[y-1][x+1];\
	    else\
	      *ptr = next_label++;\
	    continue;\
	  }\
\
	if (x == (xsize-1))\
	  {\
	    if (val == src_data[y-1][x])\
	      *ptr = dst_data[y-1][x];\
	    else\
	    if (val == src_data[y][x-1])\
	      *ptr = dst_data[y][x-1];\
	    else\
	    if (val == src_data[y-1][x-1])\
	      *ptr = dst_data[y-1][x-1];\
	    else\
	      *ptr = next_label++;\
	    continue;\
	  }\
\
	s1 = src_data[y][x-1];\
	s2 = src_data[y-1][x];\
	s3 = src_data[y-1][x-1];\
	s4 = src_data[y-1][x+1];\
\
	if ((val == s2) && (val == s1))\
	  {\
	    ai = dst_data[y][x-1];\
	    bi = dst_data[y-1][x];\
	    if (ai != bi) {\
	      while (table[ai] != ai) ai = table[ai];\
	      while (table[bi] != bi) bi = table[bi];\
	      if (ai != bi) table[bi] = ai;\
	    }\
	    *ptr = (ai < bi) ? ai : bi;\
	    continue;\
	  }\
\
	if ((val == s3) && (val != s2) && (val == s4))\
	  {\
	    ai = dst_data[y-1][x-1];\
	    bi = dst_data[y-1][x+1];\
	    if (ai != bi) {\
	      while (table[ai] != ai) ai = table[ai];\
	      while (table[bi] != bi) bi = table[bi];\
	      if (ai != bi) table[bi] = ai;\
	    }\
	    *ptr = (ai < bi) ? ai : bi;\
	    continue;\
	  }\
\
	if ((val == s1) && (val != s2) && (val != s3) && (val == s4))\
	  {\
	    ai = dst_data[y][x-1];\
	    bi = dst_data[y-1][x+1];\
	    if (ai != bi) {\
	      while (table[ai] != ai) ai = table[ai];\
	      while (table[bi] != bi) bi = table[bi];\
	      if (ai != bi) table[bi] = ai;\
	    }\
	    *ptr = (ai < bi) ? ai : bi;\
	    continue;\
	  }\
\
	if (val == s2)\
	  {\
	    *ptr = dst_data[y-1][x];\
	    continue;\
	  }\
\
	if (val == s1)\
	  {\
	    *ptr = dst_data[y][x-1];\
	    continue;\
	  }\
\
	if (val == s3)\
	  {\
	    *ptr = dst_data[y-1][x-1];\
	    continue;\
	  }\
\
	if (val == s4)\
	  {\
	    *ptr = dst_data[y-1][x+1];\
	    continue;\
	  }\
\
	*ptr = next_label++;\
      }\
\
  /* ラベルの再割当 */\
  for (x = 1; x < next_label; x++)\
    {\
      index = x;\
      while (table[index] != index) index = table[index];\
      table[x] = index;\
    }\
  label = typenew1(next_label, long);\
  index = 1;\
  for (x = 1; x < next_label; x++)\
    if (table[x] == x) label[x] = index++;\
  for (y = 0; y < ysize; y++)\
    for (x = 0; x < xsize; x++)\
      dst_data[y][x] = label[table[dst_data[y][x]]];\
\
  typefree1(table);\
  typefree1(label);\
\
  return index-1;\
}



DEFINE_LABELLING_N8(UChar,uchar,bit1)
DEFINE_LABELLING_N8(Short,short,bit1)
DEFINE_LABELLING_N8(UShort,ushort,bit1)
DEFINE_LABELLING_N8(Long,long,bit1)

DEFINE_LABELLING_N8(UChar,uchar,char)
DEFINE_LABELLING_N8(Short,short,char)
DEFINE_LABELLING_N8(UShort,ushort,char)
DEFINE_LABELLING_N8(Long,long,char)

DEFINE_LABELLING_N8(UChar,uchar,uchar)
DEFINE_LABELLING_N8(Short,short,uchar)
DEFINE_LABELLING_N8(UShort,ushort,uchar)
DEFINE_LABELLING_N8(Long,long,uchar)

DEFINE_LABELLING_N8(UChar,uchar,ushort)
DEFINE_LABELLING_N8(Short,short,ushort)
DEFINE_LABELLING_N8(UShort,ushort,ushort)
DEFINE_LABELLING_N8(Long,long,ushort)

DEFINE_LABELLING_N8(UChar,uchar,short)
DEFINE_LABELLING_N8(Short,short,short)
DEFINE_LABELLING_N8(UShort,ushort,short)
DEFINE_LABELLING_N8(Long,long,short)

DEFINE_LABELLING_N8(UChar,uchar,long)
DEFINE_LABELLING_N8(Short,short,long)
DEFINE_LABELLING_N8(UShort,ushort,long)
DEFINE_LABELLING_N8(Long,long,long)



DEFINE_LABELLING_N8_FG(UChar,uchar,bit1)
DEFINE_LABELLING_N8_FG(Short,short,bit1)
DEFINE_LABELLING_N8_FG(UShort,ushort,bit1)
DEFINE_LABELLING_N8_FG(Long,long,bit1)

DEFINE_LABELLING_N8_FG(UChar,uchar,char)
DEFINE_LABELLING_N8_FG(Short,short,char)
DEFINE_LABELLING_N8_FG(UShort,ushort,char)
DEFINE_LABELLING_N8_FG(Long,long,char)

DEFINE_LABELLING_N8_FG(UChar,uchar,uchar)
DEFINE_LABELLING_N8_FG(Short,short,uchar)
DEFINE_LABELLING_N8_FG(UShort,ushort,uchar)
DEFINE_LABELLING_N8_FG(Long,long,uchar)

DEFINE_LABELLING_N8_FG(UChar,uchar,ushort)
DEFINE_LABELLING_N8_FG(Short,short,ushort)
DEFINE_LABELLING_N8_FG(UShort,ushort,ushort)
DEFINE_LABELLING_N8_FG(Long,long,ushort)

DEFINE_LABELLING_N8_FG(UChar,uchar,short)
DEFINE_LABELLING_N8_FG(Short,short,short)
DEFINE_LABELLING_N8_FG(UShort,ushort,short)
DEFINE_LABELLING_N8_FG(Long,long,short)

DEFINE_LABELLING_N8_FG(UChar,uchar,long)
DEFINE_LABELLING_N8_FG(Short,short,long)
DEFINE_LABELLING_N8_FG(UShort,ushort,long)
DEFINE_LABELLING_N8_FG(Long,long,long)



typedef long (*f_caller) P ((image dest,image src));

static caller_table N8_uchar_table[] = {
  {Bit1,  (void*)image__N8_labelling_uchar_uchar},
  {Char,  (void*)image__N8_labelling_uchar_char},
  {UChar, (void*)image__N8_labelling_uchar_uchar},
  {Short, (void*)image__N8_labelling_uchar_short},
  {UShort,(void*)image__N8_labelling_uchar_ushort},
  {Long,  (void*)image__N8_labelling_uchar_long},
};

static caller_table N8_short_table[] = {
  {Bit1,  (void*)image__N8_labelling_short_uchar},
  {Char,  (void*)image__N8_labelling_short_char},
  {UChar, (void*)image__N8_labelling_short_uchar},
  {Short, (void*)image__N8_labelling_short_short},
  {UShort,(void*)image__N8_labelling_short_ushort},
  {Long,  (void*)image__N8_labelling_short_long},
};

static caller_table N8_ushort_table[] = {
  {Bit1,  (void*)image__N8_labelling_ushort_uchar},
  {Char,  (void*)image__N8_labelling_ushort_char},
  {UChar, (void*)image__N8_labelling_ushort_uchar},
  {Short, (void*)image__N8_labelling_ushort_short},
  {UShort,(void*)image__N8_labelling_ushort_ushort},
  {Long,  (void*)image__N8_labelling_ushort_long},
};

static caller_table N8_long_table[] = {
  {Bit1,  (void*)image__N8_labelling_long_uchar},
  {Char,  (void*)image__N8_labelling_long_char},
  {UChar, (void*)image__N8_labelling_long_uchar},
  {Short, (void*)image__N8_labelling_long_short},
  {UShort,(void*)image__N8_labelling_long_ushort},
  {Long,  (void*)image__N8_labelling_long_long},
};

/*------------------------------------------------------------------------*
 * ８近傍のラベリング。できあがりはuchar型
 *------------------------------------------------------------------------*/
long image__N8_labelling_uchar(image label, image original)
{
  f_caller caller;

  caller = (f_caller)image__caller("N8_labelling_uchar",
				   original,
				   Number(N8_uchar_table),
				   N8_uchar_table);

  if (caller) return caller(label,original);

  return 0;
}

/*------------------------------------------------------------------------*
 * ８近傍のラベリング。できあがりはshort型
 *------------------------------------------------------------------------*/
long image__N8_labelling_short(image label, image original)
{
  f_caller caller;

  caller = (f_caller)image__caller("N8_labelling_short",
				   original,
				   Number(N8_short_table),
				   N8_short_table);

  if (caller) return caller(label,original);

  return 0;
}

/*------------------------------------------------------------------------*
 * ８近傍のラベリング。できあがりはushort型
 *------------------------------------------------------------------------*/
long image__N8_labelling_ushort(image label, image original)
{
  f_caller caller;

  caller = (f_caller)image__caller("N8_labelling_ushort",
				   original,
				   Number(N8_ushort_table),
				   N8_ushort_table);

  if (caller) return caller(label,original);

  return 0;
}

/*------------------------------------------------------------------------*
 * ８近傍のラベリング。できあがりはlong型
 *------------------------------------------------------------------------*/
long image__N8_labelling_long(image label, image original)
{
  f_caller caller;

  caller = (f_caller)image__caller("N8_labelling_long",
				   original,
				   Number(N8_long_table),
				   N8_long_table);

  if (caller) return caller(label,original);

  return 0;
}



/*------------------------------------------------------------------------*
 * ８近傍のラベリング。
 *------------------------------------------------------------------------*/
long image__N8_labelling(image label, image original, long output_type)
{
  switch (output_type)
    {
    case UChar:
      return image__N8_labelling_uchar(label, original);
      break;
    case Short:
      return image__N8_labelling_short(label, original);
      break;
    case UShort:
      return image__N8_labelling_ushort(label, original);
      break;
    case Long:
      return image__N8_labelling_long(label, original);
      break;
    default:
      //fprintf(stderr, "image__N8_labelling:unsupported output type (%s)\n", typename(output_type));
      fprintf(stderr, "image__N8_labelling:unsupported output type (%d)\n", output_type);
      fprintf(stderr, "\toutput type is long.\n");
    }

  return image__N8_labelling_long(label, original);
}



static caller_table N8FG_uchar_table[] = {
  {Bit1,  (void*)image__N8_labelling_foreground_uchar_uchar},
  {Char,  (void*)image__N8_labelling_foreground_uchar_char},
  {UChar, (void*)image__N8_labelling_foreground_uchar_uchar},
  {Short, (void*)image__N8_labelling_foreground_uchar_short},
  {UShort,(void*)image__N8_labelling_foreground_uchar_ushort},
  {Long,  (void*)image__N8_labelling_foreground_uchar_long},
};

static caller_table N8FG_short_table[] = {
  {Bit1,  (void*)image__N8_labelling_foreground_short_uchar},
  {Char,  (void*)image__N8_labelling_foreground_short_char},
  {UChar, (void*)image__N8_labelling_foreground_short_uchar},
  {Short, (void*)image__N8_labelling_foreground_short_short},
  {UShort,(void*)image__N8_labelling_foreground_short_ushort},
  {Long,  (void*)image__N8_labelling_foreground_short_long},
};

static caller_table N8FG_ushort_table[] = {
  {Bit1,  (void*)image__N8_labelling_foreground_ushort_uchar},
  {Char,  (void*)image__N8_labelling_foreground_ushort_char},
  {UChar, (void*)image__N8_labelling_foreground_ushort_uchar},
  {Short, (void*)image__N8_labelling_foreground_ushort_short},
  {UShort,(void*)image__N8_labelling_foreground_ushort_ushort},
  {Long,  (void*)image__N8_labelling_foreground_ushort_long},
};

static caller_table N8FG_long_table[] = {
  {Bit1,  (void*)image__N8_labelling_foreground_long_uchar},
  {Char,  (void*)image__N8_labelling_foreground_long_char},
  {UChar, (void*)image__N8_labelling_foreground_long_uchar},
  {Short, (void*)image__N8_labelling_foreground_long_short},
  {UShort,(void*)image__N8_labelling_foreground_long_ushort},
  {Long,  (void*)image__N8_labelling_foreground_long_long},
};

/*------------------------------------------------------------------------*
 * ８近傍のラベリング。できあがりはuchar型
 *------------------------------------------------------------------------*/
long image__N8_labelling_foreground_uchar(image label, image original)
{
  f_caller caller;

  caller = (f_caller)image__caller("image__N8_labelling_foreground_uchar",
				   original,
				   Number(N8FG_uchar_table),
				   N8FG_uchar_table);

  if (caller) return caller(label,original);

  return 0;
}

/*------------------------------------------------------------------------*
 * ８近傍のラベリング。できあがりはshort型
 *------------------------------------------------------------------------*/
long image__N8_labelling_foreground_short(image label, image original)
{
  f_caller caller;

  caller = (f_caller)image__caller("image__N8_labelling_foreground_short",
				   original,
				   Number(N8FG_short_table),
				   N8FG_short_table);

  if (caller) return caller(label,original);

  return 0;
}

/*------------------------------------------------------------------------*
 * ８近傍のラベリング。できあがりはushort型
 *------------------------------------------------------------------------*/
long image__N8_labelling_foreground_ushort(image label, image original)
{
  f_caller caller;

  caller = (f_caller)image__caller("image__N8_labelling_foreground_ushort",
				   original,
				   Number(N8FG_ushort_table),
				   N8FG_ushort_table);

  if (caller) return caller(label,original);

  return 0;
}

/*------------------------------------------------------------------------*
 * ８近傍のラベリング。できあがりはlong型
 *------------------------------------------------------------------------*/
long image__N8_labelling_foreground_long(image label, image original)
{
  f_caller caller;

  caller = (f_caller)image__caller("image__N8_labelling_foreground_long",
				   original,
				   Number(N8FG_long_table),
				   N8FG_long_table);

  if (caller) return caller(label,original);

  return 0;
}


/*------------------------------------------------------------------------*
 * ８近傍のラベリング。
 *------------------------------------------------------------------------*/
long image__N8_labelling_foreground(image label, image original, long output_type)
{
  switch (output_type)
    {
    case UChar:
      return image__N8_labelling_foreground_uchar(label, original);
      break;
    case Short:
      return image__N8_labelling_foreground_short(label, original);
      break;
    case UShort:
      return image__N8_labelling_foreground_ushort(label, original);
      break;
    case Long:
      return image__N8_labelling_foreground_long(label, original);
      break;
    default:
      //fprintf(stderr, "image__N8_labelling_foreground:unsupported output type (%s)\n", typename(output_type));
      fprintf(stderr, "image__N8_labelling_foreground:unsupported output type (%d)\n", output_type);
      fprintf(stderr, "\toutput type is long.\n");
    }

  return image__N8_labelling_foreground_long(label, original);
}
