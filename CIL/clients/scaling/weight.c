/*
 * filename : weight.c
 * author   : Takahiro Sugiyama
 * date     : Friday, May 27 1994
 * describe : Weight Average Scale
 */



#include "Image.h"



/*==================================================================*
 * image__scaling_weight_average : 荷重平均補間を用いてリサイズする
 *==================================================================*/
void image__scaling_weight_average
  _P4 (( image, output    ),
       ( image, input     ),
       ( long , new_xsize ),
       ( long , new_ysize ))
{
  long x, y, old_xsize, old_ysize;
  register bit1 **new_data;
  register bit1 **old_data;
  double ry, rx, dx, dy, dd;
  int px, py, nx, ny;

  Image.make( output, __TYPE( input ), new_xsize, new_ysize );
  old_xsize = __XSIZE( input );
  old_ysize = __YSIZE( input );

  old_data = __DATA( input , bit1 );
  new_data = __DATA( output, bit1 );

  dx = new_xsize / old_xsize;
  dy = new_ysize / old_ysize;
  dd = sqrt( dx*dx + dy*dy );

  for ( y = 0; y < new_ysize; y++ )
    {
      ry = (double)y * (old_ysize-1) / (new_ysize-1);
      py = ry;
      ny = py + 1;
      if (( ry == (double)py ) || ( ny >= old_ysize )) ny = py;

      for ( x = 0; x < new_xsize; x++ )
	{
	  rx = (double)x * (old_xsize-1) / (new_xsize-1);
	  px = rx;
	  nx = px + 1;
	  if (( rx == (double)px ) || ( nx >= old_xsize )) nx = px;

	  {
	    int a, b, c, d;
	    double A, wa, wb, wc, wd;
	    a = old_data[py][px];
	    b = old_data[py][nx];
	    c = old_data[ny][px];
	    d = old_data[ny][nx];
	    wa = 1.0 / ( dd + sqrt( (px-rx)*(px-rx) + (py-ry)*(py-ry) ) );
	    wb = 1.0 / ( dd + sqrt( (nx-rx)*(nx-rx) + (py-ry)*(py-ry) ) );
	    wc = 1.0 / ( dd + sqrt( (px-rx)*(px-rx) + (ny-ry)*(ny-ry) ) );
	    wd = 1.0 / ( dd + sqrt( (nx-rx)*(nx-rx) + (ny-ry)*(ny-ry) ) );
	    A = (wa*a + wb*b + wc*c + wd*d) / (wa + wb + wc + wd);
	    new_data[ y ][ x ] = ( A >= 0.5 ) ? 1 : 0;
	  }
	}
    }
}
