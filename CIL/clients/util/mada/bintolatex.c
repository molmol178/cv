/*
 * binary image to latex.
 */



#include "Image.h"
#include <stdio.h>



long image__toLatex P ((image src,long xmm,long ymm));



void main(argc,argv)
     int argc;
     char *argv[];
{
  image src;
  long xmm = 0,ymm = 0,num;

  if (argc < 2)
    {
      fprintf(stderr,"Usage : bintolatex <input-binary-image> [xsize(mm)] [ysize(mm)]\n");
      exit(-1);
    }

  if (argc > 2) xmm = atol(argv[2]);
  if (argc > 3) ymm = atol(argv[3]);

  src = Image.createFromFilename("src",argv[1]);
  if ((src == 0) || (Image.type(src) != Bit1))
    {
      fprintf(stderr,"Image is wrong.\n");
      exit(-1);
    }

  if (xmm != 0 && ymm == 0) ymm = Image.ysize(src) * xmm / Image.xsize(src);
  if (xmm == 0 && ymm != 0) xmm = Image.xsize(src) * ymm / Image.ysize(src);
  if (xmm == 0) xmm = Image.xsize(src);
  if (ymm == 0) ymm = Image.xsize(src);
  num = image__toLatex(src,xmm,ymm);

  fprintf(stderr,"\\put -> %d \n",num);

  Image.destroy(src);
}



long image__toLatex(src,xmm,ymm)
     image src;
     long xmm,ymm;
{
  long xsize,ysize;
  register long x,y;
  register bit1 **data;
  double xunit,yunit,xtoy;
  long num;

  data  = Image.data(src);
  xsize = Image.xsize(src);
  ysize = Image.ysize(src);

  xunit = (double)xmm / (double)xsize;
  yunit = (double)ymm / (double)ysize;
  xtoy  = (double)yunit / (double)xunit;

  printf("\\setlength{\\unitlength}{%.3fmm}\n",xunit);
  printf("\\begin{picture}(%d,%.3f)\n",xsize,(double)ysize * xtoy);
  printf("\\put(0,0){\\framebox(%d,%.3f){}}\n",xsize,(double)ysize*xtoy);

  num = 0;

  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      {
	long length,start;
	while ((x < xsize) && (data[y][x] == 0)) x++;
	if (x >= xsize) continue;

	start = x;
	length = 0;
	while ((x < xsize) && (data[y][x] == 1)) {x++;length++;}
	if (length) 
	  {
	    num++;
	    printf("\\put(%3d,%.3f){\\rule{%.3fmm}{%.3fmm}}\n",
		   start,(double)(ysize-y-1)*xtoy,
		   xunit*(double)length,yunit*1.1);
	  }
      }

  printf("\\end{picture}\n");

  return num;
}
