#include "XRGB.h"
#include <stdio.h>



XcilPanelItem item[] = {
  {{"FN" ,__TOP,__TOP},{XCIL_LABEL,"FILENAME"}},
  {{"RFN",__PRE,__TOP},{XCIL_LABEL}},

  {{"CLOSE",__TOP,__PRE},{XCIL_COMMAND,"Close"},{Xcil_hdr_unmap_popup}},
  {{"QUIT" ,__PRE,__SAM},{XCIL_COMMAND,"Quit" },{Xcil_hdr_quit}},
};



void xrgb_open(rgb)
     XRGB *rgb;
{
  static char buf[128];

  rgb->display = XcilOpenDisplay();
  rgb->bg      = 0x0ffffff;
  rgb->context = XUniqueContext();
  rgb->gc = Xcil_black_gc;

  rgb->img = Image.createFromFilename( "XRGB", rgb->fname );
  if ( __TYPE( rgb->img ) != UChar3 )
    {
      fprintf( stderr, "error:Bad pixel type.\n" );
      exit( -1 );
    }

  sprintf( buf, "RGB:Dispersion:%s", rgb->fname );

  rgb->fwidth  = XcilTextWidth( 0 ) + 2;
  rgb->fheight = XcilTextHeight( 0 ) + 2;
  rgb->xsize   = 2 * (256 + rgb->fheight + 2);
  rgb->ysize   = 2 * (256 + rgb->fheight + 2);

  rgb->win = XcilCreateWindow(rgb->display,0,100,100,rgb->xsize,rgb->ysize,1,
			      buf);
  XcilManagedWindow( rgb->display, rgb->win, rgb->context );

  PI_string( item[1] ) = rgb->fname;
  rgb->menu = XcilCreatePanelWindow(rgb->display,0,rgb->context,0,0,
				    item,XcilNumber(item),"XRGB:MENU");
  XSetWindowBackgroundPixmap( rgb->display, rgb->menu, Xcil_gray_xpixmap[3] );
  PI_value( item[2] ) = rgb->menu;
  XcilMapWindow(rgb->display,rgb->win);
  XFlush(rgb->display);
}
