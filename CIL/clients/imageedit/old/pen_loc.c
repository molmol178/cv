/*
 * pen_loc.c
 */



#include "Edit.h"



extern Edit ed;



void pen__bit1_draw(uchar **data,long x,long y,long value)
{
  data[y][x] = 1;
}



void pen__make(Edit e)
{
  long size = e->pen.size;

  if (size <= 0) size = 1;

  Image.make(e->pen.pat,Bit1,size,size);

  switch (e->pen.shape)
    {
    case SHAPE_BOX:
      CG.fillBox(Image.data(e->pen.pat),e->pen.bit1_pen,0,0,size-1,size-1);
      break;
    case SHAPE_CIRCLE:
      CG.fillCircle4(Image.data(e->pen.pat),e->pen.bit1_pen,0,0,size-1,size-1);
      break;
    }
}



void pen__draw_bit1(bit1 **data,long x,long y)
{
  bit1 src = ed->pen.binary;
  bit1 dst = data[y][x];

  switch(ed->pen.func)
    {
    case FUNC_SET:
      data[y][x] = src;
      break;
    case FUNC_NOT:
      data[y][x] = (! dst);
      break;
    case FUNC_MAX:
    case FUNC_OR:
    case FUNC_ADD:
      data[y][x] = dst | src;
      break;
    case FUNC_AND:
    case FUNC_MIN:
    case FUNC_MEAN:
      data[y][x] = dst & src;
      break;
    case FUNC_XOR:
      data[y][x] = (dst ^ src) & 1;
      break;
    case FUNC_SUB:
      data[y][x] = (src && (! dst));
      break;
    }
}



void pen__draw_uchar(uchar **data,long x,long y)
{
  uchar src = ed->pen.gray;
  uchar dst = data[y][x];
  switch(ed->pen.func)
    {
    case FUNC_SET:
      data[y][x] = src;
      break;
    case FUNC_MAX:
      data[y][x] = maximum(src,dst);
      break;
    case FUNC_MIN:
      data[y][x] = minimum(src,dst);
      break;
    case FUNC_MEAN:
      data[y][x] = (src + dst) / 2;
      break;
    case FUNC_NOT:
      data[y][x] = ~dst;
      break;
    case FUNC_OR:
      data[y][x] = dst | src;
      break;
    case FUNC_AND:
      data[y][x] = dst & src;
      break;
    case FUNC_XOR:
      data[y][x] = dst ^ src;
      break;
    case FUNC_ADD:
      {
	long value = dst + src;
	if (value > 255) value = 255;
	data[y][x] = value;
      }
      break;
    case FUNC_SUB:
      {
	long value = dst - src;
	if (value < 0) value = 0;
	data[y][x] = value;
      }
      break;
    }
}



void pen__draw_uchar3(uchar3 **data,long x,long y)
{
  uchar3 src = ed->pen.color;
  uchar3 dst = data[y][x];
  switch(ed->pen.func)
    {
    case FUNC_SET:
      data[y][x] = src;
      break;
    case FUNC_MAX:
      data[y][x].at[0] = maximum(src.at[0],dst.at[0]);
      data[y][x].at[1] = maximum(src.at[1],dst.at[1]);
      data[y][x].at[2] = maximum(src.at[2],dst.at[2]);
      break;
    case FUNC_MIN:
      data[y][x].at[0] = minimum(src.at[0],dst.at[0]);
      data[y][x].at[1] = minimum(src.at[1],dst.at[1]);
      data[y][x].at[2] = minimum(src.at[2],dst.at[2]);
      break;
    case FUNC_MEAN:
      data[y][x].at[0] = (src.at[0] + dst.at[0]) / 2;
      data[y][x].at[1] = (src.at[1] + dst.at[1]) / 2;
      data[y][x].at[2] = (src.at[2] + dst.at[2]) / 2;
      break;
    case FUNC_NOT:
      data[y][x].at[0] = ~dst.at[0];
      data[y][x].at[1] = ~dst.at[1];
      data[y][x].at[2] = ~dst.at[2];
      break;
    case FUNC_OR:
      data[y][x].at[0] = src.at[0] | dst.at[0];
      data[y][x].at[1] = src.at[1] | dst.at[1];
      data[y][x].at[2] = src.at[2] | dst.at[2];
      break;
    case FUNC_AND:
      data[y][x].at[0] = src.at[0] & dst.at[0];
      data[y][x].at[1] = src.at[1] & dst.at[1];
      data[y][x].at[2] = src.at[2] & dst.at[2];
      break;
    case FUNC_XOR:
      data[y][x].at[0] = src.at[0] ^ dst.at[0];
      data[y][x].at[1] = src.at[1] ^ dst.at[1];
      data[y][x].at[2] = src.at[2] ^ dst.at[2];
    case FUNC_ADD:
      {
	long value0,value1,value2;
	value0 = dst.at[0] + src.at[0];
	value1 = dst.at[1] + src.at[1];
	value2 = dst.at[2] + src.at[2];
	if (value0 > 255) value0 = 255;
	if (value1 > 255) value1 = 255;
	if (value2 > 255) value2 = 255;
	data[y][x].at[0] = value0;
	data[y][x].at[1] = value1;
	data[y][x].at[2] = value2;
      }
      break;
    case FUNC_SUB:
      {
	long value0,value1,value2;
	value0 = dst.at[0] - src.at[0];
	value1 = dst.at[1] - src.at[1];
	value2 = dst.at[2] - src.at[2];
	if (value0 < 0) value0 = 0;
	if (value1 < 0) value1 = 0;
	if (value2 < 0) value2 = 0;
	data[y][x].at[0] = value0;
	data[y][x].at[1] = value1;
	data[y][x].at[2] = value2;
      }
      break;
    }
}



void pen__draw(char **data,long x,long y,long value)
{
  register bit1 **pat,**draw;
  int ix,iy,c,size;
  long px,py;
  long type;
  long xsize,ysize;

  pat   = (bit1 **)Image.data(ed->pen.pat);
  draw  = (bit1 **)Image.data(ed->drawn);
  xsize = Edit__image_xsize(ed);
  ysize = Edit__image_ysize(ed);
  type  = Edit__image_type(ed);
  size = ed->pen.size;
  c = size / 2;

  for (iy = 0; iy < size; iy++)
    {
      py = iy - c + y;
      if ((py < 0) || (py >= ysize)) continue;
      for (ix = 0; ix < size; ix++)
	{
	  px = ix - c + x;
	  if ((px < 0) || (px >= xsize)) continue;

	  if (draw[py][px]) continue;
	  if (pat[iy][ix] == 0) continue;

	  if (ed->pen.select)
	    {
	      if (type == UChar)
		{
		  long min,max;
		  long value = ((uchar **)data)[py][px];
		  min = minimum(ed->pen.ch1.gray,ed->pen.ch2.gray);
		  max = maximum(ed->pen.ch1.gray,ed->pen.ch2.gray);
		  if (ed->pen.negative)
		    {
		      if ((min <= value) && (value <= max)) continue;
		    }
		  else
		    {
		      if (!((min <= value) && (value <= max))) continue;
		    }
		}
	      else
	      if (type == UChar3)
		{
		  long min[3],max[3],val[3];
		  uchar3 ch1,ch2;
		  val[0] = ((uchar3 **)data)[py][px].at[0];
		  val[1] = ((uchar3 **)data)[py][px].at[1];
		  val[2] = ((uchar3 **)data)[py][px].at[2];
		  ch1 = ed->pen.ch1.color;
		  ch2 = ed->pen.ch2.color;
		  min[0] = minimum(ch1.at[0],ch2.at[0]);
		  max[0] = maximum(ch1.at[0],ch2.at[0]);
		  min[1] = minimum(ch1.at[1],ch2.at[1]);
		  max[1] = maximum(ch1.at[1],ch2.at[1]);
		  min[2] = minimum(ch1.at[2],ch2.at[2]);
		  max[2] = maximum(ch1.at[2],ch2.at[2]);
		  if (ed->pen.negative)
		    {
		      if (((min[0] <= val[0]) && (val[0] <= max[0])) &&
			  ((min[1] <= val[1]) && (val[1] <= max[1])) &&
			  ((min[2] <= val[2]) && (val[2] <= max[2])))
			continue;
		    }
		  else
		    {
		      if (!(((min[0] <= val[0]) && (val[0] <= max[0])) &&
			    ((min[1] <= val[1]) && (val[1] <= max[1])) &&
			    ((min[2] <= val[2]) && (val[2] <= max[2]))))
			continue;
		    }
		}
	    }

	  draw[py][px] = 1;

	  if (ed->maxx < px) ed->maxx = px;
	  if (ed->minx > px) ed->minx = px;
	  if (ed->maxy < py) ed->maxy = py;
	  if (ed->miny > py) ed->miny = py;

	  if (((rand() / 100) % 100) > ed->pen.dots) continue;

	  if (type == Bit1) pen__draw_bit1((bit1 **)data,px,py);
	  else
	  if (type == UChar) pen__draw_uchar((uchar **)data,px,py);
	  else
	  if (type == UChar3) pen__draw_uchar3((uchar3 **)data,px,py);
	}
    }
}
