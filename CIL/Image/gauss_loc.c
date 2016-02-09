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
 * filter__make_G_shift : 少し位置ずれしたガウシアン・フィルタ
 *========================================================================*/
t_filter filter__make_G_shift /* ガウシアン・フィルタ */
  _P3 (( double, sigma ), /* 標準偏差 */
       ( double, dx    ), /* Ｘシフト */
       ( double, dy    )) /* Ｙシフト */
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
 * filter__make_GGX_shift : 少し位置ずれしたＸ一階微分ガウシアン・フィルタ
 *========================================================================*/
t_filter filter__make_GGX_shift /* 一階ガウシアン・フィルタ */
  _P3 (( double, sigma ), /* 標準偏差 */
       ( double, dx    ), /* Ｘシフト */
       ( double, dy    )) /* Ｙシフト */
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
 * filter__make_GGY_shift : 少し位置ずれしたＹ一階微分ガウシアン・フィルタ
 *========================================================================*/
t_filter filter__make_GGY_shift /* 一階ガウシアン・フィルタ */
  _P3 (( double, sigma ), /* 標準偏差 */
       ( double, dx    ), /* Ｘシフト */
       ( double, dy    )) /* Ｙシフト */
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
 * filter__make_GGX : ガウシアンをＸ一階微分したフィルタ
 *========================================================================*/
t_filter filter__make_GGX /* Ｘ一階ガウシアン・フィルタ */
  _P1 (( double, sigma )) /* 標準偏差           */
{
  return filter__make_GGX_shift( sigma, 0.0, 0.0 );
}



/*========================================================================*
 * filter__make_GGY : ガウシアンをＹ一階微分したフィルタ
 *========================================================================*/
t_filter filter__make_GGY /* Ｙ一階ガウシアン・フィルタ */
  _P1 (( double, sigma )) /* 標準偏差           */
{
  return filter__make_GGY_shift( sigma, 0.0, 0.0 );
}



/*==============================================*
 * filter__make_G : ガウシアン・フィルタ
 *==============================================*/
t_filter filter__make_G /* ガウシアン・フィルタ */
  _P1 (( double, sigma ))/* 標準偏差           */
{
  return filter__make_G_shift( sigma, 0.0, 0.0 );
}
