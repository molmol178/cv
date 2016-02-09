/*
 * Usage : histogramflattering [input] [output]
 */



#include "Funcs.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "misc/memlib.h"



typedef struct {
  short x;
  short y;
  ushort p[3];
} histRec;



static histRec *make_histogram
  P1 (( register image, src ))

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
  register histRec *hist;
  long area;
  long pix;

  hist = make_histogram(src);

  Image.make( histogramflattering, UChar, __XSIZE( src ), __YSIZE( src ) );

  area = __AREA( src );

  for ( i = 0; i < area; i++ )
    {
      __PIXEL( histogramflattering, hist[i].x, hist[i].y, uchar ) = tone * i / area;
    }
  typefree1( hist );
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



static int cmp_ext
  _P2 (( histRec *, a ),
       ( histRec *, b ))
{
  if ( a->p[ 0 ] != b->p[ 0 ] ) return a->p[ 0 ] - b->p[ 0 ];
  if ( a->p[ 1 ] != b->p[ 1 ] ) return a->p[ 1 ] - b->p[ 1 ];
  if ( a->p[ 2 ] != b->p[ 2 ] ) return a->p[ 2 ] - b->p[ 2 ];

  return 0;
}

static histRec *make_histogram
  _P1 (( register image, src ))
{
  histRec *hist;
  register long i ,x, y;
  long p;

  hist = typenew1( __AREA( src ), histRec );

  i = 0;
  for ( y = 0; y < __YSIZE( src ); y++ )
    for ( x = 0; x < __XSIZE( src ); x++ )
      {
	long lp, lx, ly;
	register histRec *g;

	p = __PIXEL( src, x, y, uchar );
	g = &hist[ i++ ];
	g->x = x;
	g->y = y;
	g->p[ 0 ] = p;
	g->p[ 1 ] = p;
	g->p[ 2 ] = p;

	for (ly = -1; ly <= 1; ly ++)
	 {
	   long yy = y + ly;
	   yy = __FIX_Y( src, yy );

	   for (lx = -1; lx <= 1; lx ++)
	     {
	       long xx = x + lx;
	       xx = __FIX_X( src, xx );

	       lp = __PIXEL( src, xx, yy, uchar );
	       if ( lx == 0 || ly == 0 ) g->p[ 1 ] += lp;
	       g->p[ 2 ] += lp;
	     }
	 }
      }
#ifdef __ANSI__
  qsort( hist, i, sizeof( histRec ), (int (* )(const void *  , const void * ))cmp_ext );
#else
  qsort( hist, i, sizeof( histRec ), (int (* )())cmp_ext );
#endif

  return hist;
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
