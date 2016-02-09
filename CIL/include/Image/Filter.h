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
 * filter__make : �ե��륿���������
 *===================================*/
t_filter filter__make /* �ե��륿�Υݥ��� */
  P5 (( int, type  ), /* �����̻� */
      ( int, xsize ), /* �ե��륿�إ����� */
      ( int, ysize ), /* �ե��륿�٥����� */
      ( int, xspot ), /* �濴�غ�ɸ */
      ( int, yspot )) /* �濴�ٺ�ɸ */

/*===================================*
 * filter__free : �ե��륿���������
 *===================================*/
void filter__free
  P1 (( t_filter, filter ))



/*===============================================================*
 * image__filtering_regular : ���������ư���Ū�ʥե��륿�򤫤���
 *===============================================================*/
void image__filtering_regular
  P3 (( image   , output ), /* �ե��륿��󥰷��(float) */
      ( image   , input  ), /* ���ϲ��� */
      ( t_filter, filter )) /* �ե��륿 */




/*====================================*
 * t_localfunc : ������ؿ��η����
 *====================================*/
typedef void (*t_localfunc)
     P5 (( image, output ),
	 ( image, input  ),
	 ( int  , x      ),
	 ( int  , y      ),
	 ( char*, user   ))

/*=============================================*
 * image__filtering : ����Ū�ʥե��륿�򤫤���
 *=============================================*/
void image__filtering
  P5 (( image      , output ), /* �ե��륿��󥰷��(float) */
      ( int        , output_type ), /* ���Ϥβ��Ƿ� */
      ( image      , input  ), /* ���ϲ��� */
      ( t_localfunc, func   ), /* ������ؿ� */
      ( char *     , user   )) /* �桼�������� */


/*========================================================================*
 * filter__make_G_shift : �������֤��줷�����������󡦥ե��륿
 *========================================================================*/
t_filter filter__make_G_shift /* ���������󡦥ե��륿 */
  P3 (( double, sigma ), /* ɸ���к� */
      ( double, dx    ), /* �إ��ե� */
      ( double, dy    )) /* �٥��ե� */

/*========================================================================*
 * filter__make_GGX_shift : �������֤��줷���ذ쳬��ʬ���������󡦥ե��륿
 *========================================================================*/
t_filter filter__make_GGX_shift /* �쳬���������󡦥ե��륿 */
  P3 (( double, sigma ), /* ɸ���к� */
      ( double, dx    ), /* �إ��ե� */
      ( double, dy    )) /* �٥��ե� */

/*========================================================================*
 * filter__make_GGY_shift : �������֤��줷���ٰ쳬��ʬ���������󡦥ե��륿
 *========================================================================*/
t_filter filter__make_GGY_shift /* �쳬���������󡦥ե��륿 */
  P3 (( double, sigma ), /* ɸ���к� */
      ( double, dx    ), /* �إ��ե� */
      ( double, dy    )) /* �٥��ե� */



/*========================================================================*
 * filter__make_GGX : �����������ذ쳬��ʬ�����ե��륿
 *========================================================================*/
t_filter filter__make_GGX /* �ذ쳬���������󡦥ե��륿 */
  P1 (( double, sigma )) /* ɸ���к� */

/*========================================================================*
 * filter__make_GGY : �����������ٰ쳬��ʬ�����ե��륿
 *========================================================================*/
t_filter filter__make_GGY /* �ٰ쳬���������󡦥ե��륿 */
  P1 (( double, sigma )) /* ɸ���к� */

/*==============================================*
 * filter__make_G : ���������󡦥ե��륿
 *==============================================*/
t_filter filter__make_G /* ���������󡦥ե��륿 */
  P1 (( double, sigma )) /* ɸ���к� */



/*************************
 * Round Pixel Extension
 *************************/

/*=============================================================*
 * filter__make_round_pixel : �饦��ɡ��ԥ�����Υޥ�������
 * �����ź���� -radius ���� +radius �ǥ��������Ǥ���
 *=============================================================*/
t_filter filter__make_round_pixel /* �饦��ɡ��ԥ�����Υݥ��� */
  P1 (( long, radius )) /* Ⱦ�¤β��ǿ� */



#endif __image_filter_h
