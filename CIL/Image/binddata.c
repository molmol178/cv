/*
 * filename : bindata.c
 * author   : Takahiro Sugiyama
 * date     : Thursday, April 7 1994
 * describe : bind data
 */



#include "Funcs.h"



/*=========================================*
 * image__bind_data : データをバインドする
 *=========================================*/
void image__bind_data
  _P5 (( image  , output ),
       ( char **, data   ),
       ( long   , type   ),
       ( long   , xsize  ),
       ( long   , ysize  ))
{
  //__DATA( output, char ) = data;
  output->data = data;
  __TYPE( output ) = type;
  __XSIZE( output ) = xsize;
  __YSIZE( output ) = ysize;
}
