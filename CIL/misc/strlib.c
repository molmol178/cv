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
 * strnew : 新しい文字列を確保する
 *=================================*/
char *strnew /* 文字列 */
  _P1 (( char *, str )) /* 文字列 */
{
  if ( str == 0 ) return 0;
  return ( char * )strcpy(( char * )malloc( strlength( str ) + 1 ), str );
}



/*=============================*
 * strfree :  文字列を解放する
 *=============================*/
void strfree
  _P1 (( char *, str )) /* 文字列 */
{
  if ( str ) free( str );
}



/*==========================*
 * strlength : 文字列の長さ
 *==========================*/
long strlength /* 文字数 */
  _P1 (( char *, str )) /* 文字列 */
{
  return ( str ) ? strlen( str ) : 0;
}



/*=========================================*
 * strprintlength : 表示可能の文字列の長さ
 *=========================================*/
long strprintlength /* 文字数 */
  _P1 (( char *, str )) /* 文字列 */
{
  int i, n, esc;

  esc = 0;
  n = strlength( str );
  for ( i = 0; i < n; i++ )
    if ( str[ i ] == '' ) esc++;

  return n - esc * 4;
}



/*===============================*
 * strcompare : 文字列を比較する
 *===============================*/
int strcompare /* 文字列の差 */
  _P2 (( char *, str1 ), /* 文字列１ */
       ( char *, str2 )) /* 文字列２ */
{
  if ( str1 == 0 || str2 == 0 ) return ( int )( str1 - str2 );

  return strcmp( str1, str2 );
}



/*======================================*
 * strsubcompare : 文字列を部分比較する
 *======================================*/
int strsubcompare /* 文字列の差 */
  _P3 (( char *, str1 ), /* 文字列１ */
       ( char *, str2 ), /* 文字列２ */
       ( int   , n    )) /* 文字数   */
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
 * strxcompare : 文字列を大文字小文字の区別なしに比較する。
 *==========================================================*/
int strxcompare /* 文字列の差  */
  _P2 (( char *, str1 ), /* 文字列１ */
       ( char *, str2 )) /* 文字列２ */
{
  if ( str1 == 0 || str2 == 0 ) return ( int )( str1 - str2 );

  return strcasecmp( str1, str2 );
}



/*===============================================================*
 * strxsubcompare : 文字列を大文字小文字の区別なしに部分比較する
 *===============================================================*/
int strxsubcompare /* 文字列の差 */
  _P3 (( char *, str1 ), /* 文字列１ */
       ( char *, str2 ), /* 文字列２ */
       ( int   , n    )) /* 文字数   */
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
 * strcopy : 文字列をコピーします。
 *=================================*/
char *strcopy /* コピー先文字列ポインタ */
  _P2 (( char *, dest ), /* コピー先文字列ポインタ */
       ( char *, src  )) /* コピー元文字列ポインタ */
{
  if ( dest == 0 || src == 0 ) return dest;

  return strcpy( dest, src );
}



/*========================================*
 * strsubcopy : 文字列を部分コピーします。
 *=======================================*/
char *strsubcopy /* コピー先文字列ポインタ */
  _P3 (( char *, dest ), /* コピー先文字列ポインタ */
       ( char *, src  ), /* コピー元文字列ポインタ */
       ( int   , n    )) /* コピー文字数           */
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
 * strappen     : 文字列を文字列にアペンドします。
 *=================================================*/
char *strappend /* アペンド先文字列ポインタ */
  _P2 (( char *, dest ), /* アペンド先文字列ポインタ */
       ( char *, src  )) /* アペンド元文字列ポインタ */
{
  if ( dest == 0 || src == 0 ) return dest;

  return strcat( dest, src );
}



/*=======================================================*
 * strsubappend : 文字列の部分を文字列にアペンドします。
 *=======================================================*/
char *strsubappend /* アペンド先文字列ポインタ */
  _P3 (( char *, dest ), /* アペンド先文字列ポインタ */
       ( char *, src  ), /* アペンド元文字列ポインタ */
       ( int   , n    )) /* アペンド文字数           */
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
 * __strin : 文字リストに文字が入っているか調べる。
 *-------------------------------------------------*/
static long __strin /* 真理値 */
  _P2 (( int   , c     ), /* チェック文字 */
       ( char *, clist )) /* 文字リスト   */
{
  register int i;

  for ( i = 0; clist[ i ]; i++ ) if ( c == clist[ i ] ) return 1;

  return 0;
}



/*==============================================================*
 * strsplit : 文字列 words を 区切り文字 separater で分離して、
 *            各先頭を head に入れる。
 *            返り値として区切れた文字列の数を返す
 *==============================================================*/
long strsplit /* 文字列の数 */
  _P3 (( char  *, words     ), /* 文字列           */
       ( char **, head      ), /* 分離文字列リスト */
       ( char  *, separater )) /* 区切り文字リスト */
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
 * strtolong : 文字列を long 型に変換します。
 *===========================================*/
long strtolong /* 整数値 */
  _P1 (( char *, str )) /* 文字列 */
{
  return ( str ) ? atol( str ) : 0;
}



/*=============================================*
 * strtoulong : 文字列を ulong 型に変換します。
 *=============================================*/
unsigned long strtoulong /* 整数値 */
  _P1 (( char *, str )) /* 文字列 */
{
  return ( str ) ? atol( str ) : 0;
}



/*================================================*
 * strtodouble : 文字列を double 型に変換します。
 *================================================*/
double strtodouble /* 実数値 */
  _P1 (( char *, str )) /* 文字列 */
{
  return ( str ) ? atof( str ) : 0;
}



/*=================================================*
 * strtolboolean : 文字列を boolean 型に変換します。
 *=================================================*/
long strtoboolean /* 真理値 */
  _P1 (( char *, str )) /* 文字列 */
{
  return ( strxcompare( str, "true" ) == 0 );
}



/*====================================*
 * strfromlong : long 型に変換します。
 *====================================*/
char *strfromlong /* 文字列 */
  _P2 (( long  , src  ), /* 整数値 */
       ( char *, dest )) /* 文字列 */
{
  static char tmp[ 64 ];
  if ( dest == 0 ) dest = tmp;
  sprintf( dest, "%d", src );
  return dest;
}



/*=======================================*
 * strfromulong : ulong 型に変換します。
 *=======================================*/
char *strfromulong /* 文字列 */
  _P2 (( unsigned long, src  ), /* 整数値 */
       ( char *       , dest )) /* 文字列 */
{
  static char tmp[ 64 ];
  if ( dest == 0 ) dest = tmp;
  sprintf( dest, "%u", src );
  return dest;
}



/*========================================*
 * strfromdouble : double 型に変換します。
 *========================================*/
char *strfromdouble /* 文字列 */
  _P2 (( double, src  ), /* 実数値 */
       ( char *, dest )) /* 文字列 */
{
  static char tmp[ 64 ];
  if ( dest == 0 ) dest = tmp;
  sprintf( dest, "%.5f", src );
  return dest;
}



/*==============================================================*
 * strfromboolean : long 型を文字列("true","false")に変換します。
 *==============================================================*/
char *strfromboolean /* 文字列 */
  _P2 (( long  , src  ), /* 真理値 */
       ( char *, dest )) /* 文字列 */
{
  static char tmp[ 8 ];

  if ( dest == 0 ) dest = tmp;
  if ( src ) sprintf( dest, "true" ); else sprintf( dest, "false" );
    
  return dest;
}



/*============================================================*
 * strjistoeuc : JIS 漢字コードを EUC 漢字コードに変換します。
 *============================================================*/
void strjistoeuc       
  _P2 (( char *, dst ), /* EUC 漢字コード文字列 */
       ( char *, str )) /* JIS 漢字コード文字列 */
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
 * strisdigit : 文字列が数字だけかどうか調べます。
 *================================================*/
long strisdigit /* 真理値 */
  _P1 (( char *, str )) /* 文字列 */
{
  int i;

  if ( str == 0 ) return 0;

  for ( i = 0; str[ i ]; i++ )
    if ( ! isdigit( str[ i ] ) ) return 0;

  return 1;
}



/*================================================*
 * strisreal : 文字列が実数かどうか調べます。
 *================================================*/
long strisreal /* 真理値 */
  _P1 (( char *, str )) /* 文字列 */
{
  int i;

  if (str == 0) return 0;

  for (i = 0; str[i]; i++)
    if ((! isdigit(str[i])) && (str[i] != '.')) return 0;

  return 1;
}



/*=======================================================*
 * strismoney : 文字列が数字と”，”だけかどうか調べます。
 *=======================================================*/
long strismoney /* 真理値 */
  _P1 (( char *, str )) /* 文字列 */
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
 * strtomoney : 数字と”，”の文字列を整数に変換します。
 *======================================================*/
long strtomoney /* 整数値 */
  _P1 (( char *, str )) /* 文字列 */
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
 * strfrommoney : 整数を数字と”，”の文字列に変換します。
 *========================================================*/
char *strfrommoney /* 文字列 */
  _P2 (( long  , src  ), /* 整数値 */
       ( char *, dest )) /* 文字列 */
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
 * strprintf : 文字列にプリントする
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
