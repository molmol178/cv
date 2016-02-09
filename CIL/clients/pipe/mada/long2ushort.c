/*
 * Usage : long2ushort [input] [output]
 */



#include "Image.h"
#include <stdio.h>



int main(argc,argv)
     int argc;
     char **argv;
{
  image src,dst;
  char *input = "-";
  char *output = "-";

  src = Image.createFromFilename("SRC",input);
  dst = Image.createMake("DST",UShort,Image.xsize(src),Image.ysize(src));

  if (src == 0)
    {
      fprintf(stderr,"long2ushort:can't open file (%s)\n",input);
      exit(-1);
    }

  if (src->type == Long)
    {
      int num = Image.area(src);
      register int i;
      register long *sp;
      register ushort *dp;

      sp = Image.raster(src);
      dp = Image.raster(dst);
      for (i = 0; i < num; i++) *dp++ = *sp++;
      Image.save(dst,output,0);

      Image.destroy(src);
      Image.destroy(dst);
      
      return 0;
    }

  if (src->type == UShort)
    {
      Image.save(src,output,0);
      Image.destroy(src);
      Image.destroy(dst);
      return 0;
    }

  fprintf(stderr,"long2ushort:bad type (%s)\n",typename(src->type));

  Image.destroy(src);
  Image.destroy(dst);

  return -1;
}
