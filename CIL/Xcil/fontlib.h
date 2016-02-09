/*
 * fontname : fontlib.h
 * author   : Takahiro Sugiyama
 * date     : 1993/09/17
 */



#ifndef __fontlib_h
#define __fontlib_h



#include "../misc/comacros.h"



/*================================*
 * fontRec : $@%U%)%s%H9=B$BN$NDj5A(J
 *================================*/
struct fontRec {
  char *name;
  long xsize,ysize;
  char *data;
};



/*==================================*
 * fontsearch : $@%U%)%s%H$r8!:w$9$k!#(J
 *==================================*/
struct fontRec *fontsearch /* $@%U%)%s%H9=B$BN(J */
  P1 (( char *, name )) /* $@%U%)%s%HL>(J */



#endif  __fontlib_h
