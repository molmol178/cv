/*
 * Usage : strength [input] [output]
 */



#include "Image.h"
#include "misc/optlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



void image__strength
  P2 (( register image, strength ),
      ( register image, sobel    ))



char *option[] = {
  "The absolute of a short2-vector image.",
  "input:*:1:(image) name of the input short2 image",
  "output:*:1:(image) name of the output float image",
  "help:-h:print help messages",
};

int main(argc,argv)
     int argc;
     char **argv;
{
  image src, del;
  char *input;
  char *output;

  OPTION_SETUP( option );

  if ( argc == 1 ) optusage( 1 );
  if ( optspecified( "help" ) ) optmanual( 1 );

  input  = optvalue("input");
  output = optvalue("output");

  src = Image.createFromFilename( "SOURCE", input );
  del = Image.create( output );
  if ( src == 0 )
    {
      fprintf( stderr, "strength:can't open file %s\n", input );
      exit( -1 );
    }
  if ( __TYPE( src ) != Short2 )
    {
      fprintf( stderr, "strength:image is wrong %s\n", input );
      exit( -1 );
    }

  image__strength( del, src );
  Image.save( del, output, "strength" );

  Image.destroy( src );
  Image.destroy( del );

  return 0;
}



void image__strength
  _P2 (( register image, strength ),
       ( register image, sobel    ))
{
  register long x, y;

  Image.make( strength, Float, __XSIZE( sobel ), __YSIZE( sobel ) );

  for ( y = 0; y < __YSIZE( sobel ); y++ )
    for ( x = 0; x < __XSIZE( sobel ); x++ )
      {
	double dx = __PIXEL0( sobel, x, y, short2 );
	double dy = __PIXEL1( sobel, x, y, short2 );
	__PIXEL( strength, x, y, float ) = sqrt( dx * dx + dy * dy );
      }
}
