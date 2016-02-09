/*
 * XImage 1 Plane
 */



#include "XImage.h"



typedef struct CallerRec {
  long type;
  XCreateImageFunc func;
} Caller;

static Caller table[] = {
  {PackedBit1,XCreateImageFromData_PackedBit1},
  {Bit1,XCreateImageFromData_Bit1},
  {Char,  XCreateImageFromData_1Plane_Char},
  {Long,  XCreateImageFromData_1Plane_Long},
  {UChar, XCreateImageFromData_1Plane_UChar},
  {UChar3, XCreateImageFromData_1Plane_UChar3},
};



XImage *XCreateImageFromData_1Plane
  _P7 (( Display *, display   ),
       ( long     , win_xsize ),
       ( long     , win_ysize ),
       ( long     , type      ),
       ( char **  , data      ),
       ( long     , img_xsize ),
       ( long     , img_ysize ))
{
  XImage *src;
  Caller *caller;

  caller = typeselectconst( type, Caller, table );

  src = caller->func( display, win_xsize, win_ysize,
		      data, img_xsize, img_ysize );

  return src;
}
