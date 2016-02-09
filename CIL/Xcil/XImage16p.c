/*
 * XImage 16 Planes
 *
 * 1111 1_111 111_1 1111
 *
 *
 */



#include "XImage.h"



typedef struct CallerRec {
  long type;
  XCreateImageFunc func;
} Caller;



static Caller table[] = {
  {PackedBit1,XCreateImageFromData_PackedBit1},
  {Bit1,XCreateImageFromData_Bit1},
  {Char,  XCreateImageFromData_16Planes_Char},
  {Long,  XCreateImageFromData_16Planes_Long},
  {UChar, XCreateImageFromData_16Planes_UChar},
  {UChar3, XCreateImageFromData_16Planes_UChar3},
};



XImage *XCreateImageFromData_16Planes
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
