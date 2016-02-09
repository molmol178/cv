/*
 * filename : xmaclib.h
 * author   : Takahiro Sugiyama
 * date     : Monday, January 16 1995
 * describe : Macintosh GUI for X Window.
 */




#ifndef __xmaclib_h
#define __xmaclib_h



#include "comacros.h"



typedef struct mac_item {
  char *label;
  char *value;
} mac_item;



/*===============================================*
 * xmac_file_dialog : ファイルダイアログを開く。
 *===============================================*/
long xmac_file_dialog
  _P3 (( char *, filename ), /* 選択されたファイル名 */
       ( char *, mode     ),
       ( , ))
{
  return ;
}



#endif //__xmaclib_h
