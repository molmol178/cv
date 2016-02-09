/*
 * filename : cglib.c
 * author   : Takahiro Sugiyama
 * date     : 1993/09/26
 */



#include "cglib.h"
#include <math.h>



/*=========================*
 * cgdrawpoint : 点を打つ。
 *=========================*/
void cgdrawpoint
  _P4 (( char   *, data ),
       ( gcRec  *, gc   ),
       ( long    , x    ),
       ( long    , y    ))
{
  CGdrawpoint( data, gc, x, y );
}



/*==============================*
 * cgdrawpoints : 複数点を打つ。
 *==============================*/
void cgdrawpoints
  _P4 (( char     *, data ),
       ( gcRec    *, gc   ),
       ( long      , num  ),
       ( cg_point *, list ))
{
  long i;

  for ( i = 0; i < num; i++ )
    CGdrawpoint( data, gc, list[ i ].x, list[ i ].y );
}



/*========================*
 * cgdrawline : 線を描く。
 *========================*/
void cgdrawline
  _P6 (( char   *, data ),
       ( gcRec  *, gc   ),
       ( long    , x1   ),
       ( long    , y1   ),
       ( long    , x2   ),
       ( long    , y2   ))
{
  long step;
  long dx, dy;
  long s, i;

  if (( x1 < x2 ) == ( y1 < y2 )) step = 1; else step = -1;
  dx = abs( x2 - x1 );
  dy = abs( y2 - y1 );

  if (dx > dy)
    {
      if ( x1 > x2 )
	{
	  x1 = x2;
	  y1 = y2;
	}
      CGdrawpoint( data, gc, x1, y1 );
      s = dx / 2;
      for ( i = x1 + 1; i <= x1 + dx; i++ )
	{
	  s -= dy;
	  if ( s < 0 ) { s += dx; y1 += step; }
	  CGdrawpoint( data, gc, i, y1 );
	}
    }
  else
    {
      if ( y1 > y2 )
	{
	  y1 = y2;
	  x1 = x2;
	}
      CGdrawpoint( data, gc, x1, y1 );
      s = dy / 2;
      for ( i = y1 + 1; i <= y1 + dy; i++ )
	{
	  s -= dx;
	  if ( s < 0 ) { s += dy; x1 += step; }
	  CGdrawpoint( data, gc, x1, i );
	}
    }
}




/*=============================*
 * cgdrawlines : 複数線を描く。
 *=============================*/
void cgdrawlines
  _P4 (( char     *, data ),
       ( gcRec    *, gc   ),
       ( long      , num  ),
       ( cg_point *, list ))
{
  long i;

  for ( i = 0; i < num - 1; i++ )
    cgdrawline( data, gc, list[ i ].x, list[ i ].y,
			  list[ i + 1 ].x, list[ i + 1 ].y );
}



/*==================================*
 * cgdrawsegments : 線分を複数描く。
 *==================================*/
void cgdrawsegments
  _P4 (( char       *, data ),
       ( gcRec      *, gc   ),
       ( long        , num  ),
       ( cg_segment *, list ))
{
  long i;

  for ( i = 0; i < num; i++ )
    cgdrawline( data, gc, list[ i ].x1, list[ i ].y1,
			  list[ i ].x2, list[ i ].y2 );
}



/*===============================*
 * cgdrawrectangle : 矩形を描く。
 *===============================*/
void cgdrawrectangle
  _P6 (( char   *, data   ),
       ( gcRec  *, gc     ),
       ( long    , x      ),
       ( long    , y      ),
       ( long    , width  ),
       ( long    , height ))
{
  cgdrawbox( data, gc, x, y, x + width - 1, y + height - 1 );
}



/*====================================*
 * cgdrawrectangles : 矩形を複数描く。
 *====================================*/
void cgdrawrectangles
  _P4 (( char         *, data ),
       ( gcRec        *, gc   ),
       ( long          , num  ),
       ( cg_rectangle *, list ))
{
  long i;

  for ( i = 0; i < num; i++ )
    cgdrawrectangle( data, gc, list[ i ].x, list[ i ].y,
			       list[ i ].width, list[ i ].height );
}



/*=======================*
 * cgdrawbox : 箱を描く。
 *=======================*/
void cgdrawbox
  _P6 (( char   *, data ),
       ( gcRec  *, gc   ),
       ( long    , x1   ),
       ( long    , y1   ),
       ( long    , x2   ),
       ( long    , y2   ))
{
  cgdrawline( data, gc, x1, y1, x2, y1 );
  cgdrawline( data, gc, x2, y1, x2, y2 );
  cgdrawline( data, gc, x2, y2, x1, y2 );
  cgdrawline( data, gc, x1, y2, x1, y1 );
}



/*=============================*
 * cgdrawboxes : 箱を複数描く。
 *=============================*/
void cgdrawboxes
  _P4 (( char   *, data ),
       ( gcRec  *, gc   ),
       ( long    , num  ),
       ( cg_box *, list ))
{
  long i;

  for ( i = 0; i < num; i++ )
    cgdrawbox( data, gc, list[ i ].x1, list[ i ].y1,
			 list[ i ].x2, list[ i ].y2 );
}



/*==========================*
 * cgdrawcircle : 円を描く。
 *==========================*/
void cgdrawcircle
  _P5 (( char   *, data ),
       ( gcRec  *, gc   ),
       ( long    , cx   ),
       ( long    , cy   ),
       ( long    , r    ))
{
  long x = r;
  long y = 0;
  long s = r;

  if ( r == 0 ) { CGdrawpoint( data, gc, cx, cy ); return; }

  while( x >= y )
    {
      CGdrawpoint( data, gc, cx + x, cy + y );
      CGdrawpoint( data, gc, cx + x, cy - y );
      CGdrawpoint( data, gc, cx - x, cy + y );
      CGdrawpoint( data, gc, cx - x, cy - y );
      CGdrawpoint( data, gc, cx + y, cy + x );
      CGdrawpoint( data, gc, cx + y, cy - x );
      CGdrawpoint( data, gc, cx - y, cy + x );
      CGdrawpoint( data, gc, cx - y, cy - x );
      s = s - y * 2 - 1;
      y++;
      if ( s < 0 )
	{
	  s = s + ( x - 1 ) * 2;
	  x--;
	}
    }
}



/*===========================================*
 * cgdrawcircle2 : 中心と一点を通る円を描く。
 *===========================================*/
void cgdrawcircle2
  _P6 (( char   *, data ),
       ( gcRec  *, gc   ),
       ( long    , cx   ),
       ( long    , cy   ),
       ( long    , x1   ),
       ( long    , y1   ))
{
  long r = sqrt( ( double ) (( cx - x1 ) * ( cx - x1 ) +
			     ( cy - y1 ) * ( cy - y1 )));

  cgdrawcircle( data, gc, cx, cy, r );
}



/*=====================================*
 * cgdrawcircle3 : 二点を通る円を描く。
 *=====================================*/
void cgdrawcircle3
  _P6 (( char   *, data ),
       ( gcRec  *, gc   ),
       ( long    , x1   ),
       ( long    , y1   ),
       ( long    , x2   ),
       ( long    , y2   ))
{
  cgdrawcircle2( data, gc, ( x1 + x2 ) / 2, ( y1 + y2 ) / 2, x2, y2 );
}



/*=======================================*
 * cgdrawcircle4 : 矩形に接する円を描く。
 *=======================================*/
void cgdrawcircle4
  _P6 (( char   *, data ),
       ( gcRec  *, gc   ),
       ( long    , x1   ),
       ( long    , y1   ),
       ( long    , x2   ),
       ( long    , y2   ))
{
  cgdrawellipse4( data, gc, x1, y1, x2, y2 );
}



/*=========================================*
 * cgdrawellipse : 中心と半径の楕円を描く。
 *=========================================*/
void cgdrawellipse
  _P6 (( char   *, data ),
       ( gcRec  *, gc   ),
       ( long    , x    ),
       ( long    , y    ),
       ( long    , rx   ),
       ( long    , ry   ))
{
  long px,py;
  long nx,ny;
  long i;

  px = x + rx;
  py = y;
  for ( i = 1; i <= 360; i++ )
    {
      double a = i * 3.14159265358979323846 / 180;
      nx = x + rx * cos( a )+0.5;
      ny = y + ry * sin( a )+0.5;
      cgdrawline( data, gc, px, py, nx, ny );
      px = nx;
      py = ny;
    }
}



/*==============================================*
 * cgdrawellipse2 : 中心と一点を通る楕円を描く。
 *==============================================*/
void cgdrawellipse2
  _P6 (( char   *, data ),
       ( gcRec  *, gc   ),
       ( long    , cx   ),
       ( long    , cy   ),
       ( long    , x1   ),
       ( long    , y1   ))
{
  cgdrawellipse( data, gc, cx, cy, abs( cx - x1 ), abs( cy - y1 ) );
}



/*============================================*
 * cgdrawellipse3 : 矩形に内接する楕円を描く。
 *============================================*/
void cgdrawellipse3
  _P6 (( char   *, data ),
       ( gcRec  *, gc   ),
       ( long    , x1   ),
       ( long    , y1   ),
       ( long    , x2   ),
       ( long    , y2   ))
{
  cgdrawellipse2( data, gc, ( x1 + x2 ) / 2, ( y1 + y2 ) / 2, x2, y2 );
}



/*============================================*
 * cgdrawellipse4 : 矩形に内接する楕円を描く。
 *============================================*/
void cgdrawellipse4
  _P6 (( char   *, data ),
       ( gcRec  *, gc   ),
       ( long    , x1   ),
       ( long    , y1   ),
       ( long    , x2   ),
       ( long    , y2   ))
{
  long px,py;
  long nx,ny;
  long i;
  double x,y,rx,ry;

  x = ( x1 + x2 ) / 2.0;
  y = ( y1 + y2 ) / 2.0;
  rx = abs( x1 - x2 ) / 2.0;
  ry = abs( y1 - y2 ) / 2.0;

  px = x + rx;
  py = y;
  for ( i = 1; i <= 360; i++ )
    {
      double a = i * 3.14159265358979323846 / 180;
      nx = x + rx * cos( a )+0.5;
      ny = y + ry * sin( a )+0.5;
      cgdrawline( data, gc, px, py, nx, ny );
      px = nx;
      py = ny;
    }
}



/*===============================*
 * cgdrawpolygon : 多角形を描く。
 *===============================*/
void cgdrawpolygon
  _P4 (( char     *, data ),
       ( gcRec    *, gc   ),
       ( long      , num  ),
       ( cg_point *, list ))
{
  cgdrawlines( data, gc, num, list );
  if (( list[ 0 ].x != list[ num - 1 ].x ) ||
      ( list[ 0 ].y != list[ num - 1 ].y ))
    cgdrawline( data, gc, list[ 0 ].x, list[ 0 ].y,
			  list[ num - 1 ].x, list[ num - 1 ].y );
}



/*=============================*
 * cgfillbox : 箱を塗りつぶす。
 *=============================*/
void cgfillbox
  _P6 (( char   *, data ),
       ( gcRec  *, gc   ),
       ( long    , x1   ),
       ( long    , y1   ),
       ( long    , x2   ),
       ( long    , y2   ))
{
  long x, y;

  if ( y1 > y2 ) { long tmp = y2; y2 = y1; y1 = tmp; }
  if ( x1 > x2 ) { long tmp = x2; x2 = x1; x1 = tmp; }

  for ( y = y1; y <= y2; y++ )
    for ( x = x1; x <= x2; x++ )
      CGdrawpoint( data, gc, x, y );
}



/*================================*
 * cgfillcircle : 円を塗りつぶす。
 *================================*/
void cgfillcircle
  _P5 (( char   *, data ),
       ( gcRec  *, gc   ),
       ( long    , cx   ),
       ( long    , cy   ),
       ( long    , r    ))
{
  long ix, iy;
  long r2 = r * r;
  double dy;

  for ( iy = -r; iy <= r; iy++ )
    {
      if ( iy < 0 )
	dy = iy + 0.5;
      else
	dy = iy - 0.5;
      ix = sqrt( r2 - dy * dy )+0.5;
      cgdrawline( data, gc, cx - ix, cy + iy, cx + ix, cy + iy );
    }
}



/*=================================================*
 * cgfillcircle2 : 中心と一点を通る円を塗りつぶす。
 *=================================================*/
void cgfillcircle2
  _P6 (( char   *, data ),
       ( gcRec  *, gc   ),
       ( long    , cx   ),
       ( long    , cy   ),
       ( long    , x1   ),
       ( long    , y1   ))
{
  long r = sqrt(( double )(( cx - x1 ) * ( cx - x1 ) +
			   ( cy - y1 ) * ( cy - y1 )));

  cgfillcircle( data, gc, cx, cy, r );
}



/*=====================================*
 * cgfillcircle3 : 二点を通る円を描く。
 *=====================================*/
void cgfillcircle3
  _P6 (( char   *, data ),
       ( gcRec  *, gc   ),
       ( long    , x1   ),
       ( long    , y1   ),
       ( long    , x2   ),
       ( long    , y2   ))
{
  cgfillcircle2( data, gc, ( x1 + x2 ) / 2, ( y1 + y2 ) / 2, x2, y2 );
}



/*=======================================*
 * cgfillcircle4 : 矩形に接する円を描く。
 *=======================================*/
void cgfillcircle4
  _P6 (( char   *, data ),
       ( gcRec  *, gc   ),
       ( long    , x1   ),
       ( long    , y1   ),
       ( long    , x2   ),
       ( long    , y2   ))
{
  cgfillellipse4( data, gc, x1, y1, x2, y2 );
}



/*===============================================*
 * cgfillellipse : 中心と半径の楕円を塗りつぶす。
 *===============================================*/
void cgfillellipse
  _P6 (( char   *, data ),
       ( gcRec  *, gc   ),
       ( long    , cx   ),
       ( long    , cy   ),
       ( long    , rx   ),
       ( long    , ry   ))
{
  long ix, iy;
  long ry2 = ry * ry;
  double dy;

  for ( iy = -ry; iy <= ry; iy++ )
    {
      if ( iy < 0 ) dy = iy + 0.5; else	dy = iy - 0.5;
      ix = rx * sqrt( 1.0 - dy * dy / ry2 ) + 0.5;
      cgdrawline( data, gc, cx - ix, cy + iy, cx + ix, cy + iy );
    }
}



/*====================================================*
 * cgfillellipse2 : 中心と一点を通る楕円を塗りつぶす。
 *====================================================*/
void cgfillellipse2
  _P6 (( char   *, data ),
       ( gcRec  *, gc   ),
       ( long    , cx   ),
       ( long    , cy   ),
       ( long    , x1   ),
       ( long    , y1   ))
{
  cgfillellipse( data, gc, cx, cy, abs( cx - x1 ), abs( cy - y1 ) );
}



/*==================================================*
 * cgfillellipse3 : 矩形に内接する楕円を塗りつぶす。
 *==================================================*/
void cgfillellipse3
  _P6 (( char   *, data ),
       ( gcRec  *, gc   ),
       ( long    , x1   ),
       ( long    , y1   ),
       ( long    , x2   ),
       ( long    , y2   ))
{
  cgfillellipse2( data, gc, ( x1 + x2 ) / 2, ( y1 + y2 ) / 2, x2, y2 );
}



/*==================================================*
 * cgfillellipse4 : 矩形に内接する楕円を塗りつぶす。
 *==================================================*/
void cgfillellipse4
  _P6 (( char   *, data ),
       ( gcRec  *, gc   ),
       ( long    , x1   ),
       ( long    , y1   ),
       ( long    , x2   ),
       ( long    , y2   ))
{
  double iy;
  double ix;
  double ry2;
  double dy;
  double cx, cy, rx, ry;
  double ax, bx;
  long odd_f;
  long idx, idy;

  idx = abs( x1 - x2 );
  idy = abs( y1 - y2 );
  if (( idx == idy ) && ( idx < 7 ))
    {
      long x,y;
      static char pattern2[3][3] = {{0,1,0},{1,1,1},{0,1,0}};

      static char pattern3[4][4] = {{0,1,1,0},
				    {1,1,1,1},
				    {1,1,1,1},
				    {0,1,1,0}};

      static char pattern4[5][5] = {{0,1,1,1,0},
				    {1,1,1,1,1},
				    {1,1,1,1,1},
				    {1,1,1,1,1},
				    {0,1,1,1,0}};

      static char pattern5[6][6] = {{0,1,1,1,1,0},
				    {1,1,1,1,1,1},
				    {1,1,1,1,1,1},
				    {1,1,1,1,1,1},
				    {1,1,1,1,1,1},
				    {0,1,1,1,1,0}};

      static char pattern6[7][7] = {{0,0,1,1,1,0,0},
				    {0,1,1,1,1,1,0},
				    {1,1,1,1,1,1,1},
				    {1,1,1,1,1,1,1},
				    {1,1,1,1,1,1,1},
				    {0,1,1,1,1,1,0},
				    {0,0,1,1,1,0,0}};
      if ( x1 > x2 ) x1 = x2;
      if ( y1 > y2 ) y1 = y2;

      switch (idx) {
      case 0:
	CGdrawpoint( data, gc, x1, y1 );
	break;

      case 1:
	CGdrawpoint( data, gc, x1, y1 );
	CGdrawpoint( data, gc, x1 + 1, y1 );
	CGdrawpoint( data, gc, x1, y1 + 1 );
	CGdrawpoint( data, gc, x1 + 1, y1 + 1 );
	break;

      case 2:
	for ( y = 0; y <= 2; y++ )
	  for ( x = 0; x <= 2; x++ )
	    if ( pattern2[ y ][ x ] ) CGdrawpoint( data, gc, x1 + x, y1 + y );
	break;

      case 3:
	for ( y = 0; y <= 3; y++ )
	  for ( x = 0; x <= 3; x++ )
	    if ( pattern3[ y ][ x ] ) CGdrawpoint( data, gc, x1 + x, y1 + y );
	break;

      case 4:
	for ( y = 0; y <= 4; y++ )
	  for ( x = 0; x <= 4; x++ )
	    if ( pattern4[ y ][ x ] ) CGdrawpoint( data, gc, x1 + x, y1 + y );
	break;

      case 5:
	for ( y = 0; y <= 5; y++ )
	  for ( x = 0; x <= 5; x++ )
	    if ( pattern5[ y ][ x ] ) CGdrawpoint( data, gc, x1 + x, y1 + y );
	break;

      case 6:
	for ( y = 0; y <= 6; y++ )
	  for ( x = 0; x <= 6; x++ )
	    if ( pattern6[ y ][ x ] ) CGdrawpoint( data, gc, x1 + x, y1 + y );
	break;
      }
      return;
    }

  cx = ( double )( x1 + x2 ) / 2.0;
  cy = ( double )( y1 + y2 ) / 2.0;
  rx = abs( x1 - x2 ) / 2.0;
  ry = abs( y1 - y2 ) / 2.0;
  ry2 = ry * ry;

  for ( iy = -ry; iy <= ry; iy++ )
    {
      if ( iy < 0 ) dy = iy + 0.5; else dy = iy - 0.5;
      ix = rx * sqrt( 1.0 - dy * dy / ry2 );
      ax = ix - 0.5;
      bx = ix + 0.5;
      cgdrawline( data, gc, ( long )( cx - ax ), ( long )( cy + iy ),
			    ( long )( cx + bx ), ( long )( cy + iy ));
    }
}



static long longcomp( a, b )
     long *a, *b;
{
  return *a - *b;
}



#define DEBUG 0

/*=====================================*
 * cgfillpolygon : 多角形を塗りつぶす。
 *=====================================*/
void cgfillpolygon
  _P4 (( char     *, data ),
       ( gcRec    *, gc   ),
       ( long      , num  ),
       ( cg_point *, list ))
{
  struct P_SUB {
    int ymax;
    int ymin;
    int sx;
    int ex;
    int dx;
    int dy;
  } *cndt;
  int i,j;
  int y,ymin,ymax;
  cg_point p0,p1;
  cg_point *point;

  if ( num == 0 ) return;

  if ( num == 1 )
    {
      CGdrawpoint( data, gc, list[ 0 ].x, list[ 0 ].y );
      return;
    }

  if ( num == 2 )
    {
      cgdrawline( data, gc, list[ 0 ].x, list[ 0 ].y,
			    list[ 1 ].x, list[ 1 ].y );
      return;
    }

  point = ( cg_point * )calloc( num + 1, sizeof( cg_point ) );
  point[ 0 ] = list[ 0 ];
  for ( j = i = 1; i < num; i++ )
    {
      if (( point[ j - 1 ].x == list[ i ].x ) &&
	  ( point[ j - 1 ].y == list[ i ].y )) continue;
      point[ j++ ] = list[ i ];
    }
  if (( point[ j - 1 ].x != point[ 0 ].x ) ||
      ( point[ j - 1 ].y != point[ 0 ].y ))
    {
      point[ j++ ] = point[ 0 ];
    }
  num = j - 1;
  cndt = ( struct P_SUB * )calloc( num, sizeof( struct P_SUB ) );

  ymax = point[ 0 ].y;
  ymin = point[ 0 ].y;
  p0 = point[ 0 ];
  for ( i = 0; i < num; i++ )
    {
      p1 = point[ i + 1 ];

      if ( ymax < p1.y ) ymax = p1.y;
      if ( ymin > p1.y ) ymin = p1.y;

      cndt[ i ].dx = p0.x - p1.x;
      cndt[ i ].dy = p0.y - p1.y;

      if ( p0.y < p1.y )
	{
	  cndt[ i ].ymax = p1.y + 1;
	  cndt[ i ].ymin = p0.y;
	  cndt[ i ].sx   = p0.x;
	  cndt[ i ].ex   = p1.x;
	}
      else
	{
	  cndt[ i ].ymax = p0.y + 1;
	  cndt[ i ].ymin = p1.y;
	  cndt[ i ].sx   = p1.x;
	  cndt[ i ].ex   = p0.x;
	}
      if ( cndt[ i ].dy != 0 )
	{
	  cndt[ i ].sx *= cndt[ i ].dy;
	}
      p0 = p1;
    }
  
  for ( y = ymin; y <= ymax; y++ )
    {
      long xpoint[ 100 ];
      int pc;
	
      /* 線分の交点探索 */
      for ( pc = i = 0; i < num; i++ )
	{
	  if ( ( cndt[ i ].ymin == y ) && ( cndt[ i ].dy == 0 ) )
	    {
/*
	      xpoint[pc++] = cndt[i].sx;
	      xpoint[pc++] = cndt[i].ex;
*/
	    }
	  else
	    {
	      if ((( y == ymax ) && ( cndt[ i ].ymin <= y ) &&
				    ( y < cndt[ i ].ymax )) ||
		  (( y != ymax ) && ( cndt[ i ].ymin <= y ) &&
				    ( y < cndt[ i ].ymax - 1 )))
		{
		  if ( y == cndt[ i ].ymin )
		    xpoint[ pc ] = cndt[ i ].sx / cndt[ i ].dy;
		  else
		  if ( y == cndt[ i ].ymax )
		    xpoint[ pc ] = cndt[ i ].ex;
		  else
		    xpoint[ pc ] = ( cndt[ i ].sx + cndt[ i ].dy / 2 )
							/ cndt[ i ].dy;
		  cndt[ i ].sx += cndt[ i ].dx;
		  pc++;
		}
	    }
	}

      /* 交点のソーティング */
      qsort( xpoint, pc, sizeof( long ), longcomp );

      /* 描写 */
      for ( i = 0; i < pc; i += 2 )
	{
	  cgdrawline( data, gc, xpoint[ i ], y, xpoint[ i + 1 ], y );
	}
    }

  free( cndt );
  free( point );
}
