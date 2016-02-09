/*
 * filename  : ColorImage.h
 * author    : Yoshihiro Kameyama
 * date      : 1994/01/18
 * redate    : 1994/01/21  ＨＳＶ空間の距離
 *
 * co-author : Takahiro Sugiyama
 * date      : 1994/01/19  関数テーブルの採用
 * update    : 1994/01/22  ＨＳＶ空間の距離のマクロ追加
 */



#ifndef _ColorImage_h
#define _ColorImage_h



#include "Image.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



/*============================================================*
 * カラーモデルの定義
 *============================================================*/
enum {
  HSV_MODEL_Smith = 0,		/* Smithの双六角錐モデル            */
  HSL_MODEL_Joblove,		/* Jobloveらの双六角錐モデル        */
  HSV_MODEL_Ohta,		/* Ohtaらのモデル                   */
  HSV_MODEL_Abe,		/* 双六角錐モデルを修正したモデル１ */
  HSV_MODEL_Yagi,		/* 双六角錐モデルを修正したモデル２ */
  UCS_MODEL_Lab,		/*  */
  UCS_MODEL_Luv,		/*  */
  MODEL_NUM,			/* カラーモデルの個数               */
};



/*========================================*
 * カラーモデルの変換関数の定義
 *========================================*/
typedef void (*UCharRGB_To_Function) P2 (( uchar3 *, rgb ),
					 ( float3 *, hsv ))

typedef void (*FloatRGB_To_Function) P2 (( float3 *, rgb ),
					 ( float3 *, hsv ))



/*========================================*
 * カラーモデルの変換関数の宣言
 *========================================*/
extern UCharRGB_To_Function UCharRGB_To_Function_Table[];
extern FloatRGB_To_Function FloatRGB_To_Function_Table[];



/*========================================*
 * カラーモデルの変換関数のマクロ
 *========================================*/
#define UCHAR_RGB_TO_FUNCTION( model ) UCharRGB_To_Function_Table[ model ]
#define FLOAT_RGB_TO_FUNCTION( model ) FloatRGB_To_Function_Table[ model ]

#define UCHAR_RGB_TO( rgb, hsv, model ) \
  UCHAR_RGB_TO_FUNCTION( model )( rgb, hsv )

#define FLOAT_RGB_TO( rgb, hsv, model ) \
  FLOAT_RGB_TO_FUNCTION( model )( rgb, hsv )



/*=======================================================================*
 * RGBtoHSV_uchar3 : ＲＧＢ(uchar3)をＨＳＶ(float3)に変換する。
 * RGBtoHSV_float3 : ＲＧＢ(float3)をＨＳＶ(float3)に変換する。
 *=======================================================================*/
void UCharRGB_To
  P3 (( uchar3 *, rgb   ), /* 入力ＲＧＢ */
      ( float3 *, hsv   ), /* 出力ＨＳＶ */
      ( int     , model )) /* モデル名   */

void FloatRGB_To
  P3 (( float3 *, rgb   ), /* 入力ＲＧＢ */
      ( float3 *, hsv   ), /* 出力ＨＳＶ */
      ( int     , model )) /* モデル名   */



/*=======================================================================*
 * RGBImageTo:ＲＧＢ画像(uchar3)をＨＳＶ画像(float3)に変換する。
 *=======================================================================*/
void RGBImage_To
  P3 (( image, rgb   ), /* 入力ＲＧＢ画像(uchar3) */
      ( image, hsv   ), /* 出力ＨＳＶ画像(float3) */
      ( int  , model )) /* モデル名               */



/*=======================*
 * Smithの双六角形モデル *
 *=======================*/
void UCharRGB_To_HSV_Smith
  P2 (( uchar3 *, rgb ),
      ( float3 *, hsv ))

void FloatRGB_To_HSV_Smith
  P2 (( float3 *, rgb ),
      ( float3 *, hsv ))



/*=======================*
 * Jobloveの六角錐モデル *
 *=======================*/
void UCharRGB_To_HSL_Joblove
  P2 (( uchar3 *, rgb ),
      ( float3 *, hsl ))

void FloatRGB_To_HSL_Joblove
  P2 (( float3 *, rgb ),
      ( float3 *, hsl ))



/*================*
 * Ohtaらのモデル *
 *================*/
void UCharRGB_To_HSV_Ohta
  P2 (( uchar3 *, rgb ),
      ( float3 *, hsv ))

void FloatRGB_To_HSV_Ohta
  P2 (( float3 *, rgb ),
      ( float3 *, hsv ))



/*===========================*
 * 六角錐モデルの修正モデル１*
 *===========================*/
void UCharRGB_To_HSV_Abe
  P2 (( uchar3 *, rgb ),
      ( float3 *, hsv ))

void FloatRGB_To_HSV_Abe
  P2 (( float3 *, rgb ),
      ( float3 *, hsv ))



/*===========================*
 * 六角錐モデルの修正モデル２*
 *===========================*/
void UCharRGB_To_HSV_Yagi
  P2 (( uchar3 *, rgb ),
      ( float3 *, hsv ))

void FloatRGB_To_HSV_Yagi
  P2 (( float3 *, rgb ),
      ( float3 *, hsv ))



/*
   定数X0,Y0,Z0はXYZ表色系における標準白色の定数である。ここではこれを
 R=G=B=255 を標準白色としたときのX,Y,Zの値と解釈している。USCカラーモデル
 L*u*v*表色系の定数U0、V0も同様にして求めた。これでいいのか100%自信はない
*/
   
#define X0  100.00
#define Y0  100.00
#define Z0  100.00
#define U0  0.21053
#define V0  0.47368

/*===================*
 * UCSモデル  L*a*b* *
 *===================*/
void UCharRGB_To_UCS_Lab
  P2 (( uchar3 *, rgb ),
      ( float3 *, ucs ))

void FloatRGB_To_UCS_Lab
  P2 (( float3 *, rgb ),
      ( float3 *, ucs ))



/*===================*
 * UCSモデル  L*u*v* *
 *===================*/
void UCharRGB_To_UCS_Luv
  P2 (( uchar3 *, rgb ),
      ( float3 *, ucs ))

void FloatRGB_To_UCS_Luv
  P2 (( float3 *, rgb ),
      ( float3 *, ucs ))



/*=======================================*
 * HSV空間上での２点間のユークリッド距離 *
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
