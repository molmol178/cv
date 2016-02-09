/*
 * filename : disp.c
 * author   : Takahiro Sugiyama
 * date     : from 1993
 * describe : display routine
 */


#include "Xdisp.h"


void disp(XdispApp app)
{
  long i;

  for (i = 0; i < app->num; i++)
    {
      struct ITEM *item = &app->list[i];
      item__make_image(item);
      if (Image.area(item->img))
	{
	  char *name = item->img->name;
	  item->img->name = item->org->name;
	  XShowImagePartOfImage(app->display,item->win,
				Xcil_black_gc,item->img,
				item->ox,item->oy,item->rx,item->ry);
	  item->img->name = name;
	}
    }
}
