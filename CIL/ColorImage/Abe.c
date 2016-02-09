/*
 * filename  : Abe.c
 * author    : Yoshihiro Kameyama
 * date      : 1994/01/18
 * redate    :
 *
 * co-author : Takahiro Sugiyama
 * update    : 1994/01/19 マクロ化
 */



#include "ColorImage.h"



#define min2(a,b) ((a) < (b) ? (a) : (b))
#define min3(a,b,c) ( (a) < (b) ? min2( (a), (c) ) : min2( (b), (c) ) )
#define max2(a,b) ((a) > (b) ? (a) : (b))
#define max3(a,b,c) ( (a) > (b) ? max2( (a), (c) ) : max2( (b), (c) ) )



/*===========================*
 * 六角錐モデルの修正モデル１*
 *===========================*/
#define DEFINE_RGB_TO( FUNCTION_NAME, RGB_TYPE )		\
void FUNCTION_NAME						\
  _P2 (( RGB_TYPE *, rgb ),					\
       ( float3 *  , hsv ))					\
{								\
  double r, g, b;						\
  double hue, satulation, value;				\
  double max, min, max_min;					\
  double rc, gc, bc;						\
								\
  r = (double)rgb->at[ 0 ] / 255.0;				\
  g = (double)rgb->at[ 1 ] / 255.0;				\
  b = (double)rgb->at[ 2 ] / 255.0;				\
  max = max3( r, g, b );					\
  min = min3( r, g, b );					\
  max_min = max - min;						\
								\
  value = ( max + min ) / 2.0;					\
								\
  if ( max == min )						\
    {/* r = g = b, achromatic case */				\
      satulation = 0;						\
      hue = 0;							\
    }								\
  else								\
    {/* chromatic case */					\
      if ( value <= 0.5 )					\
	satulation = max_min / ( max + min );			\
      else							\
	satulation = max_min / ( 2 - max - min );		\
								\
      satulation *= ( 1.0 - fabs( 1.0 - 2.0 * value ) );	\
								\
      rc = ( max - r ) / max_min;				\
      gc = ( max - g ) / max_min;				\
      bc = ( max - b ) / max_min;				\
								\
      /* resulting color between YELLOW  & MAGENTA */		\
      if ( r == max ) hue = bc - gc; else			\
      /* resulting color between CYAN    & YELLOW  */		\
      if ( g == max ) hue = 2 + rc - bc; else			\
      /* resulting color between MAGENTA & CYAN    */		\
      if ( b == max ) hue = 4 + gc - rc;			\
    }								\
  hue = hue * 60.0; /* hue [0,360] (degree), exchange scaling */	\
  if ( hue < 0.0 )    hue = hue + 360.0;				\
  if ( hue >= 360.0 ) hue = hue - 360.0;				\
								\
  hsv->at[ 0 ] = hue;						\
  hsv->at[ 1 ] = satulation * 255.0;				\
  hsv->at[ 2 ] = value * 255.0;					\
}



DEFINE_RGB_TO(UCharRGB_To_HSV_Abe,uchar3)
DEFINE_RGB_TO(FloatRGB_To_HSV_Abe,float3)
