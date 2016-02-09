/*
 * Usage : downtone [tone] [input] [output]
 */



#include "Image.h"
#include "Image/Funcs.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>



void image__downtone P ((image dest,image src,long th));



void main(argc,argv)
     int argc;
     char **argv;
{
  image src,del;
  char *input  = "-";
  char *output = "-";
  long th = 16;

  if ((argc == 2) && (strcmp(argv[1],"help") == 0))
    {
      fprintf(stderr,"Usage : downtone [tone] [input] [output]\n");
      exit(0);
    }

  if (argc > 1) if ( strcmp( argv[ 1 ], "-" ) != 0 ) th = atol(argv[1]);
  if (argc > 2) input  = argv[2];
  if (argc > 3) output = argv[3];

  src = Image.createFromFilename(input,input);
  del = Image.create("DEL");
  if (src == 0)
    {
      fprintf(stderr,"downtone:can't open file %s\n",input);
      exit(-1);
    }
  if (Image.type(src) != UChar)
    {
      fprintf(stderr,"downtone:image is wrong %s\n",input);
      exit(-1);
    }

  image__downtone(del,src,th);
  Image.save(del,output,"downtone");

  Image.destroy(src);
  Image.destroy(del);
}



void image__downtone(dest,src,th)
     image dest,src;
     long th;
{
  long max,min,range;
  long n;
  register int i;
  register uchar *dp,*sp;

  Image.make(dest,UChar,Image.xsize(src),Image.ysize(src));
  n  = Image.area(src);

  sp = Image.raster(src);
  for (i = 0; i < n; i++,sp++)
    if (*sp > max) max = *sp;
    else
    if (*sp < min) min = *sp;
  range = max - min + 1;

  dp = Image.raster(dest);
  sp = Image.raster(src);
  for (i = 0; i < n; i++,sp++,dp++)
    *dp = th * (*sp - min) / range;
}
