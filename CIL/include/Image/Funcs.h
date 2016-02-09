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
 * ���ϲ����� ���귿 �β���
 * ���ϲ����� ���귿 �β���
 * �ΰ�򥳥ԡ����ޤ�
 * ���ԡ��Ǥ��ʤ��ΰ��̵�뤷�ޤ�
 * dest �Ϥ��餫�����ΰ����ݤ��Ƥ����Ʋ�����
 */
void image__copyarea P ((image dest,image src,long dest_x,long dest_y,
			 long src_x,long src_y,long width,long height));



/*
 * ���ϲ����� ���귿 ��ǻø����
 * ���ϲ����� Short2�� �� (x,y) �����η�̤���
 * th �����ͤǡ���̤������ͤ� th ��꾮������� 0 �Ȥ��ޤ���
 */
void image__sobel P ((image dest,image src,long th));



/*
 * ���ϲ����� ���귿 ��ǻø����
 * ���ϲ����� Short2�� �� (x,y) �����η�̤���
 * th �����ͤǡ���̤������ͤ� th ��꾮������� 0 �Ȥ��ޤ���
 */
void image__laplacian4 P ((image dest,image src,long th));



/*
 * ��ǥ�����ե��륿
 * ���ϲ����� ���귿 �β���
 * ���ϲ����� ���귿 �β���
 * median �������
 */
void image__median P ((image dest,image src,long median));



/*
 * ���ͽ���
 * ���ϲ����� ���귿 �β���
 * ���ϲ����� ���Ͳ���
 */
void image__thresholding
  P3 (( image , out       ),
      ( image , in        ),
      ( double, threshold ))



/*
 * ���ϲ����� Ǥ�դη� �β���
 * ���ϲ����Ϥ�PNM�ե����ޥåȤǥ����֤Ǥ������
 */
void image__any_to_pnm P ((image dst,image src));



/*
 * ���ϲ����� ���귿 ��ʣ�����
 * ���ϲ����� ���η� ��ñ�����
 */
void image__separate2 P ((image dst1,image dst2,image src));



/*
 * ���ϲ����� ���귿 �β���
 * ���ϲ����� ���η� �β���
 * ���ꤵ�줿���β�����ü�μ���ι¤�
 * ���ܤ�������ͤ�ʿ���ͤ���֤��ޤ�
 */
void image__fillmoat P ((image src,long width));



/*
 * ���ϲ����� ����ʣ�緿 �β���
 * ���ϲ����� ����ñ�췿 �β���
 * ʣ����Ǥγ�ʿ�Ѥ�׻����ޤ�
 */
void image__average P ((image dst,image src));



/*
 * ������ǻ�٤�ȿž���ޤ�
 */
void image__reverse P ((image dst,image src));



/*
 * ������ǻ�٤������Ѵ����ޤ�
 */
void image__liner P ((image dst,image src));



/*
 * ���ϲ����� ���귿 �β���
 * ���ϲ����� Long�� �β���
 * AnyType  -> Long
 * AnyType2 -> Long2
 * AnyType3 -> Long3
 */
void image__long_type P ((image dst,image src));



/*
 * ����������ե��륿
 * 
 * �������ե��륿��ܤ��ޤ�
 * ���ϲ����� UChar�� ��ǻø����
 * ���ϲ����� UChar�� ��ǻø����
 */
void image__gaussian P ((image dst,image src,double sigma));



/*
 * ���������󡦥���ǥ�����ȡ��ե��륿
 *
 * �������ե��륿��ܤ��ޤ�
 * ���ϲ����� UChar�� ��ǻø����
 * ���ϲ����� Float ���β���
 */
void image__gaussian_gradient P ((image mag,image grad_x,image grad_y,
				  image src,double sigma));
void image__gaussian_gradient_abs
  P3 (( image , mag   ),
      ( image , src   ),
      ( double, sigma ))



/*
 * �ɤ�Ĥ֤�
 */
void image__fill_in
  P2 (( image, dest ),
      ( image, src  ))



/*=========================================*
 * image__bind_data : �ǡ�����Х���ɤ���
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
 * ����˵�Υ�٥�󥰡�
 * ����original�����label�ˣ��֤����٥�󥰤��롣
 * �֤��ͤϥ�٥�ο����֤���
 *
 * ����: Bit1, Char, UChar, Short, UShort, Long
 * ����: UChar, Short, UShort, Long �Υ�٥뷿�ʻ����
 *------------------------------------------------------------------------*/
long image__N4_labelling /* ��٥���Ϸ� = output_type */
	P3 ((image, label), (image, original), (long, output_type))

long image__N4_labelling_uchar /* ��٥���Ϸ� = UChar */
	P2 ((image, label), (image, original))

long image__N4_labelling_short /* ��٥���Ϸ� = Short */
	P2 ((image, label), (image, original))

long image__N4_labelling_ushort /* ��٥���Ϸ� = UShort */
	P2 ((image, label), (image, original))

long image__N4_labelling_long /* ��٥���Ϸ� = Long */
	P2 ((image, label), (image, original))


/*------------------------------------------------------------------------*
 * ����˵�Υ�٥�󥰡�0�ϥ�٥���оݤˤʤ�ʤ���
 * ����original�����label�ˣ��֤����٥�󥰤��롣
 * �֤��ͤϥ�٥�ο����֤���
 *
 * ����: Bit1, Char, UChar, Short, UShort, Long
 * ����: UChar, Short, UShort, Long �Υ�٥뷿�ʻ����
 *------------------------------------------------------------------------*/
long image__N4_labelling_foreground /* ��٥���Ϸ� = output_type */
	P3 ((image, label), (image, original), (long, output_type))

long image__N4_labelling_foreground_uchar /* ��٥���Ϸ� = UChar */
	P2 ((image, label), (image, original))

long image__N4_labelling_foreground_short /* ��٥���Ϸ� = Short */
	P2 ((image, label), (image, original))

long image__N4_labelling_foreground_ushort /* ��٥���Ϸ� = UShort */
	P2 ((image, label), (image, original))

long image__N4_labelling_foreground_long /* ��٥���Ϸ� = Long */
	P2 ((image, label), (image, original))



/*------------------------------------------------------------------------*
 * ����˵�Υ�٥�󥰡�
 * ����original�����label�ˣ��֤����٥�󥰤��롣
 * �֤��ͤϥ�٥�ο����֤���
 *
 * ����: Bit1, Char, UChar, Short, UShort, Long
 * ����: UChar, Short, UShort, Long �Υ�٥뷿�ʻ����
 *------------------------------------------------------------------------*/
long image__N8_labelling /* ��٥���Ϸ� = output_type */
	P3 ((image, label), (image, original), (long, output_type))

long image__N8_labelling_uchar /* ��٥���Ϸ� = UChar */
	P2 ((image, label), (image, original))

long image__N8_labelling_short /* ��٥���Ϸ� = Short */
	P2 ((image, label), (image, original))

long image__N8_labelling_ushort /* ��٥���Ϸ� = UShort */
	P2 ((image, label), (image, original))

long image__N8_labelling_long /* ��٥���Ϸ� = Long */
	P2 ((image, label), (image, original))


/*------------------------------------------------------------------------*
 * ����˵�Υ�٥�󥰡�0�ϥ�٥���оݤˤʤ�ʤ���
 * ����original�����label�ˣ��֤����٥�󥰤��롣
 * �֤��ͤϥ�٥�ο����֤���
 *
 * ����: Bit1, Char, UChar, Short, UShort, Long
 * ����: UChar, Short, UShort, Long �Υ�٥뷿�ʻ����
 *------------------------------------------------------------------------*/
long image__N8_labelling_foreground /* ��٥���Ϸ� = output_type */
	P3 ((image, label), (image, original), (long, output_type))

long image__N8_labelling_foreground_uchar /* ��٥���Ϸ� = UChar */
	P2 ((image, label), (image, original))

long image__N8_labelling_foreground_short /* ��٥���Ϸ� = Short */
	P2 ((image, label), (image, original))

long image__N8_labelling_foreground_ushort /* ��٥���Ϸ� = UShort */
	P2 ((image, label), (image, original))

long image__N8_labelling_foreground_long /* ��٥���Ϸ� = Long */
	P2 ((image, label), (image, original))



/*
 * ���Ѷػ�:
 * ���ϲ����� ���귿 �Υ��饹������
 * ���ϲ����� Long�� �Υ�٥����
 * �ؿ����֤��ͤϥ�٥�κ�����
 * ��٥�ϣ��֤����դ����ޤ�
 * ���Ǥ˻��Ѥ���Ƥ��ʤ�(97/2/6)
 */
long image__labeling P ((image dst,image src));



#endif  _image_funcs_h
