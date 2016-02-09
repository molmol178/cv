/*
 * panel.c
 */



#include "Edit.h"



void draw_object(Edit e);
void panel_object_on(Edit e);
void panel_object_off(Edit e);
void normal_pen(Edit e);
void reverse_pen(Edit e);
void panel_set_firstpoint(Edit e,long x,long y);
void panel_set_secondpoint(Edit e,long x,long y);
void panel_coord_draw(Edit e,long x,long y);
void gradation_draw(Edit e);
void diffusion_draw(Edit e);
void store_mode_draw(Edit e);

void polygon_set_first(Edit e,long x,long y);
void polygon_set_last(Edit e,long x,long y);
void polygon_add(Edit e,long x,long y);
void polygon_delete_all(Edit e);
void polygon_delete_last(Edit e);



long panel__translate_coord(Edit e,XEvent *event,long *xp,long *yp);



static xw_event panel_events[] = {
  {Expose,(xw_handler)panel__expose},
  {MotionNotify,(xw_handler)panel_motion},
  {ButtonPress,(xw_handler)panel_press},
  {EnterNotify,(xw_handler)panel_enter},
  {LeaveNotify,(xw_handler)panel_leave}
};

static xw_event panel_key_events[] = {
  {'r',(xw_handler)panel__expose}
};



void panel__open(Edit e)
{
  e->panel = XwObject.create(e->world,XwHard.class,0,"Edit & Rupe");
  if (e->xsize > 1024) e->xsize = 1024;
  if (e->ysize > 800) e->ysize = 800;
  XwObject.resize(e->panel,e->xsize*edit__scale(e),e->ysize*edit__scale(e));
  XwObject.addEvents(e->panel,Number(panel_events),panel_events);
  XwObject.addKeyEvents(e->panel,Number(panel_key_events),panel_key_events);
  XwObject.map(e->panel);
}



long panel__expose(Edit e)
{
  if (panel__reconstruct(e) == 0)
    return 0;

  XShowImagePartOfImage(XwWorld.display(e->world),XwObject.id(e->panel),
			XwWorld.gc(e->world),Edit__image(e),
			e->x,e->y,e->xsize,e->ysize);

  if (draw__on(e))
    {
      draw__on(e) = 0;
      panel_object_on(e);
    }

  if (e->edit.grid_on)
    {
      edit__draw_grid(e);
    }

  return 0;
}



long panel_motion(Edit e,xw_object obj,XEvent *event)
{
  long x,y;
  static long px = -1,py = -1;

  if (panel__translate_coord(e,event,&x,&y) == 0) return 0;

  if ((x == e->x2) && (y == e->y2)) return 0;
  if ((px == x) && (py == y)) return 0;

  px = x;
  py = y;

  if (draw__func(e) == D_POINT)
    {
      x -= pen__size(e) / 2;
      y -= pen__size(e) / 2;
      if (draw__on(e)) panel_object_off(e);
      panel_set_firstpoint(e,x,y);
      panel_set_secondpoint(e,x+pen__size(e),y+pen__size(e));
      panel_object_on(e);
    }
  else
  if (draw__func(e) == RESTORE_AREA)
    {
      image buf = imageItem__image(buffer__item_current(e));
      if (Image.area(buf) != 0)
	{
	  if (draw__on(e)) panel_object_off(e);
	  panel_set_firstpoint(e,x,y);
	  panel_set_secondpoint(e,x+Image.xsize(buf)-1,y+Image.ysize(buf)-1);
	  panel_object_on(e);
	}
    }
  else
  if (draw__func(e) == D_POINTS && draw__state(e) == 1)
    {
      draw__func(e) = D_SEGMENT;
      panel_set_secondpoint(e,x,y);
      draw_object(e);
      panel_set_firstpoint(e,x,y);
      draw__func(e) = D_POINTS;
    }
  else
  if (((draw__func(e) == D_POLY)||(draw__func(e) == F_POLY)||
       (draw__func(e) == D_LINES)) &&
      draw__state(e) == 1)
    {
      panel_object_off(e);
      polygon_set_last(e,x,y);
      panel_set_secondpoint(e,x,y);
      panel_object_on(e);
    }
  else
  if (draw__state(e) == 1)
    {
      panel_object_off(e);
      panel_set_secondpoint(e,x,y);
      panel_object_on(e);
    }

  if (e->coord_on) panel_coord_draw(e,x,y);

  return 0;
}



long panel_press(Edit e,xw_object obj,XEvent *event)
{
  long x,y;

  if (panel__translate_coord(e,event,&x,&y) == 0) return 0;

  switch (event->xbutton.button)
    {
    case Button1:
      switch (draw__state(e))
	{
	case 0:
	  panel_object_off(e);
	  panel_set_firstpoint(e,x,y);
	  Edit__drawn_clear(e);
	  Edit__backup(e);
	  normal_pen(e);

	  switch (draw__func(e))
	    {
	    case D_POINT:
	    case RESTORE_AREA:
	      draw_object(e);
	      break;

	    case D_LINES:
	    case F_POLY:
	    case D_POLY:
	      draw__state(e) = 1;
	      polygon_set_first(e,x,y);
	      break;

	    default:
	      draw__state(e) = 1;
	    }
	  break;
	case 1:
	  switch (draw__func(e))
	    {
	    case D_POINTS:
	      draw__state(e) = 0;
	      normal_pen(e);
	      break;

	    case D_LINES:
	    case F_POLY:
	    case D_POLY:
	      panel_set_secondpoint(e,x,y);
	      polygon_add(e,x,y);
	      break;

	    default:
	      draw__state(e) = 0;
	      panel_object_off(e);
	      panel_set_secondpoint(e,x,y);
	      draw_object(e);
	      normal_pen(e);
	    }
	  break;
	}
      break;

    case Button2:
      if ((((draw__func(e) == F_POLY)||(draw__func(e) == D_POLY)||
	    (draw__func(e) == D_LINES)) &&
	  (draw__state(e) == 1)))
	{
	  draw__state(e) = 0;
	  panel_object_off(e);
	  polygon_set_last(e,x,y);
	  draw_object(e);
	  normal_pen(e);
	}
      else
	pen__get_color(e,x,y);

      break;

    case Button3:
      if (draw__func(e) == RESTORE_AREA) break;

      if (draw__state(e) == 0)
	{
	  panel_object_off(e);
	  panel_set_firstpoint(e,x,y);
	  Edit__drawn_clear(e);
	  Edit__backup(e);
	  reverse_pen(e);

	  switch (draw__func(e))
	    {
	    case D_POINT:
	      draw_object(e);
	      break;

	    case F_POLY:
	    case D_POLY:
	    case D_LINES:
	      draw__state(e) = 1;
	      polygon_set_first(e,x,y);
	      break;

	    default:
	      draw__state(e) = 1;
	    }
	}
      else
	{
	  if ((draw__func(e) == D_POLY)||(draw__func(e) == F_POLY)||
	      (draw__func(e) == D_LINES))
	    {
	      panel_object_off(e);
	      polygon_delete_last(e);
	      if (e->polygon_n == 0)
		draw__state(e) = 0;
	      else
		panel_object_on(e);
	    }
	  else
	    {
	      draw__state(e) = 0;
	      panel_object_off(e);
	    }
	}
      break;
    }

  panel_coord_draw(e,x,y);

  return 0;
}



long panel_enter(Edit e)
{
  if (draw__state(e) == 1)
    panel_object_on(e);
  return 0;
}



long panel_leave(Edit e)
{
  if ((draw__state(e) == 1) ||
      (draw__func(e) == D_POINT) ||
      (draw__func(e) == RESTORE_AREA))
    panel_object_off(e);
  if ((draw__func(e) == F_POLY)||(draw__func(e) == D_POLY)||
      (draw__func(e) == D_LINES))
    polygon_delete_all(e);
  draw__state(e) = 0;
  return 0;
}



#define RP 1
void panel_draw(Edit e,long x1,long y1,long x2,long y2)
{
  long sx,sy,ex,ey,dx,dy;
  long wx,wy,wsx,wsy;

  if (x1 > x2) {long tmp = x1; x1 = x2; x2 = tmp;}
  if (y1 > y2) {long tmp = y1; y1 = y2; y2 = tmp;}

  sx = x1;
  sy = y1;
  ex = x2;
  ey = y2;
  if (Edit__image_type(e) != Bit1)
    {
      sx -= RP;
      sy -= RP;
      ex += RP;
      ey += RP;
    }

  if (sx < e->x) sx = e->x;
  if (sy < e->y) sy = e->y;
  if (ex >= (e->x + e->xsize)) ex = e->x + e->xsize - 1;
  if (ey >= (e->y + e->ysize)) ey = e->y + e->ysize - 1;

  dx = ex - sx + 1;
  dy = ey - sy + 1;

  wx  = edit__scale(e) * (sx - e->x);
  wy  = edit__scale(e) * (sy - e->y);
  wsx = edit__scale(e) * dx;
  wsy = edit__scale(e) * dy;
  XShowImage(XwWorld.display(e->world),XwObject.id(e->panel),
	     XwWorld.gc(e->world),Edit__image(e),
	     wx,wy,wsx,wsy,sx,sy,dx,dy);
  if (edit__grid_on(e)) edit__draw_grid_area(e,sx,sy,ex,ey);
}



void draw_object(Edit e)
{
  long size = 1;

  e->quit_flag = 0;

  imageItem__set_edit(Edit__original(e));

  if ((pen__store_mode(e)) && (draw__func(e) < STORE_AREA))
    {
      store_mode_draw(e);
    }
  else
  if ((pen__func(e) == FUNC_DIFF) && (Edit__image_type(e) != Bit1) &&
      (draw__func(e) < STORE_AREA))
    {
      diffusion_draw(e);
    }
  else
  if (pen__gradation(e) && (Edit__image_type(e) != Bit1) &&
      (draw__func(e) < STORE_AREA))
    {
      gradation_draw(e);
    }
  else
  if ((draw__func(e) >= F_BOX) && (draw__func(e) <= F_ELLIPSE2))
    {
      Image.swap(e->pen.pat,e->pen.fill);
      size = pen__size(e);
      pen__size(e) = 1;
      DrawObjectList[draw__func(e)](e,e->x1,e->y1,e->x2,e->y2);
      Image.swap(e->pen.pat,e->pen.fill);
      pen__size(e) = size;
    }
  else
    {
      DrawObjectList[draw__func(e)](e,e->x1,e->y1,e->x2,e->y2);
    }


  {
    register bit1 **data;
    register int x,y;
    long do_draw = 0;

    data = (bit1 **)Image.data(e->drawn);

    for (y = e->miny; y <= e->maxy; y++)
      for (x = e->minx; x <= e->maxx; x++)
	if (data[y][x] == 1)
	  {
	    data[y][x] = 2;
	    do_draw = 1;
	  }
    if (do_draw)
      {
	panel_draw(e,e->minx,e->miny,e->maxx,e->maxy);
	view_draw(e,e->minx,e->miny,e->maxx,e->maxy);
      }
    e->maxx = e->maxy = 0;
    e->minx = Edit__image_xsize(e);
    e->miny = Edit__image_ysize(e);
  }
}



void panel_object_on(Edit e)
{
  if (draw__on(e) == 1) return;
  if ((e->x1 == e->x2) && (e->y1 == e->y2)) return;
  WinDrawObjectList[draw__func(e)](e);
  draw__on(e) = 1;
}



void panel_object_off(Edit e)
{
  if (draw__on(e) == 0) return;
  if ((e->x1 == e->x2) && (e->y1 == e->y2)) return;
  WinDrawObjectList[draw__func(e)](e);
  draw__on(e) = 0;
}



void panel_set_firstpoint(Edit e,long x,long y)
{
  e->x2 = e->x1 = x;
  e->y2 = e->y1 = y;
  e->px2 = e->px1 = (x - e->x) * e->panel->width  / e->xsize;
  e->py2 = e->py1 = (y - e->y) * e->panel->height / e->ysize;
  e->vx2 = e->vx1 = x * e->view->width  / Edit__image_xsize(e);
  e->vy2 = e->vy1 = y * e->view->height / Edit__image_ysize(e);
}



void panel_set_secondpoint(Edit e,long x,long y)
{
  e->x2 = x;
  e->y2 = y;
  e->px2 = (x - e->x) * e->panel->width  / e->xsize;
  e->py2 = (y - e->y) * e->panel->height / e->ysize;
  e->vx2 = x * e->view->width  / Edit__image_xsize(e);
  e->vy2 = y * e->view->height / Edit__image_ysize(e);
}



void panel_coord_draw(Edit e,long x,long y)
{
  char buf[512];

  if (x < 0 || y < 0 ||
      x >= Edit__image_xsize(e) || y >= Edit__image_ysize(e))
    return;

  if (Edit__image_type(e) == Bit1)
    {
      bit1 **data;
      data = (bit1 **)Image.data(Edit__image(e));
      sprintf(buf," I(%3d,%3d) = %d",x,y,data[y][x]);
    }
  else
  if (Edit__image_type(e) == UChar)
    {
      uchar **data;
      data = (uchar **)Image.data(Edit__image(e));
      sprintf(buf," I(%3d,%3d) = %3d",x,y,data[y][x]);
    }
  else
  if (Edit__image_type(e) == UChar3)
    {
      uchar3 **data;
      data = (uchar3 **)Image.data(Edit__image(e));
      sprintf(buf," I(%3d,%3d) = {%3d,%3d,%3d}",
	      x,y,data[y][x].at[0],data[y][x].at[1],data[y][x].at[2]);
    }
  XwObject.setLabel(e->message,buf);
}



void normal_pen(Edit e)
{
  if (e->pen.reverse == 0) return;
  e->pen.reverse = 0;
  e->pen.binary = 1 - e->pen.binary;
  e->pen.gray = 255 - e->pen.gray;
  e->pen.color.at[0] = 255 - e->pen.color.at[0];
  e->pen.color.at[1] = 255 - e->pen.color.at[1];
  e->pen.color.at[2] = 255 - e->pen.color.at[2];
}



void reverse_pen(Edit e)
{
  if (e->pen.reverse) return;
  e->pen.reverse = 1;
  e->pen.binary = 1 - e->pen.binary;
  e->pen.gray = 255 - e->pen.gray;
  e->pen.color.at[0] = 255 - e->pen.color.at[0];
  e->pen.color.at[1] = 255 - e->pen.color.at[1];
  e->pen.color.at[2] = 255 - e->pen.color.at[2];
}



void gradation_draw(Edit e)
{
  long func;
  long size;
  long type;
  long xsize,ysize;
  register bit1 **p;
  register long x,y,i;
  long width,height;
  register char **data;
  long maxx,maxy,minx,miny;
  uchar3 color = e->pen.color;
  uchar gray = e->pen.gray;;

  type  = Edit__image_type(e);
  xsize = Edit__image_xsize(e);
  ysize = Edit__image_ysize(e);

  Image.make(e->gradation,Bit1,xsize,ysize);
  if (Image.type(e->gradation) == 0)
    {
      XwObject.setLabel(e->message,"warning:gradation_draw:can't alloc memory.");
      return;
    }
  Image.swap(Edit__image(e),e->gradation);
  Image.clear(Edit__image(e));

  size = pen__size(e);

  if ((draw__func(e) >= F_BOX) && (draw__func(e) <= F_ELLIPSE2))
    {
      Image.swap(e->pen.pat,e->pen.fill);
      pen__size(e) = 1;
    }

  func = pen__func(e);
  pen__func(e) = 0;

  /* gradation area */
  DrawObjectList[draw__func(e)](e,e->x1,e->y1,e->x2,e->y2);

  if ((draw__func(e) >= F_BOX) && (draw__func(e) <= F_ELLIPSE2))
    {
      Image.swap(e->pen.pat,e->pen.fill);
      pen__size(e) = size;
    }

  maxx = e->maxx;
  maxy = e->maxy;
  minx = e->minx;
  miny = e->miny;
  Edit__drawn_clear_area(e,minx,miny,maxx,maxy);

  Image.swap(Edit__image(e),e->gradation);
  pen__func(e) = func;

  Image.swap(e->pen.pat,e->pen.fill);
  size = pen__size(e);
  pen__size(e) = 1;

  p = (bit1 **)Image.data(e->gradation);
  data = Image.data(Edit__image(e));
  width = maxx - minx;
  if (width <= 0) width = 1;
  height = maxy - miny;
  if (height <= 0) height = 1;
  if (type == UChar)
    {
      long start,end,range;

      start = e->pen.sh1.gray;
      end   = e->pen.sh2.gray;
      range = end - start;

      switch (e->pen.grad_dir)
	{
	case GRAD_Right:
	  for (y = miny; y <= maxy; y++)
	    for (x = minx; x <= maxx; x++)
	      if (p[y][x])
		{
		  e->pen.gray = range * (x - minx) / width + start;
		  pen__draw(data,0,x,y);
		}
	  break;
	case GRAD_Left:
	  for (y = miny; y <= maxy; y++)
	    for (x = minx; x <= maxx; x++)
	      if (p[y][x])
		{
		  e->pen.gray = range * (width - (x - minx)) / width + start;
		  pen__draw(data,0,x,y);
		}
	  break;
	case GRAD_Up:
	  for (y = miny; y <= maxy; y++)
	    for (x = minx; x <= maxx; x++)
	      if (p[y][x])
		{
		  e->pen.gray = range * (height - (y - miny)) / height + start;
		  pen__draw(data,0,x,y);
		}
	  break;
	case GRAD_Down:
	  for (y = miny; y <= maxy; y++)
	    for (x = minx; x <= maxx; x++)
	      if (p[y][x])
		{
		  e->pen.gray = range * (y - miny) / height + start;
		  pen__draw(data,0,x,y);
		}
	  break;
	}
    }
  else
    {
      long start[3],end[3],range[3];

      for (i = 0; i < 3; i++)
	{
	  start[i] = e->pen.sh1.color.at[i];
	  end[i]   = e->pen.sh2.color.at[i];
	  range[i] = end[i] - start[i];
	}

      switch(e->pen.grad_dir)
	{
	case GRAD_Right:
	  for (y = miny; y <= maxy; y++)
	    for (x = minx; x <= maxx; x++)
	      if (p[y][x])
		{
		  for (i = 0; i < 3; i++)
		    e->pen.color.at[i] = range[i] * (x-minx) / width + start[i];
		  pen__draw(data,0,x,y);
		}
	  break;
	case GRAD_Left:
	  for (y = miny; y <= maxy; y++)
	    for (x = minx; x <= maxx; x++)
	      if (p[y][x])
		{
		  for (i = 0; i < 3; i++)
		    e->pen.color.at[i] = range[i] * (width - (x-minx)) / width + start[i];
		  pen__draw(data,0,x,y);
		}
	  break;
	case GRAD_Up:
	  for (y = miny; y <= maxy; y++)
	    for (x = minx; x <= maxx; x++)
	      if (p[y][x])
		{
		  for (i = 0; i < 3; i++)
		    e->pen.color.at[i] = range[i] * (height - (y - miny)) / height + start[i];
		  pen__draw(data,0,x,y);
		}
	  break;
	case GRAD_Down:
	  for (y = miny; y <= maxy; y++)
	    for (x = minx; x <= maxx; x++)
	      if (p[y][x])
		{
		  for (i = 0; i < 3; i++)
		    e->pen.color.at[i] = range[i] * (y - miny) / height + start[i];
		  pen__draw(data,0,x,y);
		}
	  break;
	}
    }
  e->pen.color = color;
  e->pen.gray = gray;
  Image.swap(e->pen.pat,e->pen.fill);
  pen__size(e) = size;
  Image.free(e->gradation);
}



void diffusion_draw(Edit e)
{
  long func;
  long size;
  long type;
  long xsize,ysize;
  register bit1 **p;
  register long x,y;
  long width,height;
  register char **data;
  long maxx,maxy,minx,miny;
  uchar3 color = e->pen.color;
  uchar gray = e->pen.gray;

  long dxsize,dysize;
  double **w, n, strength;

  type  = Edit__image_type(e);
  xsize = Edit__image_xsize(e);
  ysize = Edit__image_ysize(e);

  Image.make(e->gradation,Bit1,xsize,ysize);
  Image.make(e->diffusion,type,xsize,ysize);
  if (Image.type(e->gradation) == 0)
    {
      XwObject.setLabel(e->message,"warning:diffusion_draw:can't alloc memory.");
      return;
    }
  Image.swap(Edit__image(e),e->gradation);
  Image.clear(Edit__image(e));

  size = pen__size(e);

  if ((draw__func(e) >= F_BOX) && (draw__func(e) <= F_ELLIPSE2))
    {
      Image.swap(e->pen.pat,e->pen.fill);
      pen__size(e) = 1;
    }

  func = pen__func(e);
  pen__func(e) = 0;

  /* gradation area */
  DrawObjectList[draw__func(e)](e,e->x1,e->y1,e->x2,e->y2);

  if ((draw__func(e) >= F_BOX) && (draw__func(e) <= F_ELLIPSE2))
    {
      Image.swap(e->pen.pat,e->pen.fill);
      pen__size(e) = size;
    }

  maxx = e->maxx;
  maxy = e->maxy;
  minx = e->minx;
  miny = e->miny;
  Edit__drawn_clear(e);

  Image.swap(Edit__image(e),e->gradation);

  Image.swap(e->pen.pat,e->pen.fill);
  size = pen__size(e);
  pen__size(e) = 1;

  p = (bit1 **)Image.data(e->gradation);
  data = Image.data(Edit__image(e));
  width = maxx - minx;
  if (width <= 0) width = 1;
  height = maxy - miny;
  if (height <= 0) height = 1;

  dxsize = e->pen.diff_xsize + 1;
  dysize = e->pen.diff_ysize + 1;
  w = (double **)memnew2(dxsize,dysize,sizeof(double));
  strength = (double)e->pen.diff_strength;
  n = 0;
  for (y = 0; y < dysize; y++)
    {
      int ry = abs(y - dysize / 2);
      for (x = 0; x < dxsize; x++)
	{
	  int rx = abs(x - dxsize / 2);
	  int d = 10 * (3 * maximum(rx,ry) + minimum(rx,ry));
	  w[y][x] = strength / (d + strength);
	  n += w[y][x];
	}
    }

  if (type == UChar)
    {
      uchar **tmp = (uchar **)Image.data(e->diffusion);
      for (y = miny; y <= maxy; y++)
	for (x = minx; x <= maxx; x++)
	  {
	    int lx=0,ly=0;
	    double sum;

	    sum = n = 0;
	    for (ly = 0; ly < dysize; ly++)
	      {
		int iy = y + ly - dysize / 2;
		if (iy < 0 || iy >= ysize) continue;
		for (lx = 0; lx < dxsize; lx++)
		  {
		    int ix = x + lx - dxsize / 2;
		    if (ix < 0 || ix >= xsize) continue;
		    sum += w[ly][lx] * ((uchar **)data)[iy][ix];
		    n += w[ly][lx];
		  }
	      }
	    tmp[y][x] = sum / n;
	  }
      for (y = miny; y <= maxy; y++)
	for (x = minx; x <= maxx; x++)
	  {
	    e->pen.gray = tmp[y][x];
	    pen__draw(data,0,x,y);
	  }
    }
  else
    {
      uchar3 **tmp = (uchar3 **)Image.data(e->diffusion);

      for (y = miny; y <= maxy; y++)
	for (x = minx; x <= maxx; x++)
	  {
	    int lx=0,ly=0;
	    double sum[3],n;

	    sum[0] = sum[1] = sum[2] = n = 0;
	    for (ly = 0; ly < dysize; ly++)
	      {
		int iy = y + ly - dysize / 2;
		if (iy < 0 || iy >= ysize) continue;
		for (lx = 0; lx < dxsize; lx++)
		  {
		    int ix = x + lx - dxsize / 2;
		    if (ix < 0 || ix >= xsize) continue;
		    sum[0] += w[ly][lx] * ((uchar3 **)data)[iy][ix].at[0];
		    sum[1] += w[ly][lx] * ((uchar3 **)data)[iy][ix].at[1];
		    sum[2] += w[ly][lx] * ((uchar3 **)data)[iy][ix].at[2];
		    n += w[ly][lx];
		  }
	      }
	    tmp[y][x].at[0] = sum[0] / n;
	    tmp[y][x].at[1] = sum[1] / n;
	    tmp[y][x].at[2] = sum[2] / n;
	  }
      for (y = miny; y <= maxy; y++)
	for (x = minx; x <= maxx; x++)
	  {
	    e->pen.color = tmp[y][x];
	    pen__draw(data,0,x,y);
	  }
    }
  e->pen.color = color;
  e->pen.gray = gray;
  Image.swap(e->pen.pat,e->pen.fill);
  pen__size(e) = size;
  pen__func(e) = func;
  Image.free(e->gradation);
  Image.free(e->diffusion);
  memfree2((char **)w);
}



void polygon_set_last(Edit e,long x,long y)
{
  e->pi_point[e->polygon_n].x = x;
  e->pi_point[e->polygon_n].y = y;
  e->pp_point[e->polygon_n].x = (x - e->x) * e->panel->width  / e->xsize;
  e->pp_point[e->polygon_n].y = (y - e->y) * e->panel->height / e->ysize;
  e->pv_point[e->polygon_n].x = x * e->view->width  / Edit__image_xsize(e);
  e->pv_point[e->polygon_n].y = y * e->view->height / Edit__image_ysize(e);
}



void polygon_set_first(Edit e,long x,long y)
{
  e->polygon_n = 0;
  polygon_add(e,x,y);
}



void polygon_add(Edit e,long x,long y)
{
  polygon_set_last(e,x,y);

  e->polygon_n++;
  e->pi_point[e->polygon_n] = e->pi_point[e->polygon_n-1];
  e->pp_point[e->polygon_n] = e->pp_point[e->polygon_n-1];
  e->pv_point[e->polygon_n] = e->pv_point[e->polygon_n-1];

  e->pi_point[e->polygon_n+1] = e->pi_point[0];
  e->pp_point[e->polygon_n+1] = e->pp_point[0];
  e->pv_point[e->polygon_n+1] = e->pv_point[0];
}



void polygon_delete_last(Edit e)
{
  e->pi_point[e->polygon_n-1] = e->pi_point[e->polygon_n];
  e->pp_point[e->polygon_n-1] = e->pp_point[e->polygon_n];
  e->pv_point[e->polygon_n-1] = e->pv_point[e->polygon_n];
  e->pi_point[e->polygon_n] = e->pi_point[e->polygon_n+1];
  e->pp_point[e->polygon_n] = e->pp_point[e->polygon_n+1];
  e->pv_point[e->polygon_n] = e->pv_point[e->polygon_n+1];
  e->polygon_n--;
}



void polygon_delete_all(Edit e)
{
  e->polygon_n = 0;
}



void store_mode_draw(Edit e)
{
  long func;
  long size;
  long type;
  long xsize,ysize;
  register bit1 **p;
  register long x,y;
  long width,height;
  register char **data;
  long maxx,maxy,minx,miny;
  uchar3 color = e->pen.color;
  uchar gray = e->pen.gray;

  long dxsize,dysize;
  double **w, n, strength;

  type  = Edit__image_type(e);
  xsize = Edit__image_xsize(e);
  ysize = Edit__image_ysize(e);

  Image.make(e->gradation,Bit1,xsize,ysize);
  Image.make(e->diffusion,type,xsize,ysize);
  if (Image.type(e->gradation) == 0)
    {
      XwObject.setLabel(e->message,"warning:diffusion_draw:can't alloc memory.");
      return;
    }
  Image.swap(Edit__image(e),e->gradation);
  Image.clear(Edit__image(e));

  size = pen__size(e);

  if ((draw__func(e) >= F_BOX) && (draw__func(e) <= F_ELLIPSE2))
    {
      Image.swap(e->pen.pat,e->pen.fill);
      pen__size(e) = 1;
    }

  func = pen__func(e);
  pen__func(e) = 0;

  /* gradation area */
  DrawObjectList[draw__func(e)](e,e->x1,e->y1,e->x2,e->y2);

  if ((draw__func(e) >= F_BOX) && (draw__func(e) <= F_ELLIPSE2))
    {
      Image.swap(e->pen.pat,e->pen.fill);
      pen__size(e) = size;
    }

  maxx = e->maxx;
  maxy = e->maxy;
  minx = e->minx;
  miny = e->miny;
  Edit__drawn_clear(e);

  Image.swap(Edit__image(e),e->gradation);

  Image.swap(e->pen.pat,e->pen.fill);
  size = pen__size(e);
  pen__size(e) = 1;

  switch (pen__store_mode(e))
    {
    case STORE_ONLY:
      buffer__copy_area(e,minx,miny,maxx,maxy);
      break;
    case STORE_AND_DRAW:
      break;
    case DRAW_AND_STORE:
      break;
    }

  p = (bit1 **)Image.data(e->gradation);
  data = Image.data(Edit__image(e));
  width = maxx - minx;
  if (width <= 0) width = 1;
  height = maxy - miny;
  if (height <= 0) height = 1;

  dxsize = e->pen.diff_xsize + 1;
  dysize = e->pen.diff_ysize + 1;
  w = (double **)memnew2(dxsize,dysize,sizeof(double));
  strength = (double)e->pen.diff_strength;
  n = 0;
  for (y = 0; y < dysize; y++)
    {
      int ry = abs(y - dysize / 2);
      for (x = 0; x < dxsize; x++)
	{
	  int rx = abs(x - dxsize / 2);
	  int d = 10 * (3 * maximum(rx,ry) + minimum(rx,ry));
	  w[y][x] = strength / (d + strength);
	  n += w[y][x];
	}
    }

  if (type == UChar)
    {
      uchar **tmp = (uchar **)Image.data(e->diffusion);
      for (y = miny; y <= maxy; y++)
	for (x = minx; x <= maxx; x++)
	  {
	    int lx=0,ly=0;
	    double sum;

	    sum = n = 0;
	    for (ly = 0; ly < dysize; ly++)
	      {
		int iy = y + ly - dysize / 2;
		if (iy < 0 || iy >= ysize) continue;
		for (lx = 0; lx < dxsize; lx++)
		  {
		    int ix = x + lx - dxsize / 2;
		    if (ix < 0 || ix >= xsize) continue;
		    sum += w[ly][lx] * ((uchar **)data)[iy][ix];
		    n += w[ly][lx];
		  }
	      }
	    tmp[y][x] = sum / n;
	  }
      for (y = miny; y <= maxy; y++)
	for (x = minx; x <= maxx; x++)
	  {
	    e->pen.gray = tmp[y][x];
	    pen__draw(data,0,x,y);
	  }
    }
  else
    {
      uchar3 **tmp = (uchar3 **)Image.data(e->diffusion);

      for (y = miny; y <= maxy; y++)
	for (x = minx; x <= maxx; x++)
	  {
	    int lx=0,ly=0;
	    double sum[3],n;

	    sum[0] = sum[1] = sum[2] = n = 0;
	    for (ly = 0; ly < dysize; ly++)
	      {
		int iy = y + ly - dysize / 2;
		if (iy < 0 || iy >= ysize) continue;
		for (lx = 0; lx < dxsize; lx++)
		  {
		    int ix = x + lx - dxsize / 2;
		    if (ix < 0 || ix >= xsize) continue;
		    sum[0] += w[ly][lx] * ((uchar3 **)data)[iy][ix].at[0];
		    sum[1] += w[ly][lx] * ((uchar3 **)data)[iy][ix].at[1];
		    sum[2] += w[ly][lx] * ((uchar3 **)data)[iy][ix].at[2];
		    n += w[ly][lx];
		  }
	      }
	    tmp[y][x].at[0] = sum[0] / n;
	    tmp[y][x].at[1] = sum[1] / n;
	    tmp[y][x].at[2] = sum[2] / n;
	  }
      for (y = miny; y <= maxy; y++)
	for (x = minx; x <= maxx; x++)
	  {
	    e->pen.color = tmp[y][x];
	    pen__draw(data,0,x,y);
	  }
    }
  e->pen.color = color;
  e->pen.gray = gray;
  Image.swap(e->pen.pat,e->pen.fill);
  pen__size(e) = size;
  pen__func(e) = func;
  Image.free(e->gradation);
  Image.free(e->diffusion);
  memfree2((char **)w);
}



long panel__reconstruct(Edit e)
{
  long xsize,ysize;
  long xsizeR,ysizeR;
  long scale;
  char buf[100];

  XwObject.getGeometry(e->panel,0,0,&xsizeR,&ysizeR);

  scale = (xsizeR + e->xsize/2) / e->xsize;
  if (scale <= 0) scale = 1;
  edit__scale(e) = scale;
  edit__redraw_scale(e);

  xsize = e->xsize * edit__scale(e);
  ysize = e->ysize * edit__scale(e);

  if (((xsize > 1024) || (ysize > 800)) && (edit__scale(e) != 1))
    {
      edit__scale(e) = 1;
      while (((e->xsize * edit__scale(e)) <= 1024) &&
	     ((e->ysize * edit__scale(e)) <= 800)) edit__scale(e) ++;
      edit__scale(e) --;
      if (edit__scale(e) <= 0)
	{
	  edit__scale(e) = 1;
	  view__draw_zoom(e);
	  if (e->xsize > 1024) e->xsize = 1024;
	  if (e->ysize > 800) e->ysize = 800;
	  view__draw_zoom(e);
	}

      xsize = e->xsize * edit__scale(e);
      ysize = e->ysize * edit__scale(e);
      sprintf(buf,"%d",edit__scale(e));
      XwObject.child.setLabel(e->edit.menu,"w0",buf);
    }

  if ((xsize != xsizeR) || (ysize != ysizeR))
    {
      XwObject.resize(e->panel,xsize,ysize);
      return 0;
    }

  return 1;
}



long panel__translate_coord(Edit e,XEvent *event,long *xp,long *yp)
{
  long x,y;
  long xsnap,ysnap;

  x = e->x + e->xsize * event->xbutton.x / e->panel->width;
  y = e->y + e->ysize * event->xbutton.y / e->panel->height;
  xsnap = edit__grid_xsize(e);
  ysnap = edit__grid_ysize(e);

  if (edit__snap_on(e))
    {
      x = ((x + xsnap / 2) / xsnap) * xsnap;
      y = ((y + ysnap / 2) / ysnap) * ysnap;
    }

  *xp = x;
  *yp = y;

  if (x < 0 || y < 0 || x >= Edit__image_xsize(e) || y >= Edit__image_ysize(e))
    return 0;

  return 1;
}
