/*
 * draw_obj.c
 */



#include "Edit.h"



static void d_point(Edit e,long x1,long y1,long x2,long y2);
static void d_points(Edit e,long x1,long y1,long x2,long y2);
static void d_segment(Edit e,long x1,long y1,long x2,long y2);
static void d_lines(Edit e,long x1,long y1,long x2,long y2);
static void d_box(Edit e,long x1,long y1,long x2,long y2);
static void d_poly(Edit e,long x1,long y1,long x2,long y2);
static void d_circle1(Edit e,long x1,long y1,long x2,long y2);
static void d_circle2(Edit e,long x1,long y1,long x2,long y2);
static void d_circle3(Edit e,long x1,long y1,long x2,long y2);
static void d_circle4(Edit e,long x1,long y1,long x2,long y2);
static void d_ellipse1(Edit e,long x1,long y1,long x2,long y2);
static void d_ellipse2(Edit e,long x1,long y1,long x2,long y2);
static void f_box(Edit e,long x1,long y1,long x2,long y2);
static void f_poly(Edit e,long x1,long y1,long x2,long y2);
static void f_circle1(Edit e,long x1,long y1,long x2,long y2);
static void f_circle2(Edit e,long x1,long y1,long x2,long y2);
static void f_circle3(Edit e,long x1,long y1,long x2,long y2);
static void f_circle4(Edit e,long x1,long y1,long x2,long y2);
static void f_ellipse1(Edit e,long x1,long y1,long x2,long y2);
static void f_ellipse2(Edit e,long x1,long y1,long x2,long y2);
static void store_area(Edit e,long x1,long y1,long x2,long y2);
static void restore_area(Edit e,long x1,long y1,long x2,long y2);
static void zoom_area(Edit e,long x1,long y1,long x2,long y2);



DrawObject DrawObjectList[] = {
  d_point,
  d_points,
  d_segment,
  d_lines,

  d_box,
  d_poly,
  d_circle1,
  d_circle2,
  d_circle3,
  d_circle4,
  d_ellipse1,
  d_ellipse2,

  f_box,
  f_poly,
  f_circle1,
  f_circle2,
  f_circle3,
  f_circle4,
  f_ellipse1,
  f_ellipse2,

  store_area,
  restore_area,
  zoom_area
};



static void win_d_point(Edit e);
static void win_d_points(Edit e);
static void win_d_segment(Edit e);
static void win_d_lines(Edit e);
static void win_d_box(Edit e);
static void win_d_poly(Edit e);
static void win_d_circle1(Edit e);
static void win_d_circle2(Edit e);
static void win_d_circle3(Edit e);
static void win_d_circle4(Edit e);
static void win_d_ellipse1(Edit e);
static void win_d_ellipse2(Edit e);
static void win_f_box(Edit e);
static void win_f_poly(Edit e);
static void win_f_circle1(Edit e);
static void win_f_circle2(Edit e);
static void win_f_circle3(Edit e);
static void win_f_circle4(Edit e);
static void win_f_ellipse1(Edit e);
static void win_f_ellipse2(Edit e);
static void win_restore_area(Edit e);
static void win_store_area(Edit e);
static void win_zoom_area(Edit e);



WinDrawObject WinDrawObjectList[] = {
  win_d_point,
  win_d_points,
  win_d_segment,
  win_d_lines,

  win_d_box,
  win_d_poly,
  win_d_circle1,
  win_d_circle2,
  win_d_circle3,
  win_d_circle4,
  win_d_ellipse1,
  win_d_ellipse2,

  win_f_box,
  win_f_poly,
  win_f_circle1,
  win_f_circle2,
  win_f_circle3,
  win_f_circle4,
  win_f_ellipse1,
  win_f_ellipse2,

  win_store_area,
  win_restore_area,
  win_zoom_area
};



static void d_point(Edit e,long x1,long y1,long x2,long y2)
{
  CG.drawPoint(Edit__image_data(e),e->pen.pen,x1,y1);
}



static void d_points(Edit e,long x1,long y1,long x2,long y2)
{
  CG.drawLine(Edit__image_data(e),e->pen.pen,x1,y1,x2,y2);
}



static void d_segment(Edit e,long x1,long y1,long x2,long y2)
{
  CG.drawLine(Edit__image_data(e),e->pen.pen,x1,y1,x2,y2);
}



static void d_lines(Edit e,long x1,long y1,long x2,long y2)
{
  CG.drawLines(Edit__image_data(e),e->pen.pen,
	       e->polygon_n+1,e->pi_point);
}



static void d_box(Edit e,long x1,long y1,long x2,long y2)
{
  CG.drawBox(Edit__image_data(e),e->pen.pen,x1,y1,x2,y2);
}


static void d_poly(Edit e,long x1,long y1,long x2,long y2)
{
  CG.drawPolygon(Edit__image_data(e),e->pen.pen,
		 e->polygon_n+2,e->pi_point);
}



static void d_circle1(Edit e,long x1,long y1,long x2,long y2)
{
  long r1,r2;

  r1 = abs(x1 - x2);
  r2 = abs(y1 - y2);
  CG.drawCircle(Edit__image_data(e),e->pen.pen,x1,y1,minimum(r1,r2));
}



static void d_circle2(Edit e,long x1,long y1,long x2,long y2)
{
  CG.drawCircle2(Edit__image_data(e),e->pen.pen,x1,y1,x2,y2);
}



static void d_circle3(Edit e,long x1,long y1,long x2,long y2)
{
  CG.drawCircle3(Edit__image_data(e),e->pen.pen,x1,y1,x2,y2);
}



static void d_circle4(Edit e,long x1,long y1,long x2,long y2)
{
  long r;

  r = minimum(abs(x1-x2),abs(y1-y2));
  if (x1 < x2) x2 = x1 + r; else x2 = x1 - r;
  if (y1 < y2) y2 = y1 + r; else y2 = y1 - r;

  CG.drawCircle4(Edit__image_data(e),e->pen.pen,x1,y1,x2,y2);
}



static void d_ellipse1(Edit e,long x1,long y1,long x2,long y2)
{
  CG.drawEllipse(Edit__image_data(e),e->pen.pen,
		 x1,y1,abs(x1-x2),abs(y1-y2));
}



static void d_ellipse2(Edit e,long x1,long y1,long x2,long y2)
{
  CG.drawEllipse4(Edit__image_data(e),e->pen.pen,x1,y1,x2,y2);
}



static void f_box(Edit e,long x1,long y1,long x2,long y2)
{
  CG.fillBox(Edit__image_data(e),e->pen.pen,x1,y1,x2,y2);
}



static void f_circle1(Edit e,long x1,long y1,long x2,long y2)
{
  long r1,r2;

  r1 = abs(x1 - x2);
  r2 = abs(y1 - y2);
  CG.fillCircle(Edit__image_data(e),e->pen.pen,x1,y1,minimum(r1,r2));
}



static void f_circle2(Edit e,long x1,long y1,long x2,long y2)
{
  CG.fillCircle2(Edit__image_data(e),e->pen.pen,x1,y1,x2,y2);
}



static void f_circle3(Edit e,long x1,long y1,long x2,long y2)
{
  CG.fillCircle3(Edit__image_data(e),e->pen.pen,x1,y1,x2,y2);
}



static void f_circle4(Edit e,long x1,long y1,long x2,long y2)
{
  long r;

  r = minimum(abs(x1-x2),abs(y1-y2));
  if (x1 < x2) x2 = x1 + r; else x2 = x1 - r;
  if (y1 < y2) y2 = y1 + r; else y2 = y1 - r;

  CG.fillCircle4(Edit__image_data(e),e->pen.pen,x1,y1,x2,y2);
}



static void f_ellipse1(Edit e,long x1,long y1,long x2,long y2)
{
  CG.fillEllipse(Edit__image_data(e),e->pen.pen,
		 x1,y1,abs(x1-x2),abs(y1-y2));
}



static void f_ellipse2(Edit e,long x1,long y1,long x2,long y2)
{
  CG.fillEllipse4(Edit__image_data(e),e->pen.pen,x1,y1,x2,y2);
}



static void f_poly(Edit e,long x1,long y1,long x2,long y2)
{
  CG.fillPolygon(Edit__image_data(e),e->pen.pen,
		 e->polygon_n+2,e->pi_point);
}



static void store_area(Edit e,long x1,long y1,long x2,long y2)
{
  if (x1 > x2) {long tmp = x1; x1 = x2; x2 = tmp;}
  if (y1 > y2) {long tmp = y1; y1 = y2; y2 = tmp;}
  buffer__copy_area(e,x1,y1,x2,y2);
}



static void restore_area(Edit e,long x1,long y1,long x2,long y2)
{
  long x,y;
  long xsize,ysize;
  long dest_xsize,dest_ysize;
  long type,src_type;
  bit1 binary = e->pen.binary;
  uchar gray = e->pen.gray;
  uchar3 color = e->pen.color;

  image      src,dest;
  image      src_mask,dest_mask;
  char     **src_data,**dest_data;
  bit1     **src_masking,**dest_masking;

  if (x1 > x2) {long tmp = x1; x1 = x2; x2 = tmp;}
  if (y1 > y2) {long tmp = y1; y1 = y2; y2 = tmp;}

  dest  = imageItem__image(Edit__original(e));
  src   = imageItem__image(buffer__item_current(e));

  dest_mask = imageItem__mask(Edit__original(e));
  src_mask  = imageItem__mask(buffer__item_current(e));

  dest_data = Image.data(dest);
  src_data  = Image.data(src);

  dest_masking = (bit1 **)Image.data(dest_mask);
  src_masking  = (bit1 **)Image.data(src_mask);

  type  = Image.type(dest);
  src_type = Image.type(src);
  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  dest_xsize = Image.xsize(dest);
  dest_ysize = Image.ysize(dest);

  if (type != src_type) {
    XwObject.setLabel(e->message," warning:Image type is wrong.");
  }

  for (y = 0; (y < ysize) && (y+y1 < dest_ysize); y++)
    for (x = 0; (x < xsize) && (x+x1 < dest_xsize); x++)
      {
/*
	if (src_masking[y][x] == 0 || dest_masking[x+x1][y+y1] == 0)
	  continue;
*/
	if (type == Bit1)
	  {
	    if (src_type == UChar3)
	      e->pen.binary = ((((uchar3 **)src_data)[y][x].at[0] != 0)||
			       (((uchar3 **)src_data)[y][x].at[1] != 0)||
			       (((uchar3 **)src_data)[y][x].at[2] != 0));
	    else
	    if (src_type == UChar)
	      e->pen.binary = (((uchar **)src_data)[y][x] != 0);
	    else
	      e->pen.binary = ((bit1 **)src_data)[y][x];

	    pen__draw(dest_data,x+x1,y+y1,1);
	  }
	else
	if (type == UChar)
	  {
	    if (src_type == UChar3)
	      e->pen.gray = (((uchar3 **)src_data)[y][x].at[0] + 
			     ((uchar3 **)src_data)[y][x].at[1] + 
			     ((uchar3 **)src_data)[y][x].at[2]) / 3;
	    else
	    if (src_type == Bit1)
	      e->pen.gray = 255 * ((bit1 **)src_data)[y][x];
	    else
	      e->pen.gray = ((uchar **)src_data)[y][x];

	    pen__draw(dest_data,x+x1,y+y1,1);
	  }
	else
	if (type == UChar3)
	  {
	    if (src_type == UChar)
	      {
		e->pen.color.at[0] = ((uchar **)src_data)[y][x];
		e->pen.color.at[1] = ((uchar **)src_data)[y][x];
		e->pen.color.at[2] = ((uchar **)src_data)[y][x];
	      }
	    else
	    if (src_type == Bit1)
	      {
		e->pen.color.at[0] = 255 * ((bit1 **)src_data)[y][x];
		e->pen.color.at[1] = 255 * ((bit1 **)src_data)[y][x];
		e->pen.color.at[2] = 255 * ((bit1 **)src_data)[y][x];
	      }
	    else
	      e->pen.color = ((uchar3 **)src_data)[y][x];
	    pen__draw(dest_data,x+x1,y+y1,1);
	  }
    }

  e->pen.binary = binary;
  e->pen.gray = gray;
  e->pen.color = color;
}



static void zoom_area(Edit e,long x1,long y1,long x2,long y2)
{
  long x,y,xsize,ysize;

  x = minimum(x1,x2);
  y = minimum(y1,y2);
  xsize = abs(x1 - x2) + 1;
  ysize = abs(y1 - y2) + 1;

  view__draw_zoom(e);
  Edit__set_region(e,x,y,xsize,ysize);
  view__draw_zoom(e);
}



static void win_d_point(Edit e)
{
  if (e->pen.shape == 0)
    {
      XDrawRectangle(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
		     e->px1,e->py1,e->px2-e->px1,e->py2-e->py1);
    }
  else
    {
      XDrawArc(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
	       e->px1,e->py1,e->px2-e->px1,e->py2-e->py1,0,360*64);
    }
  return;
}



static void win_d_points(Edit e)
{
  return;
}



static void win_d_segment(Edit e)
{
  long offset = edit__scale(e)/2;
  XDrawLine(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
	    e->px1+offset,e->py1+offset,e->px2+offset,e->py2+offset);
  XDrawLine(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
	    e->vx1,e->vy1,e->vx2,e->vy2);
}



static void win_d_lines(Edit e)
{
  if ((e->polygon_n < 3) ||
      (e->pp_point[0].x != e->pp_point[e->polygon_n].x) ||
      (e->pp_point[0].y != e->pp_point[e->polygon_n].y))
    {
      XDrawLines(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
		 e->pp_point,e->polygon_n+1,CoordModeOrigin);
      XDrawLines(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
		 e->pv_point,e->polygon_n+1,CoordModeOrigin);
    }
  else
    {
      XDrawLines(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
		 e->pp_point,e->polygon_n,CoordModeOrigin);
      XDrawLines(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
		 e->pv_point,e->polygon_n,CoordModeOrigin);
      XDrawLines(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
		 e->pp_point+e->polygon_n-1,1,CoordModeOrigin);
      XDrawLines(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
		 e->pv_point+e->polygon_n-1,1,CoordModeOrigin);
    }
}



static void win_d_box(Edit e)
{
  long x,y,w,h;
  long offset = edit__scale(e) / 2;

  x = minimum(e->px1,e->px2) + offset;
  y = minimum(e->py1,e->py2) + offset;
  w = abs(e->px1 - e->px2);
  h = abs(e->py1 - e->py2);

  XDrawRectangle(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),x,y,w,h);

  x = minimum(e->vx1,e->vx2);
  y = minimum(e->vy1,e->vy2);
  w = abs(e->vx1 - e->vx2);
  h = abs(e->vy1 - e->vy2);

  XDrawRectangle(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),x,y,w,h);
}



static void win_d_poly(Edit e)
{
  XDrawLines(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
	     e->pp_point,e->polygon_n+2,CoordModeOrigin);
  XDrawLines(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
	     e->pv_point,e->polygon_n+2,CoordModeOrigin);
}



static void win_d_circle1(Edit e)
{
  long r,r1,r2;
  long offset = edit__scale(e)/2;

  r1 = abs(e->px1 - e->px2);
  r2 = abs(e->py1 - e->py2);
  r = minimum(r1,r2);

  XDrawArc(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
	   e->px1-r+offset,e->py1-r+offset,2*r,2*r,0,360*64);
  if (r1 == r)
    {
      XDrawLine(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
		e->px1,e->py1,e->px2,e->py1);
      XDrawLine(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
		e->px2,e->py1,e->px2,e->py2);
    }
  else
    {
      XDrawLine(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
		e->px1,e->py1,e->px1,e->py2);
      XDrawLine(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
		e->px1,e->py2,e->px2,e->py2);
    }

  r1 = abs(e->vx1 - e->vx2);
  r2 = abs(e->vy1 - e->vy2);
  r = minimum(r1,r2);

  XDrawArc(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
	   e->vx1-r,e->vy1-r,2*r,2*r,0,360*64);

  if (r1 == r)
    {
      XDrawLine(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
		e->vx1,e->vy1,e->vx2,e->vy1);
      XDrawLine(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
		e->vx2,e->vy1,e->vx2,e->vy2);
    }
  else
    {
      XDrawLine(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
		e->vx1,e->vy1,e->vx1,e->vy2);
      XDrawLine(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
		e->vx1,e->vy2,e->vx2,e->vy2);
    }
}



static void win_d_circle2(Edit e)
{
  long r,r1,r2;
  long offset = edit__scale(e)/2;

  r1 = abs(e->px1 - e->px2);
  r2 = abs(e->py1 - e->py2);
  r = sqrt((double)(r1*r1 + r2*r2));

  XDrawArc(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
	   e->px1-r+offset,e->py1-r+offset,2*r,2*r,0,360*64);

  r1 = abs(e->vx1 - e->vx2);
  r2 = abs(e->vy1 - e->vy2);
  r = sqrt((double)(r1*r1 + r2*r2));

  XDrawArc(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
	   e->vx1-r,e->vy1-r,2*r,2*r,0,360*64);

  win_d_segment(e);
}



static void win_d_circle3(Edit e)
{
  long r,c1,c2,r1,r2;
  long offset = edit__scale(e)/2;

  c1 = (e->px1 + e->px2) / 2;
  c2 = (e->py1 + e->py2) / 2;
  r1 = abs(e->px1 - c1);
  r2 = abs(e->py1 - c2);
  r = sqrt((double)(r1*r1 + r2*r2));

  XDrawArc(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
	   c1-r+offset,c2-r+offset,2*r,2*r,0,360*64);

  c1 = (e->vx1 + e->vx2) / 2;
  c2 = (e->vy1 + e->vy2) / 2;
  r1 = abs(e->vx1 - c1);
  r2 = abs(e->vy1 - c2);
  r = sqrt((double)(r1*r1 + r2*r2));

  XDrawArc(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
	   c1-r,c2-r,2*r,2*r,0,360*64);

  win_d_box(e);
  win_d_segment(e);
}



static void win_d_circle4(Edit e)
{
  long r2,x,y;
  long offset = edit__scale(e)/2;

  r2 = minimum(abs(e->px1 - e->px2),abs(e->py1 - e->py2));
  if (e->px1 < e->px2) x = e->px1; else x = e->px1 - r2;
  if (e->py1 < e->py2) y = e->py1; else y = e->py1 - r2;

  XDrawArc(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
	   x+offset,y+offset,r2,r2,0,360*64);

  r2 = minimum(abs(e->vx1 - e->vx2),abs(e->vy1 - e->vy2));
  if (e->vx1 < e->vx2) x = e->vx1; else x = e->vx1 - r2;
  if (e->vy1 < e->vy2) y = e->vy1; else y = e->vy1 - r2;

  XDrawArc(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
	   x,y,r2,r2,0,360*64);

  win_d_box(e);
}



static void win_d_ellipse1(Edit e)
{
  long r1,r2;
  long offset = edit__scale(e)/2;

  r1 = abs(e->px1 - e->px2);
  r2 = abs(e->py1 - e->py2);

  XDrawArc(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
	   e->px1-r1+offset,e->py1-r2+offset,
	   2*r1,2*r2,0,360*64);

  r1 = abs(e->vx1 - e->vx2);
  r2 = abs(e->vy1 - e->vy2);

  XDrawArc(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
	   e->vx1-r1,e->vy1-r2,2*r1,2*r2,0,360*64);

  win_d_box(e);
}



static void win_d_ellipse2(Edit e)
{
  long x,y,r1,r2;
  long offset = edit__scale(e)/2;

  x = minimum(e->px1,e->px2);
  y = minimum(e->py1,e->py2);
  r1 = abs(e->px1 - e->px2);
  r2 = abs(e->py1 - e->py2);

  XDrawArc(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
	   x+offset,y+offset,r1,r2,0,360*64);

  x = minimum(e->vx1,e->vx2);
  y = minimum(e->vy1,e->vy2);
  r1 = abs(e->vx1 - e->vx2);
  r2 = abs(e->vy1 - e->vy2);

  XDrawArc(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
	   x,y,r1,r2,0,360*64);

  win_d_box(e);
}



static void win_f_poly(Edit e)
{
  XDrawLines(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
	     e->pp_point,e->polygon_n+2,CoordModeOrigin);
  XDrawLines(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
	     e->pv_point,e->polygon_n+2,CoordModeOrigin);
}



static void win_f_box(Edit e)
{
  long x,y,w,h;

  x = minimum(e->px1,e->px2);
  y = minimum(e->py1,e->py2);
  w = abs(e->px1 - e->px2)+edit__scale(e);
  h = abs(e->py1 - e->py2)+edit__scale(e);

  XDrawRectangle(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),x,y,w,h);
  XDrawLine(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),x,y,x+w-1,y+h-1);
  XDrawLine(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),x+w-1,y,x,y+h-1);

  x = minimum(e->vx1,e->vx2);
  y = minimum(e->vy1,e->vy2);
  w = abs(e->vx1 - e->vx2);
  h = abs(e->vy1 - e->vy2);

  XDrawRectangle(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),x,y,w,h);
  XDrawLine(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),x,y,x+w-1,y+h-1);
  XDrawLine(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),x+w-1,y,x,y+h-1);
}



static void win_f_circle1(Edit e)
{
  long r,r1,r2;
  long offset = edit__scale(e)/2;

  r1 = abs(e->px1 - e->px2);
  r2 = abs(e->py1 - e->py2);
  r = minimum(r1,r2);

  XDrawArc(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
	   e->px1-r+offset,e->py1-r+offset,2*r,2*r,0,360*64);
  XDrawArc(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
	   e->px1-r/2+offset,e->py1-r/2+offset,r,r,0,360*64);
  if (r1 == r)
    {
      XDrawLine(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
		e->px1,e->py1,e->px2,e->py1);
      XDrawLine(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
		e->px2,e->py1,e->px2,e->py2);
    }
  else
    {
      XDrawLine(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
		e->px1,e->py1,e->px1,e->py2);
      XDrawLine(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
		e->px1,e->py2,e->px2,e->py2);
    }

  r1 = abs(e->vx1 - e->vx2);
  r2 = abs(e->vy1 - e->vy2);
  r = minimum(r1,r2);

  XDrawArc(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
	   e->vx1-r,e->vy1-r,2*r,2*r,0,360*64);
  XDrawArc(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
	   e->vx1-r/2,e->vy1-r/2,r,r,0,360*64);

  if (r1 == r)
    {
      XDrawLine(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
		e->vx1,e->vy1,e->vx2,e->vy1);
      XDrawLine(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
		e->vx2,e->vy1,e->vx2,e->vy2);
    }
  else
    {
      XDrawLine(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
		e->vx1,e->vy1,e->vx1,e->vy2);
      XDrawLine(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
		e->vx1,e->vy2,e->vx2,e->vy2);
    }
}



static void win_f_circle2(Edit e)
{
  long r,r1,r2;
  long offset = edit__scale(e)/2;

  r1 = abs(e->px1 - e->px2);
  r2 = abs(e->py1 - e->py2);
  r = sqrt((double)(r1*r1 + r2*r2));

  XDrawArc(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
	   e->px1-r+offset,e->py1-r+offset,2*r,2*r,0,360*64);
  XDrawArc(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
	   e->px1-r/2+offset,e->py1-r/2+offset,r,r,0,360*64);

  r1 = abs(e->vx1 - e->vx2);
  r2 = abs(e->vy1 - e->vy2);
  r = sqrt((double)(r1*r1 + r2*r2));

  XDrawArc(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
	   e->vx1-r,e->vy1-r,2*r,2*r,0,360*64);
  XDrawArc(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
	   e->vx1-r/2,e->vy1-r/2,r,r,0,360*64);

  win_d_segment(e);
}



static void win_f_circle3(Edit e)
{
  long r,c1,c2,r1,r2;
  long offset = edit__scale(e)/2;

  c1 = (e->px1 + e->px2) / 2;
  c2 = (e->py1 + e->py2) / 2;
  r1 = abs(e->px1 - c1);
  r2 = abs(e->py1 - c2);
  r = sqrt((double)(r1*r1 + r2*r2));

  XDrawArc(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
	   c1-r+offset,c2-r+offset,2*r,2*r,0,360*64);
  XDrawArc(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
	   c1-r/2+offset,c2-r/2+offset,r,r,0,360*64);

  c1 = (e->vx1 + e->vx2) / 2;
  c2 = (e->vy1 + e->vy2) / 2;
  r1 = abs(e->vx1 - c1);
  r2 = abs(e->vy1 - c2);
  r = sqrt((double)(r1*r1 + r2*r2));

  XDrawArc(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
	   c1-r,c2-r,2*r,2*r,0,360*64);
  XDrawArc(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
	   c1-r/2,c2-r/2,r,r,0,360*64);

  win_d_segment(e);
  win_d_box(e);
}



static void win_f_circle4(Edit e)
{
  long r2,x,y;
  long offset = edit__scale(e)/2;

  r2 = minimum(abs(e->px1 - e->px2),abs(e->py1 - e->py2));
  if (e->px1 < e->px2) x = e->px1; else x = e->px1 - r2;
  if (e->py1 < e->py2) y = e->py1; else y = e->py1 - r2;

  XDrawArc(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
	   x+offset,y+offset,r2,r2,0,360*64);
  XDrawArc(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
	   x+offset+r2/4,y+offset+r2/4,r2/2,r2/2,0,360*64);

  r2 = minimum(abs(e->vx1 - e->vx2),abs(e->vy1 - e->vy2));
  if (e->vx1 < e->vx2) x = e->vx1; else x = e->vx1 - r2;
  if (e->vy1 < e->vy2) y = e->vy1; else y = e->vy1 - r2;

  XDrawArc(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
	   x,y,r2,r2,0,360*64);
  XDrawArc(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
	   x+r2/4,y+r2/4,r2/2,r2/2,0,360*64);

  win_d_box(e);
}



static void win_f_ellipse1(Edit e)
{
  long r1,r2;
  long offset = edit__scale(e)/2;

  r1 = abs(e->px1 - e->px2);
  r2 = abs(e->py1 - e->py2);

  XDrawArc(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
	   e->px1-r1+offset,e->py1-r2+offset,
	   2*r1,2*r2,0,360*64);
  XDrawArc(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
	   e->px1-r1/2+offset,e->py1-r2/2+offset,
	   r1,r2,0,360*64);

  r1 = abs(e->vx1 - e->vx2);
  r2 = abs(e->vy1 - e->vy2);

  XDrawArc(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
	   e->vx1-r1,e->vy1-r2,2*r1,2*r2,0,360*64);
  XDrawArc(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
	   e->vx1-r1/2,e->vy1-r2/2,r1,r2,0,360*64);

  win_d_box(e);
}



static void win_f_ellipse2(Edit e)
{
  long x,y;
  long r1,r2;
  long offset = edit__scale(e)/2;

  x = minimum(e->px1,e->px2);
  y = minimum(e->py1,e->py2);
  r1 = abs(e->px1 - e->px2);
  r2 = abs(e->py1 - e->py2);

  XDrawArc(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
	   x+offset,y+offset,r1,r2,0,360*64);
  XDrawArc(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
	   x+offset+r1/4,y+offset+r2/4,r1/2,r2/2,0,360*64);

  x = minimum(e->vx1,e->vx2);
  y = minimum(e->vy1,e->vy2);
  r1 = abs(e->vx1 - e->vx2);
  r2 = abs(e->vy1 - e->vy2);

  XDrawArc(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
	   x,y,r1,r2,0,360*64);
  XDrawArc(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
	   x+r1/4,y+r2/4,r1/2,r2/2,0,360*64);

  win_d_box(e);
}



static void win_store_area(Edit e)
{
  win_f_box(e);
}



static void win_restore_area(Edit e)
{
  win_f_box(e);
}



static void win_zoom_area(Edit e)
{
  win_f_box(e);
}
