/*
 * Usage : bithresholding [input] [output]
 */



#include "Image.h"
#include "misc/optlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



void image__bithresholding
  P2 (( register image, bithresholding ),
      ( register image, laplace    ))



char *option[] = {
  "The bithresholdingolute of a short2-vector image.",
  "input:*:1:(image) name of the input short2 image",
  "output:*:1:(image) name of the output float image",
  "threshold:-t:1:128:(float) value of the thresholding",
  "help:-h:print help messages",
};

int main(argc,argv)
     int argc;
     char **argv;
{
  image src, del;
  char *input;
  char *output;
  long threshold;

  OPTION_SETUP( option );

  if ( argc == 1 ) optusage( 1 );
  if ( optspecified( "help" ) ) optmanual( 1 );

  input  = optvalue("input");
  output = optvalue("output");
  threshold = optvalueint( "threshold" );

  src = Image.createFromFilename( "SOURCE", input );
  del = Image.create( output );
  if ( src == 0 )
    {
      fprintf( stderr, "bithresholding:can't open file %s\n", input );
      exit( -1 );
    }
  if ( __TYPE( src ) != Short )
    {
      fprintf( stderr, "bithresholding:image is wrong %s\n", input );
      exit( -1 );
    }

  image__bithresholding( del, src, threshold );
  Image.save( del, output, "bithresholding" );

  Image.destroy( src );
  Image.destroy( del );

  return 0;
}



void image__bithresholding
  _P3 (( register image, bithresholding ),
       ( register image, laplace    ),
       ( long, threshold    ))
{
  register long x, y;

  Image.make( bithresholding, Short, __XSIZE( laplace ), __YSIZE( laplace ) );

  for ( y = 0; y < __YSIZE( laplace ); y++ )
    for ( x = 0; x < __XSIZE( laplace ); x++ )
      {
	long value = __PIXEL( laplace, x, y, short );
	long abs_value = abs( value );
	if ( abs_value > threshold ) abs_value = threshold;
	if ( value < 0 ) abs_value = -abs_value;
	__PIXEL( bithresholding, x, y, short ) = abs_value;
      }
}
