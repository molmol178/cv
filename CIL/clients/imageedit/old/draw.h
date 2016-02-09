/*
 * draw.c
 */



#include "Edit.h"



#define draw__func(e)  e->draw.func
#define draw__state(e) e->draw.state
#define draw__on(e)    e->draw.on_object



void draw__open(Edit e);
long draw__sel(Edit e,xw_object obj,XEvent *event,long value);
long draw__close(Edit e);



typedef void (*DrawObject)(Edit e,long x1,long y1,long x2,long y2);
typedef void (*WinDrawObject)(Edit e);


extern DrawObject DrawObjectList[];
extern WinDrawObject WinDrawObjectList[];



enum {
  D_POINT,
  D_POINTS,
  D_SEGMENT,
  D_LINES,

  D_BOX,
  D_POLY,
  D_CIRCLE1,
  D_CIRCLE2,
  D_CIRCLE3,
  D_CIRCLE4,
  D_ELLIPSE1,
  D_ELLIPSE2,

  F_BOX,
  F_POLY,
  F_CIRCLE1,
  F_CIRCLE2,
  F_CIRCLE3,
  F_CIRCLE4,
  F_ELLIPSE1,
  F_ELLIPSE2,

  STORE_AREA,
  RESTORE_AREA,
  ZOOM_AREA
};
