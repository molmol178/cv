/*
 * opening.c
 */



#include "Funcs.h"



void image__opening(dst,src,element,cx,cy)
     image dst;
     image src;
     image element;
     long cx,cy;
{
  image temp;

  image__erosion(dst,src,element,cx,cy);
  image__dilation(dst,dst,element,cx,cy);
}
