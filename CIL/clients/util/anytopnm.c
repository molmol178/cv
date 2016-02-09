/*
 * Usage : anytopnm [input] [output]
 */



#include "Image.h"
#include "ImageFile.h"
#include "Image/Funcs.h"
#include "misc/optlib.h"
#include <stdio.h>



char *option[] = {
  "The Any to PNM",
  "input:*:1:(image) name of the input image",
  "output:*:1:(image) name of the output pnm image",
  "help:-h:print help messages",
};

int main(argc,argv)
     int argc;
     char **argv;
{
  image source, output;
  char *input;
  char *output_name;

  OPTION_SETUP( option );

  if ( argc == 1 ) optusage( 1 );
  if ( optspecified( "help" ) ) optmanual( 1 );

  input  = optvalue("input");
  output_name = optvalue("output");

  source = Image.createFromFilename( "SOURCE", input );
  output  = Image.create( "PNM" );

  if ( source == 0 )
    {
      fprintf( stderr, "output:can't open file %s\n", input );
      exit( -1 );
    }

  image__any_to_pnm(output, source);
  ImageFile.setSaveFormat("pnm");
  Image.save(output, output_name, "pnm");

  Image.destroy( source );
  Image.destroy( output );

  return 0;
}
