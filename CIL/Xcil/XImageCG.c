/*
 * filename : XImageCG.c
 * author   : Takahiro Sugiyama
 * date     : 1993/09/26
 */



#include "XImageCG.h"
#include <X11/Xlib.h>
#include <math.h>
#include <stdio.h>



XImage *XImageCreate
  _P3 (( Display *, display ),
       ( long     , xsize   ),
       ( long     , ysize   ))
{
  XImage *src;
  Window win;
  long plane;

  plane = DisplayPlanes( display, DefaultScreen( display ) );
  win = DefaultRootWindow( display );

  switch ( plane )
    {
    case 1:
      src = XGetImage( display, win, 0, 0, xsize, ysize, AllPlanes, XYPixmap );
      break;

    case 8:
      src = XGetImage( display, win, 0, 0, xsize, ysize, AllPlanes, ZPixmap );
      break;

    case 24:
      src = XGetImage( display, win, 0, 0, xsize, ysize, AllPlanes, ZPixmap );
      break;

    default:
      fprintf( stderr, "error:XImageCreate:unknow display type.\n" );
      return 0;
    }

  return src;
}



void XImageDestroy
  _P1 (( XImage *, img ))
{
  XDestroyImage( img );
}



void XImageDrawPoint
  _P4 (( XImage *     , img   ),
       ( long         , x     ),
       ( long         , y     ),
       ( unsigned long, pixel ))
{
  XPutPixel(img,x,y,pixel);
}



void XImageDrawPoints
  _P4 (( XImage *   , img  ),
       ( XImgPoint *, list ),
       ( long       , num  ),
       ( long       , mode ))
{
  long i;

  if ( mode == CoordModePrevious )
    {
      int x, y;

      x = 0;
      y = 0;
      for  ( i = 0; i < num; i++ )
	{
	  x += list[ i ].x;
	  y += list[ i ].y;
	  XPutPixel( img, x, y, list[ i ].pixel );
	}
    }
  else
    {
      for  ( i = 0; i < num; i++ )
	XPutPixel( img, list[ i ].x, list[ i ].y, list[ i ].pixel );
    }
}



void XImageDrawLine
  _P6 (( XImage *     , img   ),
       ( long         , x1    ),
       ( long         , y1    ),
       ( long         , x2    ),
       ( long         , y2    ),
       ( unsigned long, pixel ))
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
      XPutPixel( img, x1, y1, pixel );
      s = dx / 2;
      for ( i = x1 + 1; i <= x1 + dx; i++ )
	{
	  s -= dy;
	  if ( s < 0 ) { s += dx; y1 += step; }
	  XPutPixel( img, i, y1, pixel );
	}
    }
  else
    {
      if ( y1 > y2 )
	{
	  y1 = y2;
	  x1 = x2;
	}
      XPutPixel( img, x1, y1, pixel );
      s = dy / 2;
      for ( i = y1 + 1; i <= y1 + dy; i++ )
	{
	  s -= dx;
	  if ( s < 0 ) { s += dy; x1 += step; }
	  XPutPixel( img, x1, i, pixel );
	}
    }
}



void XImageDrawLines
  _P4 (( XImage *   , img  ),
       ( XImgPoint *, list ),
       ( long       , num  ),
       ( int        , mode ))
{
  long i;

  if ( mode == CoordModePrevious )
    {
      int px, py, x, y;

      x = px = list[ 0 ].x;
      y = py = list[ 0 ].y;
      for ( i = 0; i < num - 1; i++ )
	{
	  x += list[ i + 1 ].x;
	  y += list[ i + 1 ].y;
	  XImageDrawLine( img, px, py, x, y, list[ i ].pixel );
	  px = x; py = y;
	}
    }
  else
    {
      for ( i = 0; i < num - 1; i++ )
	XImageDrawLine( img, list[ i ].x, list[ i ].y,
		       list[ i + 1 ].x, list[ i + 1 ].y, list[ i ].pixel );
    }
}



void XImageDrawSegments
  _P3 (( XImage *     , img  ),
       ( XImgSegment *, list ),
       ( long         , num  ))
{
  long i;

  for ( i = 0; i < num; i++ )
    XImageDrawLine( img, list[ i ].x1, list[ i ].y1,
			 list[ i ].x2, list[ i ].y2, list[ i ].pixel );
}



void XImageDrawBox
  _P6 (( XImage *     , img   ),
       ( long         , x1    ),
       ( long         , y1    ),
       ( long         , x2    ),
       ( long         , y2    ),
       ( unsigned long, pixel ))
{
  XImageDrawLine( img, x1, y1, x2, y1, pixel );
  XImageDrawLine( img, x2, y1, x2, y2, pixel );
  XImageDrawLine( img, x2, y2, x1, y2, pixel );
  XImageDrawLine( img, x1, y2, x1, y1, pixel );
}



void XImageDrawBoxes
  _P3 (( XImage  *, img  ),
       ( XImgBox *, list ),
       ( long     , num  ))
{
  long i;

  for ( i = 0; i < num; i++ )
    {
      XImgBox *e = &list[i];
      XImageDrawBox( img, e->x1, e->y1, e->x2, e->y2, e->pixel );
    }
}



void XImageDrawRectangle
  _P6 (( XImage *     , img    ),
       ( int          , x      ),
       ( int          , y      ),
       ( unsigned int , width  ),
       ( unsigned int , height ),
       ( unsigned long, pixel  ))
{
  XImageDrawBox( img, x, y, x + width - 1, y + height - 1, pixel );
}



void XImageDrawRectangles
  _P3 (( XImage *       , img  ),
       ( XImgRectangle *, list ),
       ( long           , num  ))
{
  long i;

  for ( i = 0; i < num; i++ )
    {
      XImgRectangle *e = &list[i];
      XImageDrawRectangle( img, e->x, e->y, e->width, e->height, e->pixel );
    }
}



void XImageDrawArc
  _P8 (( XImage *     , img    ),
       ( int          , x      ),
       ( int          , y      ),
       ( unsigned int , width  ),
       ( unsigned int , height ),
       ( int          , angle1 ),
       ( int          , angle2 ),
       ( unsigned long, pixel  ))
{
  long px,py;
  long nx,ny;
  long i;
  double cx,cy,rx,ry;
  long a1, a2;

  cx = ( x + x + width  ) / 2.0;
  cy = ( y + y + height ) / 2.0;
  rx = width  / 2.0;
  ry = height / 2.0;

  a1 = angle1 / 64;
  a2 = angle2 / 64;

  px = cx + rx;
  py = cy;
  for ( i = a1; i <= a2; i++ )
    {
      double a = i * 3.14159265358979323846 / 180;
      nx = cx + rx * cos( a )+0.5;
      ny = cy + ry * sin( a )+0.5;
      XImageDrawLine( img, px, py, nx, ny, pixel );
      px = nx;
      py = ny;
    }
}



void XImageDrawArcs
  _P3 (( XImage * , img  ),
       ( XImgArc *, list ),
       ( long     , num  ))
{
  long i;

  for ( i = 0; i < num; i++ )
    {
      XImgArc *e = &list[i];
      XImageDrawArc( img, e->x, e->y, e->width, e->height,
		     e->angle1, e->angle2, e->pixel );
    }
}



void XImageFillRectangle
  _P6 (( XImage *     , img    ),
       ( int          , x      ),
       ( int          , y      ),
       ( unsigned int , width  ),
       ( unsigned int , height ),
       ( unsigned long, pixel  ))
{
  long sx, sy;

  for ( sy = 0; sy < height; sy++ )
    for ( sx = 0; sx < width; sx++ )
      XPutPixel( img, x + sx, y + sy, pixel );
}



void XImageFillRectangles
  _P3 (( XImage *       , img  ),
       ( XImgRectangle *, list ),
       ( long           , num  ))
{
  long i;

  for ( i = 0; i < num; i++ )
    {
      XImgRectangle *e = &list[i];
      XImageFillRectangle( img, e->x, e->y, e->width, e->height, e->pixel );
    }
}



void XImageFillArc
  _P8 (( XImage *     , img    ),
       ( int          , x      ),
       ( int          , y      ),
       ( unsigned int , width  ),
       ( unsigned int , height ),
       ( int          , angle1 ),
       ( int          , angle2 ),
       ( unsigned long, pixel  ))
{
  double iy;
  double ix;
  double ry2;
  double dy;
  double cx, cy, rx, ry;
  double ax, bx;

  cx = ( double )( x + x + width  ) / 2.0;
  cy = ( double )( y + y + height ) / 2.0;
  rx = width  / 2.0;
  ry = height / 2.0;
  ry2 = ry * ry;

  for ( iy = -ry; iy <= ry; iy++ )
    {
      if ( iy < 0 ) dy = iy + 0.5; else dy = iy - 0.5;
      ix = rx * sqrt( 1.0 - dy * dy / ry2 );
      ax = ix - 0.5;
      bx = ix + 0.5;
      XImageDrawLine( img, ( long )( cx - ax ), ( long )( cy + iy ),
			   ( long )( cx + bx ), ( long )( cy + iy ), pixel );
    }
}



void XImageFillArcs
  _P3 (( XImage * , img  ),
       ( XImgArc *, list ),
       ( long     , num  ))
{
  long i;

  for ( i = 0; i < num; i++ )
    {
      XImgArc *e = &list[i];
      XImageFillArc( img, e->x, e->y, e->width, e->height,
		    e->angle1, e->angle2, e->pixel );
    }
}



static long longcomp( a, b )
     long *a, *b;
{
  return *a - *b;
}



#define DEBUG 0

void XImageFillPolygon
  _P6 (( XImage *     , img   ),
       ( XImgPoint *  , list  ),
       ( long         , num   ),
       ( long         , shape ),
       ( long         , mode  ),
       ( unsigned long, pixel ))
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
  XImgPoint p0,p1;
  XImgPoint *point;

  if ( num == 0 ) return;

  if ( num == 1 )
    {
      XPutPixel( img, list[ 0 ].x, list[ 0 ].y, pixel );
      return;
    }

  if ( num == 2 )
    {
      XImageDrawLine( img, list[ 0 ].x, list[ 0 ].y,
			   list[ 1 ].x, list[ 1 ].y, pixel );
      return;
    }

  point = ( XImgPoint * )calloc( num + 1, sizeof( XImgPoint ) );
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
	  XImageDrawLine( img, xpoint[ i ], y, xpoint[ i + 1 ], y, pixel );
	}
    }

  free( cndt );
  free( point );
}
