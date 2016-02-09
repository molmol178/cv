/*
 * Usage : rgbtoy [input] [output]
 *
 * [RGB to YIQ]:
 *      Y = 0.299*R + 0.587*G + 0.114*B;
 *      I = 0.596*R - 0.274*G - 0.322*B;
 *      Q = 0.211*R - 0.523*G + 0.312*B;
 */



#include "Image.h"
#include "Image/Funcs.h"
#include "misc/optlib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>



void image__rgbtoy
  P2 (( image, dest ),
      ( image, src  ))



char *option[] = {
  "The translater RGB to Y of YIQ.",
  "[ Y = 0.299 * red + 0.587 * green + 0.114 * blue ].",
  "input:*:1:(image) name of the input rgb color image",
  "output:*:1:(image) name of the output gray-scale image",
  "help:-h:print help messages",
};



int main( argc, argv )
     int    argc;
     char **argv;
{
  image source, yofyiq;
  char *input;
  char *output;
  int i;

  OPTION_SETUP( option );

  if ( argc == 1 ) optusage( 1 );
  if ( optspecified( "help" ) ) optmanual( 1 );

  input  = optvalue( "input"  );
  output = optvalue( "output" );

  source = Image.createFromFilename( "SOURCE", input );
  yofyiq = Image.create( "YOFYIQ" );
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

  image__rgbtoy( yofyiq, source );
  Image.save( yofyiq, output, "rgbtoy" );

  Image.destroy( source );
  Image.destroy( yofyiq );

  return 0;
}



void image__rgbtoy
  _P2 (( image, dest    ),
       ( image, source  ))
{
  register long n;
  register int i;
  register uchar  *dp;
  register uchar3 *sp;

  Image.make( dest, UChar, __XSIZE( source ), __YSIZE( source ) );
  n = __AREA( source );

  sp = __RASTER( source, uchar3 );
  dp = __RASTER( dest, uchar );
  for ( i = 0; i < n; i++, sp++ )
    *dp++ = 0.299 * sp->at[ 0 ] + 0.587 * sp->at[ 1 ] + 0.114 * sp->at[ 2 ];
}
