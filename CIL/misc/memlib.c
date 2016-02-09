/*
 * filename : memlib.c
 * author   : Takahiro Sugiyama
 * date     : 1993/09/13
 */



#include "memlib.h"
#include <stdio.h>
//#include <malloc.h>



/*=====================================================*
 * MEMLIB_ERROR : ���꤬���ݤǤ��ʤ��ä��Ȥ���λ����
 *=====================================================*/
long MEMLIB_ERROR = 1;



/*=============================*
 * memnew : �������ݤ��롣
 *=============================*/
char *memnew /* �ݥ��� */
  _P1 (( long, size )) /* �Х��ȿ� */
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
 * memfree : ����������롣
 *==============================*/
void memfree
  _P1 (( char *, ptr )) /* �ݥ��� */
{
  if ( ptr ) free( ptr );
}



/*=================================*
 * memrenew : �����Ƴ��ݤ��롣
 *=================================*/
char *memrenew /* �ݥ��� */
  _P2 (( char *, ptr  ), /* �ݥ��� */
       ( long  , size )) /* �Х��ȿ� */
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
 * memcopy : ����򥳥ԡ����롣
 *================================*/
void memcopy
  _P3 (( char *, dest ), /* �ݥ��� */
       ( char *, src  ), /* �ݥ��� */
       ( long  , size )) /* �Х��ȿ� */
{
  if (( dest == 0 ) || ( src == 0 ) || ( size <= 0 )) return;
  memcpy( dest, src, size );
}



/*============================*
 * memfill : ��������롣
 *============================*/
void memfill
  _P3 (( char *, dest ), /* �ݥ���   */
       ( long  , c    ), /* ����ʸ�� */
       ( long  , size )) /* �Х��ȿ�   */
{
  if (( dest == 0 ) || ( size <= 0 )) return;
  memset( dest, c, size );
}



/*========================================*
 * memnew1 : �켡������Υ������ݤ��롣
 *========================================*/
char *memnew1 /* �켡������Υݥ��� */
  _P2 (( long, num  ), /* ���ǿ�     */
       ( long, size )) /* ���ǥ����� */
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
 * memfree1 : �켡������Υ���������롣
 *========================================*/
void memfree1
  _P1 (( char *, ptr )) /* �켡������Υݥ��� */
{
  if ( ptr == 0 ) return;
  free( ptr );
}



/*=============================================*
 * memrenew1 : �켡������Υ����Ƴ��ݤ��롣
 *=============================================*/
char *memrenew1 /* �켡������Υݥ��� */
  _P3 (( char *, ptr ), /* �켡������Υݥ��� */
       ( long  , num ), /* ���ǿ�               */
       ( long  , size)) /* ���ǥ�����           */
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
 * memcopy1 : �켡������Υ���򥳥ԡ����롣
 *============================================*/
void memcopy1
  _P4 (( char *, dest ), /* �켡������Υݥ��� */
       ( char *, src  ), /* �켡������Υݥ��� */
       ( long  , num  ), /* ���ǿ�               */
       ( long  , size )) /* ���ǥ�����           */
{
  if (( dest == 0 ) || ( src == 0 ) || ( num <= 0 ) || ( size <= 0 )) return;
  memcpy( dest, src, num * size );
}



/*==============================================*
 * memfill1 : �켡������Υ�������Ƥ����롣
 *==============================================*/
void memfill1
  _P4 (( char *, dest ), /* �켡������Υݥ��� */
       ( long  , c    ), /* ����ʸ��           */
       ( long  , num  ), /* ���ǿ�               */
       ( long  , size )) /* ���ǥ�����           */
{
  if (( dest == 0 ) || ( num <= 0 ) || ( size <= 0 )) return;
  memset( dest, c, num * size );
}



/*=========================================*
 * memnew2 : �󼡸�����Υ������ݤ��롣
 *=========================================*/
char **memnew2 /* �󼡸�����Υݥ��� */
  _P3 (( long, xnum ), /* �ؤ����ǿ� */
       ( long, ynum ), /* �٤����ǿ� */
       ( long, size )) /* ���ǥ����� */
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
 * memfree2 : �󼡸�����Υ���������롣
 *==========================================*/
void memfree2
  _P1 (( char **, ptr )) /* �󼡸�����Υݥ��� */
{
  if ( ptr == 0 ) return;
  memfree1( ptr[ 0 ] );
  memfree1(( char * )ptr );
}



/*============================================*
 * memcopy2 : �󼡸�����Υ���򥳥ԡ����롣
 *============================================*/
void memcopy2
  _P5 (( char **, dest ), /* �󼡸�����Υݥ��� */
       ( char **, src  ), /* �󼡸�����Υݥ��� */
       ( long   , xnum ), /* �ؤ����ǿ�           */
       ( long   , ynum ), /* �٤����ǿ�           */
       ( long   , size )) /* ���ǥ�����           */
{
  if (( dest == 0 ) || ( src == 0 )) return;
  if (( xnum <= 0 ) || ( ynum <= 0 ) || ( size <= 0 )) return;
  memcopy1( dest[ 0 ], src[ 0 ], xnum * ynum, size );
}



/*========================================*
 * memfill2 : �󼡸�����Υ�������롣
 *========================================*/
void memfill2
  _P5 (( char **, dest ), /* �󼡸�����Υݥ��� */
       ( long   , c    ), /* ����ʸ��           */
       ( long   , xnum ), /* �ؤ����ǿ�           */
       ( long   , ynum ), /* �٤����ǿ�           */
       ( long   , size )) /* ���ǥ�����           */
{
  if (( dest == 0 ) || ( xnum <= 0 ) || ( ynum <= 0 ) || ( size <= 0 )) return;
  memset( dest, c, ynum * xnum * size );
}




/*=========================================*
 * memnew3 : ����������Υ������ݤ��롣
 *=========================================*/
char ***memnew3 /* ����������Υݥ��� */
  _P4 (( long, xnum ), /* �ؤ����ǿ� */
       ( long, ynum ), /* �٤����ǿ� */
       ( long, znum ), /* �ڤ����ǿ� */
       ( long, size )) /* ���ǥ����� */
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
 * memfree3 : ����������Υ���������롣
 *==========================================*/
void memfree3
  _P1 (( char ***, ptr )) /* ����������Υݥ��� */
{
  int i;

  if ( ptr == 0 ) return;
  for ( i = 0; ptr[ i ]; i++ ) memfree2( ptr[ i ] );
  memfree1(( char * )ptr );
}



/*============================================*
 * memcopy3 : ����������Υ���򥳥ԡ����롣
 *============================================*/
void memcopy3
  _P6 (( char ***, dest ), /* ����������Υݥ��� */
       ( char ***, src  ), /* ����������Υݥ��� */
       ( long    , xnum ), /* �ؤ����ǿ�           */
       ( long    , ynum ), /* �٤����ǿ�           */
       ( long    , znum ), /* �ڤ����ǿ�           */
       ( long    , size )) /* ���ǥ�����           */
{
  int i;

  if (( dest == 0 ) || ( src == 0 )) return;
  if (( xnum <= 0 ) || ( ynum <= 0 ) || ( znum <= 0 ) || ( size <= 0 )) return;

  for ( i = 0; i < znum; i++ )
    memcopy2( dest[ i ], src[ i ], xnum, ynum, size );
}



/*========================================*
 * memfill3 : ����������Υ�������롣
 *========================================*/
void memfill3
  _P6 (( char ***, dest ), /* �󼡸�����Υݥ��� */
       ( long    , c    ), /* ����ʸ��           */
       ( long    , xnum ), /* �ؤ����ǿ�           */
       ( long    , ynum ), /* �٤����ǿ�           */
       ( long    , znum ), /* �ڤ����ǿ�           */
       ( long    , size )) /* ���ǥ�����           */
{
  int i;

  if ( dest == 0 ) return;
  if (( xnum <= 0 ) || ( ynum <= 0 ) || ( znum <= 0 ) || ( size <= 0 )) return;

  for ( i = 0; i < znum; i++ )
    memfill2( dest[ i ], c, xnum, ynum, size );
}
