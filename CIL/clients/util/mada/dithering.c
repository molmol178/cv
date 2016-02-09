#include "Image.h"
#include "Memory.h"
#include "ImageFile.h"



void dithering        P ((image dest,image src,long xsize,long ysize));
void dithering_uchar  P ((image dest,image src,long xsize,long ysize));
void dithering_uchar3 P ((image dest,image src,long xsize,long ysize));
void dithering_long   P ((image dest,image src,long xsize,long ysize));
void dithering_ushort P ((image dest,image src,long xsize,long ysize));



void main(argc,argv)
     int argc;
     char *argv[];
{
  image src;
  image dest;
  char *out = "out.dithering";
  long xsize,ysize;
  extern double atof();

  if (argc < 2)
    {
      fprintf(stderr,"Usage : dithering <infile> [outfile] [xsize] [ysize]\n");
      fprintf(stderr,"Usage : dithering <infile> [outfile] [scale]\n");
      exit(0);
    }

  src  = Image.createFromFilename("src",argv[1]);
  if (src == 0)
    {
      fprintf(stderr,"can't open file (%s)\n",argv[1]);
      exit(-1);
    }
  xsize = Image.xsize(src);
  ysize = Image.ysize(src);

  dest = Image.create("dithering");
  if (argc > 2) out = argv[2];

  fprintf(stderr,"dithering ... {\n");
  fprintf(stderr,"    infile  = \"%s\"\n",argv[1]);
  fprintf(stderr,"    outfile = \"%s\"\n",out);

  if (argc == 5)
    {
      xsize = atol(argv[3]);
      ysize = atol(argv[4]);
      fprintf(stderr,"    resize = (%d,%d)\n",xsize,ysize);
    }
  if (argc == 4)
    {
      double scale = (double)atof(argv[3]);
      if (scale <= 0)
	{
	  fprintf(stderr,"Scale is wrong.(%.5f)\n",scale);
	  exit(-1);
	}
      xsize = xsize * scale;
      ysize = ysize * scale;
      fprintf(stderr,"    scale = %.5f\n",scale);
    }

  ImageFile.setSaveFormat("J4");

  dithering(dest,src,xsize,ysize);

  Image.save(dest,out,"dithering");

  fprintf(stderr,"}\n");

  Image.destroy(dest);
  Image.destroy(src);
}



void dithering(dest,src,xsize,ysize)
     image dest;
     image src;
     long xsize,ysize;
{
  metatype type = Image.type(src);

  fprintf(stderr,"    type = %s\n",Type.name(type));
  if (type == Bit1.type)
    {
      Image.resize(dest,src,xsize,ysize);
      return;
    }
  else
  if (type == UChar.type)
    {
      dithering_uchar(dest,src,xsize,ysize);
      return;
    }
  else
  if (type == UChar3.type)
    {
      dithering_uchar3(dest,src,xsize,ysize);
      return;
    }
  else
  if (type == UShort.type)
    {
      dithering_ushort(dest,src,xsize,ysize);
      return;
    }
  else
  if (type == Long.type)
    {
      dithering_long(dest,src,xsize,ysize);
      return;
    }
}



void dithering_uchar(dest,src,xsize,ysize)
     image dest;
     image src;
     long xsize,ysize;
{
  long oxsize,oysize;
  long x,y,xx,yy,sx,sy;
  long ps;
  long **err,*now,*down;
  uchar **s;
  bit1  **d;

  oxsize = Image.xsize(src);
  oysize = Image.ysize(src);

  Image.make(dest,Bit1.type,xsize,ysize);

  err = (long **)Memory.alloc2D(xsize+2,2,sizeof(long));
  now  = err[1] + 1;
  down = err[0] + 1;

  s = (uchar **)Image.data(src);
  d = (bit1  **)Image.data(dest);

  for (x = 0; x < xsize + 2; x++)
    err[0][x] = err[1][x] = 0;

  for (sy = y = 0; y < ysize; y++)
    {
      yy = sy / ysize;
      {long *tmp = now; now = down; down = tmp;}
      down[-1] = down[0] = 0;
      for (ps = 0,sx = x = 0; x < xsize; x++)
	{
	  xx = sx / xsize;
	  ps += s[yy][xx] + now[x];
	  if (ps >= 255) {d[y][x] = 0;ps -= 255;} else {d[y][x] = 1;}
	  down[x+1] = 2 * ps / 11;
	  down[x-1] += down[x+1];
	  ps = 7 * ps / 22;
	  down[x] += ps;
	  sx += oxsize;
	}
      sy += oysize;
    }
  Memory.free2D(err);
}



void dithering_uchar3(dest,src,xsize,ysize)
     image dest;
     image src;
     long xsize,ysize;
{
  long oxsize,oysize;
  long x,y,xx,yy,sx,sy;
  long ps;
  long **err,*now,*down;
  uchar3 **s;
  bit1  **d;

  oxsize = Image.xsize(src);
  oysize = Image.ysize(src);

  Image.make(dest,Bit1.type,xsize,ysize);

  err = (long **)Memory.alloc2D(xsize+2,2,sizeof(long));
  now  = err[1] + 1;
  down = err[0] + 1;

  s = (uchar3 **)Image.data(src);
  d = (bit1  **)Image.data(dest);

  for (x = 0; x < xsize + 2; x++)
    err[0][x] = err[1][x] = 0;

  for (sy = y = 0; y < ysize; y++)
    {
      yy = sy / ysize;
      {long *tmp = now; now = down; down = tmp;}
      down[-1] = down[0] = 0;
      for (ps = 0,sx = x = 0; x < xsize; x++)
	{
	  uchar3 p;
	  xx = sx / xsize;
	  p = s[yy][xx];
	  ps += (p.at[0] + p.at[1] + p.at[2]) / 3 + now[x];
	  if (ps >= 255) {d[y][x] = 0;ps -= 255;} else {d[y][x] = 1;}
	  down[x+1] = 2 * ps / 11;
	  down[x-1] += down[x+1];
	  ps = 7 * ps / 22;
	  down[x] += ps;
	  sx += oxsize;
	}
      sy += oysize;
    }
  Memory.free2D(err);
}



void dithering_long(dest,src,xsize,ysize)
     image dest;
     image src;
     long xsize,ysize;
{
  register long **s;
  register bit1 **d;
  register int x,y;
  int xx,yy;
  long sx,sy;
  long px,py;
  long p;
  long oxsize,oysize;

  oxsize = Image.xsize(src);
  oysize = Image.ysize(src);

  Image.make(dest,Bit1.type,xsize,ysize);

  s = (long **)Image.data(src);
  d = (bit1 **)Image.data(dest);

  yy = xx = 0;
  for (sy = 0,y = 0; y < ysize; y++)
    {
      py = yy;
      yy = sy / ysize;
      for (sx = 0,x = 0; x < xsize; x++)
	{
	  px = xx;
	  xx = sx / xsize;
	  if (s[yy][px] != s[yy][xx] || s[py][xx] != s[yy][xx] ||
	      s[py][px] != s[yy][xx] ||
	      (x == 0 || y == 0 || x == (xsize-1) || y == (ysize-1)))
	    d[y][x] = 1;
	  else
	    d[y][x] = 0;
	  sx += oxsize;
	}
      sy += oysize;
    }
}



void dithering_ushort(dest,src,xsize,ysize)
     image dest;
     image src;
     long xsize,ysize;
{
  register ushort **s;
  register bit1 **d;
  register int x,y;
  int xx,yy;
  long sx,sy;
  long px,py;
  long p;
  long oxsize,oysize;

  oxsize = Image.xsize(src);
  oysize = Image.ysize(src);

  Image.make(dest,Bit1.type,xsize,ysize);

  s = (ushort **)Image.data(src);
  d = (bit1 **)Image.data(dest);

  yy = xx = 0;
  for (sy = 0,y = 0; y < ysize; y++)
    {
      py = yy;
      yy = sy / ysize;
      for (sx = 0,x = 0; x < xsize; x++)
	{
	  px = xx;
	  xx = sx / xsize;
	  if (s[yy][px] != s[yy][xx] || s[py][xx] != s[yy][xx] ||
	      s[py][px] != s[yy][xx] ||
	      (x == 0 || y == 0 || x == (xsize-1) || y == (ysize-1)))
	    d[y][x] = 1;
	  else
	    d[y][x] = 0;
	  sx += oxsize;
	}
      sy += oysize;
    }
}
