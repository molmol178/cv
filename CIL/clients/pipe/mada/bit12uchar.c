/*
 * Usage : touchar [input] [output]
 */



#include "Image.h"
#include <stdio.h>
#include <stdlib.h>



void image__to_uchar
  P4 (( image, dest ),
      ( image, src  ),
      ( long , low  ),
      ( long , high ))



int main(argc,argv)
     int argc;
     char **argv;
{
  image src,del;
  char *input  = "-";
  char *output = "-";
  long low = 0, high = 255;

  if ((argc == 2) && (strcmp(argv[1],"help") == 0))
    {
      fprintf(stderr,"Usage : bit1touchar [input] [output] [low] [high]\n");
      exit(0);
    }

  if (argc > 1) input  = argv[1];
  if (argc > 2) output = argv[2];
  if (argc > 3) low    = atol(argv[3]);
  if (argc > 3) high   = atol(argv[4]);

  src = Image.createFromFilename(input,input);
  del = Image.create("DEL");
  if (src == 0)
    {
      fprintf(stderr,"bit1touchar:can't open file %s\n",input);
      exit(-1);
    }

  image__to_uchar(del,src,low,high);
  Image.save(del,output,"");

  Image.destroy(src);
  Image.destroy(del);

  return 0;
}



void image__to_uchar
  _P4 (( image, dest ),
       ( image, src  ),
       ( long , low  ),
       ( long , high ))
{
  register long n;
  register int i;
  register uchar *dp;
  register bit1  *sp;

  Image.make( dest, UChar, __XSIZE( src ), __YSIZE( src ) );
  n = __AREA( src );

  dp = __RASTER( dest, uchar );
  sp = __RASTER( src , float );
  for ( i = 0; i < n; i++, sp++, dp++ )
    *dp = ( *sp ) ? high : low;
}

