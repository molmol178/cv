/*
 * filename : event.c
 * project  : Xcil Project
 * author   : Takahiro Sugiyama
 * date     : 1993/10/19
 */



#include "Xcil.h"
#include "misc/memlib.h"



long __Xcil_callback
  P3 (( Display *, display ),
      ( XContext , context ),
      ( XEvent * , event   ))



long Xcil_event_mask[ LASTEvent ] = {
  0,0,KeyPressMask,KeyReleaseMask,ButtonPressMask,ButtonReleaseMask,
  PointerMotionMask,EnterWindowMask,LeaveWindowMask,FocusChangeMask,
  FocusChangeMask,KeymapStateMask,ExposureMask,0,0,VisibilityChangeMask,
  SubstructureNotifyMask,StructureNotifyMask,StructureNotifyMask,
  StructureNotifyMask,SubstructureRedirectMask,StructureNotifyMask,
  StructureNotifyMask,SubstructureRedirectMask,StructureNotifyMask,
  ResizeRedirectMask,StructureNotifyMask,SubstructureRedirectMask,
  PropertyChangeMask,0,0,0,ColormapChangeMask,0
};



void XcilManagedWindow
  _P3 (( Display *  , display ),
       ( Window	    , window  ),
       ( XContext   , context ))
{
  XcilEventTable *tbl;

  if ( XFindContext( display, window, context, ( caddr_t * )&tbl ) == 0 )
    return;

  tbl = typenew1( 1, XcilEventTable );

  if ( XSaveContext( display, window, context, ( caddr_t )tbl ) != 0 )
    {
      fprintf( stderr, "XcilManagedWindow:error:can't save context data\n" );
      exit( -1 );
    }
}



void XcilUnmanagedWindow
  _P3 (( Display *  , display ),
       ( Window	    , window  ),
       ( XContext   , context ))
{
  int i;
  XcilEventTable *tbl;

  if ( XFindContext( display, window, context, ( caddr_t * )&tbl ) == 0 )
    {
      XDeleteContext( display, window, context );
      for ( i = 0; i < LASTEvent; i++ )
	{
	  typefree1( tbl->handler_list[ i ].callback_list );
	  typefree1( tbl->handler_list[ i ].client_value_list );
	}
      typefree1( tbl->key_callback_list );
      typefree1( tbl->key_client_value_list );
      typefree1( tbl->wc_list );
      typefree1( tbl );
    }
}



XcilEventTable *XcilGetEventTable
  _P3 (( Display *, display ),
       ( Window	  , window  ),
       ( XContext , context ))
{
  XcilEventTable *tbl;

  if ( XFindContext( display, window, context, ( caddr_t * )&tbl ) == 0 )
    return tbl;

  return 0;
}



void XcilAddEventMask
  _P3 (( Display *, display ),
       ( Window   , window  ),
       ( long     , mask    ))
{
  XWindowAttributes xwa;
  XSetWindowAttributes xswa;

  if ( XGetWindowAttributes( display, window, &xwa ) != 0 )
    {
      xswa.event_mask = xwa.your_event_mask | mask;
      XChangeWindowAttributes( display, window, CWEventMask, &xswa );
    }
}



void XcilDelEventMask
  _P3 (( Display *, display ),
       ( Window   , window  ),
       ( long     , mask    ))
{
  XWindowAttributes xwa;
  XSetWindowAttributes xswa;

  if ( XGetWindowAttributes( display, window, &xwa ) != 0 )
    {
      xswa.event_mask = xwa.your_event_mask & ~mask;
      XChangeWindowAttributes( display, window, CWEventMask, &xswa );
    }
}



void XcilAddCallback
  _P6 (( Display *    , display      ),
       ( Window	      , window       ),
       ( XContext     , context      ),
       ( long	      , type         ),
       ( xcil_callback, callback     ),
       ( caddr_t      , client_value ))
{
  XcilEventTable *tbl;
  XcilHandler *handler;
  int i, index;

  if ( XFindContext( display, window, context, ( caddr_t * )&tbl ) != 0 )
    {
      fprintf( stderr, "XcilAddCallback:auto managed window %d\n", window );
      XcilManagedWindow( display, window, context );
      XFindContext( display, window, context, ( caddr_t * )&tbl );
    }

  handler = &tbl->handler_list[ type ];

  for ( i = 0; i < handler->num; i++ )
    {
      if (( handler->callback_list[ i ] == callback ) &&
	  ( handler->client_value_list[ i ] == client_value ))
	{
	  fprintf( stderr, "XcilAddCallback:warning:same callback and same client value.\n" );
	  fprintf( stderr, "        event type = %d\n", type );
	  return;
	}
    }

  index = handler->num++;

  handler->callback_list = typerenew1( handler->callback_list, handler->num, xcil_callback );
  handler->client_value_list = typerenew1( handler->client_value_list, handler->num, caddr_t );

  handler->callback_list[ index ] = callback;
  handler->client_value_list[ index ] = client_value;

  XcilAddEventMask( display, window, Xcil_event_mask[ type ] );
}



void XcilAddCallbacks
  _P5 (( Display *     , display ),
       ( Window	       , window  ),
       ( XContext      , context ),
       ( XcilCallback *, list    ),
       ( long	       , num     ))
{
  int i;

  for ( i = 0; i < num; i ++ )
    {
      XcilCallback *e = &list[ i ];
      XcilAddCallback( display, window, context, e->type, e->callback, e->client_value );
    }
}



void XcilAddKeyCallbacks
  _P5 (( Display *     , display ),
       ( Window	       , window  ),
       ( XContext      , context ),
       ( XcilCallback *, list    ),
       ( long	       , num     ))
{
  int i;

  for ( i = 0; i < num; i ++ )
    {
      XcilCallback *e = &list[ i ];
      XcilAddKeyCallback( display, window, context,
		       e->type, e->callback, e->client_value );
    }
}



void XcilAddKeyCallback
  _P6 (( Display *    , display      ),
       ( Window	      , window       ),
       ( XContext     , context      ),
       ( long	      , ascii        ),
       ( xcil_callback, callback     ),
       ( caddr_t      , client_value ))
{
  XcilEventTable *tbl;

  if ( XFindContext( display, window, context, ( caddr_t * )&tbl ) != 0 )
    {
      fprintf( stderr, "XcilAddKeyCallback:auto managed window %d\n", window );
      XcilManagedWindow( display, window, context );
      XFindContext( display, window, context, ( caddr_t * )&tbl );
    }

  XcilAddEventMask( display, window, Xcil_event_mask[ KeyPress ] );

  if ( tbl->key_callback_list == 0 )
    {
      tbl->key_callback_list = typenew1( 128, xcil_callback );
      tbl->key_client_value_list = typenew1( 128, caddr_t );
    }

  tbl->key_callback_list[ ascii ] = callback;
  tbl->key_client_value_list[ ascii ] = client_value;
}



void XcilDelCallback
  _P5 (( Display *    , display  ),
       ( Window       , window   ),
       ( XContext     , context  ),
       ( long         , type     ),
       ( xcil_callback, callback ))
{
  int i;
  XcilEventTable *tbl;
  XcilHandler *handler;

  if ( XFindContext( display, window, context, ( caddr_t * )&tbl ) != 0 )
    return;

  handler = &tbl->handler_list[ type ];
  for ( i = 0; i < handler->num; i++ )
    if ( handler->callback_list[ i ] == callback )
      break;
  if ( i == handler->num ) return;

  for ( ; i < handler->num -1; i++ )
    {
      handler->callback_list[ i ] = handler->callback_list[ i + 1 ];
      handler->client_value_list[ i ] = handler->client_value_list[ i + 1 ];
    }
  handler->num--;
  handler->callback_list = typerenew1( handler->callback_list, handler->num, xcil_callback );
  handler->client_value_list = typerenew1( handler->client_value_list, handler->num, caddr_t );

  if ( handler->num == 0 )
    XcilDelEventMask( display, window, Xcil_event_mask[ type ] );
}



void XcilDelKeyCallback
  _P4 (( Display *, display ),
       ( Window	  , window  ),
       ( XContext , context ),
       ( long	  , ascii   ))
{
  XcilEventTable *tbl;

  if ( XFindContext( display, window, context, ( caddr_t * )&tbl ) != 0 )
    return;

  if ( tbl->key_callback_list == 0 ) return;

  tbl->key_callback_list[ ascii ] = 0;
  tbl->key_client_value_list[ ascii ] = 0;
}



long __Xcil_callback
  _P3 (( Display *, display ),
       ( XContext , context ),
       ( XEvent * , event   ))
{
  XcilEventTable *tbl;
  Window window;

  XcilHandler *handler;
  xcil_callback callback;
  caddr_t client_value;
  caddr_t server_value;
  long i;
  long state;

  window = event->xany.window;

  if ( XFindContext( display, window, context, (caddr_t *)&tbl ) != 0 )
    return 0;

  if ( ( event->type == Expose ) && ( event->xexpose.count != 0 ) )
    return 0;

  state = 0;
  server_value = 0;
  if ( event->type == KeyPress ) server_value = ( caddr_t )XcilAscii( event, 0 );

  if ( ( event->type == KeyPress ) && ( tbl->key_callback_list ) )
    {
      long ascii = ( long )server_value;

      callback = tbl->key_callback_list[ ascii ];
      client_value = tbl->key_client_value_list[ ascii ];
      if ( callback ) state += callback( display, window, context, event, client_value, server_value );
    }

  handler = &tbl->handler_list[ event->type ];
  for ( i = 0; i < handler->num; i++ )
    {
      callback = handler->callback_list[ i ];
      client_value = handler->client_value_list[ i ];
      if ( callback ) state += callback( display, window, context, event, client_value, server_value );
    }

  return state;
}



#include <X11/Xatom.h>

void __Xcil_system_callback
  _P3 (( Display *, display ),
       ( XContext , context ),
       ( XEvent * , event   ))
{
  Window window, child;
  XWMHints xwmh;
  XSetWindowAttributes xswa;
  XWindowAttributes xwa;
  XSizeHints xsh;
  int x, y;

  window = event->xany.window;

  switch ( event->type )
    {
    case MapNotify:
      xswa.backing_store = Always;
      XChangeWindowAttributes( display, window, CWBackingStore, &xswa );

      XTranslateCoordinates( display, window, Xcil_root_window,
			     0, 0, &x, &y, &child );
      xsh.flags = USPosition;
      xsh.x = x;
      xsh.y = y;
      XSetWMSizeHints( display, window, &xsh, XA_WM_SIZE_HINTS );
      break;

    case MappingNotify:
      XRefreshKeyboardMapping(&event);
      break;
    }
}



long XcilEventLoop
  _P2 (( Display *, display ),
       ( XContext , context ))
{
  XEvent event;
  long state = 0;

  while ( state == 0 )
    {
      XNextEvent( display, &event );

      __Xcil_system_callback( display, context, &event );

      state += __Xcil_callback( display, context, &event );
    }

  return state;
}



long XcilAscii
  _P2 (( XEvent *, event  ),
       ( KeySym *, keysym ))
{
  KeySym keysymRec;
  char string[ 10 ];

  if ( keysym == 0 ) keysym = &keysymRec;

  if ( XLookupString( event, string, 10, keysym, 0 ) == 1 )
    return string[ 0 ];

  return 0;
}



void XcilAddWindowContext
  _P5 (( Display *, display ),
       ( Window	  , window  ),
       ( XContext , context ),
       ( long	  , key     ),
       ( caddr_t  , wc      ))
{
  XcilEventTable *tbl;
  int i;

  if ( XFindContext( display, window, context, ( caddr_t * )&tbl ) != 0 )
    {
      fprintf( stderr, "XcilAddWindowContext:auto managed window %d\n", window );
      XcilManagedWindow( display, window, context );
      XFindContext( display, window, context, ( caddr_t * )&tbl );
    }

  for ( i = 0; i < tbl->wc_num; i++ )
    if ( tbl->wc_list[ i ].key == key )
      {
	fprintf( stderr, "XcilAddWindowContext:There is same key (%d).\n", key );
	break;
      }

  i = tbl->wc_num++;
  tbl->wc_list = typerenew1( tbl->wc_list, tbl->wc_num, XcilWC );
  tbl->wc_list[i].key     = key;
  tbl->wc_list[i].context = wc;
}



caddr_t XcilGetWindowContext
  _P4 (( Display *, display ),
       ( Window	  , window  ),
       ( XContext , context ),
       ( long	  , key     ))
{
  XcilEventTable *tbl;
  int i;

  if ( XFindContext( display, window, context, ( caddr_t * )&tbl ) == 0 )
    {
      for ( i = 0; i < tbl->wc_num; i++ )
	if ( tbl->wc_list[ i ].key == key )
	  return tbl->wc_list[ i ].context;
    }
  else
    fprintf( stderr, "XcilGetWindowContext:not managed window %d\n", window );

  return 0;
}



long XcilModalLoop
  _P3 (( Display *, display     ),
       ( XContext , context     ),
       ( XContext , sub_context ))
{
  XcilEventTable *tbl;
  Window window;
  XEvent event;
  long state = 0;

  while ( state == 0 )
    {
      XNextEvent( display, &event );

      window = event.xany.window;

      if ( XFindContext( display, window, context, (caddr_t *)&tbl ) != 0 )
	{
	  if ( event.type == Expose )
	    __Xcil_callback( display, sub_context, &event );
	  continue;
	}

      __Xcil_system_callback( display, context, &event );
      state = __Xcil_callback( display, context, &event );
    }

  return state;
}
