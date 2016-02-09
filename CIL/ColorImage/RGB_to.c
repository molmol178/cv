/*
 * filename  : RGBtoHSV.c
 * author    : Yoshihiro Kameyama
 * date      : 1994/01/18
 * redate    :
 *
 * co-author : Takahiro Sugiyama
 * date      : 1994/01/19 �ؿ��ơ��֥�κ���
 */



#include "ColorImage.h"



/*========================================*
 * ���顼��ǥ��Ѵ��ơ��֥�ؿ������
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
 * ���顼��ǥ�Υ����å�
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
 * ����ñ�̤ǣңǣ�(uchar3)��ȣӣ�(float3)���Ѵ����롣
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
 * ����ñ�̤ǣңǣ�(float3)��ȣӣ�(float3)���Ѵ����롣
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
 * RGB���顼����(uchar3��)��HSV����(float3��)���Ѵ����롣
 *========================================================*/
void RGBImage_To
  _P3 (( image, rgb   ), /* ���ϣңǣ²���(uchar3) */
       ( image, hsv   ), /* ���ϣȣӣֲ���(float3) */
       ( int  , model )) /* ��ǥ�̾               */
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
