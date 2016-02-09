/*
 * average.c
 */



#include "Funcs.h"



static void image__average_uchar3
  P2 (( image, dest ),
      ( image, src  ))



typedef void (*f_caller) P ((image dest,image src));



static caller_table table[] = {
  {UChar3, (void *)image__average_uchar3}
};



void image__average(dst,src)
     image dst;
     image src;
{
  f_caller caller;

  caller = (f_caller)image__caller("image__average",src,Number(table),table);

  if (caller) caller(dst,src);
}



static void image__average_uchar3(dest,src)
     register image dest;
     register image src;
{
  register long x, y;

  image__type_check("image__average_uchar3",src,UChar3);

  Image.make(dest,UChar,src->xsize,src->ysize);

  for ( y = 0; y < dest->ysize; y++ )
    for ( x = 0; x < dest->xsize; x++ )
      {
	uchar3 *p;
	p = &__PIXEL(src,x,y,uchar3);
	__PIXEL(dest,x,y,uchar) = (ulong)(p->at[0]+p->at[1]+p->at[2]) / 3;
      }
}
