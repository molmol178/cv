/*
 * XImage 32 Planes
 */



#include "XImage.h"



typedef struct CallerRec {
  long type;
  XCreateImageFunc func;
} Caller;



static Caller table[] = {
  {PackedBit1,XCreateImageFromData_PackedBit1},
  {Bit1,XCreateImageFromData_Bit1},
  {Char,  XCreateImageFromData_24Planes_Char},
  {Long,  XCreateImageFromData_24Planes_Long},
  {UChar, XCreateImageFromData_24Planes_UChar},
  {UChar3, XCreateImageFromData_32Planes_UChar3},
};



XImage *XCreateImageFromData_32Planes
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
