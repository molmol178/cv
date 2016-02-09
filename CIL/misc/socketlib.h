/*
 * filename : socketlib.h	
 * author   : Takahiro Sugiyama
 * date     : 1993/10/03
 */



#ifndef __socketlib_h
#define __socketlib_h



#include "comacros.h"
#include <sys/types.h>



/****************************************
 * $B%5!<%P$H%/%i%$%"%s%H$,MQ$$$k%k!<%A%s(J *
 ****************************************/

/*=============================================*
 * socketread : $B%=%1%C%H$+$i%G!<%?$rFI$_9~$`!#(J
 *=============================================*/
long socketread /* $BFI$_9~$_%P%$%H?t(J */
  P3 (( long  , fd    ), /* $B%=%1%C%H#F#D(J   */
      ( char *, buf   ), /* $B%G!<%?%]%$%s%?(J */
      ( long  , nbyte )) /* $B%G!<%?%P%$%H?t(J */

/*============================================*
 * socketwrite : $B%=%1%C%H$K%G!<%?$r=q$-9~$`!#(J
 *============================================*/
long socketwrite /* $B=q$-9~$_%P%$%H?t(J */
  P3 (( long  , fd    ), /* $B%=%1%C%H#F#D(J   */
      ( char *, buf   ), /* $B%G!<%?%]%$%s%?(J */
      ( long  , nbyte )) /* $B%G!<%?%P%$%H?t(J */

/*==================================*
 * socketclose : $B%=%1%C%H$rJD$8$k!#(J
 *==================================*/
void socketclose
  P1 (( long, fd )) /* $B%=%1%C%H#F#D(J */



/**************************
 * $B%5!<%P$,MQ$$$k%k!<%A%s(J *
 **************************/

/*=====================================*
 * socketinit : $B%=%1%C%H$r=i4|2=$9$k!#(J
 *=====================================*/
long socketinit /* $B%=%1%C%H#F#D(J */
  P1 (( char *, name )) /* $B%=%1%C%H%U%!%$%kL>(J */

/*==================================*
 * socketexit : $B%=%1%C%H$r=*N;$9$k!#(J
 *==================================*/
void socketexit
  P2 (( char *, name ), /* $B%=%1%C%H%U%!%$%kL>(J */
      ( long  , fd   )) /* $B%=%1%C%H#F#D(J       */

/*================================================*
 * socektselect : $BFI$_9~$_2DG=$J%=%1%C%H$rD4$Y$k!#(J
 *================================================*/
long socketselect /* $BFI$_9~$_#O#K$N%=%1%C%H#F#D?t(J  */
  P3 (( long    , max_fd  ), /* $B:GBg$N%=%1%C%H#F#D$NHV9f(J   */
      ( fd_set *, mask ), /* $B%=%1%C%H#F#D$N%S%C%H%^%9%/(J */
      ( double  , waitsec )) /* $BBT$A;~4V!JIC!K(J0$B$OL58B$KBT$D(J */

#define socketready( mask, fd )	FD_ISSET( fd, &mask )
#define socketfdisset( mask, fd )	FD_ISSET( fd, &mask )
#define socketfdclear( mask, fd )	FD_CLR( fd, &mask )
#define socketfdset( mask, fd )	FD_SET( fd, &mask )
#define socketfdzero( mask )		FD_ZERO( &mask )

/*===================================*
 * socketaccept : $B@\B3$r<u$1F~$l$k!#(J
 *===================================*/
long socketaccept /* $BAj<j$N%=%1%C%H#F#D(J */
  P1 (( int, fd )) /* $B%=%1%C%H#F#D(J */



/********************************
 * $B%/%i%$%"%s%H$,MQ$$$k%k!<%A%s(J *
 ********************************/

/*==================================*
 * socketconnect : $B@\B3$rMW5a$9$k!#(J
 *==================================*/
long socketconnect /* $BAj<j$N%=%1%C%H#F#D(J */
  P1 (( char *, name )) /* $B%=%1%C%H%U%!%$%kL>(J */



#endif /* __socketlib_h */
