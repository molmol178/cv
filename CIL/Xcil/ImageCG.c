/*
 * filename : ImageCG.c
 * author   : Takahiro Sugiyama
 * date     : 1993/09/26
 */



#include "ImageCG.h"
#include <math.h>
#include <stdio.h>
#include <X11/Xlib.h>



#define ImagePutPixel( type, img, x, y, pixel ) \
      (( type ** )img->data)[ y ][ x ] = *(( type * )(pixel))

#define ImagePutPixelImm( type, img, x, y, pixel ) \
      (( type ** )img->data)[ y ][ x ] = ( type )(pixel)

void ImageDrawPoint_None
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
}

void ImageDrawPoint_PackedBit1
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  static long mask[8] = { 1<<7, 1<<6, 1<<5, 1<<4, 1<<3, 1<<2, 1<<1, 1<<0 };

  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  if ( pixel )
    (( uchar ** )img->data)[ y ][ x / 8 ] |= mask[ x % 8 ];
}

void ImageDrawPoint_Bit1
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixelImm( bit1, img, x, y, pixel );
}

void ImageDrawPoint_Bit4
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixelImm( bit4, img, x, y, pixel );
}

void ImageDrawPoint_Char
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixelImm( char, img, x, y, pixel );
}

void ImageDrawPoint_Short
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixelImm( short, img, x, y, pixel );
}

void ImageDrawPoint_Int
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixelImm( int, img, x, y, pixel );
}

void ImageDrawPoint_Long
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixelImm( long, img, x, y, pixel );
}

void ImageDrawPoint_UChar
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixelImm( uchar, img, x, y, pixel );
}

void ImageDrawPoint_UShort
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixelImm( ushort, img, x, y, pixel );
}

void ImageDrawPoint_UInt
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixelImm( uint, img, x, y, pixel );
}

void ImageDrawPoint_ULong
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixelImm( ulong, img, x, y, pixel );
}

void ImageDrawPoint_Float
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixel( float, img, x, y, pixel );
}

void ImageDrawPoint_Double
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixel( double, img, x, y, pixel );
}

void ImageDrawPoint_Char2
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixel( char2, img, x, y, pixel );
}

void ImageDrawPoint_Short2
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixel( short2, img, x, y, pixel );
}

void ImageDrawPoint_Int2
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixel( int2, img, x, y, pixel );
}

void ImageDrawPoint_Long2
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixel( long2, img, x, y, pixel );
}

void ImageDrawPoint_UChar2
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixel( uchar2, img, x, y, pixel );
}

void ImageDrawPoint_UShort2
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixel( ushort2, img, x, y, pixel );
}

void ImageDrawPoint_UInt2
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixel( uint2, img, x, y, pixel );
}

void ImageDrawPoint_ULong2
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixel( ulong2, img, x, y, pixel );
}

void ImageDrawPoint_Float2
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixel( float2, img, x, y, pixel );
}

void ImageDrawPoint_Double2
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixel( double2, img, x, y, pixel );
}

void ImageDrawPoint_Char3
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixel( char3, img, x, y, pixel );
}

void ImageDrawPoint_Short3
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixel( short3, img, x, y, pixel );
}

void ImageDrawPoint_Int3
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixel( int3, img, x, y, pixel );
}

void ImageDrawPoint_Long3
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixel( long3, img, x, y, pixel );
}

void ImageDrawPoint_UChar3
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixel( uchar3, img, x, y, pixel );
}

void ImageDrawPoint_UShort3
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixel( ushort3, img, x, y, pixel );
}

void ImageDrawPoint_UInt3
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixel( uint3, img, x, y, pixel );
}

void ImageDrawPoint_ULong3
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixel( ulong3, img, x, y, pixel );
}

void ImageDrawPoint_Float3
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixel( float3, img, x, y, pixel );
}

void ImageDrawPoint_Double3
  _P4 (( image , img   ),
       ( long  , x     ),
       ( long  , y     ),
       ( char *, pixel ))
{
  if ( x < 0 || y < 0 ) return;
  if ( x >= img->xsize || y >= img->ysize ) return;

  ImagePutPixel( double3, img, x, y, pixel );
}



ImageDrawPoint_type ImageDrawPointList[] = {
 ImageDrawPoint_None,
 ImageDrawPoint_PackedBit1,
 ImageDrawPoint_Bit1,
 ImageDrawPoint_Bit4,
 ImageDrawPoint_Char,
 ImageDrawPoint_Short,
 ImageDrawPoint_Int,
 ImageDrawPoint_Long,
 ImageDrawPoint_UChar,
 ImageDrawPoint_UShort,
 ImageDrawPoint_UInt,
 ImageDrawPoint_ULong,
 ImageDrawPoint_Float,
 ImageDrawPoint_Double,
 ImageDrawPoint_Char2,
 ImageDrawPoint_Short2,
 ImageDrawPoint_Int2,
 ImageDrawPoint_Long2,
 ImageDrawPoint_UChar2,
 ImageDrawPoint_UShort2,
 ImageDrawPoint_UInt2,
 ImageDrawPoint_ULong2,
 ImageDrawPoint_Float2,
 ImageDrawPoint_Double2,
 ImageDrawPoint_Char3,
 ImageDrawPoint_Short3,
 ImageDrawPoint_Int3,
 ImageDrawPoint_Long3,
 ImageDrawPoint_UChar3,
 ImageDrawPoint_UShort3,
 ImageDrawPoint_UInt3,
 ImageDrawPoint_ULong3,
 ImageDrawPoint_Float3,
 ImageDrawPoint_Double3
};



void ImageDrawPoints
  _P4 (( image     , img  ),
       ( ImgPoint *, list ),
       ( long      , num  ),
       ( long      , mode ))
{
  long i;
  register ImageDrawPoint_type putpixel = ImageDrawPointList[img->type];

  if ( mode == CoordModePrevious )
    {
      int x, y;

      x = 0;
      y = 0;
      for  ( i = 0; i < num; i++ )
	{
	  x += list[ i ].x;
	  y += list[ i ].y;
	  putpixel( img, x, y, list[ i ].pixel );
	}
    }
  else
    {
      for  ( i = 0; i < num; i++ )
	putpixel( img, list[ i ].x, list[ i ].y, list[ i ].pixel );
    }
}



void ImageDrawLine
  _P6 (( image , img   ),
       ( long  , x1    ),
       ( long  , y1    ),
       ( long  , x2    ),
       ( long  , y2    ),
       ( char *, pixel ))
{
  long step;
  long dx, dy;
  long s, i;
  register ImageDrawPoint_type putpixel = ImageDrawPointList[img->type];

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
      putpixel( img, x1, y1, pixel );
      s = dx / 2;
      for ( i = x1 + 1; i <= x1 + dx; i++ )
	{
	  s -= dy;
	  if ( s < 0 ) { s += dx; y1 += step; }
	  putpixel( img, i, y1, pixel );
	}
    }
  else
    {
      if ( y1 > y2 )
	{
	  y1 = y2;
	  x1 = x2;
	}
      putpixel( img, x1, y1, pixel );
      s = dy / 2;
      for ( i = y1 + 1; i <= y1 + dy; i++ )
	{
	  s -= dx;
	  if ( s < 0 ) { s += dy; x1 += step; }
	  putpixel( img, x1, i, pixel );
	}
    }
}



void ImageDrawLines
  _P4 (( image     , img  ),
       ( ImgPoint *, list ),
       ( long      , num  ),
       ( int       , mode ))
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
	  ImageDrawLine( img, px, py, x, y, list[ i ].pixel );
	  px = x; py = y;
	}
    }
  else
    {
      for ( i = 0; i < num - 1; i++ )
	ImageDrawLine( img, list[ i ].x, list[ i ].y,
		       list[ i + 1 ].x, list[ i + 1 ].y, list[ i ].pixel );
    }
}



void ImageDrawSegments
  _P3 (( image       , img  ),
       ( ImgSegment *, list ),
       ( long        , num  ))
{
  long i;

  for ( i = 0; i < num; i++ )
    ImageDrawLine( img, list[ i ].x1, list[ i ].y1,
			 list[ i ].x2, list[ i ].y2, list[ i ].pixel );
}



void ImageDrawBox
  _P6 (( image , img   ),
       ( long  , x1    ),
       ( long  , y1    ),
       ( long  , x2    ),
       ( long  , y2    ),
       ( char *, pixel ))
{
  ImageDrawLine( img, x1, y1, x2, y1, pixel );
  ImageDrawLine( img, x2, y1, x2, y2, pixel );
  ImageDrawLine( img, x2, y2, x1, y2, pixel );
  ImageDrawLine( img, x1, y2, x1, y1, pixel );
}



void ImageDrawBoxes
  _P3 (( image   , img  ),
       ( ImgBox *, list ),
       ( long    , num  ))
{
  long i;

  for ( i = 0; i < num; i++ )
    {
      ImgBox *e = &list[i];
      ImageDrawBox( img, e->x1, e->y1, e->x2, e->y2, e->pixel );
    }
}



void ImageDrawRectangle
  _P6 (( image       , img    ),
       ( int         , x      ),
       ( int         , y      ),
       ( unsigned int, width  ),
       ( unsigned int, height ),
       ( char *      , pixel  ))
{
  ImageDrawBox( img, x, y, x + width - 1, y + height - 1, pixel );
}



void ImageDrawRectangles
  _P3 (( image         , img  ),
       ( ImgRectangle *, list ),
       ( long          , num  ))
{
  long i;

  for ( i = 0; i < num; i++ )
    {
      ImgRectangle *e = &list[i];
      ImageDrawRectangle( img, e->x, e->y, e->width, e->height, e->pixel );
    }
}



void ImageDrawArc
  _P8 (( image       , img    ),
       ( int         , x      ),
       ( int         , y      ),
       ( unsigned int, width  ),
       ( unsigned int, height ),
       ( int         , angle1 ),
       ( int         , angle2 ),
       ( char *      , pixel  ))
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
      ImageDrawLine( img, px, py, nx, ny, pixel );
      px = nx;
      py = ny;
    }
}



void ImageDrawArcs
  _P3 (( image   , img  ),
       ( ImgArc *, list ),
       ( long    , num  ))
{
  long i;

  for ( i = 0; i < num; i++ )
    {
      ImgArc *e = &list[i];
      ImageDrawArc( img, e->x, e->y, e->width, e->height,
		     e->angle1, e->angle2, e->pixel );
    }
}



void ImageFillRectangle
  _P6 (( image       , img    ),
       ( int         , x      ),
       ( int         , y      ),
       ( unsigned int, width  ),
       ( unsigned int, height ),
       ( char *      , pixel  ))
{
  long sx, sy;
  register ImageDrawPoint_type putpixel = ImageDrawPointList[img->type];

  for ( sy = 0; sy < height; sy++ )
    for ( sx = 0; sx < width; sx++ )
      putpixel( img, x + sx, y + sy, pixel );
}



void ImageFillRectangles
  _P3 (( image         , img  ),
       ( ImgRectangle *, list ),
       ( long          , num  ))
{
  long i;

  for ( i = 0; i < num; i++ )
    {
      ImgRectangle *e = &list[i];
      ImageFillRectangle( img, e->x, e->y, e->width, e->height, e->pixel );
    }
}



void ImageFillArc
  _P8 (( image       , img    ),
       ( int         , x      ),
       ( int         , y      ),
       ( unsigned int, width  ),
       ( unsigned int, height ),
       ( int         , angle1 ),
       ( int         , angle2 ),
       ( char *      , pixel  ))
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
      ImageDrawLine( img, ( long )( cx - ax ), ( long )( cy + iy ),
			   ( long )( cx + bx ), ( long )( cy + iy ), pixel );
    }
}



void ImageFillArcs
  _P3 (( image   , img  ),
       ( ImgArc *, list ),
       ( long    , num  ))
{
  long i;

  for ( i = 0; i < num; i++ )
    {
      ImgArc *e = &list[i];
      ImageFillArc( img, e->x, e->y, e->width, e->height,
		    e->angle1, e->angle2, e->pixel );
    }
}



static long longcomp( a, b )
     long *a, *b;
{
  return *a - *b;
}



#define DEBUG 0

void ImageFillPolygon
  _P6 (( image     , img   ),
       ( ImgPoint *, list  ),
       ( long      , num   ),
       ( long      , shape ),
       ( long      , mode  ),
       ( char *    , pixel ))
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
  ImgPoint p0,p1;
  ImgPoint *point;
  register ImageDrawPoint_type putpixel = ImageDrawPointList[img->type];

  if ( num == 0 ) return;

  if ( num == 1 )
    {
      putpixel( img, list[ 0 ].x, list[ 0 ].y, pixel );
      return;
    }

  if ( num == 2 )
    {
      ImageDrawLine( img, list[ 0 ].x, list[ 0 ].y,
			   list[ 1 ].x, list[ 1 ].y, pixel );
      return;
    }

  point = ( ImgPoint * )calloc( num + 1, sizeof( ImgPoint ) );
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
	  if ( ( cndt[ i ].ymin != y ) || ( cndt[ i ].dy != 0 ) )
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
	  register int x;
	  for ( x = xpoint[ i ]; x <= xpoint[ i + 1 ]; x++ )
	    putpixel( img, x, y, pixel );
	}
    }

  free( cndt );
  free( point );
}


/*------------------------------------------------------------------------*
 * 指定した位置に点画像を表示する。
 *------------------------------------------------------------------------*/
void ImageDrawPointImage
  _P6 (( image , img    ),
       ( int   , ox     ),
       ( int   , oy     ),
       ( image , points ),
       ( char *, pos    ),
       ( char *, pixel  ))
{
  register int x, y, xx, yy;
  register ImageDrawPoint_type putpixel = ImageDrawPointList[img->type];
  int tx, ty;
  int px, py;
  int xsize, ysize;

  xsize = __XSIZE(img);
  ysize = __YSIZE(img);

  tx = __XSIZE(points);
  ty = __YSIZE(points);

  px = ox - tx/2;
  py = oy - ty/2;

  if (pos)
    {
      if (pos[0] == 'b' || pos[1] == 'b') py = oy - ty;
      if (pos[0] == 't' || pos[1] == 't') py = oy;
      if (pos[0] == 'l' || pos[1] == 'l') px = ox;
      if (pos[0] == 'r' || pos[1] == 'r') px = ox - tx;
    }

  for (y = 0; y < ty; y++)
    {
      yy = py + y;
      for (x = 0; x < tx; x++)
	{
	  xx = px + x;
	  if (__PIXEL(points, x, y, bit1))
	    putpixel(img, xx, yy, pixel);
	}
    }
}


/*------------------------------------------------------------------------*
 * 画像を左に９０度回転する。
 *------------------------------------------------------------------------*/
void ImageRotateLeft
  _P1 ((image, self))
{
  register int x, y;
  image left;

  left = Image.createMake("left", self->type, self->ysize, self->xsize);

  switch (self->type)
    {
    case Char: case UChar: case Bit1:
      {
	for (y = 0; y < self->ysize; y++)
	  for (x = 0; x < self->xsize; x++)
	    left->data[self->xsize-x-1][y] = self->data[y][x];
      } break;
    case UChar3: case Char3:
      {
	uchar3 **src = (uchar3**)self->data;
	uchar3 **dst = (uchar3**)left->data;
	for (y = 0; y < self->ysize; y++)
	  for (x = 0; x < self->xsize; x++)
	    dst[self->xsize-x-1][y] = src[y][x];
      } break;
    case Short: case UShort:
      {
	short **src = (short**)self->data;
	short **dst = (short**)left->data;
	for (y = 0; y < self->ysize; y++)
	  for (x = 0; x < self->xsize; x++)
	    dst[self->xsize-x-1][y] = src[y][x];
      } break;
    case Long: case ULong:
      {
	long **src = (long**)self->data;
	long **dst = (long**)left->data;
	for (y = 0; y < self->ysize; y++)
	  for (x = 0; x < self->xsize; x++)
	    dst[self->xsize-x-1][y] = src[y][x];
      } break;
    }

  Image.copy(self, left);
  Image.destroy(left);
}
