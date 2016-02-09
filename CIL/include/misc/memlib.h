/*
 * filename : memlib.h
 * author   : Takahiro Sugiyama
 * date     : 1993/09/13
 */



#ifndef __memlib_h
#define __memlib_h



#include "comacros.h"



/*=====================================================*
 * MEMLIB_ERROR : $B%a%b%j$,3NJ]$G$-$J$+$C$?$H$-=*N;$9$k(B
 *=====================================================*/
extern long MEMLIB_ERROR; /* $B%G%U%)%k%H!'#1!J=*N;$9$k!K(B */



/*********************
 * $B%a%b%j$K4X$9$kA`:n(B
 *********************/

/*=============================*
 * memnew : $B%a%b%j$r3NJ]$9$k!#(B
 *=============================*/
char *memnew /* $B%]%$%s%?(B */
  P1 (( long, size )) /* $B%P%$%H?t(B */



/*==============================*
 * memfree : $B%a%b%j$r3+J|$9$k!#(B
 *==============================*/
void memfree
  P1 (( char *, ptr )) /* $B%]%$%s%?(B */



/*=================================*
 * memrenew : $B%a%b%j$r:F3NJ]$9$k!#(B
 *=================================*/
char *memrenew /* $B%]%$%s%?(B */
  P2 (( char *, ptr  ), /* $B%]%$%s%?(B */
      ( long  , size )) /* $B%P%$%H?t(B */



/*================================*
 * memcopy : $B%a%b%j$r%3%T!<$9$k!#(B
 *================================*/
void memcopy
  P3 (( char *, dest ), /* $B%]%$%s%?(B */
      ( char *, src  ), /* $B%]%$%s%?(B */
      ( long  , size )) /* $B%P%$%H?t(B */



/*============================*
 * memfill : $B%a%b%j$rKd$a$k!#(B
 *============================*/
void memfill
  P3 (( char *, dest ), /* $B%]%$%s%?(B   */
      ( long  , c    ), /* $BKd$a$kJ8;z(B */
      ( long  , size )) /* $B%P%$%H?t(B   */



/*************************
 * $B0l<!85G[Ns$K4X$9$kA`:n(B
 *************************/

/*=========================================*
 * memnew1 : $B0l<!85G[Ns$N%a%b%j$r3NJ]$9$k!#(B
 *=========================================*/
char *memnew1 /* $B0l<!85G[Ns$N%]%$%s%?(B */
  P2 (( long, num  ), /* $BMWAG?t(B     */
      ( long, size )) /* $BMWAG%5%$%:(B */



/*==========================================*
 * memfree1 : $B0l<!85G[Ns$N%a%b%j$r3+J|$9$k!#(B
 *==========================================*/
void memfree1
  P1 (( char *, ptr )) /* $B0l<!85G[Ns$N%]%$%s%?(B */



/*=============================================*
 * memrenew1 : $B0l<!85G[Ns$N%a%b%j$r:F3NJ]$9$k!#(B
 *=============================================*/
char *memrenew1 /* $B0l<!85G[Ns$N%]%$%s%?(B */
  P3 (( char *, ptr ), /* $B0l<!85G[Ns$N%]%$%s%?(B */
      ( long  , num ), /* $BMWAG?t(B               */
      ( long  , size)) /* $BMWAG%5%$%:(B           */



/*============================================*
 * memcopy1 : $B0l<!85G[Ns$N%a%b%j$r%3%T!<$9$k!#(B
 *============================================*/
void memcopy1
  P4 (( char *, dest ), /* $B0l<!85G[Ns$N%]%$%s%?(B */
      ( char *, src  ), /* $B0l<!85G[Ns$N%]%$%s%?(B */
      ( long  , num  ), /* $BMWAG?t(B               */
      ( long  , size )) /* $BMWAG%5%$%:(B           */



/*==============================================*
 * memfill1 : $B0l<!85G[Ns$N%a%b%j$NFbMF$rKd$a$k!#(B
 *==============================================*/
void memfill1
  P4 (( char *, dest ), /* $B0l<!85G[Ns$N%]%$%s%?(B */
      ( long  , c    ), /* $BKd$a$kJ8;z(B           */
      ( long  , num  ), /* $BMWAG?t(B               */
      ( long  , size )) /* $BMWAG%5%$%:(B           */



/*************************
 * $BFs<!85G[Ns$K4X$9$kA`:n(B
 *************************/

/*=========================================*
 * memnew2 : $BFs<!85G[Ns$N%a%b%j$r3NJ]$9$k!#(B
 *=========================================*/
char **memnew2 /* $BFs<!85G[Ns$N%]%$%s%?(B */
  P3 (( long, xnum ), /* $B#X$NMWAG?t(B */
      ( long, ynum ), /* $B#Y$NMWAG?t(B */
      ( long, size )) /* $BMWAG%5%$%:(B */



/*==========================================*
 * memfree2 : $BFs<!85G[Ns$N%a%b%j$r3+J|$9$k!#(B
 *==========================================*/
void memfree2
  P1 (( char **, ptr )) /* $BFs<!85G[Ns$N%]%$%s%?(B */



/*============================================*
 * memcopy2 : $BFs<!85G[Ns$N%a%b%j$r%3%T!<$9$k!#(B
 *============================================*/
void memcopy2
  P5 (( char **, dest ), /* $BFs<!85G[Ns$N%]%$%s%?(B */
      ( char **, src  ), /* $BFs<!85G[Ns$N%]%$%s%?(B */
      ( long   , xnum ), /* $B#X$NMWAG?t(B           */
      ( long   , ynum ), /* $B#Y$NMWAG?t(B           */
      ( long   , size )) /* $BMWAG%5%$%:(B           */



/*========================================*
 * memfill2 : $BFs<!85G[Ns$N%a%b%j$rKd$a$k!#(B
 *========================================*/
void memfill2
  P5 (( char **, dest ), /* $BFs<!85G[Ns$N%]%$%s%?(B */
      ( long   , c    ), /* $BKd$a$kJ8;z(B           */
      ( long   , xnum ), /* $B#X$NMWAG?t(B           */
      ( long   , ynum ), /* $B#Y$NMWAG?t(B           */
      ( long   , size )) /* $BMWAG%5%$%:(B           */



/*************************
 * $B;0<!85G[Ns$K4X$9$kA`:n(B
 *************************/

/*=========================================*
 * memnew3 : $B;0<!85G[Ns$N%a%b%j$r3NJ]$9$k!#(B
 *=========================================*/
char ***memnew3 /* $B;0<!85G[Ns$N%]%$%s%?(B */
  P4 (( long, xnum ), /* $B#X$NMWAG?t(B */
      ( long, ynum ), /* $B#Y$NMWAG?t(B */
      ( long, znum ), /* $B#Z$NMWAG?t(B */
      ( long, size )) /* $BMWAG%5%$%:(B */



/*==========================================*
 * memfree3 : $B;0<!85G[Ns$N%a%b%j$r3+J|$9$k!#(B
 *==========================================*/
void memfree3
  P1 (( char ***, ptr )) /* $B;0<!85G[Ns$N%]%$%s%?(B */



/*============================================*
 * memcopy3 : $B;0<!85G[Ns$N%a%b%j$r%3%T!<$9$k!#(B
 *============================================*/
void memcopy3
  P6 (( char ***, dest ), /* $B;0<!85G[Ns$N%]%$%s%?(B */
      ( char ***, src  ), /* $B;0<!85G[Ns$N%]%$%s%?(B */
      ( long    , xnum ), /* $B#X$NMWAG?t(B           */
      ( long    , ynum ), /* $B#Y$NMWAG?t(B           */
      ( long    , znum ), /* $B#Z$NMWAG?t(B           */
      ( long    , size )) /* $BMWAG%5%$%:(B           */



/*========================================*
 * memfill3 : $B;0<!85G[Ns$N%a%b%j$rKd$a$k!#(B
 *========================================*/
void memfill3
  P6 (( char ***, dest ), /* $BFs<!85G[Ns$N%]%$%s%?(B */
      ( long    , c    ), /* $BKd$a$kJ8;z(B           */
      ( long    , xnum ), /* $B#X$NMWAG?t(B           */
      ( long    , ynum ), /* $B#Y$NMWAG?t(B           */
      ( long    , znum ), /* $B#Z$NMWAG?t(B           */
      ( long    , size )) /* $BMWAG%5%$%:(B           */



/*****************
 * $BG[NsA`:n%^%/%m(B
 *****************/

/*====================================*
 * typenew   : $B7?$NNN0h$r3NJ]$9$k!#(B
 * typerenew : $B7?$NNN0h$r:F3NJ]$9$k!#(B
 * typefree  : $B7?$NNN0h$r3+J|$9$k!#(B
 *====================================*/
#define typenew( type ) ( type * )memnew( sizeof( type ) )
#define typerenew( ptr, type ) ( type * )memrenew( ( char * )ptr, sizeof( type ))
#define typefree( ptr ) memfree( ( char * )ptr )



/*==============================================*
 * typenew1   : $B0l<!85G[Ns$r7?;XDj$G3NJ]$9$k!#(B
 * typerenew1 : $B0l<!85G[Ns$r7?;XDj$G:F3NJ]$9$k!#(B
 * typefree1  : $B0l<!85G[Ns$r2rJ|$9$k!#(B
 *==============================================*/
#define typenew1( x, type ) ( type * )memnew1( x, sizeof( type ))
#define typerenew1( ptr, x, type ) ( type * )memrenew1( ( char * )ptr, x, sizeof( type ))
#define typefree1( ptr ) memfree1( ( char * )ptr )



/*============================================*
 * typenew2  : $BFs<!85G[Ns$r7?;XDj$G3NJ]$9$k!#(B
 * typefree2 : $BFs<!85G[Ns$r2rJ|$9$k!#(B
 *============================================*/
#define typenew2( x, y, type ) ( type ** )memnew2( x, y, sizeof( type ))
#define typefree2( ptr ) memfree2( ( char ** )ptr )



/*============================================*
 * typenew3  : $B;0<!85G[Ns$r7?;XDj$G3NJ]$9$k!#(B
 * typefree3 : $B;0<!85G[Ns$r2rJ|$9$k!#(B
 *============================================*/
#define typenew3( x, y, z, type ) ( type *** )memnew3( x, y, z, sizeof( type ))
#define typefree3( ptr ) memfree3( ( char *** )ptr )







#endif  //__memlib_h
