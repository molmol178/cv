/*
 * filename : roundpix.c
 * author   : Takahiro Sugiyama
 * date     : Friday, April 8 1994
 * describe : round pixel
 */



#include "Funcs.h"
#include "Filter.h"



/*=============================================================*
 * filter__make_round_pixel : �饦��ɡ��ԥ�����Υޥ�������
 *=============================================================*/
t_filter filter__make_round_pixel /* �饦��ɡ��ԥ�����Υݥ��� */
  _P1 (( long, radius )) /* Ⱦ�¤β��ǿ� */
{
  t_filter filter;
  long x, y, n, dx;

  double dy;

  n = 2 * radius + 1;
  filter = filter__make( Long, n, n, radius, radius );

  for ( y = -radius; y <= radius; y++ )
    {
      if ( y < 0 ) dy = y + 0.5;
      else         dy = y - 0.5;
      dx = sqrt( radius * radius - dy * dy ) + 0.5;
      for ( x = -dx; x <= dx; x++ )
	filter->data.of_long[ y ][ x ] = 1;
    }

  return filter;
}
