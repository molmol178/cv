/*
 * filename : Smith.c
 * author   : Yoshihiro Kameyama
 * date     : 1994/01/18
 * redate   :
 *
 * co-author : Takahiro Sugiyama
 * update    : 1994/01/19 マクロ化
 */



#include "ColorImage.h"



#define min2(a,b) ((a) < (b) ? (a) : (b))
#define min3(a,b,c) ( (a) < (b) ? min2( (a), (c) ) : min2( (b), (c) ) )
#define max2(a,b) ((a) > (b) ? (a) : (b))
#define max3(a,b,c) ( (a) > (b) ? max2( (a), (c) ) : max2( (b), (c) ) )



/*=======================*
 * Smithの双六角形モデル *
 *=======================*/
#define DEFINE_RGB_TO( FUNCTION_NAME, RGB_TYPE )		\
void FUNCTION_NAME						\
  _P2 (( RGB_TYPE *, rgb ),					\
       ( float3 *  , hsv ))					\
{								\
  double r, g, b;						\
  double h, s, v;						\
  double max, min, max_min;					\
  double rc, gc, bc;						\
								\
  r = (double)rgb->at[ 0 ] / 255.0;				\
  g = (double)rgb->at[ 1 ] / 255.0;				\
  b = (double)rgb->at[ 2 ] / 255.0;				\
								\
  max = max3( r, g, b );					\
  min = min3( r, g, b );					\
  max_min = max - min;						\
								\
  v = max;							\
								\
  if ( v == 0 )							\
    s = 0;							\
  else								\
    s = max_min / max;						\
								\
  if ( s == 0 )							\
    h = 0;							\
  else								\
    {								\
      rc = ( max - r ) / max_min;				\
      gc = ( max - g ) / max_min;				\
      bc = ( max - b ) / max_min;				\
								\
      /* resulting color between YELLOW  & MAGENTA */		\
      if ( r == max ) h = bc - gc; else				\
      /* resulting color between CYAN    & YELLOW  */		\
      if ( g == max ) h = 2 + rc - bc; else			\
      /* resulting color between MAGENTA & CYAN    */		\
      if ( b == max ) h = 4 + gc - rc;				\
  }								\
  h = h * 60.0; /* h [0,360] (degree), exchange scaling */	\
  if ( h < 0.0 )    h = h + 360.0;				\
  if ( h >= 360.0 ) h = h - 360.0;				\
								\
  hsv->at[ 0 ] = h;						\
  hsv->at[ 1 ] = s * 255.0;					\
  hsv->at[ 2 ] = v * 255.0;					\
}



DEFINE_RGB_TO(UCharRGB_To_HSV_Smith,uchar3)
DEFINE_RGB_TO(FloatRGB_To_HSV_Smith,float3)
