/*
 * edit.c
 */



#include "Edit.h"
#include <math.h>



#define MAXY 20

static xw_panel panel_list[] = {
  {{"Bcc",(char *)&ShiftBM,NoHandler,NoValue,&Label},
     {"l1",NoName,NoName,FitSpace,FitSpace}},
  {{"Bcc",(char *)&LeftArrayBM,(xw_handler)edit__something,SH_LEFT,&CommandV},
     {"left",PrevName,SameName,NoSpace,SameSpace}},
  {{"Bcc",(char *)&DownArrayBM,(xw_handler)edit__something,SH_DOWN,&CommandV},
     {"down",PrevName,SameName,StickSpace,SameSpace}},
  {{"Bcc",(char *)&UpArrayBM,(xw_handler)edit__something,SH_UP,&CommandV},
     {"up",PrevName,SameName,SameSpace,SameSpace}},
  {{"Bcc",(char *)&RightArrayBM,(xw_handler)edit__something,SH_RIGHT,&CommandV},
     {"right",PrevName,SameName,SameSpace,SameSpace}},

  {{"Bcc",(char *)&StepBM,NoHandler,NoValue,&Label},
     {"l11",PrevName,NoName,FitSpace,FitSpace}},
  {{"Bcc",(char *)&DownArrayBM,(xw_handler)edit__shift_step,-1,&CommandV},
     {"s-",PrevName,SameName,NoSpace,SameSpace}},
  {{"cc"," 1",(xw_handler)edit__shift_step,0,&CommandV},
     {"s0",PrevName,SameName,StickSpace,SameSpace,0,MAXY}},
  {{"Bcc",(char *)&UpArrayBM,(xw_handler)edit__shift_step,1,&CommandV},
     {"s+",PrevName,SameName,SameSpace,SameSpace}},

  {{"Bcc",(char *)&RotationBM,NoHandler,NoValue,&Label},
     {"l12",NoName,PrevName,FitSpace,FitSpace}},
  {{"Bcc",(char *)&RightRotBM,(xw_handler)edit__something,ROT_R90,&CommandV},
     {"rr",PrevName,SameName,NoSpace,SameSpace}},
  {{"Bcc",(char *)&LeftRotBM,(xw_handler)edit__something,ROT_L90,&CommandV},
     {"lr",PrevName,SameName,StickSpace,SameSpace}},
  {{"Bcc",(char *)&RightLeftBM,(xw_handler)edit__something,ROT_XREV,&CommandV},
     {"xr",PrevName,SameName,SameSpace,SameSpace}},
  {{"Bcc",(char *)&UpDownBM,(xw_handler)edit__something,ROT_YREV,&CommandV},
     {"yr",PrevName,SameName,SameSpace,SameSpace}},
  {{"Bcc",(char *)&CrossBM,(xw_handler)edit__something,ROT_XYREV,&CommandV},
     {"zr",PrevName,SameName,SameSpace,SameSpace}},
  {{"Bcc",(char *)&RotAnyBM,(xw_handler)edit__something,ROT_ANY,&CommandV},
     {"ar",PrevName,SameName,SameSpace,SameSpace}},
  {{"Bcc",(char *)&DownArrayBM,(xw_handler)edit__angle_step,-1,&CommandV},
     {"a-",SameName,PrevName,SameSpace,StickSpace}},
  {{"cc","45",(xw_handler)edit__angle_step,0,&CommandV},
     {"a0",PrevName,SameName,SameSpace,SameSpace,0,MAXY}},
  {{"Bcc",(char *)&UpArrayBM,(xw_handler)edit__angle_step,1,&CommandV},
     {"a+",PrevName,SameName,SameSpace,SameSpace}},

  {{"Bcc",(char *)&WindowScaleBM,0,0,&Label},
     {"l2",NoName,PrevName,FitSpace,FitSpace}},
  {{"Bcc",(char *)&DownArrayBM,(xw_handler)edit__scale_step,-1,&CommandV},
     {"w-",PrevName,SameName,NoSpace,SameSpace}},
  {{"cc","1",(xw_handler)edit__scale_step,0,&CommandV},
     {"w0",PrevName,SameName,StickSpace,SameSpace,40,MAXY}},
  {{"Bcc",(char *)&UpArrayBM,(xw_handler)edit__scale_step,1,&CommandV},
     {"w+",PrevName,SameName,SameSpace,SameSpace}},

  {{"Bcc",(char *)&GridBM,NoHandler,NoValue,&Label},
     {"go",NoName,PrevName,FitSpace+3,FitSpace}},
  {{"Bcc",(char *)&OnBM,(xw_handler)edit__grid_sel,1,&SwitchV},
     {"on",PrevName,SameName,NoSpace,SameSpace}},
  {{"Bcc",(char *)&OffBM,(xw_handler)edit__grid_sel,0,&SwitchV},
     {"off",PrevName,SameName,StickSpace,SameSpace}},
  {{"Bcc",(char *)&SnapBM,NoHandler,NoValue,&Label},
     {"snap",NoName,PrevName,FitSpace,StickSpace}},
  {{"Bcc",(char *)&OnBM,(xw_handler)edit__snap_sel,1,&SwitchV},
     {"son",PrevName,SameName,NoSpace,SameSpace}},
  {{"Bcc",(char *)&OffBM,(xw_handler)edit__snap_sel,0,&SwitchV},
     {"soff",PrevName,SameName,StickSpace,SameSpace}},

  {{"Bcc",(char *)&SizeBM,NoHandler,NoValue,&Label},
     {"l3",PrevName,"l2",FitSpace,FitSpace}},
  {{"Bcc",(char *)&DownArrayBM,(xw_handler)edit__grid_x_step,-1,&CommandV},
     {"gx-",PrevName,SameName,NoSpace,SameSpace}},
  {{"cc","X:1",(xw_handler)edit__grid_x_step,0,&CommandV},
     {"gx0",PrevName,SameName,StickSpace,SameSpace,40,MAXY}},
  {{"Bcc",(char *)&UpArrayBM,(xw_handler)edit__grid_x_step,1,&CommandV},
     {"gx+",PrevName,SameName,SameSpace,SameSpace}},

  {{"Bcc",(char *)&DownArrayBM,(xw_handler)edit__grid_y_step,-1,&CommandV},
     {"gy-","l3",PrevName,NoSpace,StickSpace}},
  {{"cc","Y:1",(xw_handler)edit__grid_y_step,0,&CommandV},
     {"gy0",PrevName,SameName,StickSpace,SameSpace,40,MAXY}},
  {{"Bcc",(char *)&UpArrayBM,(xw_handler)edit__grid_y_step,1,&CommandV},
     {"gy+",PrevName,SameName,SameSpace,SameSpace}},

  {{"Bcc",(char *)&TypeBM,NoHandler,NoValue,&Label},
     {"l4",NoName,PrevName,FitSpace,FitSpace}},
  {{"Bcc",(char *)&Bit1TypeBM,(xw_handler)edit__type_sel,Bit1,&SwitchV},
     {"Bit1",NoName,PrevName,10,NoSpace}},
  {{"Bcc",(char *)&UCharTypeBM,(xw_handler)edit__type_sel,UChar,&SwitchV},
     {"UChar",NoName,PrevName,SameSpace,StickSpace}},
  {{"Bcc",(char *)&UChar3TypeBM,(xw_handler)edit__type_sel,UChar3,&SwitchV},
     {"UChar3",NoName,PrevName,SameSpace,StickSpace}},

  {{"Bcc",(char *)&ImageSizeBM,NoHandler,NoValue,&Label},
     {"l5","UChar3","gy+",FitSpace,FitSpace}},
  {{"Bcc",(char *)&DownArrayBM,(xw_handler)edit__xsize_step,-1,&CommandV},
     {"x-",SameName,PrevName,10,NoSpace}},
  {{"cc","X:256",(xw_handler)edit__xsize_step,0,&CommandV},
     {"x0",PrevName,SameName,StickSpace,SameSpace,48,20}},
  {{"Bcc",(char *)&UpArrayBM,(xw_handler)edit__xsize_step,1,&CommandV},
     {"x+",PrevName,SameName,StickSpace,SameSpace}},

  {{"Bcc",(char *)&DownArrayBM,(xw_handler)edit__ysize_step,-1,&CommandV},
     {"y-","UChar3",PrevName,10,StickSpace}},
  {{"cc","Y:256",(xw_handler)edit__ysize_step,0,&CommandV},
     {"y0",PrevName,SameName,StickSpace,SameSpace,48,20}},
  {{"Bcc",(char *)&UpArrayBM,(xw_handler)edit__ysize_step,1,&CommandV},
     {"y+",PrevName,SameName,StickSpace,SameSpace}},

  {{"Bcc",(char *)&GravityBM,NoHandler,NoValue,&Label},
     {"l6","y+","gy+",FitSpace,FitSpace}},

  {{"Bcc",(char *)&Gravity1BM,(xw_handler)edit__gravity_sel,GRAVITY_LU,&SwitchV},
     {"g1",SameName,PrevName,10,NoSpace}},
  {{"Bcc",(char *)&Gravity2BM,(xw_handler)edit__gravity_sel,GRAVITY_MU,&SwitchV},
     {"g2",PrevName,SameName,StickSpace,SameSpace}},
  {{"Bcc",(char *)&Gravity3BM,(xw_handler)edit__gravity_sel,GRAVITY_RU,&SwitchV},
     {"g3",PrevName,SameName,SameSpace,SameSpace}},

  {{"Bcc",(char *)&Gravity8BM,(xw_handler)edit__gravity_sel,GRAVITY_LM,&SwitchV},
     {"g8","y+",PrevName,10,StickSpace}},
  {{"Bcc",(char *)&Gravity0BM,(xw_handler)edit__gravity_sel,GRAVITY_MM,&SwitchV},
     {"g0",PrevName,SameName,StickSpace,StickSpace}},
  {{"Bcc",(char *)&Gravity4BM,(xw_handler)edit__gravity_sel,GRAVITY_RM,&SwitchV},
     {"g4",PrevName,SameName,StickSpace,SameSpace}},

  {{"Bcc",(char *)&Gravity7BM,(xw_handler)edit__gravity_sel,GRAVITY_LD,&SwitchV},
     {"g7","y+",PrevName,10,StickSpace}},
  {{"Bcc",(char *)&Gravity6BM,(xw_handler)edit__gravity_sel,GRAVITY_MD,&SwitchV},
     {"g6",PrevName,SameName,StickSpace,SameSpace}},
  {{"Bcc",(char *)&Gravity5BM,(xw_handler)edit__gravity_sel,GRAVITY_RD,&SwitchV},
     {"g5",PrevName,SameName,SameSpace,SameSpace}},

  {{"Bcc",(char *)&CreateBM,(xw_handler)edit__something,CREATE,&CommandV},
     {"make",NoName,PrevName,FitSpace,FitSpace}},
  {{"Bcc",(char *)&ClearBM,(xw_handler)edit__something,CLEAR_ALL,&CommandV},
     {"ca",PrevName,SameName,StickSpace,SameSpace}},
  {{"Bcc",(char *)&FillBM,(xw_handler)edit__something,FILL_ALL,&CommandV},
     {"fa",PrevName,SameName,SameSpace,SameSpace}},
  {{"Bcc",(char *)&Resize1BM,(xw_handler)edit__something,RESIZE_SZ,&CommandV},
     {"re1",PrevName,SameName,SameSpace,SameSpace}},
  {{"Bcc",(char *)&Resize2BM,(xw_handler)edit__something,RESIZE_OBJ,&CommandV},
     {"re2",PrevName,SameName,SameSpace,SameSpace}},

  {{"Bcc",(char *)&StoreBM,(xw_handler)edit__store,NoValue,&CommandV},
     {"store",NoName,PrevName,FitSpace,StickSpace}},
  {{"Bcc",(char *)&SwapBM,(xw_handler)edit__swap,NoValue,&CommandV},
     {"swap",PrevName,SameName,StickSpace,SameSpace}},
  {{"Bcc",(char *)&UnZoomBM,(xw_handler)edit__unzoom,NoValue,&CommandV},
     {"unzoom",PrevName,SameName,SameSpace,SameSpace}},
  {{"Bcc",(char *)&RedrawBM,(xw_handler)edit__redraw,NoValue,&CommandV},
     {"redraw",PrevName,SameName,SameSpace,SameSpace}},
  {{"Bcc",(char *)&UndoBM,(xw_handler)edit__undo,NoValue,&CommandV},
     {"undo",PrevName,SameName,SameSpace,SameSpace}},
  {{"Bcc",(char *)&CloseBM,(xw_handler)edit__close,NoValue,&CommandV},
     {"close",PrevName,SameName,SameSpace,SameSpace}},
};





void edit__open(Edit e)
{
  image src;

  e->edit.menu = XwObject.create(e->world,XwHard.class,0,"EditMenu");
  XwObject.panel(e->edit.menu,XwNumber(panel_list),panel_list);

  src = Edit__image(e);

  e->edit.step = 1;
  e->edit.gravity = 0;
  e->edit.grid_on = 0;
  e->edit.snap_on = 0;
  e->edit.grid_xsize = 1;
  e->edit.grid_ysize = 1;
  e->edit.type  = Image.type(src);
  e->edit.xsize = Image.xsize(src);
  e->edit.ysize = Image.ysize(src);
  e->edit.angle = 45;

  edit__redraw_scale(e);
  edit__reverse_grid(e);
  edit__reverse_snap(e);
  edit__reverse_type(e);
  edit__reverse_gravity(e);
}



long edit__close(Edit e)
{
  XwObject.unmap(e->edit.menu);
  return 0;
}



long edit__shift_step(Edit e,xw_object obj,XEvent *event,long value)
{
  long step = 1;

  switch (event->xbutton.button)
    {
    case Button1:step = 1;break;
    case Button2:step = 5;break;
    case Button3:step = 10;break;
    }

  switch (value)
    {
    case -1:
      e->edit.step /= step;
      e->edit.step --;
      e->edit.step *= step;
      break;
    case 0:e->edit.step = step;break;
    case 1:
      e->edit.step /= step;
      e->edit.step ++;
      e->edit.step *= step;
      break;
    }

  if (e->edit.step <= 0) e->edit.step = 1;
  XwObject.child.setLabel(e->edit.menu,"s0","%d",e->edit.step);

  return 0;
}



void edit__set_scale(Edit e,long scale)
{
  long xsize,ysize;

  if (scale <= 0) scale = 1;
  e->edit.scale = scale;
  edit__redraw_scale(e);
  xsize = scale * e->xsize;
  ysize = scale * e->ysize;
  XwObject.resize(e->panel,xsize,ysize);
}



long edit__scale_step(Edit e,xw_object obj,XEvent *event,long value)
{
  long step = 1;
  long scale;

  scale = edit__scale(e);
  switch (value)
    {
    case -1:scale -= step;break;
    case 0:scale = step;break;
    case 1:scale += step;break;
    }

  edit__set_scale(e,scale);

  return 0;
}



long edit__grid_x_step(Edit e,xw_object obj,XEvent *event,long value)
{
  long step = 1;
  long size = e->edit.grid_xsize;

  switch (event->xbutton.button)
    {
    case Button1: step = 1; break;
    case Button2: step = 2; break;
    case Button3: step = 5; break;
    }

  switch (value)
    {
    case -1: size = step * (size / step - 1); break;
    case  0: size = step;break;
    case  1: size = step * (size / step + 1); break;
    }

  edit__set_grid_size(e,size,e->edit.grid_ysize);

  return 0;
}



long edit__grid_y_step(Edit e,xw_object obj,XEvent *event,long value)
{
  long step = 1;
  long size = e->edit.grid_ysize;

  switch (event->xbutton.button)
    {
    case Button1: step = 1; break;
    case Button2: step = 2; break;
    case Button3: step = 5; break;
    }

  switch (value)
    {
    case -1: size = step * (size / step - 1); break;
    case  0: size = step;break;
    case  1: size = step * (size / step + 1); break;
    }

  edit__set_grid_size(e,e->edit.grid_xsize,size);

  return 0;
}



void edit__set_grid_size(Edit e,long xsize,long ysize)
{
  if (xsize <= 0) xsize = 1;
  if (ysize <= 0) ysize = 1;

  if (xsize != e->edit.grid_xsize)
    XwObject.child.setLabel(e->edit.menu,"gx0","X:%d",xsize);
  if (ysize != e->edit.grid_ysize)
    XwObject.child.setLabel(e->edit.menu,"gy0","Y:%d",ysize);

  if (e->edit.grid_on) edit__draw_grid(e);
  e->edit.grid_ysize = ysize;
  e->edit.grid_xsize = xsize;
  if (e->edit.grid_on) edit__draw_grid(e);
}



long edit__grid_sel(Edit e,xw_object obj,XEvent *event,long value)
{
  edit__frame_grid(e);

  if (e->edit.grid_on != value) edit__draw_grid(e);
  e->edit.grid_on = value;

  edit__reverse_grid(e);

  return 0;
}



long edit__snap_sel(Edit e,xw_object obj,XEvent *event,long value)
{
  edit__frame_snap(e);

  e->edit.snap_on = value;

  edit__reverse_snap(e);

  return 0;
}



long edit__type_sel(Edit e,xw_object obj,XEvent *event,long type)
{
  edit__frame_type(e);

  e->edit.type = type;

  edit__reverse_type(e);

  return 0;
}



void edit__set_size(Edit e,long xsize,long ysize)
{
  if (xsize <= 0) xsize = 1;
  if (ysize <= 0) ysize = 1;

  if (e->edit.xsize != xsize)
    XwObject.child.setLabel(e->edit.menu,"x0","X:%d",xsize);
  if (e->edit.ysize != ysize)
    XwObject.child.setLabel(e->edit.menu,"y0","Y:%d",ysize);

  e->edit.xsize = xsize;
  e->edit.ysize = ysize;
}



long edit__xsize_step(Edit e,xw_object obj,XEvent *event,long value)
{
  long step = 1;
  long size = e->edit.xsize;

  switch (event->xbutton.button)
    {
    case Button1: step = 1; break;
    case Button2: step = 5; break;
    case Button3: step = 10; break;
    }

  switch (value)
    {
    case -1: size = step * (size / step - 1); break;
    case  0:
      switch(event->xbutton.button)
	{
	case Button1:size = Image.xsize(Edit__image(e))/2;break;
	case Button2:size = Image.xsize(Edit__image(e));break;
	case Button3:size = Image.xsize(Edit__image(e))*2;break;
	}
      break;
    case  1: size = step * (size / step + 1); break;
    }

  edit__set_size(e,size,e->edit.ysize);

  return 0;
}



long edit__ysize_step(Edit e,xw_object obj,XEvent *event,long value)
{
  long step = 1;
  long size = e->edit.ysize;

  switch (event->xbutton.button)
    {
    case Button1: step = 1; break;
    case Button2: step = 5; break;
    case Button3: step = 10; break;
    }

  switch (value)
    {
    case -1: size = step * (size / step - 1); break;
    case  0:
      switch(event->xbutton.button)
	{
	case Button1:size = Image.ysize(Edit__image(e))/2;break;
	case Button2:size = Image.ysize(Edit__image(e));break;
	case Button3:size = Image.ysize(Edit__image(e))*2;break;
	}
      break;
    case  1: size = step * (size / step + 1); break;
    }

  edit__set_size(e,e->edit.xsize,size);

  return 0;
}



long edit__gravity_sel(Edit e,xw_object obj,XEvent *event,long value)
{
  edit__frame_gravity(e);

  e->edit.gravity = value;

  edit__reverse_gravity(e);

  return 0;
}



long edit__angle_step(Edit e,xw_object obj,XEvent *event,long value)
{
  long step = 1;

  switch (event->xbutton.button)
    {
    case Button1:step = 1;break;
    case Button2:step = 5;break;
    case Button3:step = 10;break;
    }

  switch (value)
    {
    case -1:
      e->edit.angle /= step;
      e->edit.angle --;
      e->edit.angle *= step;
      break;
    case 0:
      switch (event->xbutton.button)
	{
	case Button1: e->edit.angle = 0; break;
	case Button2: e->edit.angle = - e->edit.angle; break;
	case Button3: e->edit.angle = 180;break;
	}
    case 1:
      e->edit.angle /= step;
      e->edit.angle ++;
      e->edit.angle *= step;
      break;
    }

  XwObject.child.setLabel(e->edit.menu,"a0","%d",e->edit.angle);

  return 0;
}



void edit__draw_grid(Edit e)
{
  edit__draw_grid_area(e,e->x,e->y,e->x+e->xsize-1,e->y+e->ysize-1);
}



void edit__draw_grid_area(Edit e,long x1,long y1,long x2,long y2)
{
  long xsize,ysize;
  long x,y;
  long wxsize,wysize;
  static char dash[2] = {3,1};
  long xx1,xx2,yy1,yy2;

  if (edit__scale(e) == 1)
    dash[0] = 1;
  else
    dash[0] = edit__scale(e) - 1;
  XSetDashes(Edit__display(e),Edit__xorGC(e),0,dash,2);
  XSetLineAttributes(Edit__display(e),Edit__xorGC(e),0,LineOnOffDash,0,0);

  wxsize = XwObject.width(Edit__panel(e));
  wysize = XwObject.height(Edit__panel(e));
  xsize = Image.xsize(Edit__image(e));
  ysize = Image.ysize(Edit__image(e));

  xx1 = (x1 - e->x) * edit__scale(e);
  yy1 = (y1 - e->y) * edit__scale(e);
  xx2 = (x2 - e->x + 1) * edit__scale(e);
  yy2 = (y2 - e->y + 1) * edit__scale(e);

  for (y = 0; y < ysize; y += e->edit.grid_ysize)
    if ((y >= y1) && (y <= y2))
      {
	long wy = (y - e->y) * edit__scale(e);
	XDrawLine(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
		  xx1,wy,xx2,wy);
      }

  for (x = 0; x < xsize; x += e->edit.grid_xsize)
    if ((x >= x1) && (x <= x2))
      {
	long wx = (x - e->x) * edit__scale(e);
	XDrawLine(Edit__display(e),Edit__panelID(e),Edit__xorGC(e),
		  wx,yy1,wx,yy2);
      }

  XSetLineAttributes(Edit__display(e),Edit__xorGC(e),0,LineSolid,0,0);
}



void edit__redraw_scale(Edit e)
{
  XwObject.child.setLabel(e->edit.menu,"w0","%d",edit__scale(e));
}


void edit__set_grid_state(Edit e,long state)
{
  static char *name[] = {"off","on"};
  long i = e->edit.grid_on;
  XwObject.child.setState(e->edit.menu,name[i],state);
}



void edit__set_snap_state(Edit e,long state)
{
  static char *name[] = {"soff","son"};
  long i = e->edit.snap_on;
  XwObject.child.setState(e->edit.menu,name[i],state);
}



void edit__set_type_state(Edit e,long state)
{
  long type = e->edit.type;
  XwObject.child.setState(e->edit.menu,typename(type),state);
}



void edit__set_gravity_state(Edit e,long state)
{
  static char *name[] = {"g0","g1","g2","g3","g4","g5","g6","g7","g8"};
  long i = e->edit.gravity;
  XwObject.child.setState(e->edit.menu,name[i],state);
}



void edit__create(Edit e,image orig)
{
  Image.make(orig,e->edit.type,e->edit.xsize,e->edit.ysize);
  if (e->edit.type != Bit1) image__reverse(orig,orig);
}



void edit__fill_all(Edit e,image orig)
{
  long func;

  func = e->draw.func;
  Edit__drawn_clear(e);

  e->draw.func = F_BOX;
  panel_set_firstpoint(e,0,0);
  panel_set_secondpoint(e,Image.xsize(Edit__image(e))-1,
			Image.ysize(Edit__image(e))-1);

  draw_object(e);

  e->draw.func = func;
}



long edit__something(Edit e,xw_object obj,XEvent *event,long value)
{
  image_item src;
  image orig;

  Edit__backup(e);
  src = Edit__original(e);
  imageItem__set_edit(src);

  orig = imageItem__image(src);
  switch (value)
    {
    case SH_UP: case SH_DOWN: case SH_RIGHT: case SH_LEFT:
      image__shift(orig,orig,value,e->edit.step);
      break;
    case ROT_R90:
      image__rotation_right_90(orig,orig);
      break;
    case ROT_L90:
      image__rotation_left_90(orig,orig);
      break;
    case ROT_XREV:
      image__x_reverse(orig,orig);
      break;
    case ROT_YREV:
      image__y_reverse(orig,orig);
      break;
    case ROT_XYREV:
      image__xy_reverse(orig,orig);
      break;
    case ROT_ANY:
      image__rotation_any(orig,orig,e->edit.angle);
      break;
    case RESIZE_SZ:
      image__region(orig,orig,e->edit.xsize,e->edit.ysize,e->edit.gravity);
      break;
    case RESIZE_OBJ:
      image__resize(orig,orig,e->edit.xsize,e->edit.ysize);
      break;
    case CLEAR_ALL:
      image__clear(orig);
      break;
    case FILL_ALL:
      edit__fill_all(e,orig);
      return 0;
      break;
    case CREATE:
      edit__create(e,orig);
      break;
    }

  Edit__set_original(e);

  return 0;
}



long edit__redraw(Edit e)
{
  panel__expose(e);
  view__expose(e);
  return 0;
}



long edit__undo(Edit e)
{
  imageItem__swap(Edit__original(e),Edit__previous(e));
  Edit__set_original(e);

  return 0;
}



long edit__store(Edit e)
{
  buffer__copy(e);

  return 0;
}



long edit__swap(Edit e)
{
  buffer__swap(e);

  return 0;
}



long edit__unzoom(Edit e)
{
  view__draw_zoom(e);
  Edit__set_region(e,0,0,Edit__image_xsize(e),Edit__image_ysize(e));
  view__draw_zoom(e);

  return 0;
}
