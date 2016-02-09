/*
 * filename : ImageFile.h
 * author   : Takahiro Sugiyama
 * date     : 1993/09/21
 */



#include "ImageFile/FImage.h"
#include "ImageFile.h"



struct ImageFileRec ImageFile = {
  FImageGetFormat,
  FImageGetSaveFormat,
  FImageGetComment,
  FImageGetHeader,
  FImageGetQuality,

  FImageSetSaveFormat,
  FImageSetQuality,

  FImageLoad,
  FImageSave

};
