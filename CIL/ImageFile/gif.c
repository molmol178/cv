/*
 * gif.c
 */



#include "FImage.h"
#include <stdio.h>
#include "../misc/strlib.h"



long FImageLoad_GIF(self,filename)
     image self;
     char *filename;
{
  char buf[1024];

  if (filename == 0 || strlen(filename) == 0 ||
      strequal("-",filename) || strequal("stdin",filename))
    sprintf(buf,"giftoppm");
  else
    sprintf(buf,"giftoppm %s",filename);

  return FImageLoadPipe_PNM(self,buf);
}



long FImageSave_GIF(self,filename,comment)
     image self;
     char *filename;
     char *comment;
{
  char buf[1024];
  long quality = FImageGetQuality();

  if (quality < 0 || quality > 100)
    quality = 100;

  if (filename == 0 || strlen(filename) == 0 ||
      strequal(filename,"-") || strequal(filename,"stdout"))
    sprintf(buf,"ppmtogif");
  else
    sprintf(buf,"ppmtogif > %s",filename);

  return FImageSavePipe_PNM(self,buf);
}



long FImageMagic_GIF(fp)
     FILE *fp;
{
  char magic_char = getc(fp);
  ungetc(magic_char,fp);
  return magic_char == 'G';
}
