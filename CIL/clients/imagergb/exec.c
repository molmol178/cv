#include "XRGB.h"



static long resize_window();
static long big_window();
static long ratio_window();
static long halfsize_window();
static long help();
static long disp();



static char * mess[] = {
  "q ... quit",
  "R ... redraw",
  "h ... hiritsu hosei",
  "1 ... x 1",
  "2 ... x 0.5",
  "3 ... x 0.6666",
  "4 ... x 0.25",
  "B ... x 1.5",
  "? ... print this message",
  0
};

XRGB *rgbE;

void xrgb_exec(rgb)
     XRGB *rgb;
{
  static XcilCallback key[] = {
    {'q',Xcil_hdr_quit},
    {'R',(xcil_callback)disp,0},
    {'h',(xcil_callback)ratio_window,0},
    {'1',(xcil_callback)resize_window,1},
    {'2',(xcil_callback)resize_window,2},
    {'3',(xcil_callback)halfsize_window,0},
    {'4',(xcil_callback)resize_window,4},
    {'B',(xcil_callback)big_window,0},
    {'?',(xcil_callback)help}
  };

  rgbE = rgb;

  XcilAddCallback(rgb->display,rgb->win,rgb->context,
		  Expose,(xcil_callback)disp,(long)rgb);
  XcilAddCallback(rgb->display,rgb->win,rgb->context,
		  ButtonPress,Xcil_hdr_map_popup,rgb->menu);
  XcilAddKeyCallbacks(rgb->display,rgb->win,rgb->context,key,XcilNumber(key));

  XcilEventLoop(rgb->display,rgb->context);
}



static long resize_window(d,w,c,e,s)
     Display *d;
     Window w;
     XContext c;
     XEvent *e;
     int s;
{
  XResizeWindow(d,w,rgbE->xsize/s,rgbE->ysize/s);
  XSync(d,True);
  return 0;
}



static long halfsize_window(d,w,c,e)
     Display *d;
     Window w;
     XContext c;
     XEvent *e;
{
  XWindowAttributes xwa;
  XGetWindowAttributes(d,w,&xwa);
  XResizeWindow(d,w,xwa.width/2,xwa.height/2);
  XSync(d,True);
  return 0;
}



static long big_window(d,w,c,e)
     Display *d;
     Window w;
     XContext c;
     XEvent *e;
{
  XWindowAttributes xwa;
  XGetWindowAttributes(d,w,&xwa);
  XResizeWindow(d,w,3*xwa.width/2,3*xwa.height/2);
  XSync(d,True);
  return 0;
}



static long help()
{
  int i = 0;
  while(mess[i]) {putchar('\t');puts(mess[i]),i++;}
  return 0;
}



static long ratio_window(d,w,c,e)
     Display *d;
     Window w;
     XContext c;
     XEvent *e;
{
  XWindowAttributes xwa;
  XGetWindowAttributes(d,w,&xwa);
  xwa.height = xwa.width * rgbE->ysize / rgbE->xsize;
  XResizeWindow(d,w,xwa.width,xwa.height);
  return 0;
}



static long disp(d,w,c,e)
     Display *d;
     Window w;
     XContext c;
     XEvent *e;
{
  xrgb_disp(rgbE);
  return 0;
}
