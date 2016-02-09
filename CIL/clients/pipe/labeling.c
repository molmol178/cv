/*
 * Usage : labeling [input] [output]
 */



#include "Image.h"
#include "Image/Funcs.h"
#include <stdio.h>



int main( argc, argv )
     int    argc;
     char **argv;
{
  image src, dst;
  char *output = "-";
  char *input  = "-";
  int i;

  if (( argc > 1 ) && (( strcmp( argv[ 1 ], "help" ) == 0 ) ||
		       ( strcmp( argv[ 1 ], "-h"   ) == 0 )))
    {
      fprintf( stderr, "Usage : labeling [input] [output]\n" );
      exit( 1 );
    }

  i = 0;
  if ( argc > ++i ) input  = argv[ i ];
  if ( argc > ++i ) output = argv[ i ];

  src = Image.createFromFilename( "SOURCE", input );
  dst = Image.create( "LABELING" );
  if ( src == 0 )
    {
      fprintf( stderr, "labeling:can't open file %s\n", input );
      exit( -1 );
    }

  image__labeling( dst, src );
  Image.save( dst, output, "labeling" );

  Image.destroy( src );
  Image.destroy( dst );

  return 0;
}
