/*
 * filename : Xcil_scroll.c
 * project  : Xcil Project
 * author   : Takahiro Sugiyama
 * date     : 1993/10/27
 */



#include "Xcil.h"



#define XCIL_SB_VERT 0
#define XCIL_SB_HORI 1



typedef struct {
  long direction;

  struct {
    long xoffset;
    long yoffset;
    long xsize;
    long ysize;
  } total;

  struct {
    long total;
    long offset;       /* バーの現在の位置(左から右端まで) */
    long length;       /* バーの長さ */
    long view_length;  /* バーの長さ */
    long mouse_offset; /* マウスのオフセット */
    long step;	       /* バーの移動量 */
  } bar;

  xcil_sb_callback callback;
  caddr_t          client_value;

  long state;
} XcilScrollItem;



/*
|<------------------------------------------ total.xsize -------->|
|<------------------ bar.offset ----->|
|        |<-------- bar.length ------>|
|
+-----------------------------------------------------------------+ -
|        +----------------------------+                           | ^
|        |::::::::::::::::::::::::::::|                           | |
|        |::::::::::::::::::::::::::::|                           | total.ysize
|        |::::::::::::::::::::::::::::|                           | |
|        +----------------------------+                           | V
+-----------------------------------------------------------------+ -
                       /\ <... mouse cursor
                       II
                        |<----------->|
                             bar.mouse_offset

●マウスが入ると、左右カーソルになる。
●マウスの右を押すと、右カーソルになって右に bar.step 移動してイベント発生
●マウスの左を押すと、左カーソルになって左に bar.step 移動してイベント発生
●マウスの中を押すと、上カーソルになってその点に移動してイベント発生

●イベントのサーバ値は、位置を正規化したもの(double)。つまり、

		bar.offset / total.xsize
*/


static void __XcilScroll_size_set
  _P4 (( Display *       , display ),
       ( Window          , window  ),
       ( XContext        , context ),
       ( XcilScrollItem *, item    ))
{
  long xsize, ysize;

  XcilGetWindowSize( display, window, &xsize, &ysize );

  item->direction = ( xsize > ysize ) ? XCIL_SB_HORI : XCIL_SB_VERT;

  item->total.xoffset = 1;
  item->total.yoffset = 1;

  item->total.xsize = xsize - item->total.xoffset * 2;
  item->total.ysize = ysize - item->total.yoffset * 2;

  item->bar.length = 20;
  item->bar.view_length = 0;
  item->bar.offset = item->bar.length;
  item->bar.step = item->bar.length;

  item->bar.total = ( xsize > ysize ) ? item->total.xsize : item->total.ysize;
  item->bar.mouse_offset = 0;
}



static XcilScrollItem *__XcilScroll_item
  _P3 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ))
{
  XcilScrollItem *item;

  item = ( XcilScrollItem * )XcilGetWindowContext
			( display, window, context, XCIL_SCROLL_ITEM );
  if ( item == 0 )
    {
      fprintf( stderr,"error:__XcilScroll:This window can not use scroll.\n" );
      fprintf( stderr,"    You should use \"XcilScrollSet\".\n" );
      exit( -1 );
    }

  return item;
}



static void __XcilScrollBarDrawVert
  _P4 (( Display *       , display ),
       ( Window          , window  ),
       ( GC              , gc      ),
       ( XcilScrollItem *, item    ))
{
  long x, y, xsize, ysize;

  x = item->total.xoffset;
  xsize = item->total.xsize;
  if ( item->bar.length == 0 )
    {
      y = item->total.yoffset;
      ysize = item->bar.offset;
      if ( ysize < 2 ) ysize = 2;
    }
  else
    {
      y = item->total.yoffset + item->bar.offset - item->bar.length;
      ysize = item->bar.length;
      if ( y < item->total.yoffset )
	{
	  y = item->total.yoffset;
	  ysize = item->bar.offset;
	  if ( ysize < 2 ) ysize = 2;
	}
    }
  XFillRectangle( display, window, gc, x, y, xsize, ysize );

  XFlush( display );
}



static void __XcilScrollBarDrawHori
  _P4 (( Display *       , display ),
       ( Window          , window  ),
       ( GC              , gc      ),
       ( XcilScrollItem *, item    ))
{
  long x, y, xsize, ysize;

  y = item->total.yoffset;
  ysize = item->total.ysize;
  if ( item->bar.length == 0 )
    {
      x = item->total.xoffset;
      xsize = item->bar.offset;
      if ( xsize < 2 ) xsize = 2;
    }
  else
    {
      x = item->total.xoffset + item->bar.offset - item->bar.length;
      xsize = item->bar.length;
      if ( x < item->total.xoffset )
	{
	  x = item->total.xoffset;
	  xsize = item->bar.offset;
	  if ( xsize < 2 ) xsize = 2;
	}
    }

  XFillRectangle( display, window, gc, x, y, xsize, ysize );
  XFlush( display );
}



static void __XcilScrollBarDraw
  _P3 (( Display *       , display ),
       ( Window          , window  ),
       ( XcilScrollItem *, item    ))
{
  XSetTile( display, Xcil_gray_gc, Xcil_gray_xpixmap[ 1 ] );

  if ( item->direction == XCIL_SB_VERT )
    __XcilScrollBarDrawVert( display, window, Xcil_gray_gc, item );
  else
    __XcilScrollBarDrawHori( display, window, Xcil_gray_gc, item );
}



static void __XcilScrollBarClear
  _P3 (( Display *       , display ),
       ( Window          , window  ),
       ( XcilScrollItem *, item    ))
{
  if ( item->direction == XCIL_SB_VERT )
    __XcilScrollBarDrawVert( display, window, Xcil_white_gc, item );
  else
    __XcilScrollBarDrawHori( display, window, Xcil_white_gc, item );
}



static long expose_hdr
  _P5 (( Display *       , display ),
       ( Window          , window  ),
       ( XContext        , context ),
       ( XEvent *        , event   ),
       ( XcilScrollItem *, item    ))
{
  __XcilScrollBarDraw( display, window, item );

  return 0;
}



static long press_hdr
  _P5 (( Display *       , display ),
       ( Window          , window  ),
       ( XContext        , context ),
       ( XEvent *        , event   ),
       ( XcilScrollItem *, item    ))
{
  long x, y, offset;
  long state = 0;
  double server_value;

  x = event->xbutton.x - item->total.xoffset;
  y = event->xbutton.y - item->total.yoffset;
  offset = item->bar.offset;
  item->state = ButtonPress;

  switch( event->xbutton.button )
    {
    case Button1:
      if ( item->direction == XCIL_SB_VERT )
	XDefineCursor( display, window, Xcil_up_cursor );
      else
	XDefineCursor( display, window, Xcil_left_cursor );
      offset -= item->bar.step;
      break;

    case Button2:
      if ( item->direction == XCIL_SB_VERT )
	{
	  XDefineCursor( display, window, Xcil_left_cursor );
	  offset = y + item->bar.mouse_offset;
	}
      else
	{
	  XDefineCursor( display, window, Xcil_up_cursor );
	  offset = x + item->bar.mouse_offset;
	}
      break;

    case Button3:
      if ( item->direction == XCIL_SB_VERT )
	XDefineCursor( display, window, Xcil_down_cursor );
      else
	XDefineCursor( display, window, Xcil_right_cursor );
      offset += item->bar.step;
      break;
    }

  if ( offset < 0                     ) offset = 0;
  if ( offset < item->bar.view_length ) offset = item->bar.view_length;
  if ( offset > item->bar.total       ) offset = item->bar.total;

  if ( item->bar.offset == offset ) return 0;

  __XcilScrollBarClear( display, window, item );

  item->bar.offset = offset;

  __XcilScrollBarDraw( display, window, item );

  server_value = ( double )offset / item->bar.total;
  if ( item->callback )
    state = item->callback( display, window, context,
			    event, item->client_value, ( double )server_value );

  return state;
}



static long release_hdr
  _P5 (( Display *       , display ),
       ( Window          , window  ),
       ( XContext        , context ),
       ( XEvent *        , event   ),
       ( XcilScrollItem *, item    ))
{
  if ( item->direction == XCIL_SB_VERT )
    XDefineCursor( display, window, Xcil_up_down_cursor );
  else
    XDefineCursor( display, window, Xcil_right_left_cursor );
  item->state = ButtonRelease;

  return 0;
}



static long motion_hdr
  _P5 (( Display *       , display ),
       ( Window          , window  ),
       ( XContext        , context ),
       ( XEvent *        , event   ),
       ( XcilScrollItem *, item    ))
{
  long x, y, offset;
  double server_value;
  long state = 0;

  if ( item->state != ButtonPress ) return 0;
  if ( event->xbutton.button != Button2 ) return 0;

  x = event->xbutton.x - item->total.xoffset;
  y = event->xbutton.y - item->total.yoffset;
  offset = item->bar.offset;

  if ( item->direction == XCIL_SB_VERT )
    offset = y + item->bar.mouse_offset;
  else
    offset = x + item->bar.mouse_offset;

  if ( offset < 0                     ) offset = 0;
  if ( offset < item->bar.view_length ) offset = item->bar.view_length;
  if ( offset > item->bar.total       ) offset = item->bar.total;

  if ( item->bar.offset == offset ) return 0;

  __XcilScrollBarClear( display, window, item );

  item->bar.offset = offset;

  __XcilScrollBarDraw( display, window, item );

  server_value = ( double )offset / item->bar.total;
  if ( item->callback )
    state = item->callback( display, window, context,
			   event, item->client_value, ( double )server_value );

  return state;
}



void XcilScrollSet
  _P5 (( Display *       , display      ),
       ( Window          , window       ),
       ( XContext        , context      ),
       ( xcil_sb_callback, callback     ),
       ( caddr_t         , client_value ))
{
  XcilScrollItem *item;

  item = ( XcilScrollItem * )calloc( 1, sizeof( XcilScrollItem ) );

  XcilAddWindowContext
		( display, window, context, XCIL_SCROLL_ITEM, (caddr_t)item );

  XcilAddCallback( display, window, context,
		   Expose,        (xcil_callback)expose_hdr,  (caddr_t)item );
  XcilAddCallback( display, window, context,
		   ButtonPress,   (xcil_callback)press_hdr,   (caddr_t)item );
  XcilAddCallback( display, window, context,
		   ButtonRelease, (xcil_callback)release_hdr, (caddr_t)item );
  XcilAddCallback( display, window, context,
		   MotionNotify,  (xcil_callback)motion_hdr,  (caddr_t)item );

  item->callback     = callback;
  item->client_value = client_value;
  item->state = 0;

  __XcilScroll_size_set( display, window, context, item );

  if ( item->direction == XCIL_SB_VERT )
    XDefineCursor( display, window, Xcil_up_down_cursor );
  else
    XDefineCursor( display, window, Xcil_right_left_cursor );
}



void XcilScrollBarSetPosition
  _P4 (( Display *, display  ),
       ( Window   , window   ),
       ( XContext , context  ),
       ( double   , position ))
{
  XcilScrollItem *item;

  if ( position < 0.0 ) position = 0.0;
  if ( position > 1.0 ) position = 1.0;

  item = __XcilScroll_item( display, window, context );

  __XcilScrollBarClear( display, window, item );

  item->bar.offset = item->bar.total * position;

  __XcilScrollBarDraw( display, window, item );
}



void XcilScrollBarInc
  _P3 (( Display *, display  ),
       ( Window   , window   ),
       ( XContext , context  ))
{
  XcilScrollItem *item;
  XEvent event;

  item = __XcilScroll_item( display, window, context );
  event.type = ButtonPress;
  event.xbutton.x = 0;
  event.xbutton.y = 0;
  event.xbutton.button = Button3;

  press_hdr( display, window, context, &event, item );
}



void XcilScrollBarDec
  _P3 (( Display *, display  ),
       ( Window   , window   ),
       ( XContext , context  ))
{
  XcilScrollItem *item;
  XEvent event;

  item = __XcilScroll_item( display, window, context );
  event.type = ButtonPress;
  event.xbutton.x = 0;
  event.xbutton.y = 0;
  event.xbutton.button = Button1;

  press_hdr( display, window, context, &event, item );
}



void XcilScrollBarSetLength
  _P4 (( Display *, display     ),
       ( Window   , window      ),
       ( XContext , context     ),
       ( double   , length      ))
{
  XcilScrollItem *item;

  if ( length < 0.0 ) length = 0.0;
  if ( length > 1.0 ) length = 1.0;

  item = __XcilScroll_item( display, window, context );

  __XcilScrollBarClear( display, window, item );

  item->bar.length = item->bar.total * length;
  item->bar.step = item->bar.length;

  __XcilScrollBarDraw( display, window, item );
}



void XcilScrollBarSetViewLength
  _P4 (( Display *, display     ),
       ( Window   , window      ),
       ( XContext , context     ),
       ( double   , view_length ))
{
  XcilScrollItem *item;

  if ( view_length < 0.0 ) view_length = 0.0;
  if ( view_length > 1.0 ) view_length = 1.0;

  item = __XcilScroll_item( display, window, context );

  __XcilScrollBarClear( display, window, item );

  item->bar.view_length = item->bar.length * view_length;

  __XcilScrollBarDraw( display, window, item );
}



void XcilScrollBarSetMouseOffset
  _P4 (( Display *, display  ),
       ( Window   , window   ),
       ( XContext , context  ),
       ( double   , offset   ))
{
  XcilScrollItem *item;

  if ( offset < 0.0 ) offset = 0.0;
  if ( offset > 1.0 ) offset = 1.0;

  item = __XcilScroll_item( display, window, context );

  __XcilScrollBarClear( display, window, item );

  item->bar.mouse_offset = item->bar.length * offset;

  __XcilScrollBarDraw( display, window, item );
}



double XcilScrollBarGetPosition
  _P3 (( Display *, display  ),
       ( Window   , window   ),
       ( XContext , context  ))
{
  XcilScrollItem *item;
  double position;

  item = __XcilScroll_item( display, window, context );

  return ( double )item->bar.offset / item->bar.total;
}
