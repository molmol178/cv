/*
 * filename : Xcil_confirm.c
 * project  : Xcil Project
 * author   : Takahiro Sugiyama
 * date     : 1993/10/22
 */



#include "Xcil.h"



void __Xcil_position_fix
  _P5 (( Display *, display ),
       ( Window   , parent  ),
       ( Window   , window  ),
       ( long     , x       ),
       ( long     , y       ))
{
  long xsize, ysize;
  long parent_xsize, parent_ysize;

  XcilGetWindowSize( display, parent, &parent_xsize, &parent_ysize );
  XcilGetWindowSize( display, window, &xsize, &ysize );
  if ( x == 0 ) x = ( parent_xsize - xsize ) / 2;
  if ( y == 0 ) y = ( parent_ysize - ysize ) / 2;
  if ( x < 0 ) x = -x - xsize / 2 - Xcil_wm_xoffset;
  if ( y < 0 ) y = -y - ysize / 2 - Xcil_wm_yoffset;
  XMoveWindow( display, window, x, y );
  XFlush( display );
}



static long handler
  _P6 (( Display *, display	 ),
       ( Window   , window	 ),
       ( XContext , context	 ),
       ( XEvent * , event	 ),
       ( caddr_t  , client_value ), /* ユーザが指定した値 */
       ( caddr_t  , server_value )) /* サーバの値 */
{
  return ( long )client_value;
}



long XcilConfirm /* 確認したかどうか */
  _P7 (( Display *, d       ),
       ( Window   , p       ),
       ( XContext , subc    ),
       ( long     , x       ), /* if x == 0 then center */
       ( long     , y       ), /* if y == 0 then center */
       ( char *   , title   ),
       ( char *   , message ))
{
  Window w;
  long state;
  long xsize, ysize;

  static XContext c = 0;
  static XcilPanelItem list[] = {
    {{"0",  0,  0, 0, 25, 20, 20, -1 },
       {XCIL_LABEL}},
    {{"1",  0,"0", 0, 25, 20, 20, 2},
       {XCIL_BUTTON,"Confirm"},{handler,(caddr_t)1}},
    {{"1","1","0", 0, 25, 20, 20, 2},
       {XCIL_BUTTON,"Cancel" },{handler,(caddr_t)-1}},
  };

  PI_string( list[ 0 ] ) = message;

  if (Xcil_Japanese_message)
    {
      list[1].act.string = "確認";
      list[2].act.string = "取消";
    }

  if ( c == 0 ) c = XUniqueContext();

  w = XcilCreatePanelWindow( d, p, c, 0, 0, list, XcilNumber(list), title );
/*
  XSetWindowBackgroundPixmap( d, w, Xcil_gray_xpixmap[1] );
*/
  __Xcil_position_fix( d, p, w, x, y );

  XcilGetWindowSize( d, w, &xsize, &ysize );
  {
    long delta = ( xsize - 10 -
		   (PI_x(list[2])+list[2].xsize+PI_border(list[2])*2) ) / 2;
    if ( delta > 0 )
      {
	PI_x(list[1]) += delta;
	PI_x(list[2]) += delta;
	XMoveWindow( d, PI_id(list[1]), PI_x(list[1]), PI_y(list[1]) );
	XMoveWindow( d, PI_id(list[2]), PI_x(list[2]), PI_y(list[2]) );
      }
  }
  XcilMapWindow( d, w );

  state = XcilModalLoop( d, c, subc );
  state = ( state == 1 ) ? XCIL_CONFIRM : XCIL_CANCEL;

  XcilDestroyPanelWindow( d, w, c );

  return state;
}




static long close_hdr
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( XEvent * , event   ),
       ( Window   , panel   ))
{
  XcilPanelItem *list;

  list = ( XcilPanelItem * )XcilGetWindowContext
    ( display, panel, context, XCIL_PANEL_ITEM_LIST );

  XcilDestroyPanelWindow( display, panel, context );

  free( list );

  return 0;
}



static long expose_hdr
  _P5 (( Display *, display  ),
       ( Window   , window   ),
       ( XContext , context  ),
       ( XEvent * , event    ),
       ( char **  , messages ))
{
  int i, x, y, dy;
  long font_height;
  XcilPanelItem *item;

  item = ( XcilPanelItem * )XcilGetWindowContext
			( display, window, context, XCIL_PANEL_ITEM );
  __panel_redraw( display, window, context, event, item );

  i = 0;
  font_height = XcilTextHeight( 0 );
  dy = font_height + 2;

  x = font_height / 2;
  y = font_height / 2;
  for ( i = 0; messages[ i ]; i++ )
    {
      XcilPutStringBox( display, window, Xcil_black_gc,
		        x, y, 10, dy, "lc", messages[ i ] );
      y += dy;
    }

  return 0;
}



static long map_notify_hdr
  _P5 (( Display *, display  ),
       ( Window   , pwindow   ),
       ( XContext , context  ),
       ( XEvent * , event    ),
       ( char **  , messages ))
{
  int i, x, y, dy;
  long font_height;
  XcilPanelItem *item;
  Window window;

  window = XcilPanelItemWindow(display, pwindow, context, "message");

  item = ( XcilPanelItem * )XcilGetWindowContext
			( display, window, context, XCIL_PANEL_ITEM );
  __panel_redraw( display, window, context, event, item );

  i = 0;
  font_height = XcilTextHeight( 0 );
  dy = font_height + 2;

  x = font_height / 2;
  y = font_height / 2;
  for ( i = 0; messages[ i ]; i++ )
    {
      XcilPutStringBox( display, window, Xcil_black_gc,
		        x, y, 10, dy, "lc", messages[ i ] );
      y += dy;
    }

  return 0;
}



static long icon_expose_hdr
  _P5 (( Display *, display  ),
       ( Window   , window   ),
       ( XContext , context  ),
       ( XEvent * , event    ),
       ( long     , icon32   ))
{
  XcilDrawIcon32( display, window, 0, 0, icon32 );
  return 0;
}



static long icon_map_notify_hdr
  _P5 (( Display *, display  ),
       ( Window   , pwindow  ),
       ( XContext , context  ),
       ( XEvent * , event    ),
       ( long     , icon32   ))
{
  Window window;

  window = XcilPanelItemWindow(display, pwindow, context, "i");
  XcilDrawIcon32( display, window, 0, 0, icon32 );
  return 0;
}



/*=================================================*
 * XcilMessages : メッセージウインドウを表示する。
 *=================================================*/
char *Xcil_message_button_label = 0;
void XcilMessages
  _P7 (( Display *, display  ),
       ( Window   , parent   ),
       ( XContext , context  ),
       ( long     , x        ),
       ( long     , y        ),
       ( char *   , title    ),
       ( char **  , messages ))
{
  Window window, sub;
  XcilPanelItem *list;

  static XcilPanelItem list_static[] = {
    {{"0",  0,  0, 0, 0, 20, 20, 1},{XCIL_LABEL}},
    {{"1","0",  0, 0, 0, 20, 20, 2},{XCIL_COMMAND,"OK"},
       {(xcil_callback)close_hdr}},
    {{"message",  0,"0", 0, 0, 20, 20, 1},{XCIL_LABEL}},
  };

  if (Xcil_Japanese_message)
    {
      list_static[1].act.string = "はい";
    }

  if (Xcil_message_button_label)
    {
      list_static[1].act.string = Xcil_message_button_label;
    }
  
  list = ( XcilPanelItem * )calloc( 3, sizeof( XcilPanelItem ) );
  list[ 0 ] = list_static[ 0 ];
  list[ 1 ] = list_static[ 1 ];
  list[ 2 ] = list_static[ 2 ];

  PI_string( list[ 0 ] ) = title;
  XcilTextSize( messages, &PI_xsize( list[ 2 ] ), &PI_ysize( list[ 2 ] ) );

  window = XcilCreatePanelWindow
			( display, parent, context, x, y, list, 3, title );
/*
  XSetWindowBackgroundPixmap( display, window, Xcil_gray_xpixmap[ 1 ] );
*/
  PI_value( list[ 1 ] ) = ( caddr_t )window;

  sub = PI_id( list[ 2 ] );
  XcilAddCallback( display, sub, context,
		   Expose, ( xcil_callback )expose_hdr, ( caddr_t )messages );
  XcilAddCallback( display, window, context,
		   MapNotify, (xcil_callback)map_notify_hdr,
		  ( caddr_t )messages );
  XcilAddKeyCallback( display, window, context,
		      'q', ( xcil_callback )close_hdr, ( caddr_t )window );
  XcilAddKeyCallback( display, window, context,
		      0x0d, ( xcil_callback )close_hdr, ( caddr_t )window );

  XcilMapWindow( display, window );
}



/*===========================================================*
 * XcilErrorMessages : エラーメッセージウインドウを表示する。
 *===========================================================*/
char *Xcil_error_message_button_label = 0;
void XcilErrorMessages
  _P8 (( Display *, display  ),
       ( Window   , parent   ),
       ( XContext , context  ),
       ( long     , x        ),
       ( long     , y        ),
       ( char *   , title    ),
       ( char **  , messages ),
       ( long     , icon32   ))
{
  Window window, sub;
  XcilPanelItem *list;

  static XcilPanelItem list_static[] = {
    {{"i",   0,    0,32,32, 20, 20, -1},{XCIL_LABEL}},
    {{"0", "i",    0, 0,25, 20, 20,  1},{XCIL_LABEL}},
    {{"message", "i",__PRE, 0, 0, 40, 20, -1},{XCIL_LABEL}},
    {{"2", "i",__PRE,60,25, 20,  0,  2},{XCIL_BUTTON,"OK"},
       {(xcil_callback)close_hdr}},
  };

  if (Xcil_Japanese_message)
    {
      list_static[3].act.string = "確認";
    }

  if (Xcil_error_message_button_label)
    {
      list_static[3].act.string = Xcil_error_message_button_label;
    }
  
  list = ( XcilPanelItem * )calloc( 4, sizeof( XcilPanelItem ) );
  list[ 0 ] = list_static[ 0 ];
  list[ 1 ] = list_static[ 1 ];
  list[ 2 ] = list_static[ 2 ];
  list[ 3 ] = list_static[ 3 ];

  PI_string( list[ 1 ] ) = title;
  XcilTextSize( messages, &PI_xsize( list[ 2 ] ), &PI_ysize( list[ 2 ] ) );
  PI_yspace( list[ 3 ] ) = 20;
  PI_xspace( list[ 3 ] ) = PI_xsize( list[ 2 ] ) + 40 + 40 - (60 + 4);

  window = XcilCreatePanelWindow
			( display, parent, context, x, y, list, 4, title );
/*
  XSetWindowBackgroundPixmap( display, window, Xcil_gray_xpixmap[ 1 ] );
*/
  PI_value( list[ 3 ] ) = ( caddr_t )window;

  sub = PI_id( list[ 2 ] );
  XcilAddCallback( display, sub, context,
		   Expose, ( xcil_callback )expose_hdr, ( caddr_t )messages );
  XcilAddCallback( display, window, context,
		   MapNotify, (xcil_callback)map_notify_hdr, ( caddr_t )messages );
  XcilAddCallback( display, PI_id( list[ 0 ] ), context, Expose,
		   ( xcil_callback )icon_expose_hdr, ( caddr_t )icon32 );
  XcilAddCallback( display, window, context, MapNotify,
		  (xcil_callback)icon_map_notify_hdr, ( caddr_t )icon32 );
  XcilAddKeyCallback( display, window, context,
		      'q', ( xcil_callback )close_hdr, ( caddr_t )window );
  XcilAddEventMask( display, window, ButtonPress );

  {/* モーダルにする */
    XSetWindowAttributes xswa;
    xswa.override_redirect = True;
    XChangeWindowAttributes( display, window, CWOverrideRedirect, &xswa );
    XSetWindowBorderWidth( display, window, 5 );
  }

  XcilMapWindow( display, window );
}
