/*
 * filename : Xcil_menu.c
 * author   : Takahiro Sugiyama
 * date     : Thursday, January 19 1995
 * describe : Menu Bar
 */



#include "Xcil/Xcil.h"
#include "misc/memlib.h"


static XcilPanelItem menu_panel_item[MAX_MENU_NUM];
static long menu_panel_num;

static XcilMenu *Xcil_menu_list;
static long Xcil_menu_num;



#define MBHeight 20
#define MBItemHeight 18
#define ICON_WIDTH 16

extern XcilPanelItem *__XcilPanel_item
  P3 (( XcilPanelItem *, item ),
      ( long           , num  ),
      ( char *         , name ))

static long menu_open
  _P5 (( Display * , display ),
       ( Window    , window  ),
       ( XContext  , context ),
       ( XEvent *  , event   ),
       ( long      , mindex  ))
{
  long xsize, ysize;
  Window button;
  long index;
  XcilMenu *menu = &Xcil_menu_list[mindex];
  menu->index = -1;

  button = XcilPanelItemWindow( display, menu->panel, context, menu->label );
  index = XcilPanelItemIndex( display, menu->panel, context, menu->label );
  if ( Xcil_visual == PseudoColor )
    XSetFunction( display, Xcil_invert_gc, GXxor );
  __panel_draw( display, button, &menu_panel_item[index], XCIL_REVERSE );
  if ( Xcil_visual == PseudoColor )
    XSetFunction( display, Xcil_invert_gc, GXinvert );

  XRaiseWindow( display, menu->window );
  {
    XSetWindowAttributes xswa;
    long mask;
    mask = ( CWOverrideRedirect | CWBackingStore );
    xswa.override_redirect = True;
    xswa.backing_store = Always;
    XChangeWindowAttributes( display, menu->window, mask, &xswa );
    XMapWindow( display, menu->window );
  }
  XGrabPointer( display, menu->window, False,
	        ( ButtonReleaseMask | PointerMotionMask ),
	        GrabModeAsync, GrabModeAsync,
	        0, Xcil_left_cursor, 0 );
  return 0;
}



static __invertArea
  _P6 (( Display *, display ),
       ( Window   , window  ),
       ( long     , x       ),
       ( long     , y       ),
       ( long     , width   ),
       ( long     , height  ))
{
  if ( Xcil_visual == PseudoColor )
    XSetFunction( display, Xcil_invert_gc, GXxor );
  XFillRectangle( display, window, Xcil_invert_gc, x, y, width, height );
  if ( Xcil_visual == PseudoColor )
    XSetFunction( display, Xcil_invert_gc, GXinvert );
}

static long popup_up
  P5 (( Display * , display ),
      ( Window    , window  ),
      ( XContext  , context ),
      ( XEvent *  , event   ),
      ( long      , mindex  ));

static long popup_move
  _P5 (( Display * , display ),
       ( Window    , window  ),
       ( XContext  , context ),
       ( XEvent *  , event   ),
       ( long      , mindex  ))
{
  int mx, my;
  long x, y;
  long i, index, old_index;
  long xsize, ysize;
  XcilMenuItem *item;
  XcilMenu *menu = &Xcil_menu_list[mindex];

  mx = event->xbutton.x;
  my = event->xbutton.y;

  XcilGetWindowSize( display, menu->parent, &xsize, &ysize );
  if (( my < 0 ) && ( mx < 0 || mx >= xsize ))
    {
      popup_up( display, window, context, event, mindex );
      if (( mx < 0 ) && ( mindex > 0 ))
	menu_open( display, window, context, event, mindex - 1 );
      if (( mx >= xsize ) && ( mindex < Xcil_menu_num-1 ))
	menu_open( display, window, context, event, mindex + 1 );
      return 0;
    }

  XcilGetWindowSize( display, menu->window, &xsize, &ysize );
  if ( mx < 0 || mx >= xsize || my >= ysize )
    return popup_up( display, window, context, event, mindex );
  if ( my < 0 ) index = -1; else index = my / MBItemHeight;

  if ( index == menu->index ) return 0;

  old_index = menu->index;
  if (( old_index >= 0 ) && ( old_index < menu->item_num ) &&
      ( menu->item_list[old_index].state == MBI_ACTIVE ))
    {
      __invertArea( display, window, 0, old_index*MBItemHeight,
		    xsize, MBItemHeight );
    }

  if (( index >= 0 ) && ( index < menu->item_num ) &&
      ( menu->item_list[index].state == MBI_ACTIVE ))
    {
      __invertArea( display, window, 0, index*MBItemHeight,
		    xsize, MBItemHeight );
    }

  menu->index = index;

  return 0;
}

static long popup_redraw
  _P5 (( Display * , display ),
       ( Window    , window  ),
       ( XContext  , context ),
       ( XEvent *  , event   ),
       ( long      , index   ))
{
  int i;
  long xsize, ysize;
  XcilMenu *menu = &Xcil_menu_list[index];

/*XClearWindow( display, menu->window );*/
  XcilGetWindowSize( display, menu->window, &xsize, &ysize );
  for ( i = 0; i < menu->item_num; i++ )
    {
      long y = i*MBItemHeight;
      if ( menu->item_list[i].state == MBI_DELIMITER )
	{
	  XSetTile( display, Xcil_gray_gc, Xcil_pattern_pixmap[6] );
	  XFillRectangle( display, menu->window, Xcil_gray_gc,
			 0, y + MBItemHeight/2, xsize, 1 );
	  continue;
	}
      XcilDrawStringBox( display, menu->window, ICON_WIDTH, y,
			xsize, MBItemHeight, "lc", menu->item_list[i].label );
      if ( menu->item_list[i].mark == MBI_MARK )
	{
	  XcilDrawIcon16Box( display, menu->window, 0, y,
			  ICON_WIDTH, MBItemHeight, "cc", ICN16Check );
	}
      if ( menu->item_list[i].state == MBI_INACTIVE )
	{
	  XSetTile( display, Xcil_gray_gc, Xcil_pattern_pixmap[6] );
	  XSetFunction( display, Xcil_gray_gc, GXor );
	  XSetForeground( display, Xcil_gray_gc, Xcil_white_pixel );
	  XFillRectangle( display, window, Xcil_gray_gc, 0,
			  y, xsize, MBItemHeight );
	  XSetFunction( display, Xcil_gray_gc, GXcopy );
	  XSetForeground( display, Xcil_gray_gc, Xcil_black_pixel );
	}
    }

  return 0;
}


static long popup_up
  _P5 (( Display * , display ),
       ( Window    , window  ),
       ( XContext  , context ),
       ( XEvent *  , event   ),
       ( long      , mindex  ))
{
  long xsize, ysize;
  Window button;
  long index;
  XcilMenu *menu = &Xcil_menu_list[mindex];
  XcilMenuItem *item;
  xcil_callback callback = 0;
  caddr_t value;
  long state = MBI_INACTIVE;

  if (( menu->index >= 0 ) && ( menu->index < menu->item_num ))
    {
      item = &menu->item_list[ menu->index ];
      state = item->state;
      callback = item->callback;
      value = item->client_value;
    }

  button = XcilPanelItemWindow( display, menu->panel, context, menu->label );
  index = XcilPanelItemIndex( display, menu->panel, context, menu->label );

  if ( state == MBI_ACTIVE )
    {
      long xsize, ysize;
      Display *d = display;
      Window w = menu->window;
      GC gc = Xcil_invert_gc;
      long y = menu->index * MBItemHeight;
      XcilGetWindowSize( display, w, &xsize, &ysize );
      ysize = MBItemHeight;
      if ( item->blink )
	{
	  usleep( 100000 );
	  __invertArea( d, w, 0, y, xsize, ysize ); XSync( d, True );
	  usleep( 100000 );
	  __invertArea( d, w, 0, y, xsize, ysize ); XSync( d, True );
	  usleep( 100000 );
	  __invertArea( d, w, 0, y, xsize, ysize ); XSync( d, True );
	  usleep( 100000 );
	  __invertArea( d, w, 0, y, xsize, ysize ); XSync( d, True );
	  usleep( 100000 );
	}
      else
	usleep( 100000 );
      __invertArea( d, w, 0, y, xsize, ysize ); XSync( d, True );
    }

  XUnmapWindow( display, window );

  if (( state == MBI_ACTIVE ) && ( callback ))
    return callback( display, menu->window, context, event, value, item );

  __panel_draw( display, button, &menu_panel_item[index], XCIL_SIMPLE );

  return 0;
}



static void set_a_menu_popup
  _P8 (( Display * , display    ),
       ( Window    , grd_parent ),
       ( Window    , panel      ),
       ( XContext  , context    ),
       ( Window    , window     ),
       ( XcilMenu *, menu       ),
       ( XcilPanelItem *, item  ),
       ( long      , index      ))
{
  Window popup;
  long x, y, width, height;
  int i;
  long len, maxlen;

  maxlen = 0;
  for ( i = 0; i < menu->item_num; i++ )
    {
      len = strlength( menu->item_list[i].label );
      if ( len > maxlen ) maxlen = len;
      /* 各アイテムの初期化 */
      if ( strcmp(menu->item_list[i].label, "-" ) == 0 )
	menu->item_list[i].state = MBI_DELIMITER;
    }
  width = maxlen * 7 + 2*ICON_WIDTH;
  height = MBItemHeight * menu->item_num;
  x = item->x;
  y = MBHeight+3;
  popup = XcilCreateWindow( display, grd_parent, x, y, width, height, 1, "P" );
  XcilManagedWindow( display, popup, context );

  XcilAddCallback( display, window, context, ButtonPress, menu_open, index );
  XcilAddCallback( display, popup, context, MotionNotify, popup_move, index );
  XcilAddCallback( display, popup, context, Expose, popup_redraw, index );
  XcilAddCallback( display, popup, context, ButtonRelease, popup_up, index );

  menu->window = popup;
  menu->parent = window;
  menu->panel = panel;
  menu->index = -1;
}



static void set_menu_popup
  _P7 (( Display * , display   ),
       ( Window    , grd_parent ),
       ( Window    , panel     ),
       ( XContext  , context   ),
       ( long      , menu_num  ),
       ( XcilMenu *, menu_list ),
       ( XcilPanelItem *, panel_list ))
{
  int i;

  for ( i = 0; i < menu_num; i++ )
    {
      XcilMenu *menu = &menu_list[i];
      XcilPanelItem *item = &panel_list[i];

      Window window = XcilPanelItemWindow( display, panel, context,
					   menu->label );
      set_a_menu_popup( display, grd_parent,
		       panel, context, window, menu, item, i );
    }
}



static long set_menu_panel
  _P3 (( XcilPanelItem *, panel_list ),
       ( long           , menu_num   ),
       ( XcilMenu *     , menu_list  ))
{
  int i;
  int panel_num = 0;
  XcilPanelItem *a_item;
  XcilMenu *a_menu;

  for ( i = 0; i < menu_num; i++ )
    {
      a_item = &panel_list[panel_num++];
      a_menu = &menu_list[i];
      a_item->geo.name = a_menu->label;
      a_item->geo.left = (i == 0) ? __TOP : __PRE;
      a_item->geo.up = __TOP;
      a_item->geo.xsize = ( strlength( a_menu->label ) + 4 ) * 7;
      a_item->geo.ysize = MBHeight;
      a_item->geo.xspace = (i == 0) ? 50 : -1;
      a_item->geo.yspace = 1;
      a_item->geo.border = -1;
      a_item->act.action = XCIL_LABEL;
      a_item->act.string = a_menu->label;
      a_item->act.position = "cc";
      a_item->act.bitmap_data = 0;
      a_item->hdr.callback = 0;
      a_item->hdr.client_value = 0;
    }

  return panel_num;
}



#define MB_RADIUS 13
static long __menubar_redraw
  _P4 (( Display * , display ),
       ( Window    , menu    ),
       ( XContext  , context ),
       ( XEvent *  , event   ))
{
  static Pixmap corner = 0;
  long xsize, ysize;

  if ( corner == 0 )
    {
      corner = XCreatePixmap( display, menu,
			      MB_RADIUS*2, MB_RADIUS*2, Xcil_depth );
      XFillRectangle( display, corner, Xcil_black_gc,
		      0, 0, 2*MB_RADIUS, 2*MB_RADIUS );
      XFillArc( display, corner, Xcil_white_gc,
	        0, 0, 2*MB_RADIUS, 2*MB_RADIUS, 0, 360*64 );
    }
  XcilGetWindowSize( display, menu, &xsize, &ysize );
  XCopyArea( display, corner, menu, Xcil_black_gc, 0, 0, MB_RADIUS, MB_RADIUS,
	     0, 0 );
  XCopyArea( display, corner, menu, Xcil_black_gc,
	     MB_RADIUS, 0, MB_RADIUS, MB_RADIUS,
	     xsize-MB_RADIUS, 0 );
  return 0;
}

Window XcilCreateMenuBar
  _P5 (( Display * , display   ),
       ( Window    , parent    ),
       ( XContext  , context   ),
       ( long      , menu_num  ),
       ( XcilMenu *, menu_list ))
{
  long width, height;
  Window menu;
  XSetWindowAttributes xswa;

  width = DisplayWidth( display, 0 );

  Xcil_menu_list = menu_list;
  Xcil_menu_num  = menu_num;

  if ( parent == 0 ) parent = Xcil_root_window;
  menu_panel_num = set_menu_panel( menu_panel_item, menu_num, menu_list );
  menu = XcilCreatePanelWindow( display, parent, context, 0, 0,
			        menu_panel_item, menu_panel_num, "Menu" );
  set_menu_popup( display, parent, menu, context,
		  menu_num, menu_list, menu_panel_item );
  xswa.override_redirect = True;
  XChangeWindowAttributes( display, menu, CWOverrideRedirect, &xswa );
  XResizeWindow( display, menu, width, 22 );
  XcilAddEventMask( display, menu, ButtonPress );
  XcilAddCallback( display, menu, context, Expose, __menubar_redraw, 0 );
  __menubar_redraw( display, menu, context, 0 );

  return menu;
}
