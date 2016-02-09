/*
 * filename : FImage.h
 * author   : Takahiro Sugiyama
 * date     : ?
 * describe : Image loader/saver in some formats
 */



#ifndef __FImage_h
#define __FImage_h



#include "../Image.h"
#include "../misc/strlib.h"



/*
 * IMAGE DIRECTORY ENVIRONMENT NAME
 */
#define IMAGE_DIR     "IMAGE_DIR"
#define CIL_IMAGE_DIR "CIL_IMAGE_DIR"



void  FImageSetSaveFormat(/* char *format */);
void  FImageSetQuality(/* long quarity */);
void  FImageSetFormat(/* char *format */);
void  FImageSetComment(/* char *comment */);
void  FImageSetHeader(/* char *header */);
long  FImageSetBit1Type(/* long type */);

char *FImageGetSaveFormat(/* void */);
long  FImageGetQuality(/* void */);
char *FImageGetFormat(/* void */);
char *FImageGetComment(/* void */);
char *FImageGetHeader(/* void */);
long  FImageGetBitType1(/* void */);
char *FImageGetLoadDirectory(/* void */);

long FImageLoad(/* image self, char *filename */);
long FImageSave(/* image self, char *filename, char *comment */);



/*
 * PNM format
 */
#define FORMAT_PNM "pnm"
long FImageLoad_PNM(/* image self, char *filename */);
long FImageSave_PNM(/* image self, char *filename,char *comment */);
long FImageMagic_PNM(/* FILE *fp */);

long FImageLoadPipe__PNM(/* image self,char *pipename */);
long FImageSavePipe__PNM(/* image self,char *pipename */);

/*
 * C2D format
 */
#define FORMAT_C2D "c2d"
long FImageLoad_C2D(/* image self, char *filename */);
long FImageSave_C2D(/* image self, char *filename,char *comment */);
long FImageMagic_C2D(/* FILE *fp */);

/*
 * J4 format
 */
#define FORMAT_J4 "j4"
long FImageLoad_J4(/* image self, char *filename */);
long FImageSave_J4(/* image self, char *filename,char *comment */);
long FImageMagic_J4(/* FILE *fp */);

/*
 * TIFF format
 */
#define FORMAT_TIFF "tiff"
long FImageLoad_TIFF(/* image self, char *filename */);
long FImageSave_TIFF(/* image self, char *filename,char *comment */);
long FImageMagic_TIFF(/* FILE *fp */);

/*
 * GIF format
 */
#define FORMAT_GIF "gif"
long FImageLoad_GIF(/* image self, char *filename */);
long FImageSave_GIF(/* image self, char *filename,char *comment */);
long FImageMagic_GIF(/* FILE *fp */);

/*
 * S3D format
 */
#define FORMAT_S3D "s3d"
long FImageLoad_S3D(/* image self, char *filename */);
long FImageSave_S3D(/* image self, char *filename,char *comment */);
long FImageMagic_S3D(/* FILE *fp */);

/*
 * JPEG format
 */
#define FORMAT_JPEG "jpeg"
long FImageLoad_JPEG(/* image self, char *filename */);
long FImageSave_JPEG(/* image self, char *filename,char *comment */);
long FImageMagic_JPEG(/* FILE *fp */);

/*
 * XwBitmap format
 */
#define FORMAT_XwBitmap "XwBitmap"
long FImageLoad_XwBitmap(/* image self, char *filename */);
long FImageSave_XwBitmap(/* image self, char *filename,char *comment */);
long FImageMagic_XwBitmap(/* FILE *fp */);

/*
 * PS format
 */
#define FORMAT_PS "ps"
long FImageLoad_PS(/* image self, char *filename */);
long FImageSave_PS(/* image self, char *filename,char *comment */);
long FImageMagic_PS(/* FILE *fp */);

/*
 * XBM format
 */
#define FORMAT_XBM "xbm"
long FImageLoad_XBM(/* image self, char *filename */);
long FImageSave_XBM(/* image self, char *filename,char *comment */);
long FImageMagic_XBM(/* FILE *fp */);

/*
 * XWD format
 */
#define FORMAT_XWD "xwd"
long FImageLoad_XWD(/* image self, char *filename */);
long FImageSave_XWD(/* image self, char *filename,char *comment */);
long FImageMagic_XWD(/* FILE *fp */);

/*
 * VIFF format
 */
#define FORMAT_VIFF "viff"
long FImageLoad_VIFF(/* image self, char *filename */);
long FImageSave_VIFF(/* image self, char *filename,char *comment */);
long FImageMagic_VIFF(/* FILE *fp */);



/*
 * DIB format
 */ 
#define FORMAT_DIB "dib"
long FImageLoad_DIB(/* image self, char *filename */);
long FImageSave_DIB(/* image self, char *filename,char *comment */);
long FImageMagic_DIB(/* FILE *fp */);

/*
 * BMP format
 */ 
#define FORMAT_BMP "bmp"
long FImageLoad_BMP(/* image self, char *filename */);
long FImageSave_BMP(/* image self, char *filename,char *comment */);
long FImageMagic_BMP(/* FILE *fp */);



#endif // __FImage_h
