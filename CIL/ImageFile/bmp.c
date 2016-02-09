/*
 * bmp.c
 */



#include "FImage.h"
#include <stdio.h>



long FImageLoad_BMP(self,filename)
     image self;
     char *filename;
{
  char buf[1024];

  if (filename == 0 || strlen(filename) == 0 ||
      strequal("-",filename) || strequal("stdin",filename))
    sprintf(buf,"bmptopnm");
  else
    sprintf(buf,"bmptopnm %s",filename);

  return FImageLoadPipe_PNM(self,buf);
}



long FImageSave_BMP(self,filename,comment)
     image self;
     char *filename;
     char *comment;
{
  char buf[1024];

  if (filename == 0 || strlen(filename) == 0 ||
      strequal(filename,"-") || strequal(filename,"stdout"))
    sprintf(buf,"ppmtobmp");
  else
    sprintf(buf,"ppmtobmp > %s",filename);

  return FImageSavePipe_PNM(self,buf);
}



long FImageMagic_BMP(fp)
     FILE *fp;
{
  char magic_char = getc(fp);
  ungetc(magic_char,fp);
  return (magic_char == 'B' || magic_char == 'M');
}
