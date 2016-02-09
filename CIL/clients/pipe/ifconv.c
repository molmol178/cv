/*
 * ifconv <format> [input] [output]
 */



#include "Image.h"
#include "ImageFile.h"
#include <stdio.h>



int main( argc, argv )
     int   argc;
     char *argv[];
{
  image img;
  char *format = "C2D";
  char *input  = "-";
  char *output = "-";
  int i;

  if ( argc == 1 )
    {
      fprintf( stderr, "ifconv <format> [input] [output]\n" );
      fprintf( stderr, "format: C2D(rw), PNM(rw), J4(rw), TIFF(rw)\n" );
      fprintf( stderr, "        XWD(rw), XBM(rw), JPEG(rw), GIF(rw)\n" );
      fprintf( stderr, "        PS(rw), DIB(r)\n" );
      exit( 1 );
    }

  i = 0;
  format = argv[ ++i ];
  if ( argc > ++i ) input  = argv[ i ];
  if ( argc > ++i ) output = argv[ i ];

  img = Image.createFromFilename( "ORIGINAL", input );
  if ( img == 0 )
    {
      fprintf( stderr, "ifconv:can't open file %s\n", input );
      exit( -1 );
    }

  ImageFile.setSaveFormat( format );

  if ( Image.save( img,output, ImageFile.getComment() ) == 0 )
    {
      fprintf( stderr, "can't write/convert file.\"%s\" to the format(%s)\n", output, format );
      exit( -1 );
    }

  Image.destroy( img );

  return 0;
}
