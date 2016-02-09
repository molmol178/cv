/*
 * view.c
 */



#include "Edit.h"




static xw_event view_events[] = {
  {Expose,(xw_handler)view__expose},
  {MotionNotify,(xw_handler)view_motion},
  {ButtonPress,(xw_handler)view_press},
  {EnterNotify,(xw_handler)view_enter},
  {LeaveNotify,(xw_handler)view_leave}
};

static xw_event view_key_events[] = {
  {'r',(xw_handler)view__expose},
  {'o',(xw_handler)view_osize}
};



void view__open(Edit e)
{
  long xsize,ysize;

  xsize = e->view_scale * Edit__image_xsize(e);
  ysize = e->view_scale * Edit__image_ysize(e);
  if ((xsize > 256) || (ysize > 256))
    {
      xsize = 256;
      ysize = xsize * Edit__image_ysize(e) / Edit__image_xsize(e);
    }
  e->view = XwObject.create(e->world,XwHard.class,0,"ViewImage");
  XwObject.resize(e->view,xsize,ysize);
  XwObject.addEvents(e->view,Number(view_events),view_events);
  XwObject.addKeyEvents(e->view,Number(view_key_events),view_key_events);
  XwObject.map(e->view);
}



long view_press(Edit e,xw_object obj,XEvent *event)
{
  long x,y,xsize,ysize;
  long px,py;

  px = e->x;
  py = e->y;

  xsize = Edit__image_xsize(e);
  ysize = Edit__image_ysize(e);
  x = xsize * event->xbutton.x / XwObject.width(obj);
  y = ysize * event->xbutton.y / XwObject.height(obj);

  switch (event->xbutton.button)
    {
    case Button1:
      {
	switch (e->zoom.state)
	  {
	  case 0:
	    view__draw_zoom(e);
	    e->zoom.state = 1;
	    e->zoom.x1 = x;
	    e->zoom.y1 = y;
	    e->zoom.x2 = x;
	    e->zoom.y2 = y;
	    view__draw_move_zoom(e);
	    break;
	  case 1:
	    view__draw_move_zoom(e);
	    e->zoom.state = 0;
	    e->zoom.x2 = x;
	    e->zoom.y2 = y;
	    e->x = minimum(e->zoom.x1,x);
	    e->y = minimum(e->zoom.y1,y);
	    e->xsize = abs(e->zoom.x1 - x + 1);
	    e->ysize = abs(e->zoom.y1 - y + 1);
	    panel__expose(e);
	    view__draw_zoom(e);
	    break;
	  }
      }
      break;

    case Button3:
      if (e->zoom.state == 1)
	{
	  view__draw_move_zoom(e);
	  view__draw_zoom(e);
	  e->zoom.state = 0;
	  break;
	}
      view__draw_zoom(e);
      e->x = x - e->xsize / 2;
      e->y = y - e->ysize / 2;
      if (e->x < 0) e->x = 0;
      if (e->y < 0) e->y = 0;
      if (e->x + e->xsize > xsize) e->x = xsize - e->xsize;
      if (e->y + e->ysize > ysize) e->y = ysize - e->ysize;
      if ((px != e->x) || (py != e->y)) panel__expose(e);
      view__draw_zoom(e);
      break;


    case Button2:
      if (e->zoom.state == 1)
	{
	  view__draw_move_zoom(e);
	  view__draw_zoom(e);
	  e->zoom.state = 0;
	  break;
	}
      pen__get_color(e,x,y);
      break;
    }

  panel_coord_draw(e,x,y);

  return 0;
}



long view_motion(Edit e,xw_object obj,XEvent *event)
{
  long x,y,xsize,ysize;

  xsize = Edit__image_xsize(e);
  ysize = Edit__image_ysize(e);
  x = xsize * event->xbutton.x / XwObject.width(obj);
  y = ysize * event->xbutton.y / XwObject.height(obj);

  if (e->zoom.state == 1)
    {
      if ((e->zoom.x2 != x) || (e->zoom.y2 != y))
	{
	  view__draw_move_zoom(e);
	  e->zoom.x2 = x;
	  e->zoom.y2 = y;
	  view__draw_move_zoom(e);
	}
    }

  if (e->coord_on) panel_coord_draw(e,x,y);

  return 0;
}



void view_resize(Edit e)
{
  long xsize,ysize;

  xsize = Edit__image_xsize(e);
  ysize = Edit__image_ysize(e);

  if ((xsize != e->view->width) || (ysize != e->view->height))
    XwObject.resize(e->view,xsize,ysize);
  else
    {
      view__draw_zoom(e);
      XShowImageFull(Edit__display(e),Edit__viewID(e),
		     Edit__gc(e),Edit__image(e));
      view__draw_zoom(e);
    }
}



long view__expose(Edit e)
{
  long xsize,ysize;

  XwObject.getGeometry(e->view,0,0,&xsize,&ysize);

  view__draw_zoom(e);
  ysize = xsize * Edit__image_ysize(e) / Edit__image_xsize(e);
  if (ysize != e->view->height)
    {
      if (xsize <= 10 || ysize <= 10)
	{
	  xsize = 10;
	  ysize = xsize * Edit__image_ysize(e) / Edit__image_xsize(e);
	  if (ysize > 800)
	    {
	      ysize = 10;
	      xsize = ysize * Edit__image_xsize(e) / Edit__image_ysize(e);
	    }
	}
      if (ysize > 800)
	{
	  ysize = 800;
	  xsize = ysize * Edit__image_xsize(e) / Edit__image_ysize(e);
	}
      XwObject.resize(e->view,xsize,ysize);
      return 0;
    }
  XShowImageFull(Edit__display(e),Edit__viewID(e),Edit__gc(e),
		 Edit__image(e));
  view__draw_zoom(e);
  return 0;
}



long view_osize(Edit e)
{
  XwObject.resize(e->view,Edit__image_xsize(e),Edit__image_ysize(e));
  return 0;
}



#define RR 3
void view_draw(Edit e,long x1,long y1,long x2,long y2)
{
  long sx,sy,ex,ey,dx,dy;
  long wx,wy,wsx,wsy;
  long xsize,ysize;
  long wxsize,wysize;

  if (x1 > x2) {long tmp = x1; x1 = x2; x2 = tmp;}
  if (y1 > y2) {long tmp = y1; y1 = y2; y2 = tmp;}

  sx = x1;
  sy = y1;
  ex = x2;
  ey = y2;
  if (Edit__image_type(e) != Bit1)
    {
      sx -= RR;
      sy -= RR;
      ex += RR;
      ey += RR;
    }

  xsize = Edit__image_xsize(e);
  ysize = Edit__image_ysize(e);
  wxsize = XwObject.width(e->view);
  wysize = XwObject.height(e->view);

  if (sx < 0) sx = 0;
  if (sy < 0) sy = 0;
  if (ex >= xsize) ex = xsize - 1;
  if (ey >= ysize) ey = ysize - 1;

  dx = ex - sx + 1;
  dy = ey - sy + 1;

  wx  = wxsize * sx / xsize;
  wy  = wysize * sy / ysize;
  wsx = wxsize * dx / xsize;
  wsy = wysize * dy / ysize;

  view__draw_zoom(e);
  XShowImage(XwWorld.display(e->world),XwObject.id(e->view),
	     XwWorld.gc(e->world),Edit__image(e),
	     wx,wy,wsx,wsy,sx,sy,dx,dy);
  view__draw_zoom(e);
}



long view_enter(Edit e)
{
  return 0;
}



long view_leave(Edit e)
{
  if (e->zoom.state == 1) {view__draw_move_zoom(e);view__draw_zoom(e);}
  e->zoom.state = 0;
  return 0;
}



void view__draw_zoom(Edit e)
{
  long x,y,w,h;

  x = e->view->width * e->x / Edit__image_xsize(e);
  y = e->view->height * e->y / Edit__image_ysize(e);
  w = e->view->width * e->xsize / Edit__image_xsize(e);
  h = e->view->height * e->ysize / Edit__image_ysize(e);

  XDrawRectangle(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
		 x-1,y-1,w+1,h+1);
}



void view__draw_move_zoom(Edit e)
{
  long x,y,xsize,ysize,w,h;

  x = minimum(e->zoom.x1,e->zoom.x2);
  y = minimum(e->zoom.y1,e->zoom.y2);
  xsize = abs(e->zoom.x1 - e->zoom.x2 + 1);
  ysize = abs(e->zoom.y1 - e->zoom.y2 + 1);

  x = e->view->width * x / Edit__image_xsize(e);
  y = e->view->height * y / Edit__image_ysize(e);
  w = e->view->width * xsize / Edit__image_xsize(e);
  h = e->view->height * ysize / Edit__image_ysize(e);

  XDrawRectangle(Edit__display(e),Edit__viewID(e),Edit__xorGC(e),
		 x-1,y-1,w+1,h+1);
}
