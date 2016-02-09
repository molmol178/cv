/*
 * Usage : rgbtorgb [input] 
 * -> input.r, input.g, input.b
 */



#include "Image.h"
#include "Image/Funcs.h"
#include "misc/optlib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>



void image__rgbtorgb
  P4 (( image, destr ),
      ( image, destg ),
      ( image, destb ),
      ( image, src  ))



char *option[] = {
  "The translater RGB to R,G,B separete images.",
  "input:*:1:(image) name of the input rgb color image",
  "help:-h:print help messages",
};



int main( argc, argv )
     int    argc;
     char **argv;
{
  image source, r,g,b;
  char *input;
  char output[1024];
  int i;

  OPTION_SETUP( option );

  if ( argc == 1 ) optusage( 1 );
  if ( optspecified( "help" ) ) optmanual( 1 );

  input  = optvalue( "input"  );

  source = Image.createFromFilename( "SOURCE", input );
  r = Image.create( "Red" );
  g = Image.create( "Green" );
  b = Image.create( "Blue" );
  if ( source == 0 )
    {
      fprintf( stderr, "rgbtoy:can't open file %s\n", input );
      exit( -1 );
    }
  if ( __TYPE( source ) != UChar3 )
    {
      fprintf( stderr, "rgbtoy:image is wrong %s\n", input );
      exit( -1 );
    }

  image__rgbtorgb( r,g,b, source );
  sprintf(output, "%s.r", input);
  Image.save( r, output, "rgbto red" );
  sprintf(output, "%s.g", input);
  Image.save( g, output, "rgbto green" );
  sprintf(output, "%s.b", input);
  Image.save( b, output, "rgbto blue" );

  Image.destroy( source );
  Image.destroy( r );
  Image.destroy( g );
  Image.destroy( b );

  return 0;
}



void image__rgbtorgb
 _P4 (( image, r ),
      ( image, g ),
      ( image, b ),
      ( image, source  ))
{
  register long n;
  register int i;
  register uchar  *rp;
  register uchar  *gp;
  register uchar  *bp;
  register uchar3 *sp;

  Image.make( r, UChar, __XSIZE( source ), __YSIZE( source ) );
  Image.make( g, UChar, __XSIZE( source ), __YSIZE( source ) );
  Image.make( b, UChar, __XSIZE( source ), __YSIZE( source ) );
  n = __AREA( source );

  sp = __RASTER( source, uchar3 );
  rp = __RASTER( r, uchar );
  gp = __RASTER( g, uchar );
  bp = __RASTER( b, uchar );
  for ( i = 0; i < n; i++, sp++, rp++, gp++, bp++ ) {
    *rp = sp->at[ 0 ];
    *gp = sp->at[ 1 ];
    *bp = sp->at[ 2 ];
  }
}
