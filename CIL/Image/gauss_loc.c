/*
 * filename : gauss_loc.c
 * author   : Takahiro Sugiyama
 * date     : Saturday, March 19 1994
 * describe : subroutins via gaussian filter's
 */



#include "Filter.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


/*========================================================================*
 * filter__make_G_shift : �������֤��줷�����������󡦥ե��륿
 *========================================================================*/
t_filter filter__make_G_shift /* ���������󡦥ե��륿 */
  _P3 (( double, sigma ), /* ɸ���к� */
       ( double, dx    ), /* �إ��ե� */
       ( double, dy    )) /* �٥��ե� */
{
  register int x, y, n, nn;
  register t_filter filter;

  n = sigma * 4.2;
  nn = 2 * n + 1;
  filter = filter__make( Double, nn, nn, n, n );

  for ( y = -n; y <= n; y++ )
    for ( x = -n; x <= n; x++ )
      filter->data.of_double[ y ][ x ]
	= exp( -( ( y - dy ) * ( y - dy ) + ( x - dx ) * ( x - dx ) ) / ( 2.0 * sigma * sigma ) ) / ( 2 * M_PI * sigma * sigma );

  return filter;
}



/*========================================================================*
 * filter__make_GGX_shift : �������֤��줷���ذ쳬��ʬ���������󡦥ե��륿
 *========================================================================*/
t_filter filter__make_GGX_shift /* �쳬���������󡦥ե��륿 */
  _P3 (( double, sigma ), /* ɸ���к� */
       ( double, dx    ), /* �إ��ե� */
       ( double, dy    )) /* �٥��ե� */
{
  register int x, y, n, nn;
  register t_filter filter;

  n = sigma * 4.2;
  nn = 2 * n + 1;
  filter = filter__make( Double, nn, nn, n, n );

  for ( y = -n; y <= n; y++ )
    for ( x = -n; x <= n; x++ )
      filter->data.of_double[ y ][ x ]
	= - ( x - dx ) / ( 2.0 * M_PI * sigma * sigma * sigma * sigma )
	* exp( - ( ( x - dx ) * ( x - dx ) + ( y - dy ) * ( y - dy ) )
	      / ( 2.0 * sigma * sigma ));

  return filter;
}



/*========================================================================*
 * filter__make_GGY_shift : �������֤��줷���ٰ쳬��ʬ���������󡦥ե��륿
 *========================================================================*/
t_filter filter__make_GGY_shift /* �쳬���������󡦥ե��륿 */
  _P3 (( double, sigma ), /* ɸ���к� */
       ( double, dx    ), /* �إ��ե� */
       ( double, dy    )) /* �٥��ե� */
{
  register int x, y, n, nn;
  register t_filter filter;

  n = sigma * 4.2;
  nn = 2 * n + 1;
  filter = filter__make( Double, nn, nn, n, n );

  for ( y = -n; y <= n; y++ )
    for ( x = -n; x <= n; x++ )
      filter->data.of_double[ y ][ x ]
	= - ( y - dy ) / ( 2.0 * M_PI * sigma * sigma * sigma * sigma )
	* exp( - ( ( x - dx ) * ( x - dx ) + ( y - dy ) * ( y - dy ) )
	      / ( 2.0 * sigma * sigma ));

  return filter;
}



/*========================================================================*
 * filter__make_GGX : �����������ذ쳬��ʬ�����ե��륿
 *========================================================================*/
t_filter filter__make_GGX /* �ذ쳬���������󡦥ե��륿 */
  _P1 (( double, sigma )) /* ɸ���к�           */
{
  return filter__make_GGX_shift( sigma, 0.0, 0.0 );
}



/*========================================================================*
 * filter__make_GGY : �����������ٰ쳬��ʬ�����ե��륿
 *========================================================================*/
t_filter filter__make_GGY /* �ٰ쳬���������󡦥ե��륿 */
  _P1 (( double, sigma )) /* ɸ���к�           */
{
  return filter__make_GGY_shift( sigma, 0.0, 0.0 );
}



/*==============================================*
 * filter__make_G : ���������󡦥ե��륿
 *==============================================*/
t_filter filter__make_G /* ���������󡦥ե��륿 */
  _P1 (( double, sigma ))/* ɸ���к�           */
{
  return filter__make_G_shift( sigma, 0.0, 0.0 );
}
