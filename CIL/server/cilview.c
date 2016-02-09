/*
 * filename : cilview.c
 * author   : Takahiro Sugiyama
 * date     : 1993/10/01
 */



#include "Image.h"
#include <stdio.h>



int main(argc,argv)
     int argc;
     char **argv;
{
  image share;
  char *input = "-";

  if ( ( argc == 2 ) && ( strcmp( "-h", argv[ 1 ] ) == 0 ) )
    {
      fprintf(stderr,"Usage : cilview [input]\n");
      return 1;
    }

  if ( argc > 1 ) input = argv[1];

  share = Image.createFromFilename(input,input);
  if (share == 0)
    {
      fprintf(stderr,"cilview:can't open (%s)\n",input);
      return -1;
    }
  Image.display(share,0);

  return 0;
}
