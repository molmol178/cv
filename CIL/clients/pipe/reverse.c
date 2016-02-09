/*
 * Usage : reverse [input] [output]
 */



#include "Image.h"
#include "Image/Funcs.h"
#include <math.h>
#include <stdio.h>



int main( argc, argv )
     int    argc;
     char **argv;
{
  image source, reverse;
  char *input  = "-";
  char *output = "-";
  int i;

  if (( argc == 2 ) && (( strcmp( argv[ 1 ], "help" ) == 0 ) ||
			( strcmp( argv[ 1 ], "-h"   ) == 0 )))
    {
      fprintf( stderr, "Usage : reverse [input] [output]\n" );
      exit( 1 );
    }

  i = 0;
  if ( argc > ++i ) input  = argv[ i ];
  if ( argc > ++i ) output = argv[ i ];

  source  = Image.createFromFilename( "SOURCE", input );
  reverse = Image.create( "REVERSE" );
  if ( source == 0 )
    {
      fprintf( stderr, "reverse:can't open file %s\n", input );
      exit( -1 );
    }

  image__reverse( reverse, source );
  Image.save( reverse, output, "reverse" );

  Image.destroy( source );
  Image.destroy( reverse );

  return 0;
}
