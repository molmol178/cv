/*
 * filename : labeling.c
 * author   : Takahiro Sugiyama
 * date     : Saturday, June 29 1996
 * describe : labeling image
 *

注意事項:

  これらのラベリング関数は、共存ラベル表に非常に多くのメモリを使用するので、
利用する時は注意が必要である。表に使用するメモリの計算方法は、
以下の通りである。

	area = max_label * max_label / 8 [byte]

また、max_labelの初期値は、

	long N4_labelling_max_label = 4096;
	long N8_labelling_max_label = 4096;

である。これは外部変数であるので変更が可能である。

 * support type:
 *      input: bit1, char, uchar, short, ushort, long
 *     output: short, long
 */


#include <stdio.h>
#include "Image.h"
#include "Image/Funcs.h"
#include "misc/memlib.h"


/* bit mask set */
static unsigned char bit_mask[] = { 0x01, 0x02, 0x04, 0x08,
				    0x10, 0x20, 0x40, 0x80 };

/* maximum label of N4 and N8 labelling */
long N4_labelling_max_label = 4096;
long N8_labelling_max_label = 4096;
long N4_and_N8_labelling_max_label = 4096;


/*------------------------------------------------------------------------*
 * 共有ラベルテーブルを表示する。
 *------------------------------------------------------------------------*/
static void print_coexist(unsigned char **coexist, long n)
{
  int i, j;

  if (n >= 80) return;

  for (i = 1; i < n; i++)
    {
      for (j = 1; j < n; j++)
	if (coexist[i][j/8] & bit_mask[j%8])
	  fprintf(stderr,"*");
	else
	  fprintf(stderr,".");
      fprintf(stderr,"\n");
    }
}


/*------------------------------------------------------------------------*
 * 共有ラベルテーブルを伝播する。
 *------------------------------------------------------------------------*/
static void propagate_coexist(unsigned char **coexist, long n)
{
  int i, j, k;
  int change = 1;
  int byte = (n + 8) / 8;

  while (change)
    {
      change = 0;
      for (i = 1; i <= n; i++)
	for (j = 1; j <= n; j++)
	if (coexist[i][j/8] & bit_mask[j%8])
	  for (k = 0; k < byte; k++)
	    {
	      if (coexist[i][k] != coexist[j][k]) change = 1;
	      coexist[i][k] |= coexist[j][k];
	    }
    }
}


/*------------------------------------------------------------------------*
 * 共有テーブルを参照して再ラベリングを行なう。
 *------------------------------------------------------------------------*/
#define DEFINE_RELABELLING(_dst_type) \
static long relabelling_with_coexist_##_dst_type(_dst_type **label_data,\
						 unsigned char **coexist_table,\
						 int n,\
						 long xsize, long ysize)\
{\
  _dst_type *label_table;\
  int x, y;\
  int i, j, nn;\
\
  propagate_coexist(coexist_table, n);\
  label_table = typenew1(n + 1, _dst_type);\
  for (i = 0; i <= n; i++) label_table[i] = i;\
  nn = 0;\
  for (i = 1; i <= n; i++)\
    {\
      if (label_table[i] != i) continue;\
\
      nn += 1;\
      label_table[i] = nn;\
      for (j = i + 1; j <= n; j++)\
	if (coexist_table[i][j/8] & bit_mask[j%8])\
	  label_table[j] = nn;\
    }\
\
  for (y = 0; y < ysize; y++)\
    for (x = 0; x < xsize; x++)\
      label_data[y][x] = label_table[label_data[y][x]];\
\
  typefree1(label_table);\
\
  return nn;\
}

DEFINE_RELABELLING(long)
DEFINE_RELABELLING(short)

/*------------------------------------------------------------------------*
 * 4近傍ラベリング 0 はラベル付けしない。
 * ラベルの数を返す。
 *------------------------------------------------------------------------*/
#define DEFINE_N4_LABELLING(_src_type,_dst_type,_DstType) \
long N4_labelling_##_src_type##_##_dst_type(image label, image original)\
{\
  long x, y, xsize, ysize;\
  long n, p0, p3, p5, l3, l5;\
  _src_type **data;\
  _dst_type **label_data;\
  unsigned char **coexist_table;\
\
  xsize = __XSIZE(original);\
  ysize = __YSIZE(original);\
\
  Image.make(label, _DstType, xsize, ysize);\
\
  /* 共有ラベル表を作成する。*/\
  coexist_table = typenew2((N4_labelling_max_label + 1 + 7)/8,\
			   N4_labelling_max_label + 1, unsigned char);\
\
  label_data = __DATA(label, _dst_type);\
  data = __DATA(original, _src_type);\
\
  for (y = 0; y < ysize; y++)\
    for (x = 0; x < xsize; x++)\
      label_data[y][x] = 0;\
\
  n = 0;\
  for (y = 0; y < ysize; y++)\
    for (x = 0; x < xsize; x++)\
      {\
	p0 = data[y][x];\
	if (p0 == 0) continue;\
\
	p3 = (y > 0) ? data[y-1][x] : 0;\
	p5 = (x > 0) ? data[y][x-1] : 0;\
\
	if ((p3 != p0) && (p5 != p0))\
	  {/* 新しいラベル */\
	    n += 1;\
	    label_data[y][x] = n;\
	    if (n > N4_labelling_max_label)\
	      {\
		fprintf(stderr,"warning:label number is too large!\n");\
		fprintf(stderr,"        soon core dump!\n");\
	      }\
	    continue;\
	  }\
	else\
	  {\
	    l3 = (y > 0) ? label_data[y-1][x] : 0;\
	    l5 = (x > 0) ? label_data[y][x-1] : 0;\
	    if ((p0 == p3) && (p0 == p5))\
	      {\
		label_data[y][x] = l3;\
		coexist_table[l5][l3/8] |= bit_mask[l3%8];\
		coexist_table[l3][l5/8] |= bit_mask[l5%8];\
	      }\
	    else\
	    if (p0 == p3) label_data[y][x] = l3; else\
	    if (p0 == p5) label_data[y][x] = l5;\
	  }\
      }\
\
  /* 再ラベル付け */\
  n = relabelling_with_coexist_##_dst_type(label_data, coexist_table, n, xsize, ysize);\
\
  typefree2(coexist_table);\
\
  return n;\
}


/*------------------------------------------------------------------------*
 * 8近傍ラベリング 0 はラベル付けしない。
 * ラベルの数を返す。
 *------------------------------------------------------------------------*/
#define DEFINE_N8_LABELLING(_src_type,_dst_type,_DstType) \
long N8_labelling_##_src_type##_##_dst_type(image label, image original)\
{\
  long x, y, xsize, ysize;\
  long n, p0, p2, p3, p4, p5, l2, l3, l4, l5;\
  _src_type **data;\
  _dst_type **label_data;\
  unsigned char **coexist_table;\
\
  xsize = __XSIZE(original);\
  ysize = __YSIZE(original);\
\
  Image.make(label, _DstType, xsize, ysize);\
\
  /* 共有ラベル表を作成する。*/\
  coexist_table = typenew2((N8_labelling_max_label + 1 + 7)/8,\
			   N8_labelling_max_label + 1, unsigned char);\
\
  label_data = __DATA(label, _dst_type);\
  data = __DATA(original, _src_type);\
\
  for (y = 0; y < ysize; y++)\
    for (x = 0; x < xsize; x++)\
      label_data[y][x] = 0;\
\
  n = 0;\
  for (y = 0; y < ysize; y++)\
    for (x = 0; x < xsize; x++)\
      {\
	p0 = data[y][x];\
	if (p0 == 0) continue;\
\
	p2 = (y > 0 && x < xsize-1) ? data[y-1][x+1] : 0;\
	p3 = (y > 0) ? data[y-1][x] : 0;\
	p4 = (y > 0 && x > 0) ? data[y-1][x-1] : 0;\
	p5 = (x > 0) ? data[y][x-1] : 0;\
\
	if ((p2 != p0) && (p3 != p0) && (p4 != p0) && (p5 != p0))\
	  {/* 新しいラベル */\
	    n += 1;\
	    label_data[y][x] = n;\
	    if (n > N8_labelling_max_label)\
	      {\
		fprintf(stderr,"warning:label number is too large!\n");\
		fprintf(stderr,"        soon core dump!\n");\
	      }\
	    continue;\
	  }\
	else\
	  {\
	    l2 = (y > 0 && x < xsize-1) ? label_data[y-1][x+1] : 0;\
	    l3 = (y > 0) ? label_data[y-1][x] : 0;\
	    l4 = (y > 0 && x > 0) ? label_data[y-1][x-1] : 0;\
	    l5 = (x > 0) ? label_data[y][x-1] : 0;\
	    if ((p0 == p2) && (p0 == p4))\
	      {\
		label_data[y][x] = l2;\
		coexist_table[l2][l4/8] |= bit_mask[l4%8];\
		coexist_table[l4][l2/8] |= bit_mask[l2%8];\
	      }\
	    else\
	    if ((p0 == p2) && (p0 == p5))\
	      {\
		label_data[y][x] = l2;\
		coexist_table[l2][l5/8] |= bit_mask[l5%8];\
		coexist_table[l5][l2/8] |= bit_mask[l2%8];\
	      }\
	    else\
	    if ((p0 == p3) && (p0 == p5))\
	      {/* 4 neighborhood only ? */\
		label_data[y][x] = l3;\
		coexist_table[l3][l5/8] |= bit_mask[l5%8];\
		coexist_table[l5][l3/8] |= bit_mask[l3%8];\
	      }\
	    else\
	    if (p0 == p2) label_data[y][x] = l2; else\
	    if (p0 == p3) label_data[y][x] = l3; else\
	    if (p0 == p4) label_data[y][x] = l4; else\
	    if (p0 == p5) label_data[y][x] = l5;\
	  }\
      }\
\
  /* 再ラベル付け */\
  n = relabelling_with_coexist_##_dst_type(label_data, coexist_table, n, xsize, ysize);\
\
  typefree2(coexist_table);\
\
  return n;\
}


/*------------------------------------------------------------------------*
 * 8-4近傍ラベリング 0 はラベル付けしない。
 * マイナスの値ならばN8ラベリングを行ない、
 * プラスの値ならばN4ラベリングを行なう。
 * ラベルの数を返す。
 *------------------------------------------------------------------------*/
#define DEFINE_N4_AND_N8_LABELLING(_src_type,_dst_type,_DstType) \
long N4_and_N8_labelling_##_src_type##_##_dst_type(image label, image original)\
{\
  long x, y, xsize, ysize;\
  long n, p0, p2, p3, p4, p5, l2, l3, l4, l5;\
  _src_type **data;\
  _dst_type **label_data;\
  unsigned char **coexist_table;\
\
  xsize = __XSIZE(original);\
  ysize = __YSIZE(original);\
\
  Image.make(label, _DstType, xsize, ysize);\
\
  /* 共有ラベル表を作成する。*/\
  coexist_table = typenew2((N4_and_N8_labelling_max_label + 1 + 7)/8,\
			   N4_and_N8_labelling_max_label + 1, unsigned char);\
\
  label_data = __DATA(label, _dst_type);\
  data = __DATA(original, _src_type);\
\
  for (y = 0; y < ysize; y++)\
    for (x = 0; x < xsize; x++)\
      label_data[y][x] = 0;\
\
  n = 0;\
  for (y = 0; y < ysize; y++)\
    for (x = 0; x < xsize; x++)\
      {\
	p0 = data[y][x];\
	if (p0 == 0) continue;\
\
	p3 = (y > 0) ? data[y-1][x] : 0;\
	p5 = (x > 0) ? data[y][x-1] : 0;\
\
	if (p0 < 0)\
	  {/* label is minus, N8 labelling */\
	    p2 = (y > 0 && x < xsize-1) ? data[y-1][x+1] : 0;\
	    p4 = (y > 0 && x > 0) ? data[y-1][x-1] : 0;\
	    if ((p2 != p0) && (p3 != p0) && (p4 != p0) && (p5 != p0))\
	      {/* 新しいラベル */\
		n += 1;\
		label_data[y][x] = n;\
		if (n > N8_labelling_max_label)\
		  {\
		    fprintf(stderr,"warning:label number is too large!\n");\
		    fprintf(stderr,"        soon core dump!\n");\
		  }\
		continue;\
	      }\
	    else\
	      {\
		l2 = (y > 0 && x < xsize-1) ? label_data[y-1][x+1] : 0;\
		l3 = (y > 0) ? label_data[y-1][x] : 0;\
		l4 = (y > 0 && x > 0) ? label_data[y-1][x-1] : 0;\
		l5 = (x > 0) ? label_data[y][x-1] : 0;\
		if ((p0 == p2) && (p0 == p4))\
		  {\
		    label_data[y][x] = l2;\
		    coexist_table[l2][l4/8] |= bit_mask[l4%8];\
		    coexist_table[l4][l2/8] |= bit_mask[l2%8];\
		  }\
		else\
		if ((p0 == p2) && (p0 == p5))\
		  {\
		    label_data[y][x] = l2;\
		    coexist_table[l2][l5/8] |= bit_mask[l5%8];\
		    coexist_table[l5][l2/8] |= bit_mask[l2%8];\
		  }\
		else\
		if ((p0 == p3) && (p0 == p5))\
		  {/* 4 neighborhood only ? */\
		    label_data[y][x] = l3;\
		    coexist_table[l3][l5/8] |= bit_mask[l5%8];\
		    coexist_table[l5][l3/8] |= bit_mask[l3%8];\
		  }\
		else\
		if (p0 == p2) label_data[y][x] = l2; else\
		if (p0 == p3) label_data[y][x] = l3; else\
		if (p0 == p4) label_data[y][x] = l4; else\
		if (p0 == p5) label_data[y][x] = l5;\
	      }\
	  }\
	else\
	  {/* p0 is plus, N4 labelling */\
	    if ((p3 != p0) && (p5 != p0))\
	      {/* 新しいラベル */\
		n += 1;\
		label_data[y][x] = n;\
		continue;\
	      }\
	    else\
	      {\
		l3 = (y > 0) ? label_data[y-1][x] : 0;\
		l5 = (x > 0) ? label_data[y][x-1] : 0;\
		if ((p0 == p3) && (p0 == p5))\
		  {\
		    label_data[y][x] = l3;\
		    coexist_table[l5][l3/8] |= bit_mask[l3%8];\
		    coexist_table[l3][l5/8] |= bit_mask[l5%8];\
		  }\
		else\
		if (p0 == p3) label_data[y][x] = l3; else\
		if (p0 == p5) label_data[y][x] = l5;\
	      }\
	  }\
      }\
\
  /* 再ラベル付け */\
  n = relabelling_with_coexist_##_dst_type(label_data, coexist_table, n, xsize, ysize);\
\
  typefree2(coexist_table);\
\
  return n;\
}

DEFINE_N4_LABELLING(char,short,Short)
DEFINE_N4_LABELLING(uchar,short,Short)
DEFINE_N4_LABELLING(short,short,Short)
DEFINE_N4_LABELLING(ushort,short,Short)
DEFINE_N4_LABELLING(long,short,Short)

DEFINE_N4_LABELLING(char,long,Long)
DEFINE_N4_LABELLING(uchar,long,Long)
DEFINE_N4_LABELLING(short,long,Long)
DEFINE_N4_LABELLING(ushort,long,Long)
DEFINE_N4_LABELLING(long,long,Long)


DEFINE_N8_LABELLING(char,short,Short)
DEFINE_N8_LABELLING(uchar,short,Short)
DEFINE_N8_LABELLING(short,short,Short)
DEFINE_N8_LABELLING(ushort,short,Short)
DEFINE_N8_LABELLING(long,short,Short)

DEFINE_N8_LABELLING(char,long,Long)
DEFINE_N8_LABELLING(uchar,long,Long)
DEFINE_N8_LABELLING(short,long,Long)
DEFINE_N8_LABELLING(ushort,long,Long)
DEFINE_N8_LABELLING(long,long,Long)


DEFINE_N4_AND_N8_LABELLING(char,short,Short)
DEFINE_N4_AND_N8_LABELLING(uchar,short,Short)
DEFINE_N4_AND_N8_LABELLING(short,short,Short)
DEFINE_N4_AND_N8_LABELLING(ushort,short,Short)
DEFINE_N4_AND_N8_LABELLING(long,short,Short)

DEFINE_N4_AND_N8_LABELLING(char,long,Long)
DEFINE_N4_AND_N8_LABELLING(uchar,long,Long)
DEFINE_N4_AND_N8_LABELLING(short,long,Long)
DEFINE_N4_AND_N8_LABELLING(ushort,long,Long)
DEFINE_N4_AND_N8_LABELLING(long,long,Long)


typedef long (*f_caller) P ((image dest,image src));

static caller_table N4_short_table[] = {
  {Bit1,  (void*)N4_labelling_uchar_short},
  {Char,  (void*)N4_labelling_char_short},
  {UChar, (void*)N4_labelling_uchar_short},
  {Short, (void*)N4_labelling_short_short},
  {UShort,(void*)N4_labelling_ushort_short},
  {Long,  (void*)N4_labelling_long_short},
};

/*------------------------------------------------------------------------*
 * ４近傍のラベリング。できあがりはshort型
 *------------------------------------------------------------------------*/
long N4_labelling_short(image label, image original)
{
  f_caller caller;

  caller = (f_caller)image__caller("N4_labelling_short",
				   original,
				   Number(N4_short_table),N4_short_table);

  if (caller) return caller(label,original);

  return 0;
}



static caller_table N8_short_table[] = {
  {Bit1,  (void*)N8_labelling_uchar_short},
  {Char,  (void*)N8_labelling_char_short},
  {UChar, (void*)N8_labelling_uchar_short},
  {Short, (void*)N8_labelling_short_short},
  {UShort,(void*)N8_labelling_ushort_short},
  {Long,  (void*)N8_labelling_long_short},
};

/*------------------------------------------------------------------------*
 * ８近傍のラベリング。できあがりはshort型
 *------------------------------------------------------------------------*/
long N8_labelling_short(image label, image original)
{
  f_caller caller;

  caller = (f_caller)image__caller("N8_labelling_short",
				   original,
				   Number(N8_short_table),N8_short_table);

  if (caller) return caller(label,original);

  return 0;
}



static caller_table N48_short_table[] = {
  {Bit1,  (void*)N4_and_N8_labelling_uchar_short},
  {Char,  (void*)N4_and_N8_labelling_char_short},
  {UChar, (void*)N4_and_N8_labelling_uchar_short},
  {Short, (void*)N4_and_N8_labelling_short_short},
  {UShort,(void*)N4_and_N8_labelling_ushort_short},
  {Long,  (void*)N4_and_N8_labelling_long_short},
};

/*------------------------------------------------------------------------*
 * ４近傍と８近傍のラベリング。できあがりはshort型
 *------------------------------------------------------------------------*/
long N4_and_N8_labelling_short(image label, image original)
{
  f_caller caller;

  caller = (f_caller)image__caller("N4_and_N8_labelling_short",
				   original,Number(N48_short_table),
				   N48_short_table);

  if (caller) return caller(label,original);

  return 0;
}



static caller_table N4_long_table[] = {
  {Bit1,  (void*)N4_labelling_uchar_long},
  {Char,  (void*)N4_labelling_char_long},
  {UChar, (void*)N4_labelling_uchar_long},
  {Short, (void*)N4_labelling_short_long},
  {UShort,(void*)N4_labelling_ushort_long},
  {Long,  (void*)N4_labelling_long_long},
};

/*------------------------------------------------------------------------*
 * ４近傍のラベリング。できあがりはlong型
 *------------------------------------------------------------------------*/
long N4_labelling_long(image label, image original)
{
  f_caller caller;

  caller = (f_caller)image__caller("N4_labelling_long",
				   original,
				   Number(N4_long_table),N4_long_table);

  if (caller) return caller(label,original);

  return 0;
}



static caller_table N8_long_table[] = {
  {Bit1,  (void*)N8_labelling_uchar_long},
  {Char,  (void*)N8_labelling_char_long},
  {UChar, (void*)N8_labelling_uchar_long},
  {Short, (void*)N8_labelling_short_long},
  {UShort,(void*)N8_labelling_ushort_long},
  {Long,  (void*)N8_labelling_long_long},
};

/*------------------------------------------------------------------------*
 * ８近傍のラベリング。できあがりはlong型
 *------------------------------------------------------------------------*/
long N8_labelling_long(image label, image original)
{
  f_caller caller;

  caller = (f_caller)image__caller("N8_labelling_long",
				   original,
				   Number(N8_long_table),N8_long_table);

  if (caller) return caller(label,original);

  return 0;
}



static caller_table N48_long_table[] = {
  {Bit1,  (void*)N4_and_N8_labelling_uchar_long},
  {Char,  (void*)N4_and_N8_labelling_char_long},
  {UChar, (void*)N4_and_N8_labelling_uchar_long},
  {Short, (void*)N4_and_N8_labelling_short_long},
  {UShort,(void*)N4_and_N8_labelling_ushort_long},
  {Long,  (void*)N4_and_N8_labelling_long_long},
};

/*------------------------------------------------------------------------*
 * ４近傍と８近傍のラベリング。できあがりはlong型
 *------------------------------------------------------------------------*/
long N4_and_N8_labelling_long(image label, image original)
{
  f_caller caller;

  caller = (f_caller)image__caller("N4_and_N8_labelling_long",
				   original,Number(N48_long_table),
				   N48_long_table);

  if (caller) return caller(label,original);

  return 0;
}



/*------------------------------------------------------------------------*
 * ４近傍のラベリング。
 *------------------------------------------------------------------------*/
long N4_labelling(image label, image original, long type)
{
  long n;

  switch (type)
    {
    case Long:
      n = N4_labelling_long(label, original);
      break;
    case Short:
      n = N4_labelling_short(label, original);
      break;
    default:
      n = N4_labelling_short(label, original);
      break;
    }
  return n;
}

/*------------------------------------------------------------------------*
 * ８近傍のラベリング。
 *------------------------------------------------------------------------*/
long N8_labelling(image label, image original, long type)
{
  long n;

  switch (type)
    {
    case Long:
      n = N8_labelling_long(label, original);
      break;
    case Short:
      n = N8_labelling_short(label, original);
      break;
    default:
      n = N8_labelling_short(label, original);
      break;
    }
  return n;
}

/*------------------------------------------------------------------------*
 * ４近傍と８近傍のラベリング。
 *------------------------------------------------------------------------*/
long N4_and_N8_labelling(image label, image original, long type)
{
  long n;

  switch (type)
    {
    case Long:
      n = N4_and_N8_labelling_long(label, original);
      break;
    case Short:
      n = N4_and_N8_labelling_short(label, original);
      break;
    default:
      n = N4_and_N8_labelling_short(label, original);
      break;
    }
  return n;
}
