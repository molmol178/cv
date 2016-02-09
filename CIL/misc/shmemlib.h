/*
 * filename : shmemlib.h
 * author   : Takahiro Sugiyama
 * date     : 1993/09/13
 */



#ifndef __shmemlib_h
#define __shmemlib_h



#include "comacros.h"



/*==================================================================*
 * shmemnew  : $B%-!<$G;XDj$5$l$?%5%$%:$NNN0h$r6&M-%a%b%j$G3NJ]$9$k!#(B
 * shmemfree : $B%-!<$r;}$D6&M-%a%b%j$r3+J|$9$k!#(B
 *==================================================================*/
char *shmemnew /* $B%a%b%j%]%$%s%?(B */
  P2 (( long, key  ), /* $B%-!<(B   */
      ( long, size )) /* $B%5%$%:(B */

void shmemfree
  P1 (( long, key ))  /* $B%-!<(B */



/*=============================================================*
 * shmemget : $B%-!<$G;XDj$5$l$?6&M-%a%b%j$N%]%$%s%?$r3MF@$9$k!#(B
 *=============================================================*/
char *shmemget
  P1 (( long, key ))  /* $B%-!<(B */



/*=================================*
 * shmeminfoRec : $B6&M-%a%b%j$N>pJs(B
 *=================================*/
typedef struct {
  long id;		/* $B6&M-%a%b%j$G;HMQ$9$k#I#D(B */
  long key;		/* $B%-!<$N@0?tCM(B 	    */
  char mode[12];	/* $B%b!<%I(B		    */
  char owner[12];	/* $B;}$A<g(B		    */
  char group[12];	/* $B%0%k!<%WL>(B		    */
  long attach;		/* $B;2>H$5$l$F$k?t(B	    */
  long size;		/* $B6&M-%a%b%j$N%5%$%:(B	    */
  char atime[9];	/* $B%"%?%C%A;~4V(B		    */
  char dtime[9];	/* $B%G%?%C%A;~4V(B		    */
  char ctime[9];	/* $B:n@.;~4V(B		    */
} shmeminfoRec;



/*============================================================*
 * shmemgetinfo : $B%-!<$G;XDj$5$l$?6&M-%a%b%j$N>pJs$r3MF@$9$k!#(B
 *============================================================*/
long shmemgetinfo /* $B>pJs$,3MF@$G$-$?$+$I$&$+$N??M}CM(B */
  P2 (( long          , key ),  /* $B%-!<(B       */
      ( shmeminfoRec *, info )) /* $B>pJs$,F~$k(B */



/*==============================================*
 * shmemgetlist : $B6&M-%a%b%j$N>pJs$r<h$C$F$/$k!#(B
 *==============================================*/
long shmemgetlist
  P1 (( shmeminfoRec *, list ))



#endif  //__shmemlib_h
