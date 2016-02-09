/*
 * Usage : touchar [input] [output]
 */



#include "Image.h"
#include <stdio.h>
#include <stdlib.h>



void image__to_uchar
  P2 (( image, dest ),
      ( image, src  ))



int main(argc,argv)
     int argc;
     char **argv;
{
  image src,del;
  char *input  = "-";
  char *output = "-";

  if ((argc == 2) && (strcmp(argv[1],"help") == 0))
    {
      fprintf(stderr,"Usage : touchar [input] [output]\n");
      exit(0);
    }

  if (argc > 1) input  = argv[1];
  if (argc > 2) output = argv[2];

  src = Image.createFromFilename(input,input);
  del = Image.create("DEL");
  if (src == 0)
    {
      fprintf(stderr,"touchar:can't open file %s\n",input);
      exit(-1);
    }

  image__to_uchar(del,src);
  Image.save(del,output,"");

  Image.destroy(src);
  Image.destroy(del);

  return 0;
}



void image__to_uchar
  _P2 (( image, dest ),
       ( image, src  ))
{
  register long n;
  register int i;
  register uchar *dp;
  register float *sp;
  double max, min, range;

  Image.make( dest, UChar, __XSIZE( src ), __YSIZE( src ) );
  n = __AREA( src );

  max = -100000000.0;
  min =  100000000.0;
  sp = __RASTER( src , float );
  for ( i = 0; i < n; i++, sp++ )
    {
      if ( max < *sp ) max = *sp; else
      if ( min > *sp ) min = *sp;
    }

  range = max - min;
  dp = __RASTER( dest, uchar );
  sp = __RASTER( src , float );
  for ( i = 0; i < n; i++, sp++, dp++ )
    *dp = 255 * ( *sp - min ) / range;
}
