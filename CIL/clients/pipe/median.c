/*
 * Usage : median [input] [output] [times] [median]
 */



#include "Image.h"
#include "Image/Funcs.h"
#include <stdio.h>
#include <string.h>



int main( argc, argv )
     int argc;
     char **argv;
{
  image original, source, median;
  char *input  = "-";
  char *output = "-";
  long n   = 4;
  long num = 1;
  int i;

  if (( argc > 1 ) && (( strcmp( argv[ 1 ], "help" ) == 0 ) ||
		       ( strcmp( argv[ 1 ], "-h"   ) == 0 )))
    {
      fprintf( stderr, "Usage : median [input] [output] [times] [median]\n" );
      exit( 1 );
    }

  i = 0;
  if ( argc > ++i ) input  = argv[ i ];
  if ( argc > ++i ) output = argv[ i ];
  if ( argc > ++i ) if ( strcmp( argv[ i ], "-" ) != 0 ) num = atol( argv[ i ] );
  if ( argc > ++i ) if ( strcmp( argv[ i ], "-" ) != 0 ) n   = atol( argv[ i ] );

  if ( n < 0 || n > 8 )
    {
      fprintf( stderr, "median:bad value ... median = %d\n", n );
      fprintf( stderr, "       median is [0..8]\n" );
      exit( -1 );
    }

  original = Image.createFromFilename( "ORIGINAL", input );
  if ( original == 0 )
    {
      fprintf( stderr, "median:can't open file %s\n", input );
      exit( -1 );
    }

  source = Image.createFromImage( "SOURCE", original );
  median = Image.create( "MEDIAN" );
  Image.destroy( original );

  while ( num > 0 )
    {
      image__median( median, source, n );
      if ( num > 1 ) Image.swap( source, median );
      num--;
    }

  Image.save( median, output, "median" );

  Image.destroy( source );
  Image.destroy( median );

  return 0;
}
