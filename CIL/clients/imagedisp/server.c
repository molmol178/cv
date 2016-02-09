/*
 * filename : server.c
 * author   : Takahiro Sugiyama
 * date     : from 1993
 * describe : imagedisp server recepter
 */


#include "Xdisp.h"
#include <X11/Xutil.h>
#include "ImageDisp.h"


static XdispApp APP;


#define DEBUG 0

static void server_ok(item)
     struct ITEM *item;
{
}


static long server_quit(item)
     struct ITEM *item;
{
  /*
  item->info = cil_open(item->name);
  if (item->info) item->info->imagedispexec = 0;
  cil_close(item->info);
  */
  return destroy_window(APP, item->win, 0);
}


static void server_draw(item)
     struct ITEM *item;
{
  if (! IS_SHARED(item->org)) Image.load(item->org, item->filename);

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

  redraw_window(APP, item->win, 0);
}


static void server_draw_area(item)
     struct ITEM *item;
{
  if (! IS_SHARED(item->org)) Image.load(item->org, item->filename);

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

  redraw_window(APP, item->win, 0);
}



long server_exec(app, item)
     XdispApp app;
     struct ITEM *item;
{
  long state;
  long num;
  long command;

  APP = app;

  state = 0;
  /*
  item->info = cil_open(item->name);
  if (item->info) command = item->info->command; else return 1;
  cil_close(item->info);
  */
  switch (command)
    {
    case CIL_OK:
      server_ok(item);
      break;
    case CIL_QUIT:
      state = server_quit(item);
      break;
    case CIL_DRAW:
      server_draw(item);
      break;
    case CIL_DRAW_AREA:
      server_draw_area(item);
      break;
    default:
      fprintf(stderr, "imagedisp:server_exec:unknown type (%d)\n", command);
    }

  return state;
}
