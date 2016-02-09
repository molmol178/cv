/*
 * imagefunc.c
 */



#include "Edit.h"



void image__rotation_right_90(image dest,image orig)
{
  long xsize,ysize;
  long x,y;
  long type;
  image src;

  src = Image.create("orig");
  Image.copy(src,orig);

  xsize = Image.ysize(orig);
  ysize = Image.xsize(orig);
  type  = Image.type(orig);
  Image.make(dest,type,xsize,ysize);

  if (type == Bit1)
    {
      bit1 **dp = (bit1 **)Image.data(dest);
      bit1 **sp = (bit1 **)Image.data(src);

      for (y = 0; y < ysize; y++)
	for (x = 0; x < xsize; x++)
	  dp[y][x] = sp[xsize-1 - x][y];
    }
  else
  if (type == UChar)
    {
      uchar **dp = (uchar **)Image.data(dest);
      uchar **sp = (uchar **)Image.data(src);

      for (y = 0; y < ysize; y++)
	for (x = 0; x < xsize; x++)
	  dp[y][x] = sp[xsize - x - 1][y];
    }
  else
  if (type == UChar3)
    {
      uchar3 **dp = (uchar3 **)Image.data(dest);
      uchar3 **sp = (uchar3 **)Image.data(src);

      for (y = 0; y < ysize; y++)
	for (x = 0; x < xsize; x++)
	  dp[y][x] = sp[xsize - x - 1][y];
    }
  Image.destroy(src);
}




void image__rotation_left_90(image dest,image orig)
{
  long xsize,ysize;
  long x,y;
  long type;
  image src;

  src = Image.create("temp");
  Image.copy(src,orig);

  xsize = Image.ysize(orig);
  ysize = Image.xsize(orig);
  type  = Image.type(orig);
  Image.make(dest,type,xsize,ysize);

  if (type == Bit1)
    {
      bit1 **dp = (bit1 **)Image.data(dest);
      bit1 **sp = (bit1 **)Image.data(src);

      for (y = 0; y < ysize; y++)
	for (x = 0; x < xsize; x++)
	  dp[y][x] = sp[x][ysize - y - 1];
    }
  else
  if (type == UChar)
    {
      uchar **dp = (uchar **)Image.data(dest);
      uchar **sp = (uchar **)Image.data(src);

      for (y = 0; y < ysize; y++)
	for (x = 0; x < xsize; x++)
	  dp[y][x] = sp[x][ysize - y - 1];
    }
  else
  if (type == UChar3)
    {
      uchar3 **dp = (uchar3 **)Image.data(dest);
      uchar3 **sp = (uchar3 **)Image.data(src);

      for (y = 0; y < ysize; y++)
	for (x = 0; x < xsize; x++)
	  dp[y][x] = sp[x][ysize - y - 1];
    }
  Image.destroy(src);
}



void image__x_reverse(image dest,image orig)
{
  long xsize,ysize;
  long x,y;
  long type;
  image src;

  src = Image.create("temp");
  Image.copy(src,orig);

  xsize = Image.xsize(orig);
  ysize = Image.ysize(orig);
  type  = Image.type(orig);
  Image.make(dest,type,xsize,ysize);

  if (type == Bit1)
    {
      bit1 **dp = (bit1 **)Image.data(dest);
      bit1 **sp = (bit1 **)Image.data(src);

      for (y = 0; y < ysize; y++)
	for (x = 0; x < xsize; x++)
	  dp[y][x] = sp[y][xsize - x - 1];
    }
  else
  if (type == UChar)
    {
      uchar **dp = (uchar **)Image.data(dest);
      uchar **sp = (uchar **)Image.data(src);

      for (y = 0; y < ysize; y++)
	for (x = 0; x < xsize; x++)
	  dp[y][x] = sp[y][xsize - x - 1];
    }
  else
  if (type == UChar3)
    {
      uchar3 **dp = (uchar3 **)Image.data(dest);
      uchar3 **sp = (uchar3 **)Image.data(src);

      for (y = 0; y < ysize; y++)
	for (x = 0; x < xsize; x++)
	  dp[y][x] = sp[y][xsize - x - 1];
    }
  Image.destroy(src);
}



void image__y_reverse(image dest,image orig)
{
  long xsize,ysize;
  long x,y;
  long type;
  image src;

  src = Image.create("temp");
  Image.copy(src,orig);

  xsize = Image.xsize(orig);
  ysize = Image.ysize(orig);
  type  = Image.type(orig);
  Image.make(dest,type,xsize,ysize);

  if (type == Bit1)
    {
      bit1 **dp = (bit1 **)Image.data(dest);
      bit1 **sp = (bit1 **)Image.data(src);

      for (y = 0; y < ysize; y++)
	for (x = 0; x < xsize; x++)
	  dp[y][x] = sp[ysize - y - 1][x];
    }
  else
  if (type == UChar)
    {
      uchar **dp = (uchar **)Image.data(dest);
      uchar **sp = (uchar **)Image.data(src);

      for (y = 0; y < ysize; y++)
	for (x = 0; x < xsize; x++)
	  dp[y][x] = sp[ysize - y - 1][x];
    }
  else
  if (type == UChar3)
    {
      uchar3 **dp = (uchar3 **)Image.data(dest);
      uchar3 **sp = (uchar3 **)Image.data(src);

      for (y = 0; y < ysize; y++)
	for (x = 0; x < xsize; x++)
	  dp[y][x] = sp[ysize - y - 1][x];
    }
  Image.destroy(src);
}



void image__xy_reverse(image dest,image orig)
{
  long xsize,ysize;
  long x,y;
  long type;
  image src;

  src = Image.create("temp");
  Image.copy(src,orig);

  xsize = Image.xsize(orig);
  ysize = Image.ysize(orig);
  type  = Image.type(orig);
  Image.make(dest,type,xsize,ysize);

  if (type == Bit1)
    {
      bit1 **dp = (bit1 **)Image.data(dest);
      bit1 **sp = (bit1 **)Image.data(src);

      for (y = 0; y < ysize; y++)
	for (x = 0; x < xsize; x++)
	  dp[y][x] = sp[ysize - y - 1][xsize - x - 1];
    }
  else
  if (type == UChar)
    {
      uchar **dp = (uchar **)Image.data(dest);
      uchar **sp = (uchar **)Image.data(src);

      for (y = 0; y < ysize; y++)
	for (x = 0; x < xsize; x++)
	  dp[y][x] = sp[ysize - y - 1][xsize - x - 1];
    }
  else
  if (type == UChar3)
    {
      uchar3 **dp = (uchar3 **)Image.data(dest);
      uchar3 **sp = (uchar3 **)Image.data(src);

      for (y = 0; y < ysize; y++)
	for (x = 0; x < xsize; x++)
	  dp[y][x] = sp[ysize - y - 1][xsize - x - 1];
    }
  Image.destroy(src);
}



void image__rotation_any(image dest,image orig,long angle)
{
  double costh,sinth;
  int xsize,ysize;
  long type;
  image src;

  src = Image.create("temp");
  Image.copy(src,orig);

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  type  = Image.type(src);
  Image.make(dest,type,xsize,ysize);

  sinth = sin(-angle*3.1415926535897932384/180.0);
  costh = cos(-angle*3.1415926535897932384/180.0);

  {
    uchar **sp,**dp;
    int x,y;
    int sx,sy;
    double cx,cy;
    cx = xsize / 2.0;
    cy = ysize / 2.0;

    dp = (uchar **)Image.data(dest);
    sp = (uchar **)Image.data(src);
    for (y = 0; y < ysize; y++)
      {
	double ycos = costh * (y - cy);
	double ysin = sinth * (y - cy);
	for (x = 0; x < xsize; x++)
	  {
	    double xcos = costh * (x - cx);
	    double xsin = sinth * (x - cx);

	    sx = xcos - ysin + cx + 0.5;
	    sy = xsin + ycos + cy + 0.5;

	    if ((sx >= 0)&&(sy >= 0)&&(sx < xsize)&&(sy < ysize))
	      {
		if (type == UChar3)
		  ((uchar3 **)dp)[y][x] = ((uchar3 **)sp)[sy][sx];
		else
		  dp[y][x] = sp[sy][sx];
	      }
	    else
	      {
		if (type == UChar3)
		  {
		    ((uchar3 **)dp)[y][x].at[0] = 255;
		    ((uchar3 **)dp)[y][x].at[1] = 255;
		    ((uchar3 **)dp)[y][x].at[2] = 255;
		  }
		else
		if (type == UChar)
		  dp[y][x] = 255;
		else
		  dp[y][x] = 0;
	      }
	  }
      }
  }
}



void image__shift(image src,image dest,long dir,long step)
{
  long x,y;

  x = 0;
  y = 0;

  switch (dir)
    {
    case SHIFT_UP:   y += step;break;
    case SHIFT_DOWN: y -= step;break;
    case SHIFT_LEFT: x += step;break;
    case SHIFT_RIGHT:x -= step;break;
    }

  Image.sub(dest,src,x,y,Image.xsize(src),Image.ysize(src));
}



void image__region(image dest,image src,long dxsize,long dysize,long gravity)
{
  long x[9],y[9],xsize,ysize;

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);

  x[GRAVITY_MU] = x[GRAVITY_MD] = x[GRAVITY_MM] = -(dxsize - xsize) / 2;
  x[GRAVITY_LU] = x[GRAVITY_LD] = x[GRAVITY_LM] = 0;
  x[GRAVITY_RU] = x[GRAVITY_RD] = x[GRAVITY_RM] = -(dxsize - xsize);

  y[GRAVITY_RM] = y[GRAVITY_LM] = y[GRAVITY_MM] = -(dysize - ysize) / 2;
  y[GRAVITY_RU] = y[GRAVITY_LU] = y[GRAVITY_MU] = 0;
  y[GRAVITY_RD] = y[GRAVITY_LD] = y[GRAVITY_MD] = -(dysize - ysize);

  Image.sub(dest,src,x[gravity],y[gravity],dxsize,dysize);
}



void image__resize(image dest,image src,long xsize,long ysize)
{
  Image.resize(dest,src,xsize,ysize);
}



void image__clear(image src)
{
  Image.clear(src);
  if ((Image.type(src) == UChar) || (Image.type(src) == UChar3))
    image__reverse(src,src);
}



void image__fill(image dest,char *value)
{
  long type = Image.type(dest);
  long i,n;

  n = Image.area(dest);
  if (type == Bit1)
    {
      bit1 c = *((bit1 *)value);
      bit1 *p = (bit1 *)Image.raster(dest);
      for (i = 0; i < n; i++) *p++ = c;
    }
  else
  if (type == UChar)
    {
      uchar c = *((uchar *)value);
      uchar *p = (uchar *)Image.raster(dest);
      for (i = 0; i < n; i++) *p++ = c;
    }
  else
  if (type == UChar3)
    {
      uchar3 c = *((uchar3 *)value);
      uchar3 *p = (uchar3 *)Image.raster(dest);
      for (i = 0; i < n; i++) *p++ = c;
    }  
}
