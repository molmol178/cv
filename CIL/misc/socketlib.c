/*
 * filename : socketlib.c
 * author   : Takahiro Sugiyama
 * date     : 1993/10/03
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "socketlib.h"
#include <sys/time.h>



/****************************************
 * サーバとクライアントが用いるルーチン *
 ****************************************/

/*=============================================*
 * socketread : ソケットからデータを読み込む。
 *=============================================*/
long socketread /* 読み込みバイト数 */
  _P3 (( long  , fd    ), /* ソケットＦＤ   */
       ( char *, buf   ), /* データポインタ */
       ( long  , nbyte )) /* データバイト数 */
{
  long readbyte = 0;

  while ( readbyte != nbyte )
    {
      long localbyte = read( fd, buf, nbyte );
      if ( localbyte <= 0 )
	{
	  fprintf( stderr, "waning:socektread:can't read all data\n" );
	  return readbyte;
	}
      readbyte += localbyte;
    }

  return readbyte;
}



/*============================================*
 * socketwrite : ソケットにデータを書き込む。
 *============================================*/
long socketwrite /* 書き込みバイト数 */
  _P3 (( long  , fd    ), /* ソケットＦＤ   */
       ( char *, buf   ), /* データポインタ */
       ( long  , nbyte )) /* データバイト数 */
{
  long writebyte = 0;

  while ( writebyte != nbyte )
    {
      long localbyte = write( fd, buf, nbyte );
      if ( localbyte <= 0 )
	{
	  fprintf( stderr, "waning:socektwrite:can't write all data\n" );
	  return writebyte;
	}
      writebyte += localbyte;
    }

  return writebyte;
}



/**************************
 * サーバが用いるルーチン *
 **************************/

/*=====================================*
 * socketinit : ソケットを初期化する。
 *=====================================*/
long socketinit /* ソケットＦＤ */
  _P1 (( char *, name )) /* ソケットファイル名 */
{
  struct sockaddr_un addr;
  long len;
  long fd;

  fd = socket( AF_UNIX, SOCK_STREAM, 0 );
  if ( fd < 0 )
    {
      perror( "sockinit: socket" );
      return -1;
    }

  addr.sun_family = AF_UNIX;
  strcpy( addr.sun_path, name );
  unlink( name );

  len = sizeof( addr.sun_family ) + strlen( addr.sun_path );
  if ( bind( fd, ( struct sockaddr * )&addr, len ) < 0 )
    {
      perror( "sockinit: bind" );
      return -1;
    }

  if ( listen( fd, 5 ) < 0 )
    {
      perror( "socket: listen" );
      return -1;
    }

  return fd;
}



/*==================================*
 * socketexit : ソケットを終了する。
 *==================================*/
void socketexit
  _P2 (( char *, name ), /* ソケットファイル名 */
       ( long  , fd   )) /* ソケットＦＤ       */
{
  close( fd );
  unlink( name );
}



/*================================================*
 * socektselect : 読み込み可能なソケットを調べる。
 *================================================*/
long socketselect /* 読み込みＯＫのソケットＦＤ数  */
  _P3 (( long    , max_fd  ), /* 最大のソケットＦＤの番号   */
       ( fd_set *, mask ), /* ソケットＦＤのビットマスク */
       ( double  , waitsec )) /* 待ち時間（秒）             */
{
  fd_set writefd,expectfd;
  long fn;
  struct timeval tm,*tp = 0;

  socketfdzero( writefd );
  socketfdzero( expectfd );

  if ( waitsec > 0.0 )
    {
      tm.tv_sec  = (long)waitsec;
      tm.tv_usec = 1000000.0 * (waitsec - tm.tv_sec);
      tp = &tm;
    }

  fn = select( max_fd, mask, &writefd, &expectfd, tp );

  return fn;
}



/*===================================*
 * socketaccept : 接続を受け入れる。
 *===================================*/
long socketaccept /* 相手のソケットＦＤ */
  _P1 (( int, fd )) /* ソケットＦＤ */
{
  struct sockaddr_un from;
  int fromlen;
  int sock;

  fromlen = sizeof( from );
  sock = accept( fd, ( struct sockaddr * )&from, &fromlen );
  if ( sock < 0 )
    {
      perror( "socketaccept: accept" );
      return -1;
    }

  return sock;
}



/********************************
 * クライアントが用いるルーチン *
 ********************************/

/*==================================*
 * socketconnect : 接続を要求する。
 *==================================*/
long socketconnect /* 相手のソケットＦＤ */
  _P1 (( char *, name )) /* ソケットファイル名 */
{
  struct sockaddr_un addr;
  long len;
  long fd;

  fd = socket( AF_UNIX, SOCK_STREAM, 0 );
  if ( fd < 0 )
    {
      /* perror( "socketconnect: socket" ); */
      return -1;
    }

  addr.sun_family = AF_UNIX;
  strcpy( addr.sun_path, name );

  len = sizeof( addr.sun_family ) + strlen( addr.sun_path );
  if ( connect( fd, ( struct sockaddr * )&addr, len ) < 0 )
    {
      /* perror( "socketconnect: connect" ); */
      return -1;
    }

  return fd;
}



/*==================================*
 * socketclose : ソケットを閉じる。
 *==================================*/
void socketclose
  _P1 (( long, fd )) /* ソケットＦＤ */
{
  close( fd );
}
