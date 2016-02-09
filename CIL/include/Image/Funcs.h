/*
 * filename : Funcs.h
 * author   : Takahiro Sugiyama
 * date     : 1993
 * describe : Extend Image Functions
 */



#ifndef _image_funcs_h
#define _image_funcs_h



#include <math.h>
#include "Image.h"
#include "misc/typelib.h"
#include "misc/memlib.h"



typedef struct {
  long type;
  void *caller;
} caller_table;

void *image__caller P ((char *func_name,image src,long table_num,
			caller_table *table_list));
void image__function_nosupport P ((char *func_name,image src));
void image__type_check P ((char *func_name,image src, long type));



/*
 * 入力画像は 指定型 の画像
 * 出力画像は 指定型 の画像
 * 領域をコピーします
 * コピーできない領域は無視します
 * dest はあらかじめ領域を確保しておいて下さい
 */
void image__copyarea P ((image dest,image src,long dest_x,long dest_y,
			 long src_x,long src_y,long width,long height));



/*
 * 入力画像は 指定型 の濃淡画像
 * 出力画像は Short2型 の (x,y) 方向の結果の値
 * th は閾値で、結果の絶対値が th より小さければ 0 とします。
 */
void image__sobel P ((image dest,image src,long th));



/*
 * 入力画像は 指定型 の濃淡画像
 * 出力画像は Short2型 の (x,y) 方向の結果の値
 * th は閾値で、結果の絶対値が th より小さければ 0 とします。
 */
void image__laplacian4 P ((image dest,image src,long th));



/*
 * メディアンフィルタ
 * 入力画像は 指定型 の画像
 * 出力画像は 指定型 の画像
 * median は中間値
 */
void image__median P ((image dest,image src,long median));



/*
 * 閾値処理
 * 入力画像は 指定型 の画像
 * 出力画像は ２値画像
 */
void image__thresholding
  P3 (( image , out       ),
      ( image , in        ),
      ( double, threshold ))



/*
 * 入力画像は 任意の型 の画像
 * 出力画像はのPNMフォーマットでセーブできる画像
 */
void image__any_to_pnm P ((image dst,image src));



/*
 * 入力画像は 指定型 の複合画像
 * 出力画像は その型 の単一画像
 */
void image__separate2 P ((image dst1,image dst2,image src));



/*
 * 入力画像は 指定型 の画像
 * 出力画像は その型 の画像
 * 指定された幅の画像の端の周りの溝を
 * 隣接する画素値の平均値で補間します
 */
void image__fillmoat P ((image src,long width));



/*
 * 入力画像は 指定複合型 の画像
 * 出力画像は その単一型 の画像
 * 複合画素の各平均を計算します
 */
void image__average P ((image dst,image src));



/*
 * 画像の濃度を反転します
 */
void image__reverse P ((image dst,image src));



/*
 * 画像の濃度を線形変換します
 */
void image__liner P ((image dst,image src));



/*
 * 入力画像は 指定型 の画像
 * 出力画像は Long型 の画像
 * AnyType  -> Long
 * AnyType2 -> Long2
 * AnyType3 -> Long3
 */
void image__long_type P ((image dst,image src));



/*
 * ガウシアンフィルタ
 * 
 * ガウスフィルタを施します
 * 入力画像は UChar型 の濃淡画像
 * 出力画像は UChar型 の濃淡画像
 */
void image__gaussian P ((image dst,image src,double sigma));



/*
 * ガウシアン・グラディエント・フィルタ
 *
 * ガウスフィルタを施します
 * 入力画像は UChar型 の濃淡画像
 * 出力画像は Float 型の画像
 */
void image__gaussian_gradient P ((image mag,image grad_x,image grad_y,
				  image src,double sigma));
void image__gaussian_gradient_abs
  P3 (( image , mag   ),
      ( image , src   ),
      ( double, sigma ))



/*
 * 塗りつぶす
 */
void image__fill_in
  P2 (( image, dest ),
      ( image, src  ))



/*=========================================*
 * image__bind_data : データをバインドする
 *=========================================*/
void image__bind_data
  P5 (( image  , output ),
      ( char **, data   ),
      ( long   , type   ),
      ( long   , xsize  ),
      ( long   , ysize  ))


void image__and
  P3 (( image, output ),
      ( image, input1 ),
      ( image, input2 ))

void image__or
  P3 (( image, output ),
      ( image, input1 ),
      ( image, input2 ))

void image__eor
  P3 (( image, output ),
      ( image, input1 ),
      ( image, input2 ))


/*------------------------------------------------------------------------*
 * ４近傍のラベリング。
 * 入力originalを出力labelに１番からラベリングする。
 * 返り値はラベルの数を返す。
 *
 * 入力: Bit1, Char, UChar, Short, UShort, Long
 * 出力: UChar, Short, UShort, Long のラベル型（指定）
 *------------------------------------------------------------------------*/
long image__N4_labelling /* ラベル出力型 = output_type */
	P3 ((image, label), (image, original), (long, output_type))

long image__N4_labelling_uchar /* ラベル出力型 = UChar */
	P2 ((image, label), (image, original))

long image__N4_labelling_short /* ラベル出力型 = Short */
	P2 ((image, label), (image, original))

long image__N4_labelling_ushort /* ラベル出力型 = UShort */
	P2 ((image, label), (image, original))

long image__N4_labelling_long /* ラベル出力型 = Long */
	P2 ((image, label), (image, original))


/*------------------------------------------------------------------------*
 * ４近傍のラベリング。0はラベリング対象にならない。
 * 入力originalを出力labelに１番からラベリングする。
 * 返り値はラベルの数を返す。
 *
 * 入力: Bit1, Char, UChar, Short, UShort, Long
 * 出力: UChar, Short, UShort, Long のラベル型（指定）
 *------------------------------------------------------------------------*/
long image__N4_labelling_foreground /* ラベル出力型 = output_type */
	P3 ((image, label), (image, original), (long, output_type))

long image__N4_labelling_foreground_uchar /* ラベル出力型 = UChar */
	P2 ((image, label), (image, original))

long image__N4_labelling_foreground_short /* ラベル出力型 = Short */
	P2 ((image, label), (image, original))

long image__N4_labelling_foreground_ushort /* ラベル出力型 = UShort */
	P2 ((image, label), (image, original))

long image__N4_labelling_foreground_long /* ラベル出力型 = Long */
	P2 ((image, label), (image, original))



/*------------------------------------------------------------------------*
 * ８近傍のラベリング。
 * 入力originalを出力labelに１番からラベリングする。
 * 返り値はラベルの数を返す。
 *
 * 入力: Bit1, Char, UChar, Short, UShort, Long
 * 出力: UChar, Short, UShort, Long のラベル型（指定）
 *------------------------------------------------------------------------*/
long image__N8_labelling /* ラベル出力型 = output_type */
	P3 ((image, label), (image, original), (long, output_type))

long image__N8_labelling_uchar /* ラベル出力型 = UChar */
	P2 ((image, label), (image, original))

long image__N8_labelling_short /* ラベル出力型 = Short */
	P2 ((image, label), (image, original))

long image__N8_labelling_ushort /* ラベル出力型 = UShort */
	P2 ((image, label), (image, original))

long image__N8_labelling_long /* ラベル出力型 = Long */
	P2 ((image, label), (image, original))


/*------------------------------------------------------------------------*
 * ８近傍のラベリング。0はラベリング対象にならない。
 * 入力originalを出力labelに１番からラベリングする。
 * 返り値はラベルの数を返す。
 *
 * 入力: Bit1, Char, UChar, Short, UShort, Long
 * 出力: UChar, Short, UShort, Long のラベル型（指定）
 *------------------------------------------------------------------------*/
long image__N8_labelling_foreground /* ラベル出力型 = output_type */
	P3 ((image, label), (image, original), (long, output_type))

long image__N8_labelling_foreground_uchar /* ラベル出力型 = UChar */
	P2 ((image, label), (image, original))

long image__N8_labelling_foreground_short /* ラベル出力型 = Short */
	P2 ((image, label), (image, original))

long image__N8_labelling_foreground_ushort /* ラベル出力型 = UShort */
	P2 ((image, label), (image, original))

long image__N8_labelling_foreground_long /* ラベル出力型 = Long */
	P2 ((image, label), (image, original))



/*
 * 使用禁止:
 * 入力画像は 指定型 のクラスタ画像
 * 出力画像は Long型 のラベル画像
 * 関数の返り値はラベルの最大値
 * ラベルは１番から付けられます
 * すでに使用されていない(97/2/6)
 */
long image__labeling P ((image dst,image src));



#endif  _image_funcs_h
