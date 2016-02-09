/*
 * filename  : ColorImage.h
 * author    : Yoshihiro Kameyama
 * date      : 1994/01/18
 * redate    : 1994/01/21  $B#H#S#V6u4V$N5wN%(B
 *
 * co-author : Takahiro Sugiyama
 * date      : 1994/01/19  $B4X?t%F!<%V%k$N:NMQ(B
 * update    : 1994/01/22  $B#H#S#V6u4V$N5wN%$N%^%/%mDI2C(B
 */



#ifndef _ColorImage_h
#define _ColorImage_h



#include "Image.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



/*============================================================*
 * $B%+%i!<%b%G%k$NDj5A(B
 *============================================================*/
enum {
  HSV_MODEL_Smith = 0,		/* Smith$B$NAPO;3Q?m%b%G%k(B            */
  HSL_MODEL_Joblove,		/* Joblove$B$i$NAPO;3Q?m%b%G%k(B        */
  HSV_MODEL_Ohta,		/* Ohta$B$i$N%b%G%k(B                   */
  HSV_MODEL_Abe,		/* $BAPO;3Q?m%b%G%k$r=$@5$7$?%b%G%k#1(B */
  HSV_MODEL_Yagi,		/* $BAPO;3Q?m%b%G%k$r=$@5$7$?%b%G%k#2(B */
  UCS_MODEL_Lab,		/*  */
  UCS_MODEL_Luv,		/*  */
  MODEL_NUM,			/* $B%+%i!<%b%G%k$N8D?t(B               */
};



/*========================================*
 * $B%+%i!<%b%G%k$NJQ494X?t$NDj5A(B
 *========================================*/
typedef void (*UCharRGB_To_Function) P2 (( uchar3 *, rgb ),
					 ( float3 *, hsv ))

typedef void (*FloatRGB_To_Function) P2 (( float3 *, rgb ),
					 ( float3 *, hsv ))



/*========================================*
 * $B%+%i!<%b%G%k$NJQ494X?t$N@k8@(B
 *========================================*/
extern UCharRGB_To_Function UCharRGB_To_Function_Table[];
extern FloatRGB_To_Function FloatRGB_To_Function_Table[];



/*========================================*
 * $B%+%i!<%b%G%k$NJQ494X?t$N%^%/%m(B
 *========================================*/
#define UCHAR_RGB_TO_FUNCTION( model ) UCharRGB_To_Function_Table[ model ]
#define FLOAT_RGB_TO_FUNCTION( model ) FloatRGB_To_Function_Table[ model ]

#define UCHAR_RGB_TO( rgb, hsv, model ) \
  UCHAR_RGB_TO_FUNCTION( model )( rgb, hsv )

#define FLOAT_RGB_TO( rgb, hsv, model ) \
  FLOAT_RGB_TO_FUNCTION( model )( rgb, hsv )



/*=======================================================================*
 * RGBtoHSV_uchar3 : $B#R#G#B(B(uchar3)$B$r#H#S#V(B(float3)$B$KJQ49$9$k!#(B
 * RGBtoHSV_float3 : $B#R#G#B(B(float3)$B$r#H#S#V(B(float3)$B$KJQ49$9$k!#(B
 *=======================================================================*/
void UCharRGB_To
  P3 (( uchar3 *, rgb   ), /* $BF~NO#R#G#B(B */
      ( float3 *, hsv   ), /* $B=PNO#H#S#V(B */
      ( int     , model )) /* $B%b%G%kL>(B   */

void FloatRGB_To
  P3 (( float3 *, rgb   ), /* $BF~NO#R#G#B(B */
      ( float3 *, hsv   ), /* $B=PNO#H#S#V(B */
      ( int     , model )) /* $B%b%G%kL>(B   */



/*=======================================================================*
 * RGBImageTo:$B#R#G#B2hA|(B(uchar3)$B$r#H#S#V2hA|(B(float3)$B$KJQ49$9$k!#(B
 *=======================================================================*/
void RGBImage_To
  P3 (( image, rgb   ), /* $BF~NO#R#G#B2hA|(B(uchar3) */
      ( image, hsv   ), /* $B=PNO#H#S#V2hA|(B(float3) */
      ( int  , model )) /* $B%b%G%kL>(B               */



/*=======================*
 * Smith$B$NAPO;3Q7A%b%G%k(B *
 *=======================*/
void UCharRGB_To_HSV_Smith
  P2 (( uchar3 *, rgb ),
      ( float3 *, hsv ))

void FloatRGB_To_HSV_Smith
  P2 (( float3 *, rgb ),
      ( float3 *, hsv ))



/*=======================*
 * Joblove$B$NO;3Q?m%b%G%k(B *
 *=======================*/
void UCharRGB_To_HSL_Joblove
  P2 (( uchar3 *, rgb ),
      ( float3 *, hsl ))

void FloatRGB_To_HSL_Joblove
  P2 (( float3 *, rgb ),
      ( float3 *, hsl ))



/*================*
 * Ohta$B$i$N%b%G%k(B *
 *================*/
void UCharRGB_To_HSV_Ohta
  P2 (( uchar3 *, rgb ),
      ( float3 *, hsv ))

void FloatRGB_To_HSV_Ohta
  P2 (( float3 *, rgb ),
      ( float3 *, hsv ))



/*===========================*
 * $BO;3Q?m%b%G%k$N=$@5%b%G%k#1(B*
 *===========================*/
void UCharRGB_To_HSV_Abe
  P2 (( uchar3 *, rgb ),
      ( float3 *, hsv ))

void FloatRGB_To_HSV_Abe
  P2 (( float3 *, rgb ),
      ( float3 *, hsv ))



/*===========================*
 * $BO;3Q?m%b%G%k$N=$@5%b%G%k#2(B*
 *===========================*/
void UCharRGB_To_HSV_Yagi
  P2 (( uchar3 *, rgb ),
      ( float3 *, hsv ))

void FloatRGB_To_HSV_Yagi
  P2 (( float3 *, rgb ),
      ( float3 *, hsv ))



/*
   $BDj?t(BX0,Y0,Z0$B$O(BXYZ$BI=?'7O$K$*$1$kI8=`Gr?'$NDj?t$G$"$k!#$3$3$G$O$3$l$r(B
 R=G=B=255 $B$rI8=`Gr?'$H$7$?$H$-$N(BX,Y,Z$B$NCM$H2r<a$7$F$$$k!#(BUSC$B%+%i!<%b%G%k(B
 L*u*v*$BI=?'7O$NDj?t(BU0$B!"(BV0$B$bF1MM$K$7$F5a$a$?!#$3$l$G$$$$$N$+(B100%$B<+?.$O$J$$(B
*/
   
#define X0  100.00
#define Y0  100.00
#define Z0  100.00
#define U0  0.21053
#define V0  0.47368

/*===================*
 * UCS$B%b%G%k(B  L*a*b* *
 *===================*/
void UCharRGB_To_UCS_Lab
  P2 (( uchar3 *, rgb ),
      ( float3 *, ucs ))

void FloatRGB_To_UCS_Lab
  P2 (( float3 *, rgb ),
      ( float3 *, ucs ))



/*===================*
 * UCS$B%b%G%k(B  L*u*v* *
 *===================*/
void UCharRGB_To_UCS_Luv
  P2 (( uchar3 *, rgb ),
      ( float3 *, ucs ))

void FloatRGB_To_UCS_Luv
  P2 (( float3 *, rgb ),
      ( float3 *, ucs ))



/*=======================================*
 * HSV$B6u4V>e$G$N#2E@4V$N%f!<%/%j%C%I5wN%(B *
 *=======================================*/
#define DEGREE_TO_RADIAN( degree ) ( M_PI * ( degree ) / 180.0 )

#define HSV_DISTANCE( p1, p2 ) \
  sqrt((p1)->at[1]*(p1)->at[1] + (p2)->at[1]*(p2)->at[1] - \
       2 * (p1)->at[1] * (p2)->at[1] * \
       cos(fabs(DEGREE_TO_RADIAN((p1)->at[0]) - \
		DEGREE_TO_RADIAN((p2)->at[0]))) \
       + (((p1)->at[2] - (p2)->at[2]) * ((p1)->at[2] - (p2)->at[2])))

double HSV_Distance
  P2 (( float3 *, point1 ),
      ( float3 *, point2 ))



#endif //_ColorImage_h
