/*
 * open.c
 */



#include "Edit.h"



static long menu__quit(Edit e)
{
  int i;
  long total;

  total = imageItem__need_save(Edit__original(e));
  for (i = 0; i < buffer__num(e); i++)
    total += imageItem__need_save(buffer__item(e,i));
  if (total == 0) return 1;

  XwObject.setLabel(e->message," You should save image.");
  switch (e->quit_flag)
    {
    case 0:
      XwObject.setLabel(e->message," You should save image. (two more clicks)");
      break;
    case 1:
      XwObject.setLabel(e->message," You should save image. (one more click)");
      break;
    case 2:
      XwObject.setLabel(e->message,"   Ohhh!! Nooooo!! Help!!");
      return 1;
    }
  e->quit_flag ++;

  return 0;
}



enum {INFO_OBJ,BUFFER_OBJ,PEN_OBJ,DRAW_OBJ,EDIT_OBJ,VIEW_OBJ};
enum {FMT_J4, FMT_C2D, FMT_PPM, FMT_TIFF, FMT_XW, FMT_GIF, FMT_JPEG};



static char *format_str[] = {"J4","C2D","PPM","TIFF","XwBitmap","GIF","JPEG"};



#define MAINW 308
#define MAINH 20

static xw_panel menu_list[] = {
  {{"Bcc",(char *)&MessageBM,NoHandler,NoValue,&Label},
     {"l0",NoName,NoName,FitSpace,FitSpace}},
  {{"cl",0,(xw_handler)menu__coord,NoValue,&ButtonV},
     {"message",PrevName,NoName,NoSpace,SameSpace,MAINW,MAINH}},

  {{"Bcc",(char *)&FilenameBM,NoHandler,NoValue,&Label},
     {"l1",NoName,PrevName,FitSpace+4,SameSpace}},
  {{"cc",0,(xw_handler)file__change_name,NoValue,&CommandV},
     {"filename",PrevName,SameName,NoSpace,SameSpace,MAINW,MAINH}},

  {{"Bcc",(char *)&CommentBM,NoHandler,NoValue,&Label},
     {"l2",NoName,PrevName,FitSpace,SameSpace}},
  {{"cc",0,(xw_handler)file__change_comment,NoValue,&CommandV},
     {"comment",PrevName,SameName,NoSpace,SameSpace,MAINW,MAINH}},

  {{"Bcc",(char *)&FormatBM,NoHandler,NoValue,&Label},
     {"l3",NoName,PrevName,FitSpace,SameSpace}},
  {{"Bcc",(char *)&J4BM,(xw_handler)file__format_sel,FMT_J4,&SwitchV},
     {"J4",PrevName,SameName,NoSpace,SameSpace}},
  {{"Bcc",(char *)&C2DBM,(xw_handler)file__format_sel,FMT_C2D,&SwitchV},
     {"C2D",PrevName,SameName,StickSpace,SameSpace}},
  {{"Bcc",(char *)&PNMBM,(xw_handler)file__format_sel,FMT_PPM,&SwitchV},
     {"PPM",PrevName,SameName,SameSpace,SameSpace}},
  {{"Bcc",(char *)&TIFFBM,(xw_handler)file__format_sel,FMT_TIFF,&SwitchV},
     {"TIFF",PrevName,SameName,SameSpace,SameSpace}},
  {{"Bcc",(char *)&GIFBM,(xw_handler)file__format_sel,FMT_GIF,&SwitchV},
     {"GIF",PrevName,SameName,SameSpace,SameSpace}},
  {{"Bcc",(char *)&XwBM,(xw_handler)file__format_sel,FMT_XW,&SwitchV},
     {"XwBitmap",PrevName,SameName,SameSpace,SameSpace}},
  {{"Bcc",(char *)&JPEGBM,(xw_handler)file__format_sel,FMT_JPEG,&SwitchV},
     {"JPEG",PrevName,SameName,SameSpace,SameSpace}},

  {{"Bcc",(char *)&TypeBM,NoHandler,NoValue,&Label},
     {"l4",NoName,PrevName,FitSpace,FitSpace}},
  {{"Bcc",(char *)&Bit1TypeBM,NoHandler,NoValue,&LabelV},
     {"Bit1",PrevName,SameName,NoSpace,SameSpace}},
  {{"Bcc",(char *)&UCharTypeBM,NoHandler,NoValue,&LabelV},
     {"UChar",PrevName,SameName,StickSpace,SameSpace}},
  {{"Bcc",(char *)&UChar3TypeBM,NoHandler,NoValue,&LabelV},
     {"UChar3",PrevName,SameName,SameSpace,SameSpace}},

  {{"Bcc",(char *)&SizeBM,NoHandler,NoValue,&Label},
     {"l5",PrevName,SameName,FitSpace,SameSpace}},
  {{"cc","X:256",NoHandler,NoValue,&LabelV},
     {"x0",PrevName,SameName,StickSpace,SameSpace,0,MAINH}},
  {{"cc","Y:256",NoHandler,NoValue,&LabelV},
     {"y0",PrevName,SameName,SameSpace,SameSpace,0,MAINH}},

  {{"Bcc",(char *)&InfoBM,(xw_handler)menu__open_close,INFO_OBJ,&ButtonV},
     {"info",NoName,PrevName,FitSpace,FitSpace}},
  {{"Bcc",(char *)&BufferBM,(xw_handler)menu__open_close,BUFFER_OBJ,&ButtonV},
     {"buffer",PrevName,SameName,StickSpace,SameSpace}},
  {{"Bcc",(char *)&PenBM,(xw_handler)menu__open_close,PEN_OBJ,&ButtonV},
     {"pen",PrevName,SameName,SameSpace,SameSpace}},
  {{"Bcc",(char *)&DrawBM,(xw_handler)menu__open_close,DRAW_OBJ,&ButtonV},
     {"draw",PrevName,SameName,SameSpace,SameSpace}},
  {{"Bcc",(char *)&EditBM,(xw_handler)menu__open_close,EDIT_OBJ,&ButtonV},
     {"edit",PrevName,SameName,SameSpace,SameSpace}},
  {{"Bcc",(char *)&ViewBM,(xw_handler)menu__open_close,VIEW_OBJ,&ButtonV},
     {"view",PrevName,SameName,SameSpace,SameSpace}},
  {{"Bcc",(char *)&LoadBM,(xw_handler)file__load,NoValue,&CommandV},
     {"load",PrevName,SameName,FitSpace,SameSpace}},
  {{"Bcc",(char *)&SaveBM,(xw_handler)file__save,NoValue,&CommandV},
     {"save",PrevName,SameName,StickSpace,SameSpace}},
  {{"Bcc",(char *)&QuitBM,(xw_handler)menu__quit,NoValue,&CommandV},
     {"quit",PrevName,SameName,SameSpace,SameSpace}}
};



void Edit__open(Edit e)
{
  e->world = XwWorld.create(e,"ImageEdit");

  e->menu = XwObject.create(e->world,XwHard.class,0,"ImageEditMainMenu");
  XwObject.panel(e->menu,Number(menu_list),menu_list);
  XwObject.map(e->menu);
  e->message = XwObject.search(e->menu,"message");

  panel__open(e);
  view__open(e);

  e->blackGC = Edit__gc(e);
  e->whiteGC = XCreateGC(Edit__display(e),Edit__panelID(e),0,0);
  e->xorGC   = XCreateGC(Edit__display(e),Edit__panelID(e),0,0);
  XSetForeground(Edit__display(e),Edit__whiteGC(e),Edit__whitePixel(e));
  XSetFunction(Edit__display(e),Edit__xorGC(e),GXinvert);

  info__open(e);
  pen__open(e);
  draw__open(e);
  edit__open(e);
  buffer__open(e);

  XwObject.setLabel(e->message," I(0,0) = ***");
  file__redraw_name(e);
  file__redraw_comment(e);
  file__redraw_type(e);
  file__set_format(e,FMT_C2D);
  file__redraw_size(e);
  file__change_format(e,ImageFile.getLoadFormat());

  XwFlush(e->world);
}



long menu__coord(Edit e)
{
  e->coord_on = 1 - e->coord_on;
  return 0;
}



long menu__open_close(Edit e,xw_object obj,XEvent *event,long value)
{
  switch (value)
    {
    case INFO_OBJ:   obj = e->info.menu; break;
    case BUFFER_OBJ: obj = e->buffer.menu; break;
    case PEN_OBJ:    obj = e->pen.menu; break;
    case DRAW_OBJ:   obj = e->draw.menu; break;
    case EDIT_OBJ:   obj = e->edit.menu; break;
    case VIEW_OBJ:   obj = e->view; break;
    }
  if (XwObject.mapped(obj)) XwObject.unmap(obj); else XwObject.map(obj);
  return 0;
}



long menu__type_check(Edit e)
{
  long type = Edit__image_type(e);

  if (type == Bit1 || type == UChar || type == UChar3)
    return 1;

  return 0;
}



void file__set_info(Edit e)
{
  file__redraw_name(e);
  file__redraw_comment(e);
  file__redraw_type(e);
  file__redraw_size(e);
}



void file__redraw_name(Edit e)
{
  XwObject.child.setLabel(e->menu,"filename",Edit__filename(e));
}



void file__redraw_comment(Edit e)
{
  XwObject.child.setLabel(e->menu,"comment",Edit__comment(e));
}



void file__redraw_type(Edit e)
{
  XwObject.child.setState(e->menu,"Bit1",XW_FRAME);
  XwObject.child.setState(e->menu,"UChar",XW_FRAME);
  XwObject.child.setState(e->menu,"UChar3",XW_FRAME);

  XwObject.child.setState(e->menu,Edit__image_type_name(e),XW_REVERSE);
}



void file__set_format(Edit e,long fmt)
{
  XwObject.child.setState(e->menu,format_str[e->format],XW_FRAME);
  e->format = fmt;
  ImageFile.setSaveFormat(format_str[e->format]);
  XwObject.child.setState(e->menu,format_str[e->format],XW_REVERSE);
}



void file__redraw_size(Edit e)
{
  XwObject.child.setLabel(e->menu,"x0","X:%d",Edit__image_xsize(e));
  XwObject.child.setLabel(e->menu,"y0","Y:%d",Edit__image_ysize(e));
}



void file__change_format(Edit e,char *format_name)
{
  int i;

  if (format_name == 0) return;

  for (i = 0; i < Number(format_str); i++)
    if (strcmp(format_str[i],format_name) == 0)
      break;
  if (i == Number(format_str)) i = 0;

  file__set_format(e,i);
}



long file__format_sel(Edit e,xw_object obj,XEvent *event,long value)
{
  file__set_format(e,value);

  return 0;
}



long file__change_name(Edit e,xw_object obj)
{
  char buf[512];
  int n;
  image_item item;

  item = Edit__original(e);

  strcpy(buf,imageItem__filename(item));
  XwObject.unmap(obj);
  n = XwGetString(e->menu,obj->x,obj->y+17,buf,512);
  if (n != 0)
    {
      imageItem__set_filename(item,buf);
      imageItem__set_filename(Edit__previous(e),buf);
    }
  XwObject.map(obj);
  XwObject.setLabel(obj,imageItem__filename(item));
  imageItem__set_edit(item);

  return 0;
}



long file__change_comment(Edit e,xw_object obj)
{
  char buf[512];
  int n;
  image_item item;

  item = Edit__original(e);

  strcpy(buf,imageItem__comment(item));
  XwObject.unmap(obj);
  n = XwGetString(e->menu,obj->x,obj->y+17,buf,512);
  if (n != 0)
    {
      imageItem__set_comment(item,buf);
      imageItem__set_comment(Edit__previous(e),buf);
    }
  XwObject.map(obj);
  XwObject.setLabel(obj,imageItem__comment(item));
  imageItem__set_edit(item);

  return 0;
}



long file__exist(char *filename)
{
  FILE *fp;

  fp = fopen(filename,"r");
  if (fp == 0) return 0;
  fclose(fp);

  return 1;
}



long file__save(Edit e)
{
  image_item item;
  char *filename;

  item = Edit__original(e);

  if (imageItem__not_need_save(item))
    {
      XwObject.setLabel(e->message," (No changes need to be saved)");
      imageItem__set_edit(item);
      return 0;
    }

  filename = imageItem__filename(item);
  if (file__exist(filename))
    {
      char com[512];
      XwObject.setLabel(e->message," file exists \"%s\" ! backup !",filename);
      sprintf(com,"mv %s %s~",filename,filename);
      system(com);
      sprintf(com,"cp %s~ %s",filename,filename);
      system(com);
    }

  if (imageItem__save(item,filename) == 0)
    {
      XwObject.setLabel(e->message," can't write \"%s\" !",filename);
      return 0;
    }

  XwObject.setLabel(e->message," Wrote \"%s\"",filename);

  return 0;
}



long file__load(Edit e)
{
  image_item orig,prev;
  char *filename;

  orig = Edit__original(e);
  prev = Edit__previous(e);
  filename = imageItem__filename(orig);

  imageItem__swap(prev,orig);
  if (imageItem__load(orig,filename) == 0)
    {
      XwObject.setLabel(e->message," can't open \"%s\"",filename);
      imageItem__copy(orig,prev);
      return 0;
    }
  if (menu__type_check(e) == 0)
    {
      XwObject.setLabel(e->message,"Bad image type !!");
      imageItem__copy(orig,prev);
      return 0;
    }
  XwObject.setLabel(e->message," read \"%s\"",filename);

  imageItem__swap(prev,orig);
  buffer__copy(e);
  imageItem__swap(prev,orig);

  Edit__set_original(e);

  file__change_format(e,ImageFile.getLoadFormat());
  file__set_info(e);

  return 0;
}
