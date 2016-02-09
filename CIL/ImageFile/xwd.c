/*
 * xwd.c
 */



#include "FImage.h"
#include <stdio.h>



long FImageLoad_XWD(self,filename)
     image self;
     char *filename;
{
  char buf[1024];

  if (filename == 0 || strlen(filename) == 0 ||
      strequal("-",filename) || strequal("stdin",filename))
    sprintf(buf,"xwdtopnm");
  else
    sprintf(buf,"xwdtopnm %s",filename);

  return FImageLoadPipe_PNM(self,buf);
}



long FImageSave_XWD(self,filename,comment)
     image self;
     char *filename;
     char *comment;
{
  char buf[1024];

  if (filename == 0 || strlen(filename) == 0 ||
      strequal(filename,"-") || strequal(filename,"stdout"))
    sprintf(buf,"pnmtoxwd");
  else
    sprintf(buf,"pnmtoxwd > %s",filename);

  return FImageSavePipe_PNM(self,buf);
}



long FImageMagic_XWD(fp)
     FILE *fp;
{
  long key;

  fread(&key,4,1,fp);
  rewind(fp);

  return (( key & 0xffffff00 ) == 0 );
}
