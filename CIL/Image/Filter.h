/*
 * filename : Filter.h
 * author   : Takahiro Sugiyama
 * date     : Friday, April 8 1994
 * describe : general filtering
 */



#ifndef __image_filter_h
#define __image_filter_h



#include "Image.h"
#include "misc/typelib.h"
#include "misc/memlib.h"
#include <math.h>



typedef union {
  char   **of_any;
  char   **of_char;
  uchar  **of_uchar;
  short  **of_short;
  ushort **of_ushort;
  long   **of_long;
  ulong  **of_ulong;
  double **of_double;
  float  **of_float;
} t_data;



typedef struct {
  int xspot;
  int yspot;
  int xsize;
  int ysize;
  int type;
  t_data data;

  int left;
  int right;
  int top;
  int bottom;

} t_filterRec, *t_filter;



/*===================================*
 * filter__make : フィルタを作成する
 *===================================*/
t_filter filter__make /* フィルタのポインタ */
  P5 (( int, type  ), /* 型識別子 */
      ( int, xsize ), /* フィルタＸサイズ */
      ( int, ysize ), /* フィルタＹサイズ */
      ( int, xspot ), /* 中心Ｘ座標 */
      ( int, yspot )) /* 中心Ｙ座標 */

/*===================================*
 * filter__free : フィルタを解放する
 *===================================*/
void filter__free
  P1 (( t_filter, filter ))



/*===============================================================*
 * image__filtering_regular : 正規化して一般的なフィルタをかける
 *===============================================================*/
void image__filtering_regular
  P3 (( image   , output ), /* フィルタリング結果(float) */
      ( image   , input  ), /* 入力画像 */
      ( t_filter, filter )) /* フィルタ */




/*====================================*
 * t_localfunc : ローカル関数の型定義
 *====================================*/
typedef void (*t_localfunc)
     P5 (( image, output ),
	 ( image, input  ),
	 ( int  , x      ),
	 ( int  , y      ),
	 ( char*, user   ))

/*=============================================*
 * image__filtering : 一般的なフィルタをかける
 *=============================================*/
void image__filtering
  P5 (( image      , output ), /* フィルタリング結果(float) */
      ( int        , output_type ), /* 出力の画素型 */
      ( image      , input  ), /* 入力画像 */
      ( t_localfunc, func   ), /* ローカル関数 */
      ( char *     , user   )) /* ユーザ指定値 */


/*========================================================================*
 * filter__make_G_shift : 少し位置ずれしたガウシアン・フィルタ
 *========================================================================*/
t_filter filter__make_G_shift /* ガウシアン・フィルタ */
  P3 (( double, sigma ), /* 標準偏差 */
      ( double, dx    ), /* Ｘシフト */
      ( double, dy    )) /* Ｙシフト */

/*========================================================================*
 * filter__make_GGX_shift : 少し位置ずれしたＸ一階微分ガウシアン・フィルタ
 *========================================================================*/
t_filter filter__make_GGX_shift /* 一階ガウシアン・フィルタ */
  P3 (( double, sigma ), /* 標準偏差 */
      ( double, dx    ), /* Ｘシフト */
      ( double, dy    )) /* Ｙシフト */

/*========================================================================*
 * filter__make_GGY_shift : 少し位置ずれしたＹ一階微分ガウシアン・フィルタ
 *========================================================================*/
t_filter filter__make_GGY_shift /* 一階ガウシアン・フィルタ */
  P3 (( double, sigma ), /* 標準偏差 */
      ( double, dx    ), /* Ｘシフト */
      ( double, dy    )) /* Ｙシフト */



/*========================================================================*
 * filter__make_GGX : ガウシアンをＸ一階微分したフィルタ
 *========================================================================*/
t_filter filter__make_GGX /* Ｘ一階ガウシアン・フィルタ */
  P1 (( double, sigma )) /* 標準偏差 */

/*========================================================================*
 * filter__make_GGY : ガウシアンをＹ一階微分したフィルタ
 *========================================================================*/
t_filter filter__make_GGY /* Ｙ一階ガウシアン・フィルタ */
  P1 (( double, sigma )) /* 標準偏差 */

/*==============================================*
 * filter__make_G : ガウシアン・フィルタ
 *==============================================*/
t_filter filter__make_G /* ガウシアン・フィルタ */
  P1 (( double, sigma )) /* 標準偏差 */



/*************************
 * Round Pixel Extension
 *************************/

/*=============================================================*
 * filter__make_round_pixel : ラウンド・ピクセルのマスクを作る
 * 配列の添字は -radius から +radius でアクセスできる
 *=============================================================*/
t_filter filter__make_round_pixel /* ラウンド・ピクセルのポインタ */
  P1 (( long, radius )) /* 半径の画素数 */



#endif __image_filter_h
