/*
 * ヒストグラムを見る
 */



#include "Image.h"
#include "Xcil/Xcil.h"
#include "Xcil/XImage.h"
#include <stdio.h>



void histogram
  P2 (( image , in   ),
      ( long *, freq ))


void xhist_disp
  P1 (( image , img  ))


typedef struct {
  Window panel;
  image img;
  long freq[256];
} histRec;


void main( argc, argv )
     int argc;
     char *argv[];
{
  image in_image;
  char *in_file = "-";

  if ( argc < 1 )
    {
      fprintf(stderr,"Usage : hist [in]\n");
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

  xhist_disp( in_image );
}



void histogram
  _P2 (( image , in   ),
       ( register long *, freq ))
{
  register int n, i;
  register uchar *data;

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
       ( histRec *, hist    ))
{
  int i;
  long total, area, height, max;

  area = max = 0;
  for ( i = 0; i < 256; i++ )
    {
      area += hist->freq[ i ];
      if ( max < hist->freq[ i ] ) max = hist->freq[ i ];
    }

  total = 0;
  for ( i = 0; i < 256; i++ )
    {
      total += hist->freq[ i ];
      height = 200 * hist->freq[ i ] / max;
      XDrawLine( display, window, Xcil_black_gc,
		 i, 255, i, 255 - 255 * total / area );
      XFillRectangle( display, window, Xcil_invert_gc,
		      i, 255 - height, 1, height );
    }
  return 0;
}



void info_disp
  _P4 (( Display *, display ),
       ( XContext , context ),
       ( histRec *, hist    ),
       ( long     , i       ))
{
  int j, total;

  total = 0;
  for ( total = j = 0; j < i; j++ ) total += hist->freq[ j ];

  XcilSetString( display, hist->panel, context, "I",
		"I = %d\n", i );
  XcilSetString( display, hist->panel, context, "FREQ",
		"FREQ = %d\n", hist->freq[i] );
  XcilSetString( display, hist->panel, context, "SUM",
		"SUM = %d\n", total );
}



long press_hdr
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( XEvent * , event   ),
       ( histRec *, hist    ))
{
  int i, j, total;
  char buf[ 512 ];

  i = event->xbutton.x;
  if ( i < 0 || i > 255 ) return 0;

  switch( event->xbutton.button )
    {
    case Button1:
      info_disp( display, context, hist, i );
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
       ( histRec *, hist    ))
{
  XShowImageFull( display, window, Xcil_black_gc, hist->img );
  return 0;
}



long img_press_hdr
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( XEvent * , event   ),
       ( histRec *, hist    ))
{
  int x, y, i, j, total;
  char buf[ 512 ];

  x = __XSIZE( hist->img ) * event->xbutton.x / 256;
  y = __XSIZE( hist->img ) * event->xbutton.y / 256;

  if (( x < 0 ) || ( y < 0 ) ||
      (x >= __XSIZE(hist->img) ) || (y >= __YSIZE(hist->img) ) )
    return 0;

  i = __PIXEL( hist->img, x, y, uchar );

  switch ( event->xbutton.button )
    {
    case Button1:
      info_disp( display, context, hist, i );
      break;
    case Button2:
      break;
    case Button3:
      break;
    }

  return 0;
}



long new_image
  _P6 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( XEvent * , event   ),
       ( histRec *, hist    ),
       ( char *   , name    ))
{
  image img;

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

  Image.destroy( hist->img );
  histogram( img, hist->freq );
  hist->img = img;

  window = XcilPanelItemWindow( display, hist->panel, context, "SCREEN" );
  XClearWindow( display, window );
  expose_hdr( display, window, context, event, hist );

  window = XcilPanelItemWindow( display, hist->panel, context, "IMAGE" );
  img_expose_hdr( display, window, context, event, hist );

  return 0;
}



void xhist_disp
  _P1 (( image , img  ))
{
  Display *display;
  XContext context;
  Window panel, window;

  static XcilPanelItem list[] = {
    {{"TITLE"},          	    {XCIL_3D_LABEL  ,"X HISTOGRAM EXT"}},
    {{"I"    ,__PRE},	            {XCIL_3D_LABEL  ,"I = ***"}},
    {{"FREQ"  ,__PRE},	            {XCIL_3D_LABEL  ,"FREQ = *****"}},
    {{"SUM"   ,__PRE},              {XCIL_3D_LABEL  ,"SUM = *****"}},
    {{"QUIT"  ,__PRE},              {XCIL_3D_COMMAND,"Quit"},{Xcil_hdr_quit}},
    {{"FILE-T",__TOP,__PRE},        {XCIL_3D_LABEL  ,"FILENAME:"}},
    {{"FILE"  ,__PRE,__SAM,429},    {XCIL_3D_LABEL}},
    {{"SCREEN",__TOP,__PRE,256,256},{XCIL_3D_LABEL}},
    {{"IMAGE" ,__PRE,__SAM,256,256},{XCIL_3D_LABEL}},
  };
  histRec hist;

  display = XcilOpenDisplay(0);
  context = XUniqueContext();
  panel = XcilCreatePanelWindow( display, 0, context,
				 0, 0, list, XcilNumber(list), "XHIST-III" );
  XSetWindowBackgroundPixmap( display, panel, Xcil_gray_xpixmap[ 1 ] );

  histogram(img,hist.freq);
  hist.img = img;
  hist.panel = panel;

  window = XcilPanelItemWindow( display, panel, context, "SCREEN" );
  XcilAddCallback( display, window, context, Expose, expose_hdr, &hist );
  XcilAddCallback( display, window, context, ButtonPress, press_hdr, &hist );

  window = XcilPanelItemWindow( display, panel, context, "IMAGE" );
  XcilAddCallback( display, window, context, Expose, img_expose_hdr, &hist );
  XcilAddCallback( display, window, context, ButtonPress, img_press_hdr, &hist );

  window = XcilPanelItemWindow( display, panel, context, "FILE" );
  XcilTextSet( display, window, context );
  XcilTextGetsReady( display, window, context, new_image, &hist );
  XcilTextSetInput( display, window, context, __NAME( hist.img ) );

  XcilAddKeyCallback( display, panel, context, 'q', Xcil_hdr_quit, 0 );

  XcilMapWindow( display, panel );

  XcilEventLoop( display, context );
}
