/*
 * filename : Image.h
 * author   : Takahiro Sugiyama
 * date     : 1992/11/10
 * redate   : 1993/05/19
 */



#ifndef _Image_h
#define _Image_h



#include "misc/typelib.h"



/*======================*
 * image : 画像型の定義
 *======================*/
typedef struct imageRec {
  char  *name;
  long   type;
  long   xsize;
  long   ysize;
  char **data;
} *image;



/*****************************
 * Image : 画像メソッドの定義
 *****************************/

extern struct ImageRec {

/*=========================================*
 * Image.create : 画像型の領域を確保する。
 *=========================================*/
  image ( *create )	      /*    return 画像型 */
	P1 (( char *, name )) /* #1 input  画像名 */

/*==========================================*
 * Image.destroy : 画像型の領域を解放する。
 *==========================================*/
  void ( *destroy )
	P1 (( image, self )) /* #1 input 画像型 */

/*========================================*
 * Image.print : 画像型の属性を表示する。
 *========================================*/
  void ( *print )
	P1 (( image, self )) /* #1 input 画像型 */

/*=================================*
 * Image.load : 画像をロードする。
 *=================================*/
  long ( *load )		  /*    return 真理値(成功:1) */
	P2 (( image , self     ), /* #1 in/out 画像型         */
	    ( char *, filename )) /* #2 input  ファイル名     */

/*=================================*
 * Image.save : 画像をセーブする。
 *=================================*/
  long ( *save )		  /*    return 真理値(成功:1) */
	P3 (( image , self     ), /* #1 input  画像型         */
	    ( char *, filename ), /* #2 input  ファイル名     */
	    ( char *, comment  )) /* #3 input  コメント       */



/*==================================================*
 * Image.name  : 画像名を返す。
 * Image.type  : 画素の型を返す。
 * Image.xsize : Ｘサイズを返す。
 * Image.ysize : Ｙサイズを返す。
 * Image.data  : 画像の二次元配列のポインタを返す。
 *==================================================*/
  char *( *name )	     /*    return 画像名 */
	P1 (( image, self )) /* #1 input  画像型 */

  long ( *type )	     /*    return 画素の型 */
	P1 (( image, self )) /* #1 input  画像型   */

  long ( *xsize )	     /*    return Ｘサイズ */
	P1 (( image, self )) /* #1 input  画像型   */

  long ( *ysize )	     /*    return Ｙサイズ */
	P1 (( image, self )) /* #1 input  画像型   */

  char **( *data )	     /*    return 二次元配列のポインタ */
	P1 (( image, self )) /* #1 input  画像型               */

/*===============================================================*
 * Image.area   : 画像の画素数を返す。
 * Image.byte   : 画像のデータのバイト数を返す。
 * Image.raster : ラスタスキャン用の一次元配列のポインタを返す。
 *===============================================================*/
  long ( *area )	     /*    return 画素数 */
	P1 (( image, self )) /* #1 input  画像型 */

  long ( *byte )	     /*    return バイト数 */
	P1 (( image, self )) /* #1 input  画像型   */

  char *( *raster )	     /*    return 一次元配列のポインタ */
	P1 (( image, self )) /* #1 input  画像型 	       */



/*=============================================*
 * Image.make : 画像のデータの領域を確保する。
 *=============================================*/
  void ( *make )
	P4 (( image, self  ), /* #1 in/out 画像型         */
	    ( long , type  ), /* #2 input  画素の型識別子 */
	    ( long , xsize ), /* #3 input  Ｘサイズ       */
	    ( long , ysize )) /* #4 input  Ｙサイズ       */

/*=============================================*
 * Image.free : 画像のデータの領域を解放する。
 *=============================================*/
  void ( *free )
	P1 (( image, self )) /* #1 in/out 画像型 */

/*=================================*
 * Image.copy : 画像をコピーする。
 *=================================*/
  void ( *copy )
	P2 (( image, self ), /* #1 output コピー先画像型 */
	    ( image, src  )) /* #2 input  コピー元画像型 */

/*==============================================*
 * Image.clear : 画像のデータ領域をクリアする。
 *==============================================*/
  void ( *clear )
	P1 (( image, self )) /* #1 in/out 画像型 */

/*=====================================================*
 * Image.resize : 画像のデータ領域のサイズを変更する。
 *=====================================================*/
  void ( *resize )
	P4 (( image, self  ), /* #1 output 変更後画像型 */
	    ( image, src   ), /* #2 input  入力画像型   */
	    ( long , xsize ), /* #3 input  Ｘサイズ     */
	    ( long , ysize )) /* #4 input  Ｙサイズ     */

/*========================================*
 * Image.sub : 画像の部分領域を切り出す。
 *========================================*/
  void ( *sub )
	P6 (( image, self  ), /* #1 output 出力画像型 */
	    ( image, src   ), /* #2 input  入力画像型 */
	    ( long , src_x ), /* #3 input  左上Ｘ座標 */
	    ( long , src_y ), /* #4 input  左上Ｙ座標 */
	    ( long , xsize ), /* #5 input  Ｘサイズ   */
	    ( long , ysize )) /* #6 input  Ｙサイズ   */

/*=================================*
 * Image.swap : 画像を入れ替える。
 *=================================*/
  void ( *swap )
	P2 (( image, src1 ), /* #1 input 画像型 */
	    ( image, src2 )) /* #2 input 画像型 */



/*====================================================*
 * Image.createFromImage : 画像を生成してコピーする。
 *====================================================*/
  image ( *createFromImage )  /*    return 画像型 */
	P2 (( char *, name ), /* #1 input  画像名 */
	    ( image , src  )) /* #2 input  画像型 */

/*======================================================*
 * Image.createFromFilename : 画像を生成してロードする。
 *======================================================*/
  image ( *createFromFilename )	  /*    return 画像型     */
	P2 (( char *, name     ), /* #1 input  画像名     */
	    ( char *, filename )) /* #2 input  ファイル名 */

/*=========================================================*
 * Image.createMake : 画像を生成してデータ領域を確保する。
 *=========================================================*/
  image ( *createMake )	       /*    return 画像型	   */
	P4 (( char *, name  ), /* #1 input  画像名	   */
	    ( long  , type  ), /* #2 input  画素の型識別子 */
	    ( long  , xsize ), /* #3 input  Ｘサイズ	   */
	    ( long  , ysize )) /* #4 input  Ｙサイズ	   */

/*=====================================================================*
 * Image.createFromBitmapData : ビットマップデータから画像を生成する。
 *=====================================================================*/
  image ( *createFromBitmapData ) /*    return 画像型	          */
	P4 (( char  *, name  ),	  /* #1 input  画像名             */
	    ( uchar *, data  ),	  /* #2 input  ビットマップデータ */
	    ( long   , xsize ),	  /* #3 input  Ｘサイズ           */
	    ( long   , ysize ))	  /* #4 input  Ｙサイズ           */



/*====================================================*
 * Image.display : imagedisp を使って画像を表示する。
 *====================================================*/
  void ( *display )
	P2 (( image , self ), /* #1 input 画像型		 */
	    ( char *, opt  )) /* #2 input imagedisp のオプション */

/*==========================================*
 * Image.undisplay : imagedisp を終了する。
 *==========================================*/
  void ( *undisplay )
	P1 (( image , self )) /* #1 input 画像型 */

} Image;



/*--------------------------------------------------------------------*
 * IS_SHARED : 共有メモリかどうか調べる。
 *	       名前の最初の文字が＠マークならば共有メモリを使用する。
 *--------------------------------------------------------------------*/
#define IS_SHARED( self ) ( ( self )->name[ 0 ] == '@' )



/*==================*
 * アクセスマクロ集
 *==================*/

#define __XSIZE(img)		(img)->xsize
#define __YSIZE(img)		(img)->ysize
#define __NAME(img)		(img)->name
#define __TYPE(img)		(img)->type
#define __DATA(img,type)	((type **)((img)->data))
#define __RASTER(img,type)	(__DATA(img,type)[0])

#define __AREA(img) (__XSIZE(img) * __YSIZE(img))
#define __BYTE(img) (__XSIZE(img) * __YSIZE(img) * typesize(__TYPE(img)))


/*--------------------------------------------------------------*
 * __in_XSIZE : x が画像の xsize に入っていれば、真。
 * __in_YSIZE : y が画像の ysize に入っていれば、真。
 * __in_SIZE  : x, y が画像の xsize, ysize に入っていれば、真。
 *--------------------------------------------------------------*/
#define __in_XSIZE( img, x ) ( ( x >= 0 ) && ( x < __XSIZE( img ) ) )
#define __in_YSIZE( img, y ) ( ( y >= 0 ) && ( y < __YSIZE( img ) ) )
#define __in_SIZE( img, x, y ) (__in_XSIZE( img, x ) && __in_YSIZE( img, y ) )

/*----------------------------------------------------------------------*
 * __not_in_XSIZE : x が画像の xsize に入っていなければ、真。
 * __not_in_YSIZE : y が画像の ysize に入っていなければ、真。
 * __not_in_SIZE  : x, y が画像の xsize, ysize に入っていなければ、真。
 *----------------------------------------------------------------------*/
#define __not_in_XSIZE( img, x ) ( ! __in_XSIZE( img, x ) )
#define __not_in_YSIZE( img, y ) ( ! __in_YSIZE( img, y ) )
#define __not_in_SIZE( img, x, y ) ( ! __in_SIZE( img, x, y ) )

/*-----------------------------------------------------------*
 * __FIX_X : x が画像の xsize 内に入っているように修正する。
 * __FIX_Y : y が画像の ysize 内に入っているように修正する。
 *-----------------------------------------------------------*/
#define __FIX_X( img, x ) (((x) < 0) ? 0 : (((x) >= __XSIZE(img)) ? __XSIZE(img)-1 : (x) ))
#define __FIX_Y( img, y ) (((y) < 0) ? 0 : (((y) >= __YSIZE(img)) ? __YSIZE(img)-1 : (y) ))



/*-------------------------*
 * PackedBit1 用の定義など
 *-------------------------*/
extern uchar PackedBit1Mask[];

#define __PIXEL_PBIT1(img,x,y) \
  (( packedbit1 ** )(( img )->data ))[ y ][ x / 8 ] & PackedBit1Mask[ x % 8]

#define __PIXEL_PBIT1_SET(img,x,y) \
  (( packedbit1 ** )(( img )->data ))[ y ][ x / 8 ] |= PackedBit1Mask[ x % 8]

#define __PIXEL_PBIT1_RESET(img,x,y) \
  (( packedbit1 ** )(( img )->data ))[ y ][ x / 8 ] &= ~PackedBit1Mask[ x % 8]

#define P_PackedBit1      __PIXEL_PBIT1
#define PSet_PackedBit1   __PIXEL_PBIT1_SET
#define PReset_PackedBit1 __PIXEL_PBIT1_RESET

#define __PIXEL( img, x, y, type )	__DATA( img, type )[ y ][ x ]
#define __PIXELPTR( img, x, y, type )	&__PIXEL( img, x, y, type )
#define __PIXELAT( img, x, y, i, type )	__PIXEL( img, x, y, type ).at[ i ]
#define __PIXEL0( img, x, y, type )	__PIXEL( img, x, y, type ).at[ 0 ]
#define __PIXEL1( img, x, y, type )	__PIXEL( img, x, y, type ).at[ 1 ]
#define __PIXEL2( img, x, y, type )	__PIXEL( img, x, y, type ).at[ 2 ]

#define __RAXEL(img,i,type)	__RASTER( img, type )[ i ]
#define __RAXELPTR(img,i,type)	&__RAXSEL( img, i, type )
#define __RAXELAT(img,i,j,type)	__RAXEL( img, i, type ).at[ j ]
#define __RAXEL0(img,i,type)	__RAXEL( img, i, type ).at[ 0 ]
#define __RAXEL1(img,i,type)	__RAXEL( img, i, type ).at[ 1 ]
#define __RAXEL2(img,i,type)	__RAXEL( img, i, type ).at[ 2 ]



/* Extend for X */
/*=========================================================*
 * 表示可能な型(Bit1, Char, UChar, UChar3, Long)に変換する。
 *=========================================================*/
void ImageShowing
  P6 (( image, output    ),
      ( image, original  ),
      ( long , img_x     ),
      ( long , img_y     ),
      ( long , img_xsize ),
      ( long , img_ysize ))



#endif //_Image_h
