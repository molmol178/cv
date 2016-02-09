/*
 * patchimage.c
 */



#include "Image.h"
#include "Image/Funcs.h"
#include "misc/optlib.h"
#include "misc/memlib.h"
#include <stdio.h>



void patch_image
  P3 (( image , patch  ),
      ( image , source ),
      ( image , label  ))



long flag__use_average    = 0;
long flag__need_bound     = 1;
long flag__need_minus     = 1;
long flag__need_uncertain = 1;

long value__bound_m = 255;
long value__bound_r = 255;
long value__bound_g = 255;
long value__bound_b = 255;

long value__minus_m = -1;
long value__minus_r = -1;
long value__minus_g = -1;
long value__minus_b = -1;

long value__uncertain_m = -1;
long value__uncertain_r = -1;
long value__uncertain_g = -1;
long value__uncertain_b = -1;



char *option[] =
{
  "original:*:1:(image) name of the input color or gray image",
  "label:*:1:(image) name of the input label image",
  "output:*:1:(image) name of the output patched image",

  "help:-h:print this message",

  "scale:-s:1:1.0:(float) scale of the image size",

  "use-average:-a:use the mean of intensity in a region",

  "no-bound:-nb:no bound",
  "no-minus-region:-nm:use minus regions as normal regions",
  "no-uncertain-region:-nu:use uncertain regions as normal regions",

  "bound-gray:-bm:1:255:(uchar) intensity of the boundaries",
  "bound-color:-bc:3:255:255:255:(uchar) color of the boundaries",

  "minus-gray:-mm:1:-1:(uchar) intensity of the minus regions",
  "minus-color:-mc:3:-1:-1:-1:(uchar) color of the minus region",

  "uncertain-gray:-um:1:-1:(uchar) intensity of the uncertain regions",
  "uncertain-color:-uc:3:-1:-1:-1:(uchar) color of the uncertain regions",
};



int main( argc, argv )
     int    argc;
     char **argv;
{
  image  source;
  image  label;
  image  patch;
  char  *input1;
  char  *input2;
  char  *output;
  double scale;

  OPTION_SETUP( option );

  if ( argc == 1 ) optusage( 1 );
  if ( optspecified( "help" ) ) optmanual( 1 );


  input1 = optvalue( "original" );
  input2 = optvalue( "label" );
  output = optvalue( "output" );

  flag__use_average    = optspecified( "use-average" );
  flag__need_bound     = ! optspecified( "no-bound"     );
  flag__need_minus     = ! optspecified( "no-minus-region"     );
  flag__need_uncertain = ! optspecified( "no-uncertain-region" );

  value__bound_m = optvalueint( "bound-gray" );
  value__bound_r = optnvalueint( "bound-color", 0 );
  value__bound_g = optnvalueint( "bound-color", 1 );
  value__bound_b = optnvalueint( "bound-color", 2 );

  value__minus_m = optvalueint( "minus-gray" );
  value__minus_r = optnvalueint( "minus-color", 0 );
  value__minus_g = optnvalueint( "minus-color", 1 );
  value__minus_b = optnvalueint( "minus-color", 2 );

  value__uncertain_m = optvalueint( "uncertain-gray" );
  value__uncertain_r = optnvalueint( "uncertain-color", 0 );
  value__uncertain_g = optnvalueint( "uncertain-color", 1 );
  value__uncertain_b = optnvalueint( "uncertain-color", 2 );



  source = Image.createFromFilename( "SOURCE", input1 );
  label  = Image.createFromFilename( "LABEL" , input2 );
  patch  = Image.create( "PATCH" );
  if ( source == 0 )
    {
      fprintf( stderr, "patchimage:can't open file %s\n", input1 );
      exit( -1 );
    }

  if ( label == 0 )
    {
      fprintf( stderr, "patchimage:can't open file %s\n", input1 );
      exit( -1 );
    }

  image__long_type( label, label );

  scale = optvaluefloat( "scale" );
  Image.resize( source, source,
	        (long)(__XSIZE(source)*scale), (long)(__YSIZE(source)*scale) );
  Image.resize( label, label, __XSIZE(source), __YSIZE(source) );
  patch_image( patch, source, label );

  Image.save( patch, output, "patch image" );

  Image.destroy( source );
  Image.destroy( label );
  Image.destroy( patch );

  return 0;
}



void patch_image_uchar
  P3 (( image , patch  ),
      ( image , source ),
      ( image , label  ))

void patch_image_uchar3
  P3 (( image , patch  ),
      ( image , source ),
      ( image , label  ))

void patch_image
  _P3 (( image , patch  ),
       ( image , source ),
       ( image , label  ))
{
  switch ( __TYPE( source ) )
    {
    case UChar:
      patch_image_uchar( patch, source, label );
      break;

    case UChar3:
      patch_image_uchar3( patch, source, label );
      break;

    default:
      fprintf( stderr, "patchimage:image type is wrong\n" );
      exit( -1 );
    }
}



void patch_image_uchar
  _P3 (( image , patch  ),
       ( image , source ),
       ( image , label  ))
{
  image newlabel;
  long *area_table, *m_table;
  long n, x, y, xsize, ysize;
  long index, pixval;
  long m, om;

  newlabel = Image.create( "NEWLABEL" );
  n = image__labeling( newlabel, label );

  xsize = __XSIZE( source );
  ysize = __YSIZE( source );
  area_table = typenew1( n + 1, long );
  m_table = typenew1( n + 1, long );

  Image.copy( patch, source );

  for ( y = 0; y < ysize; y++ )
    for ( x = 0; x < xsize; x++ )
      {
	index = __PIXEL( newlabel, x, y, long );
	m_table[ index ] += __PIXEL( source, x, y, uchar );
	area_table[ index ]++;
      }

  for ( index = 0; index <= n; index++ )
    if ( area_table[ index ] ) m_table[ index ] /= area_table[ index ];

  for ( y = 0; y < ysize; y++ )
    for ( x = 0; x < xsize; x++ )
      {
	index  = __PIXEL( newlabel, x, y, long );
	pixval = __PIXEL( label, x, y, long );
	m = m_table[ index ];
	om = __PIXEL( source, x, y, uchar );

	if ( flag__use_average )
	  {
	    __PIXEL( patch, x, y, uchar ) = m;
	  }

	if (( flag__need_uncertain ) && ( pixval == 0 ))
	  {
	    __PIXEL( patch, x, y, uchar )
	      = ( value__uncertain_m >= 0 ) ? value__uncertain_m : om;
	  }

	if (( flag__need_minus ) && ( pixval < 0 ))
	  {
	    __PIXEL( patch, x, y, uchar )
	      = ( value__minus_m >= 0 ) ? value__minus_m : om;
	  }

	if (( flag__need_bound ) &&
	    ( x == 0 || y == 0 || x == ( xsize - 1 ) || y == ( ysize - 1 ) ||
	     ( pixval != __PIXEL( label, x+1, y  , long )) ||
	     ( pixval != __PIXEL( label, x  , y+1, long )) ||
	     ( pixval != __PIXEL( label, x+1, y+1, long ))))
	  {
	    __PIXEL( patch, x, y, uchar ) = value__bound_m;
	  }
      }
}



void patch_image_uchar3
  _P3 (( image , patch  ),
       ( image , source ),
       ( image , label  ))
{
  image newlabel;
  long *area_table;
  long *r_table, *g_table, *b_table;
  long n, x, y, xsize, ysize;
  long index, pixval;
  long r, g, b, or, og, ob;

  newlabel = Image.create( "NEWLABEL" );
  n = image__labeling( newlabel, label );

  xsize = __XSIZE( source );
  ysize = __YSIZE( source );
  area_table = typenew1( n + 1, long );
  r_table = typenew1( n + 1, long );
  g_table = typenew1( n + 1, long );
  b_table = typenew1( n + 1, long );

  Image.copy( patch, source );

  for ( y = 0; y < ysize; y++ )
    for ( x = 0; x < xsize; x++ )
      {
	index = __PIXEL( newlabel, x, y, long );
	r_table[ index ] += __PIXEL0( source, x, y, uchar3 );
	g_table[ index ] += __PIXEL1( source, x, y, uchar3 );
	b_table[ index ] += __PIXEL2( source, x, y, uchar3 );
	area_table[ index ]++;
      }

  for ( index = 0; index <= n; index++ )
    if ( area_table[ index ] )
      {
	r_table[ index ] /= area_table[ index ];
	g_table[ index ] /= area_table[ index ];
	b_table[ index ] /= area_table[ index ];
      }

  for ( y = 0; y < ysize; y++ )
    for ( x = 0; x < xsize; x++ )
      {
	index = __PIXEL( newlabel, x, y, long );
	pixval = __PIXEL( label, x, y, long );
	r = r_table[ index ];
	g = g_table[ index ];
	b = b_table[ index ];

	or = __PIXEL0( source, x, y, uchar3 );
	og = __PIXEL0( source, x, y, uchar3 );
	ob = __PIXEL0( source, x, y, uchar3 );

	if ( flag__use_average )
	  {
	    __PIXEL0( patch, x, y, uchar3 ) = r;
	    __PIXEL1( patch, x, y, uchar3 ) = g;
	    __PIXEL2( patch, x, y, uchar3 ) = b;
	  }

	if (( flag__need_uncertain ) && ( pixval == 0 ))
	  {
	    __PIXEL0( patch, x, y, uchar3 ) = ( value__uncertain_r >= 0 ) ? value__uncertain_r : or;
	    __PIXEL1( patch, x, y, uchar3 ) = ( value__uncertain_g >= 0 ) ? value__uncertain_g : og;
	    __PIXEL2( patch, x, y, uchar3 ) = ( value__uncertain_b >= 0 ) ? value__uncertain_b : ob;
	  }

	if (( flag__need_minus ) && ( pixval < 0 ))
	  {
	    __PIXEL0( patch, x, y, uchar3 ) = ( value__minus_r >= 0 ) ? value__minus_r : or;
	    __PIXEL1( patch, x, y, uchar3 ) = ( value__minus_g >= 0 ) ? value__minus_g : og;
	    __PIXEL2( patch, x, y, uchar3 ) = ( value__minus_b >= 0 ) ? value__minus_b : ob;
	  }

	if (( flag__need_bound ) &&
	    ( x == 0 || y == 0 || x == ( xsize - 1 ) || y == ( ysize - 1 ) ||
	     ( pixval != __PIXEL( label, x+1, y  , long )) ||
	     ( pixval != __PIXEL( label, x  , y+1, long )) ||
	     ( pixval != __PIXEL( label, x+1, y+1, long ))))
	  {
	    __PIXEL0( patch, x, y, uchar3 ) = value__bound_r;
	    __PIXEL1( patch, x, y, uchar3 ) = value__bound_g;
	    __PIXEL2( patch, x, y, uchar3 ) = value__bound_b;
	  }
      }
}
