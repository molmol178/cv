/*
 * filename : filter.c
 * author   : Takahiro Sugiyama
 * date     : Friday, April 8 1994
 * describe : type of filter
 */



#include "Image/Filter.h"



/*===================================*
 * filter__make : �ե��륿���������
 *===================================*/
t_filter filter__make /* �ե��륿�Υݥ��� */
  _P5 (( int, type  ), /* �����̻� */
       ( int, xsize ), /* �ե��륿�إ����� */
       ( int, ysize ), /* �ե��륿�٥����� */
       ( int, xspot ), /* �濴�غ�ɸ */
       ( int, yspot )) /* �濴�ٺ�ɸ */
{
  t_filter filter;
  int y, size;

  filter = typenew( t_filterRec );

  size = typesize( type );

  filter->xsize = xsize;
  filter->ysize = ysize;
  filter->xspot = xspot;
  filter->yspot = yspot;
  filter->type  = type;
  filter->data.of_any = memnew2( xsize, ysize, size );
  for ( y = 0; y < ysize; y++ )
    filter->data.of_any[ y ] = filter->data.of_any[ y ] + ( xspot * size );
  filter->data.of_any = filter->data.of_any + yspot;

  filter->top = - yspot;
  filter->bottom = ysize - yspot - 1;
  filter->left = - xspot;
  filter->right = xsize - xspot - 1;

  return filter;
}



/*===================================*
 * filter__free : �ե��륿���������
 *===================================*/
void filter__free
  _P1 (( t_filter, filter ))
{
  memfree2( filter->data.of_any );
  typefree( filter );
}



#include <stdio.h>
/*==============================================*
 * filter__print : �ե��륿�����Ƥ�ץ��Ȥ���
 *==============================================*/
void filter__print
  _P1 (( t_filter, filter ))
{
  fprintf( stderr, "filter = {\n" );
  fprintf( stderr, "   xspot = %d\n", filter->xspot );
  fprintf( stderr, "   yspot = %d\n", filter->yspot );
  fprintf( stderr, "   xsize = %d\n", filter->xsize );
  fprintf( stderr, "   ysize = %d\n", filter->ysize );
  fprintf( stderr, "   lefttop  : ( %d, %d )\n", filter->left, filter->top );
  fprintf( stderr, "   rightbot : ( %d, %d )\n", filter->right, filter->bottom );
  fprintf( stderr, "}\n" );
}
