/*
 * filename : Lab.c
 * author   : Yoshihiro Kameyama
 * date     : 1994/01/18
 * redate   :
 *
 * co-author : Takahiro Sugiyama
 * update    : 1994/01/19 マクロ化
 */



#include "ColorImage.h"
#include "UCS.h"



/* ==================*
 * UCSモデル  L*a*b* *
 *===================*/
#define DEFINE_RGB_TO( FUNCTION_NAME, RGB_TYPE )			\
void FUNCTION_NAME							\
  _P2 (( RGB_TYPE *, rgb ),						\
       ( float3 *  , ucs ))						\
{									\
  double x, y, z;							\
									\
  x = ( 0.192157 * (double)rgb->at[0] + 0.121565 * (double)rgb->at[1] +	\
        0.078434 * (double)rgb->at[2] ) / X0;				\
  y = ( 0.069398 * (double)rgb->at[0] + 0.318588 * (double)rgb->at[1] +	\
        0.004171 * (double)rgb->at[2] ) / Y0;				\
  z = (                                 0.003921 * (double)rgb->at[1] +	\
        0.388236 * (double)rgb->at[2] ) / Z0;				\
									\
  if ( y > 0.008856 ) ucs->at[0] = 116.0 * pow(y,(double)1/3) - 16.0;	\
  else                ucs->at[0] = 903.29 * y;				\
									\
  if ( x > 0.008856 ) x = pow( x, (double)1/3 );			\
  else                x = 7.787 * x + 16./116.; 			\
  if ( y > 0.008856 ) y = pow( y, (double)1/3 );			\
  else                y = 7.787 * y + 16./116.; 			\
  if ( z > 0.008856 ) z = pow( z, (double)1/3 );			\
  else                z = 7.787 * z + 16./116.; 			\
									\
  ucs->at[1] = 500.0 * ( x - y );					\
  ucs->at[2] = 200.0 * ( y - z );					\
}



DEFINE_RGB_TO(UCharRGB_To_UCS_Lab,uchar3)
DEFINE_RGB_TO(FloatRGB_To_UCS_Lab,float3)
