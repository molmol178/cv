/*
 * filename : section.c
 * author   : Takahiro Sugiyama
 * date     : Thursday, June 22 1995
 * describe : Section
 */



#include "Image.h"
#include "misc/optlib.h"
#include "Image/Funcs.h"
#include "Xcil/Xcil.h"
#include "Xcil/XImage.h"
#include <math.h>
#include <stdio.h>



static char *option[] = {
  "input:*:(image) name of input gray-scale or rgb-color image",
  "view:-v:1:(image) name of view gray-scale or rgb-color image",
  "scale:-s:1:1:(int) scale of section",
  "label:-l:1:I:(string) label of y coordinate",
  "overlap:-o:1:set overlap view",
  "help:-h:print this messages",
};



#define GrabA 1
#define GrabB 2
#define GrabCenter 3

typedef struct {
  Display *display;
  XContext context;
  Window back_win;
  Window image_win;
  image input;
  image view;
  image input_data;
  image view_data;
  char *label;
  long grab;
  long state;
  long ax, ay;
  long bx, by;
  long cx, cy;
  long graph_ax, graph_ay;
  long graph_bx, graph_by;
  long grab_ax, grab_ay;
  long grab_bx, grab_by;
  long grab_x, grab_y;
  long qline, qcoord, qliner;
  long scale;
  long xoff, yoff;
  Pixmap a_pixmap, b_pixmap;
  Pixmap bar, pin;
  long qpin, pinx, pin_i, pin_ix, pin_iy;
  char buf[64];
  char vbuf[64];
  long *xpoint;
  long *ypoint;
  long point_style;
  long overlap_view;
} ApplRec;



/*------------------------------------------------------------------------*
 * 純粋に断面だけを書く。
 *------------------------------------------------------------------------*/
void __pure_draw_section(ApplRec *appl, GC gc, long *list, long n)
{
  Display *d = appl->display;
  Window w = appl->back_win;
  long scale = appl->scale;
  long xoff = appl->xoff;
  long yoff = appl->yoff;
  int x1, y1, x2, y2;
  int i, np;
  XPoint *points;

  points = typenew1(3 * n, XPoint);
  np = 0;
  for (i = 0; i < n; i++)
    {
      if (list[i] == -1) continue;

      x1 = xoff + scale * i + 12;
      x2 = x1 + scale;
      y1 = yoff + 256 - list[i];
      points[np].x = x1;
      points[np].y = y1; np++;
      points[np].x = x2;
      points[np].y = y1; np++;

      if (list[i+1] == -1) continue;
      y2 = yoff + 256 - list[i+1];
      points[np].x = x2;
      points[np].y = y2; np++;
    }

  XDrawLines(d,w,gc,points,np,CoordModeOrigin);

  typefree1(points);
}



/*========================================================================*
 * 断面だけを描く:濃淡画像
 *========================================================================*/
void draw_section(ApplRec *appl, image input)
{
  long dx, dy;
  long x1, y1, x2, y2, xsize, ysize;
  long step, xstep, ystep;
  long s, i, n;
  long *list;

  xsize = __XSIZE(input);
  ysize = __YSIZE(input);

  x1 = appl->ax; y1 = appl->ay;
  x2 = appl->bx; y2 = appl->by;

  dx = abs( x2 - x1 );
  dy = abs( y2 - y1 );
  xstep = (x1 > x2) ? -1 : 1;
  ystep = (y1 > y2) ? -1 : 1;

  n = (dx > dy) ? dx : dy;

  list = typenew1(n + 2, long);
  for (i = 0; i <= n+1; i++) list[i] = -1;

  n = 0;
  if ((x1 >= 0) && (y1 >= 0) && (x1 < xsize) && (y1 < ysize))
    list[n] = __PIXEL(input, x1, y1, uchar);
  appl->xpoint[n] = x1;
  appl->ypoint[n] = y1;
  n++;
  if ((x1 != x2) || (y1 != y2))
    if (dx > dy)
      {
	s = dx / 2;
	for ( i = x1 + xstep; i != x2+xstep; i += xstep )
	  {
	    s -= dy;
	    if ( s < 0 ) { s += dx; y1 += ystep; }
	    if ((i >= 0) && (y1 >= 0) && (i < xsize) && (y1 < ysize))
	      {
		list[n] = __PIXEL(input, i, y1, uchar);
	      }
	    appl->xpoint[n] = i;
	    appl->ypoint[n] = y1;
	    n++;
	  }
      }
    else
      {
	s = dy / 2;
	for ( i = y1 + ystep; i != y2+ystep; i += ystep )
	  {
	    s -= dx;
	    if ( s < 0 ) { s += dy; x1 += xstep; }
	    if ((x1 >= 0) && (i >= 0) && (x1 < xsize) && (i < ysize))
	      list[n] = __PIXEL(input, x1, i, uchar);
	    appl->xpoint[n] = x1;
	    appl->ypoint[n] = i;
	    n++;
	  }
      }

  __pure_draw_section(appl, Xcil_black_gc, list, n);
  typefree1(list);
}



/*------------------------------------------------------------------------*
 * 断面のリストを作成する。カラー画像。
 *------------------------------------------------------------------------*/
long __make_rgb_section_list(ApplRec *appl, image input,
			     long **plist0, long **plist1, long **plist2)
{
  long dx, dy;
  long x1, y1, x2, y2, xsize, ysize;
  long step, xstep, ystep;
  long s, i, n;
  long *list0, *list1, *list2;
  char green_dash[] = {1,1};
  char blue_dash[] = {3,3};

  xsize = __XSIZE(input);
  ysize = __YSIZE(input);

  x1 = appl->ax; y1 = appl->ay;
  x2 = appl->bx; y2 = appl->by;

  dx = abs( x2 - x1 );
  dy = abs( y2 - y1 );
  xstep = (x1 > x2) ? -1 : 1;
  ystep = (y1 > y2) ? -1 : 1;

  n = (dx > dy) ? dx : dy;

  list0 = typenew1(n+2, long);
  list1 = typenew1(n+2, long);
  list2 = typenew1(n+2, long);
  for (i = 0; i <= n+1; i++)
    {
      list0[i] = -1;
      list1[i] = -1;
      list2[i] = -1;
    }

  n = 0;
  if ((x1 >= 0) && (y1 >= 0) && (x1 < xsize) && (y1 < ysize))
    {
      list0[n] = __PIXEL0(input, x1, y1, uchar3);
      list1[n] = __PIXEL1(input, x1, y1, uchar3);
      list2[n] = __PIXEL2(input, x1, y1, uchar3);
    }
  appl->xpoint[n] = x1;
  appl->ypoint[n] = y1;
  n++;

  if ((x1 != x2) || (y1 != y2))
    if (dx > dy)
      {
	s = dx / 2;
	for ( i = x1 + xstep; i != (x2+xstep); i += xstep )
	  {
	    s -= dy;
	    if ( s < 0 ) { s += dx; y1 += ystep; }
	    if ((i >= 0) && (y1 >= 0) && (i < xsize) && (y1 < ysize))
	      {
		list0[n] = __PIXEL0(input, i, y1, uchar3);
		list1[n] = __PIXEL1(input, i, y1, uchar3);
		list2[n] = __PIXEL2(input, i, y1, uchar3);
	      }
	    appl->xpoint[n] = i;
	    appl->ypoint[n] = y1;
	    n++;
	  }
      }
    else
      {
	s = dy / 2;
	for ( i = y1 + ystep; i != (y2+ystep); i += ystep )
	  {
	    s -= dx;
	    if ( s < 0 ) { s += dy; x1 += xstep; }
	    if ((x1 >= 0) && (i >= 0) && (x1 < xsize) && (i < ysize))
	      {
		list0[n] = __PIXEL0(input, x1, i, uchar3);
		list1[n] = __PIXEL1(input, x1, i, uchar3);
		list2[n] = __PIXEL2(input, x1, i, uchar3);
	      }
	    appl->xpoint[n] = x1;
	    appl->ypoint[n] = i;
	    n++;
	  }
      }

  *plist0 = list0;
  *plist1 = list1;
  *plist2 = list2;

  return n;
}


/*========================================================================*
 * 断面だけを描く:カラー画像
 *========================================================================*/
void draw_section_rgb(ApplRec *appl, image input)
{
  Display *d = appl->display;
  Window w = appl->back_win;
  GC gc = Xcil_black_gc;
  char green_dash[] = {1,1};
  char blue_dash[] = {3,3};
  long *list0, *list1, *list2;
  long n;

  n = __make_rgb_section_list(appl, input, &list0, &list1, &list2);

  switch (Xcil_plane)
    {
    case 1:
      XSetLineAttributes(d, gc, 0, LineSolid, 0, 0);
      __pure_draw_section(appl, gc, list0, n);
      XSetLineAttributes(d, gc, 0, LineOnOffDash, 0, 0);
      XSetDashes(d, gc, 0, green_dash, 2);
      __pure_draw_section(appl, gc, list1, n);
      XSetDashes(d, gc, 0, blue_dash, 2);
      __pure_draw_section(appl, gc, list2, n);
      break;
    case 8:
      if (Xcil_visual == PseudoColor)
	{
	  XImageSetColormap(d, w);
	  XSetForeground(d, gc, XLUT_PC_UChar3[RMAX-1][0][0]);
	  __pure_draw_section(appl, gc, list0, n);
	  XSetForeground(d, gc, XLUT_PC_UChar3[0][3*(GMAX-1)/4][0]);
	  __pure_draw_section(appl, gc, list1, n);
	  XSetForeground(d, gc, XLUT_PC_UChar3[0][0][BMAX-1]);
	  __pure_draw_section(appl, gc, list2, n);
	  XSetForeground(d, gc, Xcil_black_pixel);
	}
      else
	{
	  XSetLineAttributes(d, gc, 0, LineSolid, 0, 0);
	  __pure_draw_section(appl, gc, list0, n);
	  XSetLineAttributes(d, gc, 0, LineOnOffDash, 0, 0);
	  XSetDashes(d, gc, 0, green_dash, 2);
	  __pure_draw_section(appl, gc, list1, n);
	  XSetDashes(d, gc, 0, blue_dash, 2);
	  __pure_draw_section(appl, gc, list2, n);
	}
      break;
    default:
      XSetForeground(d, gc, (0xff>>R_LowBit)<<R_Shift);
      __pure_draw_section(appl, gc, list0, n);
      XSetForeground(d, gc, (0xff>>G_LowBit)<<G_Shift);
      __pure_draw_section(appl, gc, list1, n);
      XSetForeground(d, gc, (0xff>>B_LowBit)<<B_Shift);
      __pure_draw_section(appl, gc, list2, n);
      XSetForeground(d, gc, Xcil_black_pixel);
      break;
    }

  typefree1(list0);
  typefree1(list1);
  typefree1(list2);
}



void __pure_draw_pin(ApplRec *appl, int x)
{
  Display *display = appl->display;
  Window window = appl->back_win;
  int wx, wy, ix, iy, xsize, ysize;
  int dx, dy, dmax;
  int x1, y1, x2, y2;
  int scale = appl->scale;

  x1 = appl->ax;
  x2 = appl->bx;
  y1 = appl->ay;
  y2 = appl->by;

  dx = abs(x2 - x1);
  dy = abs(y2 - y1);
  dmax = (dx > dy) ? dx : dy;

  XSetTile(display, Xcil_gray_gc, Xcil_gray_xpixmap[1]);
  scale = (scale > 1) ? scale-1 : 1;
  wy = appl->yoff;
  if (appl->qpin)
    {
      if (appl->overlap_view)
	XcilPutString(display, window, Xcil_invert_gc,
		      appl->pinx+4, 32, appl->vbuf);
      XcilPutString(display, window, Xcil_invert_gc,
		    appl->pinx+4, 16, appl->buf);
      XCopyArea(display, appl->bar, window, Xcil_black_gc,
		0, 0, scale, 256, appl->pinx, wy);
      XCopyArea(display, appl->pin, appl->image_win, Xcil_black_gc,
		0, 0, 10, 10, appl->pin_ix-4, appl->pin_iy-4);
      appl->qpin = 0;
      appl->pin_i = -1;
    }

  if ((x < 0) || (x > dmax)) return;

  xsize = __XSIZE(appl->input);
  ysize = __YSIZE(appl->input);
  ix = appl->xpoint[x];
  iy = appl->ypoint[x];
  if ((ix < 0) || (iy < 0) || (ix >= xsize) || (iy >= ysize)) return;

  wx = appl->xoff + appl->scale * x + 12 + 1;
  XCopyArea(display, window, appl->bar,  Xcil_black_gc,
	    wx, wy, scale, 256, 0, 0);
  XFillRectangle(display, window, Xcil_gray_gc, wx, wy, scale, 256);
  if (__TYPE(appl->input_data) == UChar)
    sprintf(appl->buf, "%s(%d,%d)=%d", appl->label, ix, iy,
	    __PIXEL(appl->input_data,ix, iy, uchar));
  else
  if (__TYPE(appl->input_data) == UChar3)
    sprintf(appl->buf, "%s(%d,%d)={%d,%d,%d}", appl->label, ix, iy,
	    __PIXEL0(appl->input_data,ix, iy, uchar3),
	    __PIXEL1(appl->input_data,ix, iy, uchar3),
	    __PIXEL2(appl->input_data,ix, iy, uchar3));
  XcilPutString(display, window, Xcil_invert_gc, wx+4, 16, appl->buf);
  if (appl->overlap_view)
    {
      if (__TYPE(appl->view_data) == UChar)
	sprintf(appl->vbuf, "V(%d,%d)=%d", ix, iy,
		__PIXEL(appl->view_data,ix, iy, uchar));
      else
	if (__TYPE(appl->view_data) == UChar3)
	  sprintf(appl->vbuf, "V(%d,%d)={%d,%d,%d}", ix, iy,
		  __PIXEL0(appl->view_data,ix, iy, uchar3),
		  __PIXEL1(appl->view_data,ix, iy, uchar3),
		  __PIXEL2(appl->view_data,ix, iy, uchar3));
      XcilPutString(display, window, Xcil_invert_gc, wx+4, 32, appl->vbuf);
    }

  XCopyArea(display, appl->image_win, appl->pin, Xcil_black_gc,
	    ix-4, iy-4, 10, 10, 0, 0);
  if (appl->point_style == 'o')
    {
      XDrawArc(display, appl->image_win, Xcil_black_gc,
	       ix-3, iy-3, 7, 7, 64*0, 64*360);
      XDrawArc(display, appl->image_win, Xcil_white_gc,
	       ix-4, iy-4, 7, 7, 64*0, 64*360);
    }
  else
    {
      XDrawLine(display, appl->image_win, Xcil_black_gc,
		ix-2, iy-2, ix+4, iy+4);
      XDrawLine(display, appl->image_win, Xcil_black_gc,
		ix-2, iy+4, ix+4, iy-2);
      XDrawLine(display, appl->image_win, Xcil_white_gc,
		ix-3, iy-3, ix+3, iy+3);
      XDrawLine(display, appl->image_win, Xcil_white_gc,
		ix-3, iy+3, ix+3, iy-3);
    }
  appl->pin_i = x;
  appl->pin_ix = ix;
  appl->pin_iy = iy;
  appl->qpin = 1;
  appl->pinx = wx;
}



void redraw_graph(ApplRec *appl)
{
  Display *d = appl->display;
  Window w = appl->back_win;
  GC gc = Xcil_black_gc;
  long width, max_width;
  long dx, dy, xsize, ysize;
  long wx, wy;

  xsize = __XSIZE(appl->input);
  ysize = __YSIZE(appl->input);
  max_width = (xsize > ysize) ? xsize : ysize;

  dx = abs(appl->graph_ax - appl->graph_bx);
  dy = abs(appl->graph_ay - appl->graph_by);
  width = (dx > dy) ? dx : dy;
  XcilGetWindowSize(d, w, &wx, &wy);
  XFillRectangle(d, w, Xcil_white_gc,
		 appl->xoff + 13, appl->yoff, wx - 13 - appl->xoff+1, 257);
  appl->qpin = 0;

  /* 軸線を書く */
  dx = abs(appl->ax - appl->bx);
  dy = abs(appl->ay - appl->by);
  width = (dx > dy) ? dx : dy;
  XcilPutStringBox(d, w, gc, appl->xoff, appl->yoff+1, 8, 16, "lc",appl->label);
  XDrawLine(d, w, gc, appl->xoff + 12, appl->yoff,
	    appl->xoff + 12, appl->yoff+256);
  XDrawLine(d, w, gc, appl->xoff + 12, appl->yoff+256,
	    appl->xoff + 16+appl->scale*width, appl->yoff+256);

  if (__TYPE(appl->input_data) == UChar)
    draw_section(appl, appl->input_data);
  else
  if (__TYPE(appl->input_data) == UChar3)
    draw_section_rgb(appl, appl->input_data);

  if (appl->overlap_view)
    {
      static char overdash[] = {1, 1, 4, 1};
      XSetLineAttributes(d, gc, 0, LineOnOffDash, 0, 0);
      XSetDashes(d, gc, 0, overdash, 4);
      if (__TYPE(appl->view_data) == UChar)
	draw_section(appl, appl->view_data);
      else
      if (__TYPE(appl->view_data) == UChar3)
	draw_section_rgb(appl, appl->view_data);
      XSetLineAttributes(d, gc, 0, LineSolid, 0, 0);
    }

  __pure_draw_pin(appl, appl->pin_i);
}



void redraw_coord(ApplRec *appl)
{
  Display *d = appl->display;
  Window w = appl->back_win;
  GC gc = Xcil_invert_gc;
  char buf[256];
  long width;
  long dx, dy;

  if (appl->qcoord)
    {
      dx = abs(appl->graph_ax - appl->graph_bx);
      dy = abs(appl->graph_ay - appl->graph_by);
      width = (dx > dy) ? dx : dy;
      sprintf(buf,"A(%d,%d)",appl->graph_ax, appl->graph_ay);
      XcilPutStringBox(d, w, gc, appl->xoff + 1, appl->yoff+258,
		       100, 16, "lc", buf);
      sprintf(buf,"B(%d,%d)",appl->graph_bx, appl->graph_by);
      XcilPutStringBox(d, w, gc, appl->xoff + appl->scale*width,
		       appl->yoff+258, 100, 16, "lc", buf);
      appl->qcoord = 0;
    }
  appl->qcoord = 1;
  dx = abs(appl->ax - appl->bx);
  dy = abs(appl->ay - appl->by);
  width = (dx > dy) ? dx : dy;
  sprintf(buf,"A(%d,%d)",appl->ax, appl->ay);
  XcilPutStringBox(d, w, gc, appl->xoff + 1, appl->yoff+258,
		   100, 16, "lc", buf);
  sprintf(buf,"B(%d,%d)",appl->bx, appl->by);
  XcilPutStringBox(d, w, gc, appl->xoff + appl->scale*width,
		   appl->yoff+258, 100, 16, "lc", buf);

  redraw_graph(appl);
}



void draw_graph(ApplRec *appl)
{
  Display *d = appl->display;
  Window w = appl->image_win;
  GC gc = Xcil_invert_gc;
  GC bgc = Xcil_black_gc;
  GC wgc = Xcil_white_gc;
  int x1, x2, y1, y2;
  int ox1, ox2, oy1, oy2;

  x1 = appl->ax;
  y1 = appl->ay;
  x2 = appl->bx;
  y2 = appl->by;
  ox1 = appl->graph_ax;
  oy1 = appl->graph_ay;
  ox2 = appl->graph_bx;
  oy2 = appl->graph_by;
  if ((x1 == ox1) && (y1 == oy1) && (x2 == ox2) && (y2 == oy2)) return;

  XSetLineAttributes(d, gc, 1, 0, 0, 0);
  if (appl->qline)
    {
      XDrawLine(d, w, gc, ox1, oy1, ox2, oy2);
      XCopyArea(d, appl->a_pixmap, w, bgc, 0, 0, 8, 16, ox1-4, oy1-4);
      XCopyArea(d, appl->b_pixmap, w, bgc, 0, 0, 8, 16, ox2-4, oy2-4);
      appl->qline = 0;
    }
  XCopyArea(d, w, appl->a_pixmap, bgc, x1-4, y1-4, 8, 16, 0, 0);
  XCopyArea(d, w, appl->b_pixmap, bgc, x2-4, y2-4, 8, 16, 0, 0);
  XDrawLine(d, w, gc, x1, y1, x2, y2);
  XDrawString(d, w, bgc, x1-3, y1+7, "A", 1);
  XDrawString(d, w, bgc, x2-3, y2+7, "B", 1);
  XDrawString(d, w, wgc, x1-4, y1+6, "A", 1);
  XDrawString(d, w, wgc, x2-4, y2+6, "B", 1);
  appl->qline = 1;

  redraw_coord(appl);

  appl->graph_ax = x1;
  appl->graph_ay = y1;
  appl->graph_bx = x2;
  appl->graph_by = y2;
}



long button_release(Display *display, Window window, XContext context,
		    XEvent *event, ApplRec *appl, caddr_t value)
{
  XUngrabPointer(display, 0);
  appl->grab = 0;
  appl->state = 0;
  return 0;
}


long button_press(Display *display, Window window, XContext context,
		  XEvent *event, ApplRec *appl, caddr_t value)
{
  int x, y;
  long xsize, ysize;
  double da, db, dc, ab;
  int i, a, b, ax, ay, bx, by, dx, dy;

  xsize = __XSIZE(appl->input);
  ysize = __YSIZE(appl->input);

  x = event->xbutton.x;
  y = event->xbutton.y;
  if (x < 0 || x >= xsize) return 0;
  if (y < 0 || y >= ysize) return 0;

  if (event->xbutton.button != Button1) return 0;

  ax = appl->ax; ay = appl->ay; bx = appl->bx; by = appl->by;

  da = sqrt((double)(ax-x)*(ax-x)+(ay-y)*(ay-y));
  db = sqrt((double)(bx-x)*(bx-x)+(by-y)*(by-y));
  ab = sqrt((double)(ax-bx)*(ax-bx)+(ay-by)*(ay-by));

  dx = ax - bx;
  dy = ay - by;
  dc = abs(dx*y - dy*x - dx*ay + dy*ax) / sqrt((double)dx*dx+dy*dy);

  if (da <= 5)
    {/* Grab A */
      appl->state = GrabA;
    }
  else
  if (db <= 5)
    {/* Grab B */
      appl->state = GrabB;
    }
  else
  if ((dc <= 5) && (sqrt(da*da + db*db - 2*dc*dc) <= (sqrt(ab*ab) + 5)))
    {/* Grab Centre */
      appl->state = GrabCenter;
      appl->cx = x;
      appl->cy = y;
    }

  return 0;
}



long motion_notify_back(Display *display, Window window, XContext context,
			XMotionEvent *event, ApplRec *appl, caddr_t value)
{
  int x;
  Window root, child;
  int x_root, y_root;
  unsigned int keys_buttons;

  XQueryPointer(display, window, &root, &child,
		&x_root, &y_root, &event->x, &event->y, &keys_buttons);

  x = (event->x - appl->xoff - 12) / appl->scale;
  __pure_draw_pin(appl, x);

  return 0;
}



long display_value(Display *display, Window window, XContext context,
		   XMotionEvent *event, ApplRec *appl, caddr_t value)
{
  long d;
  int dx, dy, dmax, dmin;
  int i, a, b, ax, ay;
  long xsize, ysize;
  int imin;

  xsize = __XSIZE(appl->input);
  ysize = __YSIZE(appl->input);

  a = event->x;
  b = event->y;
  if (a < 0 || b < 0 || a >= xsize || b >= ysize) return 0;
  ax = appl->ax;
  ay = appl->ay;
  dx = ax - appl->bx;
  dy = ay - appl->by;
  d = abs(dx*b - dy*a - dx*ay + dy*ax) / sqrt((double)dx*dx+dy*dy);
  if (d > 50)
    {
      __pure_draw_pin(appl, -1);
      return 0;
    }

  dx = abs(dx); dy = abs(dy);
  dmax = (dx > dy) ? dx : dy;
  dmin = ((appl->xpoint[0] - a)*(appl->xpoint[0] - a) +
	  (appl->ypoint[0] - b)*(appl->ypoint[0] - b));
  imin = 0;
  for (i = 1; i < dmax; i++)
    {
      d = ((appl->xpoint[i] - a)*(appl->xpoint[i] - a) +
	   (appl->ypoint[i] - b)*(appl->ypoint[i] - b));
      if (dmin > d)
	{
	  imin = i;
	  dmin = d;
	}
    }
  if (sqrt((double)dmin) <= 50)
    __pure_draw_pin(appl, imin);
  else
    __pure_draw_pin(appl, -1);

  return 0;
}



long motion_notify(Display *display, Window window, XContext context,
		   XMotionEvent *event, ApplRec *appl, caddr_t value)
{
  int x, y;
  long xsize, ysize;
  int dx, dy;

  Window root, child;
  int x_root, y_root;
  unsigned int keys_buttons;

  XQueryPointer(display, window, &root, &child,
		&x_root, &y_root, &event->x, &event->y, &keys_buttons);

  if (((keys_buttons & Button1MotionMask) == 0) && (appl->state == 0))
    {
      display_value(display, window, context, event, appl, value);
      return 0;
    }

  if ((keys_buttons & Button1MotionMask) == 0) return 0;
  if (appl->state == 0) return 0;

  xsize = __XSIZE(appl->input);
  ysize = __YSIZE(appl->input);

  x = event->x;
  y = event->y;
  if (x < 0 || x >= xsize) return 0;
  if (y < 0 || y >= ysize) return 0;

  if (appl->grab == 0)
    {
      XGrabPointer(display, window, False,
		   (ButtonReleaseMask|PointerMotionMask),
		   GrabModeAsync, GrabModeSync,
		   None, Xcil_hand2_cursor, 0);
      appl->grab_ax = appl->ax;
      appl->grab_ay = appl->ay;
      appl->grab_bx = appl->bx;
      appl->grab_by = appl->by;
      appl->grab_x = x;
      appl->grab_y = y;
      appl->grab = 1;
      __pure_draw_pin(appl, -1);
    }

  dx = x - appl->grab_x;
  dy = y - appl->grab_y;
  switch (appl->state)
    {
    case GrabCenter:
      {
	if (keys_buttons & ShiftMask)
	  if (abs(dx) >= abs(dy)) dy = 0; else dx = 0;
	appl->ax = appl->grab_ax + dx;
	appl->ay = appl->grab_ay + dy;
	appl->bx = appl->grab_bx + dx;
	appl->by = appl->grab_by + dy;
      }
      break;

    case GrabA:
      {
	appl->ax = appl->grab_ax + dx;
	appl->ay = appl->grab_ay + dy;
	if (keys_buttons & ShiftMask)
	  if (abs(appl->ax - appl->bx) >= abs(appl->ay - appl->by))
	    appl->ay = appl->by;
	  else
	    appl->ax = appl->bx;
      }
      break;

    case GrabB:
      {
	appl->bx = appl->grab_bx + dx;
	appl->by = appl->grab_by + dy;
	if (keys_buttons & ShiftMask)
	  if (abs(appl->ax - appl->bx) >= abs(appl->ay - appl->by))
	    appl->by = appl->ay;
	  else
	    appl->bx = appl->ax;
      }
      break;
    }
  draw_graph(appl);

  return 0;
}



long expose(Display *display, Window window, XContext context,
	    XEvent *event, ApplRec *appl, caddr_t value)
{
  appl->state = 0;
  appl->qline = 0;
  appl->qpin = 0;
  appl->qcoord = 0;
  appl->graph_ax = 0;
  XClearWindow(display, window);
  XShowImageFull(display, appl->image_win, Xcil_black_gc, appl->view);
  draw_graph(appl);
  __pure_draw_pin(appl, appl->pin_i);
  return 0;
}



long zoomin(Display *display, Window window, XContext context,
	    XEvent *event, ApplRec *appl, caddr_t value)
{
  if (appl->scale <= 8)
    {
      appl->scale++;
      appl->state = 0;
      appl->qpin = 0;
      appl->qline = 0;
      appl->qcoord = 0;
      appl->graph_ax = 0;
      XClearWindow(display, appl->back_win);
      XShowImageFull(display, appl->image_win, Xcil_black_gc, appl->view);
      draw_graph(appl);
      __pure_draw_pin(appl, appl->pin_i);
    }
  return 0;
}



long zoomout(Display *display, Window window, XContext context,
	    XEvent *event, ApplRec *appl, caddr_t value)
{
  if (appl->scale > 1)
    {
      appl->scale--;
      appl->state = 0;
      appl->qline = 0;
      appl->qpin = 0;
      appl->qcoord = 0;
      appl->graph_ax = 0;
      XClearWindow(display, appl->back_win);
      XShowImageFull(display, appl->image_win, Xcil_black_gc, appl->view);
      draw_graph(appl);
      __pure_draw_pin(appl, appl->pin_i);
    }
  return 0;
}



long set_overlap_view(Display *display, Window window, XContext context,
		      XEvent *event, ApplRec *appl, caddr_t value)
{
  appl->overlap_view = 1 - appl->overlap_view;
  appl->state = 0;
  appl->qline = 0;
  appl->qpin = 0;
  appl->qcoord = 0;
  appl->graph_ax = 0;
  XClearWindow(display, appl->back_win);
  XShowImageFull(display, appl->image_win, Xcil_black_gc, appl->view);
  draw_graph(appl);
  __pure_draw_pin(appl, appl->pin_i);

  return 0;
}



long swap_view_input(Display *display, Window window, XContext context,
		     XEvent *event, ApplRec *appl, caddr_t value)
{
  image tmp;

  tmp = appl->input;
  appl->input = appl->view;
  appl->view = tmp;

  tmp = appl->input_data;
  appl->input_data = appl->view_data;
  appl->view_data = tmp;

  appl->state = 0;
  appl->qline = 0;
  appl->qpin = 0;
  appl->qcoord = 0;
  appl->graph_ax = 0;
  XClearWindow(display, appl->back_win);
  XShowImageFull(display, appl->image_win, Xcil_black_gc, appl->view);
  draw_graph(appl);
  __pure_draw_pin(appl, appl->pin_i);

  return 0;
}



long change_a_b(Display *display, Window window, XContext context,
		XEvent *event, ApplRec *appl, caddr_t value)
{
  long x, y;

  x = appl->ax; y = appl->ay;
  appl->ax = appl->bx; appl->ay = appl->by;
  appl->bx = x; appl->by = y;

  appl->state = 0;
  appl->qline = 0;
  appl->qpin = 0;
  appl->qcoord = 0;
  appl->graph_ax = 0;
  XClearWindow(display, appl->back_win);
  XShowImageFull(display, appl->image_win, Xcil_black_gc, appl->view);
  draw_graph(appl);
  __pure_draw_pin(appl, -1);

  return 0;
}



long reverse_view(Display *display, Window window, XContext context,
		  XEvent *event, ApplRec *appl, caddr_t value)
{
  image__reverse(appl->view, appl->view);

  appl->state = 0;
  appl->qline = 0;
  appl->qpin = 0;
  appl->qcoord = 0;
  appl->graph_ax = 0;
  XClearWindow(display, appl->back_win);
  XShowImageFull(display, appl->image_win, Xcil_black_gc, appl->view);
  draw_graph(appl);
  __pure_draw_pin(appl, appl->pin_i);

  return 0;
}



long trans_view(Display *display, Window window, XContext context,
		XEvent *event, ApplRec *appl, caddr_t value)
{
  if (appl->qliner)
    Image.copy(appl->view, appl->view_data);
  else
    image__liner(appl->view, appl->view_data);
  appl->qliner = 1 - appl->qliner;

  appl->state = 0;
  appl->qline = 0;
  appl->qpin = 0;
  appl->qcoord = 0;
  appl->graph_ax = 0;
  XClearWindow(display, appl->back_win);
  XShowImageFull(display, appl->image_win, Xcil_black_gc, appl->view);
  draw_graph(appl);
  __pure_draw_pin(appl, appl->pin_i);

  return 0;
}



long change_style(Display *display, Window window, XContext context,
		  XKeyEvent *event, ApplRec *appl, long ascii)
{
  if (appl->point_style != ascii)
    {
      appl->point_style = ascii;
      appl->state = 0;
      appl->qline = 0;
      appl->qpin = 0;
      appl->qcoord = 0;
      appl->graph_ax = 0;
      XClearWindow(display, appl->back_win);
      XShowImageFull(display, appl->image_win, Xcil_black_gc, appl->view);
      draw_graph(appl);
      __pure_draw_pin(appl,appl->pin_i);
    }
  return 0;
}



long print_short_cut(Display *display, Window window, XContext context,
		     XKeyEvent *event, ApplRec *appl, long ascii)
{
  fprintf(stderr, "X Section Display Short Cut Keys and Mouse Operator...\n");
  fprintf(stderr, "\t+ ..... Zoom In Graph.\n");
  fprintf(stderr, "\t- ..... Zoom Out Graph.\n");
  fprintf(stderr, "\to ..... Change Pointer 'o'.\n");
  fprintf(stderr, "\tx ..... Change Pointer 'x'.\n");
  fprintf(stderr, "\tr ..... Reverse View Image.\n");
  fprintf(stderr, "\tt ..... Liner Transform View Image.\n");
  fprintf(stderr, "\tv ..... View Image Overlap.\n");
  fprintf(stderr, "\ts ..... Swap View and Input Image.\n");
  fprintf(stderr, "\tc ..... Change A and B.\n");
  fprintf(stderr, "\tq ..... Quit.\n");
  fprintf(stderr, "\t? ..... Print This Message.\n");
  fprintf(stderr, "\tDrag ........... Move A, B or Bar.\n");
  fprintf(stderr, "\tShift + Drag ... Move constrain in X or Y coordinate.\n");
  return 0;
}



void x_section(image input, image view)
{
  ApplRec application;
  Display *display;
  XContext context;
  long xsize, ysize, maxsize;
  long width, height;
  long graph_width, graph_height;
  Window back_win, image_win;

  display = XcilOpenDisplay(0);
  context = XUniqueContext();
  xsize = __XSIZE(input);
  ysize = __YSIZE(input);
  maxsize = (xsize > ysize) ? xsize : ysize;

  graph_width = sqrt((double)xsize*xsize + ysize*ysize) + 8*5;
  graph_height = 256 + 16;

  width = xsize + 400 + 2 + 5 * 2;
  height = ((graph_height > ysize) ? graph_height : ysize) + 2 + 5 * 2;

  back_win = XcilCreateWindow(display, 0, 0, 0,
			      width, height, 1, "XSection");
  image_win = XcilCreateWindow(display, back_win, 5, 5,
			       xsize, ysize, 1, "Image");

  XcilMapWindow(display, back_win);
  XcilMapWindow(display, image_win);

  XcilManagedWindow(display, back_win, context);
  XcilManagedWindow(display, image_win, context);
  XcilAddKeyCallback(display, back_win, context, 'q', Xcil_hdr_quit, 0);
  XcilAddKeyCallback(display, back_win, context, '+',
		     (xcil_callback)zoomin, (caddr_t)&application);
  XcilAddKeyCallback(display, back_win, context, '-',
		     (xcil_callback)zoomout, (caddr_t)&application);
  XcilAddKeyCallback(display, back_win, context, 'o',
		     (xcil_callback)change_style, (caddr_t)&application);
  XcilAddKeyCallback(display, back_win, context, 'x',
		     (xcil_callback)change_style, (caddr_t)&application);
  XcilAddKeyCallback(display, back_win, context, '?',
		     (xcil_callback)print_short_cut, (caddr_t)&application);
  XcilAddKeyCallback(display, back_win, context, 'v',
		     (xcil_callback)set_overlap_view, (caddr_t)&application);
  XcilAddKeyCallback(display, back_win, context, 's',
		     (xcil_callback)swap_view_input, (caddr_t)&application);
  XcilAddKeyCallback(display, back_win, context, 'c',
		     (xcil_callback)change_a_b, (caddr_t)&application);
  XcilAddKeyCallback(display, back_win, context, 'r',
		     (xcil_callback)reverse_view, (caddr_t)&application);
  XcilAddKeyCallback(display, back_win, context, 't',
		     (xcil_callback)trans_view, (caddr_t)&application);
  XcilAddCallback(display, back_win, context, MotionNotify,
		  (xcil_callback)motion_notify_back, (caddr_t)&application);

  application.back_win = back_win;
  application.image_win = image_win;
  application.input = input;
  application.input_data = Image.createFromImage("IValue", input);
  application.view = view;
  application.view_data = Image.createFromImage("VValue", view);
  application.display = display;
  application.context = context;
  XcilAddCallback(display, image_win, context, ButtonRelease,
		  (xcil_callback)button_release,(caddr_t)&application);
  XcilAddCallback(display, image_win, context, ButtonPress,
		  (xcil_callback)button_press, (caddr_t)&application);
  XcilAddCallback(display, image_win, context, MotionNotify,
		  (xcil_callback)motion_notify, (caddr_t)&application);
  XcilAddCallback(display, back_win, context, Expose,
		  (xcil_callback)expose, (caddr_t)&application);

  XShowImageFull(display, image_win, Xcil_black_gc, view);

  application.xoff = xsize + 10 + 2;
  application.yoff = 5;
  application.ax = xsize / 4;
  application.ay = ysize / 2;
  application.bx = 3 * xsize / 4;
  application.by = ysize / 2;
  application.scale = optvalueint("scale");
  application.state = 0;
  application.grab = 0;
  application.qpin = 0;
  application.pin_i = -1;
  application.qline = 0;
  application.qliner = 0;
  application.qcoord = 0;
  application.point_style = 'x';
  application.overlap_view = optspecified("overlap");
  application.xpoint = typenew1(2*graph_width,long);
  application.ypoint = typenew1(2*graph_width,long);
  application.a_pixmap = XCreatePixmap(display, image_win, 16, 16, Xcil_depth);
  application.b_pixmap = XCreatePixmap(display, image_win, 16, 16, Xcil_depth);
  application.bar = XCreatePixmap(display, back_win, 12, 256, Xcil_depth);
  application.pin = XCreatePixmap(display, image_win, 12, 12, Xcil_depth);
  application.label = optvalue("label");
  draw_graph(&application);

  XcilEventLoop(display, context);
}



/*========================================================================*
 * 画像の型を補正する。
 *========================================================================*/
void calibration_imagetype(image input)
{
  long type = __TYPE(input);
  image newimg;
  long x, y, xsize, ysize;
  register int i, n;

  xsize = __XSIZE(input);
  ysize = __YSIZE(input);
  n = __AREA(input);

  if ((type == UChar3) || (type == UChar)) return;

  newimg = Image.createMake("New", UChar, xsize, ysize);
  switch (type)
    {
    case Bit1:
      {
	register uchar *d = __RASTER(newimg, uchar);
	register bit1 *s = __RASTER(input, bit1);
	for (i = 0; i < n; i++)
	  *d++ = 200 - 200 * *s++;
      }
      break;
    case Char:
      {
	register uchar *d = __RASTER(newimg, uchar);
	register char *s = __RASTER(input, char);
	long smax, smin;
	smax = smin = *s;
	for (i = 0; i < n; i++,s++)
	  if (*s > smax) smax = *s; else if (*s < smin) smin = *s;
	s = __RASTER(input, char);
	for (i = 0; i < n; i++,s++,d++)
	  *d = 256 * (*s - smin) / (smax - smin + 1);
      }
      break;
    case Short:
      {
	register uchar *d = __RASTER(newimg, uchar);
	register short *s = __RASTER(input, short);
	long smax, smin;
	smax = smin = *s;
	for (i = 0; i < n; i++,s++)
	  if (*s > smax) smax = *s; else if (*s < smin) smin = *s;
	s = __RASTER(input, short);
	for (i = 0; i < n; i++,s++,d++)
	  *d = 256 * (*s - smin) / (smax - smin + 1);
      }
    case UShort:
      {
	register uchar *d = __RASTER(newimg, uchar);
	register ushort *s = __RASTER(input, ushort);
	ulong smax, smin;
	smax = smin = *s;
	for (i = 0; i < n; i++,s++)
	  if (*s > smax) smax = *s; else if (*s < smin) smin = *s;
	s = __RASTER(input, ushort);
	for (i = 0; i < n; i++,s++,d++)
	  *d = 256 * (*s - smin) / (smax - smin + 1);
      }
      break;
    case Long:
      {
	register uchar *d = __RASTER(newimg, uchar);
	register long *s = __RASTER(input, long);
	long smax, smin;
	smax = smin = *s;
	for (i = 0; i < n; i++,s++)
	  if (*s > smax) smax = *s; else if (*s < smin) smin = *s;
	s = __RASTER(input, long);
	for (i = 0; i < n; i++,s++,d++)
	  *d = 256 * (*s - smin) / (smax - smin + 1);
      }
    case ULong:
      {
	register uchar *d = __RASTER(newimg, uchar);
	register ulong *s = __RASTER(input, ulong);
	ulong smax, smin;
	smax = smin = *s;
	for (i = 0; i < n; i++,s++)
	  if (*s > smax) smax = *s; else if (*s < smin) smin = *s;
	s = __RASTER(input, ulong);
	for (i = 0; i < n; i++,s++,d++)
	  *d = 256 * (*s - smin) / (smax - smin + 1);
      }
      break;
    case Float:
      {
	register uchar *d = __RASTER(newimg, uchar);
	register float *s = __RASTER(input, float);
	double smax, smin;
	  for (i = 0; i < n; i++,s++,d++)
	    *d = 255 * (*s);
	  break;
	smax = smin = *s;
	for (i = 0; i < n; i++,s++)
	  if (*s > smax) smax = *s; else if (*s < smin) smin = *s;
	s = __RASTER(input, float);
	if ((smin>-1)&&(smax <= 512)&&(smax>=5))
	  for (i = 0; i < n; i++,s++,d++)
	    *d = *s;
	else
	  for (i = 0; i < n; i++,s++,d++)
	    *d = 255 * (*s - smin) / (smax - smin);
      }
      break;
    defaults:
      fprintf(stderr,"I don't know how to calibrate (%s)\n", typename(type));
      break;
    }
  Image.copy(input, newimg);
  Image.destroy(newimg);
}



/*========================================================================*
 * メインだよ。
 *========================================================================*/
int main( argc, argv )
     int argc;
     char *argv[];
{
  char *name_of_input;
  image input;
  image view;

  OPTION_SETUP( option );

  if ( optspecified( "help" ) ) optmanual( 1 );
  if ( argc == 1 ) optusage( 1 );

  name_of_input = optvalue("input");
  input = Image.createFromFilename("Input", name_of_input);
  if (input == 0)
    {
      fprintf(stderr,"cannot open input file (%s)\n", name_of_input);
      exit(-1);
    }
  calibration_imagetype(input);

  if (optspecified("view"))
    {
      char *name_of_view = optvalue("view");
      view = Image.createFromFilename("View", name_of_view);
      if (view == 0)
	{
	  fprintf(stderr,"cannot open view file (%s)\n", name_of_view);
	  exit(-1);
	}
      calibration_imagetype(view);
    }
  else
    {
      view = input;
    }

  x_section(input, view);

  return 0;
}
