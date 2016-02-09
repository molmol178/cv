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
 * strnew : ¿·¤·¤¤Ê¸»úÎó¤ò³ÎÊİ¤¹¤ë
 *=================================*/
char *strnew /* Ê¸»úÎó */
  _P1 (( char *, str )) /* Ê¸»úÎó */
{
  if ( str == 0 ) return 0;
  return ( char * )strcpy(( char * )malloc( strlength( str ) + 1 ), str );
}



/*=============================*
 * strfree :  Ê¸»úÎó¤ò²òÊü¤¹¤ë
 *=============================*/
void strfree
  _P1 (( char *, str )) /* Ê¸»úÎó */
{
  if ( str ) free( str );
}



/*==========================*
 * strlength : Ê¸»úÎó¤ÎÄ¹¤µ
 *==========================*/
long strlength /* Ê¸»ú¿ô */
  _P1 (( char *, str )) /* Ê¸»úÎó */
{
  return ( str ) ? strlen( str ) : 0;
}



/*=========================================*
 * strprintlength : É½¼¨²ÄÇ½¤ÎÊ¸»úÎó¤ÎÄ¹¤µ
 *=========================================*/
long strprintlength /* Ê¸»ú¿ô */
  _P1 (( char *, str )) /* Ê¸»úÎó */
{
  int i, n, esc;

  esc = 0;
  n = strlength( str );
  for ( i = 0; i < n; i++ )
    if ( str[ i ] == '' ) esc++;

  return n - esc * 4;
}



/*===============================*
 * strcompare : Ê¸»úÎó¤òÈæ³Ó¤¹¤ë
 *===============================*/
int strcompare /* Ê¸»úÎó¤Îº¹ */
  _P2 (( char *, str1 ), /* Ê¸»úÎó£± */
       ( char *, str2 )) /* Ê¸»úÎó£² */
{
  if ( str1 == 0 || str2 == 0 ) return ( int )( str1 - str2 );

  return strcmp( str1, str2 );
}



/*======================================*
 * strsubcompare : Ê¸»úÎó¤òÉôÊ¬Èæ³Ó¤¹¤ë
 *======================================*/
int strsubcompare /* Ê¸»úÎó¤Îº¹ */
  _P3 (( char *, str1 ), /* Ê¸»úÎó£± */
       ( char *, str2 ), /* Ê¸»úÎó£² */
       ( int   , n    )) /* Ê¸»ú¿ô   */
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
 * strxcompare : Ê¸»úÎó¤òÂçÊ¸»ú¾®Ê¸»ú¤Î¶èÊÌ¤Ê¤·¤ËÈæ³Ó¤¹¤ë¡£
 *==========================================================*/
int strxcompare /* Ê¸»úÎó¤Îº¹  */
  _P2 (( char *, str1 ), /* Ê¸»úÎó£± */
       ( char *, str2 )) /* Ê¸»úÎó£² */
{
  if ( str1 == 0 || str2 == 0 ) return ( int )( str1 - str2 );

  return strcasecmp( str1, str2 );
}



/*===============================================================*
 * strxsubcompare : Ê¸»úÎó¤òÂçÊ¸»ú¾®Ê¸»ú¤Î¶èÊÌ¤Ê¤·¤ËÉôÊ¬Èæ³Ó¤¹¤ë
 *===============================================================*/
int strxsubcompare /* Ê¸»úÎó¤Îº¹ */
  _P3 (( char *, str1 ), /* Ê¸»úÎó£± */
       ( char *, str2 ), /* Ê¸»úÎó£² */
       ( int   , n    )) /* Ê¸»ú¿ô   */
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
 * strcopy : Ê¸»úÎó¤ò¥³¥Ô¡¼¤·¤Ş¤¹¡£
 *=================================*/
char *strcopy /* ¥³¥Ô¡¼ÀèÊ¸»úÎó¥İ¥¤¥ó¥¿ */
  _P2 (( char *, dest ), /* ¥³¥Ô¡¼ÀèÊ¸»úÎó¥İ¥¤¥ó¥¿ */
       ( char *, src  )) /* ¥³¥Ô¡¼¸µÊ¸»úÎó¥İ¥¤¥ó¥¿ */
{
  if ( dest == 0 || src == 0 ) return dest;

  return strcpy( dest, src );
}



/*========================================*
 * strsubcopy : Ê¸»úÎó¤òÉôÊ¬¥³¥Ô¡¼¤·¤Ş¤¹¡£
 *=======================================*/
char *strsubcopy /* ¥³¥Ô¡¼ÀèÊ¸»úÎó¥İ¥¤¥ó¥¿ */
  _P3 (( char *, dest ), /* ¥³¥Ô¡¼ÀèÊ¸»úÎó¥İ¥¤¥ó¥¿ */
       ( char *, src  ), /* ¥³¥Ô¡¼¸µÊ¸»úÎó¥İ¥¤¥ó¥¿ */
       ( int   , n    )) /* ¥³¥Ô¡¼Ê¸»ú¿ô           */
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
 * strappen     : Ê¸»úÎó¤òÊ¸»úÎó¤Ë¥¢¥Ú¥ó¥É¤·¤Ş¤¹¡£
 *=================================================*/
char *strappend /* ¥¢¥Ú¥ó¥ÉÀèÊ¸»úÎó¥İ¥¤¥ó¥¿ */
  _P2 (( char *, dest ), /* ¥¢¥Ú¥ó¥ÉÀèÊ¸»úÎó¥İ¥¤¥ó¥¿ */
       ( char *, src  )) /* ¥¢¥Ú¥ó¥É¸µÊ¸»úÎó¥İ¥¤¥ó¥¿ */
{
  if ( dest == 0 || src == 0 ) return dest;

  return strcat( dest, src );
}



/*=======================================================*
 * strsubappend : Ê¸»úÎó¤ÎÉôÊ¬¤òÊ¸»úÎó¤Ë¥¢¥Ú¥ó¥É¤·¤Ş¤¹¡£
 *=======================================================*/
char *strsubappend /* ¥¢¥Ú¥ó¥ÉÀèÊ¸»úÎó¥İ¥¤¥ó¥¿ */
  _P3 (( char *, dest ), /* ¥¢¥Ú¥ó¥ÉÀèÊ¸»úÎó¥İ¥¤¥ó¥¿ */
       ( char *, src  ), /* ¥¢¥Ú¥ó¥É¸µÊ¸»úÎó¥İ¥¤¥ó¥¿ */
       ( int   , n    )) /* ¥¢¥Ú¥ó¥ÉÊ¸»ú¿ô           */
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
 * __strin : Ê¸»ú¥ê¥¹¥È¤ËÊ¸»ú¤¬Æş¤Ã¤Æ¤¤¤ë¤«Ä´¤Ù¤ë¡£
 *-------------------------------------------------*/
static long __strin /* ¿¿ÍıÃÍ */
  _P2 (( int   , c     ), /* ¥Á¥§¥Ã¥¯Ê¸»ú */
       ( char *, clist )) /* Ê¸»ú¥ê¥¹¥È   */
{
  register int i;

  for ( i = 0; clist[ i ]; i++ ) if ( c == clist[ i ] ) return 1;

  return 0;
}



/*==============================================================*
 * strsplit : Ê¸»úÎó words ¤ò ¶èÀÚ¤êÊ¸»ú separater ¤ÇÊ¬Î¥¤·¤Æ¡¢
 *            ³ÆÀèÆ¬¤ò head ¤ËÆş¤ì¤ë¡£
 *            ÊÖ¤êÃÍ¤È¤·¤Æ¶èÀÚ¤ì¤¿Ê¸»úÎó¤Î¿ô¤òÊÖ¤¹
 *==============================================================*/
long strsplit /* Ê¸»úÎó¤Î¿ô */
  _P3 (( char  *, words     ), /* Ê¸»úÎó           */
       ( char **, head      ), /* Ê¬Î¥Ê¸»úÎó¥ê¥¹¥È */
       ( char  *, separater )) /* ¶èÀÚ¤êÊ¸»ú¥ê¥¹¥È */
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
 * strtolong : Ê¸»úÎó¤ò long ·¿¤ËÊÑ´¹¤·¤Ş¤¹¡£
 *===========================================*/
long strtolong /* À°¿ôÃÍ */
  _P1 (( char *, str )) /* Ê¸»úÎó */
{
  return ( str ) ? atol( str ) : 0;
}



/*=============================================*
 * strtoulong : Ê¸»úÎó¤ò ulong ·¿¤ËÊÑ´¹¤·¤Ş¤¹¡£
 *=============================================*/
unsigned long strtoulong /* À°¿ôÃÍ */
  _P1 (( char *, str )) /* Ê¸»úÎó */
{
  return ( str ) ? atol( str ) : 0;
}



/*================================================*
 * strtodouble : Ê¸»úÎó¤ò double ·¿¤ËÊÑ´¹¤·¤Ş¤¹¡£
 *================================================*/
double strtodouble /* ¼Â¿ôÃÍ */
  _P1 (( char *, str )) /* Ê¸»úÎó */
{
  return ( str ) ? atof( str ) : 0;
}



/*=================================================*
 * strtolboolean : Ê¸»úÎó¤ò boolean ·¿¤ËÊÑ´¹¤·¤Ş¤¹¡£
 *=================================================*/
long strtoboolean /* ¿¿ÍıÃÍ */
  _P1 (( char *, str )) /* Ê¸»úÎó */
{
  return ( strxcompare( str, "true" ) == 0 );
}



/*====================================*
 * strfromlong : long ·¿¤ËÊÑ´¹¤·¤Ş¤¹¡£
 *====================================*/
char *strfromlong /* Ê¸»úÎó */
  _P2 (( long  , src  ), /* À°¿ôÃÍ */
       ( char *, dest )) /* Ê¸»úÎó */
{
  static char tmp[ 64 ];
  if ( dest == 0 ) dest = tmp;
  sprintf( dest, "%d", src );
  return dest;
}



/*=======================================*
 * strfromulong : ulong ·¿¤ËÊÑ´¹¤·¤Ş¤¹¡£
 *=======================================*/
char *strfromulong /* Ê¸»úÎó */
  _P2 (( unsigned long, src  ), /* À°¿ôÃÍ */
       ( char *       , dest )) /* Ê¸»úÎó */
{
  static char tmp[ 64 ];
  if ( dest == 0 ) dest = tmp;
  sprintf( dest, "%u", src );
  return dest;
}



/*========================================*
 * strfromdouble : double ·¿¤ËÊÑ´¹¤·¤Ş¤¹¡£
 *========================================*/
char *strfromdouble /* Ê¸»úÎó */
  _P2 (( double, src  ), /* ¼Â¿ôÃÍ */
       ( char *, dest )) /* Ê¸»úÎó */
{
  static char tmp[ 64 ];
  if ( dest == 0 ) dest = tmp;
  sprintf( dest, "%.5f", src );
  return dest;
}



/*==============================================================*
 * strfromboolean : long ·¿¤òÊ¸»úÎó("true","false")¤ËÊÑ´¹¤·¤Ş¤¹¡£
 *==============================================================*/
char *strfromboolean /* Ê¸»úÎó */
  _P2 (( long  , src  ), /* ¿¿ÍıÃÍ */
       ( char *, dest )) /* Ê¸»úÎó */
{
  static char tmp[ 8 ];

  if ( dest == 0 ) dest = tmp;
  if ( src ) sprintf( dest, "true" ); else sprintf( dest, "false" );
    
  return dest;
}



/*============================================================*
 * strjistoeuc : JIS ´Á»ú¥³¡¼¥É¤ò EUC ´Á»ú¥³¡¼¥É¤ËÊÑ´¹¤·¤Ş¤¹¡£
 *============================================================*/
void strjistoeuc       
  _P2 (( char *, dst ), /* EUC ´Á»ú¥³¡¼¥ÉÊ¸»úÎó */
       ( char *, str )) /* JIS ´Á»ú¥³¡¼¥ÉÊ¸»úÎó */
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
 * strisdigit : Ê¸»úÎó¤¬¿ô»ú¤À¤±¤«¤É¤¦¤«Ä´¤Ù¤Ş¤¹¡£
 *================================================*/
long strisdigit /* ¿¿ÍıÃÍ */
  _P1 (( char *, str )) /* Ê¸»úÎó */
{
  int i;

  if ( str == 0 ) return 0;

  for ( i = 0; str[ i ]; i++ )
    if ( ! isdigit( str[ i ] ) ) return 0;

  return 1;
}



/*================================================*
 * strisreal : Ê¸»úÎó¤¬¼Â¿ô¤«¤É¤¦¤«Ä´¤Ù¤Ş¤¹¡£
 *================================================*/
long strisreal /* ¿¿ÍıÃÍ */
  _P1 (( char *, str )) /* Ê¸»úÎó */
{
  int i;

  if (str == 0) return 0;

  for (i = 0; str[i]; i++)
    if ((! isdigit(str[i])) && (str[i] != '.')) return 0;

  return 1;
}



/*=======================================================*
 * strismoney : Ê¸»úÎó¤¬¿ô»ú¤È¡É¡¤¡É¤À¤±¤«¤É¤¦¤«Ä´¤Ù¤Ş¤¹¡£
 *=======================================================*/
long strismoney /* ¿¿ÍıÃÍ */
  _P1 (( char *, str )) /* Ê¸»úÎó */
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
 * strtomoney : ¿ô»ú¤È¡É¡¤¡É¤ÎÊ¸»úÎó¤òÀ°¿ô¤ËÊÑ´¹¤·¤Ş¤¹¡£
 *======================================================*/
long strtomoney /* À°¿ôÃÍ */
  _P1 (( char *, str )) /* Ê¸»úÎó */
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
 * strfrommoney : À°¿ô¤ò¿ô»ú¤È¡É¡¤¡É¤ÎÊ¸»úÎó¤ËÊÑ´¹¤·¤Ş¤¹¡£
 *========================================================*/
char *strfrommoney /* Ê¸»úÎó */
  _P2 (( long  , src  ), /* À°¿ôÃÍ */
       ( char *, dest )) /* Ê¸»úÎó */
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
 * strprintf : Ê¸»úÎó¤Ë¥×¥ê¥ó¥È¤¹¤ë
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
