/*
 * filename : Xcil_dialog.c
 * author   : Takahiro Sugiyama
 * date     : Tuesday, January 17 1995
 * describe : Display Dialog
 */



#include <stdlib.h>
#include "Xcil/Xcil.h"
#include "misc/filelib.h"
#include "misc/memlib.h"
#include "misc/strlib.h"



#define MAX_PANEL_NUM 16


/*----------------------------------------*
 * set_geo : パネルの位置情報を設定する。
 *----------------------------------------*/
static set_geo
  _P9 (( XcilPanelItem *, an_item ),
       ( char *         , name    ),
       ( char *         , left    ),
       ( char *         , up      ),
       ( long           , xspace  ),
       ( long           , yspace  ),
       ( long           , xsize   ),
       ( long           , ysize   ),
       ( long           , border  ))
{
  an_item->geo.name   = name;
  an_item->geo.left   = left;
  an_item->geo.up     = up;
  an_item->geo.xspace = xspace;
  an_item->geo.yspace = yspace;
  an_item->geo.xsize  = xsize;
  an_item->geo.ysize  = ysize;
  an_item->geo.border = border;
}



/*----------------------------------------*
 * set_act : パネルの動的情報を設定する。
 *----------------------------------------*/
static set_act
  _P4 (( XcilPanelItem *, an_item  ),
       ( char *         , action   ),
       ( char *         , string   ),
       ( char *         , position ))
{
  an_item->act.action   = action;
  an_item->act.string   = string;
  an_item->act.position = position;
}



/*----------------------------------------*
 * set_hdr : パネルの事象情報を設定する。
 *----------------------------------------*/
static set_hdr
  _P3 (( XcilPanelItem *, an_item      ),
       ( xcil_callback  , callback     ),
       ( caddr_t        , client_value ))
{
  an_item->hdr.callback = callback;
  an_item->hdr.client_value = client_value;
}



/*----------------------------------------------------*
 * button_callback : ボタンが押された時のコールバック
 *----------------------------------------------------*/
static long button_callback
  _P6 (( Display *     , display      ),
       ( Window        , window       ),
       ( XContext      , context      ),
       ( XEvent *      , event        ),
       ( long          , button_index ),
       ( XcilPanelItem *, item        ))
{
  XcilDialogAttributes *attribute;
  Window text;

  attribute = (XcilDialogAttributes *)XcilGetWindowContext
    ( display, item->parent, context, XCIL_DIALOG_PROPERTY );

  if ( attribute->default_answer )
    {
      text = XcilPanelItemWindow( display, item->parent, context, "text" );
      XcilTextGetInput( display, text, context,
		        attribute->text_returned, 512 );
    }
  attribute->button_returned = attribute->button_list[ button_index ];

  return button_index + 1;
}



/*-----------------------------------------------------------*
 * return_callback : リターンキーが押された時のコールバック
 *-----------------------------------------------------------*/
static long return_callback
  _P6 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( XEvent * , event   ),
       ( Window   , parent  ),
       ( char *   , log     ))
{
  Window text;
  XcilDialogAttributes *attribute;

  attribute = (XcilDialogAttributes *)XcilGetWindowContext
    ( display, parent, context, XCIL_DIALOG_PROPERTY );

  if ( attribute->default_answer )
    {
      text = XcilPanelItemWindow( display, parent, context, "text" );
      XcilTextGetInput( display, text, context, attribute->text_returned, 512 );
    }
  if ( attribute->default_button < 0 ) return 0;

  attribute->button_returned = attribute->button_list[ attribute->default_button ];

  return attribute->default_button + 1;
}



/*--------------------------------------*
 * Window size : ウィンドウサイズの定義
 *--------------------------------------*/
#define DDT_W 400
#define DDT_H 25

#define DDB_X 10
#define DDB_Y 10
#define DDB_W 30
#define DDB_H 25

#define DDB_BW 1
#define DDB_DBW 3



/*-----------------------------------*
 * init_panel : パネルを初期化する。
 *-----------------------------------*/
static void init_panel
  _P1 (( XcilPanelItem *, panel_list ))
{
  int i;

  for ( i = 0; i < MAX_PANEL_NUM; i++ )
    {
      panel_list[i].act.bitmap_data = 0;
      panel_list[i].act.decoration = 0;
      panel_list[i].act.body_gc = 0;
      panel_list[i].act.frame_gc = 0;
      panel_list[i].x = 0;
      panel_list[i].y = 0;
      panel_list[i].xsize = 0;
      panel_list[i].ysize = 0;
      panel_list[i].state = 0;
      panel_list[i].string = 0;
      panel_list[i].shadow = 0;
    }
}



/*----------------------------------------*
 * make_panels : パネルリストを作成する。
 *----------------------------------------*/
static long make_panels
  _P2 (( XcilDialogAttributes *, attribute     ),
       ( XcilPanelItem *       , panel_list    ))
{
  XcilPanelItem *an_item;
  long panel_num = 0;
  char name[16];
  long i;
  long nmax, ddb_w, xoff;
  char *x_top = __TOP;

  long button_num;

  /* パネルの初期化 */
  init_panel( panel_list );

  if (( attribute->icon32 > 0 ) && ( attribute->icon32 < ICN32Last))
    {
      an_item = &panel_list[panel_num++];
      set_geo( an_item, "icn3", __TOP, __TOP, 20, 20, 32, 32, -1 );
      set_act( an_item, XCIL_LABEL, 0, "cc" );
      set_hdr( an_item, 0, 0 );
      x_top = "icn3";
    }

  {/* メッセージパネル */
    static char mesname[25][12];
    static char buf[2048];
    char *head[256];
    int n, nbuf;

    strcopy(buf, attribute->message);
    nbuf = strlength(buf);
    n = 0;
    head[n] = buf;
    for (i = 0; i < nbuf; i++)
      if (buf[i] == '\n') { buf[i] = '\0'; n++; head[n] = buf + i + 1; }
    if (n <= 0)
      {
	an_item = &panel_list[panel_num++];
	set_geo( an_item, "mess", x_top, __TOP, 20, 20, 0, 0, -1 );
	set_act( an_item, XCIL_LABEL, attribute->message, "l" );
	set_hdr( an_item, 0, 0 );
      }
    else
      {
	for (i = 0; i <= n; i++)
	  {
	    sprintf(mesname[i],"mess%d", i);
	    an_item = &panel_list[panel_num++];
	    set_geo(an_item, mesname[i],
		    x_top, (i)?__PRE:__TOP, 20, (i)?1:20, 0, 0, -1);
	    set_act( an_item, XCIL_LABEL, head[i], "l" );
	    set_hdr( an_item, 0, 0 );
	  }
      }
  }
  
  if (( attribute->default_button >= attribute->button_num ) ||
      ( attribute->default_button < -1 )) attribute->default_button = -1;

  if ( attribute->default_answer )
    {
      an_item = &panel_list[panel_num++];
      set_geo( an_item, "text", x_top, __PRE, 20, 15, DDT_W, DDT_H, 1 );
      set_act( an_item, XCIL_LABEL, "", "l" );
      set_hdr( an_item, 0, 0 );
    }

  if ( attribute->button_num == 0 )
    {
      static char *button_list[] = { "Cancel", "Ok" };
      attribute->button_num = 2;
      attribute->button_list = button_list;
    }
  button_num = attribute->button_num;

  nmax = 0;
  for ( i = 0; i < button_num; i++ )
    {
      long nstr = strlength( attribute->button_list[i] );
      if ( nmax < nstr ) nmax = nstr;
    }
  ddb_w = (nmax + 2) * 8;
  xoff = (DDT_W + 2 + 40) - ((DDB_X + ddb_w + 2*DDB_BW)*( button_num - 1 )+20);
  if ( attribute->default_button == -1 )
    xoff = xoff - (ddb_w + 2*DDB_BW);
  else
    xoff = xoff - (ddb_w + 2*DDB_DBW);
  if ( xoff < 0 ) xoff = 20;

  for ( i = 0; i < button_num; i++ )
    {
      long border = ( i == attribute->default_button ) ? DDB_DBW : DDB_BW;
      long y = ( i == attribute->default_button ) ? DDB_Y-2 : DDB_Y;
      an_item = &panel_list[panel_num++];
      sprintf( name, "%02d", i );
      if ( i == 0 )
	set_geo( an_item, strnew(name), x_top, __PRE, xoff, y, ddb_w, DDB_H, border );
      else
	set_geo( an_item, strnew(name), __PRE, __SAM, DDB_X, y, ddb_w, DDB_H, border );
      set_act( an_item, XCIL_BUTTON, attribute->button_list[i], "cc" );
      set_hdr( an_item, button_callback, i );
    }

  return panel_num;
}



/*-------------------------------------------------------*
 * setup_local_window : ローカルウィンドウの設定をする。
 *-------------------------------------------------------*/
static void setup_local_window
  _P4 (( Display *             , display   ),
       ( Window	               , window    ),
       ( XContext              , context   ),
       ( XcilDialogAttributes *, attribute ))
{
  Window text;

  if ( attribute->default_answer )
    {/* テキストウィンドウの設定 */
      text = XcilPanelItemWindow( display, window, context, "text" );
      XcilTextSet( display, text, context );
      XcilTextGetsReady( display ,text, context, return_callback, window );
      XcilTextClear( display, text, context );
      XClearWindow( display, text );
      XcilTextSetInput( display, text, context, attribute->default_answer );
    }

  XcilAddKeyCallback( display, window, context, 0x0d, return_callback, window );
}



static long __draw_icon32
  _P5 (( Display *             , display   ),
       ( Window	               , window    ),
       ( XContext              , context   ),
       ( XEvent *              , event     ),
       ( XcilDialogAttributes *, attribute ))
{
  Window icn;
  icn = XcilPanelItemWindow( display, window, context, "icn3" );
  XcilDrawIcon32( display, icn, 0, 0, attribute->icon32 );
  return 0;
}


/*===========================================*
 * XcilDisplayDialog : ダイアログを表示する。
 *===========================================*/
long XcilDisplayDialog /* 押されたボタンのインデックス */
  _P6 (( Display *             , display   ),
       ( Window	               , parent    ),
       ( XContext              , context   ),
       ( long                  , x         ),
       ( long                  , y         ),
       ( XcilDialogAttributes *, attribute ))
{
  static XContext modal_context = 0;
  Window window;
  long button_returned; /* 押されたボタンのインデックス */

  XcilPanelItem panel_list[MAX_PANEL_NUM];
  long panel_num;

  if ( modal_context == 0 ) modal_context = XUniqueContext();

  panel_num = make_panels( attribute, panel_list );

  window = XcilCreatePanelWindow( display, parent, modal_context, x, y,
				  panel_list, panel_num, "Dialog" );
  XcilAddWindowContext( display, window, modal_context,
		       XCIL_DIALOG_PROPERTY, (caddr_t)attribute );
  if (( attribute->icon32 > 0 ) && ( attribute->icon32 < ICN32Last ))
    XcilAddCallback( display, window, modal_context, Expose, __draw_icon32,
		     attribute );
  setup_local_window( display, window, modal_context, attribute );

  XcilMapWindow( display, window );
  button_returned = XcilModalLoop( display, modal_context, context );

  XcilDestroyPanelWindow( display, window, modal_context );

  return button_returned - 1;
}
