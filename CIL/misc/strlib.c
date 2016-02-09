/*
 * filename : strlib.c
 * author   : Takahiro Sugiyama
 * date     : 1993/09/13
 */



#include <stdio.h>
#include "strlib.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
//#include <malloc.h>



/*=================================*
 * strnew : ������ʸ�������ݤ���
 *=================================*/
char *strnew /* ʸ���� */
  _P1 (( char *, str )) /* ʸ���� */
{
  if ( str == 0 ) return 0;
  return ( char * )strcpy(( char * )malloc( strlength( str ) + 1 ), str );
}



/*=============================*
 * strfree :  ʸ������������
 *=============================*/
void strfree
  _P1 (( char *, str )) /* ʸ���� */
{
  if ( str ) free( str );
}



/*==========================*
 * strlength : ʸ�����Ĺ��
 *==========================*/
long strlength /* ʸ���� */
  _P1 (( char *, str )) /* ʸ���� */
{
  return ( str ) ? strlen( str ) : 0;
}



/*=========================================*
 * strprintlength : ɽ����ǽ��ʸ�����Ĺ��
 *=========================================*/
long strprintlength /* ʸ���� */
  _P1 (( char *, str )) /* ʸ���� */
{
  int i, n, esc;

  esc = 0;
  n = strlength( str );
  for ( i = 0; i < n; i++ )
    if ( str[ i ] == '' ) esc++;

  return n - esc * 4;
}



/*===============================*
 * strcompare : ʸ�������Ӥ���
 *===============================*/
int strcompare /* ʸ����κ� */
  _P2 (( char *, str1 ), /* ʸ���� */
       ( char *, str2 )) /* ʸ���� */
{
  if ( str1 == 0 || str2 == 0 ) return ( int )( str1 - str2 );

  return strcmp( str1, str2 );
}



/*======================================*
 * strsubcompare : ʸ�������ʬ��Ӥ���
 *======================================*/
int strsubcompare /* ʸ����κ� */
  _P3 (( char *, str1 ), /* ʸ���� */
       ( char *, str2 ), /* ʸ���� */
       ( int   , n    )) /* ʸ����   */
{
  if ( n <= 0 )
    {
      fprintf( stderr, "warning:strsubcompare:arg #3 n (%d) is wrong.\n", n );
      return 0;
    }

  if ( str1 == 0 || str2 == 0 ) return ( int )( str1 - str2 );

  return strncmp( str1, str2, n );
}



/*==========================================================*
 * strxcompare : ʸ�������ʸ����ʸ���ζ��̤ʤ�����Ӥ��롣
 *==========================================================*/
int strxcompare /* ʸ����κ�  */
  _P2 (( char *, str1 ), /* ʸ���� */
       ( char *, str2 )) /* ʸ���� */
{
  if ( str1 == 0 || str2 == 0 ) return ( int )( str1 - str2 );

  return strcasecmp( str1, str2 );
}



/*===============================================================*
 * strxsubcompare : ʸ�������ʸ����ʸ���ζ��̤ʤ�����ʬ��Ӥ���
 *===============================================================*/
int strxsubcompare /* ʸ����κ� */
  _P3 (( char *, str1 ), /* ʸ���� */
       ( char *, str2 ), /* ʸ���� */
       ( int   , n    )) /* ʸ����   */
{
  if ( n <= 0 )
    {
      fprintf( stderr, "warning:strxsubcompare:arg #3 n (%d) is wrong.\n", n );
      return 0;
    }

  if ( str1 == 0 || str2 == 0 ) return ( int )( str1 - str2 );

  return strncasecmp( str1, str2, n );
}



/*=================================*
 * strcopy : ʸ����򥳥ԡ����ޤ���
 *=================================*/
char *strcopy /* ���ԡ���ʸ����ݥ��� */
  _P2 (( char *, dest ), /* ���ԡ���ʸ����ݥ��� */
       ( char *, src  )) /* ���ԡ���ʸ����ݥ��� */
{
  if ( dest == 0 || src == 0 ) return dest;

  return strcpy( dest, src );
}



/*========================================*
 * strsubcopy : ʸ�������ʬ���ԡ����ޤ���
 *=======================================*/
char *strsubcopy /* ���ԡ���ʸ����ݥ��� */
  _P3 (( char *, dest ), /* ���ԡ���ʸ����ݥ��� */
       ( char *, src  ), /* ���ԡ���ʸ����ݥ��� */
       ( int   , n    )) /* ���ԡ�ʸ����           */
{
  if ( n <= 0 )
    {
      fprintf( stderr, "warning:strsubcopy:arg #3 n (%d) is wrong.\n", n );
      return 0;
    }

  if ( dest == 0 || src == 0 ) return dest;

  return strncpy( dest, src, n );
}



/*=================================================*
 * strappen     : ʸ�����ʸ����˥��ڥ�ɤ��ޤ���
 *=================================================*/
char *strappend /* ���ڥ����ʸ����ݥ��� */
  _P2 (( char *, dest ), /* ���ڥ����ʸ����ݥ��� */
       ( char *, src  )) /* ���ڥ�ɸ�ʸ����ݥ��� */
{
  if ( dest == 0 || src == 0 ) return dest;

  return strcat( dest, src );
}



/*=======================================================*
 * strsubappend : ʸ�������ʬ��ʸ����˥��ڥ�ɤ��ޤ���
 *=======================================================*/
char *strsubappend /* ���ڥ����ʸ����ݥ��� */
  _P3 (( char *, dest ), /* ���ڥ����ʸ����ݥ��� */
       ( char *, src  ), /* ���ڥ�ɸ�ʸ����ݥ��� */
       ( int   , n    )) /* ���ڥ��ʸ����           */
{
  if ( n <= 0 )
    {
      fprintf( stderr, "warning:strsubcopy:arg #3 n (%d) is wrong.\n", n );
      return 0;
    }

  if ( dest == 0 || src == 0 ) return dest;

  return strncat( dest, src, n );
}



/*-------------------------------------------------*
 * __strin : ʸ���ꥹ�Ȥ�ʸ�������äƤ��뤫Ĵ�٤롣
 *-------------------------------------------------*/
static long __strin /* ������ */
  _P2 (( int   , c     ), /* �����å�ʸ�� */
       ( char *, clist )) /* ʸ���ꥹ��   */
{
  register int i;

  for ( i = 0; clist[ i ]; i++ ) if ( c == clist[ i ] ) return 1;

  return 0;
}



/*==============================================================*
 * strsplit : ʸ���� words �� ���ڤ�ʸ�� separater ��ʬΥ���ơ�
 *            ����Ƭ�� head ������롣
 *            �֤��ͤȤ��ƶ��ڤ줿ʸ����ο����֤�
 *==============================================================*/
long strsplit /* ʸ����ο� */
  _P3 (( char  *, words     ), /* ʸ����           */
       ( char **, head      ), /* ʬΥʸ����ꥹ�� */
       ( char  *, separater )) /* ���ڤ�ʸ���ꥹ�� */
{
  char *back_letters;
  int n;
  int nwords = strlength( words );
  int i;

  if ( nwords == 0 ) { head[ 0 ] = "" ; return 0; }

  back_letters = strnew(words);

  for ( i = 0; i < nwords; i++ )
    {
      if ( __strin( words[ i ], separater ) )
	{
	  words[ i ] = 0;
	}
      else
	{
	  if ( words[ i ] == '"' )
	    {
	      words[ i++ ] = '\0';
	      while (( i < nwords ) && ( words[ i ] != '"' )) i++;
	      if ( i < nwords ) words[ i ] = '\0';
	    }
	  else
	  if ( words[ i ] == '\'' )
	    {
	      int bi = i;
	      char bc = words[ i ];

	      words[ i++ ] = '\0';
	      while (( i < nwords ) && ( words[ i ] != '\'' )) i++;
	      if ( i < nwords )
		words[ i ] = '\0';
	      else
		{
		  words[ bi ] = bc;
		  i = bi;
		}
	    }
	}
    }

  n = 0;
  i = 0;
  while ( i < nwords )
    {
      while (( words[ i ] == '\0' ) && ( i < nwords ))
	{
	  if (back_letters[i] == '\"' && back_letters[i+1] == '\"')
	    {
	      head[ n++ ] = words + i;
	    }
	  i++;
	}
      if ( i < nwords )
	{
	  head[ n++ ] = words + i;
	  while (( words[ i ] != '\0' ) && ( i < nwords ))
	    i++;
	}
    }
  strfree(back_letters);

  return n;
}



/*===========================================*
 * strtolong : ʸ����� long �����Ѵ����ޤ���
 *===========================================*/
long strtolong /* ������ */
  _P1 (( char *, str )) /* ʸ���� */
{
  return ( str ) ? atol( str ) : 0;
}



/*=============================================*
 * strtoulong : ʸ����� ulong �����Ѵ����ޤ���
 *=============================================*/
unsigned long strtoulong /* ������ */
  _P1 (( char *, str )) /* ʸ���� */
{
  return ( str ) ? atol( str ) : 0;
}



/*================================================*
 * strtodouble : ʸ����� double �����Ѵ����ޤ���
 *================================================*/
double strtodouble /* �¿��� */
  _P1 (( char *, str )) /* ʸ���� */
{
  return ( str ) ? atof( str ) : 0;
}



/*=================================================*
 * strtolboolean : ʸ����� boolean �����Ѵ����ޤ���
 *=================================================*/
long strtoboolean /* ������ */
  _P1 (( char *, str )) /* ʸ���� */
{
  return ( strxcompare( str, "true" ) == 0 );
}



/*====================================*
 * strfromlong : long �����Ѵ����ޤ���
 *====================================*/
char *strfromlong /* ʸ���� */
  _P2 (( long  , src  ), /* ������ */
       ( char *, dest )) /* ʸ���� */
{
  static char tmp[ 64 ];
  if ( dest == 0 ) dest = tmp;
  sprintf( dest, "%d", src );
  return dest;
}



/*=======================================*
 * strfromulong : ulong �����Ѵ����ޤ���
 *=======================================*/
char *strfromulong /* ʸ���� */
  _P2 (( unsigned long, src  ), /* ������ */
       ( char *       , dest )) /* ʸ���� */
{
  static char tmp[ 64 ];
  if ( dest == 0 ) dest = tmp;
  sprintf( dest, "%u", src );
  return dest;
}



/*========================================*
 * strfromdouble : double �����Ѵ����ޤ���
 *========================================*/
char *strfromdouble /* ʸ���� */
  _P2 (( double, src  ), /* �¿��� */
       ( char *, dest )) /* ʸ���� */
{
  static char tmp[ 64 ];
  if ( dest == 0 ) dest = tmp;
  sprintf( dest, "%.5f", src );
  return dest;
}



/*==============================================================*
 * strfromboolean : long ����ʸ����("true","false")���Ѵ����ޤ���
 *==============================================================*/
char *strfromboolean /* ʸ���� */
  _P2 (( long  , src  ), /* ������ */
       ( char *, dest )) /* ʸ���� */
{
  static char tmp[ 8 ];

  if ( dest == 0 ) dest = tmp;
  if ( src ) sprintf( dest, "true" ); else sprintf( dest, "false" );
    
  return dest;
}



/*============================================================*
 * strjistoeuc : JIS ���������ɤ� EUC ���������ɤ��Ѵ����ޤ���
 *============================================================*/
void strjistoeuc       
  _P2 (( char *, dst ), /* EUC ����������ʸ���� */
       ( char *, str )) /* JIS ����������ʸ���� */
{
  int i,j,n = 0;

  n = strlength( str );

  for ( j = i = 0; i < n; i++ )
    {
      if ( str[i] == 0x1b &&  str[i+1] == 0x24 && str[i+2] == 0x40 )
	{
	  i += 3;
	  while ( str[ i ] != 0x1b && str[i] != '\0')
	    dst[ j++ ] = 0x80 | str[ i++ ];
	  i += 3;
	}
      else
	{
	  while ( str[ i ] != 0x1b && str[ i ] != '\0' )
	    dst[ j++ ] = str[ i++ ];
	}
      i--;
    }
  dst[ j ] = '\0';
}



/*================================================*
 * strisdigit : ʸ���󤬿����������ɤ���Ĵ�٤ޤ���
 *================================================*/
long strisdigit /* ������ */
  _P1 (( char *, str )) /* ʸ���� */
{
  int i;

  if ( str == 0 ) return 0;

  for ( i = 0; str[ i ]; i++ )
    if ( ! isdigit( str[ i ] ) ) return 0;

  return 1;
}



/*================================================*
 * strisreal : ʸ���󤬼¿����ɤ���Ĵ�٤ޤ���
 *================================================*/
long strisreal /* ������ */
  _P1 (( char *, str )) /* ʸ���� */
{
  int i;

  if (str == 0) return 0;

  for (i = 0; str[i]; i++)
    if ((! isdigit(str[i])) && (str[i] != '.')) return 0;

  return 1;
}



/*=======================================================*
 * strismoney : ʸ���󤬿����ȡɡ��ɤ������ɤ���Ĵ�٤ޤ���
 *=======================================================*/
long strismoney /* ������ */
  _P1 (( char *, str )) /* ʸ���� */
{
  long i, n;

  n = strlen( str );
  for ( i = 0; i < n; i++ )
    if (( ! isdigit( str[ i ] ) ) &&
	(str[ i ] != ' ') && (str[ i ] != '\t') && (str[ i ] != ',') )
      return 0;

  return 1;
}



/*======================================================*
 * strtomoney : �����ȡɡ��ɤ�ʸ������������Ѵ����ޤ���
 *======================================================*/
long strtomoney /* ������ */
  _P1 (( char *, str )) /* ʸ���� */
{
  long i, n, money;
  long start = 0;

  money = 0;
  n = strlen( str );
  for ( i = 0; i < n; i++ )
    {
      switch( str[ i ] )
	{
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	  money = money * 10 + str[ i ] - '0';
	  start = 1;
	  break;
	case ',':
	  if ( ! start ) return money;

	case ' ': case '\t':
	  break;

	default:
	  return money;
	}
    }
  return money;
}



/*========================================================*
 * strfrommoney : ����������ȡɡ��ɤ�ʸ������Ѵ����ޤ���
 *========================================================*/
char *strfrommoney /* ʸ���� */
  _P2 (( long  , src  ), /* ������ */
       ( char *, dest )) /* ʸ���� */
{
  static char tmp[ 64 ];
  char sub[ 64 ];
  long n, f, i, j;

  if ( dest == 0 ) dest = tmp;

  sprintf( sub, "%d", src );
  n = strlen( sub );

  if ( sub[ 0 ] == '-' ) f = n - 1; else f = n;

  for ( j = i = 0; i < n; i++ )
    {
      dest[ j++ ] = sub[ i ];
      if (((( f - i - 1 ) % 3 ) == 0 ) && ( i != ( n - 1)) && sub[ i ] != '-' )
	{
	  dest[ j++ ] = ',';
	}
    }
  dest[ j ] = 0;

  return dest;
}



/*==================================*
 * strprintf : ʸ����˥ץ��Ȥ���
 *==================================*/
#ifdef __strprintf__
#include <varargs.h>
long strprintf( buf, format, va_alist )
     char *buf;
     char *format;
     int va_alist;
{
  char string[ 1024 ];
  va_list args;

  va_start( args );
  vsprintf( string, format, args );
  va_end( args );

  strcopy( buf, string );

  return strlength( buf );
}
#endif
