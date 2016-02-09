/*
 * XImage for long
 */



#include "XImage.h"
#include <stdlib.h>


#define ZP8SG (2*255/3)
#define MP8SG (255/3)
#define LPP8SG (3*255/4)
#define LMP8SG (255/4)

#define ZP8GS (SMAX+2*GrayMax/3)
#define MP8GS (SMAX+GrayMax/3)
#define LPP8GS (SMAX+3*GrayMax/4)
#define LMP8GS (SMAX+GrayMax/4)

/* #define ZP8PC (XLUT_PC_UChar3[RMAX/2][GMAX/2][BMAX/2]) */
#define ZP8PC (MBASE+MMAX/2)
#define MP8PC (XLUT_PC_UChar3[0][0][BMAX-1])
#define LPP8PC (XLUT_PC_UChar3[0][GMAX-1][0])
#define LMP8PC (XLUT_PC_UChar3[RMAX-1][0][0])

#define ZP24 (((2*255/3)<<16)|((2*255/3)<<8)|(2*255/3))
#define MP24 (0x0000ff)
#define LPP24 (0x00ff00)
#define LMP24 (0xff0000)

#define ZP16 (((R_MAX_16/2)<<R_SHIFT_16)|((G_MAX_16/2)<<G_SHIFT_16)|((B_MAX_16/2)<<B_SHIFT_16))
#define MP16 ((0<<R_SHIFT_16)|(0<<G_SHIFT_16)|(B_MAX_16<<B_SHIFT_16))
#define LPP16 ((0<<R_SHIFT_16)|(G_MAX_16<<G_SHIFT_16)|(0<<B_SHIFT_16))
#define LMP16 ((R_MAX_16<<R_SHIFT_16)|(0<<G_SHIFT_16)|(0<<B_SHIFT_16))

#define ZP15 (((0x1f/2)<<10)|((0x1f)<<5)|(0x1f))
#define MP15  (0x001f)
#define LPP15 (0x03e0)
#define LMP15 (0x7c00)


static void draw_image_bit1
  _P7 (( Display *         , display   ),
       ( register XImage * , src       ),
       ( register long     , win_xsize ),
       ( register long     , win_ysize ),
       ( register long **  , data      ),
       ( long              , img_xsize ),
       ( long              , img_ysize ))
{
  register int x, y, sx, sy, xx, yy, px, py;
  register unsigned long black, white;
  register long label;

  black = BlackPixel( display, DefaultScreen( display ) );
  white = WhitePixel( display, DefaultScreen( display ) );

  for ( yy = sy = y = 0; y < win_ysize; y++, sy += img_ysize )
    {
      py = yy;
      while ( sy >= win_ysize ) { yy++; sy -= win_ysize; }

      for ( xx = sx = x = 0; x < win_xsize; x++, sx += img_xsize )
	{
	  px = xx;
	  while ( sx >= win_xsize ) { xx++; sx -= win_xsize; }

	  label = data[ yy ][ xx ];
	  if (( label == 0 ) ||
	      ( data[ yy ][ px ] != label ) ||
	      ( data[ py ][ xx ] != label ) ||
	      ( data[ py ][ px ] != label ) ||
	      ( x == 0 || y == 0 ||
	        x == ( win_xsize - 1 ) || y == ( win_ysize - 1 )))
	    XPutPixel( src, x, y, black );
	  else
	  if (( label < 0 ) && ((( y + x ) % 2 ) == 0 ))
	    XPutPixel( src, x, y, black );
	  else
	    XPutPixel( src, x, y, white );
	}
    }
}



static void draw_image_gray
  _P11 (( Display *         , display           ),
	( register XImage * , src               ),
	( register long     , win_xsize         ),
	( register long     , win_ysize         ),
	( register long **  , data              ),
	( long              , img_xsize         ),
	( long              , img_ysize         ),
	( unsigned long     , zero_pixel        ),
	( unsigned long     , minus_pixel       ),
	( unsigned long     , limit_plus_pixel  ),
	( unsigned long     , limit_minus_pixel ))
{
  register int x, y, sx, sy, xx, yy, px, py;
  register unsigned long black, white;
  register long label;

  black = BlackPixel( display, DefaultScreen( display ) );
  white = WhitePixel( display, DefaultScreen( display ) );

  for ( yy = sy = y = 0; y < win_ysize; y++, sy += img_ysize )
    {
      py = yy;
      while ( sy >= win_ysize ) { yy++; sy -= win_ysize; }

      for ( xx = sx = x = 0; x < win_xsize; x++, sx += img_xsize )
	{
	  px = xx;
	  while ( sx >= win_xsize ) { xx++; sx -= win_xsize; }

	  label = data[ yy ][ xx ];
	  if (( x == 0 || y == 0 ||
	        x == ( win_xsize - 1 ) || y == ( win_ysize - 1 )) ||
	      ( data[ yy ][ px ] != label ) ||
	      ( data[ py ][ xx ] != label ) ||
	      ( data[ py ][ px ] != label ))
	    XPutPixel( src, x, y, black );
	  else
	  if ( label == 0 )
	    {
	      XPutPixel( src, x, y, zero_pixel );
	    }
	  else
	  if ( label <= CIL_LABEL_LIMIT_MINUS )
	    {
	      XPutPixel( src, x, y, limit_minus_pixel );
	    }
	  else
	  if ( label >= CIL_LABEL_LIMIT_PLUS )
	    {
	      XPutPixel( src, x, y, limit_plus_pixel );
	    }
	  else
	  if ( label < 0 )
	    {
	      XPutPixel( src, x, y, minus_pixel );
	    }
	  else
	    XPutPixel( src, x, y, white );
	}
    }
}



static int __CATEGORY_LABEL
  _P1 ((long, l))
{
  if (l == 0) return 0;
  if ( l <= CIL_LABEL_LIMIT_MINUS ) return 1;
  if ( l >= CIL_LABEL_LIMIT_PLUS ) return 2;
  if ( l < 0 ) return 3;
  return 4;
}

static void draw_image_color
  _P11 (( Display *         , display           ),
	( register XImage * , src               ),
	( register long     , win_xsize         ),
	( register long     , win_ysize         ),
	( register long **  , data              ),
	( long              , img_xsize         ),
	( long              , img_ysize         ),
	( unsigned long     , zero_pixel        ),
	( unsigned long     , minus_pixel       ),
	( unsigned long     , limit_plus_pixel  ),
	( unsigned long     , limit_minus_pixel ))
{
  register int x, y, sx, sy, xx, yy, px, py;
  register unsigned long black, white;
  register long label;
  int category;

  black = BlackPixel( display, DefaultScreen( display ) );
  white = WhitePixel( display, DefaultScreen( display ) );

  for ( yy = sy = y = 0; y < win_ysize; y++, sy += img_ysize )
    {
      py = yy;
      while ( sy >= win_ysize ) { yy++; sy -= win_ysize; }

      for ( xx = sx = x = 0; x < win_xsize; x++, sx += img_xsize )
	{
	  px = xx;
	  while ( sx >= win_xsize ) { xx++; sx -= win_xsize; }

	  label = data[ yy ][ xx ];
	  category = __CATEGORY_LABEL(label);
	  if (( x == 0 || y == 0 ||
	        x == ( win_xsize - 1 ) || y == ( win_ysize - 1 )) ||
	      (( data[yy][px] != label) &&
	       ( category == __CATEGORY_LABEL( data[ yy ][ px ] ))) ||
	      (( data[ py ][ xx ] != label ) &&
	       ( category == __CATEGORY_LABEL( data[ py ][ xx ] ))) ||
	      (( data[ py ][ px ] != label ) &&
	       ( category == __CATEGORY_LABEL( data[ py ][ px ] ))))
	    XPutPixel( src, x, y, black );
	  else
	    {
	      switch (category)
		{
		case 0: XPutPixel( src, x, y, zero_pixel ); break;
		case 1: XPutPixel( src, x, y, limit_minus_pixel ); break;
		case 2: XPutPixel( src, x, y, limit_plus_pixel ); break;
		case 3: XPutPixel( src, x, y, minus_pixel ); break;
		case 4: XPutPixel( src, x, y, white ); break;
		}
	    }
	}
    }
}


XImage *XCreateImageFromData_1Plane_Long
  _P6 (( Display * , display   ),
       ( long      , win_xsize ),
       ( long      , win_ysize ),
       ( long   ** , org       ),
       ( long      , img_xsize ),
       ( long      , img_ysize ))
{
  XImage *src;
  uchar3 **data;

  src = XGetImage( display, DefaultRootWindow( display ),
		  0, 0, win_xsize,win_ysize,AllPlanes,ZPixmap);
  draw_image_bit1( display, src, win_xsize, win_ysize,
		  org, img_xsize, img_ysize );

  return src;
}



XImage *XCreateImageFromData_8Planes_PseudoColor_Long
  _P6 (( Display * , display   ),
       ( long      , win_xsize ),
       ( long      , win_ysize ),
       ( long   ** , org       ),
       ( long      , img_xsize ),
       ( long      , img_ysize ))
{
  XImage *src;
  uchar3 **data;

  src = XGetImage( display, DefaultRootWindow( display ),
		  0, 0, win_xsize,win_ysize,AllPlanes,ZPixmap);
  draw_image_color( display, src, win_xsize, win_ysize,
		    org, img_xsize, img_ysize, ZP8PC, MP8PC, LPP8PC, LMP8PC );

  return src;
}



XImage *XCreateImageFromData_8Planes_GrayScale_Long
  _P6 (( Display * , display   ),
       ( long      , win_xsize ),
       ( long      , win_ysize ),
       ( long   ** , org       ),
       ( long      , img_xsize ),
       ( long      , img_ysize ))
{
  XImage *src;
  uchar3 **data;

  src = XGetImage( display, DefaultRootWindow( display ),
		  0, 0, win_xsize,win_ysize,AllPlanes,ZPixmap);
  draw_image_gray( display, src, win_xsize, win_ysize,
		   org, img_xsize, img_ysize, ZP8GS, MP8GS, LPP8GS, LMP8GS );

  return src;
}



XImage *XCreateImageFromData_8Planes_StaticGray_Long
  _P6 (( Display * , display   ),
       ( long      , win_xsize ),
       ( long      , win_ysize ),
       ( long   ** , org       ),
       ( long      , img_xsize ),
       ( long      , img_ysize ))
{
  XImage *src;
  uchar3 **data;

  src = XGetImage( display, DefaultRootWindow( display ),
		  0, 0, win_xsize,win_ysize,AllPlanes,ZPixmap);
  draw_image_gray( display, src, win_xsize, win_ysize,
		   org, img_xsize, img_ysize, ZP8SG, MP8SG, LPP8SG, LMP8SG );

  return src;
}



XImage *XCreateImageFromData_24Planes_Long
  _P6 (( Display * , display   ),
       ( long      , win_xsize ),
       ( long      , win_ysize ),
       ( long   ** , org       ),
       ( long      , img_xsize ),
       ( long      , img_ysize ))
{
  XImage *src;
  uchar3 **data;

  src = XGetImage( display, DefaultRootWindow( display ),
		  0, 0, win_xsize,win_ysize,AllPlanes,ZPixmap);
  draw_image_color( display, src, win_xsize, win_ysize,
		    org, img_xsize, img_ysize, ZP24, MP24, LPP24, LMP24  );

  return src;
}



XImage *XCreateImageFromData_16Planes_Long
  _P6 (( Display * , display   ),
       ( long      , win_xsize ),
       ( long      , win_ysize ),
       ( long   ** , org       ),
       ( long      , img_xsize ),
       ( long      , img_ysize ))
{
  XImage *src;
  uchar3 **data;

  src = XGetImage( display, DefaultRootWindow( display ),
		  0, 0, win_xsize,win_ysize,AllPlanes,ZPixmap);
  draw_image_color( display, src, win_xsize, win_ysize,
		    org, img_xsize, img_ysize, ZP16, MP16, LPP16, LMP16  );

  return src;
}



XImage *XCreateImageFromData_15Planes_Long
  _P6 (( Display * , display   ),
       ( long      , win_xsize ),
       ( long      , win_ysize ),
       ( long   ** , org       ),
       ( long      , img_xsize ),
       ( long      , img_ysize ))
{
  XImage *src;
  uchar3 **data;

  src = XGetImage( display, DefaultRootWindow( display ),
		  0, 0, win_xsize,win_ysize,AllPlanes,ZPixmap);
  draw_image_color( display, src, win_xsize, win_ysize,
		    org, img_xsize, img_ysize, ZP15, MP15, LPP15, LMP15  );

  return src;
}
