/*
 * Edit.c
 */



#include "Edit.h"



void Edit__set_original(Edit e)
{
  Edit__set_region(e,0,0,Edit__image_xsize(e),Edit__image_ysize(e));
  imageItem__set_mask(Edit__original(e));
  Edit__drawn_make(e);
  file__set_info(e);
  edit__redraw(e);
}



void Edit__set_region(Edit e,long x,long y,long xsize,long ysize)
{
  e->x = x;
  e->y = y;
  e->xsize = xsize;
  e->ysize = ysize;

  panel__reconstruct(e);
}



void Edit__backup(Edit e)
{
  imageItem__copy(Edit__previous(e),Edit__original(e));
}



void Edit__drawn_clear(Edit e)
{
  register bit1 **data;
  register int x,y;

  Image.clear(e->drawn);

/*
  data = (bit1 **)Image.data(e->drawn);
  for (y = e->miny; y <= e->maxy; y++)
    for (x = e->minx; x <= e->maxx; x++)
      data[y][x] = 0;
*/
  e->maxx = e->maxy = 0;
  e->minx = Edit__image_xsize(e);
  e->miny = Edit__image_ysize(e);
}



void Edit__drawn_clear_area(Edit e,int x1,int y1,int x2,int y2)
{
  register bit1 **data;
  register int x,y;

  data = (bit1 **)Image.data(e->drawn);
  for (y = y1; y <= y2; y++)
    for (x = x1; x <= x2; x++)
      data[y][x] = 0;

  e->maxx = e->maxy = 0;
  e->minx = Edit__image_xsize(e);
  e->miny = Edit__image_ysize(e);
}



void Edit__drawn_make(Edit e)
{
  Image.make(e->drawn,Bit1,Edit__image_xsize(e),Edit__image_ysize(e));
  e->maxx = e->maxy = 0;
  e->minx = Edit__image_xsize(e);
  e->miny = Edit__image_ysize(e);
}
