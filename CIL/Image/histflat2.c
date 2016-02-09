/*
 * Usage : histogramflattering [input] [output]
 */



#include "Funcs.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "misc/memlib.h"



static void histogram_flattering_uchar
  P3 (( register image, histogramflattering ),
      ( register image, src         ),
      ( long	      , tone	    ))

static void histogram_flattering_uchar3
  P3 (( register image, histogramflattering ),
      ( register image, src        ),
      ( long          , tone       ))



static void histogram_flattering_uchar
  _P3 (( register image, histogramflattering ),
       ( register image, src        ),
       ( long          , tone       ))
{
  register long i;
  register uchar *p, *q;
  double cdf[256];
  long area;

  area = __AREA( src );
  p = __RASTER(src, uchar);
  for (i = 0; i < 256; i++) cdf[i] = 0; /* 初期化 */
  for (i = 0; i < area; i++, p++) cdf[*p]++; /* ヒストグラム */
  for (i = 1; i < 256; i++) cdf[i] += cdf[i - 1]; /* 累積 */
  for (i = 0; i < 256; i++) cdf[i] = 255 * cdf[i] / area; /* 256正規化 */
  /* for (i = 0; i < 256; i++) fprintf(stderr, "%3.0f ", cdf[i]); */

  if (src != histogramflattering)
    Image.make( histogramflattering, UChar, __XSIZE( src ), __YSIZE( src ) );
  p = __RASTER(src, uchar);
  q = __RASTER(histogramflattering, uchar);
  for ( i = 0; i < area; i++, p++, q++)
    *q = cdf[*p];
}



static void image__separate3
  _P4 (( image, r   ),
       ( image, g   ),
       ( image, b   ),
       ( image, src ))
{
  int i,n;

  Image.make( r, UChar, __XSIZE( src ), __YSIZE( src ) );
  Image.make( g, UChar, __XSIZE( src ), __YSIZE( src ) );
  Image.make( b, UChar, __XSIZE( src ), __YSIZE( src ) );

  n = __AREA( src );
  for ( i = 0; i < n; i++ )
    {
      __RAXEL( r, i, uchar ) = __RAXEL0( src, i, uchar3 );
      __RAXEL( g, i, uchar ) = __RAXEL1( src, i, uchar3 );
      __RAXEL( b, i, uchar ) = __RAXEL2( src, i, uchar3 );
    }
}



static void image__conbination3
  _P4 (( image, src ),
       ( image, r   ),
       ( image, g   ),
       ( image, b   ))
{
  int i,n;

  Image.make( src, UChar3, __XSIZE( r ), __YSIZE( r ) );

  n = __AREA( src );
  for ( i = 0; i < n; i++ )
    {
      __RAXEL0( src, i, uchar3 ) = __RAXEL( r, i, uchar );
      __RAXEL1( src, i, uchar3 ) = __RAXEL( g, i, uchar );
      __RAXEL2( src, i, uchar3 ) = __RAXEL( b, i, uchar );
    }
}



static void histogram_flattering_uchar3
  _P3 (( register image, histogramflattering ),
       ( register image, src        ),
       ( long          , tone       ))
{
  register long i;
  image r,g,b,hr,hg,hb;

  r = Image.create("r");
  g = Image.create("g");
  b = Image.create("b");
  hr = Image.create("hr");
  hg = Image.create("hg");
  hb = Image.create("hb");

  image__separate3( r, g, b, src ); Image.free( src );
  histogram_flattering_uchar( hr, r, tone ); Image.destroy( r );
  histogram_flattering_uchar( hg, g, tone ); Image.destroy( g );
  histogram_flattering_uchar( hb, b, tone ); Image.destroy( b );
  image__conbination3( histogramflattering, hr, hg, hb );

  Image.destroy( hr );
  Image.destroy( hg );
  Image.destroy( hb );
}



typedef void (*f_caller)
     P3 (( image, out  ),
	 ( image, in   ),
	 ( long , tone ))



static caller_table table[] = {
  {UChar, histogram_flattering_uchar },
  {UChar3,histogram_flattering_uchar3},
};



void image__histogram_flattering
  _P3 (( image, out  ),
       ( image, in   ),
       ( long , tone ))
{
  f_caller caller;

  caller = (f_caller)image__caller("image__histogram_flattering",
				   in,Number(table),table);

  if (caller) caller(out,in,tone);
}
