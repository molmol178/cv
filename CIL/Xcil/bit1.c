/*
 * XImage for bit1
 */



#include "XImage.h"



XImage *XCreateImageFromData_Bit1
  _P6 (( Display *       , display   ),
       ( register long   , win_xsize ),
       ( register long   , win_ysize ),
       ( register bit1 **, data      ),
       ( long            , img_xsize ),
       ( long            , img_ysize ))
{
  register XImage *src;

  unsigned long black = BlackPixel( display, DefaultScreen( display ) );
  unsigned long white = WhitePixel( display, DefaultScreen( display ) );

  src = XGetImage( display, DefaultRootWindow( display ),
		   0, 0, win_xsize, win_ysize, AllPlanes, ZPixmap );

  if (( win_ysize == img_ysize ) && ( win_xsize == img_xsize ))
    {
      register int x, y;

      for ( y = 0; y < win_ysize; y++ )
	for ( x = 0; x < win_xsize; x++ )
	  if ( data[ y ][ x ] ) XPutPixel( src, x, y, black );
	  else			XPutPixel( src, x, y, white );
    }
  else
    {
      register int x, y, xx, yy, sx, sy;

      for ( yy = sy = y = 0; y < win_ysize; y++, sy += img_ysize )
	{
	  while ( sy >= win_ysize ) { yy++; sy -= win_ysize; }
	  for ( xx = sx = x = 0; x < win_xsize; x++, sx += img_xsize )
	    {
	      while ( sx >= win_xsize ) { xx++; sx -= win_xsize; }
	      if ( data[ yy ][ xx ] ) XPutPixel( src, x, y, black );
	      else		      XPutPixel( src, x, y, white );
	    }
	}
    }

  return src;
}



XImage *XCreateImageFromData_PackedBit1
  _P6 (( Display *             , display   ),
       ( register long         , win_xsize ),
       ( register long         , win_ysize ),
       ( register packedbit1 **, data      ),
       ( long                  , img_xsize ),
       ( long                  , img_ysize ))
{
  register XImage *src;
  static long bitmask[8] = {1<<7,1<<6,1<<5,1<<4,1<<3,1<<2,1<<1,1<<0};

  unsigned long black = BlackPixel(display,DefaultScreen(display));
  unsigned long white = WhitePixel(display,DefaultScreen(display));

  src = XGetImage( display, DefaultRootWindow( display ),
		   0, 0, win_xsize, win_ysize, AllPlanes, ZPixmap );

  if (( win_ysize == img_ysize ) && ( win_xsize == img_xsize ))
    {
      register int x, y;

      for ( y = 0; y < win_ysize; y++ )
	for ( x = 0; x < win_xsize; x++ )
	  if ( data[ y ][ x >> 3 ] & bitmask[ x & 7 ] )
	    XPutPixel( src, x, y, black );
	  else
	    XPutPixel( src, x, y, white );
    }
  else
    {
      register int x, y, xx, yy, sx, sy;

      for ( yy = sy = y = 0; y < win_ysize; y++, sy += img_ysize )
	{
	  while ( sy >= win_ysize ) { yy++; sy -= win_ysize; }
	  for ( xx = sx = x = 0; x < win_xsize; x++, sx += img_xsize )
	    {
	      while ( sx >= win_xsize ) { xx++; sx -= win_xsize; }
	      if ( data[ yy ][ xx >> 3 ] & bitmask[ xx & 7 ])
		XPutPixel( src, x, y, black );
	      else
		XPutPixel( src, x, y, white );
	    }
	}
    }

  return src;
}
