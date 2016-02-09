/*
 * XImage for uchar
 */



#include "XImage.h"



XImage *XCreateImageFromData_1Plane_UChar
  _P6 (( Display *        , display   ),
       ( register long    , win_xsize ),
       ( register long    , win_ysize ),
       ( register uchar **, org       ),
       ( long             , img_xsize ),
       ( long             , img_ysize ))
{
  register XImage *src;
  register int x, y, xx, yy, sx, sy;
  register long ps;

  unsigned long black = BlackPixel( display, DefaultScreen( display ) );
  unsigned long white = WhitePixel( display, DefaultScreen( display ) );

  long **err = typenew2( win_xsize + 2, 2, long );
  register long *now  = err[ 1 ] + 1;
  register long *down = err[ 0 ] + 1;
  register uchar *error_lut90 = __XEPT_90;
  register uchar *error_lut45 = __XEPT_45;

  src = XGetImage( display, DefaultRootWindow( display ),
		   0, 0, win_xsize, win_ysize, AllPlanes, ZPixmap );

  for ( yy = sy = y = 0; y < win_ysize; y++, sy += img_ysize )
    {
      while ( sy >= win_ysize ) { yy++; sy -= win_ysize; }

      { long *tmp = now; now = down; down = tmp; }
      ps = down[ -1 ] = down[ 0 ] = 0;

      for ( xx = sx = x = 0; x < win_xsize; x++, sx += img_xsize )
	{
	  while ( sx >= win_xsize ) { xx++; sx -= win_xsize; }

	  ps += org[ yy ][ xx ] + now[ x ];
	  if ( ps >= 255 ) { XPutPixel( src, x, y, white ); ps -= 255; }
	  else             { XPutPixel( src, x, y, black ); }

	  down[ x + 1 ]  = error_lut45[ ps ];
	  down[ x - 1 ] += error_lut45[ ps ];
	  ps = error_lut90[ ps ];
	  down[ x ] += ps;
	}
    }
  typefree2( err );

  return src;
}



void __XDrawImageWithLUT_UChar
  _P7 (( register XImage *, src       ),
       ( register long    , win_xsize ),
       ( register long    , win_ysize ),
       ( register uchar **, data      ),
       ( long             , img_xsize ),
       ( long             , img_ysize ),
       ( register uchar * , lut       ))
{
  register int x, y, xx, yy, sx, sy;

  for ( yy = sy = y = 0; y < win_ysize; y++, sy += img_ysize )
    {
      while ( sy >= win_ysize ) { yy++; sy -= win_ysize; }
      for ( xx = sx = x = 0; x < win_xsize; x++, sx += img_xsize )
	{
	  while ( sx >= win_xsize ) { xx++; sx -= win_xsize; }
	  XPutPixel( src, x, y, lut[ data[ yy ][ xx ] ] );
	}
    }
}



XImage *XCreateImageFromData_8Planes_PseudoColor_UChar
  _P6 (( Display *, display   ),
       ( long     , win_xsize ),
       ( long     , win_ysize ),
       ( uchar ** , org       ),
       ( long     , img_xsize ),
       ( long     , img_ysize ))
{
  XImage *src;

  src = XGetImage( display, DefaultRootWindow( display ),
		   0, 0, win_xsize, win_ysize, AllPlanes, ZPixmap );

  __XDrawImageWithLUT_UChar
	( src, win_xsize, win_ysize,
	  org, img_xsize, img_ysize, XLUT_PC_UChar );

  return src;
}



XImage *XCreateImageFromData_8Planes_GrayScale_UChar
  _P6 (( Display *, display   ),
       ( long     , win_xsize ),
       ( long     , win_ysize ),
       ( uchar ** , org       ),
       ( long     , img_xsize ),
       ( long     , img_ysize ))
{
  XImage *src;

  src = XGetImage( display, DefaultRootWindow( display ),
		   0, 0, win_xsize, win_ysize, AllPlanes, ZPixmap );

  __XDrawImageWithLUT_UChar
    ( src, win_xsize, win_ysize,
      org, img_xsize, img_ysize, XLUT_GS_UChar );

  return src;
}



XImage *XCreateImageFromData_8Planes_StaticGray_UChar
  _P6 (( Display *, display   ),
       ( long     , win_xsize ),
       ( long     , win_ysize ),
       ( uchar ** , org       ),
       ( long     , img_xsize ),
       ( long     , img_ysize ))
{
  XImage *src;

  src = XGetImage( display, DefaultRootWindow( display ),
		   0, 0, win_xsize, win_ysize, AllPlanes, ZPixmap );

  __XDrawImageWithLUT_UChar
    ( src, win_xsize, win_ysize,
      org, img_xsize, img_ysize, XLUT_SG_UChar );

  return src;
}



XImage *XCreateImageFromData_24Planes_UChar
  _P6 (( Display *         , display   ),
       ( register long     , win_xsize ),
       ( register long     , win_ysize ),
       ( register uchar ** , org       ),
       ( long              , img_xsize ),
       ( long              , img_ysize ))
{
  register XImage *src;
  register int x, y, xx, yy, sx, sy;

  src = XGetImage( display, DefaultRootWindow( display ),
		   0, 0, win_xsize, win_ysize, AllPlanes, ZPixmap );

  for ( yy = sy = y = 0; y < win_ysize; y++, sy += img_ysize )
    {
      while ( sy >= win_ysize ) { yy++; sy -= win_ysize; }
      for ( xx = sx = x = 0; x < win_xsize; x++, sx += img_xsize )
	{
	  register unsigned long p, pixel;

	  while ( sx >= win_xsize ) { xx++; sx -= win_xsize; }
	  p = org[ yy ][ xx ];
	  pixel = (( (p>>R_LowBit) << R_Shift ) | 
		   ( (p>>G_LowBit) << G_Shift ) | 
		   ( (p>>B_LowBit) << B_Shift ));
	  //pixel = ( p ) | ( p << 8 ) | ( p << 16 );
	  XPutPixel( src, x, y, pixel );
	}
    }

  return src;
}



XImage *XCreateImageFromData_16Planes_UChar
  _P6 (( Display *         , display   ),
       ( register long     , win_xsize ),
       ( register long     , win_ysize ),
       ( register uchar ** , org       ),
       ( long              , img_xsize ),
       ( long              , img_ysize ))
{
  register XImage *src;
  register int x, y, xx, yy, sx, sy;

  src = XGetImage( display, DefaultRootWindow( display ),
		   0, 0, win_xsize, win_ysize, AllPlanes, ZPixmap );

  for ( yy = sy = y = 0; y < win_ysize; y++, sy += img_ysize )
    {
      while ( sy >= win_ysize ) { yy++; sy -= win_ysize; }
      for ( xx = sx = x = 0; x < win_xsize; x++, sx += img_xsize )
	{
	  register unsigned long pixel;
	  register unsigned long p, r, g, b;

	  while ( sx >= win_xsize ) { xx++; sx -= win_xsize; }
	  p = org[ yy ][ xx ];
	  pixel = (( (p>>R_LowBit) << R_Shift ) | 
		   ( (p>>G_LowBit) << G_Shift ) | 
		   ( (p>>B_LowBit) << B_Shift ));
	  /*
	    r = R_MAX_16 * p / 255;
	    g = G_MAX_16 * p / 255;
	    b = B_MAX_16 * p / 255;
	    pixel = (r << R_SHIFT_16) | (g << G_SHIFT_16) | (b << B_SHIFT_16);
	  */

	  XPutPixel( src, x, y, pixel );
	}
    }

  return src;
}



XImage *XCreateImageFromData_15Planes_UChar
  _P6 (( Display *         , display   ),
       ( register long     , win_xsize ),
       ( register long     , win_ysize ),
       ( register uchar ** , org       ),
       ( long              , img_xsize ),
       ( long              , img_ysize ))
{
  register XImage *src;
  register int x, y, xx, yy, sx, sy;

  src = XGetImage( display, DefaultRootWindow( display ),
		   0, 0, win_xsize, win_ysize, AllPlanes, ZPixmap );

  for ( yy = sy = y = 0; y < win_ysize; y++, sy += img_ysize )
    {
      while ( sy >= win_ysize ) { yy++; sy -= win_ysize; }
      for ( xx = sx = x = 0; x < win_xsize; x++, sx += img_xsize )
	{
	  register unsigned long p, pixel;

	  while ( sx >= win_xsize ) { xx++; sx -= win_xsize; }
	  pixel = (( (p>>R_LowBit) << R_Shift ) | 
		   ( (p>>G_LowBit) << G_Shift ) | 
		   ( (p>>B_LowBit) << B_Shift ));
	  /* p = org[ yy ][ xx ]>>3;
	     pixel = ( p ) | ( p << 5 ) | ( p << 10 ); */
	  XPutPixel( src, x, y, pixel );
	}
    }

  return src;
}
