/*
 * filename : load.c
 * author   : Takahiro Sugiyama
 * date     : from 1993
 * describe : open image files
 */


#include "Xdisp.h"


#define DEBUG 0
void load(app)
     XdispApp app;
{
  long i;
  long load_num = 0;

  for (i = 0; i < app->num; i++)
    {
      struct ITEM *item = &app->list[i];

      if (optspecified("key"))
	{
	  if (DEBUG > 2) fprintf(stderr, "(+create info:create");
	  /*
	  item->info = cil_create(item->name);
	  if (item->info)
	    {
	      if (DEBUG > 2) fprintf(stderr, ":success");
	      strcpy(item->filename, item->info->filename);
	    }
	  fprintf(stderr, "imagedisp:load file (%s)\n", item->info->filename);
	  if (DEBUG > 2) fprintf(stderr, ":close+)");
	  cil_close(item->info);
	  */
	}
      if (Image.load(item->org, item->filename) == 0)
	{
	  fprintf(stderr, "warning:can't open file (%s).\n", item->filename);
	  continue;
	}

      if (item->org)
	{
	  load_num ++;

	  Image.print(item->org);

	  if (typesize(item->org->type) == 1)
	    if (optspecified("char"))
	      item->org->type = Char;

	  if (Image.type(item->org) == Char)
	    {
	      if (optspecified("uchar"))
		item->org->type = UChar;
	      else
		{
		  item->px = XImageFontGetXSize() + XImageFontGetXSpace();
		  item->py = XImageFontGetYSize() + XImageFontGetYSpace();
		}
	    }

	  if (! optspecified("image-size"))
	    {
	      item->rx = Image.xsize(item->org);
	      item->ry = Image.ysize(item->org);
	    }

	  if ((item->ox + item->rx) > item->org->xsize)
	    { item->ox = 0; item->rx = item->org->xsize;}
	  if ((item->oy + item->ry) > item->org->ysize)
	    { item->oy = 0; item->ry = item->org->ysize;}
	}
    }

  if (load_num == 0) exit(0);
}
