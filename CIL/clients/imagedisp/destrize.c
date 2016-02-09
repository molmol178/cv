/*
 * filename : destrize.c
 * author   : Takahiro Sugiyama
 * date     : from 1993
 * describe : destrize routine
 */


#include "Xdisp.h"


void destrize(app)
     XdispApp app;
{
  long i;
  char command[256];

  for (i = 0; i < app->num; i++)
    {
      struct ITEM *item = &app->list[i];

      if (item->org && IS_SHARED(item->org))
	{
	  if (item->org) Image.destroy(item->org);
	  if (item->info)
	    {
	      /*	      item->info = cil_open(item->name);
	      if (item->info) unlink(item->info->filename);
	      cil_close(item->info);*/
	    }
	}
      XDestroyWindow(app->display, item->win);
    }

  free((char *)app->list);
}
