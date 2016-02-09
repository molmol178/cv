/*
 * filename : memlib.c
 * author   : Takahiro Sugiyama
 * date     : 1993/09/13
 */



#include "memlib.h"
#include <stdio.h>
//#include <malloc.h>



/*=====================================================*
 * MEMLIB_ERROR : メモリが確保できなかったとき終了する
 *=====================================================*/
long MEMLIB_ERROR = 1;



/*=============================*
 * memnew : メモリを確保する。
 *=============================*/
char *memnew /* ポインタ */
  _P1 (( long, size )) /* バイト数 */
{
  char *ptr;

  if ( size <= 0 ) return 0;

  ptr = malloc( size );
  if (( ptr == 0 ) && MEMLIB_ERROR )
    {
      fprintf( stderr, "error:memnew:can't alloc memory\n" );
      fprintf( stderr, "    memnew(%d)\n", size );
      exit( -1 );
    }

  return ptr;
}



/*==============================*
 * memfree : メモリを開放する。
 *==============================*/
void memfree
  _P1 (( char *, ptr )) /* ポインタ */
{
  if ( ptr ) free( ptr );
}



/*=================================*
 * memrenew : メモリを再確保する。
 *=================================*/
char *memrenew /* ポインタ */
  _P2 (( char *, ptr  ), /* ポインタ */
       ( long  , size )) /* バイト数 */
{
  if ( ptr == 0 ) return memnew( size );
  if ( size <= 0 )
    {
      memfree( ptr );
      return 0;
    }

  ptr = realloc( ptr, size );
  if (( ptr == 0 ) && MEMLIB_ERROR )
    {
      fprintf( stderr, "error:memrenew:can't alloc memory\n" );
      fprintf( stderr, "    memnew(0x%08x,%d)\n", ptr, size );
      exit( -1 );
    }

  return ptr;
}



/*================================*
 * memcopy : メモリをコピーする。
 *================================*/
void memcopy
  _P3 (( char *, dest ), /* ポインタ */
       ( char *, src  ), /* ポインタ */
       ( long  , size )) /* バイト数 */
{
  if (( dest == 0 ) || ( src == 0 ) || ( size <= 0 )) return;
  memcpy( dest, src, size );
}



/*============================*
 * memfill : メモリを埋める。
 *============================*/
void memfill
  _P3 (( char *, dest ), /* ポインタ   */
       ( long  , c    ), /* 埋める文字 */
       ( long  , size )) /* バイト数   */
{
  if (( dest == 0 ) || ( size <= 0 )) return;
  memset( dest, c, size );
}



/*========================================*
 * memnew1 : 一次元配列のメモリを確保する。
 *========================================*/
char *memnew1 /* 一次元配列のポインタ */
  _P2 (( long, num  ), /* 要素数     */
       ( long, size )) /* 要素サイズ */
{
  char *ptr;

  if (( size <= 0 ) || ( num <= 0 )) return 0;

  //  fprintf(stderr,"calloc/");
  ptr = calloc( num, size );
  //fprintf(stderr,"..");

  if (( ptr == 0 ) && MEMLIB_ERROR )
    {
      fprintf( stderr, "error:memnew1:can't alloc memory\n" );
      fprintf( stderr, "    memnew1(%d,%d)\n", num, size );
      exit( -1 );
    }

  return ptr;
}



/*========================================*
 * memfree1 : 一次元配列のメモリを開放する。
 *========================================*/
void memfree1
  _P1 (( char *, ptr )) /* 一次元配列のポインタ */
{
  if ( ptr == 0 ) return;
  free( ptr );
}



/*=============================================*
 * memrenew1 : 一次元配列のメモリを再確保する。
 *=============================================*/
char *memrenew1 /* 一次元配列のポインタ */
  _P3 (( char *, ptr ), /* 一次元配列のポインタ */
       ( long  , num ), /* 要素数               */
       ( long  , size)) /* 要素サイズ           */
{
  if ( ptr == 0 ) return memnew1( num, size );
  if (( size <= 0 ) || ( num <= 0 ))
    {
      memfree1( ptr );
      return 0;
    }

  fprintf(stderr,"memrenew1:realloc(0x%08x, %d);", ptr, num*size);

  ptr = realloc( ptr, num * size );
  if (( ptr == 0 ) && MEMLIB_ERROR )
    {
      fprintf( stderr, "error:memrenew1:can't alloc memory\n" );
      fprintf( stderr, "    memrenew1(0x%08x,%d,%d)\n", ptr, num, size );
      exit( -1 );
    }

  fprintf(stderr, "return 0x%08x;\n", ptr);

  return ptr;
}



/*============================================*
 * memcopy1 : 一次元配列のメモリをコピーする。
 *============================================*/
void memcopy1
  _P4 (( char *, dest ), /* 一次元配列のポインタ */
       ( char *, src  ), /* 一次元配列のポインタ */
       ( long  , num  ), /* 要素数               */
       ( long  , size )) /* 要素サイズ           */
{
  if (( dest == 0 ) || ( src == 0 ) || ( num <= 0 ) || ( size <= 0 )) return;
  memcpy( dest, src, num * size );
}



/*==============================================*
 * memfill1 : 一次元配列のメモリの内容を埋める。
 *==============================================*/
void memfill1
  _P4 (( char *, dest ), /* 一次元配列のポインタ */
       ( long  , c    ), /* 埋める文字           */
       ( long  , num  ), /* 要素数               */
       ( long  , size )) /* 要素サイズ           */
{
  if (( dest == 0 ) || ( num <= 0 ) || ( size <= 0 )) return;
  memset( dest, c, num * size );
}



/*=========================================*
 * memnew2 : 二次元配列のメモリを確保する。
 *=========================================*/
char **memnew2 /* 二次元配列のポインタ */
  _P3 (( long, xnum ), /* Ｘの要素数 */
       ( long, ynum ), /* Ｙの要素数 */
       ( long, size )) /* 要素サイズ */
{
  int i;
  char **ptr;

  if (( xnum <= 0 ) || ( ynum <= 0 ) || ( size <= 0 )) return 0;

  ptr = ( char ** )memnew1( ynum, sizeof( char * ));
  ptr[ 0 ] = ( char * )memnew1( ynum, xnum * size );

  for ( i = 1; i < ynum; i++ ) ptr[ i ] = ptr[ i - 1 ] + xnum * size;

  return ptr;
}



/*==========================================*
 * memfree2 : 二次元配列のメモリを開放する。
 *==========================================*/
void memfree2
  _P1 (( char **, ptr )) /* 二次元配列のポインタ */
{
  if ( ptr == 0 ) return;
  memfree1( ptr[ 0 ] );
  memfree1(( char * )ptr );
}



/*============================================*
 * memcopy2 : 二次元配列のメモリをコピーする。
 *============================================*/
void memcopy2
  _P5 (( char **, dest ), /* 二次元配列のポインタ */
       ( char **, src  ), /* 二次元配列のポインタ */
       ( long   , xnum ), /* Ｘの要素数           */
       ( long   , ynum ), /* Ｙの要素数           */
       ( long   , size )) /* 要素サイズ           */
{
  if (( dest == 0 ) || ( src == 0 )) return;
  if (( xnum <= 0 ) || ( ynum <= 0 ) || ( size <= 0 )) return;
  memcopy1( dest[ 0 ], src[ 0 ], xnum * ynum, size );
}



/*========================================*
 * memfill2 : 二次元配列のメモリを埋める。
 *========================================*/
void memfill2
  _P5 (( char **, dest ), /* 二次元配列のポインタ */
       ( long   , c    ), /* 埋める文字           */
       ( long   , xnum ), /* Ｘの要素数           */
       ( long   , ynum ), /* Ｙの要素数           */
       ( long   , size )) /* 要素サイズ           */
{
  if (( dest == 0 ) || ( xnum <= 0 ) || ( ynum <= 0 ) || ( size <= 0 )) return;
  memset( dest, c, ynum * xnum * size );
}




/*=========================================*
 * memnew3 : 三次元配列のメモリを確保する。
 *=========================================*/
char ***memnew3 /* 三次元配列のポインタ */
  _P4 (( long, xnum ), /* Ｘの要素数 */
       ( long, ynum ), /* Ｙの要素数 */
       ( long, znum ), /* Ｚの要素数 */
       ( long, size )) /* 要素サイズ */
{
  int i;
  char ***ptr;

  if (( xnum <= 0 ) || ( ynum <= 0 ) || ( znum <= 0 ) || ( size <= 0 ))
    return 0;

  ptr = ( char *** )memnew1( znum + 1, sizeof( char ** ));

  for ( i = 0; i < znum; i++ )
    {
      ptr[ i ] = memnew2( xnum, ynum, size );
      if ( ptr[i] == 0 )
	{
	  fprintf(stderr,"error:memnew3:can't allocate memory\n");
	  fprintf(stderr,"     { xnum:%d, ynum:%d, znum:%d, size:%d }\n",
		  xnum, ynum, znum, size );
	  fprintf(stderr,"     at zi = %d\n", i );
	}
    }
  ptr[ i ] = 0;

  return ptr;
}



/*==========================================*
 * memfree3 : 三次元配列のメモリを開放する。
 *==========================================*/
void memfree3
  _P1 (( char ***, ptr )) /* 三次元配列のポインタ */
{
  int i;

  if ( ptr == 0 ) return;
  for ( i = 0; ptr[ i ]; i++ ) memfree2( ptr[ i ] );
  memfree1(( char * )ptr );
}



/*============================================*
 * memcopy3 : 三次元配列のメモリをコピーする。
 *============================================*/
void memcopy3
  _P6 (( char ***, dest ), /* 三次元配列のポインタ */
       ( char ***, src  ), /* 三次元配列のポインタ */
       ( long    , xnum ), /* Ｘの要素数           */
       ( long    , ynum ), /* Ｙの要素数           */
       ( long    , znum ), /* Ｚの要素数           */
       ( long    , size )) /* 要素サイズ           */
{
  int i;

  if (( dest == 0 ) || ( src == 0 )) return;
  if (( xnum <= 0 ) || ( ynum <= 0 ) || ( znum <= 0 ) || ( size <= 0 )) return;

  for ( i = 0; i < znum; i++ )
    memcopy2( dest[ i ], src[ i ], xnum, ynum, size );
}



/*========================================*
 * memfill3 : 三次元配列のメモリを埋める。
 *========================================*/
void memfill3
  _P6 (( char ***, dest ), /* 二次元配列のポインタ */
       ( long    , c    ), /* 埋める文字           */
       ( long    , xnum ), /* Ｘの要素数           */
       ( long    , ynum ), /* Ｙの要素数           */
       ( long    , znum ), /* Ｚの要素数           */
       ( long    , size )) /* 要素サイズ           */
{
  int i;

  if ( dest == 0 ) return;
  if (( xnum <= 0 ) || ( ynum <= 0 ) || ( znum <= 0 ) || ( size <= 0 )) return;

  for ( i = 0; i < znum; i++ )
    memfill2( dest[ i ], c, xnum, ynum, size );
}
