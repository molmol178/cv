/*
 * filename  : RGBtoHSV.c
 * author    : Yoshihiro Kameyama
 * date      : 1994/01/18
 * redate    :
 *
 * co-author : Takahiro Sugiyama
 * date      : 1994/01/19 関数テーブルの採用
 */



#include "ColorImage.h"



/*========================================*
 * カラーモデル変換テーブル関数の宣言
 *========================================*/
UCharRGB_To_Function UCharRGB_To_Function_Table[] = {
  UCharRGB_To_HSV_Smith,
  UCharRGB_To_HSL_Joblove,
  UCharRGB_To_HSV_Ohta,
  UCharRGB_To_HSV_Abe,
  UCharRGB_To_HSV_Yagi,
  UCharRGB_To_UCS_Lab,
  UCharRGB_To_UCS_Luv,
};

FloatRGB_To_Function FloatRGB_To_Function_Table[] = {
  FloatRGB_To_HSV_Smith,
  FloatRGB_To_HSL_Joblove,
  FloatRGB_To_HSV_Ohta,
  FloatRGB_To_HSV_Abe,
  FloatRGB_To_HSV_Yagi,
  FloatRGB_To_UCS_Lab,
  FloatRGB_To_UCS_Luv,
};



/*========================================*
 * カラーモデルのチェック
 *========================================*/
static long __model_check
  _P2 (( long  , model ),
       ( char *, name  ))
{
  if ( model < 0 || model > MODEL_NUM )
    {
      fprintf( stderr, "error:%s:not defined model (%d)\n", name, model );
      exit( -1 );
    }

  return 0;
}



/*============================================================*
 * 画素単位でＲＧＢ(uchar3)をＨＳＶ(float3)に変換する。
 *============================================================*/
void UCharRGB_To
  _P3 (( uchar3 *, rgb   ),
       ( float3 *, hsv   ),
       ( int     , model ))
{
  __model_check( model, "UCharRGB_To" );
  UCHAR_RGB_TO( rgb, hsv, model );
}



/*============================================================*
 * 画素単位でＲＧＢ(float3)をＨＳＶ(float3)に変換する。
 *============================================================*/
void FloatRGB_To
  _P3 (( float3 *, rgb   ),
       ( float3 *, hsv   ),
       ( int     , model ))
{
  __model_check( model, "FloatRGB_To" );
  FLOAT_RGB_TO( rgb, hsv, model );
}



/*========================================================*
 * RGBカラー画像(uchar3型)をHSV画像(float3型)に変換する。
 *========================================================*/
void RGBImage_To
  _P3 (( image, rgb   ), /* 入力ＲＧＢ画像(uchar3) */
       ( image, hsv   ), /* 出力ＨＳＶ画像(float3) */
       ( int  , model )) /* モデル名               */
{
  register long i, n;
  register uchar3 *rgb_ras;
  register float3 *hsv_ras;
  register UCharRGB_To_Function func;

  if ( __TYPE( rgb ) != UChar3 )
    {
      //fprintf( stderr, "error:RGBImage_To:The image (%s) is wrong type (%s).\n", __NAME( rgb ), typename( __TYPE( rgb ) ) );
      fprintf( stderr, "error:RGBImage_To:The image (%s) is wrong type (%d).\n", __NAME( rgb ), __TYPE( rgb ) );
      exit( -1 );
    }

  __model_check( model, "RGBImage_To" );

  func = UCHAR_RGB_TO_FUNCTION( model );

  Image.make( hsv, Float3, __XSIZE( rgb ), __YSIZE( rgb ) );

  rgb_ras = __RASTER( rgb, uchar3 );
  hsv_ras = __RASTER( hsv, float3 );
  n = __AREA( rgb );

  for ( i = 0; i < n; i++ )
    func( rgb_ras++, hsv_ras++ );
}
