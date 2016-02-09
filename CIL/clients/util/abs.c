/*
 * Usage : img_abs [input] [output]
 */



#include "Image.h"
#include "misc/optlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



void image__img_abs
  P2 (( register image, img_abs ),
      ( register image, laplace    ))



char *option[] = {
  "The img_absolute of a short2-vector image.",
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
      fprintf( stderr, "img_abs:can't open file %s\n", input );
      exit( -1 );
    }
  if ( __TYPE( src ) != Short )
    {
      fprintf( stderr, "img_abs:image is wrong %s\n", input );
      exit( -1 );
    }

  image__img_abs( del, src );
  Image.save( del, output, "img_abs" );

  Image.destroy( src );
  Image.destroy( del );

  return 0;
}



void image__img_abs
  _P2 (( register image, img_abs ),
       ( register image, laplace    ))
{
  register long x, y;

  Image.make( img_abs, Short, __XSIZE( laplace ), __YSIZE( laplace ) );

  for ( y = 0; y < __YSIZE( laplace ); y++ )
    for ( x = 0; x < __XSIZE( laplace ); x++ )
      __PIXEL( img_abs, x, y, short ) = abs(__PIXEL( laplace, x, y, short ));
}
