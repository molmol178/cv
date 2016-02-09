/*
 * filename : distance.c
 * author   : Yoshihiro Kameyama
 * date     : 1994/01/20
 * redate   :
 *
 * co-author : Takahiro Sugiyama
 * update    : 1994/01/22
 */



#include "ColorImage.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


double HSV_Distance
  _P2 (( float3 *, point1 ),
       ( float3 *, point2 ))
{
  double theta, xy_distance2, distance;
  double hue1, saturation1, value1;
  double hue2, saturation2, value2;

  hue1        = point1->at[ 0 ];
  saturation1 = point1->at[ 1 ];
  value1      = point1->at[ 2 ];

  hue2        = point2->at[ 0 ];
  saturation2 = point2->at[ 1 ];
  value2      = point2->at[ 2 ];

  theta = fabs( DEGREE_TO_RADIAN( hue1 ) - DEGREE_TO_RADIAN( hue2 ) );

  xy_distance2 = saturation1 * saturation1 + saturation2 * saturation2
			- 2 * saturation1 * saturation2 * cos( theta );

  distance = sqrt( xy_distance2 + ( value1 - value2 ) * ( value1 - value2 ));

  return distance;
}
