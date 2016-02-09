/*
 * filename : liner.c
 * author   : Takahiro Sugiyama
 * date     : Friday, May 27 1994
 * describe : Liner Complement
 */



#include "Image.h"



/*================================================================*
 * image__scaling_liner_complement : 線形補間を用いてリサイズする
 *================================================================*/
void image__scaling_liner_complement
  _P4 (( image, output    ),
       ( image, input     ),
       ( long , new_xsize ),
       ( long , new_ysize ))
{
  long x, y, old_xsize, old_ysize;
  register bit1 **new_data;
  register bit1 **old_data;
  double ry, rx;
  int px, py, nx, ny;

  Image.make( output, __TYPE( input ), new_xsize, new_ysize );
  old_xsize = __XSIZE( input );
  old_ysize = __YSIZE( input );

  old_data = __DATA( input , bit1 );
  new_data = __DATA( output, bit1 );

  for ( y = 0; y < new_ysize; y++ )
    {
      ry = (double)y * old_ysize / new_ysize;
      py = ry;
      ny = py + 1;
      if (( ry == (double)py ) || ( ny >= old_ysize )) ny = py;

      for ( x = 0; x < new_xsize; x++ )
	{
	  rx = (double)x * old_xsize / new_xsize;
	  px = rx;
	  nx = px + 1;
	  if (( rx == (double)px ) || ( nx >= old_xsize )) nx = px;

	  {
	    int a, b, c, d, A;
	    a = old_data[py][px];
	    b = old_data[py][nx];
	    c = old_data[ny][px];
	    d = old_data[ny][nx];
	    A = a + b + c + d;
	    new_data[ y ][ x ] = ( A >= 2 ) ? 1 : 0;
	  }
	}
    }
}
