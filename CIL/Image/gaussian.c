/*
 * gaussian.c
 */



#include <math.h>
#include "Funcs.h"
#include "../misc/memlib.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static double **make_gaussian_filter(sigma,n_ptr)
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

  ss = - 2 * sigma * sigma;
  PIss = 2 * M_PI * sigma * sigma;

  for (i = -n; i <= n; i++)
    {
      int ii = i*i;
      for (j = -n; j <= n; j++)
	{
	  *p++ = exp((ii + j*j)/ss) / PIss;
	}
    }

  *n_ptr = n;

  return filter;
}



static void free_filter(filter)
     double **filter;
{
  typefree2( filter );
}



#define DEFINE_GAUSSIAN_1D(TYPE_NAME,TYPE)\
static void gaussian_##TYPE(dest,src,sigma) \
     image dest; \
     image src; \
     double sigma; \
{ \
  register TYPE **src_data; \
  register TYPE **dest_data; \
  long x,y; \
  double **filter; \
  long xsize,ysize,n; \
 \
  image__type_check("image__gaussian",src,TYPE_NAME); \
 \
  filter = make_gaussian_filter(sigma,&n); \
  xsize = Image.xsize(src); \
  ysize = Image.ysize(src); \
  Image.make(dest,Image.type(src),xsize,ysize); \
 \
  src_data  = (TYPE **)Image.data(src); \
  dest_data = (TYPE **)Image.data(dest); \
 \
  for (y = 0; y < ysize; y++) \
    for (x = 0; x < xsize; x++) \
      { \
	long lx,ly; \
	double v = 0; \
	double sum = 0;\
	for (ly = -n; ly <= n; ly++) \
	  { \
	    long yy = y + ly; \
	    if ((yy < 0) || (yy >= ysize)) continue;\
	    for (lx = -n; lx <= n; lx++) \
	      { \
		long xx = x + lx; \
		if ((xx < 0) || (xx >= xsize)) continue;\
		v += src_data[yy][xx] * filter[ly+n][lx+n]; \
		sum += filter[ly+n][lx+n]; \
	      } \
	  } \
	dest_data[y][x] = v / sum; \
      } \
  free_filter(filter); \
}



#define DEFINE_GAUSSIAN_3D(TYPE_NAME,TYPE)\
static void gaussian_##TYPE(dest,src,sigma) \
     image dest; \
     image src; \
     double sigma; \
{ \
  register TYPE **src_data; \
  register TYPE **dest_data; \
  long x,y; \
  double **filter; \
  long xsize,ysize,n; \
 \
  image__type_check("image__gaussian",src,TYPE_NAME); \
 \
  filter = make_gaussian_filter(sigma,&n); \
  xsize = Image.xsize(src); \
  ysize = Image.ysize(src); \
  Image.make(dest,Image.type(src),xsize,ysize); \
 \
  src_data  = (TYPE **)Image.data(src); \
  dest_data = (TYPE **)Image.data(dest); \
 \
  for (y = 0; y < ysize; y++) \
    for (x = 0; x < xsize; x++) \
      { \
	long lx,ly; \
	double v0,v1,v2; \
 \
	v0 = v1 = v2 = 0; \
	for (ly = -n; ly <= n; ly++) \
	  { \
	    long yy = y + ly; \
	    if (yy < 0) yy = 0; else if (yy >= ysize) yy = ysize -1; \
	    for (lx = -n; lx <= n; lx++) \
	      { \
		long xx = x + lx; \
		if (xx < 0) xx = 0; else if (xx >= xsize) xx = xsize -1; \
		v0 += src_data[yy][xx].at[0] * filter[ly+n][lx+n]; \
		v1 += src_data[yy][xx].at[1] * filter[ly+n][lx+n]; \
		v2 += src_data[yy][xx].at[2] * filter[ly+n][lx+n]; \
	      } \
	  } \
	dest_data[y][x].at[0] = v0; \
	dest_data[y][x].at[1] = v1; \
	dest_data[y][x].at[2] = v2; \
      } \
  free_filter(filter); \
}



DEFINE_GAUSSIAN_1D(UChar,uchar)
DEFINE_GAUSSIAN_1D(Short,short)
DEFINE_GAUSSIAN_1D(Float,float)
DEFINE_GAUSSIAN_1D(Double,double)
DEFINE_GAUSSIAN_3D(UChar3,uchar3)



typedef void (*f_caller) P ((image dest,image src,double sigma));



static caller_table table[] = {
  {UChar, gaussian_uchar},
  {Short, gaussian_short},
  {Float, gaussian_float},
  {Double, gaussian_double},
  {UChar3, gaussian_uchar}
};



void image__gaussian(dst,src,sigma)
     image dst;
     image src;
     double sigma;
{
  f_caller caller;

  caller = (f_caller)image__caller("image__gaussian",src,Number(table),table);

  if (caller) caller(dst,src,sigma);
}
