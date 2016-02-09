/*
 * filename : scaling.h
 * author   : Takahiro Sugiyama
 * date     : Friday, May 27 1994
 * describe : scaling program header file
 */



#ifndef __scaling_h
#define __scaling_h



#include "Image.h"



/*=========================================================*
 * image__scaling_gaussian : ������ʬ�ۤ��ꤷ�ꥵ��������
 *=========================================================*/
void image__scaling_gaussian
  P5 (( image , output ),
      ( image , input  ),
      ( long  , xsize  ),
      ( long  , ysize  ),
      ( double, sigma ))



/*==================================================================*
 * image__scaling_weight_average : �ٽ�ʿ����֤��Ѥ��ƥꥵ��������
 *==================================================================*/
void image__scaling_weight_average
  P4 (( image, output    ),
      ( image, input     ),
      ( long , new_xsize ),
      ( long , new_ysize ))



/*================================================================*
 * image__scaling_liner_complement : ������֤��Ѥ��ƥꥵ��������
 *================================================================*/
void image__scaling_liner_complement
  P4 (( image, output    ),
      ( image, input     ),
      ( long , new_xsize ),
      ( long , new_ysize ))



/*================================================================*
 * image__scaling_arithmetic : ���ꥹ��ƥ��å����
 *================================================================*/
static void image__scaling_arithmetic
  P4 (( image, output    ),
      ( image, input     ),
      ( long , new_xsize ),
      ( long , new_ysize ))



#endif /* __scaling_h */
