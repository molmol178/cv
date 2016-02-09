/*
 * imagefillin.c
 */



#include <stdio.h>
#include "Image.h"
#include "misc/optlib.h"



static char *item[] = {
  "The binary image fillin regions.",
  "input:*:1:(image) name of the input binary image",
  "output:*:1:(image) name of the output binary image",
  "help:-h:print the help message",
};



int main(argc,argv)
     long argc;
     char *argv[];
{
  image src;
  image dest;
  long i;
  char *in = "-";
  char *out = "-";

  OPTION_SETUP( item );

  if ( optspecified( "help" ) ) optmanual( 1 );
  if ( argc == 1 ) optusage( 1 );

  in  = optvalue("input");
  out = optvalue("output");

  src = Image.createFromFilename( "INPUT", in );
  dest = Image.create( "OUTPUT" );

  if ( src == 0 )
    {
      fprintf(stderr,"error:imagefillin:can't read file (%s).\n",in);
      return -1;
    }

  if ( __TYPE( src ) != Bit1 )
    {
      fprintf(stderr,"error:imagefillin:wrong type (%d).(%s).",__TYPE(src),in);
      return -1;
    }

  image__fill_in(dest,src);

  Image.save(dest,out,"image fill in");

  Image.destroy(dest);
  Image.destroy(src);

  return 0;
}
