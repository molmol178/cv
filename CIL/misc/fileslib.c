/*
 * filename : fileslib.c
 * author   : Takahiro Sugiyama
 * date     : 1993/02/22
 * date     : 1993/09/28
 */



#include "fileslib.h"
#include <stdio.h>
#include <string.h>
#include "memlib.h"
#include "typelib.h"



typedef struct {
  FILE *fp;
  char *filename;
  long lines;
} stackRec;



struct _FILES_REC {
  long num;
  stackRec *stack;
  stackRec *top;
};



static char *__filestopopen
  _P1 (( FILES *, fp ))
{
  if ( fp->top == 0 ) return 0;
  if ( fp->top->fp ) return fp->top->filename;

  do
    {
      fp->top->fp = fopen( fp->top->filename, "r" );
      if ( fp->top->fp == 0 )
	{
	  char *filename = filespop( fp );
	  fprintf( stderr,
		   "warning:filesopen:can't open file (%s)\n", filename );
	  if ( fp->top == 0 ) return 0;
	}
    }
  while ( fp->top->fp == 0 );
  fp->top->lines = 0;

  return fp->top->filename;
}



static void __filestopclose
  _P1 (( FILES *, fp ))
{
  if ( fp->top && fp->top->fp ) fclose( fp->top->fp );
}



FILES *filesopen
  _P2 (( long   , filename_num  ),
       ( char **, filename_list ))
{
  long i;
  FILES *fp;

  fp = typenew( FILES );
  fp->num = 0;
  fp->stack = 0;

  for ( i = filename_num - 1; i >= 0; i-- )
    filespush( fp, filename_list[ i ] );
  if ( __filestopopen( fp ) == 0 )
    {
      typefree( fp );
      return 0;
    }

  return fp;
}



void filesclose
  _P1 (( FILES *, fp ))
{
  if ( fp->top ) fclose( fp->top->fp );
  typefree1( fp->stack );
  typefree( fp );
}



void filespush
  _P2 (( FILES *, fp       ),
       ( char * , filename ))
{
  long index;

  index = fp->num++;

  fp->stack = typerenew1( fp->stack, fp->num, stackRec );

  fp->top = &fp->stack[ index ];
  fp->top->fp = 0;
  fp->top->filename = filename;
  fp->top->lines = 0;
}



char *filespop
  _P1 (( FILES *, fp ))
{
  long index;
  char *filename;

  if ( fp->num == 0 ) return 0;

  filename = fp->top->filename;

  fp->num--;
  index = fp->num - 1;
  if (fp->num <= 0)
    {
      typefree1( fp->stack );
      fp->stack = 0;
      fp->top = 0;
      return filename;
    }
  fp->stack = typerenew1( fp->stack, fp->num, stackRec );
  fp->top = &fp->stack[ index ];

  return filename;
}



char *filesname
  _P1 (( FILES *, fp ))
{
  return fp->top->filename;
}



long fileslines
  _P1 (( FILES *, fp ))
{
  return fp->top->lines;
}



char filesgetc
  _P1 (( FILES *, fp ))
{
  long c;
  char *err;

  if ( fp->top->fp == 0 ) __filestopopen( fp );

  c = fgetc( fp->top->fp );

  if ( c == '\n' ) fp->top->lines++;

  while ( c == EOF )
    {
      __filestopclose( fp );
      filespop( fp );
      err = __filestopopen( fp );
      if ( err == 0 ) return 0;
      c = fgetc( fp->top->fp );
      if ( c == '\n' ) fp->top->lines++;
    }

  return c;
}



char filesungetc
  _P2 (( FILES *, fp ),
       ( long   , c  ))
{
  ungetc( c, fp->top->fp );
}



char *filesgets
  _P3 (( FILES *, fp  ),
       ( char * , buf ),
       ( long   , n   ))
{
  char *err;
  
  if ( fp->top->fp == 0 ) __filestopopen( fp );

  fp->top->lines++;

  while (! ( err = fgets( buf, n, fp->top->fp ) ) )
    {
      __filestopclose( fp );
      filespop( fp );
      err = __filestopopen( fp );
      if ( err == 0 ) return 0;
      fp->top->lines++;
    }

  return err;
}



long filesreadbyte
  _P3 (( FILES *, fp    ),
       ( char * , buf   ),
       ( long   , bytes ))
{
  char *err;
  long n;

  if (fp->top->fp == 0) __filestopopen( fp );
  
  while (! ( n = fread( buf, 1, bytes, fp->top->fp ) ) )
    {
      __filestopclose( fp );
      filespop( fp );
      err = __filestopopen( fp );
      if ( err == 0 ) return 0;
    }

  return n;
}



long filesread
  _P4 (( FILES *, fp        ),
       ( char * , buf       ),
       ( long   , elem_num  ),
       ( long   , elem_size ))
{
  char *err;
  long n;

  if ( fp->top->fp == 0 ) __filestopopen( fp );

  while (! (n = fread( buf, elem_size, elem_num, fp->top->fp ) ) )
    {
      __filestopclose( fp );
      filespop( fp );
      err = __filestopopen( fp );
      if ( err == 0 ) return 0;
    }

  return n;
}
