/*
 * filename : Luv.c
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
 * UCSモデル  L*u*v* *
 *===================*/
#define DEFINE_RGB_TO( FUNCTION_NAME, RGB_TYPE )				\
void FUNCTION_NAME								\
  _P2 (( RGB_TYPE *, rgb ),							\
       ( float3 *  , ucs ))							\
{										\
  double x, y, z, yy, wf;								\
										\
  x = 0.192157 * rgb->at[0] + 0.121565 * rgb->at[1] + 0.078434 * rgb->at[2];	\
  y = 0.069398 * rgb->at[0] + 0.318588 * rgb->at[1] + 0.004171 * rgb->at[2];	\
  z =                         0.003921 * rgb->at[1] + 0.388236*rgb->at[2];	\
  yy = y / 100.000;								\
  if ( yy > 0.008856 ) ucs->at[0] = 116.0 * pow(yy,(double)1/3) -16.0;		\
  else                 ucs->at[0] = 903.29 * yy;				\
										\
  wf = (x + 15.0*y + 3.0*z);\
  if ( wf == 0.0 )\
    {\
      ucs->at[1] = 0.0;\
      ucs->at[2] = 0.0;\
    }\
  else\
    {\
      ucs->at[1] = 13.0 * ucs->at[0] * ( 4.0 * x / wf - U0 );\
      ucs->at[2] = 13.0 * ucs->at[0] * ( 9.0 * y / wf - V0 );\
    }\
}



DEFINE_RGB_TO(UCharRGB_To_UCS_Luv,uchar3)
DEFINE_RGB_TO(FloatRGB_To_UCS_Luv,float3)
