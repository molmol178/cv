/*
 * filename : ciladd.c
 * author   : Takahiro Sugiyama
 * date     : 1993/10/03
 */



#include "cilserver.h"
#include <stdio.h>



int main(argc,argv)
     int argc;
     char *argv[];
{
  cilRec *info;

  if ( argc == 1 )
    {
      fprintf( stderr, "Usage : ciladd <name>\n");
      exit( 1 );
    }

  info = cil_create( argv[1] );
  if ( info ) fprintf( stderr, "added %s\n", argv[1] );

  return 0;
}
