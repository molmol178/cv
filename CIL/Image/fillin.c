/*
 * fillin.c
 */



#include "Funcs.h"
#include <stdio.h>
#include "misc/memlib.h"


/*
 * スキャンパターンの道筋を作る
 */
static void fill_in_init(src,paths)
     image src;
     int **paths[4];
{
  register int x, y, n;
  int xsize,ysize;
  register int **path;

  xsize = __XSIZE( src );
  ysize = __YSIZE( src );

  /* bottom -> top */
  path = paths[ 0 ];
  n = 1;
  for ( y = 0; y < ysize; y++ )
    if ( y % 2 ) /* odd */
      for ( x = xsize - 1; x >= 0; x-- )
	path[ y ][ x ] = n++;
    else /* even */
      for ( x = 0; x < xsize; x++ )
	path[ y ][ x ] = n++;

  /* top -> bottom */
  path = paths[ 1 ];
  n = 1;
  for ( y = ysize - 1; y >= 0 ; y-- )
    if ( y % 2 ) /* odd */
      for ( x = 0; x < xsize; x++ )
	path[ y ][ x ] = n++;
    else /* even */
      for ( x = xsize - 1; x >= 0; x-- )
	path[ y ][ x ] = n++;

  /* left -> right */
  path = paths[ 2 ];
  n = 1;
  for ( x = 0; x < xsize; x++ )
    if ( y % 2 ) /* odd */
      for ( y = ysize - 1; y >= 0 ; y-- )
	path[ y ][ x ] = n++;
    else /* even */
      for ( y = 0; y < ysize; y++ )
	path[ y ][ x ] = n++;

  /* right -> left */
  path = paths[ 3 ];
  n = 1;
  for ( x = xsize - 1; x >= 0; x-- )
    if ( y % 2 ) /* odd */
      for ( y = ysize - 1; y >= 0 ; y-- )
	path[ y ][ x ] = n++;
    else /* even */
      for ( y = 0; y < ysize; y++ )
	path[ y ][ x ] = n++;
}



/*
 * １画面塗りつぶし処理
 */
static void scan_and_fill( src, path )
     image src;
     register int **path;
{
  register int i;
  register int x,y;
  register uchar **data;
  int find;
  int xsize, ysize, area;

  xsize = __XSIZE( src );
  ysize = __YSIZE( src );
  data  = __DATA( src, uchar );

  find = 0;
  for ( y = 0; y < ysize; y++ )
    {
      for ( x = 0; x < xsize; x++ )
	if ( path[ y ][ x ] == 1 )
	  {
	    find = 1;
	    break;
	  }
      if ( find ) break;
    }

  xsize--;
  ysize--;
  area = __AREA( src );
  for ( i = 1; i <= area; i++ )
    {
      if ( data[ y ][ x ] == 0 )
	{
	  if ( x == xsize || y == ysize || x == 0 ||y == 0 ||
	       data[ y ][ x + 1 ] == 128 || data[ y + 1 ][ x ] == 128 ||
	       data[ y ][ x - 1 ] == 128 || data[ y - 1 ][ x ] == 128 )
	    data[ y ][ x ] = 128;
	}
      if (( x < xsize ) && ( path[ y ][ x + 1 ] == i )) x++;
      if (( y < ysize ) && ( path[ y + 1 ][ x ] == i )) y++;
      if (( x > 0     ) && ( path[ y ][ x - 1 ] == i )) x--;
      if (( y > 0     ) && ( path[ y - 1 ][ x ] == i )) y--;
    }
}



/*
 * ４方向スキャン塗りつぶし処理
 */
void image__fill_in
  _P2 (( image, dest ),
       ( image, src  ))
{
  image tmp;
  register int i, area;
  register uchar *p;
  int **paths[4];  /* スキャンパターン */

  if ( __TYPE( src ) != Bit1 )
    {
      fprintf( stderr, "warning:image__fill_in:type is wrong.\n" );
      return;
    }

  tmp = Image.createFromImage( "tmp", src );

  for ( i = 0; i < 4; i++ )
    paths[ i ] = typenew2( __XSIZE( src ), __YSIZE( src ), int );

  fill_in_init( tmp, paths );

  for ( i = 0; i < 4; i++ )
    scan_and_fill( tmp, paths[ i ] );

  p = __RASTER( tmp, uchar );
  area = __AREA( tmp );
  for ( i = 0; i < area; i++)
    p[ i ] = ( p[ i ] == 128 ) ? 0 : 1;

  for ( i = 0; i < 4; i++ )
    typefree2( paths[ i ] );

  Image.copy( dest, tmp );
  Image.destroy( tmp );
}
