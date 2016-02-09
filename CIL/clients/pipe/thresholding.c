/*
 * Usage : thresholding [input] [output] [threshold]
 */



#include "Image.h"
#include "Image/Funcs.h"
#include <stdio.h>
#include <stdlib.h>



int main( argc, argv )
     int    argc;
     char **argv;
{
  image img_in, img_out;
  char *input  = "-";
  char *output = "-";
  double th = 128;
  int i;

  if (( argc > 1 ) && (( strcmp( argv[ 1 ], "help" ) == 0 ) ||
		       ( strcmp( argv[ 1 ], "-h"   ) == 0 )))
    {
      fprintf( stderr, "Usage : thresholding [input] [output] [threshold]\n" );
      exit( 1 );
    }

  i = 0;
  if ( argc > ++i ) input  = argv[ i ];
  if ( argc > ++i ) output = argv[ i ];
  if ( argc > ++i ) if ( strcmp( argv[ i ], "-" ) != 0 ) th = atof( argv[ i ] );

  img_in  = Image.createFromFilename( "SOURCE" ,input);
  img_out = Image.create( "THRESHOLD" );
  if ( img_in == 0 )
    {
      fprintf( stderr, "thresholding:can't open file %s\n", input );
      exit( -1 );
    }

  image__thresholding( img_out, img_in, th );

  Image.save( img_out, output, "thresholding" );

  Image.destroy( img_out );
  Image.destroy( img_in  );

  return 0;
}
