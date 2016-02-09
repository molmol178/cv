/*
 * tiff.c
 */



#include "FImage.h"
#include <stdio.h>
#include "../misc/strlib.h"



long FImageLoad_JPEG(self,filename)
     image self;
     char *filename;
{
  char buf[1024];

  if (filename == 0 || strlen(filename) == 0 ||
      strequal("-",filename) || strequal("stdin",filename))
    sprintf(buf,"djpeg -P");
  else
    sprintf(buf,"djpeg -P %s",filename);

  return FImageLoadPipe_PNM(self,buf);
}



long FImageSave_JPEG(self,filename,comment)
     image self;
     char *filename;
     char *comment;
{
  char buf[1024];
  long quality = FImageGetQuality();

  if ((quality < 0)||(quality > 100))
      quality = 100;

  if (filename == 0 || strlen(filename) == 0 ||
      strequal(filename,"-") || strequal(filename,"stdout"))
    sprintf(buf,"cjpeg -Q %d -o",quality);
  else
    sprintf(buf,"cjpeg -Q %d > %s",quality,filename);

  return FImageSavePipe_PNM(self,buf);
}



long FImageMagic_JPEG(fp)
     FILE *fp;
{
  return 1;
}
