/*
 * Usage : sobel [input] [output]
 */



#include "Image.h"
#include "Image/Funcs.h"
#include "misc/optlib.h"
#include <math.h>
#include <stdio.h>



char *option[] = {
  "The Sobel filtering.",
  "input:*:1:(image) name of the input image",
  "output:*:1:(image) name of the output short2 image",
  "help:-h:print help messages",
};

int main(argc,argv)
     int argc;
     char **argv;
{
  image source, sobel;
  char *input;
  char *output;

  OPTION_SETUP( option );

  if ( argc == 1 ) optusage( 1 );
  if ( optspecified( "help" ) ) optmanual( 1 );

  input  = optvalue("input");
  output = optvalue("output");

  source = Image.createFromFilename( "SOURCE", input );
  sobel  = Image.create( "SOBEL" );

  if ( source == 0 )
    {
      fprintf( stderr, "sobel:can't open file %s\n", input );
      exit( -1 );
    }

  image__sobel( sobel, source, 0 );
  Image.save( sobel, output, "sobel" );

  Image.destroy( source );
  Image.destroy( sobel );

  return 0;
}
