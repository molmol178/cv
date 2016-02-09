/*
 * xrgb
 */



#include <math.h>
#include "Image.h"
#include "Xcil/Xcil.h"
#include "Xcil/XImage.h"


typedef struct _xrgb {
  Display *display;
  XContext context;
  GC       gc;
  Window menu;
  Window win;
  ulong bg;
  long fwidth;
  long fheight;

  char * fname;
  image img;

  long rx,ry,rxsize,rysize;
  long gx,gy,gxsize,gysize;
  long bx,by,bxsize,bysize;
  long mx,my,mxsize,mysize;

  long xsize,ysize;
} XRGB;



void xrgb_open  P ((XRGB *rgb));
void xrgb_disp  P ((XRGB *rgb));
void xrgb_exec  P ((XRGB *rgb));
void xrgb_close P ((XRGB *rgb));
