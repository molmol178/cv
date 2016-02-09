/*
 * dilation.c
 */



#include "Funcs.h"



static void dilation_bit1(dest,original,str_elem,cx,cy)
     image dest;
     image original;
     image str_elem;
     long cx;
     long cy;
{
  image src;
  register bit1 **spp, **dpp, **epp;
  register long x,y;
  long xsize,ysize;
  long lxsize,lysize;

  image__type_check("image__dilation",original,Bit1);

  src = Image.create("original-moat");

  xsize  = Image.xsize(original);
  ysize  = Image.ysize(original);

  lxsize = Image.xsize(str_elem);
  lysize = Image.ysize(str_elem);

  Image.sub(src,original,0,0,xsize,ysize);
  Image.make(dest,Bit1,xsize,ysize);

  xsize = Image.xsize(dest);
  ysize = Image.ysize(dest);

  spp = (bit1 **)Image.data(src);
  dpp = (bit1 **)Image.data(dest);
  epp = (bit1 **)Image.data(str_elem);

  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      {
	register long ix,iy,lx,ly,c;

	c = 0;
	for ( iy = 0, ly = y - cy; iy < lysize; ly++, iy++ )
	  {
	    if (c == 1) break;
	    if (ly < 0 || ly >= ysize) continue;

	    for ( ix = 0, lx = x - cx; ix < lxsize; lx++, ix++ )
	      {
		if (c == 1) break;
		if (lx < 0 || lx >= xsize) continue;

		c = epp[iy][ix] & spp[ly][lx];
	      }
	  }
	dpp[y][x] = c;
      }

  Image.destroy(src);
}



static void dilation_uchar(dest,original,str_elem,cx,cy)
     image dest;
     image original;
     image str_elem;
     long cx,cy;
{
  image src;
  register uchar **spp, **dpp, **epp;
  register long x,y;
  long xsize,ysize;
  long lxsize,lysize;

  image__type_check("image__erosion",original,UChar);

  src = Image.create("original-moat");

  xsize  = Image.xsize(original);
  ysize  = Image.ysize(original);

  lxsize = Image.xsize(str_elem);
  lysize = Image.ysize(str_elem);

  Image.sub(src,original,0,0,xsize,ysize);
  Image.make(dest,UChar,xsize,ysize);

  spp = (uchar **)Image.data(src);
  dpp = (uchar **)Image.data(dest);
  epp = (uchar **)Image.data(str_elem);

  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      {
	register long ix,iy,lx,ly,c,cmax;

	cmax = 0;
	for (iy = lysize - 1, ly = y - cy; iy >= 0; ly++, iy--)
	  {
	    if (ly < 0 || ly >= ysize) continue;

	    for (ix = lxsize - 1, lx = x - cx; ix >= 0; lx++, ix--)
	      {
		if (lx < 0 || lx >= xsize) continue;
		if (epp[iy][ix] == 0) continue;

		c = epp[iy][ix] + spp[ly][lx];
		if (c >= 255) {c = cmax = 255;break;}
		if (c > cmax) cmax = c;
	      }
	    if (cmax == 255) break;
	  }
	dpp[y][x] = cmax;
      }

  Image.destroy(src);
}



typedef void (*f_caller) P ((image dest,image src,image elem,long cx,long cy));



static caller_table table[] = {
  {Bit1, dilation_bit1},
  {UChar, dilation_uchar}
};



void image__dilation(dst,src,element,cx,cy)
     image dst;
     image src;
     image element;
     long cx,cy;
{
  f_caller caller;

  caller = (f_caller)image__caller("image__dilation",src,Number(table),table);

  if (caller) caller(dst,src,element,cx,cy);
}
