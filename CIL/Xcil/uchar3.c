/*
 * XImage for uchar3
 */



#include "XImage.h"



#define RGB_TO_Y( r, g, b ) (( 30 * (long)(r) + 59 * (long)(g) + 11 * (long)(b) ) / 100)



XImage *XCreateImageFromData_1Plane_UChar3
  _P6 (( Display *         , display   ),
       ( register long     , win_xsize ),
       ( register long     , win_ysize ),
       ( register uchar3 **, data      ),
       ( long              , img_xsize ),
       ( long              , img_ysize ))
{
  register XImage *src;
  register int x, y, sx, sy, xx, yy;
  register uchar *p;

  unsigned long black = BlackPixel( display, DefaultScreen( display ) );
  unsigned long white = WhitePixel( display, DefaultScreen( display ) );

  long **err = typenew2( win_xsize + 2, 2, long );
  register long *now  = err[ 1 ] + 1;
  register long *down = err[ 0 ] + 1;
  register long ps;

  register uchar *error_lut90 = __XEPT_90;
  register uchar *error_lut45 = __XEPT_45;

  src = XGetImage( display, DefaultRootWindow( display ),
		   0, 0, win_xsize, win_ysize, AllPlanes, ZPixmap );

  for ( sy = 0, y = 0, yy = 0; y < win_ysize; y++, sy += img_ysize )
    {
      while ( sy >= win_ysize ) { yy++; sy -= win_ysize; }

      { long *tmp = now; now = down; down = tmp; }
      ps = down[ -1 ] = down[ 0 ] = 0;

      for ( sx = 0, x = 0, xx = 0; x < win_xsize; x++, sx += img_xsize )
	{
	  while ( sx >= win_xsize ) { xx++; sx -= win_xsize; }

	  p = data[ yy ][ xx ].at;

	  ps += RGB_TO_Y( p[ 0 ], p[ 1 ], p[ 2 ] ) + now[ x ];
	  if ( ps >= 255 ) { XPutPixel( src, x, y, white ); ps -= 255; }
	  else		   { XPutPixel( src, x, y, black ); }

	  down[ x + 1 ]  = error_lut45[ ps ];
	  down[ x - 1 ] += error_lut45[ ps ];
	  ps = error_lut90[ ps ];
	  down[ x ] += ps;
	}
    }
  typefree2( err );

  return src;
}



XImage *XCreateImageFromData_8Planes_PseudoColor_UChar3
  _P6 (( Display *         , display   ),
       ( register long     , win_xsize ),
       ( register long     , win_ysize ),
       ( register uchar3 **, data      ),
       ( long              , img_xsize ),
       ( long              , img_ysize ))
{
  register XImage *src;
  register int x, y, xx, yy, sx, sy;

  register long ps_r, ps_g, ps_b;
  register uchar *error_lut90 = __XEPT_90;
  register uchar *error_lut45 = __XEPT_45;

  unsigned long r, g, b;
  long **err_r = typenew2( win_xsize + 2, 2, long );
  long **err_g = typenew2( win_xsize + 2, 2, long );
  long **err_b = typenew2( win_xsize + 2, 2, long );
  register long *now_r  = err_r[1] + 1;
  register long *down_r = err_r[0] + 1;
  register long *now_g  = err_g[1] + 1;
  register long *down_g = err_g[0] + 1;
  register long *now_b  = err_b[1] + 1;
  register long *down_b = err_b[0] + 1;

  src = XGetImage( display, DefaultRootWindow( display ),
		   0, 0, win_xsize, win_ysize, AllPlanes, ZPixmap );

  for ( yy = sy = 0, y = 0; y < win_ysize; y++, sy += img_ysize )
    {
      while ( sy >= win_ysize ) { yy++; sy -= win_ysize; }

      { long *tmp = now_r; now_r = down_r; down_r = tmp; }
      { long *tmp = now_g; now_g = down_g; down_g = tmp; }
      { long *tmp = now_b; now_b = down_b; down_b = tmp; }

      down_r[ -1 ] = down_r[ 0 ] = 0;
      down_g[ -1 ] = down_g[ 0 ] = 0;
      down_b[ -1 ] = down_b[ 0 ] = 0;

      ps_r = ps_g = ps_b = 0;
      for ( xx = sx = 0, x = 0; x < win_xsize; x++, sx += img_xsize )
	{
	  while ( sx >= win_xsize ) { xx++; sx -= win_xsize; }

	  ps_r += data[ yy ][ xx ].at[ 0 ] + now_r[ x ];
	  ps_g += data[ yy ][ xx ].at[ 1 ] + now_g[ x ];
	  ps_b += data[ yy ][ xx ].at[ 2 ] + now_b[ x ];

	  if ( ps_r >= 255 ) { r = RMAX-1; ps_r -= 255;}
	  else		     { r = XLUT_PCR[ ps_r ]; ps_r -= XLUT_PCRerr[ r ]; }

	  if ( ps_g >= 255 ) { g = GMAX-1; ps_g -= 255; }
	  else		     { g = XLUT_PCG[ ps_g ]; ps_g -= XLUT_PCGerr[ g ]; }

	  if ( ps_b >= 255 ) { b = BMAX-1; ps_b -= 255; }
	  else		     { b = XLUT_PCB[ ps_b ]; ps_b -= XLUT_PCBerr[ b ]; }

	  XPutPixel( src, x, y, XLUT_PC_UChar3[ r ][ g ][ b ] );

	  down_r[ x + 1 ]  = error_lut45[ ps_r ];
	  down_r[ x - 1 ] += error_lut45[ ps_r ];
	  ps_r = error_lut90[ ps_r ];
	  down_r[ x ] += ps_r;

	  down_g[ x + 1 ]  = error_lut45[ ps_g ];
	  down_g[ x - 1 ] += error_lut45[ ps_g ];
	  ps_g = error_lut90[ ps_g ];
	  down_g[ x ] += ps_g;

	  down_b[ x + 1 ]  = error_lut45[ ps_b ];
	  down_b[ x - 1 ] += error_lut45[ ps_b ];
	  ps_b = error_lut90[ ps_b ];
	  down_b[ x ] += ps_b;
	}
    }

  typefree2( err_r );
  typefree2( err_g );
  typefree2( err_b );

  return src;
}



XImage *XCreateImageFromData_8Planes_GrayScale_UChar3
  _P6 (( Display *         , display   ),
       ( register long     , win_xsize ),
       ( register long     , win_ysize ),
       ( register uchar3 **, data      ),
       ( long              , img_xsize ),
       ( long              , img_ysize ))
{
  register XImage *src;
  register int x, y, xx, yy, sx, sy;
  register unsigned long pixel;
  register uchar *p;
  register uchar *lut = XLUT_GS_UChar;

  src = XGetImage( display, DefaultRootWindow( display ),
		   0, 0, win_xsize, win_ysize, AllPlanes, ZPixmap );

  for ( yy = sy = 0, y = 0; y < win_ysize; y++, sy += img_ysize )
    {
      while ( sy >= win_ysize ) { yy++; sy -= win_ysize; }

      for ( xx = sx = 0, x = 0; x < win_xsize; x++, sx += img_xsize )
	{
	  while ( sx >= win_xsize ) { xx++; sx -= win_xsize; }

	  p = data[ yy ][ xx ].at;
	  pixel = RGB_TO_Y( p[ 0 ], p[ 1 ], p[ 2 ] );
	  XPutPixel( src, x, y, lut[ pixel ] );
	}
    }

  return src;
}



XImage *XCreateImageFromData_8Planes_StaticGray_UChar3
  _P6 (( Display *         , display   ),
       ( register long     , win_xsize ),
       ( register long     , win_ysize ),
       ( register uchar3 **, data      ),
       ( long              , img_xsize ),
       ( long              , img_ysize ))
{
  register XImage *src;
  register int x, y, xx, yy, sx, sy;
  register unsigned long pixel;
  register uchar *p;
  register uchar *lut = XLUT_SG_UChar;

  src = XGetImage( display, DefaultRootWindow( display ),
		   0, 0, win_xsize, win_ysize, AllPlanes, ZPixmap );

  for ( yy = sy = 0, y = 0; y < win_ysize; y++, sy += img_ysize )
    {
      while ( sy >= win_ysize ) { yy++; sy -= win_ysize; }

      for ( xx =  sx = 0, x = 0; x < win_xsize; x++, sx += img_xsize )
	{
	  while ( sx >= win_xsize ) { xx++; sx -= win_xsize; }

	  p = data[ yy ][ xx ].at;
	  pixel = RGB_TO_Y( p[ 0 ], p[ 1 ], p[ 2 ] );
	  XPutPixel( src, x, y, lut[ pixel ] );
	}
    }

  return src;
}



XImage *XCreateImageFromData_24Planes_UChar3
  _P6 (( Display *         , display   ),
       ( register long     , win_xsize ),
       ( register long     , win_ysize ),
       ( register uchar3 **, data      ),
       ( long              , img_xsize ),
       ( long              , img_ysize ))
{
  register XImage *src;
  register int x, y, xx, yy, sx, sy;
  register unsigned long pixel;
  register uchar *p;
  register int r_shift, g_shift, b_shift;

  /*
    unsigned long r_mask = DefaultVisual(display,DefaultScreen(display))->red_mask;
    unsigned long g_mask = DefaultVisual(display,DefaultScreen(display))->green_mask;
    unsigned long b_mask = DefaultVisual(display,DefaultScreen(display))->blue_mask;
    if (r_mask & 0xff000000) r_shift = 24; else
      if (r_mask & 0xff0000) r_shift = 16; else
	if (r_mask & 0x00ff00) r_shift = 8; else
	  if (r_mask & 0x0000ff) r_shift = 0;
    if (g_mask & 0xff000000) g_shift = 24; else
      if (g_mask & 0xff0000) g_shift = 16; else
	if (g_mask & 0x00ff00) g_shift = 8; else
	  if (g_mask & 0x0000ff) g_shift = 0;
    if (b_mask & 0xff000000) b_shift = 24; else
      if (b_mask & 0xff0000) b_shift = 16; else
	if (b_mask & 0x00ff00) b_shift = 8; else
	  if (b_mask & 0x0000ff) b_shift = 0;
   */
  src = XGetImage( display, DefaultRootWindow( display ),
		   0, 0, win_xsize, win_ysize, AllPlanes, ZPixmap );

  for ( yy =  sy = y = 0; y < win_ysize; y++, sy += img_ysize )
    {
      while ( sy >= win_ysize ) { yy++; sy -= win_ysize; }

      for ( xx =  sx = x = 0; x < win_xsize; x++, sx += img_xsize )
	{
	  while ( sx >= win_xsize ) { xx++; sx -= win_xsize; }

	  p = data[ yy ][ xx ].at;
	  pixel = (( (p[0]>>R_LowBit) << R_Shift ) | 
		   ( (p[1]>>G_LowBit) << G_Shift ) | 
		   ( (p[2]>>B_LowBit) << B_Shift ));
	  //pixel = ( p[ 0 ] << r_shift ) | ( p[ 1 ] << g_shift ) | ( p[ 2 ] << b_shift );
	  XPutPixel( src, x, y, pixel );
	}
    }

  return src;
}



XImage *XCreateImageFromData_32Planes_UChar3
  _P6 (( Display *         , display   ),
       ( register long     , win_xsize ),
       ( register long     , win_ysize ),
       ( register uchar3 **, data      ),
       ( long              , img_xsize ),
       ( long              , img_ysize ))
{
  register XImage *src;
  register int x, y, xx, yy, sx, sy;
  register unsigned long pixel;
  register uchar *p;

  fprintf(stderr, "32 plane window AllPlanes=%d,ZPixmap=%d!",
	  AllPlanes,ZPixmap);

  src = XGetImage( display, DefaultRootWindow( display ),
		   0, 0, win_xsize, win_ysize, AllPlanes, ZPixmap );

  for ( yy =  sy = y = 0; y < win_ysize; y++, sy += img_ysize )
    {
      while ( sy >= win_ysize ) { yy++; sy -= win_ysize; }

      for ( xx =  sx = x = 0; x < win_xsize; x++, sx += img_xsize )
	{
	  while ( sx >= win_xsize ) { xx++; sx -= win_xsize; }

	  p = data[ yy ][ xx ].at;
	  pixel = (( (p[0]>>R_LowBit) << R_Shift ) | 
		   ( (p[1]>>G_LowBit) << G_Shift ) | 
		   ( (p[2]>>B_LowBit) << B_Shift ));
	  //pixel = ( p[ 0 ] ) | ( p[ 1 ] << 8 ) | ( p[ 2 ] << 16 );
	  XPutPixel( src, x, y, 0x01000000|pixel );
	}
    }

  return src;
}



XImage *XCreateImageFromData_16Planes_UChar3
  _P6 (( Display *         , display   ),
       ( register long     , win_xsize ),
       ( register long     , win_ysize ),
       ( register uchar3 **, data      ),
       ( long              , img_xsize ),
       ( long              , img_ysize ))
{
  register XImage *src;
  register int x, y, xx, yy, sx, sy;
  register unsigned long pixel;
  register uchar *p;

  src = XGetImage( display, DefaultRootWindow( display ),
		   0, 0, win_xsize, win_ysize, AllPlanes, ZPixmap );

  for ( yy = sy = y = 0; y < win_ysize; y++, sy += img_ysize )
    {
      while ( sy >= win_ysize ) { yy++; sy -= win_ysize; }

      for ( xx = sx = x = 0; x < win_xsize; x++, sx += img_xsize )
	{
	  register unsigned long r, g, b;
	  while ( sx >= win_xsize ) { xx++; sx -= win_xsize; }

	  p = data[ yy ][ xx ].at;
	  pixel = (( (p[0]>>R_LowBit) << R_Shift ) | 
		   ( (p[1]>>G_LowBit) << G_Shift ) | 
		   ( (p[2]>>B_LowBit) << B_Shift ));
	  XPutPixel( src, x, y, pixel );
	}
    }

  return src;
}



XImage *XCreateImageFromData_15Planes_UChar3
  _P6 (( Display *         , display   ),
       ( register long     , win_xsize ),
       ( register long     , win_ysize ),
       ( register uchar3 **, data      ),
       ( long              , img_xsize ),
       ( long              , img_ysize ))
{
  register XImage *src;
  register int x, y, xx, yy, sx, sy;
  register unsigned long pixel;
  register uchar *p;
  register int r_shift, g_shift, b_shift;

  /*{
    unsigned long r_mask = DefaultVisual(display,DefaultScreen(display))->red_mask;
    unsigned long g_mask = DefaultVisual(display,DefaultScreen(display))->green_mask;
    unsigned long b_mask = DefaultVisual(display,DefaultScreen(display))->blue_mask;
    if (r_mask & 0x7c00) r_shift = 10; else
      if (r_mask & 0x03e0) r_shift = 5; else
	if (r_mask & 0x001f) r_shift = 0;
    if (g_mask & 0x7c00) g_shift = 10; else
      if (g_mask & 0x03e0) g_shift = 5; else
	if (g_mask & 0x001f) g_shift = 0;
    if (b_mask & 0x7c00) b_shift = 10; else
      if (b_mask & 0x03e0) b_shift = 5; else
	if (b_mask & 0x001f) b_shift = 0;
	}*/
  src = XGetImage( display, DefaultRootWindow( display ),
		   0, 0, win_xsize, win_ysize, AllPlanes, ZPixmap );

  for ( yy =  sy = y = 0; y < win_ysize; y++, sy += img_ysize )
    {
      while ( sy >= win_ysize ) { yy++; sy -= win_ysize; }

      for ( xx =  sx = x = 0; x < win_xsize; x++, sx += img_xsize )
	{
	  while ( sx >= win_xsize ) { xx++; sx -= win_xsize; }

	  p = data[ yy ][ xx ].at;
	  pixel = (( (p[0]>>R_LowBit) << R_Shift ) | 
		   ( (p[1]>>G_LowBit) << G_Shift ) | 
		   ( (p[2]>>B_LowBit) << B_Shift ));
	  //pixel = ( (p[0]>>3) << r_shift ) | ( (p[1]>>3) << g_shift ) | ( (p[2]>>3) << b_shift );
	  XPutPixel( src, x, y, pixel );
	}
    }

  return src;
}
