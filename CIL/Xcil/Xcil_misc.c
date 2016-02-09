/*
 * filename : misc.c
 * project  : Xcil Project
 * author   : Takahiro Sugiyama
 * date     : 1993/10/19
 */



#include "Xcil.h"



void XcilPositionCoord
  _P7 (( char *, position  ),
       ( long  , box_xsize ),
       ( long  , box_ysize ),
       ( long  , xsize     ),
       ( long  , ysize     ),
       ( long *, x         ),
       ( long *, y         ))
{
  if ( position == 0 || *position == 0 ) position = "cc";

  *x = ( box_xsize - xsize ) / 2;
  *y = ( box_ysize - ysize ) / 2;

  if ( position[ 0 ] == 't' || position[ 1 ] == 't' ) *y = 0;
  if ( position[ 0 ] == 'l' || position[ 1 ] == 'l' ) *x = 0;
  if ( position[ 0 ] == 'b' || position[ 1 ] == 'b' ) *y = box_ysize - ysize;
  if ( position[ 0 ] == 'r' || position[ 1 ] == 'r' ) *x = box_xsize - xsize;
}



void XcilDrawPixmapBox
  _P10 (( Display *, display      ),
	( Window   , window       ),
	( long	   , x		  ),
	( long	   , y		  ),
	( long	   , xsize	  ),
	( long	   , ysize	  ),
	( char *   , position	  ),
	( Pixmap   , pixmap	  ),
	( long	   , pixmap_xsize ),
	( long	   , pixmap_ysize ))
{
  long rx, ry;

  XcilPositionCoord
	( position, xsize, ysize, pixmap_xsize, pixmap_ysize, &rx, &ry );

  XCopyArea(display, pixmap, window, Xcil_black_gc,
	    0, 0, pixmap_xsize, pixmap_ysize, x + rx, y + ry );
}

void XcilPutIcon16Box
  _P9 (( Display *, display  ),
       ( Window   , window   ),
       ( GC       , gc       ),
       ( long     , x        ),
       ( long     , y        ),
       ( long     , xsize    ),
       ( long     , ysize    ),
       ( char *   , position ),
       ( long     , icnID    ))
{
  long rx, ry;

  XcilPositionCoord
	( position, xsize, ysize, 16, 16, &rx, &ry );

  XCopyArea(display, Xcil_icon16_pixmap[icnID], window, gc,
	    0, 0, 16, 16, x + rx, y + ry );
}

void XcilDrawIcon16Box
  _P8 (( Display *, display      ),
       ( Window   , window       ),
       ( long	   , x		  ),
       ( long	   , y		  ),
       ( long	   , xsize	  ),
       ( long	   , ysize	  ),
       ( char *   , position	  ),
       ( long     , icnID	  ))
{
  long rx, ry;

  XcilPositionCoord
	( position, xsize, ysize, 16, 16, &rx, &ry );

  XCopyArea(display, Xcil_icon16_pixmap[icnID], window, Xcil_black_gc,
	    0, 0, 16, 16, x + rx, y + ry );
}


void XcilDrawIcon16
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( long     , x       ),
       ( long     , y       ),
       ( long     , icnID   ))
{
  XCopyArea(display, Xcil_icon16_pixmap[icnID], window, Xcil_black_gc,
	    0, 0, 16, 16, x, y );
}


void XcilPutIcon32Box
  _P9 (( Display *, display  ),
       ( Window   , window   ),
       ( GC       , gc       ),
       ( long     , x        ),
       ( long     , y        ),
       ( long     , xsize    ),
       ( long     , ysize    ),
       ( char *   , position ),
       ( long     , icnID    ))
{
  long rx, ry;

  XcilPositionCoord
	( position, xsize, ysize, 32, 32, &rx, &ry );

  XCopyArea(display, Xcil_icon32_pixmap[icnID], window, gc,
	    0, 0, 32, 32, x + rx, y + ry );
}

void XcilDrawIcon32Box
  _P8 (( Display *, display      ),
       ( Window   , window       ),
       ( long	   , x		  ),
       ( long	   , y		  ),
       ( long	   , xsize	  ),
       ( long	   , ysize	  ),
       ( char *   , position	  ),
       ( long     , icnID	  ))
{
  long rx, ry;

  XcilPositionCoord
	( position, xsize, ysize, 32, 32, &rx, &ry );

  XCopyArea(display, Xcil_icon32_pixmap[icnID], window, Xcil_black_gc,
	    0, 0, 32, 32, x + rx, y + ry );
}


void XcilDrawIcon32
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( long     , x       ),
       ( long     , y       ),
       ( long     , icnID   ))
{
  XCopyArea(display, Xcil_icon32_pixmap[icnID], window, Xcil_black_gc,
	    0, 0, 32, 32, x, y );
}


void XcilPutIcon20Box
  _P9 (( Display *, display  ),
       ( Window   , window   ),
       ( GC       , gc       ),
       ( long     , x        ),
       ( long     , y        ),
       ( long     , xsize    ),
       ( long     , ysize    ),
       ( char *   , position ),
       ( long     , icnID    ))
{
  long rx, ry;

  XcilPositionCoord
	( position, xsize, ysize, 20, 20, &rx, &ry );

  XCopyArea(display, Xcil_icon20_pixmap[icnID], window, gc,
	    0, 0, 20, 20, x + rx, y + ry );
}

void XcilDrawIcon20Box
  _P8 (( Display *, display      ),
       ( Window   , window       ),
       ( long	   , x		  ),
       ( long	   , y		  ),
       ( long	   , xsize	  ),
       ( long	   , ysize	  ),
       ( char *   , position	  ),
       ( long     , icnID	  ))
{
  long rx, ry;

  XcilPositionCoord
	( position, xsize, ysize, 20, 20, &rx, &ry );

  XCopyArea(display, Xcil_icon20_pixmap[icnID], window, Xcil_black_gc,
	    0, 0, 20, 20, x + rx, y + ry );
}


void XcilDrawIcon20
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( long     , x       ),
       ( long     , y       ),
       ( long     , icnID   ))
{
  XCopyArea(display, Xcil_icon20_pixmap[icnID], window, Xcil_black_gc,
	    0, 0, 20, 20, x, y );
}


void XcilDrawStringBox
  _P8 (( Display *, display  ),
       ( Window	  , window   ),
       ( long	  , xtop     ),
       ( long	  , ytop     ),
       ( long	  , xsize    ),
       ( long	  , ysize    ),
       ( char *	  , position ),
       ( char *	  , string   ))
{
  long x, y, xoffset, yoffset;
  long string_xsize, string_ysize;
  int ascent, descent;
  XCharStruct over;

  if ( string == 0 || *string == 0 ) return ;

  XcilTextExtents( string, &ascent, &descent, &over );

  string_xsize = XcilTextWidth( string );
  string_ysize = XcilTextHeight( string );

  XcilPositionCoord
    ( position, xsize, ysize, string_xsize, string_ysize, &xoffset, &yoffset );

  x = xtop + xoffset;
  y = ytop + yoffset + Xcil_font_struct->ascent;
  XcilPutString( display, window, Xcil_black_gc, x, y, string );
}



void XcilGetWindowSize
  _P4 (( Display *, display ),
       ( Window   , window  ),
       ( long *   , xsize   ),
       ( long *   , ysize   ))
{
  XWindowAttributes xwa;

  if ( window == 0 )
    {
      *xsize = DisplayWidth( display, 0 );
      *ysize = DisplayHeight( display, 0 );
      return;
    }

  XGetWindowAttributes( display, window, &xwa );
  *xsize = xwa.width;
  *ysize = xwa.height;
}



long XcilMouseInWindow
  _P2 (( Display *, display ),
       ( Window   , window  ))
{
  Window root, child = 0;
  int root_x, root_y;
  int win_x, win_y;
  unsigned int mask;
  long xsize, ysize;

  XcilGetWindowSize( display, window, &xsize, &ysize );

  XQueryPointer( display, window, &root, &child,
		 &root_x, &root_y, &win_x, &win_y, &mask );

  if (( win_x >= 0 ) && ( win_y >= 0 ) &&
      ( win_x < xsize ) && ( win_y < ysize ))
    return 1;

  return 0;
}
