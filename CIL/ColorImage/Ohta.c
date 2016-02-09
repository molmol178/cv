/*
 * filename : Ohta.c
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

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


/*================*
 * Ohtaらのモデル *
 *================*/
#define DEFINE_RGB_TO( FUNCTION_NAME, RGB_TYPE )		\
void FUNCTION_NAME						\
  _P2 (( RGB_TYPE *, rgb ),					\
       ( float3 *  , hsv ))					\
{								\
  double r, g, b;						\
  double h, s, v;						\
  double sum, min;						\
								\
  r = (double)rgb->at[ 0 ] / 255.0;				\
  g = (double)rgb->at[ 1 ] / 255.0;				\
  b = (double)rgb->at[ 2 ] / 255.0;				\
								\
  sum = r + g + b;						\
  min = min3( r, g, b );					\
								\
  v = sum / 3.0;						\
  if ( v == 0.0 )						\
    {/* schromatic case */					\
      h = 0.0;							\
      s = 0.0;							\
    }								\
  else								\
    {								\
      s = ( sum - min * 3.0 ) / sum; /* saturation */		\
								\
      if ( s == 0.0 )						\
	h = 0.0;						\
      else							\
	h = atan2( sqrt( 3.0 ) * ( g - b ), 2.0 * r - g - b );	\
    }								\
								\
  /* exchange scaling */					\
  h = h * 360.0 / ( 2.0 * M_PI );				\
  if ( h <   0.0 ) h = h + 360.0;				\
  if ( h > 360.0 ) h = h - 360.0;				\
								\
  hsv->at[ 0 ] = h;						\
  hsv->at[ 1 ] = s * 255.0;					\
  hsv->at[ 2 ] = v * 255.0;					\
}



DEFINE_RGB_TO(UCharRGB_To_HSV_Ohta,uchar3)
DEFINE_RGB_TO(FloatRGB_To_HSV_Ohta,float3)
