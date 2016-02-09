/*
 * viff.c
 */



#include "FImage.h"
#include <stdio.h>



long FImageLoad_VIFF(self,filename)
     image self;
     char *filename;
{
  char buf[1024];

  if (filename == 0 || strlen(filename) == 0 ||
      strequal("-",filename) || strequal("stdin",filename))
    sprintf(buf,"viff2rast -i - -o - | rasttopnm");
  else
    sprintf(buf,"viff2rast -i %s -o - | rasttopnm",filename);

  return FImageLoadPipe_PNM(self,buf);
}



long FImageSave_VIFF(self,filename,comment)
     image self;
     char *filename;
     char *comment;
{
  char buf[1024];
  char tmp[1024];
  char cat[1024];

  sprintf(tmp,"/tmp/#saveviff%d.tiff",getpid());
  sprintf(cat,"/tmp/#saveviff%d.cat",getpid());

  sprintf(buf,"pnmtotiff > %s",tmp);
  FImageSavePipe_PNM(self,buf);

  if (filename == 0 || strlen(filename) == 0 ||
      strequal(filename,"-") || strequal(filename,"stdout"))
    {
      sprintf(buf,"tiff2viff -i %s -o %s",tmp,cat);
      system(buf);
      sprintf(buf,"cat %s",cat);
      system(buf);
      sprintf(buf,"rm -f %s %s",tmp,cat);
      system(buf);
    }
  else
    {
      sprintf(buf,"tiff2viff -i %s -o %s",tmp,filename);
      system(buf);
      sprintf(buf,"rm -f %s",tmp);
      system(buf);
    }

  return 1;
}



long FImageMagic_VIFF(fp)
     FILE *fp;
{
  unsigned char magic_char = getc(fp);
  ungetc(magic_char,fp);
  return (magic_char == 0xAB);
}
