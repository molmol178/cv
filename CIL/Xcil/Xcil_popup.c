/*
 * Popup window
 */



#include "Xcil.h"
#include "misc/memlib.h"




typedef struct {

  Window parent;

  Window window;
  Window shadow;

  long xsize, ysize, height;

  XcilPopupMenuItem *list;
  long num;
  long index;

  XcilCallback *callback_list;

} XcilPopupWindowItem;




long Xcil_popup_shadow = 1;
long Xcil_popup_shadow_xoffset = 5;
long Xcil_popup_shadow_yoffset = 5;
long Xcil_popup_shadow_xsize = 3;
long Xcil_popup_shadow_ysize = 3;



static long Xcil_popup_motion
  _P6 (( Display *            , display ),
       ( Window               , window  ),
       ( XContext             , context ),
       ( XEvent *             , event   ),
       ( XcilPopupWindowItem *, item    ),
       ( long                 , server  ))
{
  long mx, my;
  long x, y;
  long i, index, old_index;
  XcilPopupMenuItem *menu;

  my = event->xbutton.y;
  mx = event->xbutton.x;

  if ( mx < 0 || mx >= item->xsize )
    {
      index = -1;
    }
  else
  for ( i = 0; i < item->num; i++ )
    {
      menu = &item->list[ i ];
      if ( ( menu->yoffset <= my ) &&
	   (( menu->yoffset + menu->height ) > my ) )
	{ index = i; break; }
    }

  if ( index == item->index ) return 0;

  old_index = item->index;

  if ( ( old_index >= 0 ) && ( old_index < item->num ) &&
       ( item->list[ old_index ].string != XcilPopupLINE ) &&
       ( item->list[ old_index ].callback != 0 ))
    {
      old_index = item->index;
      y = item->list[ old_index ].yoffset;
      XFillRectangle( display, window, Xcil_invert_gc,
		      1, y, item->xsize-2, item->list[ old_index ].height+1 );
    }

  if ( ( index >= 0 ) && ( index < item->num ) &&
       ( item->list[ index ].string != XcilPopupLINE ) &&
       ( item->list[ index ].callback != 0 ) )
    {
      y = item->list[ index ].yoffset;
      XFillRectangle( display, window, Xcil_invert_gc,
		      1, y, item->xsize-2, item->list[ index ].height+1 );
    }

  item->index = index;

  return 0;
}



static long Xcil_popup_release
  _P6 (( Display *            , display ),
       ( Window               , popup   ),
       ( XContext             , context ),
       ( XEvent *             , event   ),
       ( XcilPopupWindowItem *, item    ),
       ( long                 , server  ))
{
  xcil_popup_callback hdr;
  long state = 0;
  caddr_t client_value;
  XcilPopupMenuItem *menu;

  XUngrabPointer( display, 0 );
  if ( item->shadow ) XUnmapWindow( display, item->shadow );
  XUnmapWindow( display, popup );

  if ( ( item->index < 0 ) || ( item->index >= item->num ) ) return 0;

  menu = &item->list[ item->index ];
  if ( ( menu->string != XcilPopupLINE ) && ( menu->callback != 0 ) )
    {
      hdr = menu->callback;
      client_value = menu->client_value;
      state = hdr( display, item->parent, context, event,
		   client_value, item->index, item->list );
    }

  return state;
}



static long Xcil_popup_expose
  _P6 (( Display *            , display ),
       ( Window               , popup   ),
       ( XContext             , context ),
       ( XEvent *             , event   ),
       ( XcilPopupWindowItem *, item    ),
       ( long                 , server  ))
{
  int i, x, y, ysize;
  char *pos;
  XcilPopupMenuItem *menu;

  ysize = item->height;
  x = ysize / 2;

  for ( i = 0; i < item->num; i++ )
    {
      menu = &item->list[ i ];
      y = menu->yoffset;
      if ( menu->string != XcilPopupLINE )
	{
	  pos = ( menu->callback == 0 ) ? "cc" : "lc";
	  x   = ( menu->callback == 0 ) ?    0 : ( ysize / 2 );
	  XcilDrawStringBox( display, popup, x, y,
			     item->xsize, menu->height,
			     pos, menu->string );
	}
      else
	{
	  XDrawLine( display, popup, Xcil_black_gc,
		     0, y + menu->height / 2,
		     item->xsize - 1, y + menu->height / 2 );
	}
    }

  if ( ( item->index >= 0 ) && ( item->index < item->num ) &&
       ( item->list[ item->index ].string != XcilPopupLINE ) &&
       ( item->list[ item->index ].callback != 0 ) )
    {
      y = item->list[ i ].yoffset;
      XFillRectangle( display, popup, Xcil_invert_gc,
		      1, y, item->xsize-2, item->list[ i ].height+1 );
    }

  return 0;
}


long Xcil_hdr_popupmenu
  _P6 (( Display *            , display ),
       ( Window               , window  ),
       ( XContext             , context ),
       ( XEvent *             , event   ),
       ( XcilPopupWindowItem *, list    ),
       ( long                 , server  ))
{
  long x, y;
  XcilPopupWindowItem *item;

  item = &list[event->xbutton.button];

  if ( item->window == 0 ) return 0;

  item->index  = -1;

  x = event->xbutton.x_root - item->xsize / 2;
  y = event->xbutton.y_root - item->height / 5;

  XMoveWindow( display, item->window, x, y );
  if ( item->shadow )
    {
      XSetWindowBackground( display, item->shadow, Xcil_black_pixel );
      XMoveWindow( display, item->shadow,
		   x + Xcil_popup_shadow_xoffset,
		   y + Xcil_popup_shadow_yoffset );
    }

  XcilMapPopup( display, item->window );
  XRaiseWindow( display, item->window );

  if ( item->shadow )
    {
      Window stack[2];
      XcilMapPopup( display, item->shadow );
      stack[ 0 ] = item->window;
      stack[ 1 ] = item->shadow;
      XRestackWindows( display, stack, 2 );
    }

  XFlush( display );

  XGrabPointer( display, item->window, False,
	        ( ButtonReleaseMask | PointerMotionMask ),
	        GrabModeAsync, GrabModeAsync,
	        0, Xcil_left_cursor, 0 );

  return 0;
}



void XcilPopupSet
  _P6 (( Display *          , display ),
       ( Window             , window  ),
       ( XContext           , context ),
       ( long               , button  ),
       ( XcilPopupMenuItem *, list    ),
       ( long               , num     ))
{
  int i;
  Window popup;
  long x, y, xsize, ysize, height, width;
  XcilPopupWindowItem *item;

  static XcilCallback static_event_list[] = {
    { MotionNotify  , (xcil_callback)Xcil_popup_motion,  0 },
    { ButtonRelease , (xcil_callback)Xcil_popup_release, 0 },
    { Expose        , (xcil_callback)Xcil_popup_expose,  0 },
  };

  /*----------------------------------------------------------------*
   * すでにウインドウにポップアップが登録されているかチェックする。
   *----------------------------------------------------------------*/
  item = ( XcilPopupWindowItem * )XcilGetWindowContext
			( display, window, context, XCIL_POPUP_ITEM_LIST );
  if ( item == 0 )
    {/* はじめてのポップアップの登録 */
      item = ( XcilPopupWindowItem * )calloc( 4, sizeof(XcilPopupWindowItem) );
      XcilAddWindowContext
	( display, window, context, XCIL_POPUP_ITEM_LIST, (caddr_t)item );
      item[Button1].callback_list = typenew1( 3, XcilCallback );
      item[Button2].callback_list = typenew1( 3, XcilCallback );
      item[Button3].callback_list = typenew1( 3, XcilCallback );
    }

  xsize = 0;
  height = XcilTextHeight( 0 ) + 2;
  ysize = height / 4;
  for ( i = 0; i < num; i++ )
    {
      if ( list[ i ].string == XcilPopupEOM ) break;
      list[ i ].yoffset = ysize;
      if ( list[ i ].string != XcilPopupLINE )
	{
	  width = XcilTextWidth( list[ i ].string );
	  if ( width > xsize ) xsize = width;
	  list[ i ].height = height;
	  ysize += height;
	}
      else
	{
	  list[ i ].height = height / 2;
	  ysize += height / 2;
	}
    }
  num = i;
  x = DisplayWidth( display, 0 );
  y = DisplayHeight( display, 0 );
  ysize += height / 4;
  xsize = xsize + height;

  if ( item[button].parent == 0 )
    {/* はじめてのポップアップウインドウの作成 */
      popup = XcilCreateWindow( display, 0, x, y, xsize, ysize, 1, "Popup" );
      XcilManagedWindow( display, popup, context );
      if ( Xcil_popup_shadow )
	item[button].shadow = XcilCreateWindow
	  ( display, 0,
	   x + Xcil_popup_shadow_xoffset,
	   y + Xcil_popup_shadow_yoffset,
	   xsize + 2 + Xcil_popup_shadow_xsize - Xcil_popup_shadow_xoffset,
	   ysize + 2 + Xcil_popup_shadow_ysize - Xcil_popup_shadow_yoffset,
	   0, "S" );
      XcilAddCallback( display, window, context,
		      ButtonPress, (xcil_callback)Xcil_hdr_popupmenu,
		      (caddr_t)item );
      for ( i = 0; i < 3; i++ )
	{
	  item[button].callback_list[ i ] = static_event_list[ i ];
	  item[button].callback_list[ i ].client_value
					    = (caddr_t)(&item[button]);
	}
      XcilAddCallbacks( display, popup, context,
		       item[button].callback_list, 3 );
    }
  else
    {/* すでにポップアップウインドウが存在する */
      popup = item[button].window;
      XMoveResizeWindow( display, popup, x, y, xsize, ysize );
      if ( Xcil_popup_shadow )
	XMoveResizeWindow( display, item[button].shadow,
	   x + Xcil_popup_shadow_xoffset,
	   y + Xcil_popup_shadow_yoffset,
	   xsize + 2 + Xcil_popup_shadow_xsize - Xcil_popup_shadow_xoffset,
	   ysize + 2 + Xcil_popup_shadow_ysize - Xcil_popup_shadow_yoffset );
    }

  item[button].parent = window;
  item[button].window = popup;
  item[button].list   = list;
  item[button].num    = num;
  item[button].xsize  = xsize;
  item[button].ysize  = ysize;
  item[button].height = height;
  item[button].index  = -1;
}



void XcilMapPopup
  _P2 (( Display *, display ),
       ( Window   , popup   ))
{
  XSetWindowAttributes xswa;
  long mask;

  mask = ( CWOverrideRedirect | CWBackingStore );
  xswa.override_redirect = True;
  xswa.backing_store = NotUseful;
  XChangeWindowAttributes( display, popup, mask, &xswa );
  XMapWindow( display, popup );
  XFlush( display );
}
