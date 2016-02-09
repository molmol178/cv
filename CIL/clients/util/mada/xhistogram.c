/*
 * xhistogram.c
 */



#include "Image.h"
#include "ImageFile.h"
#include "Image/Funcs.h"
#include "Xcil/XImage.h"
#include "Xcil/Xcil.h"
#include "misc/optlib.h"



typedef struct {
  Display *display;
  XContext context;
  Window panel;
  Window histogram;

  image input;
  image masking;
  long  binary;
  long  liner;

  long order;
  long range;
  long ymax;
  long yran;

  long max;
  long min;
} XH;



void xh_init  P ((XH *h,int argc,char *argv[]));
void xh_open  P ((XH *h));
void xh_exec  P ((XH *h));
void xh_disp  P ((XH *h));
void xh_close P ((XH *h));



void xh_panelRedraw P ((XH *h));
void xh_histogramDraw P ((XH *h));
void xh_panelDraw P ((XH *h));


#define XO 8*8
#define YO 16*2



XH hR;
XH *h = &hR;



int main(argc,argv)
     int argc;
     char *argv[];
{
  xh_init(h,argc,argv);
  xh_open(h);
  xh_exec(h);
  xh_close(h);

  return 0;
}



static char *opt_item[] = {
  "help:-h:print help message",
  "reverse:-r:reverse image",
  "order:-or:1:10:order of frequency",
  "ymax:-ym:1:300:y maximum",
  "yrange:-yr:1:100:y range",
  "level-min:-min:1:0:label minimun",
  "level-max:-max:1:255:label maximun"
};

void xh_init(h,argc,argv)
     XH *h;
     int argc;
     char *argv[];
{
  h->order = 10;

  optinit(Number(opt_item),opt_item,argc,argv);
  if (optargc == 0)
    {
      optmanual("Usage : xhistogram [options] <image> [options]");
      exit(-1);
    }
  h->input = Image.createFromFilename(optargv[0],optargv[0]);
  if (h->input == 0) exit(-1);
  if (optspecified("reverse")) image__reverse(h->input,h->input);
  h->masking = Image.create("dest");
  Image.copy(h->masking,h->input);
  h->order = optvalueint("order");
  h->ymax  = optvalueint("ymax");
  h->yran  = optvalueint("yrange");
  h->max  = optvalueint("level-max");
  h->min  = optvalueint("level-min");
  h->binary = 1;
  h->liner = 1;

  fprintf(stderr,"xhistogram = {\n");
  fprintf(stderr,"    filename = %s\n",optargv[0]);
  fprintf(stderr,"    ymax     = %d\n",h->ymax);
  fprintf(stderr,"    yrange   = %d\n",h->yran);
  fprintf(stderr,"    min      = %d\n",h->min);
  fprintf(stderr,"    max      = %d\n",h->max);
  fprintf(stderr,"}\n");
}



static long xh_expose(d,w,c,e)
     Display *d;
     Window w;
     XContext c;
     XEvent *e;
{
  if (w == h->panel)
    xh_panelRedraw(h);
  else
    xh_histogramDraw(h);

  return 0;
}

static long xh_press(d,w,c,e)
     Display *d;
     Window w;
     XContext c;
     XEvent *e;
{
  char buf[512];

  if (w != h->histogram) return;

  switch(e->xbutton.button)
    {
    case Button1:
      h->min = e->xbutton.x - XO;
      if (h->min > h->max) h->max = h->min;
      break;
    case Button2:
      h->min = e->xbutton.x - XO;
      h->max = e->xbutton.x - XO;
      break;
    case Button3:
      h->max = e->xbutton.x - XO;
      if (h->min > h->max) h->min = h->max;
      break;
    }
  if (h->max > 255) h->max = 255;
  if (h->min < 0) h->min = 0;
  if (h->min > 255) h->min = 255;
  if (h->max < 0) h->max = 0;

  XcilSetString(h->display,h->panel,h->context,"min","min = %d", h->min);
  XcilSetString(h->display,h->panel,h->context,"max","max = %d", h->max);

  xh_histogramDraw(h);

  return 0;
}

static long xh_quit()
{
  return 1;
}



long xh_apply P ((XH *h));
long xh_origin P ((XH *h));
long xh_bin P ((XH *h));
long xh_line P ((XH *h));
long xh_order P ((XH *h));
long xh_range P ((XH *h));

#define TW 120
#define AW 200
static XcilPanelItem panel_item[] = {
  {{"title",__TOP,__TOP,700},{XCIL_3D_LABEL,"The histogram cutter"}},

  {{"image",__TOP,__PRE,256,256},{XCIL_3D_LABEL}},

  {{"t1", __PRE,__SAM   },{XCIL_3D_LABEL,"Filename : "}},
  {{"t2", __SAM,__PRE,TW},{XCIL_3D_LABEL,"Comment : "}},
  {{"t3", __SAM,__PRE,TW},{XCIL_3D_LABEL,"Type : "}},
  {{"t4", __SAM,__PRE,TW},{XCIL_3D_LABEL,"XSize x YSize : "}},

  {{"filename","t1","title",AW},{XCIL_3D_LABEL}},
  {{"comment", "t2","t1",AW},{XCIL_3D_LABEL}},
  {{"type","t3","t2",AW},{XCIL_3D_LABEL}},
  {{"size","t4","t3",AW},{XCIL_3D_LABEL}},

  {{"ap","image","size"},{XCIL_3D_COMMAND,"Apply"},{xh_apply}},
  {{"or","ap","size"},{XCIL_3D_COMMAND,"Original"},{xh_origin}},
  {{"bin","or","size"},{XCIL_3D_COMMAND,"Binary"},{xh_bin}},
  {{"lin","bin","size"},{XCIL_3D_COMMAND,"Liner"},{xh_line}},
  {{"quit","lin","size"},{XCIL_3D_COMMAND,"Quit"},{xh_quit}},

  {{"ord-","image","ap"},{XCIL_3D_COMMAND,"order--"},{xh_order,1}},
  {{"ord+","ord-","ap"},{XCIL_3D_COMMAND,"order++"},{xh_order,2}},
  {{"ran-","ord+","ap"},{XCIL_3D_COMMAND,"range--"},{xh_range,1}},
  {{"ran+","ran-","ap"},{XCIL_3D_COMMAND,"range++"},{xh_range,2}},

  {{"min","image","ord-"},{XCIL_3D_LABEL," min = *** "}},
  {{"max","min","ord-"},{XCIL_3D_LABEL," max = *** "}},
  {{"order","max","ord-"},{XCIL_3D_LABEL," order = *** "}},
  {{"range","order","ord-"},{XCIL_3D_LABEL," range = *** "}},
};

#define Min(a,b) (((a) < (b)) ? (a) : (b))

void xh_open()
{
  h->display     = XcilOpenDisplay(0);
  h->context    = XUniqueContext();
  h->panel     = XcilCreatePanelWindow(h->display,0,h->context,0,0,panel_item,XcilNumber(panel_item),"X Histogram");
  h->histogram = XcilCreateWindow(h->display,0,200,0,256+8*14,h->ymax+16*4,1,"HISTOGRAM");
}



static XcilCallback event_list[] = {
  {Expose,(xcil_callback)xh_expose},
  {ButtonPress,(xcil_callback)xh_press}
};

static XcilCallback event_list2[] = {
  {Expose,(xcil_callback)xh_expose}
};

static XcilCallback key_list[] = {
  {'q',(xcil_callback)xh_quit},
  {'Q',(xcil_callback)xh_quit}
};

void xh_exec()
{
  XcilAddCallbacks(h->display,h->panel,h->context,event_list2,XcilNumber(event_list2));
  XcilAddKeyCallbacks(h->display,h->panel,h->context,key_list,XcilNumber(key_list));

  XcilAddCallbacks(h->display,h->histogram,h->context,event_list,XcilNumber(event_list));
  XcilAddKeyCallbacks(h->display,h->histogram,h->context,key_list,XcilNumber(key_list));

  XcilMapWindow(h->display,h->panel);
  XcilMapWindow(h->display,h->histogram);
  XFlush(h->display);
  xh_disp(h);

  XcilEventLoop(h->display,h->context);
}



void xh_panelDraw()
{
  Window w;
  char buf[512];

  XcilSetString( h->display, h->panel, h->context, "filename", Image.name(h->input));
  XcilSetString( h->display, h->panel, h->context, "comment", "%s", ImageFile.getComment());
  XcilSetString( h->display, h->panel, h->context, "type", typename(__TYPE(h->input)));
  XcilSetString( h->display, h->panel, h->context, "size", "%d x %d",__XSIZE(h->input),__YSIZE(h->input));
  XcilSetString( h->display, h->panel, h->context, "min", "min = %d",h->min);
  XcilSetString( h->display, h->panel, h->context, "max", "max = %d",h->max);
  XcilSetString( h->display, h->panel, h->context, "order", "order = %d",h->order);
  XcilSetString( h->display, h->panel, h->context, "range", "range = %d",h->range);

  w = XcilPanelItemWindow(h->display,h->panel,h->context,"image");
  XShowImageFull(h->display,w,Xcil_black_gc,h->masking);
}



void xh_panelRedraw()
{
  Window w;

  w = XcilPanelItemWindow(h->display,h->panel,h->context,"image");
  XShowImageFull(h->display,w,Xcil_black_gc,h->masking);
}



void xh_close()
{
}



void xh_disp()
{
  xh_panelDraw(h);
  xh_histogramDraw(h);
}



void xh_histogramDrawUChar P ((XH *h));
void xh_histogramDrawUChar3 P ((XH *h));

void xh_histogramDraw()
{
  if (Image.type(h->input) == UChar)
    xh_histogramDrawUChar(h);
  else
  if (Image.type(h->input) == UChar3)
    xh_histogramDrawUChar3(h);
}




static char dash_list[] = {1,1};

void xh_histogramDrawUChar()
{
  long freq[256];
  long i,n;
  register uchar *p;
  char buf[256];

  XClearWindow(h->display,h->histogram);

  n = Image.area(h->input);
  p = (uchar *)Image.raster(h->input);
  for (i = 0; i < 256; i++) freq[i] = 0;
  for (i = 0; i < n; i++) freq[p[i]]++;

  XDrawLine(h->display,h->histogram,Xcil_black_gc,XO,YO/2,XO,YO+h->ymax+1);
  XDrawLine(h->display,h->histogram,Xcil_black_gc,XO,YO+h->ymax+1,256+XO,YO+h->ymax+1);

  XSetDashes(h->display,Xcil_black_gc,0,dash_list,2);
  XSetLineAttributes(h->display,Xcil_black_gc,0,LineOnOffDash,CapNotLast,JoinMiter);
  for (i = 0; i < h->min; i ++)
    {
      long ymax = YO+h->ymax - freq[i]/h->order;
      if (ymax < 0) ymax = 0;
      XSetDashes(h->display,Xcil_black_gc,i%2,dash_list,2);
      XDrawLine(h->display,h->histogram,Xcil_black_gc,XO+i,YO+h->ymax,XO+i,ymax);
    }

  XSetLineAttributes(h->display,Xcil_black_gc,0,LineSolid,CapNotLast,JoinMiter);
  for (i = h->min; (i <= h->max) && (i < 256); i ++)
    {
      long ymax = YO+h->ymax - freq[i]/h->order;
      if (ymax < 0) ymax = 0;
      XDrawLine(h->display,h->histogram,Xcil_black_gc,XO+i,YO+h->ymax,XO+i,ymax);
    }

  XSetLineAttributes(h->display,Xcil_black_gc,0,LineOnOffDash,CapNotLast,JoinMiter);
  for (i = h->max+1; i < 256; i ++)
    {
      long ymax = YO+h->ymax - freq[i]/h->order;
      if (ymax < 0) ymax = 0;
      XSetDashes(h->display,Xcil_black_gc,i%2,dash_list,2);
      XDrawLine(h->display,h->histogram,Xcil_black_gc,XO+i,YO+h->ymax,XO+i,ymax);
    }

  XSetLineAttributes(h->display,Xcil_black_gc,0,LineSolid,CapNotLast,JoinMiter);

  for (i = 0; i <= h->ymax / h->yran; i++)
    {
      long y = YO + h->ymax - i * h->yran;
      sprintf(buf,"%d",i*h->order*h->yran);
      XcilPutStringBox(h->display,h->histogram,Xcil_black_gc,XO-5,y,-1,1,"cr",buf);
      if (i != 0) XDrawLine(h->display,h->histogram,Xcil_black_gc,XO,y,256+XO,y);
    }
  XcilPutStringBox(h->display,h->histogram,Xcil_black_gc,256+XO,YO+h->ymax+5,-1,-1,"tl","255");
}



long xh_apply()
{
  long i,n;
  register uchar *p,*s;
  Window w;

  n = Image.area(h->input);
  p = (uchar *)Image.raster(h->masking);
  s = (uchar *)Image.raster(h->input);

  if (! h->binary)
    {
      if (h->liner)
	{
	  for (i = 0; i < n; i++)
	    {
	      if (h->max < s[i])
		p[i] = h->max;
	      else
		if (h->min > s[i])
		  p[i] = h->min;
		else
		  p[i] = s[i];
	    }
	  image__liner(h->masking,h->masking);
	}
      else
	{
	  for (i = 0; i < n; i++)
	    {
	      if (h->max < s[i])
		p[i] = 255;
	      else
		if (h->min > s[i])
		  p[i] = 0;
		else
		  p[i] = s[i];
	    }
	}
    }
  else
    {
      for (i = 0; i < n; i++)
	{
	  if (h->max < s[i])
	    p[i] = 255;
	  else
	  if (h->min > s[i])
	    p[i] = 255;
	  else
	    p[i] = 0;
	}
    }

  w = XcilPanelItemWindow(h->display,h->panel,h->context,"image");
  XShowImageFull( h->display, w, Xcil_black_gc, h->masking );

  return 0;
}


long xh_origin()
{
  long i,n;
  register uchar *p,*s;
  Window w;

  n = Image.area(h->input);
  p = (uchar *)Image.raster(h->masking);
  s = (uchar *)Image.raster(h->input);
  for (i = 0; i < n; i++) p[i] = s[i];

  w = XcilPanelItemWindow(h->display,h->panel,h->context,"image");
  XShowImageFull(h->display,w,Xcil_black_gc,h->masking);

  return 0;
}


void xh_histogramDrawUChar3()
{
}



long xh_bin()
{
  h->binary = 1 - h->binary;
  if (h->binary)
    XcilSetString(h->display,h->panel,h->context,"bin","Binary");
  else
    XcilSetString(h->display,h->panel,h->context,"bin","Gray");

  xh_apply(h);

  return 0;
}



long xh_line()
{
  h->liner = 1 - h->liner;
  if (h->liner)
    XcilSetString(h->display,h->panel,h->context,"lin","Liner");
  else
    XcilSetString(h->display,h->panel,h->context,"lin","Normal");

  xh_apply(h);

  return 0;
}



long xh_order(d,w,c,e,v)
     Display *d;
     Window w;
     XContext c;
     XEvent *e;
     long v;
{
  char buf[512];

  switch(v)
    {
    case 2:
      if (h->order >= 100) h->order += 100;
      else if (h->order >= 10) h->order += 10;
      else h->order++;
      break;
    case 1:
      if (h->order <= 10) h->order--;
      else if (h->order <= 100) h->order -= 10;
      else h->order -= 100;
      break;
    }
  if (h->order <= 0) h->order = 1;
  xh_histogramDraw(h);

  XcilSetString(h->display,h->panel,h->context,"order","order = %d", h->order);

  return 0;
}



long xh_range(d,w,c,e,v)
     Display *d;
     Window w;
     XContext c;
     XEvent *e;
     long v;
{
  char buf[512];

  switch(v)
    {
    case 2:
      if (h->yran >= 100) h->yran += 100;
      else if (h->yran >= 10) h->yran += 10;
      break;
    case 1:
      if (h->yran <= 100) h->yran -= 10;
      else h->yran -= 100;
      break;
    }
  if (h->yran <= 20) h->yran = 20;
  xh_histogramDraw(h);

  XcilSetString(h->display,h->panel,h->context,"range","range = %d", h->range);

  return 0;
}
