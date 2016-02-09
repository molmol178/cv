/*
 * tiff.c
 */



#include "FImage.h"
#include <stdio.h>



long FImageLoad_TIFF(self,filename)
     image self;
     char *filename;
{
  char buf[1024];

  if (filename == 0 || strlen(filename) == 0 ||
      strequal("-",filename) || strequal("stdin",filename))
    sprintf(buf,"tifftopnm");
  else
    sprintf(buf,"tifftopnm %s",filename);

  return FImageLoadPipe_PNM(self,buf);
}



long FImageSave_TIFF(self,filename,comment)
     image self;
     char *filename;
     char *comment;
{
  char buf[1024];

  if (filename == 0 || strlen(filename) == 0 ||
      strequal(filename,"-") || strequal(filename,"stdout"))
    sprintf(buf,"ppm2tiff");
  else
    sprintf(buf,"ppm2tiff %s",filename);

  return FImageSavePipe_PNM(self,buf);
}



long FImageMagic_TIFF(fp)
     FILE *fp;
{
  char magic_char = getc(fp);
  ungetc(magic_char,fp);
  return (magic_char == 0115 || magic_char == 0111);
}
