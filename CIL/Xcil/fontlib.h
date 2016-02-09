/*
 * fontname : fontlib.h
 * author   : Takahiro Sugiyama
 * date     : 1993/09/17
 */



#ifndef __fontlib_h
#define __fontlib_h



#include "../misc/comacros.h"



/*================================*
 * fontRec : フォント構造体の定義
 *================================*/
struct fontRec {
  char *name;
  long xsize,ysize;
  char *data;
};



/*==================================*
 * fontsearch : フォントを検索する。
 *==================================*/
struct fontRec *fontsearch /* フォント構造体 */
  P1 (( char *, name )) /* フォント名 */



#endif  __fontlib_h
