/*
 * filename : ImageCG.h
 * author   : Takahiro Sugiyama
 * date     : 1993/09/26
 */



#ifndef __ImageCG_h
#define __ImageCG_h



#include "../misc/comacros.h"
#include "../Image.h"
#include <X11/Xlib.h>


typedef struct {
  short x;
  short y;
  char *pixel;
} ImgPoint;



typedef struct {
  int x1, y1;
  int x2, y2;
  char *pixel;
} ImgSegment, ImgBox;



typedef struct {
  int x, y;
  unsigned int width, height;
  char *pixel;
} ImgRectangle;



typedef struct {
  short x, y;
  unsigned short width, height;
  short angle1, angle2;
  char *pixel;
} ImgArc;



typedef void (*ImageDrawPoint_type)
     P4 (( image , img   ),
	 ( long  , x     ),
	 ( long  , y     ),
	 ( char *, pixel ))

extern ImageDrawPoint_type ImageDrawPointList[];

#define ImageDrawPoint( img, x, y, pixel ) \
  ImageDrawPointList[(img)->type] ( img, x, y, pixel )



void ImageDrawPoints
  P4 (( image     , img  ),
      ( ImgPoint *, list ),
      ( long      , num  ),
      ( long      , mode ))



void ImageDrawLine
  P6 (( image , img   ),
      ( long  , x1    ),
      ( long  , y1    ),
      ( long  , x2    ),
      ( long  , y2    ),
      ( char *, pixel ))


void ImageDrawLines
  P4 (( image     , img  ),
      ( ImgPoint *, list ),
      ( long      , num  ),
      ( int       , mode ))


void ImageDrawSegments
  P3 (( image       , img  ),
      ( ImgSegment *, list ),
      ( long        , num  ))


void ImageDrawBox
  P6 (( image , img   ),
      ( long  , x1    ),
      ( long  , y1    ),
      ( long  , x2    ),
      ( long  , y2    ),
      ( char *, pixel ))


void ImageDrawBoxes
  P3 (( image    , img  ),
      ( ImgBox  *, list ),
      ( long     , num  ))



void ImageDrawRectangle
  P6 (( image       , img    ),
      ( int         , x      ),
      ( int         , y      ),
      ( unsigned int, width  ),
      ( unsigned int, height ),
      ( char *      , pixel  ))


void ImageDrawRectangles
  P3 (( image         , img  ),
      ( ImgRectangle *, list ),
      ( long          , num  ))



void ImageDrawArc
  P8 (( image       , img    ),
      ( int         , x      ),
      ( int         , y      ),
      ( unsigned int, width  ),
      ( unsigned int, height ),
      ( int         , angle1 ),
      ( int         , angle2 ),
      ( char *      , pixel  ))



void ImageDrawArcs
  P3 (( image   , img  ),
      ( ImgArc *, list ),
      ( long    , num  ))



void ImageFillRectangle
  P6 (( image       , img    ),
      ( int         , x      ),
      ( int         , y      ),
      ( unsigned int, width  ),
      ( unsigned int, height ),
      ( char *      , pixel  ))



void ImageFillRectangles
  P3 (( image         , img  ),
      ( ImgRectangle *, list ),
      ( long          , num  ))



void ImageFillArc
  P8 (( image       , img    ),
      ( int         , x      ),
      ( int         , y      ),
      ( unsigned int, width  ),
      ( unsigned int, height ),
      ( int         , angle1 ),
      ( int         , angle2 ),
      ( char *      , pixel  ))



void ImageFillArcs
  P3 (( image   , img  ),
      ( ImgArc *, list ),
      ( long    , num  ))



void ImageFillPolygon
  P6 (( image     , img   ),
      ( ImgPoint *, list  ),
      ( long      , num   ),
      ( long      , shape ),
      ( long      , mode  ),
      ( char *    , pixel ))




void ImageSetStringFont
  P1 (( char *, name ))

void ImageSetStringFont16
  P1 (( char *, name ))

void ImageDrawString
  P6 (( image , img    ),
      ( long  , x      ),
      ( long  , y      ),
      ( char *, str    ),
      ( long  , length ),
      ( char *, pixel  ))

image ImageCreateString
  P2 (( char *, str    ),
      ( long  , length ))

void ImageDrawString16
  P6 (( image    , img    ),
      ( long     , x      ),
      ( long     , y      ),
      ( XChar2b *, str    ),
      ( long     , length ),
      ( char *   , pixel  ))


void ImageDrawImageString
  P7 (( image , img       ),
      ( long  , x         ),
      ( long  , y         ),
      ( char *, str       ),
      ( long  , length    ),
      ( char *, forepixel ),
      ( char *, backpixel ))

void ImageDrawImageString16
  P7 (( image    , img       ),
      ( long     , x         ),
      ( long     , y         ),
      ( XChar2b *, str       ),
      ( long     , length    ),
      ( char *   , forepixel ),
      ( char *   , backpixel ))

/*------------------------------------------------------------------------*
 * 指定した位置に点画像を表示する。
 *------------------------------------------------------------------------*/
void ImageDrawPointImage
  P6 (( image , img    ),
      ( int   , ox     ),
      ( int   , oy     ),
      ( image , points ),
      ( char *, pos    ),
      ( char *, pixel  ))

/*------------------------------------------------------------------------*
 * 指定した位置にテキストを表示する。
 *------------------------------------------------------------------------*/
void ImagePutString
  P6 (( image , img   ),
      ( long  , ox    ),
      ( long  , oy    ),
      ( char *, pos   ),
      ( char *, str   ),
      ( char *, pixel ))

/*------------------------------------------------------------------------*
 * 画像を左に９０度回転する。
 *------------------------------------------------------------------------*/
void ImageRotateLeft
  P1 ((image, self))



void ImageDrawPoint_PackedBit1
  P4 (( image             , img        ),
      ( long              , x          ),
      ( long              , y          ),
      ( char * /* long */ , pixel_bit1 ))

void ImageDrawPoint_Bit1
  P4 (( image             , img        ),
      ( long              , x          ),
      ( long              , y          ),
      ( char * /* long */ , pixel_bit1 ))

void ImageDrawPoint_Bit4
  P4 (( image             , img        ),
      ( long              , x          ),
      ( long              , y          ),
      ( char * /* long */ , pixel_bit4 ))

void ImageDrawPoint_Char
  P4 (( image             , img        ),
      ( long              , x          ),
      ( long              , y          ),
      ( char * /* long */ , pixel_char ))

void ImageDrawPoint_Short
  P4 (( image             , img         ),
      ( long              , x           ),
      ( long              , y           ),
      ( char * /* long */ , pixel_short ))

void ImageDrawPoint_Int
  P4 (( image             , img       ),
      ( long              , x         ),
      ( long              , y         ),
      ( char * /* long */ , pixel_int ))

void ImageDrawPoint_Long
  P4 (( image             , img        ),
      ( long              , x          ),
      ( long              , y          ),
      ( char * /* long */ , pixel_long ))

void ImageDrawPoint_UChar
  P4 (( image             , img         ),
      ( long              , x           ),
      ( long              , y           ),
      ( char * /* ulong */, pixel_uchar ))

void ImageDrawPoint_UShort
  P4 (( image             , img          ),
      ( long              , x            ),
      ( long              , y            ),
      ( char * /* ulong */, pixel_ushort ))

void ImageDrawPoint_UInt
  P4 (( image             , img        ),
      ( long              , x          ),
      ( long              , y          ),
      ( char * /* ulong */, pixel_uint ))

void ImageDrawPoint_ULong
  P4 (( image             , img         ),
      ( long              , x           ),
      ( long              , y           ),
      ( char * /* ulong */, pixel_ulong ))

void ImageDrawPoint_Float
  P4 (( image               , img             ),
      ( long                , x               ),
      ( long                , y               ),
      ( char * /* float * */, pixel_float_ptr ))

void ImageDrawPoint_Double
  P4 (( image                , img              ),
      ( long                 , x                ),
      ( long                 , y                ),
      ( char * /* double * */, pixel_double_ptr ))

void ImageDrawPoint_Char2
  P4 (( image               , img             ),
      ( long                , x               ),
      ( long                , y               ),
      ( char * /* char2 * */, pixel_char2_ptr ))

void ImageDrawPoint_Short2
  P4 (( image                , img              ),
      ( long                 , x                ),
      ( long                 , y                ),
      ( char * /* short2 * */, pixel_short2_ptr ))

void ImageDrawPoint_Int2
  P4 (( image              , img            ),
      ( long               , x              ),
      ( long               , y              ),
      ( char * /* int2 * */, pixel_int2_ptr ))

void ImageDrawPoint_Long2
  P4 (( image               , img             ),
      ( long                , x               ),
      ( long                , y               ),
      ( char * /* long2 * */, pixel_long2_ptr ))

void ImageDrawPoint_UChar2
  P4 (( image                , img              ),
      ( long                 , x                ),
      ( long                 , y                ),
      ( char * /* uchar2 * */, pixel_uchar2_ptr ))

void ImageDrawPoint_UShort2
  P4 (( image                 , img               ),
      ( long                  , x                 ),
      ( long                  , y                 ),
      ( char * /* ushort2 * */, pixel_ushort2_ptr ))

void ImageDrawPoint_UInt2
  P4 (( image               , img             ),
      ( long                , x               ),
      ( long                , y               ),
      ( char * /* uint2 * */, pixel_uint2_ptr ))

void ImageDrawPoint_ULong2
  P4 (( image                , img              ),
      ( long                 , x                ),
      ( long                 , y                ),
      ( char * /* ulong2 * */, pixel_ulong2_ptr ))

void ImageDrawPoint_Float2
  P4 (( image                , img              ),
      ( long                 , x                ),
      ( long                 , y                ),
      ( char * /* float2 * */, pixel_float2_ptr ))

void ImageDrawPoint_Double2
  P4 (( image                 , img               ),
      ( long                  , x                 ),
      ( long                  , y                 ),
      ( char * /* double2 * */, pixel_double2_ptr ))

void ImageDrawPoint_Char3
  P4 (( image               , img             ),
      ( long                , x               ),
      ( long                , y               ),
      ( char * /* char3 * */, pixel_char3_ptr ))

void ImageDrawPoint_Short3
  P4 (( image                , img              ),
      ( long                 , x                ),
      ( long                 , y                ),
      ( char * /* short3 * */, pixel_short3_ptr ))

void ImageDrawPoint_Int3
  P4 (( image              , img            ),
      ( long               , x              ),
      ( long               , y              ),
      ( char * /* int3 * */, pixel_int3_ptr ))

void ImageDrawPoint_Long3
  P4 (( image               , img             ),
      ( long                , x               ),
      ( long                , y               ),
      ( char * /* long3 * */, pixel_long3_ptr ))

void ImageDrawPoint_UChar3
  P4 (( image                , img              ),
      ( long                 , x                ),
      ( long                 , y                ),
      ( char * /* uchar3 * */, pixel_uchar3_ptr ))

void ImageDrawPoint_UShort3
  P4 (( image                 , img               ),
      ( long                  , x                 ),
      ( long                  , y                 ),
      ( char * /* ushort3 * */, pixel_ushort3_ptr ))

void ImageDrawPoint_UInt3
  P4 (( image               , img             ),
      ( long                , x               ),
      ( long                , y               ),
      ( char * /* uint3 * */, pixel_uint3_ptr ))

void ImageDrawPoint_ULong3
  P4 (( image                , img              ),
      ( long                 , x                ),
      ( long                 , y                ),
      ( char * /* ulong3 * */, pixel_ulong3_ptr ))

void ImageDrawPoint_Float3
  P4 (( image                , img              ),
      ( long                 , x                ),
      ( long                 , y                ),
      ( char * /* float3 * */, pixel_float3_ptr ))

void ImageDrawPoint_Double
  P4 (( image                , img              ),
      ( long                 , x                ),
      ( long                 , y                ),
      ( char * /* double * */, pixel_double_ptr ))



#endif /* __ImageCG_h */
