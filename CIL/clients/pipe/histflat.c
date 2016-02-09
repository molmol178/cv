/*
 * Usage : histogramflattering [input] [output]
 */



#include "Image.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "misc/memlib.h"



void image__histogramflattering
  P3 (( register image, histogramflattering ),
      ( register image, src                 ),
      ( long	      , tone	            ))

void image__histogramflattering_color
  P3 (( register image, histogramflattering ),
      ( register image, src                 ),
      ( long          , tone                ))



void main( argc, argv )
     int    argc;
     char **argv;
{
  image src,del;
  char *input  = "-";
  char *output = "-";
  long tone = 256;
  int i;

  if (( argc == 2 ) && (( strcmp( argv[ 1 ], "help" ) == 0 ) ||
			( strcmp( argv[ 1 ], "-h" ) == 0 )))
    {
      fprintf( stderr, "Usage : histogramflattering [input(uchar)] [output(uchar)] [tone]\n");
      exit( 1 );
    }

  i = 0;
  if ( argc > ++i ) input  = argv[ i ];
  if ( argc > ++i ) output = argv[ i ];
  if ( argc > ++i ) tone   = atol( argv[ i ] );

  src = Image.createFromFilename( "SOURCE", input );
  del = Image.create( "FLAT" );
  if ( src == 0 )
    {
      fprintf( stderr, "histogramflattering:can't open file %s\n", input );
      exit( -1 );
    }

  fprintf(stderr,"Histogram flattering.\n");
  if ( typesize( __TYPE( src ) ) == 1 )
    {
      image__histogramflattering( del, src, tone );
      if ( tone == 2  ) __TYPE( del ) = Bit1;
      if ( tone == 16 ) __TYPE( del ) = Bit4;
    }
  else
  if ( __TYPE( src ) == UChar3 )
    image__histogramflattering_color( del, src, tone );
  else
    {
      fprintf( stderr, "histogramflattering:image is wrong %s\n", input );
      exit( -1 );
    }

  Image.save( del, output, "histogramflattering" );

  Image.destroy( src );
  Image.destroy( del );
}



typedef struct {
  short x;
  short y;
  ushort p[3];
} histRec;


histRec *make_histogram
  P1 (( register image, src  ))


void image__histogramflattering
  _P3 (( register image, histogramflattering ),
       ( register image, src                 ),
       ( long          , tone                ))
{
  register long i;
  register histRec *hist;
  register long area;

  hist = make_histogram( src );

  Image.make( histogramflattering, UChar, __XSIZE( src ), __YSIZE( src ) );

  area = __AREA( src );

  for ( i = 0; i < area; i++ )
    {
      __PIXEL( histogramflattering, hist[ i ].x, hist[ i ].y, uchar )
	= tone * i / area;
    }

  typefree1( hist );
}



void image__separate3
  _P4 (( register image, r   ),
       ( register image, g   ),
       ( register image, b   ),
       ( register image, src ))
{
  register int i,n;

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



void image__conbination3
  _P4 (( register image, src ),
       ( register image, r   ),
       ( register image, g   ),
       ( register image, b   ))
{
  register int i, n;

  Image.make( src, UChar3, __XSIZE( r ), __YSIZE( r ) );

  n = __AREA( src );
  for ( i = 0; i < n; i++ )
    {
      __RAXEL0( src, i, uchar3 ) = __RAXEL( r, i, uchar );
      __RAXEL1( src, i, uchar3 ) = __RAXEL( g, i, uchar );
      __RAXEL2( src, i, uchar3 ) = __RAXEL( b, i, uchar );
    }
}



void image__histogramflattering_color
  _P3 (( register image, histogramflattering ),
       ( register image, src                 ),
       ( long          , tone                ))
{
  register long i;
  image r, g, b, hr, hg, hb;

  r  = Image.create( "r"  );
  g  = Image.create( "g"  );
  b  = Image.create( "b"  );
  hr = Image.create( "hr" );
  hg = Image.create( "hg" );
  hb = Image.create( "hb" );

  image__separate3( r, g, b, src ); Image.free( src );
  image__histogramflattering( hr, r, tone ); Image.destroy( r );
  image__histogramflattering( hg, g, tone ); Image.destroy( g );
  image__histogramflattering( hb, b, tone ); Image.destroy( b );
  image__conbination3( histogramflattering, hr, hg, hb );

  Image.destroy( hr );
  Image.destroy( hg );
  Image.destroy( hb );
}



int cmp_ext
  _P2 (( histRec *, a ),
       ( histRec *, b ))
{
  if ( a->p[ 0 ] != b->p[ 0 ] ) return a->p[ 0 ] - b->p[ 0 ];
  if ( a->p[ 1 ] != b->p[ 1 ] ) return a->p[ 1 ] - b->p[ 1 ];
  if ( a->p[ 2 ] != b->p[ 2 ] ) return a->p[ 2 ] - b->p[ 2 ];

  return 0;
}

histRec *make_histogram
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

  qsort( hist, i, sizeof( histRec ), cmp_ext );

  return hist;
}
