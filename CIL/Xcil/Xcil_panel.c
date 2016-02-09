/*
 * filename : panel.c
 * project  : Xcil Project
 * author   : Takahiro Sugiyama
 * date     : 1993/10/19
 */



#include "Xcil.h"
#include <string.h>
//#include <varargs.h>
#include <stdarg.h>


/*======================*
 * 省略された時の初期値
 *======================*/
long XcilPanel_default_border = 1;
long XcilPanel_default_xspace = 5;
long XcilPanel_default_yspace = 5;



#define BORDER_3D 2
#define SIDE_3D 4



#define _p_name(item)   (item)->geo.name
#define _p_left(item)   (item)->geo.left
#define _p_up(item)     (item)->geo.up
#define _p_xspc(item)   (item)->geo.xspace
#define _p_yspc(item)   (item)->geo.yspace
#define _p_xsize(item)  (item)->geo.xsize
#define _p_ysize(item)  (item)->geo.ysize
#define _p_border(item) (item)->geo.border

#define _p_act(item)      (item)->act.action
#define _p_pos(item)      (item)->act.position
#define _p_string(item)   (item)->act.string
#define _p_bm_data(item)  (item)->act.bitmap_data
#define _p_bm_xsize(item) (item)->act.bitmap_xsize
#define _p_bm_ysize(item) (item)->act.bitmap_ysize
#define _p_deco(item)     (item)->act.decoration
#define _p_bodyGC(item)   (item)->act.body_gc
#define _p_frameGC(item)  (item)->act.frame_gc
#define _p_pixmap(item)   (item)->act.pixmap

#define _p_hdr(item)      (item)->hdr.callback
#define _p_value(item)    (item)->hdr.client_value

#define _p_id(item)       (item)->id
#define _p_shadow(item)   (item)->shadow
#define _p_x(item)        (item)->x
#define _p_y(item)        (item)->y
#define _p_state(item)    (item)->state

#define _p_lookxsize(item) (item->xsize + _p_border(item)*2)
#define _p_lookysize(item) (item->ysize + _p_border(item)*2)




long __panel_leave
  P5 (( Display *      , d    ),
      ( Window         , w    ),
      ( XContext       , c    ),
      ( XEvent *       , e    ),
      ( XcilPanelItem *, item ))


long __panel_redraw
  P5 (( Display *      , d    ),
      ( Window         , w    ),
      ( XContext       , c    ),
      ( XEvent *	, e    ),
      ( XcilPanelItem *, item ))

XcilPanelItem *__XcilPanel_item
  _P3 (( XcilPanelItem *, item ),
       ( long		, num  ),
       ( char *		, name ))
{
  int i;

  for ( i = 0; i < num; i++ )
    if ( strcmp( name, _p_name( &item[ i ] ) ) == 0 )
      return &item[ i ];

  fprintf( stderr, "warning:__XcilPanel_item:can't find name %s\n", name );
  fprintf( stderr, "    num = %d\n", num );
  for ( i = 0; i < num; i++ )
    fprintf( stderr, "    name of item[%d] = \"%s\"\n", i, _p_name( &item[ i ] ));

  exit( -1 );
}



static long __draw_shadow
  _P3 (( Display *      , display ),
       ( XcilPanelItem *, item    ),
       ( long           , offset  ))
{
  if ( offset != item->shadow )
    XClearArea( display, item->parent,
	        item->x + item->shadow, item->y + item->shadow, 
	        _p_lookxsize(item), _p_lookysize(item), False );
  if ( offset )
    {
      item->shadow = offset;
      XFillRectangle( display, item->parent, Xcil_black_gc,
		      item->x + item->shadow, item->y + item->shadow, 
		      _p_lookxsize(item), _p_lookysize(item) );
    }

  return 0;
}


static long __shadow_expose
  _P4 (( Display *, display ),
       ( Window   , panel   ),
       ( XContext , context ),
       ( XEvent * , event   ))
{
  XcilPanelItem *list;
  long i, num;

  list = (XcilPanelItem *)XcilGetWindowContext
			( display, panel, context, XCIL_PANEL_ITEM_LIST );
  num  = (long)XcilGetWindowContext
			( display, panel, context, XCIL_PANEL_ITEM_NUM );

  for ( i = 0; i < num; i++ )
    __draw_shadow( display, &list[i], list[i].shadow );
  return 0;
}



static long __map_notify
  _P4 (( Display *, display ),
       ( Window   , panel   ),
       ( XContext , context ),
       ( XEvent * , event   ))
{
  XcilPanelItem *list;
  long i, num;

  list = (XcilPanelItem *)XcilGetWindowContext
			( display, panel, context, XCIL_PANEL_ITEM_LIST );
  num  = (long)XcilGetWindowContext
			( display, panel, context, XCIL_PANEL_ITEM_NUM );

  for ( i = 0; i < num; i++ )
    __panel_redraw( display, _p_id(&list[i]), context, event, &list[i] );

  return 0;
}



void __loc_panel_draw
  _P3 (( Display *      , d    ),
       ( Window         , w    ),
       ( XcilPanelItem *, item ))
{
  if ( _p_bm_data(item) )
    {
      if ( _p_pixmap(item) == 0 )
	_p_pixmap(item) = XCreatePixmapFromBitmapData
	  ( d, w, _p_bm_data(item), _p_bm_xsize(item), _p_bm_ysize(item),
	   Xcil_black_pixel, Xcil_white_pixel, Xcil_depth );

      XcilDrawPixmapBox( d, w, 0, 0, item->xsize, item->xsize,
			 _p_pos(item), _p_pixmap(item),
			 _p_bm_xsize(item), _p_bm_ysize(item) );
    }
  else
    {
      if ( _p_deco(item) )
	XcilPutStringBoxDeco( d, w, _p_bodyGC(item), _p_frameGC(item),
			      0, 0, item->xsize, item->ysize,
			      _p_pos(item), _p_deco(item), item->string );
      else
	XcilDrawStringBox( d, w, 0, 0, item->xsize, item->ysize,
			   _p_pos(item), item->string );
    }
}



void __loc_panel_clear
  _P3 (( Display *      , d    ),
       ( Window         , w    ),
       ( XcilPanelItem *, item ))
{
  XClearWindow( d, item->id );
  _p_state(item) = XCIL_CLEAR;
}



void __loc_panel_simple
  _P3 (( Display *      , d    ),
       ( Window         , w    ),
       ( XcilPanelItem *, item ))
{
  int i;

  switch( _p_state(item) )
    {
    case XCIL_REVERSE:
    case XCIL_3D_REVERSE:
    case XCIL_3D_ON_REV:
      __loc_panel_clear( d, item->id, item );
    case XCIL_CLEAR:
      __loc_panel_draw( d, item->id, item );
      break;
    case XCIL_FOCUS:
    case XCIL_3D_FOCUS:
      for ( i = 0; i < BORDER_3D; i++ )
	XDrawRectangle( d, item->id, Xcil_white_gc,
		        i, i, item->xsize-2*i-1, item->ysize-2*i-1 );
    case XCIL_3D_OFF:
    case XCIL_3D_ON:
    case XCIL_3D_SIMPLE:
    case XCIL_SIMPLE:
      break;
    }
  XFlush( d );

  _p_state(item) = XCIL_SIMPLE;
}



void __loc_panel_3d_simple
  _P3 (( Display *      , d    ),
       ( Window         , w    ),
       ( XcilPanelItem *, item ))
{
  __loc_panel_simple(d,w,item);
  XMoveWindow( d, item->id, item->x, item->y );
  __draw_shadow( d, item, 2 );
  XFlush( d );

  _p_state(item) = XCIL_3D_SIMPLE;
}



void __loc_panel_3d_reverse
  _P3 (( Display *      , d    ),
       ( Window         , w    ),
       ( XcilPanelItem *, item ))
{
  __loc_panel_3d_simple( d, w, item );
  XFillRectangle( d, item->id, Xcil_invert_gc,
		  1, 1, item->xsize-2, item->ysize-2 );
  XFlush( d );

  _p_state(item) = XCIL_3D_REVERSE;
}



void __loc_panel_reverse
  _P3 (( Display *      , d    ),
       ( Window         , w    ),
       ( XcilPanelItem *, item ))
{
  __loc_panel_simple( d, w, item );
  XFillRectangle( d, item->id, Xcil_invert_gc, 0,0, item->xsize, item->ysize );
  XFlush( d );

  _p_state(item) = XCIL_REVERSE;
}



void __loc_panel_3d_focus
  _P3 (( Display *      , d    ),
       ( Window         , w    ),
       ( XcilPanelItem *, item ))
{
  int i;

  __loc_panel_3d_simple( d, w, item );
  for ( i = 0; i < BORDER_3D; i++ )
    XDrawRectangle( d, item->id, Xcil_black_gc,
		    i, i, item->xsize-2*i-1, item->ysize-2*i-1 );
  XFlush( d );

  _p_state(item) = XCIL_3D_FOCUS;
}



void __loc_panel_focus
  _P3 (( Display *      , d    ),
       ( Window         , w    ),
       ( XcilPanelItem *, item ))
{
  int i;

  __loc_panel_simple( d, w, item );
  for ( i = 0; i < BORDER_3D; i++ )
    XDrawRectangle( d, item->id, Xcil_black_gc,
		    i, i, item->xsize-2*i-1, item->ysize-2*i-1 );
  XFlush( d );

  _p_state(item) = XCIL_FOCUS;
}



void __loc_panel_3d_on
  _P3 (( Display *      , d    ),
       ( Window         , w    ),
       ( XcilPanelItem *, item ))
{
  __loc_panel_3d_simple( d, w, item );
  XMoveWindow( d, item->id, item->x - 1, item->y - 1 );
  __draw_shadow( d, item, 3 );
  XFlush( d );

  _p_state(item) = XCIL_3D_ON;
}



void __loc_panel_3d_off
  _P3 (( Display *      , d    ),
       ( Window         , w    ),
       ( XcilPanelItem *, item ))
{
  __loc_panel_3d_simple( d, w, item );
  XMoveWindow( d, item->id, item->x + 1, item->y + 1 );
  __draw_shadow( d, item, 2 );
  XFlush( d );

  _p_state(item) = XCIL_3D_OFF;
}



void __loc_panel_3d_on_rev
  _P3 (( Display *      , d    ),
       ( Window         , w    ),
       ( XcilPanelItem *, item ))
{
  __loc_panel_3d_on( d, w, item );
  XFillRectangle( d, item->id, Xcil_invert_gc,
		  1, 1, item->xsize-2, item->ysize-2 );
  XFlush( d );

  _p_state(item) = XCIL_3D_ON_REV;
}



void __panel_draw
  _P4 (( Display *      , d     ),
       ( Window         , w     ),
       ( XcilPanelItem *, item  ),
       ( long           , state ))
{
  switch ( state )
    {
    case XCIL_NONE:
      break;
    case XCIL_CLEAR:
      __loc_panel_clear( d, w, item );
      break;

    case XCIL_SIMPLE:
      __loc_panel_simple( d, w, item );
      break;
    case XCIL_FOCUS:
      __loc_panel_focus( d, w, item );
      break;
    case XCIL_REVERSE:
      __loc_panel_reverse( d, w, item );
      break;

    case XCIL_3D_SIMPLE:
      __loc_panel_3d_simple( d, w, item );
      break;
    case XCIL_3D_FOCUS:
      __loc_panel_3d_focus( d, w, item );
      break;
    case XCIL_3D_REVERSE:
      __loc_panel_3d_reverse( d, w, item );
      break;
    case XCIL_3D_ON:
      __loc_panel_3d_on( d, w, item );
      break;
    case XCIL_3D_OFF:
      __loc_panel_3d_off( d, w, item );
      break;
    case XCIL_3D_ON_REV:
      __loc_panel_3d_on_rev( d, w, item );
      break;
    }
}



long __panel_redraw
  _P5 (( Display *      , d    ),
       ( Window         , w    ),
       ( XContext       , c    ),
       ( XEvent *	, e    ),
       ( XcilPanelItem *, item ))
{
  long state;

  state = _p_state(item);
  __loc_panel_clear( d, w, item );
  __panel_draw( d, w, item, state );

  return 0;
}



long __panel_press
  _P5 (( Display *      , d    ),
       ( Window         , w    ),
       ( XContext       , c    ),
       ( XEvent *	, e    ),
       ( XcilPanelItem *, item ))
{
  long dum, state = 0;

  if ( e->xbutton.button != Button1 ) return 0;

  __panel_draw( d, w, item, _p_act(item)[__PRESS] );

  return state;
}



long __panel_release
  _P5 (( Display *      , d    ),
       ( Window         , w    ),
       ( XContext       , c    ),
       ( XEvent *	, e    ),
       ( XcilPanelItem *, item ))
{
  long state = 0;
  long dum;

  if ( e->xbutton.button != Button1 ) return 0;

  if (( _p_hdr(item) ) && XcilMouseInWindow( d, item->id ) )
    state = _p_hdr(item)( d, item->id, c, e, _p_value(item), (caddr_t)item );

  if ( XFindContext( d, item->id, c, (caddr_t *)&dum ) == 0 )
    {
      __panel_draw( d, item->id, item, _p_act(item)[__RELEASE] );
      if ( XcilMouseInWindow( d, item->id ) == 0 )
	__panel_leave( d, item->id, c, e, item );
    }

  return state;
}



long __panel_enter
  _P5 (( Display *      , d    ),
       ( Window         , w    ),
       ( XContext       , c    ),
       ( XEvent *	, e    ),
       ( XcilPanelItem *, item ))
{
  __panel_draw( d, w, item, _p_act(item)[__ENTER] );

  return 0;
}



long __panel_leave
  _P5 (( Display *      , d    ),
       ( Window         , w    ),
       ( XContext       , c    ),
       ( XEvent *	, e    ),
       ( XcilPanelItem *, item ))
{
  __panel_draw( d, w, item, _p_act(item)[__LEAVE] );

  return 0;
}


Window XcilCreatePanelWindow
  _P8 (( Display *      , d      ),
       ( Window	        , parent ),
       ( XContext       , c      ),
       ( long	        , x      ),
       ( long	        , y 	 ),
       ( XcilPanelItem *, list	 ),
       ( long	        , num	 ),
       ( char *	        , title	 ))
{
  Window w;
  XcilPanelItem *item, *up, *left;
  int i;
  long xsize, ysize, xspace, yspace;

  /* default */
  for ( i = 0; i < num; i++ )
    {
      item = &list[ i ];

      if (_p_string(item))
	item->string = strcpy( (char *)malloc( strlen(_p_string(item)) + 1 ),
			       _p_string(item) );

      if (_p_left(item) == __SAM) _p_left(item) = (i) ? _p_left(&list[i-1]) : 0;
      if (_p_left(item) == __PRE) _p_left(item) = (i) ? _p_name(&list[i-1]) : 0;
      if (_p_up(item)   == __SAM) _p_up(item)   = (i) ? _p_up(&list[i-1]) : 0;
      if (_p_up(item)   == __PRE) _p_up(item)   = (i) ? _p_name(&list[i-1]) : 0;

      if ( _p_act(item) == 0 || ( ( long )strlen( _p_act(item) ) < 5 ))
	_p_act(item) = XCIL_LABEL;

      if (_p_border(item) == 0 ) _p_border(item) = XcilPanel_default_border;
      if (_p_border(item) < 0 ) _p_border(item) = 0;

      if (_p_xspc(item) == 0) _p_xspc(item) = XcilPanel_default_xspace;
      if (_p_yspc(item) == 0) _p_yspc(item) = XcilPanel_default_yspace;
    }

  /* size */
  for ( i = 0; i < num; i++ )
    {
      item = &list[ i ];

      if ( _p_xsize(item) == 0 )
	{
	  if ( _p_bm_data(item) )
	    item->xsize = _p_bm_xsize(item);
	  else
	    item->xsize = XcilTextWidth( item->string ) + 16;
	  item->xsize += 4;
	}
      else
	item->xsize = _p_xsize(item);

      if ( _p_ysize(item) == 0 )
	{
	  if ( _p_bm_data(item) )
	    item->ysize = _p_bm_ysize(item);
	  else
	    item->ysize = XcilTextHeight( item->string );
	  item->ysize += 4;
	}
      else
	item->ysize = _p_ysize(item);
    }

  /* position */
  xspace = yspace = 100000;
  for ( i = 0; i < num; i++ )
    {
      item = &list[ i ];

      if ( _p_left(item) == 0 )
	{
	  _p_x(item) = _p_xspc(item);
	  if ( xspace > _p_xspc(item) ) xspace = _p_xspc(item);
	}
      else
	{
	  left = __XcilPanel_item( list, num, _p_left(item) );
	  _p_x(item) = _p_xspc(item) + _p_x( left ) + _p_lookxsize( left );
	}

      if ( _p_up(item) == 0 )
	{
	  _p_y(item) = _p_yspc(item);
	  if ( yspace > _p_yspc(item) ) yspace = _p_yspc(item);
	}
      else
	{
	  up = __XcilPanel_item( list, num, _p_up(item) );
	  _p_y(item) = _p_yspc(item) + _p_y( up ) + _p_lookysize( up );
	}
    }

  /* panel window size */
  xsize = ysize = 0;
  for ( i = 0; i < num; i++ )
    {
      long loc_xsize, loc_ysize;
      item = &list[ i ];
      loc_xsize = _p_x(item) + _p_lookxsize(item) + xspace;
      loc_ysize = _p_y(item) + _p_lookysize(item) + yspace;
      if ( xsize < loc_xsize ) xsize = loc_xsize;
      if ( ysize < loc_ysize ) ysize = loc_ysize;
    }

  w = XcilCreateWindow( d, parent, x, y, xsize, ysize, 1, title );
  for ( i = 0; i < num; i++ )
    {
      item = &list[ i ];

      item->parent = w;

      _p_id(item) = XcilCreateWindow
	( d, w, _p_x(item), _p_y(item),
	 item->xsize, item->ysize, _p_border(item), _p_name(item) );
      if ( _p_act(item) == 0 || ( (long)strlen( _p_act(item) ) < 5 ) )
	_p_act(item) = XCIL_LABEL;

      _p_state(item) = _p_act(item)[__INITIAL];
      XcilManagedWindow( d, _p_id(item), c );
      XcilAddWindowContext( d, _p_id(item), c,
			    XCIL_PANEL_ITEM, ( caddr_t )item );
      XcilAddCallback( d, _p_id(item), c, Expose       ,
		       (xcil_callback)__panel_redraw , ( caddr_t )item );
      XcilAddCallback( d, _p_id(item), c, ButtonPress  ,
		       (xcil_callback)__panel_press  , ( caddr_t )item );
      XcilAddCallback( d, _p_id(item), c, ButtonRelease,
		       (xcil_callback)__panel_release, ( caddr_t )item );
      XcilAddCallback( d, _p_id(item), c, EnterNotify  ,
		       (xcil_callback)__panel_enter  , ( caddr_t )item );
      XcilAddCallback( d, _p_id(item), c, LeaveNotify  ,
		       (xcil_callback)__panel_leave  , ( caddr_t )item );
    }

  for ( i = 0; i < num; i++ )
    {
      item = &list[ i ];
      XMapRaised( d, _p_id(item) );
    }

  XcilManagedWindow( d, w, c );
  XcilAddWindowContext( d, w, c, XCIL_PANEL_ITEM_LIST, (caddr_t)list );
  XcilAddWindowContext( d, w, c, XCIL_PANEL_ITEM_NUM , (caddr_t)num  );
  XcilAddCallback( d, w, c, Expose,
		   (xcil_callback)__shadow_expose, ( caddr_t )item );

  XcilAddCallback( d, w, c, MapNotify,
		   (xcil_callback)__map_notify, ( caddr_t )item );

  return w;
}



void XcilSetString( Display *display, Window window, XContext context, char *name, 
		    char *format, ... )
{
  XcilPanelItem *list;
  XcilPanelItem *item;
  long num;

  char string[ 1024 ];
  va_list args;

  va_start( args, format );
  vsprintf( string, format, args );
  va_end( args );

  list = ( XcilPanelItem * )XcilGetWindowContext( display, window, context,
						  XCIL_PANEL_ITEM_LIST );
  num  = ( long )XcilGetWindowContext( display, window, context,
				       XCIL_PANEL_ITEM_NUM );

  item = __XcilPanel_item( list, num, name );
  if ( item == 0 )
    {
      fprintf( stderr, "warning:XcilSetLabel:no name %s\n", name );
      return;
    }

  if ( item->string ) free( item->string );
  item->string = strcpy( ( char * )malloc( strlen( string ) + 1), string );
  __panel_redraw(display,_p_id(item),context,0,item);
}



void XcilDestroyPanelWindow
  _P3 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ))
{
  XcilPanelItem *list;
  long i, num;

  list = (XcilPanelItem *)XcilGetWindowContext(d,w,c,XCIL_PANEL_ITEM_LIST);
  num  = (long)XcilGetWindowContext( d, w, c, XCIL_PANEL_ITEM_NUM );

  for ( i = 0; i < num; i++ ) XcilUnmanagedWindow( d, list[i].id, c );

  XcilUnmanagedWindow( d, w, c );
  XUnmapWindow( d, w );
  XDestroySubwindows( d, w );
  XDestroyWindow( d, w );
}



Window XcilPanelItemWindow
  _P4 (( Display *, d    ),
       ( Window   , w    ),
       ( XContext , c    ),
       ( char *   , name ))
{
  XcilPanelItem *list;
  XcilPanelItem *item;
  long num;

  list = (XcilPanelItem *)XcilGetWindowContext(d,w,c,XCIL_PANEL_ITEM_LIST);
  num  = (long)XcilGetWindowContext( d, w, c, XCIL_PANEL_ITEM_NUM );

  item = __XcilPanel_item( list, num, name );
  if ( item == 0 )
    {
      fprintf(stderr,"warning:XcilSetLabel:no name %s\n",name);
      return 0;
    }

  return item->id;
}



long XcilPanelItemIndex
  _P4 (( Display *, display ),
       ( Window   , panel   ),
       ( XContext , context ),
       ( char *   , name    ))
{
  XcilPanelItem *list;
  long i, num;

  list = (XcilPanelItem *)XcilGetWindowContext
			( display, panel, context, XCIL_PANEL_ITEM_LIST );
  num  = (long)XcilGetWindowContext
			( display, panel, context, XCIL_PANEL_ITEM_NUM );

  for ( i = 0; i < num; i++ )
    if ( strcmp( name, _p_name( &list[ i ] ) ) == 0 )
      return i;

  fprintf( stderr, "error:XcilPanelItemIndex:can't find name %s\n", name );
  exit( -1 );
}
