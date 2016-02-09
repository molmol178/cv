/*
 * filename : open.c
 * author   : Takahiro Sugiyama
 * date     : from 1993
 * describe : open windows, and interrupt routines
 */


#include "Xdisp.h"
#include <X11/Xutil.h>
#include <signal.h>


static XdispApp App;


extern long quit_all P ((Display *display, Window win, XContext context, XEvent *e, XdispApp app));


void break_handler(sig)
     long sig;
{
  quit_all(App->display,0,App->context,0,App);
  destrize(App);

  exit(0);
}



void bestmatch_rxry(item)
     struct ITEM *item;
{
  if (Image.type(item->org) != Char)
    {
      item->rx = item->wx / item->px;
      item->ry = item->wy / item->py;
    }
  else
    {
      item->rx = (item->wx - XImageFontGetXSpace()) / item->px;
      item->ry = (item->wy - XImageFontGetYSpace()) / item->py;
    }

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
}



long MAX_WX;
long MAX_WY;

void bestmatch_wxwy(item)
     struct ITEM *item;
{
  item->wx = item->rx * item->px;
  item->wy = item->ry * item->py;

  if (Image.type(item->org) == Char)
    {
      item->wx += XImageFontGetXSpace();
      item->wy += XImageFontGetYSpace();
    }

  if ((item->wx > MAX_WX) || (item->wy > MAX_WY))
    {
      if (item->wx > MAX_WX) item->wx = MAX_WX;
      if (item->wy > MAX_WY) item->wy = MAX_WY;
      bestmatch_rxry(item);      
    }
}



void set_max_window_size(app)
     XdispApp app;
{
  Window root;
  Display *d;
  Window rr;
  int x,y;
  unsigned int w,h,b,dp;

  d = app->display;
  root = DefaultRootWindow(d);

  XGetGeometry(app->display,root,&rr,&x,&y,&w,&h,&b,&dp);
  XFlush(d);

  MAX_WX = w;
  MAX_WY = h;

  if (app->debug) fprintf(stderr,"RootWindowSize = (%d,%d)\n",w,h);
}


void xopen(app)
     XdispApp app;
{
  long i;
  long id;
  int xoff = 10, yoff = 10;
  int xstep = 25, ystep = 25;
  int xoffstart = xoff;
  int ymax;

  set_max_window_size(app);

  if (optspecified("tile"))
    {
       xoff = 5; yoff = 5;
       xstep = 5; ystep = 25;
       ymax = 0;
    }
  else
  if (optspecified("center"))
    {
      xoff = yoff = 0;
    }
  else
    {
       xoff = 10; yoff = 10;
       xstep = 15; ystep = 25;
       xoffstart = xoff;
    }

  for (i = 0; i < app->num; i++)
    {
      struct ITEM *item = &app->list[i];

      if (Image.area(item->org) == 0) continue;

      bestmatch_wxwy(item);

      item->win = XcilCreateWindow(app->display, 0, xoff, yoff,
				   item->wx, item->wy,
				   5, item->name);
      if (optspecified("tile"))
	{
	  xoff += (xstep + item->wx);
	  if (ymax < item->wy) ymax = item->wy;
	  if (xoff > (MAX_WX-item->wx))
	    { yoff += (ystep + ymax); xoff = 5;
	      if (yoff > (MAX_WY-ymax)) {yoff = 5;}
	      ymax = 0;
	    }
	}
      else
      if (optspecified("center"))
	{
	}
      else
	{
	  xoff += xstep; yoff += ystep;
	  if ((xoff > (MAX_WX-item->wx)) || (yoff > (MAX_WY-item->wy)))
	    { xoffstart += 4*xstep; xoff = xoffstart; yoff = 10; }
	}
      XcilMapWindow(app->display, item->win);
      XcilManagedWindow(app->display, item->win, app->context);
    }

  if (signal(SIGINT,SIG_IGN) != SIG_IGN) signal(SIGINT,break_handler);
  signal(SIGTERM,break_handler);
  signal(SIGQUIT,break_handler);
  signal(SIGSTOP,break_handler);
  App = app;
}
