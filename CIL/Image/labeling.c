/*
 * labeling.c
 */



#include "Funcs.h"
#include <stdio.h>



static long Table_num = 0;
static long Table_offset = 0;
static long *Table = 0;
static long *labelTable = 0;

static void same_table P ((long a,long b));
static long new_label P ((void));
static long table_check P ((void));



#define DEFINE_LABELING(TYPE_NAME,TYPE) \
static long labeling_##TYPE(dst,src)\
     image dst;\
     image src;\
{\
  register long x, y, xsize, ysize, num;\
  register TYPE **src_data;\
  register long **dst_data;\
\
  image__type_check( "image__labeling", src, TYPE_NAME );\
\
  xsize = __XSIZE( src );\
  ysize = __YSIZE( src );\
  Image.make( dst, Long, xsize, ysize );\
\
  src_data = __DATA( src, TYPE );\
  dst_data = __DATA( dst, long );\
\
  for ( y = 0; y < ysize; y++ )\
    for ( x = 0; x < xsize; x++ )\
      {\
	register long val = src_data[ y ][ x ];\
	if ( x == 0 && y == 0 )\
	  {\
	    dst_data[ y ][ x ] = new_label();\
	    continue;\
	  }\
\
	if ( x == 0 )\
	  {\
	    if ( val == src_data[ y - 1 ][ x ] )\
	      dst_data[ y ][ x ] = dst_data[ y - 1 ][ x ];\
	    else\
	      dst_data[ y ][ x ] = new_label();\
	    continue;\
	  }\
\
	if ( y == 0 )\
	  {\
	    if ( val == src_data[ y ][ x - 1 ] )\
	      dst_data[ y ][ x ] = dst_data[ y ][ x - 1 ];\
	    else\
	      dst_data[ y ][ x ] = new_label();\
	    continue;\
	  }\
\
	if ( val == src_data[ y - 1 ][ x ] &&\
	     val == src_data[ y ][ x - 1 ] )\
	  {\
	    same_table( dst_data[ y ][ x - 1 ], dst_data[ y - 1 ][ x ] );\
	    dst_data[ y ][ x ] = dst_data[ y - 1 ][ x ];\
	    continue;\
	  }\
\
	if ( val == src_data[ y - 1 ][ x ] )\
	  {\
	    dst_data[ y ][ x ] = dst_data[ y - 1 ][ x ];\
	    continue;\
	  }\
\
	if ( val == src_data[ y ][ x - 1 ])\
	  {\
	    dst_data[ y ][ x ] = dst_data[ y ][ x - 1 ];\
	    continue;\
	  }\
\
	  {\
	    dst_data[ y ][ x ] = new_label();\
	    continue;\
	  }\
      }\
\
  num = table_check();\
\
  {\
    register *label = labelTable;\
    register *table = Table;\
    for ( y = 0; y < ysize; y++ )\
      for ( x = 0; x < xsize; x++ )\
        dst_data[ y ][ x ] = label[ table[ dst_data[ y ][ x ] ] ];\
  }\
\
  if ( Table_num )\
    {\
      free( Table );\
      free( labelTable );\
      Table = 0;\
      labelTable = 0;\
      Table_num = 0;\
      Table_offset = 0;\
    }\
\
  return num;\
}



DEFINE_LABELING(Long,long)
DEFINE_LABELING(UChar,uchar)
DEFINE_LABELING(Bit1,bit1)
DEFINE_LABELING(Char,char)
DEFINE_LABELING(UShort,ushort)
DEFINE_LABELING(Short,short)



typedef long (*f_caller) P ((image dest,image src));



static caller_table table2[] = {
  {Long,  labeling_long},
  {UChar, labeling_uchar},
  {Bit1,  labeling_bit1},
  {Char,  labeling_char},
  {UShort,labeling_ushort},
  {Short, labeling_short},
};



long image__labeling(dst,src)
     image dst;
     image src;
{
  f_caller caller;

  caller = (f_caller)image__caller("image__labeling",src,Number(table2),table2);

  if (caller) return caller(dst,src);

  return 0;
}



static long new_label()
{
  register long new = Table_offset;
  register long *old_table;

  Table_offset++;
  if ( Table_offset >= Table_num )
    {
      Table_num += 512;
      old_table = Table;
      Table = ( long * )calloc( Table_num, sizeof( long ) );
      memcpy( Table, old_table, sizeof( long ) * new );
      free( old_table );
    }

  if ( Table == 0 )
    {
      fprintf( stderr, "can't alloc memory.\n" );
      fprintf( stderr, "can't create new label.new_label-long\n" );
      exit(-1);
    }

  Table[ new ] = new;

  return new;
}



static long table_check()
{
  register long i, index;
  register long *table, *label;

  table = Table;
  for ( i = 0; i < Table_offset; i++ )
    {
      index = i;
      while ( table[ index ] != index ) index = table[ index ];
      table[i] = index;
    }

  labelTable = ( long * )calloc( Table_offset, sizeof( long ) );
  if ( labelTable == 0 )
    {
      fprintf( stderr, "can't alloc memory. It's too large label table.\n" );
      exit( -1 );
    }

  label = labelTable;
  index = 1;
  for ( i = 0; i < Table_offset; i++ )
    {
      if ( table[i] == i )
	{
	  label[ i ] = index;
	  index++;
	}
    }

  return index - 1;
}



static void same_table(a,b)
     register long a,b;
{
  register int ai, bi;
  register long *table = Table;

  if ( a == b ) return;

  ai = a;
  while ( table[ ai ] != ai ) ai = table[ ai ];

  bi = b;
  while ( table[ bi ] != bi ) bi = table[ bi ];

  if ( ai == bi ) return;

  table[ bi ] = ai;
}
