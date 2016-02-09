/*
 * erosion.c
 */



#include "Funcs.h"


#include <stdio.h>
static void erosion_bit1(dest,original,str_elem,cx,cy)
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

  image__type_check("image__erosion",original,Bit1);

  src = Image.create("original-moat");

  xsize  = Image.xsize(original);
  ysize  = Image.ysize(original);

  lxsize = Image.xsize(str_elem);
  lysize = Image.ysize(str_elem);

  Image.sub(src,original,0,0,xsize,ysize);
  Image.make(dest,Bit1,xsize,ysize);

  spp = (bit1 **)Image.data(src);
  dpp = (bit1 **)Image.data(dest);
  epp = (bit1 **)Image.data(str_elem);

  cy = lysize - cy - 1;
  cx = lxsize - cx - 1;

  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      {
	register long ix,iy,lx,ly,c,sbit;

	c = 1;
	for ( iy = lysize - 1, ly = y - cy; iy >= 0; ly++, iy-- )
	  {
	    if ( c == 0 ) break;
	    for ( ix = lxsize - 1, lx = x - cx; ix >= 0; lx++, ix-- )
	      {
		if ( c == 0 ) break;
		sbit = (( ly < 0 || ly >= ysize ) ||
			( lx < 0 || lx >= xsize )) ? 0 : spp[ly][lx];
		c = (epp[iy][ix] == 0) || ( epp[iy][ix] & sbit );
	      }
	  }
	dpp[y][x] = c;
      }

  Image.destroy(src);
}



static void erosion_uchar(dest,original,str_elem,cx,cy)
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
	register long ix,iy,lx,ly,c,cmin;

	cmin = 255;
	for (iy = 0, ly = y - cy; iy < lysize; ly++, iy++)
	  {
	    if (ly < 0 || ly >= ysize) continue;

	    for (ix = 0, lx = x - cx; ix < lxsize; lx++, ix++)
	      {
		if (lx < 0 || lx >= xsize) continue;
		if (epp[iy][ix] == 0) continue;

		c = spp[ly][lx] - epp[iy][ix];
		if (c < 0) c = 0;
		if (c < cmin) cmin = c;
		if (cmin == 0) break;
	      }
	    if (cmin == 0) break;
	  }
	dpp[y][x] = cmin;
      }

  Image.destroy(src);
}



typedef void (*f_caller) P ((image dest,image src,image elem,long cx,long cy));



static caller_table table[] = {
  {Bit1, erosion_bit1},
  {UChar, erosion_uchar}
};



void image__erosion(dst,src,element,cx,cy)
     image dst;
     image src;
     image element;
     long cx,cy;
{
  f_caller caller;

  caller = (f_caller)image__caller("image__erosion",src,Number(table),table);

  if (caller) caller(dst,src,element,cx,cy);
}
