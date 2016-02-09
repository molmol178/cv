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
 * image : �����������
 *======================*/
typedef struct imageRec {
  char  *name;
  long   type;
  long   xsize;
  long   ysize;
  char **data;
} *image;



/*****************************
 * Image : �����᥽�åɤ����
 *****************************/

extern struct ImageRec {

/*=========================================*
 * Image.create : ���������ΰ����ݤ��롣
 *=========================================*/
  image ( *create )	      /*    return ������ */
	P1 (( char *, name )) /* #1 input  ����̾ */

/*==========================================*
 * Image.destroy : ���������ΰ��������롣
 *==========================================*/
  void ( *destroy )
	P1 (( image, self )) /* #1 input ������ */

/*========================================*
 * Image.print : ��������°����ɽ�����롣
 *========================================*/
  void ( *print )
	P1 (( image, self )) /* #1 input ������ */

/*=================================*
 * Image.load : ��������ɤ��롣
 *=================================*/
  long ( *load )		  /*    return ������(����:1) */
	P2 (( image , self     ), /* #1 in/out ������         */
	    ( char *, filename )) /* #2 input  �ե�����̾     */

/*=================================*
 * Image.save : �����򥻡��֤��롣
 *=================================*/
  long ( *save )		  /*    return ������(����:1) */
	P3 (( image , self     ), /* #1 input  ������         */
	    ( char *, filename ), /* #2 input  �ե�����̾     */
	    ( char *, comment  )) /* #3 input  ������       */



/*==================================================*
 * Image.name  : ����̾���֤���
 * Image.type  : ���Ǥη����֤���
 * Image.xsize : �إ��������֤���
 * Image.ysize : �٥��������֤���
 * Image.data  : �������󼡸�����Υݥ��󥿤��֤���
 *==================================================*/
  char *( *name )	     /*    return ����̾ */
	P1 (( image, self )) /* #1 input  ������ */

  long ( *type )	     /*    return ���Ǥη� */
	P1 (( image, self )) /* #1 input  ������   */

  long ( *xsize )	     /*    return �إ����� */
	P1 (( image, self )) /* #1 input  ������   */

  long ( *ysize )	     /*    return �٥����� */
	P1 (( image, self )) /* #1 input  ������   */

  char **( *data )	     /*    return �󼡸�����Υݥ��� */
	P1 (( image, self )) /* #1 input  ������               */

/*===============================================================*
 * Image.area   : �����β��ǿ����֤���
 * Image.byte   : �����Υǡ����ΥХ��ȿ����֤���
 * Image.raster : �饹����������Ѥΰ켡������Υݥ��󥿤��֤���
 *===============================================================*/
  long ( *area )	     /*    return ���ǿ� */
	P1 (( image, self )) /* #1 input  ������ */

  long ( *byte )	     /*    return �Х��ȿ� */
	P1 (( image, self )) /* #1 input  ������   */

  char *( *raster )	     /*    return �켡������Υݥ��� */
	P1 (( image, self )) /* #1 input  ������ 	       */



/*=============================================*
 * Image.make : �����Υǡ������ΰ����ݤ��롣
 *=============================================*/
  void ( *make )
	P4 (( image, self  ), /* #1 in/out ������         */
	    ( long , type  ), /* #2 input  ���Ǥη����̻� */
	    ( long , xsize ), /* #3 input  �إ�����       */
	    ( long , ysize )) /* #4 input  �٥�����       */

/*=============================================*
 * Image.free : �����Υǡ������ΰ��������롣
 *=============================================*/
  void ( *free )
	P1 (( image, self )) /* #1 in/out ������ */

/*=================================*
 * Image.copy : �����򥳥ԡ����롣
 *=================================*/
  void ( *copy )
	P2 (( image, self ), /* #1 output ���ԡ�������� */
	    ( image, src  )) /* #2 input  ���ԡ��������� */

/*==============================================*
 * Image.clear : �����Υǡ����ΰ�򥯥ꥢ���롣
 *==============================================*/
  void ( *clear )
	P1 (( image, self )) /* #1 in/out ������ */

/*=====================================================*
 * Image.resize : �����Υǡ����ΰ�Υ��������ѹ����롣
 *=====================================================*/
  void ( *resize )
	P4 (( image, self  ), /* #1 output �ѹ�������� */
	    ( image, src   ), /* #2 input  ���ϲ�����   */
	    ( long , xsize ), /* #3 input  �إ�����     */
	    ( long , ysize )) /* #4 input  �٥�����     */

/*========================================*
 * Image.sub : ��������ʬ�ΰ���ڤ�Ф���
 *========================================*/
  void ( *sub )
	P6 (( image, self  ), /* #1 output ���ϲ����� */
	    ( image, src   ), /* #2 input  ���ϲ����� */
	    ( long , src_x ), /* #3 input  ����غ�ɸ */
	    ( long , src_y ), /* #4 input  ����ٺ�ɸ */
	    ( long , xsize ), /* #5 input  �إ�����   */
	    ( long , ysize )) /* #6 input  �٥�����   */

/*=================================*
 * Image.swap : �����������ؤ��롣
 *=================================*/
  void ( *swap )
	P2 (( image, src1 ), /* #1 input ������ */
	    ( image, src2 )) /* #2 input ������ */



/*====================================================*
 * Image.createFromImage : �������������ƥ��ԡ����롣
 *====================================================*/
  image ( *createFromImage )  /*    return ������ */
	P2 (( char *, name ), /* #1 input  ����̾ */
	    ( image , src  )) /* #2 input  ������ */

/*======================================================*
 * Image.createFromFilename : �������������ƥ��ɤ��롣
 *======================================================*/
  image ( *createFromFilename )	  /*    return ������     */
	P2 (( char *, name     ), /* #1 input  ����̾     */
	    ( char *, filename )) /* #2 input  �ե�����̾ */

/*=========================================================*
 * Image.createMake : �������������ƥǡ����ΰ����ݤ��롣
 *=========================================================*/
  image ( *createMake )	       /*    return ������	   */
	P4 (( char *, name  ), /* #1 input  ����̾	   */
	    ( long  , type  ), /* #2 input  ���Ǥη����̻� */
	    ( long  , xsize ), /* #3 input  �إ�����	   */
	    ( long  , ysize )) /* #4 input  �٥�����	   */

/*=====================================================================*
 * Image.createFromBitmapData : �ӥåȥޥåץǡ�������������������롣
 *=====================================================================*/
  image ( *createFromBitmapData ) /*    return ������	          */
	P4 (( char  *, name  ),	  /* #1 input  ����̾             */
	    ( uchar *, data  ),	  /* #2 input  �ӥåȥޥåץǡ��� */
	    ( long   , xsize ),	  /* #3 input  �إ�����           */
	    ( long   , ysize ))	  /* #4 input  �٥�����           */



/*====================================================*
 * Image.display : imagedisp ��ȤäƲ�����ɽ�����롣
 *====================================================*/
  void ( *display )
	P2 (( image , self ), /* #1 input ������		 */
	    ( char *, opt  )) /* #2 input imagedisp �Υ��ץ���� */

/*==========================================*
 * Image.undisplay : imagedisp ��λ���롣
 *==========================================*/
  void ( *undisplay )
	P1 (( image , self )) /* #1 input ������ */

} Image;



/*--------------------------------------------------------------------*
 * IS_SHARED : ��ͭ���꤫�ɤ���Ĵ�٤롣
 *	       ̾���κǽ��ʸ�������ޡ����ʤ�ж�ͭ�������Ѥ��롣
 *--------------------------------------------------------------------*/
#define IS_SHARED( self ) ( ( self )->name[ 0 ] == '@' )



/*==================*
 * ���������ޥ���
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
 * __in_XSIZE : x �������� xsize �����äƤ���С�����
 * __in_YSIZE : y �������� ysize �����äƤ���С�����
 * __in_SIZE  : x, y �������� xsize, ysize �����äƤ���С�����
 *--------------------------------------------------------------*/
#define __in_XSIZE( img, x ) ( ( x >= 0 ) && ( x < __XSIZE( img ) ) )
#define __in_YSIZE( img, y ) ( ( y >= 0 ) && ( y < __YSIZE( img ) ) )
#define __in_SIZE( img, x, y ) (__in_XSIZE( img, x ) && __in_YSIZE( img, y ) )

/*----------------------------------------------------------------------*
 * __not_in_XSIZE : x �������� xsize �����äƤ��ʤ���С�����
 * __not_in_YSIZE : y �������� ysize �����äƤ��ʤ���С�����
 * __not_in_SIZE  : x, y �������� xsize, ysize �����äƤ��ʤ���С�����
 *----------------------------------------------------------------------*/
#define __not_in_XSIZE( img, x ) ( ! __in_XSIZE( img, x ) )
#define __not_in_YSIZE( img, y ) ( ! __in_YSIZE( img, y ) )
#define __not_in_SIZE( img, x, y ) ( ! __in_SIZE( img, x, y ) )

/*-----------------------------------------------------------*
 * __FIX_X : x �������� xsize ������äƤ���褦�˽������롣
 * __FIX_Y : y �������� ysize ������äƤ���褦�˽������롣
 *-----------------------------------------------------------*/
#define __FIX_X( img, x ) (((x) < 0) ? 0 : (((x) >= __XSIZE(img)) ? __XSIZE(img)-1 : (x) ))
#define __FIX_Y( img, y ) (((y) < 0) ? 0 : (((y) >= __YSIZE(img)) ? __YSIZE(img)-1 : (y) ))



/*-------------------------*
 * PackedBit1 �Ѥ�����ʤ�
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
 * ɽ����ǽ�ʷ�(Bit1, Char, UChar, UChar3, Long)���Ѵ����롣
 *=========================================================*/
void ImageShowing
  P6 (( image, output    ),
      ( image, original  ),
      ( long , img_x     ),
      ( long , img_y     ),
      ( long , img_xsize ),
      ( long , img_ysize ))



#endif //_Image_h
