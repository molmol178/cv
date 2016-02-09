/*
 * XImage 8 Planes
 */



#include "XImage.h"



typedef struct CallerRec {
  long type;
  XCreateImageFunc func;
} Caller;



static Caller table_PseudoColor[] = {
  {PackedBit1,XCreateImageFromData_PackedBit1},
  {Bit1,XCreateImageFromData_Bit1},
  {Char,  XCreateImageFromData_8Planes_PseudoColor_Char},
  {Long,  XCreateImageFromData_8Planes_PseudoColor_Long},
  {UChar, XCreateImageFromData_8Planes_PseudoColor_UChar},
  {UChar3, XCreateImageFromData_8Planes_PseudoColor_UChar3},
};



static Caller table_GrayScale[] = {
  {PackedBit1,XCreateImageFromData_PackedBit1},
  {Bit1,XCreateImageFromData_Bit1},
  {Char,  XCreateImageFromData_8Planes_GrayScale_Char},
  {Long,  XCreateImageFromData_8Planes_GrayScale_Long},
  {UChar, XCreateImageFromData_8Planes_GrayScale_UChar},
  {UChar3, XCreateImageFromData_8Planes_GrayScale_UChar3},
};



static Caller table_StaticGray[] = {
  {PackedBit1,XCreateImageFromData_PackedBit1},
  {Bit1,XCreateImageFromData_Bit1},
  {Char,  XCreateImageFromData_8Planes_StaticGray_Char},
  {Long,  XCreateImageFromData_8Planes_StaticGray_Long},
  {UChar, XCreateImageFromData_8Planes_StaticGray_UChar},
  {UChar3, XCreateImageFromData_8Planes_StaticGray_UChar3},
};



void XImageSetColormap(display,w)
     Display *display;
     Window w;
{
  if (DisplayPlanes(display,DefaultScreen(display)) != 8) return;

  switch (DefaultVisual(display,DefaultScreen(display))->class)
    {
    case PseudoColor:
      XImageSetColormap_8Planes_PseudoColor(display,w);
      break;
    case GrayScale:
      XImageSetColormap_8Planes_GrayScale(display,w);
      break;
    }
}



void XImageSetColormap_8Planes_PseudoColor(display,w)
     Display *display;
     Window w;
{
  XColor color[256];
  Visual *visual;
  uint ri,gi,bi;
  int i,r,g,b,v;
  ulong p;

  static Colormap cmap;
  static long allocated = 0;

  if (allocated)
    {
      XSetWindowColormap(display,w,cmap);
      return;
    }
  allocated = 1;

  visual = DefaultVisual(display,DefaultScreen(display));
  cmap = XCreateColormap(display,w,visual,AllocAll);
  for (i = 0; i < SMAX; i++)
    {
      color[i].pixel = i;
      color[i].flags = DoRed|DoGreen|DoBlue;
    }
  XQueryColors(display,DefaultColormap(display,DefaultScreen(display)),
	       color,SMAX);
  for (i = 0; i < FMAX; i++)
    {
      v = i * 0xffff / (FMAX-1);
      color[FBASE+i].red   = color[i].red;
      color[FBASE+i].green = color[i].green;
      color[FBASE+i].blue  = color[i].blue;
      color[FBASE+i].pixel = i + FBASE;
      color[FBASE+i].flags = DoRed|DoGreen|DoBlue;
    }
  color[255].red = 0;
  color[255].green = 0;
  color[255].blue = 0;
  color[255].pixel = 255;

  color[254].red = 0xffff;
  color[254].green = 0xffff;
  color[254].blue = 0xffff;
  color[254].pixel = 254;

  for (i = 0; i < MMAX; i++)
    {
      v = i * 0xffff / (MMAX-1);
      color[MBASE+i].red = color[MBASE+i].green = color[MBASE+i].blue  = v;
      color[MBASE+i].pixel = i + MBASE;
      color[MBASE+i].flags = DoRed|DoGreen|DoBlue;
    }
  for (ri = 0; ri < RMAX; ri++)
    {
      r = ri * 0xffff / (RMAX-1);
      for (gi = 0; gi < GMAX; gi++)
	{
	  g = gi * 0xffff / (GMAX-1);
	  for (bi = 0; bi < BMAX; bi++)
	    {
	      b = bi * 0xffff / (BMAX-1);
	      p = ri * BMAX * GMAX + gi * BMAX + bi + CBASE;
	      color[p].red   = r;
	      color[p].green = g;
	      color[p].blue  = b;
	      color[p].pixel = p;
	      color[p].flags = DoRed|DoGreen|DoBlue;
	    }
	}
    }
  XStoreColors(display,cmap,color,256);
  XSetWindowColormap(display,w,cmap);
}



void XImageSetColormap_8Planes_GrayScale(display,w)
     Display *display;
     Window w;
{
  XColor color[256];
  Visual *visual;
  uint ri,gi,bi;
  int i,r,g,b,v;
  ulong p;

  static Colormap cmap;
  static long allocated = 0;

  if (allocated)
    {
      XSetWindowColormap(display,w,cmap);
      return;
    }
  allocated = 1;

  visual = DefaultVisual(display,DefaultScreen(display));
  cmap = XCreateColormap(display,w,visual,AllocAll);
  for (i = 0; i < SMAX; i++)
    {
      color[i].pixel = i;
      color[i].flags = DoRed|DoGreen|DoBlue;
    }
  XQueryColors(display,DefaultColormap(display,DefaultScreen(display)),
	       color,SMAX);
  for (i = 0; i < SMAX; i++)
    {
      color[i].red   = color[i].red;
      color[i].green = color[i].green;
      color[i].blue  = color[i].blue;
      color[i].pixel = i;
      color[i].flags = DoRed|DoGreen|DoBlue;
    }
  for (i = 0; i < GrayMax; i++)
    {
      v = i * 0xffff / (GrayMax-1);
      color[GrayBase+i].red   = v;
      color[GrayBase+i].green = v;
      color[GrayBase+i].blue  = v;
      color[GrayBase+i].pixel = i + GrayBase;
      color[GrayBase+i].flags = DoRed|DoGreen|DoBlue;
    }
  color[255].red = 0;
  color[255].green = 0;
  color[255].blue = 0;
  color[255].pixel = 255;
  color[255].flags = DoRed|DoGreen|DoBlue;

  color[254].red = 0xffff;
  color[254].green = 0xffff;
  color[254].blue = 0xffff;
  color[254].pixel = 254;
  color[254].flags = DoRed|DoGreen|DoBlue;

  XStoreColors(display,cmap,color,256);
  XSetWindowColormap(display,w,cmap);
}



XImage *XCreateImageFromData_8Planes
  _P7 (( Display *, display   ),
       ( long     , win_xsize ),
       ( long     , win_ysize ),
       ( long     , type      ),
       ( char **  , data      ),
       ( long     , img_xsize ),
       ( long     , img_ysize ))
{
  XImage *src;

   switch (DefaultVisual(display,DefaultScreen(display))->class)
     {
     case PseudoColor:
       src = XCreateImageFromData_8Planes_PseudoColor
		( display, win_xsize, win_ysize,
		  type, data, img_xsize, img_ysize );
       break;

     case GrayScale:
       src = XCreateImageFromData_8Planes_GrayScale
		( display, win_xsize, win_ysize,
		 type, data, img_xsize, img_ysize );
       break;

     case StaticGray:
       src = XCreateImageFromData_8Planes_StaticGray
		( display, win_xsize, win_ysize,
		  type, data, img_xsize, img_ysize );
       break;
     }

  return src;
}



XImage *XCreateImageFromData_8Planes_PseudoColor
  _P7 (( Display *, display   ),
       ( long     , win_xsize ),
       ( long     , win_ysize ),
       ( long     , type      ),
       ( char **  , data      ),
       ( long     , img_xsize ),
       ( long     , img_ysize ))
{
  XImage *src;
  Caller *caller;

  caller = typeselectconst( type, Caller, table_PseudoColor );
  src = caller->func( display, win_xsize, win_ysize,
		      data, img_xsize, img_ysize );

  return src;
}



XImage *XCreateImageFromData_8Planes_GrayScale
  _P7 (( Display *, display   ),
       ( long     , win_xsize ),
       ( long     , win_ysize ),
       ( long     , type      ),
       ( char **  , data      ),
       ( long     , img_xsize ),
       ( long     , img_ysize ))
{
  XImage *src;
  Caller *caller;

  caller = typeselectconst( type, Caller, table_GrayScale );
  src = caller->func( display, win_xsize, win_ysize,
		      data, img_xsize, img_ysize );

  return src;
}



XImage *XCreateImageFromData_8Planes_StaticGray
  _P7 (( Display *, display   ),
       ( long     , win_xsize ),
       ( long     , win_ysize ),
       ( long     , type      ),
       ( char **  , data      ),
       ( long     , img_xsize ),
       ( long     , img_ysize ))
{
  XImage *src;
  Caller *caller;

  caller = typeselectconst( type, Caller, table_StaticGray );
  src = caller->func( display, win_xsize, win_ysize,
		      data, img_xsize, img_ysize );

  return src;
}
