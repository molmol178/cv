/*
 * filename : arithmetic.c
 * author   : Takahiro Sugiyama
 * date     : Saturday, May 28 1994
 * describe : Arithmetic scaling routine
 */



#include "Image.h"



static int easy_determine
  _P8 (( image , input ),
       ( double, rx    ),
       ( double, ry    ),
       ( int   , px    ),
       ( int   , py    ),
       ( int   , nx    ),
       ( int   , ny    ),
       ( int * , pixel ))
{
  int a, b, c, d;

  a = __PIXEL( input, px, py, bit1 );
  b = __PIXEL( input, nx, py, bit1 );
  c = __PIXEL( input, px, ny, bit1 );
  d = __PIXEL( input, nx, ny, bit1 );

  *pixel = a;
  if (( px == nx ) && ( py == ny )) return 1;
  if (( px == nx ) && a == c ) return 1;
  if (( py == ny ) && a == b ) return 1;
  if ((( px - rx ) == ( py - ry )) && a == d ) return 1;
  if ( a == b && b == c && c == d && d == a ) return 1;

  *pixel = b;
  if ((( nx - rx ) == ( ry - py )) && b == c ) return 1;

  return 0;
}



static void detect_neighbor_region
  _P1 (( image, effect ))
{
  static image label = 0;
  register int x, y, xsize, ysize;
  int cx, cy;
  long effect_label0;
  long effect_label1;
  long effect_label2;
  long effect_label3;

  label = ( label == 0 ) ? Image.create( "label" ) : label;

  xsize = __XSIZE(effect);
  ysize = __YSIZE(effect);

  cx = xsize / 2;
  cy = ysize / 2;

  /* 隣接画素の対象を抽出する */
  image__labeling( label, effect );
  effect_label0 = __PIXEL( label, cx  , cy  , long );
  effect_label1 = __PIXEL( label, cx  , cy+1, long );
  effect_label2 = __PIXEL( label, cx+1, cy  , long );
  effect_label3 = __PIXEL( label, cx+1, cy+1, long );
  for ( y = 0; y < ysize; y++ )
    for ( x = 0; x < xsize; x++ )
      if (( __PIXEL( label, x, y, long ) != effect_label0 ) &&
	  ( __PIXEL( label, x, y, long ) != effect_label1 ) &&
	  ( __PIXEL( label, x, y, long ) != effect_label2 ) &&
	  ( __PIXEL( label, x, y, long ) != effect_label3 ))
	__PIXEL( effect, x, y, bit1 ) = 0;

  /* 塗りつぶす */
  image__labeling( label, effect );
  effect_label0 = __PIXEL( label, cx  , cy  , long );
  effect_label1 = __PIXEL( label, cx  , cy+1, long );
  effect_label2 = __PIXEL( label, cx+1, cy  , long );
  effect_label3 = __PIXEL( label, cx+1, cy+1, long );
  for ( y = 0; y < ysize; y++ )
    for ( x = 0; x < xsize; x++ )
      if (( __PIXEL( label, x, y, long ) != effect_label0 ) &&
	  ( __PIXEL( label, x, y, long ) != effect_label1 ) &&
	  ( __PIXEL( label, x, y, long ) != effect_label2 ) &&
	  ( __PIXEL( label, x, y, long ) != effect_label3 ))
	__PIXEL( effect, x, y, bit1 ) = 1;
}



static int determine_pixel_bit1
  _P3 (( image , input ),
       ( double, rx    ),
       ( double, ry    ))
{
  int pixel;
  register int ix, iy, xx, yy;
  int px, py, nx, ny;
  double dx, dy, yerr, xerr;
  double value, sum, pixel_area;
  static image effect = 0;
  int nearest_x, nearest_y;
  int Nx, Ny;
  double r, R;

  py = ry;
  ny = py + 1;
  if (( ry == (double)py ) || ( ny >= __YSIZE(input) )) ny = py;

  px = rx;
  nx = px + 1;
  if (( rx == (double)px ) || ( nx >= __XSIZE(input) )) nx = px;

  if ( easy_determine( input, rx, ry, px, py, nx, ny, &pixel ) ) return pixel;

  Ny = 5;
  Nx = 5;
  if ( effect == 0 )
    effect = Image.createMake( "effect", Bit1, Nx*2+1, Ny*2+1 );
  image__copyarea( effect, input, 0, 0,
		   px - Nx, py - Ny, __XSIZE(effect), __YSIZE(effect) );

  detect_neighbor_region( effect );

  yerr = ry - py;
  xerr = rx - px;

  value = 0;
  R = Nx * Nx + Ny * Ny;
  sum = 0;
  for ( iy = -Ny; iy <= Ny; iy++ )
    {
      dy = 1;
      if ( iy == -Ny ) dy = 1 - yerr;
      if ( iy ==  Ny ) dy = 1 + yerr;

      for ( ix = -Nx; ix <= Nx; ix++ )
	{
	  dx = 1;
	  if ( ix == -Nx ) dx = 1 - xerr;
	  if ( ix ==  Nx ) dx = 1 + xerr;

	  r = ix * ix + iy * iy;
	  if ( r > R ) continue;

	  pixel_area = dy * dx;
	  value += pixel_area * __PIXEL( effect, ix + Nx, iy + Ny, bit1 );
	  sum += pixel_area;
	}
    }

  return ((value / sum) >= 0.5) ? 1 : 0;
}



void image__scaling_arithmetic
  _P4 (( image, output    ),
       ( image, input     ),
       ( long , new_xsize ),
       ( long , new_ysize ))
{
  int x, y, old_xsize, old_ysize;
  register bit1 **new_data;
  register bit1 **old_data;
  double rx, ry;

  Image.make( output, __TYPE( input ), new_xsize, new_ysize );
  old_xsize = __XSIZE( input );
  old_ysize = __YSIZE( input );

  old_data = __DATA( input , bit1 );
  new_data = __DATA( output, bit1 );

  for ( y = 0; y < new_ysize; y++ )
    {
      ry = (double)y * old_ysize / new_ysize;
      for ( x = 0; x < new_xsize; x++ )
	{
	  rx = (double)x * old_xsize / new_xsize;
	  new_data[ y ][ x ] = determine_pixel_bit1( input, rx, ry );
	}
    }
}
