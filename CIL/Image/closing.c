/*
 * closing.c
 */



#include "Funcs.h"



void image__closing(dst,src,element,cx,cy)
     image dst;
     image src;
     image element;
     long cx,cy;
{
  image temp;
  long xsize,ysize,exsize,eysize;

  temp = Image.create(Image.name(src));

  xsize  = Image.xsize(src);
  ysize  = Image.ysize(src);
  exsize = Image.xsize(element);
  eysize = Image.ysize(element);

  Image.sub(temp,src,-exsize,-eysize,xsize+2*exsize,ysize+2*eysize);

  image__dilation(dst,temp,element,cx,cy);
  image__erosion(dst,dst,element,cx,cy);

  Image.sub(dst,dst,exsize,eysize,xsize,ysize);
}

