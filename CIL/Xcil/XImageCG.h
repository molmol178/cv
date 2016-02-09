/*
 * filename : XImageCG.h
 * author   : Takahiro Sugiyama
 * date     : 1993/09/26
 */



#ifndef __XImageCG_h
#define __XImageCG_h



#include "../misc/comacros.h"
#include <X11/Xlib.h>



typedef struct {
  short x;
  short y;
  unsigned long pixel;
} XImgPoint;



typedef struct {
  int x1, y1;
  int x2, y2;
  unsigned long pixel;
} XImgSegment, XImgBox;



typedef struct {
  int x, y;
  unsigned int width, height;
  unsigned long pixel;
} XImgRectangle;



typedef struct {
  short x, y;
  unsigned short width, height;
  short angle1, angle2;
  unsigned long pixel;
} XImgArc;



XImage *XImageCreate
  P3 (( Display *, display ),
      ( long     , xsize   ),
      ( long     , ysize   ))



void XImageDestroy
  P1 (( XImage *, img ))



void XImageDrawPoint
  P4 (( XImage *     , img   ),
      ( long         , x     ),
      ( long         , y     ),
      ( unsigned long, pixel ))



void XImageDrawPoints
  P4 (( XImage *   , img  ),
      ( XImgPoint *, list ),
      ( long       , num  ),
      ( long       , mode ))



void XImageDrawBox
  P6 (( XImage *     , img   ),
      ( long         , x1    ),
      ( long         , y1    ),
      ( long         , x2    ),
      ( long         , y2    ),
      ( unsigned long, pixel ))



void XImageDrawBoxes
  P3 (( XImage  *, img  ),
      ( XImgBox *, list ),
      ( long     , num  ))



void XImageDrawRectangle
  P6 (( XImage *     , img    ),
      ( int          , x      ),
      ( int          , y      ),
      ( unsigned int , width  ),
      ( unsigned int , height ),
      ( unsigned long, pixel  ))



void XImageDrawRectangles
  P3 (( XImage *       , img  ),
      ( XImgRectangle *, list ),
      ( long           , num  ))



void XImageDrawArc
  P8 (( XImage *     , img    ),
      ( int          , x      ),
      ( int          , y      ),
      ( unsigned int , width  ),
      ( unsigned int , height ),
      ( int          , angle1 ),
      ( int          , angle2 ),
      ( unsigned long, pixel  ))



void XImageDrawArcs
  P3 (( XImage * , img  ),
      ( XImgArc *, list ),
      ( long     , num  ))



void XImageFillRectangle
  P6 (( XImage *     , img    ),
      ( int          , x      ),
      ( int          , y      ),
      ( unsigned int , width  ),
      ( unsigned int , height ),
      ( unsigned long, pixel  ))



void XImageFillRectangles
  P3 (( XImage *       , img  ),
      ( XImgRectangle *, list ),
      ( long           , num  ))



void XImageFillArc
  P8 (( XImage *     , img    ),
      ( int          , x      ),
      ( int          , y      ),
      ( unsigned int , width  ),
      ( unsigned int , height ),
      ( int          , angle1 ),
      ( int          , angle2 ),
      ( unsigned long, pixel  ))



void XImageFillArcs
  P3 (( XImage * , img  ),
      ( XImgArc *, list ),
      ( long     , num  ))



void XImageFillPolygon
  P6 (( XImage *     , img   ),
      ( XImgPoint *  , list  ),
      ( long         , num   ),
      ( long         , shape ),
      ( long         , mode  ),
      ( unsigned long, pixel ))




void XImageSetStringFont
  P1 (( char *, name ))

void XImageSetStringFont16
  P1 (( char *, name ))

void XImageDrawString
  P6 (( XImage *      , img    ),
      ( long          , x      ),
      ( long          , y      ),
      ( char *        , str    ),
      ( long          , length ),
      ( unsigned long , pixel  ))

void XImageDrawString16
  P6 (( XImage *      , img    ),
      ( long          , x      ),
      ( long          , y      ),
      ( XChar2b *     , str    ),
      ( long          , length ),
      ( unsigned long , pixel  ))


void XImageDrawImageString
  P7 (( XImage *      , img       ),
      ( long          , x         ),
      ( long          , y         ),
      ( char *        , str       ),
      ( long          , length    ),
      ( unsigned long , forepixel ),
      ( unsigned long , backpixel ))

void XImageDrawImageString16
  P7 (( XImage *      , img       ),
      ( long          , x         ),
      ( long          , y         ),
      ( XChar2b *     , str       ),
      ( long          , length    ),
      ( unsigned long , forepixel ),
      ( unsigned long , backpixel ))



#endif __XImageCG_h
