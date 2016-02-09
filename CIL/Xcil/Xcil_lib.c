/*
 * filename : xcillib.c
 * project  : Xcil Project
 * author   : Takahiro Sugiyama
 * date     : 1993/10/19
 */



#include "Xcil.h"
#include "misc/strlib.h"
#include <X11/Xatom.h>
#include <stdlib.h>



XFontStruct *Xcil_font_struct;
XFontStruct *Xcil_kanji_struct;
Display *Xcil_display;
long Xcil_depth;
long Xcil_plane;
long Xcil_visual;
Window Xcil_root_window;

GC Xcil_black_gc;
GC Xcil_white_gc;
GC Xcil_gray_gc;
GC Xcil_invert_gc;
GC Xcil_or_gc;

Pixmap Xcil_gray_xpixmap[ 5 ];
Pixmap Xcil_gray_ypixmap[ 5 ];

Pixmap Xcil_pattern_pixmap[ 64 ];
Pixmap Xcil_icon16_pixmap[ ICN16Last ];
Pixmap Xcil_icon20_pixmap[ ICN20Last ];
Pixmap Xcil_icon32_pixmap[ ICN32Last ];

unsigned long Xcil_black_pixel;
unsigned long Xcil_white_pixel;

/* ABOUT WM INFORMATION */
long Xcil_wm_xoffset;
long Xcil_wm_yoffset;
long Xcil_wm_xsize;
long Xcil_wm_ysize;

Cursor Xcil_up_cursor = 0;
Cursor Xcil_down_cursor = 0;
Cursor Xcil_right_cursor = 0;
Cursor Xcil_left_cursor = 0;
Cursor Xcil_up_down_cursor = 0;
Cursor Xcil_right_left_cursor = 0;
Cursor Xcil_pointer_cursor = 0;
Cursor Xcil_wait_cursor = 0;
Cursor Xcil_cross_cursor = 0;
Cursor Xcil_hand2_cursor = 0;

Colormap Xcil_cmap = 0;

long Xcil_Japanese_message = 0;


static void __Xcil_set_cmap
  P1 (( Display *, display ))



void XcilMapWindow
  _P2 (( Display *, display ),
       ( Window   , window  ))
{
  Window child;
  XWindowAttributes xwa;
  XSizeHints xsh;
  int trans_x, trans_y;

  XSync( display, False );

  XGetWindowAttributes( display, window, &xwa );

  XTranslateCoordinates( display, window, Xcil_root_window,
			 0, 0, &trans_x, &trans_y, &child );

  if ( xwa.map_state == IsViewable )
    {
      trans_x -= xwa.x;
      trans_y -= xwa.y;
    }

  xsh.flags = USPosition;
  xsh.x = trans_x;
  xsh.y = trans_y;
  XSetStandardProperties( display, window, 0, 0, None, 0,0, &xsh );

  XMapRaised( display, window );
/*
  XSetWindowColormap( display, window, Xcil_cmap );
*/
}


#include <X11/Xatom.h>
Window XcilCreateWindow
  _P8 (( Display *, display ),
       ( Window	  , parent  ),
       ( long	  , x	    ),
       ( long	  , y	    ),
       ( long	  , xsize   ),
       ( long	  , ysize   ),
       ( long	  , border  ),
       ( char *	  , title   ))
{
  XWMHints xwmh;
  XSizeHints xsh;
  Window window;
  Bool use_position = False;

  if (( x == 0 ) && ( parent == 0 ))
    x = ( DisplayWidth( display, 0 ) - xsize - Xcil_wm_xsize ) / 2;
  if (( y == 0 ) && ( parent == 0 ))
    y = ( DisplayHeight( display, 0 ) - ysize - Xcil_wm_ysize ) / 2;

  if (( x >= 0 ) && ( y >= 0 ) && ( parent == 0 ))
    use_position = True;

  if ( parent == 0 ) parent = RootWindow( display, 0 );

  window = XCreateSimpleWindow( display, parent, x, y, xsize, ysize, border,
			        Xcil_black_pixel, Xcil_white_pixel );
  XcilAddEventMask( display, window, Xcil_event_mask[ MapNotify ] );
  XcilAddEventMask( display, window, Xcil_event_mask[ MappingNotify ] );

  if ( use_position )
    {
      xsh.x = x;
      xsh.y = y;
      xsh.flags = USPosition;
      XSetStandardProperties( display, window, title, title, None, 0,0, &xsh );
    }
  else
    {
      XSetStandardProperties( display, window, title, title, None, 0,0,0 );
    }

  xwmh.flags = InputHint | StateHint;
  xwmh.input = True;
  xwmh.initial_state = NormalState;
  XSetWMHints( display, window, &xwmh );

  XFlush( display );

  XDefineCursor( display, window, Xcil_pointer_cursor );
  if ( Xcil_plane == 8 )
    {
      XSetWindowColormap( display, window, Xcil_cmap );
      if ( parent == Xcil_root_window )
	{
	  Atom ATOM_WM_COLMAP_WIN;
	  ATOM_WM_COLMAP_WIN = XInternAtom( display,
					   "WM_COLORMAP_WINDOWS", False );
	  XChangeProperty( display, parent, ATOM_WM_COLMAP_WIN, XA_WINDOW, 32,
			   PropModeReplace,
			  (unsigned char *)&window, 1 );
	}
    }

  return window;
}



static char pattern_list[64][8] = {
/*00*/ {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
/*01*/ {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
/*02*/ {0xff, 0xff, 0xff, 0xf7, 0xff, 0xff, 0xff, 0x7f},
/*03*/ {0xff, 0xf7, 0xff, 0x7f, 0xff, 0xf7, 0xff, 0x7f},
/*04*/ {0xff, 0xdd, 0xff, 0x77, 0xff, 0xdd, 0xff, 0x77},
/*05*/ {0xdd, 0x77, 0xdd, 0x77, 0xdd, 0x77, 0xdd, 0x77},
/*06*/ {0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55},
/*07*/ {0x22, 0x88, 0x22, 0x88, 0x22, 0x88, 0x22, 0x88},
/*08*/ {0x00, 0x22, 0x00, 0x88, 0x00, 0x22, 0x00, 0x88},
/*09*/ {0x00, 0x08, 0x00, 0x80, 0x00, 0x08, 0x00, 0x80},
/*10*/ {0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x80},
/*11*/ {0x00, 0xaa, 0x00, 0xaa, 0x00, 0xaa, 0x00, 0xaa},
/*12*/ {0x00, 0x11, 0x00, 0xaa, 0x00, 0x44, 0x00, 0xaa},
/*13*/ {0x00, 0xaa, 0x00, 0x88, 0x00, 0xaa, 0x00, 0x88},
/*14*/ {0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00, 0x88},
/*15*/ {0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00},
/*16*/ {0xee, 0xdd, 0xbb, 0x77, 0xee, 0xdd, 0xbb, 0x77},
/*17*/ {0x33, 0x99, 0xcc, 0x66, 0x33, 0x99, 0xcc, 0x66},
/*18*/ {0x22, 0x11, 0x88, 0x44, 0x22, 0x11, 0x88, 0x44},
/*19*/ {0x70, 0x38, 0x1c, 0x0e, 0x07, 0x83, 0xc1, 0xe0},
/*20*/ {0x03, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x81},
/*21*/ {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80},
/*22*/ {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff},
/*23*/ {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff},
/*24*/ {0xdd, 0xee, 0x77, 0xbb, 0xdd, 0xee, 0x77, 0xbb},
/*25*/ {0x99, 0x33, 0x66, 0xcc, 0x99, 0x33, 0x66, 0xcc},
/*26*/ {0x11, 0x22, 0x44, 0x88, 0x11, 0x22, 0x44, 0x88},
/*27*/ {0x07, 0x0e, 0x1c, 0x38, 0x70, 0xe0, 0xc1, 0x83},
/*28*/ {0x30, 0x18, 0x0c, 0x06, 0x03, 0x81, 0xc0, 0x60},
/*29*/ {0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x80, 0x40},
/*30*/ {0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81},
/*31*/ {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xff},
/*32*/ {0xaa, 0xff, 0xaa, 0xff, 0xaa, 0xff, 0xaa, 0xff},
/*33*/ {0x77, 0x77, 0x77, 0x00, 0x77, 0x77, 0x77, 0x00},
/*34*/ {0x88, 0x88, 0x88, 0xff, 0x88, 0x88, 0x88, 0xff},
/*35*/ {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa},
/*36*/ {0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff},
/*37*/ {0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88},
/*38*/ {0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff},
/*39*/ {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80},
/*40*/ {0xcf, 0xc9, 0x39, 0x3f, 0xf3, 0x93, 0x9c, 0xfc},
/*41*/ {0x30, 0x36, 0xc6, 0xc0, 0x0c, 0x6c, 0x63, 0x03},
/*42*/ {0x10, 0x01, 0x20, 0x04, 0x80, 0x08, 0x40, 0x02},
/*43*/ {0x80, 0x80, 0x80, 0xff, 0x08, 0x08, 0x08, 0xff},
/*44*/ {0x03, 0x30, 0x48, 0x84, 0x03, 0x30, 0x48, 0x84},
/*45*/ {0x11, 0x0a, 0x04, 0x0a, 0x11, 0xa0, 0x40, 0xa0},
/*46*/ {0x40, 0x04, 0x04, 0x40, 0x40, 0x04, 0x04, 0x40},
/*47*/ {0x01, 0x80, 0x00, 0x08, 0x10, 0x20, 0x00, 0x02},
/*48*/ {0x87, 0x87, 0x87, 0x78, 0x78, 0x78, 0x78, 0x87},
/*49*/ {0x11, 0xaa, 0x44, 0xaa, 0x11, 0xaa, 0x44, 0xaa},
/*50*/ {0x14, 0x22, 0x41, 0x88, 0x00, 0xaa, 0x00, 0x88},
/*51*/ {0x00, 0x80, 0x00, 0x88, 0x00, 0x80, 0x00, 0xaa},
/*52*/ {0x80, 0x41, 0x3e, 0x08, 0x08, 0x14, 0xe3, 0x80},
/*53*/ {0x90, 0x09, 0x06, 0x18, 0x20, 0x40, 0x40, 0x60},
/*54*/ {0x21, 0x12, 0x0c, 0x04, 0x02, 0x01, 0x80, 0xc0},
/*55*/ {0x82, 0x00, 0x00, 0x10, 0x28, 0x00, 0x00, 0x01},
/*56*/ {0xc8, 0xf8, 0xf8, 0x77, 0x8c, 0x8f, 0x8f, 0x77},
/*57*/ {0x8f, 0x1b, 0xb1, 0xf8, 0x7c, 0x36, 0x63, 0xc7},
/*58*/ {0x00, 0xfe, 0xfe, 0x86, 0x86, 0x86, 0x86, 0xfe},
/*59*/ {0x05, 0x88, 0x88, 0x88, 0x88, 0x50, 0x20, 0x02},
/*60*/ {0x02, 0x15, 0xaa, 0xff, 0x20, 0x10, 0x08, 0x04},
/*61*/ {0x08, 0x14, 0x2a, 0x55, 0x2a, 0x14, 0x08, 0x00},
/*62*/ {0x82, 0x01, 0x01, 0x55, 0x28, 0x10, 0x10, 0x55},
/*63*/ {0x11, 0x4e, 0x11, 0xa0, 0x40, 0x40, 0x40, 0xa0}
};



Display *XcilOpenDisplay
  _P1 (( char *, display_name ))
{
  Display *display;
  Window window;
  char *name;
  int i;
  unsigned long black;
  unsigned long white;

  static char or_gra1_bits[] = { 0x00, 0x04, 0x00, 0x01 };

  static char gray0_xbits[] = { 0x0b, 0x0e };
  static char gray1_xbits[] = { 0x0a, 0x05 };
  static char gray2_xbits[] = { 0x04, 0x01 };
  static char gray3_xbits[] = { 0x01, 0x00, 0x04, 0x00 };
  static char gray4_xbits[] = { 0x00, 0x01, 0x00, 0x00 };

  static char gray0_ybits[] = { 0x03, 0x01, 0x03, 0x02 };
  static char gray1_ybits[] = { 0x01, 0x02, 0x01, 0x02 };
  static char gray2_ybits[] = { 0x00, 0x01, 0x00, 0x02 };
  static char gray3_ybits[] = { 0x01, 0x00, 0x04, 0x00 };
  static char gray4_ybits[] = { 0x00, 0x01, 0x00, 0x00 };
  static Pixmap or_gray_pattern = 0;

  name = ( display_name ) ? display_name : getenv( "DISPLAY" );
  display = XOpenDisplay( name );
  if ( display == 0 )
    {
      fprintf( stderr, "error:can't open display.\n" );
      fprintf( stderr, "      check your environ variable DISPLAY\n" );
      exit( -1 );
    }
  window = RootWindow( display, 0 );
  black  = BlackPixel( display, 0 );
  white  = WhitePixel( display, 0 );

  Xcil_display = display;
  Xcil_root_window = window;
  Xcil_black_pixel = black;
  Xcil_white_pixel = white;
  Xcil_depth = DefaultDepth( display, 0 );
  Xcil_plane = DisplayPlanes( display, 0 );
  Xcil_visual = DefaultVisual( display, 0 )->class;
  Xcil_font_struct = XLoadQueryFont( display, "a14" );
  Xcil_kanji_struct = XLoadQueryFont( display, "k14" );

  Xcil_black_gc  = XCreateGC( display, window, 0, 0 );
  Xcil_white_gc  = XCreateGC( display, window, 0, 0 );
  Xcil_gray_gc   = XCreateGC( display, window, 0, 0 );
  Xcil_invert_gc = XCreateGC( display, window, 0, 0 );
  Xcil_or_gc     = XCreateGC( display, window, 0, 0 );

  XSetForeground( display, Xcil_black_gc , black );
  XSetForeground( display, Xcil_white_gc , white );
  XSetForeground( display, Xcil_gray_gc  , black );
  XSetForeground( display, Xcil_invert_gc, black );
  XSetForeground( display, Xcil_or_gc    , black );

  XSetFont( display, Xcil_black_gc , Xcil_font_struct->fid );
  XSetFont( display, Xcil_white_gc , Xcil_font_struct->fid );
  XSetFont( display, Xcil_gray_gc  , Xcil_font_struct->fid );
  XSetFont( display, Xcil_invert_gc, Xcil_font_struct->fid );
  XSetFont( display, Xcil_or_gc    , Xcil_font_struct->fid );

  if (( Xcil_plane == 24 ) || ( Xcil_plane == 16 ))
    XSetFunction( display, Xcil_or_gc, GXand );
  else			  XSetFunction( display, Xcil_or_gc, GXor );
  XSetFunction( display, Xcil_invert_gc, GXinvert );

  or_gray_pattern = XCreatePixmapFromBitmapData
    ( display, window, or_gra1_bits, 4, 4, black, white, Xcil_depth );

  Xcil_gray_xpixmap[ 0 ] = XCreatePixmapFromBitmapData
    ( display, window, gray0_xbits, 4, 2, black, white, Xcil_depth );
  Xcil_gray_xpixmap[ 1 ] = XCreatePixmapFromBitmapData
    ( display, window, gray1_xbits, 4, 2, black, white, Xcil_depth );
  Xcil_gray_xpixmap[ 2 ] = XCreatePixmapFromBitmapData
    ( display, window, gray2_xbits, 4, 2, black, white, Xcil_depth );
  Xcil_gray_xpixmap[ 3 ] = XCreatePixmapFromBitmapData
    ( display, window, gray3_xbits, 4, 4, black, white, Xcil_depth );
  Xcil_gray_xpixmap[ 4 ] = XCreatePixmapFromBitmapData
    ( display, window, gray4_xbits, 4, 4, black, white, Xcil_depth );

  Xcil_gray_ypixmap[ 0 ] = XCreatePixmapFromBitmapData
    ( display, window, gray0_ybits, 2, 4, black, white, Xcil_depth );
  Xcil_gray_ypixmap[ 1 ] = XCreatePixmapFromBitmapData
    ( display, window, gray1_ybits, 2, 4, black, white, Xcil_depth );
  Xcil_gray_ypixmap[ 2 ] = XCreatePixmapFromBitmapData
    ( display, window, gray2_ybits, 2, 4, black, white, Xcil_depth );
  Xcil_gray_ypixmap[ 3 ] = XCreatePixmapFromBitmapData
    ( display, window, gray3_ybits, 4, 4, black, white, Xcil_depth );
  Xcil_gray_ypixmap[ 4 ] = XCreatePixmapFromBitmapData
    ( display, window, gray4_ybits, 4, 4, black, white, Xcil_depth );

  for ( i = 0; i < 64; i++ )
    Xcil_pattern_pixmap[ i ] = XCreatePixmapFromBitmapData
      ( display, window, pattern_list[i], 8, 8, black, white, Xcil_depth );

  XSetFillStyle( display, Xcil_or_gc, FillTiled );
  XSetFillStyle( display, Xcil_gray_gc, FillTiled );

  XSetTile( display, Xcil_or_gc, Xcil_gray_xpixmap[ 1 ] );

  {/* WM TEST */
    Window test;
    Window root, parent, *sub;
    XEvent event;
    unsigned int nsub, xsize, ysize, border, depth;
    int x, y;

    test = XcilCreateWindow( display, 0, 1, 1, 100, 1, 1, "WMT" );
    XcilMapWindow( display, test );

    while ( event.type != MapNotify ) XNextEvent( display, &event );

    XQueryTree( display, test, &root, &parent, &sub, &nsub );
    XGetGeometry( display, test, &root,
		  &x, &y, &xsize, &ysize, &border, &depth );
    Xcil_wm_xoffset = x;
    Xcil_wm_yoffset = y;
    XGetGeometry( display, parent, &root,
		  &x, &y, &xsize, &ysize, &border, &depth );
    Xcil_wm_xoffset += border;
    Xcil_wm_yoffset += border;
    Xcil_wm_xsize = xsize - 100 + border * 2;
    Xcil_wm_ysize = ysize - 1 + border * 2;

    XDestroyWindow( display, test );
  }

  __Xcil_set_cursors( display );
  __Xcil_set_icon16( display );
  __Xcil_set_icon20( display );
  __Xcil_set_icon32( display );
  __Xcil_set_cmap( display );

  return display;
}



void XcilTransGeometry
  _P5 (( char *, string ),
       ( long *, x      ),
       ( long *, y      ),
       ( long *, xsize  ),
       ( long *, ysize  ))
{
  char buf[ 256 ];
  char *head[ 10 ];
  long n;

  strcpy( buf, string );

  n = strsplit( buf, head, "x+-" );

  if ( n > 0 ) *xsize = atol( head[ 0 ] );
  if ( n > 1 ) *ysize = atol( head[ 1 ] );
  if ( n > 2 )
    {
      *x = atol( head[ 2 ] );
      if ( string[ head[ 2 ] - buf - 1 ] == '-' ) *x = - *x;
    }

  if ( n > 3 )
    {
      *y = atol( head[ 3 ] );
      if ( string[ head[ 3 ] - buf - 1 ] == '-' ) *y = - *y;
    }
}



void __Xcil_set_cursors
  _P1 (( Display *, display ))
{
  if ( Xcil_up_cursor != 0 ) return;

  Xcil_up_cursor         = XCreateFontCursor( display, XC_sb_up_arrow );
  Xcil_down_cursor       = XCreateFontCursor( display, XC_sb_down_arrow );
  Xcil_left_cursor       = XCreateFontCursor( display, XC_sb_left_arrow );
  Xcil_right_cursor      = XCreateFontCursor( display, XC_sb_right_arrow );
  Xcil_up_down_cursor    = XCreateFontCursor( display, XC_sb_v_double_arrow );
  Xcil_right_left_cursor = XCreateFontCursor( display, XC_sb_h_double_arrow );

  Xcil_pointer_cursor    = XCreateFontCursor( display, XC_left_ptr );
  Xcil_wait_cursor       = XCreateFontCursor( display, XC_watch );
  Xcil_cross_cursor      = XCreateFontCursor( display, XC_tcross );
  Xcil_hand2_cursor      = XCreateFontCursor( display, XC_hand2 );
}



#include "Xcil/bitmap/None16"
#include "Xcil/bitmap/Apple16"
#include "Xcil/bitmap/Check16"
#include "Xcil/bitmap/Folder16"
#include "Xcil/bitmap/Doc16"
#include "Xcil/bitmap/App16"
#include "Xcil/bitmap/SymLnk16"
#include "Xcil/bitmap/Cmd16"

static char *icon16_list[] = {
  None16_bits,
  Apple16_bits,
  Check16_bits,
  Folder16_bits,
  Doc16_bits,
  App16_bits,
  SymLnk16_bits,
  Cmd16_bits,
};

void __Xcil_set_icon16
  _P1 (( Display *, display ))
{
  int i;

  for ( i = 0; i < ICN16Last; i++ )
    Xcil_icon16_pixmap[ i ] = XCreatePixmapFromBitmapData
      ( display, Xcil_root_window, icon16_list[i], 16, 16,
       Xcil_black_pixel, Xcil_white_pixel, Xcil_depth );
}



#include "Xcil/bitmap/None20"
#include "Xcil/bitmap/Button20"
#include "Xcil/bitmap/CheckBox20"
#include "Xcil/bitmap/CheckedBox20"
#include "Xcil/bitmap/RadioButton20"
#include "Xcil/bitmap/Radio1Button20"
#include "Xcil/bitmap/ScrollBar20"
#include "Xcil/bitmap/ScrollUp20"
#include "Xcil/bitmap/ScrollDown20"
#include "Xcil/bitmap/ScrollLeft20"
#include "Xcil/bitmap/ScrollRight20"
#include "Xcil/bitmap/ScrollSize20"

static char *icon20_list[] = {
  None20_bits,
  Button20_bits,
  CheckBox20_bits,
  CheckedBox20_bits,
  RadioButton20_bits,
  Radio1Button20_bits,
  ScrollBar20_bits,
  ScrollUp20_bits,
  ScrollDown20_bits,
  ScrollLeft20_bits,
  ScrollRight20_bits,
  ScrollSize20_bits,
};

void __Xcil_set_icon20
  _P1 (( Display *, display ))
{
  int i;

  for ( i = 0; i < ICN20Last; i++ )
    Xcil_icon20_pixmap[ i ] = XCreatePixmapFromBitmapData
      ( display, Xcil_root_window, icon20_list[i], 20, 20,
       Xcil_black_pixel, Xcil_white_pixel, Xcil_depth );
}



#include "Xcil/bitmap/None32"
#include "Xcil/bitmap/Bomb32"
#include "Xcil/bitmap/Caution32"
#include "Xcil/bitmap/Note32"
#include "Xcil/bitmap/Stop32"

static char *icon32_list[] = {
  None32_bits,
  Bomb32_bits,
  Caution32_bits,
  Note32_bits,
  Stop32_bits,
};

void __Xcil_set_icon32
  _P1 (( Display *, display ))
{
  int i;

  for ( i = 0; i < ICN32Last; i++ )
    Xcil_icon32_pixmap[ i ] = XCreatePixmapFromBitmapData
      ( display, Xcil_root_window, icon32_list[i], 32, 32,
       Xcil_black_pixel, Xcil_white_pixel, Xcil_depth );
}




static void XcilSetColormap_8Planes_GrayScale
  P1 (( Display *, display ))

static void XcilSetColormap_8Planes_PseudoColor
  P1 (( Display *, display ))

static void __Xcil_set_cmap
  _P1 (( Display *, display ))
{
  if ( Xcil_plane != 8 ) return;

  switch ( Xcil_visual )
    {
    case PseudoColor:
      XcilSetColormap_8Planes_PseudoColor(display);
      break;
    case GrayScale:
      XcilSetColormap_8Planes_GrayScale(display);
      break;
    }
}



/*
 * ８ビットプレーンのためのカラーマップ情報
 *
 * PseudoColor
 */
#define SBASE 0
#define SMAX  16

#define MBASE (SBASE+SMAX)
#define MMAX  64

#define CBASE (MBASE+MMAX)
#define RMAX  6
#define GMAX  7
#define BMAX  4
#define CMAX  (RMAX * GMAX * BMAX)

#define FBASE (CBASE+CMAX)
#define FMAX  8

static void XcilSetColormap_8Planes_PseudoColor
  _P1 (( Display *, display ))
{
  XColor color[256];
  Visual *visual;
  uint ri,gi,bi;
  int i,r,g,b,v;
  unsigned long p;

  visual = DefaultVisual(display,DefaultScreen(display));
  Xcil_cmap = XCreateColormap(display,Xcil_root_window,visual,AllocAll);
  for (i = 0; i < SMAX; i++)
    {
      color[i].pixel = i;
      color[i].flags = DoRed|DoGreen|DoBlue;
    }
  XQueryColors(display,DefaultColormap(display,DefaultScreen(display)),
	       color,SMAX);
  for (i = 0; i < FMAX; i++)
    {
      v = i * 0xffff / (FMAX-1);
      color[FBASE+i].red   = color[i].red;
      color[FBASE+i].green = color[i].green;
      color[FBASE+i].blue  = color[i].blue;
      color[FBASE+i].pixel = i + FBASE;
      color[FBASE+i].flags = DoRed|DoGreen|DoBlue;
    }
  color[255].red = 0;
  color[255].green = 0;
  color[255].blue = 0;
  color[255].pixel = 255;

  color[254].red = 0xffff;
  color[254].green = 0xffff;
  color[254].blue = 0xffff;
  color[254].pixel = 254;

  for (i = 0; i < MMAX; i++)
    {
      v = i * 0xffff / (MMAX-1);
      color[MBASE+i].red = color[MBASE+i].green = color[MBASE+i].blue  = v;
      color[MBASE+i].pixel = i + MBASE;
      color[MBASE+i].flags = DoRed|DoGreen|DoBlue;
    }
  for (ri = 0; ri < RMAX; ri++)
    {
      r = ri * 0xffff / (RMAX-1);
      for (gi = 0; gi < GMAX; gi++)
	{
	  g = gi * 0xffff / (GMAX-1);
	  for (bi = 0; bi < BMAX; bi++)
	    {
	      b = bi * 0xffff / (BMAX-1);
	      p = ri * BMAX * GMAX + gi * BMAX + bi + CBASE;
	      color[p].red   = r;
	      color[p].green = g;
	      color[p].blue  = b;
	      color[p].pixel = p;
	      color[p].flags = DoRed|DoGreen|DoBlue;
	    }
	}
    }
  XStoreColors( display, Xcil_cmap, color, 256 );
}



/*
 * For GrayScale
 */
#define GrayBase  SMAX
#define GrayMax   (256-SMAX-2)

static void XcilSetColormap_8Planes_GrayScale
  _P1 (( Display *, display ))
{
  XColor color[256];
  Visual *visual;
  uint ri,gi,bi;
  int i,r,g,b,v;
  unsigned long p;

  visual = DefaultVisual(display,DefaultScreen(display));
  Xcil_cmap = XCreateColormap(display,Xcil_root_window,visual,AllocAll);
  for (i = 0; i < SMAX; i++)
    {
      color[i].pixel = i;
      color[i].flags = DoRed|DoGreen|DoBlue;
    }
  XQueryColors(display,DefaultColormap(display,DefaultScreen(display)),
	       color,SMAX);
  for (i = 0; i < SMAX; i++)
    {
      color[i].red   = color[i].red;
      color[i].green = color[i].green;
      color[i].blue  = color[i].blue;
      color[i].pixel = i;
      color[i].flags = DoRed|DoGreen|DoBlue;
    }
  for (i = 0; i < GrayMax; i++)
    {
      v = i * 0xffff / (GrayMax-1);
      color[GrayBase+i].red   = v;
      color[GrayBase+i].green = v;
      color[GrayBase+i].blue  = v;
      color[GrayBase+i].pixel = i + GrayBase;
      color[GrayBase+i].flags = DoRed|DoGreen|DoBlue;
    }
  color[255].red = 0;
  color[255].green = 0;
  color[255].blue = 0;
  color[255].pixel = 255;
  color[255].flags = DoRed|DoGreen|DoBlue;

  color[254].red = 0xffff;
  color[254].green = 0xffff;
  color[254].blue = 0xffff;
  color[254].pixel = 254;
  color[254].flags = DoRed|DoGreen|DoBlue;

  XStoreColors( display, Xcil_cmap, color, 256 );
}
