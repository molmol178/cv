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
 * $@%5!<%P$H%/%i%$%"%s%H$,MQ$$$k%k!<%A%s(J *
 ****************************************/

/*=============================================*
 * socketread : $@%=%1%C%H$+$i%G!<%?$rFI$_9~$`!#(J
 *=============================================*/
long socketread /* $@FI$_9~$_%P%$%H?t(J */
  _P3 (( long  , fd    ), /* $@%=%1%C%H#F#D(J   */
       ( char *, buf   ), /* $@%G!<%?%]%$%s%?(J */
       ( long  , nbyte )) /* $@%G!<%?%P%$%H?t(J */
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
 * socketwrite : $@%=%1%C%H$K%G!<%?$r=q$-9~$`!#(J
 *============================================*/
long socketwrite /* $@=q$-9~$_%P%$%H?t(J */
  _P3 (( long  , fd    ), /* $@%=%1%C%H#F#D(J   */
       ( char *, buf   ), /* $@%G!<%?%]%$%s%?(J */
       ( long  , nbyte )) /* $@%G!<%?%P%$%H?t(J */
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
 * $@%5!<%P$,MQ$$$k%k!<%A%s(J *
 **************************/

/*=====================================*
 * socketinit : $@%=%1%C%H$r=i4|2=$9$k!#(J
 *=====================================*/
long socketinit /* $@%=%1%C%H#F#D(J */
  _P1 (( char *, name )) /* $@%=%1%C%H%U%!%$%kL>(J */
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
 * socketexit : $@%=%1%C%H$r=*N;$9$k!#(J
 *==================================*/
void socketexit
  _P2 (( char *, name ), /* $@%=%1%C%H%U%!%$%kL>(J */
       ( long  , fd   )) /* $@%=%1%C%H#F#D(J       */
{
  close( fd );
  unlink( name );
}



/*================================================*
 * socektselect : $@FI$_9~$_2DG=$J%=%1%C%H$rD4$Y$k!#(J
 *================================================*/
long socketselect /* $@FI$_9~$_#O#K$N%=%1%C%H#F#D?t(J  */
  _P3 (( long    , max_fd  ), /* $@:GBg$N%=%1%C%H#F#D$NHV9f(J   */
       ( fd_set *, mask ), /* $@%=%1%C%H#F#D$N%S%C%H%^%9%/(J */
       ( double  , waitsec )) /* $@BT$A;~4V!JIC!K(J             */
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
 * socketaccept : $@@\B3$r<u$1F~$l$k!#(J
 *===================================*/
long socketaccept /* $@Aj<j$N%=%1%C%H#F#D(J */
  _P1 (( int, fd )) /* $@%=%1%C%H#F#D(J */
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
 * $@%/%i%$%"%s%H$,MQ$$$k%k!<%A%s(J *
 ********************************/

/*==================================*
 * socketconnect : $@@\B3$rMW5a$9$k!#(J
 *==================================*/
long socketconnect /* $@Aj<j$N%=%1%C%H#F#D(J */
  _P1 (( char *, name )) /* $@%=%1%C%H%U%!%$%kL>(J */
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
 * socketclose : $@%=%1%C%H$rJD$8$k!#(J
 *==================================*/
void socketclose
  _P1 (( long, fd )) /* $@%=%1%C%H#F#D(J */
{
  close( fd );
}
