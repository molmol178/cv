/*
 * Usage : laplacian8 [input] [output]
 */



#include "Image.h"
#include "Image/Funcs.h"
#include "misc/optlib.h"
#include <math.h>
#include <stdio.h>



char *option[] = {
  "The Laplacian8 filtering.",
  "input:*:1:(image) name of the input image",
  "output:*:1:(image) name of the output short image",
  "help:-h:print help messages",
};

int main(argc,argv)
     int argc;
     char **argv;
{
  image source, laplacian8;
  char *input;
  char *output;

  OPTION_SETUP( option );

  if ( argc == 1 ) optusage( 1 );
  if ( optspecified( "help" ) ) optmanual( 1 );

  input  = optvalue("input");
  output = optvalue("output");

  source = Image.createFromFilename( "SOURCE", input );
  laplacian8  = Image.create( "LAPLACIAN8" );

  if ( source == 0 )
    {
      fprintf( stderr, "laplacian8:can't open file %s\n", input );
      exit( -1 );
    }

  image__laplacian8( laplacian8, source );
  Image.save( laplacian8, output, "laplacian8" );

  Image.destroy( source );
  Image.destroy( laplacian8 );

  return 0;
}
