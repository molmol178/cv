/*
 * filename : ImageFile.h
 * author   : Takahiro Sugiyama
 * date     : 1993/05/15
 */



#ifndef _ImageFile_h
#define _ImageFile_h



#include "Image.h"
#include <stdio.h>



extern struct ImageFileRec {
  char *(*getLoadFormat) P ((void));
  char *(*getSaveFormat) P ((void));
  char *(*getComment)    P ((void));
  char *(*getHeader)     P ((void));
  long  (*getQuality)    P ((void));

  void (*setSaveFormat) P ((char *name));
  void (*setQuality)    P ((long quality));

  long (*load) P ((image self,char *filename));
  long (*save) P ((image self,char *filename,char *comment));

} ImageFile;



#endif //_ImageFile_h
