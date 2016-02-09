/*
 * filename : exec.c
 * author   : Takahiro Sugiyama
 * date     : from 1993
 * describe : event handers
 *
 * update   : Friday, February 7 1997 by Takahiro Sugiyama
 * describe : translate Xworld library to Xcil library
 */


#include "Xdisp.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "ImageDisp.h"
#include <string.h>
#include <stdlib.h>


extern double atof();

#define MV_LEFT  0
#define MV_DOWN  1
#define MV_UP    2
#define MV_RIGHT 3
#define MV_LEFT_C  4
#define MV_DOWN_C  5
#define MV_UP_C    6
#define MV_RIGHT_C 7

/* Context Key */
#define XDISP_ITEM 0x44495350 /* DISP */

int VerboseMode = 0;
int ScalingMode = SCALE_PIXEL;

static char *key[] = {
  "q/Q ... quit (all)",
  "r ... redraw",
  "R ... reverse",
  "t ... liner scale translate (toggle)",
  "e ... histogram translate (toggle)",
  "o ... original image",
  "I ... infomation of image",
  "? ... print this message",
  "c ... load image",
  "S ... silence mode",
  "V ... verbose mode",
  "v ... small region's value display",
  "w ... scaling window mode",
  "p ... scaling pixel mode",
  "i ... scaling image mode",
  "d ... scaling defined window size mode",
  "1-9 ... scale image/window",
  "SHIFT + 1-9 ... 0.? scale pixel/window",
  "h/H ... move left",
  "j/J ... move down",
  "k/K ... move up",
  "l/L ... move right",
  "f ... resize window to fit image",
  "s ... scaling image to fit window",
  "F ... display property is feature",
  "G ... display property is gray",
  "N ... display property is label",
  "C ... display property is color label",
  "Button1 ... print pixel infomation",
  "Button2 ... move to point (rel)",
  "Button3 ... move to point (abs)",
  0
};

static void server_trans_event(display, item, context, event, type)
     Display *display;
     struct ITEM *item;
     XContext context;
     XEvent *event;
     long type;
{
  long x, y, width, height;
  char string[10];
  KeySym key;
  CILEvent cilevent;

  XcilGetWindowSize(display, item->win, &width, &height);

  x = item->ox + item->rx * event->xbutton.x / width;
  y = item->oy + item->ry * event->xbutton.y / height;

  cilevent.any.type = type;
  cilevent.any.detail = 0;
  cilevent.any.x = x;
  cilevent.any.y = y;
  cilevent.any.window_x = event->xbutton.x;
  cilevent.any.window_y = event->xbutton.y;

  switch(event->type)
    {
    case Expose:
    case MotionNotify:
    case EnterNotify:
    case LeaveNotify:
      break;

    case ButtonPress:
    case ButtonRelease:
      cilevent.mouse.button = event->xbutton.button;
      break;

    case KeyPress:
    case KeyRelease:
      cilevent.keyboard.ascii = 0;
      cilevent.keyboard.keysym = 0;
      if (XLookupString((XKeyEvent*)event, string, 10, &key, 0) == 1)
	{
	  cilevent.keyboard.ascii = string[0];
	}
      cilevent.keyboard.keysym = key;
      break;
    }
  /*
  item->info = cil_open(item->name);
  if (item->info) item->info->event = cilevent;
  cil_close(item->info);
  */
}

struct CILEventDetail {
  long xe;
  long ie;
  long imask;
} cil_event_detail[] = {
  {Expose, 	 ImageExpose, 	 ImageExposeMask},
  {MotionNotify, MouseMove, 	 MouseMoveMask},
  {ButtonPress, 	 MousePress, 	 MousePressMask},
  {ButtonRelease, MouseRelease, 	 MouseReleaseMask},
  {EnterNotify, 	 MouseEnter, 	 MouseEnterMask},
  {LeaveNotify, 	 MouseLeave, 	 MouseLeaveMask},
  {KeyPress, 	 KeyboardPress, 	 KeyboardPressMask},
  {KeyRelease, 	 KeyboardRelease, KeyboardReleaseMask}
};
#define DEBUG 0
static long event_store(Display *display,
			Window win,
			XContext context,
			XEvent *e)
{
  struct ITEM *item;
  int i;

  if (DEBUG > 2) fprintf(stderr, "(@store");

  item = (struct ITEM *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  if (item->info == 0) return 0;

  if (DEBUG > 2) fprintf(stderr, ":open");
  /*
  item->info = cil_open(item->name);
  if (item->info == 0) return 0;
  for (i = 0; i < Number(cil_event_detail); i++)
    {
      struct CILEventDetail *detail = &cil_event_detail[i];
      if ((e->type == detail->xe) && (item->info->mask & detail->imask))
	{
	  if (DEBUG > 2) fprintf(stderr, ":find");
	  server_trans_event(display, item, context, e, detail->ie);
	  break;
	}
    }
  if (DEBUG > 2) fprintf(stderr, ":close");
  cil_close(item->info);
  */
  if (DEBUG > 2) fprintf(stderr, "@)");

  return 0;
}



long destroy_window(Display *display,
		    Window win,
		    XContext context,
		    XEvent *e,
		    XdispApp app)
{
  int i;
  struct ITEM *item;

  item = (struct ITEM *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  if (VerboseMode) fprintf(stderr, "quit %s\n", item->name);

  if (item->info)
    {
      /*      item->info = cil_open(item->name);
      if (item->info)
	{
	  item->info->imagedispexec = 0;
	  if (item->info->name[0] != '@')
	    {
	      if (strncmp("/tmp/#", item->info->filename, 5) == 0)
		unlink(item->info->filename);
	    }
	}
	cil_close(item->info);*/
    }

  if (item->win) XDestroyWindow(display, win);
  Image.destroy(item->org);
  item->org = 0;
  item->win = 0;

  for (i = 0; i < app->num; i++)
    if (app->list[i].win) break;
  if (i == app->num) return 1;

  return 0;
}



long quit_all(Display *display,
	      Window win,
	      XContext context,
	      XEvent *e,
	      XdispApp app)
{
  int i;
  if (VerboseMode) fprintf(stderr, "all quit\n");

  for (i = 0; i < app->num; i++)
    {
      struct ITEM *item = &app->list[i];
      if (item->org) destroy_window(display, item->win, context, e, app);
    }
  return 1;
}



static long verbose_on(Display *display,
		       Window win,
		       XContext context,
		       XEvent *e)
{
  VerboseMode = 1;
  fprintf(stderr, "verbose mode\n");
  return 0;
}



static long verbose_off(Display *display,
			Window win,
			XContext context,
			XEvent *e)
{
  if (VerboseMode) fprintf(stderr, "silence mode\n");
  VerboseMode = 0;
  return 0;
}



void item__free_image(item)
     struct ITEM *item;
{
  Image.free(item->img);
}


void item__make_image(item)
     struct ITEM *item;
{
  Image.copy(item->img, item->org);
  if (item->reveresed) image__reverse(item->img, item->img);
  if (item->transed)  image__liner(item->img, item->img);
  if (item->histflat) image__histogram_flattering(item->img, item->img, 256);
}


static long sized_check_and_resized(display, item)
     Display *display;
     struct ITEM *item;
{
  long wx, wy;

  XcilGetWindowSize(display, item->win, &wx, &wy);

  if ((wx != item->wx) || (wy != item->wy))
    {/* Window resize */
      item->wx = wx;
      item->wy = wy;
      bestmatch_rxry(item);
    }
  else
    {/* others resize */
      wx = item->wx;
      wy = item->wy;
    }

  bestmatch_wxwy(item);
  if ((wx != item->wx) || (wy != item->wy))
    {
      XResizeWindow(display, item->win, item->wx, item->wy);
      return 1;
    }

  return 0;
}


long redraw_window(Display *display,
		   Window win,
		   XContext context,
		   XEvent *e)
{
  struct ITEM *item;

  item = (struct ITEM *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  if ((e !=  0) && (item->info)) event_store(display, win, context, e);

  if (VerboseMode) fprintf(stderr, "redraw window %s\n", item->name);

  item__make_image(item);

  if (sized_check_and_resized(display, item) == 1) return 0;

  {
    char *name = item->img->name;
    item->img->name = item->org->name;
    XShowImagePartOfImage(display, win, Xcil_black_gc, item->img,
			  item->ox, item->oy, item->rx, item->ry);
    item->img->name = name;
  }

  XFlush(display);

  item__free_image(item);

  return 0;
}



static long change_win(Display *display,
		       Window win,
		       XContext context,
		       XEvent *e)
{
  ScalingMode = SCALE_WINDOW;

  if (VerboseMode) fprintf(stderr, "scaleing is window\n");

  return 0;
}

static long change_pix(Display *display,
		       Window win,
		       XContext context,
		       XEvent *e)
{
  ScalingMode = SCALE_PIXEL;

  if (VerboseMode) fprintf(stderr, "scaleing is pixl\n");

  return 0;
}

static long change_img(Display *display,
		       Window win,
		       XContext context,
		       XEvent *e)
{
  ScalingMode = SCALE_IMAGE;

  if (VerboseMode) fprintf(stderr, "scaleing is image\n");

  return 0;
}



static long change_def(Display *display,
		       Window win,
		       XContext context,
		       XEvent *e)
{
  ScalingMode = SCALE_DEFINE;

  if (VerboseMode) fprintf(stderr, "scaleing is defined\n");

  return 0;
}



static long reverse_window(Display *display,
			   Window win,
			   XContext context,
			   XEvent *e)
{
  struct ITEM *item;

  item = (struct ITEM *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  if (VerboseMode) fprintf(stderr, "reverse image %s\n", item->name);

  item->reveresed = 1 - item->reveresed;
  redraw_window(display, win, context, e);

  if (VerboseMode) fprintf(stderr, "complete reverse ...\n");

  return 0;
}



static long trans_window(Display *display,
			 Window win,
			 XContext context,
			 XEvent *e)
{
  long i, befree = 0;
  struct ITEM *item;

  item = (struct ITEM *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  if (VerboseMode) fprintf(stderr, "liner scale trans image %s\n", item->name);

  item->transed = 1 - item->transed;
  redraw_window(display, win, context, e);

  if (VerboseMode) fprintf(stderr, "complete translate ...\n");

  return 0;
}



static long histflat_window(Display *display,
			    Window win,
			    XContext context,
			    XEvent *e)
{
  long i, befree = 0;
  struct ITEM *item;

  item = (struct ITEM *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  if (VerboseMode) fprintf(stderr, "histgram flattering trans image %s\n", item->name);

  item->histflat = 1 - item->histflat;
  redraw_window(display, win, context, e);

  if (VerboseMode) fprintf(stderr, "complete flattering ...\n");

  return 0;
}



static long load_image(Display *display,
		       Window win,
		       XContext context,
		       XEvent *e)
{
  struct ITEM *item;

  item = (struct ITEM *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  if (VerboseMode) fprintf(stderr, "load %s\n", item->filename);
  Image.load(item->org, item->filename);

  if ((item->ox + item->rx) > item->org->xsize)
    {
      item->ox = 0;
      if (item->rx > item->org->xsize) item->rx = item->org->xsize;
    }
  if ((item->oy + item->ry) > item->org->ysize)
    {
      item->oy = 0;
      if (item->ry > item->org->ysize) item->ry = item->org->ysize;
    }

  redraw_window(display, win, context, e);
  if (VerboseMode) fprintf(stderr, "complete load ...\n");

  return 0;
}



static long print_key()
{
  int i = 0;

  printf("key operations.\n");
  while (key[i]) {
    putchar('\t');
    puts(key[i]);
    i++;
  }

  return 0;
}



static long original_image(Display *display,
			   Window win,
			   XContext context,
			   XEvent *e)
{
  long befree = 0;
  struct ITEM *item;

  item = (struct ITEM *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  if (VerboseMode) fprintf(stderr, "original image %s\n", item->name);

  item->reveresed = 0;
  item->transed = 0;
  item->histflat = 0;
  redraw_window(display, win, context, e);

  return 0;
}



static long scale_image(Display *display,
			Window win,
			XContext context,
			XEvent *e,
			char *scale_str)
{
  long i;
  long xsize, ysize;
  double scale;
  struct ITEM *item;

  if (VerboseMode) fprintf(stderr, "scale ... ");

  item = (struct ITEM *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  if (VerboseMode) fprintf(stderr, "%s's ", item->name);

  scale = atof(scale_str);

  switch (ScalingMode)
    {
    case SCALE_WINDOW:
      if (VerboseMode) fprintf(stderr, "window");
      item->rx = item->rx * scale;
      item->ry = item->ry * scale;
      break;

    case SCALE_DEFINE:
      if (VerboseMode) fprintf(stderr, "define");
      if (scale < 1) scale = scale * 10 + 9;

      if (Image.type(item->org) != Char)
	{
	  item->px = 64 * scale / item->rx;
	  item->py = 64 * scale / item->ry;
	}
      else
	{
	  item->rx = 64 * scale / item->px;
	  item->ry = 64 * scale / item->py;
	}

      break;

    case SCALE_PIXEL:
      if (VerboseMode) fprintf(stderr, "pixel");
      if (Image.type(item->org) != Char)
	{
	  item->px = scale;
	  item->py = scale;
	  item->rx = item->wx / item->px;
	  item->ry = item->wy / item->py;
	}
      break;

    case SCALE_IMAGE:
      if (VerboseMode) fprintf(stderr, "image");
      if (Image.type(item->org) != Char)
	{
	  item->px = scale;
	  item->py = scale;
	}
      break;
    }
  if (VerboseMode) fprintf(stderr, "\n");

  if ((item->ox + item->rx) > item->org->xsize)
    {
      item->ox = 0;
      if (item->rx > item->org->xsize) item->rx = item->org->xsize;
    }
  if ((item->oy + item->ry) > item->org->ysize)
    {
      item->oy = 0;
      if (item->ry > item->org->ysize) item->ry = item->org->ysize;
    }

  redraw_window(display, win, context, e);

  return 0;
}



static long property_change(Display *display,
			    Window win,
			    XContext context,
			    XEvent *e,
			    char *prop_str)
{
  static char *type_list[] = {
    0, 0, 0, "CIL_DP_BIT4", "CIL_DP_CHAR", "CIL_DP_SHORT", "CIL_DP_LONG",
    "CIL_DP_LONG", "CIL_DP_UCHAR", "CIL_DP_USHORT", "CIL_DP_ULONG",
    "CIL_DP_ULONG", "CIL_DP_FLOAT", "CIL_DP_DOUBLE"};
  char buf[512], *str;
  struct ITEM *item;

  item = (struct ITEM *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;
  if ((Bit4 > __TYPE(item->org)) || (__TYPE(item->org) > Double))
    return 0;

  if (VerboseMode) fprintf(stderr, "%s's property change to %s\n",
			    item->name, prop_str);

  sprintf(buf, "%s=%s", type_list[__TYPE(item->org)], prop_str);
  str = strcpy((char*)malloc(strlen(buf)+1), buf);
  putenv(str);
  redraw_window(display, win, context, e);

  return 0;
}



static long information_image(Display *display,
			      Window win,
			      XContext context,
			      XEvent *e)
{
  struct ITEM *item;
  long i, befree = 0;
  Window root;
  int x, y;
  unsigned int width, height, border, depth;

  item = (struct ITEM *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  printf("information of image %s\n", item->name);

  Image.print(item->org);
  printf("Window %s = {\n", item->name);
  XGetGeometry(display, win, &root, &x, &y, &width, &height, &border, &depth);
  printf("    ID     = %d\n", win);
  printf("    x      = %d\n", x);
  printf("    y      = %d\n", y);
  printf("    width  = %d\n", width);
  printf("    height = %d\n", height);
  printf("}\n");

  return 0;
}



static long new_position(Display *display,
			 Window win,
			 XContext context,
			 XEvent *e)
{
  long x, y, width, height;
  long px, py, ox, oy;
  long xsize, ysize;
  struct ITEM *item;

  item = (struct ITEM *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  xsize = Image.xsize(item->org);
  ysize = Image.ysize(item->org);

  XcilGetWindowSize(display, win, &width, &height);
  px = item->ox + item->rx * e->xbutton.x / width;
  py = item->oy + item->ry * e->xbutton.y / height;

  switch (e->xbutton.button)
    {
    case Button2:/* window rel */
      ox = px - item->rx / 2;
      oy = py - item->ry / 2;
      break;
    case Button3:/* window abs */
      ox = xsize * e->xbutton.x / width  - item->rx / 2;
      oy = ysize * e->xbutton.y / height - item->ry / 2;
      break;
    case Button1:
      ox = item->ox;
      oy = item->oy;
      break;
    }

  if ((ox + item->rx) > xsize) ox = xsize - item->rx;
  if ((oy + item->ry) > ysize) oy = ysize - item->ry;
  if (ox < 0) ox = 0;
  if (oy < 0) oy = 0;

  if ((ox != item->ox) || (oy != item->oy))
    {
      item->ox = ox;
      item->oy = oy;
      return 1;
    }

  return 0;
}


static long information_pixel(Display *display,
			      Window win,
			      XContext context,
			      XEvent *e)
{
  long i, befree = 0;
  long width, height;
  long px, py;
  char **data;
  long size;
  struct ITEM *item;
  long type;

  item = (struct ITEM *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  if (item->info) event_store(display, win, context, e);

  if (e->xbutton.button != Button1)
    {
      if (new_position(display, win, context, e))
	redraw_window(display, win, context, e);
      return 0;
    }

  XcilGetWindowSize(display, win, &width, &height);
  px = item->ox + item->rx * e->xbutton.x / width;
  py = item->oy + item->ry * e->xbutton.y / height;

  type = Image.type(item->org);
  size = typesize(Image.type(item->org));
  data = Image.data(item->org);

  if (px < 0 || py < 0) return 0;
  if (px >= __XSIZE(item->org) || py >= __YSIZE(item->org)) return 0;

  printf("%s (x = %d, y = %d) : ", typename(type), px, py);

  if (item->reveresed) printf("[reveresed] : ");
  if (item->transed) printf("[transed] : ");
  if (item->histflat) printf("[histflat] : ");

  switch (type)
    {
    case UChar:
      printf("(i = %u)\n", (ulong)*(uchar *)PTR_2(data, px, py, size));
      break;

    case UShort:
      printf("(i = %u)\n", (ulong)*(ushort *)PTR_2(data, px, py, size));
      break;

    case Bit4:
      printf("(i = %u)\n", (ulong)*(bit4 *)PTR_2(data, px, py, size));
      break;

    case Bit1:
      printf("(i = %u)\n", (ulong)*(bit1 *)PTR_2(data, px, py, size));
      break;

    case ULong:
      printf("(i = %u)\n", (ulong)*(ulong *)PTR_2(data, px, py, size));
      break;

    case Char:
      {
	char ch = *(char *)PTR_2(data, px, py, size);
	char pch = (ch + 128) % 128;

	if (isprint(pch))
	  printf("(i = %4d['%c', 0x%02X])\n", (long)ch, pch, ch&0xff);
	else
	  printf("(i = %4d[' ', 0x%02X])\n", (long)ch, ch&0xff);
      }
      break;

    case Short:
      printf("(i = %d)\n", (long)*(short *)PTR_2(data, px, py, size));
      break;

    case Long:
      printf("(i = %d)\n", (long)*(long *)PTR_2(data, px, py, size));
      break;

    case Float:
      printf("(i = %.3f)\n", (double)*(float *)PTR_2(data, px, py, size));
      break;

    case UChar3:
      {
	uchar3 *p = (uchar3 *)PTR_2(data, px, py, size);
	printf("(r = %u, g = %u, b = %u)\n",
	       (ulong)p->at[0], (ulong)p->at[1], (ulong)p->at[2]);
      }
      break;

    case UShort3:
      {
	ushort3 *p = (ushort3 *)PTR_2(data, px, py, size);
	printf("(r = %u, g = %u, b =%u)\n",
	       (ulong)p->at[0], (ulong)p->at[1], (ulong)p->at[2]);
      }
      break;

    case Short3:
      {
	short3 *p = (short3 *)PTR_2(data, px, py, size);
	printf("(r = %d, g = %d, b = %d)\n",
	       (long)p->at[0], (long)p->at[1], (long)p->at[2]);
      }
      break;

    case Short2:
      {
	short2 *p = (short2 *)PTR_2(data, px, py, size);
	printf("(x = %d, y = %d)\n", (long)p->at[0], (long)p->at[1]);
      }
      break;

    default:
      printf("\n");
    }

  return 0;
}


static long move_image(Display *display,
		       Window win,
		       XContext context,
		       XEvent *e,
		       long wh)
{
  long px, py, ox, oy;
  long xsize, ysize;
  struct ITEM *item;

  item = (struct ITEM *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  xsize = Image.xsize(item->org);
  ysize = Image.ysize(item->org);

  ox = item->ox;
  oy = item->oy;
  switch (wh)
    {
    case MV_LEFT: ox--; break;
    case MV_RIGHT: ox++; break;
    case MV_UP: oy--; break;
    case MV_DOWN: oy++; break;
    case MV_LEFT_C: ox -= 8; break;
    case MV_RIGHT_C: ox += 8; break;
    case MV_UP_C: oy -= 8; break;
    case MV_DOWN_C: oy += 8; break;
    }

  if ((ox + item->rx) > xsize) ox = xsize - item->rx;
  if ((oy + item->ry) > ysize) oy = ysize - item->ry;
  if (ox < 0) ox = 0;
  if (oy < 0) oy = 0;

  if ((ox != item->ox) || (oy != item->oy))
    {
      item->ox = ox;
      item->oy = oy;
      redraw_window(display, win, context, e);
    }

  return 0;
}


static long fit_window(Display *display,
		       Window win,
		       XContext context,
		       XEvent *e)
{
  struct ITEM *item;

  item = (struct ITEM *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  if (VerboseMode) fprintf(stderr, "fit image %s\n", item->name);

  item->ox = 0;
  item->oy = 0;
  item->rx = item->org->xsize;
  item->ry = item->org->ysize;
  redraw_window(display, win, context, e);

  if (VerboseMode) fprintf(stderr, "complete reverse ...\n");

  return 0;
}



static long fit_image(Display *display,
		      Window win,
		      XContext context,
		      XEvent *e)
{
  struct ITEM *item;

  item = (struct ITEM *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  if (VerboseMode) fprintf(stderr, "fit image %s\n", item->name);

  if (Image.type(item->org) == Char) return 0;
  item->ox = 0;
  item->oy = 0;
  item->rx = item->org->xsize;
  item->ry = item->org->ysize;
  item->px = (double)item->wx / item->rx;
  item->py = (double)item->wy / item->ry;
  redraw_window(display, win, context, e);

  if (VerboseMode) fprintf(stderr, "complete reverse ...\n");

  return 0;
}


enum {
  is_bit1, is_char,
  is_long, is_long2, is_long3,
  is_ulong, is_ulong2, is_ulong3,
  is_float, is_float2, is_float3
};

struct print_valueRec {
  long type;
  long type_is;
  char fmt[30];
  char blank[30];
  long n;
  union {
    char  c[3];
    long  l[3];
    ulong u[3];
    float f[3];
  } v;
} print_value_list[] = {
  {Bit1, is_bit1, "%c", "*"},
  {Bit4, is_long, "%2u", "**"},

  {Char, is_char, "[%c:%02x]", "******"},

  {UChar, is_ulong, "%3u",          "***"},
  {UChar2, is_ulong2, "[%3u:%3u]",    "*********"},
  {UChar3, is_ulong3, "[%3u:%3u:%3u]", "*************"},

  {Short, is_long, "%6d",          "******"},
  {Short2, is_long2, "[%4d:%4d]",    "***********"},
  {Short3, is_long3, "[%6d:%6d:%6d]", "**********************"},

  {UShort, is_ulong, "%6u",          "******"},
  {UShort2, is_ulong2, "[%6u:%6u]",    "***************"},
  {UShort3, is_ulong3, "[%6u:%6u:%6u]", "**********************"},

  {Long, is_long, "%6d",          "******"},
  {Long2, is_long2, "[%6d:%6d]",    "***************"},
  {Long3, is_long3, "[%6d:%6d:%6d]", "**********************"},

  {ULong, is_ulong, "%6u",          "******"},
  {ULong2, is_ulong2, "[%6u:%6u]",    "***************"},
  {ULong3, is_ulong3, "[%6u:%6u:%6u]", "**********************"},

  {Float, is_float, "%7.3f",              "*******"},
  {Float2, is_float2, "[%3.0f:%3.0f]",      "*********"},
  {Float3, is_float3, "[%3.0f:%3.0f:%3.0f]", "*************"}
};



static struct print_valueRec *set_print_values(type)
     long type;
{
  int i;

  for (i = 0; i < Number(print_value_list); i++)
    if (print_value_list[i].type == type) break;
  if (i == Number(print_value_list)) return 0;

  print_value_list[i].n = 80/(strlen(print_value_list[i].blank)+1);

  return &print_value_list[i];
}



static void print_each_value P ((struct print_valueRec *pvdata, char **data,
				 long xx, long yy));


static long show_value(Display *display,
		       Window win,
		       XContext context,
		       XEvent *e)
{
  long i;
  long x, y, width, height;
  long px, py;
  char **data;
  struct ITEM *item;
  long type;
  long xsize, ysize;
  struct print_valueRec *pvdata;

  item = (struct ITEM *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  XcilGetWindowSize(display, win, &width, &height);
  px = item->ox + item->rx * e->xbutton.x / width;
  py = item->oy + item->ry * e->xbutton.y / height;

  type = Image.type(item->org);
  data = Image.data(item->org);

  xsize = Image.xsize(item->org);
  ysize = Image.ysize(item->org);

  printf("%s (x = %d, y = %d) : ", typename(type), px, py);

  if (item->reveresed) printf("[reveresed] : ");
  if (item->transed) printf("[transed] : ");
  printf("\n");

  pvdata = set_print_values(type);
  if (pvdata == 0) return 0;

  for (y = 0; y < pvdata->n; y++)
    {
      long yy = y + py - pvdata->n/2;

      for (x = 0; x < pvdata->n; x++)
	{
	  long xx = x + px - pvdata->n/2;

	  if (xx < 0 || xx >= xsize || yy < 0 || yy >= ysize)
	    printf(pvdata->blank);
	  else
	    print_each_value(pvdata, data, xx, yy);

	  if (x == (pvdata->n-1)) printf("\n");
	  else
	    {
	      if ((xx == px-1) && (yy == py)) printf("<");
	      else
	      if ((xx == px) && (yy == py)) printf(">");
	      else
		printf(" ");
	    }
	}
    }

  return 0;
}



static void let_in_value(pvdata, data, xx, yy)
     struct print_valueRec *pvdata;
     char **data;
     long xx, yy;
{
  if (pvdata->type == Bit1)
    pvdata->v.l[0] = ((bit1 **)data)[yy][xx];

  if (pvdata->type == Bit4)
    pvdata->v.l[0] = ((bit4 **)data)[yy][xx];

  if (pvdata->type == Char)
    pvdata->v.c[0] = ((char **)data)[yy][xx];

  if (pvdata->type == UChar)
    pvdata->v.u[0] = ((uchar **)data)[yy][xx];

  if (pvdata->type == UChar2)
    {
      pvdata->v.u[0] = ((uchar2 **)data)[yy][xx].at[0];
      pvdata->v.u[1] = ((uchar2 **)data)[yy][xx].at[1];
    }

  if (pvdata->type == UChar3)
    {
      pvdata->v.u[0] = ((uchar3 **)data)[yy][xx].at[0];
      pvdata->v.u[1] = ((uchar3 **)data)[yy][xx].at[1];
      pvdata->v.u[2] = ((uchar3 **)data)[yy][xx].at[2];
    }

  if (pvdata->type == Short)
    pvdata->v.l[0] = ((short **)data)[yy][xx];

  if (pvdata->type == Short2)
    {
      pvdata->v.l[0] = ((short2 **)data)[yy][xx].at[0];
      pvdata->v.l[1] = ((short2 **)data)[yy][xx].at[1];
    }

  if (pvdata->type == Short3)
    {
      pvdata->v.l[0] = ((short3 **)data)[yy][xx].at[0];
      pvdata->v.l[1] = ((short3 **)data)[yy][xx].at[1];
      pvdata->v.l[2] = ((short3 **)data)[yy][xx].at[2];
    }

  if (pvdata->type == UShort)
    pvdata->v.u[0] = ((ushort **)data)[yy][xx];

  if (pvdata->type == UShort2)
    {
      pvdata->v.u[0] = ((ushort2 **)data)[yy][xx].at[0];
      pvdata->v.u[1] = ((ushort2 **)data)[yy][xx].at[1];
    }

  if (pvdata->type == UShort3)
    {
      pvdata->v.u[0] = ((ushort3 **)data)[yy][xx].at[0];
      pvdata->v.u[1] = ((ushort3 **)data)[yy][xx].at[1];
      pvdata->v.u[2] = ((ushort3 **)data)[yy][xx].at[2];
    }

  if (pvdata->type == Long)
    pvdata->v.l[0] = ((long **)data)[yy][xx];

  if (pvdata->type == Long2)
    {
      pvdata->v.l[0] = ((long2 **)data)[yy][xx].at[0];
      pvdata->v.l[1] = ((long2 **)data)[yy][xx].at[1];
    }

  if (pvdata->type == Long3)
    {
      pvdata->v.l[0] = ((long3 **)data)[yy][xx].at[0];
      pvdata->v.l[1] = ((long3 **)data)[yy][xx].at[1];
      pvdata->v.l[2] = ((long3 **)data)[yy][xx].at[2];
    }

  if (pvdata->type == ULong)
    pvdata->v.u[0] = ((ulong **)data)[yy][xx];

  if (pvdata->type == ULong2)
    {
      pvdata->v.u[0] = ((ulong2 **)data)[yy][xx].at[0];
      pvdata->v.u[1] = ((ulong2 **)data)[yy][xx].at[1];
    }

  if (pvdata->type == ULong3)
    {
      pvdata->v.u[0] = ((ulong3 **)data)[yy][xx].at[0];
      pvdata->v.u[1] = ((ulong3 **)data)[yy][xx].at[1];
      pvdata->v.u[2] = ((ulong3 **)data)[yy][xx].at[2];
    }

  if (pvdata->type == Float)
    pvdata->v.f[0] = ((float **)data)[yy][xx];

  if (pvdata->type == Float2)
    {
      pvdata->v.f[0] = ((float2 **)data)[yy][xx].at[0];
      pvdata->v.f[1] = ((float2 **)data)[yy][xx].at[1];
    }

  if (pvdata->type == Float3)
    {
      pvdata->v.f[0] = ((float3 **)data)[yy][xx].at[0];
      pvdata->v.f[1] = ((float3 **)data)[yy][xx].at[1];
      pvdata->v.f[2] = ((float3 **)data)[yy][xx].at[2];
    }
}



static void print_each_value(pvdata, data, xx, yy)
     struct print_valueRec *pvdata;
     char **data;
     long xx, yy;
{
  let_in_value(pvdata, data, xx, yy);

  switch(pvdata->type_is)
    {
    case is_char:
      {
	if (isprint(pvdata->v.c[0]))
	  printf(pvdata->fmt, pvdata->v.c[0], pvdata->v.c[0]);
	else
	  printf(pvdata->fmt, ' ', pvdata->v.c[0]);
	break;
      }

    case is_bit1:
      {
	if (pvdata->v.l[0])
	  printf("o");
	else
	  printf(".");
	break;
      }

    case is_long:
      printf(pvdata->fmt, pvdata->v.l[0]);break;
    case is_long2:
      printf(pvdata->fmt, pvdata->v.l[0], pvdata->v.l[1]);break;
    case is_long3:
      printf(pvdata->fmt, pvdata->v.l[0], pvdata->v.l[1], pvdata->v.l[2]);break;

    case is_ulong:
      printf(pvdata->fmt, pvdata->v.u[0]);break;
    case is_ulong2:
      printf(pvdata->fmt, pvdata->v.u[0], pvdata->v.u[1]);break;
    case is_ulong3:
      printf(pvdata->fmt, pvdata->v.u[0], pvdata->v.u[1], pvdata->v.u[2]);break;

    case is_float:
      printf(pvdata->fmt, pvdata->v.f[0]);break;
    case is_float2:
      printf(pvdata->fmt, pvdata->v.f[0], pvdata->v.f[1]);break;
    case is_float3:
      printf(pvdata->fmt, pvdata->v.f[0], pvdata->v.f[1], pvdata->v.f[2]);break;
    }
}



static long client_event(Display *display,
			 Window win,
			 XContext context,
			 XClientMessageEvent *e,
			 XdispApp app)
{
  int i, state;
  struct ITEM *item;

  if (DEBUG > 2) fprintf(stderr, "(@clientevent");

  if (DEBUG > 2) fprintf(stderr, ":search");
  item = (struct ITEM *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  if (DEBUG > 2) fprintf(stderr, ":find:exec");
  state = server_exec(app, item);

  if (DEBUG > 2) fprintf(stderr, ":update");
  /*  item->info = cil_open(item->name);
  if ((item->info) && (item->win)) item->info->id = item->win;
  cil_close(item->info);
  */
  if (DEBUG > 2) fprintf(stderr, ":end@)");
  return state;
}



static long map_notify(Display *display,
		       Window win,
		       XContext context,
		       XEvent *e)
{
  int i;
  struct ITEM *item;

  if (DEBUG > 1) fprintf(stderr, "Get Map Event\n");

  item = (struct ITEM *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item==0) return 0;
  /*
  item->info = cil_open((unsigned char *)item->info);
  if (item->info)
    {
      item->info->id = item->win;
      item->info->imagedispexec = 1;
      item->info->event.type = NoEvent;
      item->info->command = 0;
    }
  cil_close(item->info);
  */
  return 0;
}



void exec(app)
     XdispApp app;
{
  long i;

  static XcilCallback share_entry[] = {
    {MotionNotify,  (xcil_callback)event_store},
    {KeyPress,      (xcil_callback)event_store},
    {KeyRelease,    (xcil_callback)event_store},
    {EnterNotify,   (xcil_callback)event_store},
    {LeaveNotify,   (xcil_callback)event_store},
    {ButtonRelease, (xcil_callback)event_store},
    {ClientMessage, (xcil_callback)client_event},
    {MapNotify,     (xcil_callback)map_notify}
  };

  static XcilCallback entry[] = {
    {ButtonPress,   (xcil_callback)information_pixel},
    {Expose,        (xcil_callback)redraw_window}
  };

  static XcilCallback key_entry[] = {
    {'q', (xcil_callback)destroy_window},
    {'Q', (xcil_callback)quit_all},
    {'R', (xcil_callback)reverse_window},
    {'t', (xcil_callback)trans_window},
    {'e', (xcil_callback)histflat_window},
    {'p', (xcil_callback)change_pix},
    {'w', (xcil_callback)change_win},
    {'i', (xcil_callback)change_img},
    {'d', (xcil_callback)change_def},
    {'r', (xcil_callback)redraw_window},
    {'o', (xcil_callback)original_image},
    {'I', (xcil_callback)information_image},
    {'v', (xcil_callback)show_value},
    {'?', (xcil_callback)print_key},
    {'c', (xcil_callback)load_image},
    {'V', (xcil_callback)verbose_on},
    {'S', (xcil_callback)verbose_off},
    {'1', (xcil_callback)scale_image, (caddr_t)"1"},
    {'2', (xcil_callback)scale_image, (caddr_t)"2"},
    {'3', (xcil_callback)scale_image, (caddr_t)"3"},
    {'4', (xcil_callback)scale_image, (caddr_t)"4"},
    {'5', (xcil_callback)scale_image, (caddr_t)"5"},
    {'6', (xcil_callback)scale_image, (caddr_t)"6"},
    {'7', (xcil_callback)scale_image, (caddr_t)"7"},
    {'8', (xcil_callback)scale_image, (caddr_t)"8"},
    {'9', (xcil_callback)scale_image, (caddr_t)"9"},
    {'!', (xcil_callback)scale_image, (caddr_t)"0.1"},
    {'@', (xcil_callback)scale_image, (caddr_t)"0.2"},
    {'#', (xcil_callback)scale_image, (caddr_t)"0.3"},
    {'$', (xcil_callback)scale_image, (caddr_t)"0.4"},
    {'%', (xcil_callback)scale_image, (caddr_t)"0.5"},
    {'^', (xcil_callback)scale_image, (caddr_t)"0.6"},
    {'&', (xcil_callback)scale_image, (caddr_t)"0.7"},
    {'*', (xcil_callback)scale_image, (caddr_t)"0.8"},
    {'(', (xcil_callback)scale_image, (caddr_t)"0.9"},
    {'h', (xcil_callback)move_image, (caddr_t)MV_LEFT},
    {'j', (xcil_callback)move_image, (caddr_t)MV_DOWN},
    {'k', (xcil_callback)move_image, (caddr_t)MV_UP},
    {'l', (xcil_callback)move_image, (caddr_t)MV_RIGHT},
    {'H', (xcil_callback)move_image, (caddr_t)MV_LEFT_C},
    {'J', (xcil_callback)move_image, (caddr_t)MV_DOWN_C},
    {'K', (xcil_callback)move_image, (caddr_t)MV_UP_C},
    {'L', (xcil_callback)move_image, (caddr_t)MV_RIGHT_C},
    {'f', (xcil_callback)fit_window},
    {'s', (xcil_callback)fit_image},
    {'F', (xcil_callback)property_change, (caddr_t)"feature"},
    {'G', (xcil_callback)property_change, (caddr_t)"gray"},
    {'N', (xcil_callback)property_change, (caddr_t)"label"},
    {'C', (xcil_callback)property_change, (caddr_t)"color-label"},
  };

  share_entry[6].client_value = (caddr_t)app; /* client message */
  key_entry[0].client_value = (caddr_t)app; /* quit */
  key_entry[1].client_value = (caddr_t)app; /* quit all */

  VerboseMode = optspecified("verbose");

  if (VerboseMode) print_key(app);

  if (VerboseMode) fprintf(stderr, "Window initialize ... \n");
  for (i = 0; i < app->num; i++)
    {
      struct ITEM *item = &app->list[i];

      if (item->win == 0) continue;
      XcilAddCallbacks(app->display, item->win, app->context,
		       entry, XcilNumber(entry));
      XcilAddKeyCallbacks(app->display, item->win, app->context,
			  key_entry, XcilNumber(key_entry));
      XcilAddWindowContext(app->display, item->win, app->context,
			   XDISP_ITEM, (caddr_t)item);
      if (item->info)
	{
	  XcilAddCallbacks(app->display, item->win, app->context,
			   share_entry, XcilNumber(share_entry));
	}
    }

  if (VerboseMode) fprintf(stderr, "start imagedisp ... \n");

  XcilEventLoop(app->display, app->context);
}
