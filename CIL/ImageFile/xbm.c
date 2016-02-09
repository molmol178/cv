/*
 * xbm.c
 */



#include "FImage.h"
#include <stdio.h>



long FImageLoad_XBM(self,filename)
     image self;
     char *filename;
{
  char buf[1024];

  if (filename == 0 || strlen(filename) == 0 ||
      strequal("-",filename) || strequal("stdin",filename))
    sprintf(buf,"xbmtopnm");
  else
    sprintf(buf,"xbmtopbm %s",filename);

  fprintf( stderr, "loading ... \"%s\"\n", buf );

  return FImageLoadPipe_PNM( self, buf );
}



long FImageSave_XBM(self,filename,comment)
     image self;
     char *filename;
     char *comment;
{
  char buf[1024];

  if (filename == 0 || strlen(filename) == 0 ||
      strequal(filename,"-") || strequal(filename,"stdout"))
    sprintf(buf,"pnmtoxbm");
  else
    sprintf(buf,"pbmtoxbm > %s",filename);

  return FImageSavePipe_PNM(self,buf);
}



long FImageMagic_XBM(fp)
     FILE *fp;
{
  char buf[8];

  fread(buf,7,1,fp);
  rewind(fp);

  if (strncmp(buf,"#define",7) == 0)
    {
      fprintf( stderr, "This is x window bitmap file.\n");
      return 1;
    }
  return 0;
}
