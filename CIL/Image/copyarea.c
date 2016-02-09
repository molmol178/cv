/*
 * copyarea.c
 */



#include "Funcs.h"



#define DEFINE_COPYAREA(TYPE_NAME,TYPE) \
static void copyarea_##TYPE(dest,src,dest_x,dest_y,src_x,src_y,xsize,ysize) \
     register image dest;\
     register image src;\
     long dest_x;\
     long dest_y;\
     long src_x;\
     long src_y;\
     long xsize;\
     long ysize;\
{\
  register long x,y;\
  register long sx,sy,dx,dy;\
\
  image__type_check("image__copyarea",src,TYPE_NAME);\
\
  for (y = 0; y < ysize; y++)\
    {\
      sy = y + src_y;\
      dy = y + dest_y;\
      if (sy < 0 || sy >= src->ysize || dy < 0 || dy >= dest->ysize) continue;\
      for (x = 0; x < xsize; x++)\
	{\
	  sx = x + src_x;\
	  dx = x + dest_x;\
	  if (sx < 0 || sx >= src->xsize || dx < 0 || dx >= dest->xsize)\
	    continue;\
	  __PIXEL(dest,dx,dy,TYPE) = __PIXEL(src,sx,sy,TYPE);\
	}\
    }\
}



DEFINE_COPYAREA(Bit1,bit1)
DEFINE_COPYAREA(UChar,uchar)
DEFINE_COPYAREA(Long,long)
DEFINE_COPYAREA(UChar3,uchar3)
DEFINE_COPYAREA(Short2,short2)



typedef void (*f_caller) P ((image dest,image src,long dest_x,long dest_y,
			     long src_x,long src_y,long width,long height));



static caller_table table[] = {
  {Bit1,  copyarea_bit1},
  {Long,  copyarea_long},
  {UChar, copyarea_uchar},
  {UChar3,copyarea_uchar3},
  {Short2,copyarea_short2}
};



void image__copyarea(dst,src,dest_x,dest_y,src_x,src_y,width,height)
     image dst;
     image src;
     long dest_x;
     long dest_y;
     long src_x;
     long src_y;
     long width;
     long height;
{
  f_caller caller;

  caller = (f_caller)image__caller("image__copyarea",src,Number(table),table);

  if (caller) caller(dst,src,dest_x,dest_y,src_x,src_y,width,height);
}
