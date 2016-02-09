/*
 * buffer.c
 */



#include "Edit.h"
#include <malloc.h>



static xw_event event_list[] = {
  {Expose,(xw_handler)buffer__redraw}
};

static xw_event key_list[] = {
  {'r',(xw_handler)buffer__redraw}
};


#define IX 36
#define IZ 40
#define LBUF 26
#define SCW 39

static xw_panel panel_list[] = {
  {{"cc",0,(xw_handler)buffer__select, 0,&LabelV,&Box},
     {"B0",NoName,NoName,FitSpace,FitSpace,IZ,IZ}},
  {{"cc",NoLabel,SameHandler, 1,SameAction},
     {"B1",PrevName,SameName,StickSpace,SameSpace,SameSize,SameSize}},
  {{"cc",NoLabel,SameHandler, 2,&LabelV,&Box},
     {"B2",PrevName,SameName,SameSpace,SameSpace,SameSize,SameSize}},
  {{"cc",NoLabel,SameHandler, 3,&LabelV,&Box},
     {"B3",PrevName,SameName,SameSpace,SameSpace,SameSize,SameSize}},
  {{"cc",NoLabel,SameHandler, 4,&LabelV,&Box},
     {"B4",PrevName,SameName,SameSpace,SameSpace,SameSize,SameSize}},
  {{"cc",NoLabel,SameHandler, 5,&LabelV,&Box},
     {"B5",PrevName,SameName,SameSpace,SameSpace,SameSize,SameSize}},
  {{"cc",NoLabel,SameHandler, 6,&LabelV,&Box},
     {"B6",PrevName,SameName,SameSpace,SameSpace,SameSize,SameSize}},
  {{"cc",NoLabel,SameHandler, 7,&LabelV,&Box},
     {"B7",PrevName,SameName,SameSpace,SameSpace,SameSize,SameSize}},
  {{"cc",NoLabel,SameHandler, 8,&LabelV,&Box},
     {"B8",PrevName,SameName,SameSpace,SameSpace,SameSize,SameSize}},
  {{"cc",NoLabel,SameHandler, 9,&LabelV,&Box},
     {"B9",PrevName,SameName,SameSpace,SameSpace,SameSize,SameSize}},
  {{"cc",NoLabel,SameHandler,10,&LabelV,&Box},
     {"B10",PrevName,SameName,SameSpace,SameSpace,SameSize,SameSize}},
  {{"cc",NoLabel,SameHandler,11,&LabelV,&Box},
     {"B11",PrevName,SameName,SameSpace,SameSpace,SameSize,SameSize}},
  {{"cc",NoLabel,SameHandler,12,&LabelV,&Box},
     {"B12",PrevName,SameName,SameSpace,SameSpace,SameSize,SameSize}},
  {{"Bcc",(char *)&UpArrayBM,(xw_handler)buffer__scroll,-1,&CommandV,&Box},
     {"bs+",PrevName,SameName,SameSpace,SameSpace}},
  {{"Bcc",0,(xw_handler)buffer__scroll,0,&LabelV,&Box},
     {"bs","B12",PrevName,StickSpace,StickSpace,20,SCW}},
  {{"Bcc",(char *)&DownArrayBM,(xw_handler)buffer__scroll,1,&CommandV,&Box},
     {"bs-",SameName,PrevName,StickSpace,StickSpace}},
  {{"Bcc",(char *)&KillBM,(xw_handler)buffer__kill,0,&CommandV,&Box},
     {"kill",PrevName,0,SameSpace,FitSpace}},

  {{"cc",NoLabel,(xw_handler)buffer__select,13,&LabelV,&Box},
     {"B13",NoName,PrevName,FitSpace,StickSpace,SameSize,SameSize}},
  {{"cc",NoLabel,SameHandler,14,&LabelV,&Box},
     {"B14",PrevName,SameName,StickSpace,SameSpace,SameSize,SameSize}},
  {{"cc",NoLabel,SameHandler,15,&LabelV,&Box},
     {"B15",PrevName,SameName,SameSpace,SameSpace,SameSize,SameSize}},
  {{"cc",NoLabel,SameHandler,16,&LabelV,&Box},
     {"B16",PrevName,SameName,SameSpace,SameSpace,SameSize,SameSize}},
  {{"cc",NoLabel,SameHandler,17,&LabelV,&Box},
     {"B17",PrevName,SameName,SameSpace,SameSpace,SameSize,SameSize}},
  {{"cc",NoLabel,SameHandler,18,&LabelV,&Box},
     {"B18",PrevName,SameName,SameSpace,SameSpace,SameSize,SameSize}},
  {{"cc",NoLabel,SameHandler,19,&LabelV,&Box},
     {"B19",PrevName,SameName,SameSpace,SameSpace,SameSize,SameSize}},
  {{"cc",NoLabel,SameHandler,20,&LabelV,&Box},
     {"B20",PrevName,SameName,SameSpace,SameSpace,SameSize,SameSize}},
  {{"cc",NoLabel,SameHandler,21,&LabelV,&Box},
     {"B21",PrevName,SameName,SameSpace,SameSpace,SameSize,SameSize}},
  {{"cc",NoLabel,SameHandler,22,&LabelV,&Box},
     {"B22",PrevName,SameName,SameSpace,SameSpace,SameSize,SameSize}},
  {{"cc",NoLabel,SameHandler,23,&LabelV,&Box},
     {"B23",PrevName,SameName,SameSpace,SameSpace,SameSize,SameSize}},
  {{"cc",NoLabel,SameHandler,24,&LabelV,&Box},
     {"B24",PrevName,SameName,SameSpace,SameSpace,SameSize,SameSize}},
  {{"cc",NoLabel,SameHandler,25,&LabelV,&Box},
     {"B25",PrevName,SameName,SameSpace,SameSpace,SameSize,SameSize}},
  {{"Bcc",(char *)&CloseBM,(xw_handler)buffer__close,0,&CommandV,&Box},
     {"close","bs",SameName,SameSpace,SameSpace}}
};



static Display *d;
static GC gc;
static Window id;

void buffer__open(Edit e)
{
  int i,n;
  long argc;
  image_item item;
  char *filename;

  e->buffer.menu = XwObject.create(e->world,XwHard.class,0,"BufferMenu");
  XwObject.panel(e->buffer.menu,XwNumber(panel_list),panel_list);
  XwObject.addEvents(e->buffer.menu,XwNumber(event_list),event_list);
  XwObject.addKeyEvents(e->buffer.menu,XwNumber(key_list),key_list);

  e->buffer.offset = 0;
  e->buffer.current = 0;
  e->buffer.num = DEFAULT_BUFFER_NUM;
  e->buffer.list = (struct image_itemRec *)calloc(e->buffer.num,sizeof(struct image_itemRec));

  for (i = 0; i < e->buffer.num; i++)
    {
      item = buffer__item(e,i);
      imageItem__initialize(item);
    }

  argc = optargc;
  for (n = 0, i = 1; (i < argc) && (n < e->buffer.num); i++)
    {
      item = buffer__item(e,n);
      filename = optargv[i];
      if (imageItem__load(item,filename)) n++;
    }

  for ( ; i < argc; i++)
    fprintf(stderr,"sorry no buffer ... ignore (%s) !\n",optargv[i]);

  buffer__focus(e,0);

  d  = XwWorld.display(e->world);
  gc = XwWorld.gc(e->world);
}



long buffer__redraw(Edit e)
{
  XwObject.map(e->buffer.menu);

  id = XwObject.id(e->buffer.menu);

  buffer__redraw_all_images(e);

  buffer__redraw_scroll(e);

  return 0;
}



void buffer__set_current(Edit e,long i)
{
  buffer__unfocus(e,e->buffer.current);
  e->buffer.current = i;
  buffer__focus(e,e->buffer.current);
}



void buffer__copy_somewhere(Edit e,long i_offset)
{
  int n;
  image_item src,dest;

  src = buffer__item_offset(e,i_offset);
  if (imageItem__not_occupy(src)) return;

  n = buffer__search_free_number(e);
  if (n == -1) return;

  dest = buffer__item(e,n);
  imageItem__copy(dest,src);

  buffer__redraw_image(e, n - e->buffer.offset);
}



void buffer__copy_current_somewhere(Edit e)
{
  buffer__copy_somewhere(e,e->buffer.current);
}



void buffer__copy(Edit e)
{
  buffer__copy_original(e,e->buffer.current);
}



void buffer__swap(Edit e)
{
  buffer__swap_original(e,e->buffer.current);
}



void buffer__copy_original(Edit e,long i_offset)
{
  image_item item;
  image_item original;

  item = buffer__item_offset(e,i_offset);
  original = Edit__original(e);

  if (imageItem__need_save(item) || imageItem__state(item) == ItemLoad)
    buffer__copy_somewhere(e,i_offset);

  imageItem__copy(item,original);

  buffer__redraw_image(e,i_offset);
}



void buffer__swap_original(Edit e,long i_offset)
{
  image_item item;
  image_item original;

  item = buffer__item_offset(e,i_offset);
  original = Edit__original(e);

  Edit__backup(e);
  imageItem__swap(item,original);

  buffer__redraw_image(e,i_offset);
  Edit__set_original(e);
}



void buffer__swap_or_copy(Edit e,long i_offset)
{
  image_item item;

  item = buffer__item_offset(e,i_offset);

  if (imageItem__not_occupy(item))
    buffer__copy_original(e,i_offset);
  else
    buffer__swap_original(e,i_offset);
}



long buffer__select(Edit e,xw_object obj,XEvent *event,long i_offset)
{
  switch (event->xbutton.button)
    {
    case Button1:
      buffer__set_current(e,i_offset);
      break;

    case Button3:
      buffer__kill(e,i_offset);
      break;

    case Button2:
      buffer__swap_or_copy(e,i_offset);
      break;
    }

  return 0;
}



void buffer__copy_area(Edit e,long x1,long y1,long x2,long y2)
{
  image_item dest,src;

  if (x1 > x2) {long tmp = x1; x1 = x2; x2 = tmp;}
  if (y1 > y2) {long tmp = y1; y1 = y2; y2 = tmp;}

  dest = buffer__item_current(e);
  src  = Edit__original(e);

  if (imageItem__need_save(dest) || imageItem__state(dest) == ItemLoad)
    buffer__copy_current_somewhere(e);
  imageItem__copy_area(dest,src,x1,y1,x2,y2);

  buffer__redraw_current(e);
}



long buffer__close(Edit e)
{
  XwObject.unmap(e->buffer.menu);
  return 0;
}



long buffer__kill(Edit e,long i_offset)
{
  image_item item;
  int num;

  num = buffer__number(e,i_offset);
  item = buffer__item_offset(e,i_offset);

  if (imageItem__need_save(item) && (! e->quit_flag))
    {
      XwObject.setLabel(e->message," #%d was not saved. (one click)\n",num);
      e->quit_flag ++;
      return 0;
    }
  e->quit_flag = 0;

  if (imageItem__not_occupy(item))
    {
      XwObject.setLabel(e->message," #%d is not set image.\n",num);
      return 0;
    }

  XwObject.setLabel(e->message," kill #%d buffer.\n",num);

  imageItem__free(item);
  buffer__redraw_image(e,i_offset);

  return 0;
}



long buffer__scroll(Edit e,xw_object obj,XEvent *event,long value)
{
  long step = 1;
  long offset = e->buffer.offset;

  switch (event->xbutton.button)
    {
    case Button1: step = 1; break;
    case Button2: step = LBUF/2; break;
    case Button3: step = LBUF; break;
    }

  switch (value)
    {
    case -1: offset -= step; break;
    case  0: offset = e->buffer.num * event->xbutton.y / SCW; break;
    case  1: offset += step; break;
    }

  buffer__set_offset(e,offset);

  return 0;
}



void buffer__redraw_scroll(Edit e)
{
  long y,wy;
  xw_object w;

  w = XwObject.search(e->buffer.menu,"bs");
  XwObject.map(w);
  y = w->y + SCW * e->buffer.offset / e->buffer.num;
  wy = (w->y + SCW * (LBUF + e->buffer.offset) / e->buffer.num) - y;
  XFillRectangle(d,id,gc,w->x+2,y+1,18,wy);
}



long buffer__number(Edit e,long i_offset)
{
  return i_offset + e->buffer.offset;
}



image_item buffer__item(Edit e,long i)
{
  return &e->buffer.list[i];
}



image_item buffer__item_offset(Edit e,long i_offset)
{
  return &(e->buffer.list[i_offset + e->buffer.offset]);
}



image_item buffer__item_current(Edit e)
{
  return &(e->buffer.list[e->buffer.current+e->buffer.offset]);
}



void buffer__redraw_all_images(Edit e)
{
  int i;
  for (i = 0; i < LBUF; i++) buffer__redraw_image(e,i);
}



void buffer__redraw_current(Edit e)
{
  buffer__redraw_image(e,e->buffer.current);
}



void buffer__redraw_image(Edit e,long i_offset)
{
  char buf[100];
  image_item item;
  image img;
  xw_object w;

  if ((i_offset < 0) || (i_offset >= LBUF)) return;
  if (! XwObject.mapped(e->buffer.menu)) return;

  item = buffer__item_offset(e,i_offset);
  img = imageItem__image(item);

  sprintf(buf,"B%d",i_offset);
  w = XwObject.search(e->buffer.menu,buf);
  if (Image.area(img))
    {
      long xsize,ysize;
      xsize = Image.xsize(img);
      ysize = Image.ysize(img);
      if ((xsize <= IX) && (ysize <= IX))
	XShowImagePartOfWindow(d,id,gc,img,w->x+3,w->y+3,xsize,ysize);
      else
	XShowImagePartOfWindow(d,id,gc,img,w->x+3,w->y+3,IX,IX);
    }
  else
    {
      XClearArea(d,id,w->x+3,w->y+3,IX,IX,False);
      XDrawLine(d,id,gc,w->x+3,w->y+3,w->x+2+IX,w->y+2+IX);
      XDrawLine(d,id,gc,w->x+3,w->y+2+IX,w->x+2+IX,w->y+3);
    }
}



void buffer__set_state(Edit e,long i_offset,long state)
{
  char buf[12];
  sprintf(buf,"B%d",i_offset);
  XwObject.child.setState(e->buffer.menu,buf,state);
}



void buffer__focus(Edit e,long i_offset)
{
  buffer__set_state(e,i_offset,XW_FOCUS);
  buffer__redraw_image(e,i_offset);
}



void buffer__unfocus(Edit e,long i_offset)
{
  buffer__set_state(e,i_offset,XW_FRAME);
  buffer__redraw_image(e,i_offset);
}



long buffer__search_free_number(Edit e)
{
  int i;
  image_item item;

  for (i = 0; i < e->buffer.num; i++)
    {
      item = buffer__item(e,i);
      if (imageItem__not_occupy(item)) return i;
    }

  for (i = 0; i < e->buffer.num; i++)
    {
      item = buffer__item(e,i);
      if (imageItem__state(item) == ItemCreate) return i;
    }

  for (i = e->buffer.num - 1; i >= 0; i--)
    {
      item = buffer__item(e,i);
      if (imageItem__state(item) == ItemStore) return i;
    }

  for (i = e->buffer.num - 1; i >= 0; i--)
    {
      item = buffer__item(e,i);
      if (imageItem__state(item) == ItemLoad) return i;
    }

  return -1;
}



void buffer__set_offset(Edit e,long offset)
{
  if (offset < 0) offset = 0;
  if (offset >= (e->buffer.num - LBUF)) offset = e->buffer.num - LBUF;

  e->buffer.offset = offset;

  XwObject.setLabel(e->message,"set buffer offset #%d",offset);

  buffer__redraw_all_images(e);
  buffer__redraw_scroll(e);
}
