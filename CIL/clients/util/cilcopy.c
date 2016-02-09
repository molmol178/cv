/*
 * filename : cilcopy.c
 * author   : Takahiro Sugiyama
 * date     : Sunday, February 27 1994
 * describe : cil image copy in any format.
 */



#include "Image.h"
#include "ImageFile.h"
#include "misc/optlib.h"
#include <stdio.h>



static char *option[] = {
  "input:*:1:stdin:(image) name of the input image",
  "output:*:1:stdout:(image) name of the output image",
  "help:-h:print this messages",
  "format:-f:1:pnm:(format) name of the ouput format",
  "The image formats are followings...",
  "    c2d(rw), pnm(rw), j4(rw), tiff(rw), ps(rw),",
  "    xwd(rw), xbm(rw), jpeg(rw), gif(rw), dib(r)",
};



int main( argc, argv )
     int   argc;
     char *argv[];
{
  image img;
  char *format;
  char *input;
  char *output;

  OPTION_SETUP( option );

  if ( optspecified( "help" ) ) optmanual( 1 );
  if ( argc == 1 ) optusage( 1 );

  format = optvalue( "format" );
  input  = optvalue( "input" );
  output = optvalue( "output" );

  img = Image.createFromFilename( "ORIGINAL", input );
  if ( img == 0 )
    {
      fprintf( stderr, "%s:can't open file %s\n", optcommand, input );
      exit( -1 );
    }

  ImageFile.setSaveFormat( format );

  if ( Image.save( img,output, ImageFile.getComment() ) == 0 )
    {
      fprintf( stderr, "%s:can't write/convert file.\"%s\" in format(%s)\n",
	       optcommand, output, format );
      exit( -1 );
    }

  Image.destroy( img );

  return 0;
}
