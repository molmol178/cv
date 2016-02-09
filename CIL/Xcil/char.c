/*
 * XImage for char
 */



#include "XImage.h"
#include "fontlib.h"


static char *XImageFontName = "g5";
static char *XImageFontData = 0;
static long  XImageFontXSize = 5;
static long  XImageFontYSize = 5;
static long  XImageFontXSpace = 1;
static long  XImageFontYSpace = 1;



long XImageFontGetXSize()
{
  return XImageFontXSize;
}



long XImageFontGetYSize()
{
  return XImageFontYSize;
}



long XImageFontGetXSpace()
{
  return XImageFontXSpace;
}



long XImageFontGetYSpace()
{
  return XImageFontYSpace;
}



char *XImageFontSet(name)
     char *name;
{
  struct fontRec *font;
  char *retname;

  retname = XImageFontName;

  font = fontsearch(name);
  if (font == 0)
    {
      fprintf(stderr, "waning: cannot find font '%s'\n", name);
      return 0;
    }

  XImageFontXSize = font->xsize;
  XImageFontYSize = font->ysize;
  XImageFontData  = font->data;
  XImageFontName  = font->name;

  return retname;
}



void  XImageFontSetSpace(xspace,yspace)
     long xspace,yspace;
{
  XImageFontXSpace = xspace;
  XImageFontYSpace = yspace;
}



packedbit1 **XImageCharToPackedBit1(src,iwidth,iheight,xsizeptr,ysizeptr)
     char **src;
     long iwidth;
     long iheight;
     long *xsizeptr;
     long *ysizeptr;
{
  packedbit1 **data;
  long x,y,xsize,ysize;
  long width,height,fwidth,fheight;
  static long bitmask[8] = {1<<7,1<<6,1<<5,1<<4,1<<3,1<<2,1<<1,1<<0};

  if (XImageFontData == 0) XImageFontSet("g5");

  width   = XImageFontXSize + XImageFontXSpace;
  height  = XImageFontYSize + XImageFontYSpace;
  fwidth  = XImageFontYSize + 1;
  fheight = XImageFontYSize + 1;

  *xsizeptr = xsize = width  * iwidth  + XImageFontXSpace;
  *ysizeptr = ysize = height * iheight + XImageFontYSpace;

  data = (packedbit1 **)memnew2( ( xsize + 7 ) / 8, ysize, 1);

  for (y = 0; y < iheight; y++)
    for (x = 0; x < iwidth; x++)
      {
	long do_reverse = 0;
	long c = src[y][x];
	long cx,cy,lx,ly;

	if (c < 0) {do_reverse = 1;c = -c;}
	c = (c + 128) % 128;
	cx = c % 16;
	cy = c / 16;
	for (ly = 0; ly < XImageFontYSize; ly++)
	  {
	    long yoffset = (cy * fheight + ly) * 16 * fwidth;
	    long yy = y * height + ly + XImageFontYSpace;
	    if (yy < 0) continue;
	    if (yy >= ysize) break;

	    for (lx = 0; lx < XImageFontXSize; lx++)
	      {
		long xoffset = cx * fwidth + lx;
		long xx = x * width + lx + XImageFontXSpace;
		long pix;
		if (xx < 0) continue;
		if (xx >= xsize) break;

		pix  = XImageFontData[xoffset + yoffset];
		if (do_reverse)
		  {
		    if (pix == '0')
		      data[yy][xx / 8] |= bitmask[xx % 8];
		  }
		else
		  {
		    if (pix == '1')
		      data[yy][xx / 8] |= bitmask[xx % 8];
		  }
	      }
	  }
      }

  return data;
}



XImage *XCreateImageFromData_1Plane_Char(display,wwidth,wheight,org,iwidth,iheight)
     Display *display;
     long wwidth;
     long wheight;
     register char **org;
     long iwidth;
     long iheight;
{
  XImage *src;
  packedbit1 **data;
  long xsize,ysize;

  data = XImageCharToPackedBit1(org,iwidth,iheight,&xsize,&ysize);
  src = XCreateImageFromData_PackedBit1(display,wwidth,wheight,data,
					xsize,ysize);
  typefree2(data);

  return src;
}



XImage *XCreateImageFromData_8Planes_PseudoColor_Char(display,wwidth,wheight,org,iwidth,iheight)
     Display *display;
     long wwidth;
     long wheight;
     register char **org;
     long iwidth;
     long iheight;
{
  XImage *src;
  packedbit1 **data;
  long xsize,ysize;

  data = XImageCharToPackedBit1(org,iwidth,iheight,&xsize,&ysize);
  src = XCreateImageFromData_PackedBit1(display,wwidth,wheight,data,xsize,ysize);
  typefree2(data);

  return src;
}



XImage *XCreateImageFromData_8Planes_GrayScale_Char(display,wwidth,wheight,org,iwidth,iheight)
     Display *display;
     long wwidth;
     long wheight;
     register char **org;
     long iwidth;
     long iheight;
{
  XImage *src;
  packedbit1 **data;
  long xsize,ysize;

  data = XImageCharToPackedBit1(org,iwidth,iheight,&xsize,&ysize);
  src = XCreateImageFromData_PackedBit1(display,wwidth,wheight,data,xsize,ysize);
  typefree2(data);

  return src;
}



XImage *XCreateImageFromData_8Planes_StaticGray_Char(display,wwidth,wheight,org,iwidth,iheight)
     Display *display;
     long wwidth;
     long wheight;
     register char **org;
     long iwidth;
     long iheight;
{
  XImage *src;
  packedbit1 **data;
  long xsize,ysize;

  data = XImageCharToPackedBit1(org,iwidth,iheight,&xsize,&ysize);
  src = XCreateImageFromData_PackedBit1(display,wwidth,wheight,data,xsize,ysize);
  typefree2(data);

  return src;
}



XImage *XCreateImageFromData_24Planes_Char(display,wwidth,wheight,org,iwidth,iheight)
     Display *display;
     long wwidth;
     long wheight;
     register char **org;
     long iwidth;
     long iheight;
{
  XImage *src;
  packedbit1 **data;
  long xsize,ysize;

  data = XImageCharToPackedBit1(org,iwidth,iheight,&xsize,&ysize);
  src = XCreateImageFromData_PackedBit1(display,wwidth,wheight,data,xsize,ysize);
  typefree2(data);

  return src;
}



XImage *XCreateImageFromData_16Planes_Char(display,wwidth,wheight,org,iwidth,iheight)
     Display *display;
     long wwidth;
     long wheight;
     register char **org;
     long iwidth;
     long iheight;
{
  XImage *src;
  packedbit1 **data;
  long xsize,ysize;

  data = XImageCharToPackedBit1(org,iwidth,iheight,&xsize,&ysize);
  src = XCreateImageFromData_PackedBit1(display,wwidth,wheight,data,xsize,ysize);
  typefree2(data);

  return src;
}



XImage *XCreateImageFromData_15Planes_Char(display,wwidth,wheight,org,iwidth,iheight)
     Display *display;
     long wwidth;
     long wheight;
     register char **org;
     long iwidth;
     long iheight;
{
  XImage *src;
  packedbit1 **data;
  long xsize,ysize;

  data = XImageCharToPackedBit1(org,iwidth,iheight,&xsize,&ysize);
  src = XCreateImageFromData_PackedBit1(display,wwidth,wheight,data,xsize,ysize);
  typefree2(data);

  return src;
}
