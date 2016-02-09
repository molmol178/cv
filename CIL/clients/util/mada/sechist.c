/*
 *
 */



#include "Image.h"
#include "Xcil/Xcil.h"
#include "Xcil/XImage.h"
#include <stdio.h>



void xsec_disp
  P1 (( image , img  ))

void histogram
  P2 (( image , in   ),
      ( long *, freq ))

void info_disp
  P3 (( Display *, display ),
      ( XContext , context ),
      ( secRec *, sec      ))

typedef struct {
  Window panel;
  Window iwin, xwin, ywin;
  image img;
  long x, y;
  long shift;
  long freq[256];
} secRec;


void main( argc, argv )
     int argc;
     char *argv[];
{
  image in_image;
  char *in_file = "-";

  if ( argc < 1 )
    {
      fprintf(stderr,"Usage : sec [in]\n");
      exit(-1);
    }

  if ( argc > 1 ) in_file = argv[ 1 ];

  in_image = Image.createFromFilename( in_file, in_file );
  if ( in_image == 0 )
    {
      fprintf( stderr, "error:%s:can't open file\n", argv[0] );
      exit(-1);
    }

  if ( __TYPE( in_image ) != UChar )
    {
      fprintf( stderr, "error:%s:bad image\n", argv[0] );
      exit(-1);
    }

  xsec_disp( in_image );
}



void set_xy
  _P6 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( secRec * , sec     ),
       ( long     , x       ),
       ( long     , y       ))
{
  if (( sec->x != -1 ) && ( sec->y != -1 ))
    {
      XDrawLine( display, window, Xcil_invert_gc, sec->x, 0, sec->x, 255 );
      XDrawLine( display, window, Xcil_invert_gc, 0, sec->y, 256, sec->y );
    }
  sec->x = x;
  sec->y = y;

  XDrawLine( display, window, Xcil_invert_gc, sec->x, 0, sec->x, 255 );
  XDrawLine( display, window, Xcil_invert_gc, 0, sec->y, 256, sec->y );
}



void info_disp
  _P3 (( Display *, display ),
       ( XContext , context ),
       ( secRec * , sec     ))
{
  long x, y, i, j, total;

  x = __XSIZE(sec->img) * sec->x / 256;
  y = __YSIZE(sec->img) * sec->y / 256;
  i = __PIXEL(sec->img,x,y,uchar);

  XcilSetString( display, sec->panel, context, "I",
		"I(%d,%d) = %d\n", x, y, i );
  total = 0;
  for ( total = j = 0; j < i; j++ ) total += sec->freq[ j ];

  XcilSetString( display, sec->panel, context, "FREQ",
		"FREQ = %d\n", sec->freq[i] );
  XcilSetString( display, sec->panel, context, "SUM",
		"SUM = %d\n", total );
}



char dash0[] = { 1, 1 };


/* X 軸切断 */
long xsec_expose_hdr
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( XEvent * , event   ),
       ( secRec * , sec     ))
{
  int i;
  long x, y, px, py, xx, yy;
  long xsize, ysize;

  xsize = __XSIZE( sec->img );
  ysize = __YSIZE( sec->img );

  XClearWindow( display, window );

  if ( sec->y == -1 ) return 0;

  XSetDashes( display, Xcil_black_gc, 0, dash0, 2 );
  XSetLineAttributes( display, Xcil_black_gc, 0, LineOnOffDash, 0, 0 );
  XDrawLine( display, window, Xcil_black_gc, sec->x, 0, sec->x, 255 );
  XSetLineAttributes( display, Xcil_black_gc, 0, LineSolid, 0, 0 );

  yy = ysize * sec->y / 256;
  py = y = 256 - 256 * __PIXEL(sec->img,0,yy,uchar) / ysize;
  for ( px = x = 0; x < 256; x++ )
    {
      xx = xsize * x / 256;

      y = 256 - 256 * __PIXEL(sec->img,xx,yy,uchar) / ysize;

      XDrawLine( display, window, Xcil_black_gc, px, py, x, py );
      XDrawLine( display, window, Xcil_black_gc, x, py, x, y );

      px = x;
      py = y;
    }

  return 0;
}



long ysec_expose_hdr
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( XEvent * , event   ),
       ( secRec * , sec     ))
{
  int i;
  long x, y, px, py, xx, yy;
  long xsize, ysize;

  xsize = __XSIZE( sec->img );
  ysize = __YSIZE( sec->img );

  XClearWindow( display, window );

  if ( sec->x == -1 ) return 0;

  XSetDashes( display, Xcil_black_gc, 0, dash0, 2 );
  XSetLineAttributes( display, Xcil_black_gc, 0, LineOnOffDash, 0, 0 );
  XDrawLine( display, window, Xcil_black_gc, 0, sec->y, 255, sec->y );
  XSetLineAttributes( display, Xcil_black_gc, 0, LineSolid, 0, 0 );

  xx = xsize * sec->x / 256;
  px = x = 256 * __PIXEL(sec->img,0,xx,uchar) / xsize;
  for ( py = y = 0; y < 256; y++ )
    {
      yy = ysize * y / 256;

      x = 256 * __PIXEL(sec->img,xx,yy,uchar) / xsize;

      XDrawLine( display, window, Xcil_black_gc, px, py, px, y );
      XDrawLine( display, window, Xcil_black_gc, px, y, x, y );

      px = x;
      py = y;
    }

  return 0;
}



long xsec_press_hdr
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( XEvent * , event   ),
       ( secRec * , sec     ))
{
  int x, y, i, j, total;
  char buf[ 512 ];

  x = __XSIZE( sec->img ) * event->xbutton.x / 256;

  if ( ( x < 0 ) || (x >= __XSIZE(sec->img) ) )
    return 0;

  switch ( event->xbutton.button )
    {
    case Button1:
      set_xy(display,sec->iwin,context,sec,x,sec->y);
      xsec_expose_hdr( display, sec->xwin, context, event, sec );
      ysec_expose_hdr( display, sec->ywin, context, event, sec );
      info_disp( display, context, sec );
      break;
    case Button2:
      break;
    case Button3:
      break;
    }

  return 0;
}



long ysec_press_hdr
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( XEvent * , event   ),
       ( secRec * , sec     ))
{
  int x, y, i, j, total;
  char buf[ 512 ];

  y = __YSIZE( sec->img ) * event->xbutton.y / 256;

  if ( ( y < 0 ) || (y >= __YSIZE(sec->img) ) )
    return 0;

  switch ( event->xbutton.button )
    {
    case Button1:
      set_xy(display,sec->iwin,context,sec,sec->x,y);
      xsec_expose_hdr( display, sec->xwin, context, event, sec );
      ysec_expose_hdr( display, sec->ywin, context, event, sec );
      info_disp( display, context, sec );
      break;
    case Button2:
      break;
    case Button3:
      break;
    }

  return 0;
}



long img_expose_hdr
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( XEvent * , event   ),
       ( secRec * , sec     ))
{
  XShowImageFull( display, window, Xcil_black_gc, sec->img );
  return 0;
}



long img_press_hdr
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( XEvent * , event   ),
       ( secRec *, sec      ))
{
  int x, y, i, j, total;
  char buf[ 512 ];

  x = __XSIZE( sec->img ) * event->xbutton.x / 256;
  y = __XSIZE( sec->img ) * event->xbutton.y / 256;

  if (( x < 0 ) || ( y < 0 ) ||
      (x >= __XSIZE(sec->img) ) || (y >= __YSIZE(sec->img) ) )
    return 0;

  switch ( event->xbutton.button )
    {
    case Button1:
      set_xy(display,window,context,sec,x,y);
      xsec_expose_hdr( display, sec->xwin, context, event, sec );
      ysec_expose_hdr( display, sec->ywin, context, event, sec );
      info_disp( display, context, sec );
      break;
    case Button2:
      break;
    case Button3:
      break;
    }

  return 0;
}



long img_key_hdr
  _P6 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( XEvent * , event   ),
       ( secRec * , sec     ),
       ( long     , ascii   ))
{
  int x, y;
  char buf[ 512 ];
  KeySym keysym;

  keysym = XLookupKeysym(event,0);

  if (( 0xFFE1 == keysym ) || ( 0xFFE2 == keysym ))
    if ( event->type == KeyRelease )
      sec->shift = 1;
    else
      sec->shift = 8;

  if ( event->type == KeyRelease ) return 0;

  x = sec->x;
  y = sec->y;
  switch (keysym)
    {
    case 0xff52: y = sec->y - sec->shift; break;
    case 0xff54: y = sec->y + sec->shift; break;
    case 0xff53: x = sec->x + sec->shift; break;
    case 0xff51: x = sec->x - sec->shift; break;
    default: return 0;
    }

  if ( x < 0 ) x = 0;
  if ( y < 0 ) y = 0;
  if ( x > 255 ) x = 255;
  if ( y > 255 ) y = 255;

  set_xy(display,sec->iwin,context,sec,x,y);

  xsec_expose_hdr( display, sec->xwin, context, event, sec );
  ysec_expose_hdr( display, sec->ywin, context, event, sec );
  info_disp( display, context, sec );

  return 0;
}



long new_image
  _P6 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( XEvent * , event   ),
       ( secRec *, sec      ),
       ( char *   , name    ))
{
  image img;

  if ( name == 0 || name[0] == '\0' ) return 0;

  img = Image.createFromFilename( name, name );
  if ( img == 0 )
    {
      fprintf(stderr,"can't open file (%s)\n",name );
      return 0;
    }

  if ( __TYPE( img ) != UChar )
    {
      fprintf( stderr, "bad image type (%s)\n", name );
      Image.destroy( img );
      return 0;
    }

  Image.destroy( sec->img );
  sec->img = img;
  histogram( img, sec->freq );

  window = XcilPanelItemWindow( display, sec->panel, context, "SCREEN" );
  XClearWindow( display, window );
  expose_hdr( display, window, context, event, sec );

  window = XcilPanelItemWindow( display, sec->panel, context, "XSEC" );
  XClearWindow( display, window );
  xsec_expose_hdr( display, window, context, event, sec );

  window = XcilPanelItemWindow( display, sec->panel, context, "YSEC" );
  XClearWindow( display, window );
  ysec_expose_hdr( display, window, context, event, sec );

  window = XcilPanelItemWindow( display, sec->panel, context, "IMAGE" );
  img_expose_hdr( display, window, context, event, sec );

  event->xbutton.x = sec->x * 256 / __XSIZE( sec->img );
  event->xbutton.y = sec->y * 256 / __XSIZE( sec->img );
  event->xbutton.button = Button1;
  sec->x = -1;
  sec->y = -1;
  img_press_hdr( display, window, context, event, sec );

  return 0;
}



void histogram
  _P2 (( image , in   ),
       ( long *, freq ))
{
  register int n, i;
  register uchar *data;
  long total;

  data = __RASTER( in, uchar );
  n = __AREA( in );

  for ( i = 0; i < 256; i++ ) freq[ i ] = 0;
  for ( i = 0; i < n; i++ ) freq[ *data++ ]++;
}



long expose_hdr
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( XEvent * , event   ),
       ( secRec *, sec    ))
{
  int i;
  long total, area, height, max;

  area = max = 0;
  for ( i = 0; i < 256; i++ )
    {
      area += sec->freq[ i ];
      if ( max < sec->freq[ i ] ) max = sec->freq[ i ];
    }

  total = 0;
  for ( i = 0; i < 256; i++ )
    {
      total += sec->freq[ i ];
      height = 230 * sec->freq[ i ] / max;
      if ( total )
	XDrawLine( display, window, Xcil_black_gc,
		   i, 255, i, 255 - 255 * total / area );
      if ( height )
	XFillRectangle( display, window, Xcil_invert_gc,
		        i, 255 - height, 1, height );
    }
  return 0;
}



long press_hdr
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( XEvent * , event   ),
       ( secRec *, sec    ))
{
  int i, j, total;
  char buf[ 512 ];

  i = event->xbutton.x;
  if ( i < 0 || i > 255 ) return 0;

  switch( event->xbutton.button )
    {
    case Button1:
      info_disp( display, context, sec );
      break;
    case Button2:
      break;
    case Button3:
      break;
    }

  return 0;
}



void xsec_disp
  _P1 (( image , img  ))
{
  Display *display;
  XContext context;
  Window panel, window;

  static XcilPanelItem list[] = {
    {{"TITLE"},                     {XCIL_3D_LABEL  ,"XY SECTION"}},
    {{"I",__PRE},                   {XCIL_3D_LABEL  ," I(***,***) = *** "}},
    {{"FREQ"  ,__PRE},	            {XCIL_3D_LABEL  ,"FREQ = *****"}},
    {{"SUM"   ,__PRE},              {XCIL_3D_LABEL  ,"SUM = *****"}},

    {{"QUIT"     ,__PRE},              {XCIL_3D_COMMAND,"Quit"},{Xcil_hdr_quit}},
    {{"FILE-T"   ,__TOP,__PRE},        {XCIL_3D_LABEL  ,"FILENAME:"}},
    {{"FILE"     ,__PRE,__SAM,423},    {XCIL_3D_LABEL}},
    {{"IMAGE"    ,__TOP,__PRE,256,256},{XCIL_3D_LABEL}},
    {{"YSEC"     ,__PRE,__SAM,256,256,-1},  {XCIL_3D_LABEL}},
    {{"XSEC"     ,__TOP,__PRE,256,256,0,-1},{XCIL_3D_LABEL}},
    {{"SCREEN"   ,__PRE,__SAM,256,256,-1,-1},{XCIL_3D_LABEL}},
  };
  secRec sec;

  display = XcilOpenDisplay(0);
  context = XUniqueContext();
  panel = XcilCreatePanelWindow( display, 0, context,
				 0, 0, list, XcilNumber(list), "XSECTION" );
  XSetWindowBackgroundPixmap( display, panel, Xcil_gray_xpixmap[ 1 ] );

  sec.img = img;
  sec.panel = panel;
  sec.shift = 1;
  sec.x = -1;
  sec.y = -1;
  histogram(img,sec.freq);

  window = XcilPanelItemWindow( display, panel, context, "SCREEN" );
  XcilAddCallback( display, window, context, Expose, expose_hdr, &sec );
  XcilAddCallback( display, window, context, ButtonPress, press_hdr, &sec );

  window = XcilPanelItemWindow( display, panel, context, "IMAGE" );
  XcilAddCallback( display, window, context, Expose, img_expose_hdr, &sec );
  XcilAddCallback( display, window, context, ButtonPress, img_press_hdr, &sec );
  XcilAddCallback( display, window, context, KeyPress  , img_key_hdr, &sec );
  XcilAddCallback( display, window, context, KeyRelease, img_key_hdr, &sec );
  XcilAddKeyCallback( display, window, context, 'q', Xcil_hdr_quit, 0 );
  sec.iwin = window;

  window = XcilPanelItemWindow( display, panel, context, "XSEC" );
  XcilAddCallback( display, window, context, Expose, xsec_expose_hdr, &sec );
  XcilAddCallback( display, window, context, ButtonPress, xsec_press_hdr, &sec );
  XcilAddCallback( display, window, context, KeyPress  , img_key_hdr, &sec );
  XcilAddCallback( display, window, context, KeyRelease, img_key_hdr, &sec );
  XcilAddKeyCallback( display, window, context, 'q', Xcil_hdr_quit, 0 );
  sec.xwin = window;

  window = XcilPanelItemWindow( display, panel, context, "YSEC" );
  XcilAddCallback( display, window, context, Expose, ysec_expose_hdr, &sec );
  XcilAddCallback( display, window, context, ButtonPress, ysec_press_hdr, &sec );
  XcilAddCallback( display, window, context, KeyPress  , img_key_hdr, &sec );
  XcilAddCallback( display, window, context, KeyRelease, img_key_hdr, &sec );
  XcilAddKeyCallback( display, window, context, 'q', Xcil_hdr_quit, 0 );
  sec.ywin = window;

  window = XcilPanelItemWindow( display, panel, context, "FILE" );
  XcilTextSet( display, window, context );
  XcilTextGetsReady( display, window, context, new_image, &sec );
  XcilTextSetInput( display, window, context, __NAME( sec.img ) );

  XcilAddKeyCallback( display, panel, context, 'q', Xcil_hdr_quit, 0 );

  XcilMapWindow( display, panel );

  XcilEventLoop( display, context );
}
