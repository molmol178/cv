/*
 * font to image
 */



#include "Image.h"
#include "ImageFile.h"
#include "Xcil/XImage.h"
#include "misc/optlib.h"
#include "Xw/XwWorld.h"
#include "Xw/XwObject.h"



struct f2iRec {
  xw_world world;
  xw_object panel;
  xw_object show;

  char font[256];
  char kanji[256];

  char text[256];
  char filename[256];

  image img;
  long scale;
};



void f2i__construct(struct f2iRec *f2i,int argc,char *argv[]);
void f2i__open(struct f2iRec *f2i);
void f2i__exec(struct f2iRec *f2i);
void f2i__close(struct f2iRec *f2i);
void f2i__destruct(struct f2iRec *f2i);

long f2i__load_font(struct f2iRec *f2i,char *font);
long f2i__load_kanji(struct f2iRec *f2i,char *font);
void f2i__text_to_image(struct f2iRec *f2i);



static long f2i__change_filename(struct f2iRec *f2i,xw_object o);
static long f2i__change_text(struct f2iRec *f2i,xw_object o);
static long f2i__change_font(struct f2iRec *f2i,xw_object o);
static long f2i__change_kanji(struct f2iRec *f2i,xw_object o);

static long f2i__save(struct f2iRec *f2i);
static long f2i__show(struct f2iRec *f2i);
static long f2i__quit(struct f2iRec *f2i);

static void f2i__get_string(xw_object o,char *text);
static void f2i__get_string_kanji(xw_object o,char *text);

static long f2i__scale(struct f2iRec *f2i,xw_object o,XEvent *e,long value);
static long f2i__show_window_redraw(struct f2iRec *f2i);
void f2i__text_to_image(struct f2iRec *f2i);



int main(int argc,char *argv[])
{
  struct f2iRec f2iR;

  f2i__construct(&f2iR,argc,argv);
  f2i__open(&f2iR);
  f2i__exec(&f2iR);
  f2i__close(&f2iR);
  f2i__destruct(&f2iR);

  return 0;
}



void f2i__construct(struct f2iRec *f2i,int argc,char *argv[])
{
  static char *list[] = {
    "Usage:[options]",
    "help:-h:print this message",
    "file:-f:1:read from file",
    "font:-fn:1:a14:font name",
    "kanji:-fk:1:k14:kanji name",
    "scale:-s:1:4:show window scale",
    "config file ...",
    "\tfilename \"text...\""
  };

  optinit(Number(list),list,argc,argv);
  if (optspecified("help")) optmanual( 1 );

  strcpy(f2i->font,optvalue("font"));
  strcpy(f2i->kanji,optvalue("kanji"));
  f2i->scale = atol(optvalue("scale"));

  f2i->text[0] = '\0';
  f2i->filename[0] = '\0';
  f2i->img = Image.create("img");

  ImageFile.setSaveFormat("J4");
}



#define FitHeight 40
#define FitWidth 500
#define Tx 140
#define Mx ((24+2)*4)

static xw_panel panel[] = {
  {{"cr","Filename :",NoHandler,NoValue,&Label},
     {"labelfilename",NoName,NoName,FitSpace,FitSpace,Tx,FitHeight}},
  {{"cc",NoLabel,(xw_handler)f2i__change_filename,NoValue,&CommandV},
     {"filename",PrevName,SameName,FitSpace,FitSpace,FitWidth,FitHeight}},

  {{"cr","Text :",NoHandler,NoValue,&Label},
     {"labeltext",NoName,PrevName,FitSpace,FitSpace,Tx,FitHeight}},
  {{"cc",NoLabel,(xw_handler)f2i__change_text,NoValue,&CommandV},
     {"text",PrevName,SameName,FitSpace,FitSpace,FitWidth,FitHeight}},

  {{"cr","Fontname :",NoHandler,NoValue,&Label},
     {"labelfont",NoName,PrevName,FitSpace,FitSpace,Tx,FitHeight}},
  {{"cc",NoLabel,(xw_handler)f2i__change_font,NoValue,&CommandV},
     {"font",PrevName,SameName,FitSpace,FitSpace,FitWidth,FitHeight}},

  {{"cr","Kanjiname :",NoHandler,NoValue,&Label},
     {"labelkanji",NoName,PrevName,FitSpace,FitSpace,Tx,FitHeight}},
  {{"cc",NoLabel,(xw_handler)f2i__change_kanji,NoValue,&CommandV},
     {"kanji",PrevName,SameName,FitSpace,FitSpace,FitWidth,FitHeight}},

  {{"cc","Show",(xw_handler)f2i__show,NoValue,&CommandV},
     {"show",NoName,PrevName,FitSpace+100,FitSpace,Mx,FitHeight}},
  {{"cc","Save",(xw_handler)f2i__save,NoValue,&CommandV},
     {"save",PrevName,SameName,FitSpace,FitSpace,Mx,FitHeight}},
  {{"cc","Quit",(xw_handler)f2i__quit,NoValue,&CommandV},
     {"quit",PrevName,SameName,FitSpace,FitSpace,Mx,FitHeight}}
};



static xw_event key_list[] = {
  {'1',(xw_handler)f2i__scale,1},
  {'2',(xw_handler)f2i__scale,2},
  {'3',(xw_handler)f2i__scale,3},
  {'4',(xw_handler)f2i__scale,4},
  {'5',(xw_handler)f2i__scale,5},
  {'6',(xw_handler)f2i__scale,6},
  {'7',(xw_handler)f2i__scale,7},
  {'8',(xw_handler)f2i__scale,8},
  {'9',(xw_handler)f2i__scale,9},
  {'r',(xw_handler)f2i__show_window_redraw,0}
};

static xw_event event_list[] = {
  {Expose,(xw_handler)f2i__show_window_redraw,0}
};

void f2i__open(struct f2iRec *f2i)
{
  f2i->world = XwWorld.create(f2i,"Font to Image");
  f2i->panel = XwObject.create(f2i->world,XwHard.class,0,"Font to Image");
  f2i->show  = XwObject.create(f2i->world,XwHard.class,0,"Show Font Image");

  XwObject.panel(f2i->panel,XwNumber(panel),panel);
  XwObject.configure(f2i->show,0,0,50*f2i->scale,50*f2i->scale,1);
  XwObject.addEvents(f2i->show,XwNumber(event_list),event_list);
  XwObject.addKeyEvents(f2i->show,XwNumber(key_list),key_list);

  XwObject.child.setLabel(f2i->panel,"font",f2i->font);
  XwObject.child.setLabel(f2i->panel,"kanji",f2i->kanji);

  XwObject.map(f2i->panel);
  XwObject.map(f2i->show);

  f2i__load_font(f2i,f2i->font);
  f2i__load_kanji(f2i,f2i->kanji);
}



void f2i__exec(struct f2iRec *f2i)
{
  XwWorld.mainLoop(f2i->world);
}



void f2i__close(struct f2iRec *f2i)
{
  XwWorld.destroy(f2i->world);
}



void f2i__destruct(struct f2iRec *f2i)
{
}



static long f2i__change_filename(struct f2iRec *f2i,xw_object o)
{
  f2i__get_string(o,f2i->filename);

  return 0;
}



static long f2i__change_text(struct f2iRec *f2i,xw_object o)
{
  f2i__get_string_kanji(o,f2i->text);

  return 0;
}



static long f2i__change_font(struct f2iRec *f2i,xw_object o)
{
  char font[256];

  strcpy(font,f2i->font);
  f2i__get_string(o,font);
  if (f2i__load_font(f2i,font) == 0)
    {
      XwObject.setLabel(o,f2i->font);
      return 0;
    }
  strcpy(f2i->font,font);
  XwObject.setLabel(o,font);

  return 0;
}



static long f2i__change_kanji(struct f2iRec *f2i,xw_object o)
{
  char font[256];

  strcpy(font,f2i->kanji);
  f2i__get_string(o,font);
  if (f2i__load_kanji(f2i,font) == 0)
    {
      XwObject.setLabel(o,f2i->kanji);
      return 0;
    }
  strcpy(f2i->kanji,font);
  XwObject.setLabel(o,font);

  return 0;
}



static long f2i__show(struct f2iRec *f2i)
{
  f2i__show_window_redraw(f2i);

  return 0;
}



static long f2i__save(struct f2iRec *f2i)
{
  f2i__show(f2i);

  if (strlen(f2i->text) == 0) return 0;

  while (strlen(f2i->filename) == 0)
    {
      f2i__change_filename(f2i,XwObject.search(f2i->panel,"filename"));
    }

  Image.save(f2i->img,f2i->filename,f2i->text);

  return 0;
}



static long f2i__quit(struct f2iRec *f2i)
{
  return 1;
}



static void f2i__get_string(xw_object o,char *text)
{
  char buf[256];
  int n;

  strcpy(buf,text);
  XwObject.unmap(o);
  n = XwGetString(o->parent,o->x+20,o->y + 3*FitHeight/4,buf,256);
  if (n != 0) strcpy(text,buf);

  XwObject.setLabel(o,text);
  XwObject.map(o);
}



static void f2i__get_string_kanji(xw_object o,char *text)
{
  char buf[256];
  int n;

  strcpy(buf,text);
  XwObject.unmap(o);
  n = XwGetStringKanji(o->parent,o->x+20,o->y + 3*FitHeight/4,buf,256);
  if (n != 0) strcpy(text,buf);

  XwObject.setLabel(o,text);
  XwObject.map(o);
}



long f2i__load_font(struct f2iRec *f2i,char *font)
{
  XFontStruct *fs = XLoadQueryFont(f2i->world->display,font);
  if (fs == 0)
    {
      fprintf(stderr,"can't load font (%s)\n",font);
      return 0;
    }
  XwSetFont(f2i->world,fs->fid);
  return 1;
}



long f2i__load_kanji(struct f2iRec *f2i,char *font)
{
  XFontStruct *fs = XLoadQueryFont(f2i->world->display,font);
  if (fs == 0)
    {
      fprintf(stderr,"can't load font (%s)\n",font);
      return 0;
    }
  XwWorld.setKanji(f2i->world,font);
  return 1;
}



static long f2i__scale(struct f2iRec *f2i,xw_object o,XEvent *e,long value)
{
  f2i->scale = value;
  f2i__show_window_redraw(f2i);
  return 0;
}



static long f2i__show_window_redraw(struct f2iRec *f2i)
{
  f2i__text_to_image(f2i);

  XShowImageFull(f2i->world->display,f2i->show->id,f2i->world->gc,f2i->img);

  return 0;
}



void f2i__text_to_image(struct f2iRec *f2i)
{
  XCharStruct over;
  int dir,asc,des;
  long xsize,ysize;

  if (strlen(f2i->text) == 0) return;

  XwTextExtents(f2i->show,f2i->text,&dir,&asc,&des,&over);

  xsize = over.rbearing + over.lbearing + 2;
  ysize = asc + des + 2;

  XwObject.resize(f2i->show, xsize * f2i->scale,ysize * f2i->scale);

  Image.make(f2i->img,Bit1,xsize,ysize);

  XwObject.clear(f2i->show);
  XwPutString(f2i->show,0,asc,f2i->text);

  {
    XImage *img;
    int pixmap_format;
    long x,y;
    bit1 **data = (bit1 **)Image.data(f2i->img);
    Display *d = f2i->world->display;

    pixmap_format = DisplayPlanes(d,DefaultScreen(d)) > 1 ? ZPixmap : XYPixmap;
    img = XGetImage(f2i->world->display,f2i->show->id,
		    0,0,xsize,ysize,AllPlanes,pixmap_format);
    for (y = 0; y < ysize; y++)
      for (x = 0; x < xsize; x++)
	if (XGetPixel(img,x,y))
	  data[y+1][x+1] = 1;
  }
}
