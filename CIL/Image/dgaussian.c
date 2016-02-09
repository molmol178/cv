/*
 * dgaussian.c
 */



#include <math.h>
#include "Funcs.h"
#include "../misc/memlib.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static double **make_dgaussian_filter(sigma,n_ptr)
     double sigma;
     long *n_ptr;
{
  int j,i,n;
  double **filter;
  double ss,PIss;
  register double *p;

  n = sigma * 4.2;
  filter = typenew2( 2 * n + 1, 2 * n + 1, double );

  p = (double *)&(filter[0][0]);

  ss = 2.0 * sigma * sigma;
  PIss = 2.0 * M_PI * sigma * sigma * sigma * sigma;

  for (i = -n; i <= n; i++)
    for (j = -n; j <= n; j++)
      *p++ = (- j / PIss) * exp(- (i*i+j*j) / ss);

  *n_ptr = n;

  return filter;
}



static void free_filter(filter)
     double **filter;
{
  typefree2( filter );
}



#define DEFINE_dGAUSSIAN_1D(TYPE_NAME,TYPE)\
static void dgaussian_##TYPE(mag,grad_x,grad_y,src,sigma)\
     register image mag; \
     register image grad_x,grad_y; \
     register image src; \
     double sigma; \
{ \
  register long x,y,xsize,ysize; \
    long n;\
  register double **filter; \
  register float **grad_x_data;\
  register float **grad_y_data;\
  register float **mag_data;\
  register TYPE  **src_data;\
 \
  image__type_check("image__gaussian_gradient",src,TYPE_NAME); \
 \
  xsize = __XSIZE( src );\
  ysize = __YSIZE( src );\
  filter = make_dgaussian_filter(sigma,&n); \
  Image.make(mag,Float,xsize,ysize); \
  Image.make(grad_x,Float,xsize,ysize); \
  Image.make(grad_y,Float,xsize,ysize); \
 \
  src_data    = __DATA( src, TYPE );\
  grad_x_data = __DATA( grad_x, float );\
  grad_y_data = __DATA( grad_y, float );\
  mag_data    = __DATA( mag   , float );\
 \
  for (y = 0; y < ysize; y++) \
    for (x = 0; x < xsize; x++) \
      { \
	register long lx,ly; \
	double vx = 0,vy = 0; \
	for (ly = -n; ly <= n; ly++) \
	  { \
	    register long yy = y + ly; \
	    if (yy < 0) yy = 0; \
	    if (yy >= ysize) yy = ysize -1; \
	    for (lx = -n; lx <= n; lx++) \
	      { \
		register long xx = x + lx; \
		if (xx < 0) xx = 0; \
		if (xx >= xsize) xx = xsize -1; \
		vx += src_data[yy][xx] * filter[ly+n][lx+n]; \
		vy += src_data[yy][xx] * filter[lx+n][ly+n]; \
	      } \
	  } \
	mag_data[ y ][ x ] = sqrt(vx * vx + vy * vy); \
	grad_x_data[ y ][ x ] = vx;\
	grad_y_data[ y ][ x ] = vy;\
      } \
  free_filter(filter); \
}



DEFINE_dGAUSSIAN_1D(UChar,uchar)
DEFINE_dGAUSSIAN_1D(Short,short)
DEFINE_dGAUSSIAN_1D(Float,float)
DEFINE_dGAUSSIAN_1D(Double,double)



typedef void (*f_caller) P ((image mag,image grad_x,image grad_y,
			     image src,double sigma));



static caller_table table[] = {
  {UChar,  dgaussian_uchar},
  {Short,  dgaussian_short},
  {Float,  dgaussian_float},
  {Double, dgaussian_double}
};



void image__gaussian_gradient(mag,grad_x,grad_y,src,sigma)
     image mag;
     image grad_x;
     image grad_y;
     image src;
     double sigma;
{
  f_caller caller;

  caller = (f_caller)image__caller("image__gaussian_gradient",
				   src,Number(table),table);

  if (caller) caller(mag,grad_x,grad_y,src,sigma);
}
