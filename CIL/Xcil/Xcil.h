/*
 * filename : Xcil.h
 * author   : Takahiro Sugiyama
 * date     : 1993/10/19
 */



#ifndef __Xcil_h
#define __Xcil_h



#include "../misc/comacros.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/cursorfont.h>
#include <stdio.h>
#include <stdlib.h>



/*********************************
 * 型定義＆マクロ定義のセクション
 *********************************/



/*=============================*
 * XcilNumber : 配列の要素の数
 *=============================*/
#define XcilNumber( arr ) ( sizeof( arr ) / sizeof( arr[ 0 ] ) )



/*=======================================*
 * xcil_callback : イベントコールバック
 *=======================================*/
typedef long (*xcil_callback)
     P6 (( Display *, display	   ),
	 ( Window   , window	   ),
	 ( XContext , context	   ),
	 ( XEvent * , event	   ),
	 ( caddr_t  , client_value ), /* ユーザが指定した値 */
	 ( caddr_t  , server_value )) /* サーバの値 */



/*-----------------------------------*
 * XcilWC : ウインドウ・コンテキスト
 *-----------------------------------*/
typedef struct {
  long    key;     /* キー         */
  caddr_t context; /* コンテキスト */
} XcilWC;



/*-----------------------------------------------------*
 * XcilEventTable : ウインドウ・イベント・コンテキスト
 *-----------------------------------------------------*/
typedef struct {
  long          num;
  xcil_callback *callback_list;
  caddr_t       *client_value_list;
} XcilHandler;

typedef struct {
  XcilHandler    handler_list[ LASTEvent ];
  xcil_callback *key_callback_list;
  caddr_t	*key_client_value_list;
  long		 wc_num;
  XcilWC *	 wc_list;
} XcilEventTable;



/*-------------------------*
 * XCIL WINDOW CONTEXT KEY
 *-------------------------*/
enum {
 XCIL_PANEL_ITEM,
 XCIL_PANEL_ITEM_LIST,
 XCIL_PANEL_ITEM_NUM,

 XCIL_TEXT_ITEM,
 XCIL_SCROLL_ITEM,
 XCIL_POPUP_ITEM_LIST,

 XCIL_DIALOG_PROPERTY,
};



/*====================================*
 * XcilCallback : イベント・コールバック
 *====================================*/
typedef struct {
  long		type;
  xcil_callback	callback;
  caddr_t	client_value;
} XcilCallback;



/*----------------------------*
 * X CIL WINDOW BUTTON STATES
 *----------------------------*/
#define __INITIAL 0
#define __ENTER   1
#define __PRESS   2
#define __RELEASE 3
#define __LEAVE   4

#define XCIL_NONE    '.'
#define XCIL_CLEAR   'c'

#define XCIL_SIMPLE  's'
#define XCIL_FOCUS   'f'
#define XCIL_REVERSE 'r'

#define XCIL_3D_SIMPLE  'S'
#define XCIL_3D_FOCUS   'F'
#define XCIL_3D_REVERSE 'R'
#define XCIL_3D_ON      '+'
#define XCIL_3D_OFF     '-'
#define XCIL_3D_ON_REV  '*'



/*================================*
 * X CIL WINDOW BUTTON ACTION IDS
 *================================*/

/*
 * 平面ボタンの推奨値
 */
#define XCIL_LABEL	"s...."
#define XCIL_COMMAND	"sfrfs"
#define XCIL_BUTTON	"s.rs."

/*
 * 立体ボタンの推奨値
 */
#define XCIL_3D_LABEL	XCIL_LABEL1
#define XCIL_3D_COMMAND	XCIL_COMMAND1
#define XCIL_3D_BUTTON	XCIL_BUTTON2

/*
 * 普通は使わない
 */
#define XCIL_LABEL1	"S...."
#define XCIL_LABEL2	"F...."
#define XCIL_LABEL3	"R...."
#define XCIL_LABEL4	"+...."
#define XCIL_LABEL5	"-...."
#define XCIL_LABEL6	"*...."

#define XCIL_COMMAND1	"S+-+S"
#define XCIL_COMMAND2	"SF-FS"
#define XCIL_COMMAND3	"SR-RS"
#define XCIL_COMMAND4	"+*-*+"
#define XCIL_COMMAND5	"-SRS-"

#define XCIL_BUTTON1	"S.-S."
#define XCIL_BUTTON2	"+.-+."
#define XCIL_BUTTON3	"S.RS."
#define XCIL_BUTTON4	"-.+-."
#define XCIL_BUTTON5	"-.S-."



/*---------------------------------------------------*
 * XcilGeometryItem : ウインドウ・ボタンの位置の情報
 *---------------------------------------------------*/
#define __TOP 0
#define __SAM (char *)1
#define __PRE (char *)2

typedef struct {
  char *	name;   /*     ウインドウ名 */
  char *	left;   /* 左のウインドウ名 */
  char *	up;     /* 上のウインドウ名 */

  long		xsize;	/* if == 0 then auto size */
  long		ysize;	/* if == 0 then auto size */
  long		xspace;
  long		yspace;
  long		border;
} XcilGeometryItem;

/*-------------------------------------------------------*
 * XcilActionItem : ウインドウ・ボタンのアクションの情報
 *-------------------------------------------------------*/
typedef struct {
  char *	action;
  char *	string;

  char *	position;
  char *	bitmap_data;
  long		bitmap_xsize;
  long		bitmap_ysize;

  char *decoration;
  GC	body_gc;
  GC	frame_gc;

  Pixmap	pixmap;
} XcilActionItem;

typedef struct {
  xcil_callback	callback;
  caddr_t       client_value;
} XcilHandlerItem;

/*==============================*
 * XcilPanelItem : パネルの情報
 *==============================*/
typedef struct {
  XcilGeometryItem geo;
  XcilActionItem   act;
  XcilHandlerItem  hdr;

  /* return value */
  Window        parent;
  Window	id;
  long		x;
  long		y;
  long		xsize;
  long		ysize;
  long		state;
  char *	string;
  long          shadow;
} XcilPanelItem;

/*------------*
 * 参照マクロ
 *------------*/
#define PI_name(item) (item).geo.name
#define PI_left(item) (item).geo.left
#define PI_up(item)   (item).geo.up
#define PI_xspace(item) (item).geo.xspace
#define PI_yspace(item) (item).geo.yspace
#define PI_xsize(item)  (item).geo.xsize
#define PI_ysize(item)  (item).geo.ysize
#define PI_border(item) (item).geo.border

#define PI_action(item)   (item).act.action
#define PI_position(item) (item).act.position
#define PI_string(item)   (item).act.string
#define PI_bm_data(item)  (item).act.bitmap_data
#define PI_bm_xsize(item) (item).act.bitmap_xsize
#define PI_bm_ysize(item) (item).act.bitmap_ysize
#define PI_deco(item)     (item).act.decoration
#define PI_bodyGC(item)   (item).act.body_gc
#define PI_frameGC(item)  (item).act.frame_gc
#define PI_pixmap(item)   (item).act.pixmap

#define PI_handler(item)  (item).hdr.callback
#define PI_value(item)    (item).hdr.client_value

#define PI_id(item)       (item).id
#define PI_x(item)        (item).x
#define PI_y(item)        (item).y
#define PI_state(item)    (item).state



/*
 * Ｘウインドウの情報
 */
extern Display *Xcil_display;
extern Window Xcil_root_window;
extern long Xcil_depth;
extern long Xcil_plane;
extern long Xcil_visual;
extern Colormap Xcil_cmap;

/*
 * ＧＣの情報
 */
extern GC Xcil_black_gc;
extern GC Xcil_white_gc;
extern GC Xcil_gray_gc;
extern GC Xcil_invert_gc;
extern GC Xcil_or_gc;

/*
 * フォント情報
 */
extern XFontStruct *Xcil_font_struct;
extern XFontStruct *Xcil_kanji_struct;

/*
 * 濃淡を表すビットマップ
 */
extern Pixmap Xcil_gray_xpixmap[5];
extern Pixmap Xcil_gray_ypixmap[5];
extern unsigned long Xcil_black_pixel;
extern unsigned long Xcil_white_pixel;

/*
 * イベントとイベントマスクの関係
 */
extern long Xcil_event_mask[ LASTEvent ];

/*
 * Xcil で良く使うカーソルの定義
 */
extern Cursor Xcil_up_cursor;
extern Cursor Xcil_down_cursor;
extern Cursor Xcil_right_cursor;
extern Cursor Xcil_left_cursor;
extern Cursor Xcil_up_down_cursor;
extern Cursor Xcil_right_left_cursor;

extern Cursor Xcil_pointer_cursor;
extern Cursor Xcil_wait_cursor;
extern Cursor Xcil_cross_cursor;
extern Cursor Xcil_hand2_cursor;

/*
 * ＷＭ（ウインドウ・マネージャ）に関する情報
 * Xcil_wm_xoffset : 実際のウインドウの位置のオフセット
 * Xcil_wm_yoffset : 実際のウインドウの位置のオフセット
 * Xcil_wm_xsize   : ＷＭがデコレーションで追加するＸサイズ
 * Xcil_wm_ysize   : ＷＭがデコレーションで追加するＹサイズ
 */
extern long Xcil_wm_xoffset;
extern long Xcil_wm_yoffset;
extern long Xcil_wm_xsize;
extern long Xcil_wm_ysize;



/*----------------------------------------------*
 * XcilPanelItem : ウインドウ・ボタン位置の情報
 *----------------------------------------------*/
extern long XcilPanel_default_border;
extern long XcilPanel_default_xspace;
extern long XcilPanel_default_yspace;


/*------------------------------------------------------------------------*
 * メッセージを日本語で表示するかどうか。デフォルトは英語。
 *------------------------------------------------------------------------*/
extern long Xcil_Japanese_message;



Display *XcilOpenDisplay
  P1 (( char *, display_name ))

#define XcilUniqueContext() XUniqueContext()

Window XcilCreateWindow
  P8 (( Display *, display ),
      ( Window	 , parent  ),
      ( long	 , x	   ),
      ( long	 , y	   ),
      ( long	 , xsize   ),
      ( long	 , ysize   ),
      ( long	 , border  ),
      ( char *	 , title   ))



void XcilMapWindow
  P2 (( Display *, display ),
      ( Window   , window  ))

#define XcilUnmapWindow(display, window) XUnmapWindow(display, window)

void XcilMapPopup
  P2 (( Display *, display ),
      ( Window   , popup   ))



void XcilManagedWindow
  P3 (( Display *  , display ),
      ( Window	   , windw   ),
      ( XContext   , context ))



void XcilUnmanagedWindow
  P3 (( Display *  , display ),
      ( Window	   , windw   ),
      ( XContext   , context ))



Window XcilCreatePanelWindow
  P8 (( Display *      , display ),
      ( Window	       , parent	 ),
      ( XContext       , context ),
      ( long	       , x       ),
      ( long	       , y 	 ),
      ( XcilPanelItem *, list	 ),
      ( long	       , num	 ),
      ( char *	       , title	 ))



/*===============================================*
 * XcilSetString : パネルの表示内容を変更します。
 *===============================================*/
void XcilSetString( Display *display, Window window, XContext context, char *name, 
		    char *format, ... );
//void XcilSetString(/* display, panel, context, name, format, ... */);
/*
  P6 (( Display *, display  ),
      ( Window   , panel    ),
      ( XContext , context  ),
      ( char *   , name     ),
      ( char *   , format   ),
      ( int      , va_alist ))
*/


void XcilDestroyPanelWindow
  P3 (( Display *, display ),
      ( Window   , panel   ),
      ( XContext , context ))



Window XcilPanelItemWindow
  P4 (( Display *, display ),
      ( Window   , panel   ),
      ( XContext , context ),
      ( char *   , name    ))



long XcilPanelItemIndex
  P4 (( Display *, display ),
      ( Window   , panel   ),
      ( XContext , context ),
      ( char *   , name    ))



/*===================================*
 * XCIL_state : 特殊イベントの返り値
 *===================================*/
#define XCIL_OPEN     2
#define XCIL_CONFIRM  1
#define XCIL_CANCEL   0

/*==========================================================*
 * XcilConfirm : モーダル・ウインドウを表示して確認をする。
 *==========================================================*/
long XcilConfirm /* 確認したかどうか */
  P7 (( Display *, display ),
      ( Window   , parent  ),
      ( XContext , c       ),
      ( long     , x       ), /* if x == 0 then center */
      ( long     , y       ), /* if y == 0 then center */
      ( char *   , title   ),
      ( char *   , message ))

long XcilModalLoop
  P3 (( Display *, display    ),
      ( XContext , context    ),
      ( XContext , subcontext ))



/*=================================================*
 * XcilMessages : メッセージウインドウを表示する。
 *=================================================*/
extern char *Xcil_message_button_label;
void XcilMessages
  P7 (( Display *, display  ),
      ( Window   , parent   ),
      ( XContext , context  ),
      ( long     , x        ),
      ( long     , y        ),
      ( char *   , title    ),
      ( char **  , messages ))



/*=========================================================*
 * XcilErrorMessages : エラーメッセージウインドウを表示する。
 *=========================================================*/
extern char *Xcil_error_message_button_label;
void XcilErrorMessages
  P8 (( Display *, display  ),
      ( Window   , parent   ),
      ( XContext , context  ),
      ( long     , x        ),
      ( long     , y        ),
      ( char *   , title    ),
      ( char **  , messages ),
      ( long     , icon32   ))



/*==============================================*
 * XcilAddEventMask : イベントマスクを追加する。
 *==============================================*/
void XcilAddEventMask
  P3 (( Display *, display ),
      ( Window   , window  ),
      ( long     , mask    ))



/*==============================================*
 * XcilDelEventMask : イベントマスクを削除する。
 *==============================================*/
void XcilDelEventMask
  P3 (( Display *, display ),
      ( Window   , window  ),
      ( long     , mask    ))



/*=====================================================*
 * XcilAddCallback : イベント・コールバックを登録する。
 *=====================================================*/
void XcilAddCallback
  P6 (( Display *    , display      ),
      ( Window	     , window       ),
      ( XContext     , context      ),
      ( long	     , type         ),
      ( xcil_callback, callback     ),
      ( caddr_t      , client_value ))



/*=====================================================*
 * XcilDelCallback : イベント・コールバックを削除する。
 *=====================================================*/
void XcilDelCallback
  P5 (( Display *    , display  ),
      ( Window       , windw    ),
      ( XContext     , context  ),
      ( long         , type     ),
      ( xcil_callback, callback ))



/*==============================================================*
 * XcilAddKeyCallback : イベント・キー・コールバックを登録する。
 *==============================================================*/
void XcilAddKeyCallback
  P6 (( Display *    , display	    ),
      ( Window	     , window	    ),
      ( XContext     , context	    ),
      ( long	     , ascii	    ),
      ( xcil_callback, callback	    ),
      ( caddr_t      , client_value ))



/*==============================================================*
 * XcilDelKeyCallback : イベント・キー・コールバックを削除する。
 *==============================================================*/
void XcilDelKeyCallback
  P4 (( Display *, display ),
      ( Window	 , windw   ),
      ( XContext , context ),
      ( long	 , ascii   ))



/*==========================================================*
 * XcilAddCallbacks : イベント・コールバックを複数登録する。
 *==========================================================*/
void XcilAddCallbacks
  P5 (( Display *     , display ),
      ( Window        , windw   ),
      ( XContext      , context ),
      ( XcilCallback *, list    ),
      ( long	      , num     ))



/*==========================================================*
 * XcilAddKeyCallbacks : キー・コールバックを複数登録する。
 *==========================================================*/
void XcilAddKeyCallbacks
  P5 (( Display *     , display ),
      ( Window        , windw   ),
      ( XContext      , context ),
      ( XcilCallback *, list    ),
      ( long	      , num     ))



/*=============================================*
 * XcilEventLoop : イベント・ループを起動する。
 *=============================================*/
long XcilEventLoop
  P2 (( Display *, display ),
      ( XContext , context ))



/*============================================================*
 * XcilAddWindowContext : ウインドウにコンテキストを登録する。
 *============================================================*/
void XcilAddWindowContext
  P5 (( Display *, d   ),
      ( Window   , w   ),
      ( XContext , c   ),
      ( long     , key ),
      ( caddr_t  , wc  ))



/*======================================================================*
 * XcilGetWindowContext : ウインドウに登録したコンテキストをキーで得る。
 *======================================================================*/
caddr_t XcilGetWindowContext
  P4 (( Display *, d   ),
      ( Window	 , w   ),
      ( XContext , c   ),
      ( long	 , key ))



void XcilDrawPixmapBox
  P10 (( Display *, display	 ),
       ( Window	  , window	 ),
       ( long	  , x		 ),
       ( long	  , y		 ),
       ( long	  , xsize	 ),
       ( long	  , ysize	 ),
       ( char *	  , position	 ),
       ( Pixmap	  , pixmap	 ),
       ( long	  , pixmap_xsize ),
       ( long	  , pixmap_ysize ))



void XcilTextPutIcon16
  P5 (( Display *, display ),
      ( Window   , window  ),
      ( XContext , context ),
      ( GC       , gc      ),
      ( long     , icnID   ))

void XcilDrawIcon16
  P5 (( Display *, display ),
      ( Window   , window  ),
      ( long     , x       ),
      ( long     , y       ),
      ( long     , icnID   ))

void XcilDrawIcon16Box
  P8 (( Display *, display  ),
      ( Window   , window   ),
      ( long     , x        ),
      ( long     , y        ),
      ( long     , xsize    ),
      ( long     , ysize    ),
      ( char *   , position ),
      ( long     , icnID    ))

void XcilPutIcon16Box
  P9 (( Display *, display  ),
      ( Window   , window   ),
      ( GC       , gc       ),
      ( long     , x        ),
      ( long     , y        ),
      ( long     , xsize    ),
      ( long     , ysize    ),
      ( char *   , position ),
      ( long     , icnID    ))



void XcilTextPutIcon20
  P5 (( Display *, display ),
      ( Window   , window  ),
      ( XContext , context ),
      ( GC       , gc      ),
      ( long     , icnID   ))

void XcilDrawIcon20
  P5 (( Display *, display ),
      ( Window   , window  ),
      ( long     , x       ),
      ( long     , y       ),
      ( long     , icnID   ))

void XcilDrawIcon20Box
  P8 (( Display *, display  ),
      ( Window   , window   ),
      ( long     , x        ),
      ( long     , y        ),
      ( long     , xsize    ),
      ( long     , ysize    ),
      ( char *   , position ),
      ( long     , icnID    ))

void XcilPutIcon20Box
  P9 (( Display *, display  ),
      ( Window   , window   ),
      ( GC       , gc       ),
      ( long     , x        ),
      ( long     , y        ),
      ( long     , xsize    ),
      ( long     , ysize    ),
      ( char *   , position ),
      ( long     , icnID    ))



void XcilTextPutIcon32
  P5 (( Display *, display ),
      ( Window   , window  ),
      ( XContext , context ),
      ( GC       , gc      ),
      ( long     , icnID   ))

void XcilDrawIcon32
  P5 (( Display *, display ),
      ( Window   , window  ),
      ( long     , x       ),
      ( long     , y       ),
      ( long     , icnID   ))

void XcilDrawIcon32Box
  P8 (( Display *, display  ),
      ( Window   , window   ),
      ( long     , x        ),
      ( long     , y        ),
      ( long     , xsize    ),
      ( long     , ysize    ),
      ( char *   , position ),
      ( long     , icnID    ))

void XcilPutIcon32Box
  P9 (( Display *, display  ),
      ( Window   , window   ),
      ( GC       , gc       ),
      ( long     , x        ),
      ( long     , y        ),
      ( long     , xsize    ),
      ( long     , ysize    ),
      ( char *   , position ),
      ( long     , icnID    ))



void XcilDrawStringBox
  P8 (( Display *, display  ),
      ( Window	 , window   ),
      ( long	 , x	    ),
      ( long	 , y	    ),
      ( long	 , xsize    ),
      ( long	 , ysize    ),
      ( char *	 , position ),
      ( char *	 , string   ))



void XcilPutString
  P6 (( Display *, d	   ),
      ( Window   , w	   ),
      ( GC	 , gc	   ),
      ( long	 , x	   ),
      ( long	 , y 	   ),
      ( char *	 , string ))



void XcilPutStringBox
  P9 (( Display *, d	    ),
      ( Window   , w	    ),
      ( GC	 , gc	    ),
      ( long	 , x	    ),
      ( long	 , y 	    ),
      ( long	 , xsize    ),
      ( long	 , ysize    ),
      ( char *	 , position ),
      ( char *	 , string   ))



#define __DECO_DOWN_LEFT	'1'
#define __DECO_DOWN		'2'
#define __DECO_DOWN_RIGHT	'3'

#define __DECO_LEFT		'4'
#define __DECO_CENTER		'5'
#define __DECO_RIGHT		'6'

#define __DECO_UP_LEFT		'7'
#define __DECO_UP		'8'
#define __DECO_UP_RIGHT		'9'

#define DECO_SHADOW1 "326"
#define DECO_SHADOW2 "896"
#define DECO_SHADOW3 "874"
#define DECO_SHADOW4 "412"
#define DECO_SHADOW5 "1"
#define DECO_SHADOW6 "3"
#define DECO_SHADOW7 "9"
#define DECO_SHADOW8 "7"
#define DECO_FRAME1  "42681397"
#define DECO_FRAME2  "4268"
#define DECO_WIDE    "46"

void XcilPutStringDeco
  P8 (( Display *, d	    ),
      ( Window   , w	    ),
      ( GC	 , body_gc  ),
      ( GC	 , frame_gc ),
      ( long	 , x	    ),
      ( long	 , y 	    ),
      ( char *   , deco     ),
      ( char *	 , string   ))

void XcilPutStringBoxDeco
  P11 (( Display *, d        ),
       ( Window   , w        ),
       ( GC       , body_gc  ),
       ( GC       , frame_gc ),
       ( long     , x        ),
       ( long     , y        ),
       ( long     , xsize    ),
       ( long     , ysize    ),
       ( char *   , position ),
       ( char *   , deco     ),
       ( char *   , string   ))



long XcilTextHeight
  P1 (( char *, string ))



long XcilTextWidth
  P1 (( char *, string ))



void XcilTextExtents
  P4 (( char *	     , string  ),
      ( int *	     , ascent  ),
      ( int *	     , descent ),
      ( XCharStruct *, over    ))



void XcilTextSize
  P3 (( char **, text  ),
      ( long * , xsize ),
      ( long * , ysize ))



/*===========================================*
 * XcilTransGeometry : 座標文字列を変換する。
 *	<WIDTH>x<HEIGHT>[+-]<X>[+-]<Y>
 *===========================================*/
void XcilTransGeometry
  P5 (( char *, string ),
      ( long *, x      ),
      ( long *, y      ),
      ( long *, xsize  ),
      ( long *, ysize  ))



/*===============================================*
 * XcilGetWindowSize : ウインドウのサイズを得る。
 *===============================================*/
void XcilGetWindowSize
  P4 (( Display *, d     ),
      ( Window   , w     ),
      ( long *   , xsize ),
      ( long *   , ysize ))



/*===============================================*
 * マウスがウインドウに入っているかどうか調べる。
 *===============================================*/
long XcilMouseInWindow
  P2 (( Display *, display ),
      ( Window   , window  ))



/*===============================================================*
 * XcilAscii : イベントをアスキーコードとキーシンボルに変換する。
 *===============================================================*/
long XcilAscii
  P2 (( XEvent *, event  ),
      ( KeySym *, keysym ))



/*=============================================================*
 * XcilPositionCoord : ポジションストリングから位置を計算する。
 *=============================================================*/
void XcilPositionCoord
  P7 (( char *, position  ),
      ( long  , box_xsize ),
      ( long  , box_ysize ),
      ( long  , xsize     ),
      ( long  , ysize     ),
      ( long *, x         ),
      ( long *, y         ))



/*---------------------*
 * Event Callback Sets
 *---------------------*/

long Xcil_hdr_quit
  P6 (( Display *, display      ),
      ( Window   , window       ),
      ( XContext , context      ),
      ( XEvent * , event        ),
      ( caddr_t  , client_value ),
      ( caddr_t  , server_value ))

long Xcil_hdr_map_popup
  P6 (( Display *, display      ),
      ( Window   , window       ),
      ( XContext , context      ),
      ( XEvent * , event        ),
      ( caddr_t  , menu_window  ),
      ( caddr_t  , server_value ))

long Xcil_hdr_unmap_popup
  P6 (( Display *, display      ),
      ( Window   , window       ),
      ( XContext , context      ),
      ( XEvent * , event        ),
      ( caddr_t  , menu_window  ),
      ( caddr_t  , server_value ))



/********************************************************************
 * Xcil_text LIBRARY : テキスト・ウインドウとして扱うためのルーチン
 ********************************************************************/

void XcilTextSet
  P3 (( Display *, display ),
      ( Window   , window  ),
      ( XContext , context ))



void XcilTextPuts
  P5 (( Display *, display ),
      ( Window   , window  ),
      ( XContext , context ),
      ( GC       , gc      ),
      ( char *   , string  ))



void XcilTextClear
  P3 (( Display *, display ),
      ( Window   , window  ),
      ( XContext , context ))



void XcilTextNextLine
  P3 (( Display *, display ),
      ( Window   , window  ),
      ( XContext , context ))



//void XcilTextPrintf(/* display, window, context, gc, format, va_alist */);
  void XcilTextPrintf( Display *display, Window window, XContext context, GC gc, char *format, ... );
/*   Display * display  ;
     Window    window   ;
     XContext  context  ;
     GC        gc       ;
     char *    format   ;
     int       va_alist ; */



/*===============================================================*
 * XcilTextGetc : テキストウィンドウから文字を位置文字読み込む。
 *===============================================================*/
long XcilTextGetc
  P3 (( Display *, display ),
      ( Window   , window  ),
      ( XContext , context ))



/*=========================================================*
 * XcilTextGets : テキストウィンドウから文字列を読み込む。
 *=========================================================*/
long XcilTextGets
  P5 (( Display *, display ),
      ( Window   , window  ),
      ( XContext , context ),
      ( char *   , buffer  ),
      ( long     , n       ))



/*========================================================================*
 * XcilTextGrabGets : グラブしてテキストウィンドウから文字列を読み込む。
 *========================================================================*/
long XcilTextGrabGets
  P5 (( Display *, display ),
      ( Window   , window  ),
      ( XContext , context ),
      ( char *   , buffer  ),
      ( long     , n       ))



/*======================================================================*
 * XcilTextGetsReady : テキストウィンドウを文字を入力できるようにする。
 *======================================================================*/
void XcilTextGetsReady
  P5 (( Display *    , display  ),
      ( Window       , window   ),
      ( XContext     , context  ),
      ( xcil_callback, callback ),
      ( caddr_t      , value    ))

long XcilTextGetInput
  P5 (( Display *, display ),
      ( Window   , window  ),
      ( XContext , context ),
      ( char *   , buffer  ),
      ( long     , n       ))

void XcilTextSetInput
  P4 (( Display *, display ),
      ( Window   , window  ),
      ( XContext , context ),
      ( char *   , buffer  ))



/********************************
 * スクロールに関するライブラリ *
 ********************************/



typedef long (*xcil_sb_callback)
     P6 (( Display *, display	   ),
	 ( Window   , window	   ),
	 ( XContext , context	   ),
	 ( XEvent * , event	   ),
	 ( caddr_t  , client_value ),
	 ( double   , server_value )) /* 位置[0..1]が入る */

/*=========================================================*
 * XcilScrollSet : ウインドウをスクロールウインドウにする。
 *=========================================================*/
void XcilScrollSet
  P5 (( Display *       , display      ),
      ( Window          , window       ),
      ( XContext        , context      ),
      ( xcil_sb_callback, callback     ), /* バーが移動した時に呼ばれる */
      ( caddr_t         , client_value ))



/*============================================================*
 * XcilScrollBarSetPosition : スクロールバーの位置を設定する。
 *============================================================*/
void XcilScrollBarSetPosition
  P4 (( Display *, display  ),
      ( Window   , window   ),
      ( XContext , context  ),
      ( double   , position )) /* 全体の長さの割合[0..1]で設定する */



/*===========================================================*
 * XcilScrollBarSetLength : スクロールバーの長さを設定する。
 *===========================================================*/
void XcilScrollBarSetLength
  P4 (( Display *, display ),
      ( Window   , window  ),
      ( XContext , context ),
      ( double   , length  )) /* 全体の長さの割合[0..1]で設定する */



/*====================================================================*
 * XcilScrollBarSetViewLength : 見える最小の長さを設定する。制約長さ。
 *====================================================================*/
void XcilScrollBarSetViewLength
  P4 (( Display *, display     ),
      ( Window   , window      ),
      ( XContext , context     ),
      ( double   , view_length )) /* バーの長さの割合[0..1]で設定する */



/*=============================================================*
 * XcilScrollBarSetMouseOffset : マウスのオフセットを設定する。
 *=============================================================*/
void XcilScrollBarSetMouseOffset
  P4 (( Display *, display  ),
      ( Window   , window   ),
      ( XContext , context  ),
      ( double   , offset   )) /* バーの長さの割合[0..1]で設定する */



/*==============================================================*
 * XcilScrollBarGetPosition : スクロールバーの現在の位置を得る。
 *==============================================================*/
double XcilScrollBarGetPosition /* 全体の長さの割合[0..1]で返す */
  P3 (( Display *, display  ),
      ( Window   , window   ),
      ( XContext , context  ))



/*============================================*
 * XcilScrollBarInc : スクロールバーを進める。
 *============================================*/
void XcilScrollBarInc
  P3 (( Display *, display  ),
      ( Window   , window   ),
      ( XContext , context  ))



/*===============================================*
 * XcilScrollBarDec : スクロールバーを後退する。
 *===============================================*/
void XcilScrollBarDec
  P3 (( Display *, display  ),
      ( Window   , window   ),
      ( XContext , context  ))



/*****************************************
 * ポップアップ・メニューに関するルーチン
 *****************************************/

typedef struct _XcilPopupMenuItem XcilPopupMenuItem;

typedef long (*xcil_popup_callback)
     P7 (( Display *          , display      ),
	 ( Window             , window       ),
	 ( XContext           , context      ),
	 ( XEvent *           , event        ),
	 ( caddr_t            , client_value ),
	 ( long               , server_index ),
	 ( XcilPopupMenuItem *, server_list  ))

#define XcilPopupLINE 0
#define XcilPopupEOM  ((char *)-1)

struct _XcilPopupMenuItem {
  char *              string;
  xcil_popup_callback callback;
  caddr_t             client_value;

  /* return value */
  long yoffset;
  long height;
};



/*=====================================*
 * ポップアップ・メニューをセットする。
 *=====================================*/
void XcilPopupSet
  P6 (( Display *          , display ),
      ( Window             , window  ),
      ( XContext           , context ),
      ( long               , button  ), /* Button1, Button2, Button3 */
      ( XcilPopupMenuItem *, menu    ),
      ( long               , num     ))



/*****************************
 * プライベート関数セクション
 *****************************/



/*--------------------------------------------*
 * __Xcil_set_cursors : カーソルをセットする。
 *--------------------------------------------*/
void __Xcil_set_cursors
  P1 (( Display *, display ))

/*----------------------------------------------*
 * __Xcil_set_icon16 : アイコン１６をセットする
 *----------------------------------------------*/
void __Xcil_set_icon16
  P1 (( Display *, display ))

/*----------------------------------------------*
 * __Xcil_set_icon20 : アイコン２０をセットする
 *----------------------------------------------*/
void __Xcil_set_icon20
  P1 (( Display *, display ))

/*----------------------------------------------*
 * __Xcil_set_icon32 : アイコン３２をセットする
 *----------------------------------------------*/
void __Xcil_set_icon32
  P1 (( Display *, display ))



/***********************************************************************
 * マッキントッシュのようなファイルダイアログの定義
 ***********************************************************************/
typedef struct _XcilFileDialogAttributes XcilFileDialogAttributes;

/*================================================================*
 * xcil_file_dialog_callback : ファイル・ダイアログのコールバック
 *================================================================*/
typedef long (*xcil_file_dialog_callback)
     P6 (( Display *                 , display      ),
	 ( Window                    , window       ),
	 ( XContext                  , context      ),
	 ( XEvent *                  , event        ),
	 ( caddr_t                   , client_value ),
	 ( XcilFileDialogAttributes *, attribute    ))


#define FD_MODE_CHOOSE 0
#define FD_MODE_NEW    1
/*====================================================*
 * xcil_file_dialog_mode : ファイルダイアログのモード
 *====================================================*/
#define FDMOpen  0   /* 000 choose an alias file */
#define FDMSave  1   /* 001 choose a file */
/* option */
#define FDMText  2   /* 010 text window */
#define FDMCheck 4   /* 100 check file exist */

/*===========================================================*
 * XcilFileDialogAttributes : ファイルダイアログの属性設定型
 *===========================================================*/
struct _XcilFileDialogAttributes {
  char *title;        /* タイトルバーではない */
  long  mode;         /* FDMOpen or FDMSave */

  /* ボタン設定 */
  char *open_or_save_label; /* オープン、セーブ "Open" or "Save" */
  xcil_file_dialog_callback callback; /* Open or Save が押された時呼ばれる */
  caddr_t client_value; /* クライアント返り値 */

  /* ファイルタイプ */
  long   filetype_num;  /* ファイルタイプの数の指定 */
  char **filetype_list; /* ファイルタイプリストの指定 */
  long   filetype_index;/* ファイルタイプの選択 */

  /* オプションボタン設定 */
  char *        macintosh_label;    /* 一つ上へ移動する "Macintosh HD" */
  xcil_callback macintosh_callback; /* macintosh */

  char *        desktop_label;    /* ルートへ移動する "Desktop" */
  xcil_callback desktop_callback; /* desktop */

  char *        home_label;    /* ホームへ移動する "Home" */
  xcil_callback home_callback; /* home */

  char *        cancel_label;    /* キャンセル "Cancel" */
  xcil_callback cancel_callback; /* cancel */

  /* 返り値 でもあり 初期値でもある */
  char current_dir[512]; /* カレントディレクトリを返す */
  char filename[512]; /* ファイル名だけを返す */

  /* 返り値 */
  char fullname[512]; /* ファイル名をフルパスで返す */

  /* システムが使用するデータエリア */
  Window window; /* ダイアログウィンドウ */

  char   static_current_dir[512];
  long   path_num;
  char  *path_list[32];
  XcilPopupMenuItem popup_list[32];
  long popup_num;

  XcilPopupMenuItem *filetype_popup_list;

  char **all_file_list;
  long   file_num;
  char **file_list;

  long select_offset;
  long select_file;

  int dotfile_ignore; /* ドットファイルを無視する */
};



/*=========================================================*
 * XcilCreateFileDialog : ファイル・ダイアログを作成する。
 *=========================================================*/
Window XcilCreateFileDialog
  P6 (( Display *                 , display   ),
      ( Window	                  , parent    ),
      ( XContext                  , context   ),
      ( long                      , x         ),
      ( long                      , y         ),
      ( XcilFileDialogAttributes *, attribute ))




/*========================*
 * ダイアログを表示する。
 *========================*/

typedef struct _XcilDialogAttributes XcilDialogAttributes;

struct _XcilDialogAttributes {
  /* 設定 */
  char  *message;            /* メッセージ */
  char  *default_answer;     /* 0 でなければテキストを開く */
  long   button_num;         /* ボタンの数 */
  char **button_list;        /* 初期値は { "Cancel" "Ok" } */
  long   default_button;     /* リターンキーを押した時のボタン
				初期値は "Ok" = 1 */
  long   icon32;             /* 使用しない */

  /* 返り値 */
  char text_returned[ 512 ]; /* 入力された文字列 */
  char *button_returned;     /* 押されたボタン名 */
};



/*===========================================*
 * XcilDisplayDialog : ダイアログを表示する。
 *===========================================*/
long XcilDisplayDialog /* 押されたボタンのインデックス */
  P6 (( Display *             , display   ),
      ( Window                , parent    ),
      ( XContext              , context   ),
      ( long                  , x         ),
      ( long                  , y         ),
      ( XcilDialogAttributes *, attribute ))




/*==============================*
 * メニューバーに関するルーチン
 *==============================*/
/* menu item state */
#define MBI_DELIMITER -2
#define MBI_INACTIVE -1
#define MBI_ACTIVE 0

/* menu item state */
#define MBI_NO_BLINK 0
#define MBI_BLINK 1

/* menu item mark */
#define MBI_MARK   1
#define MBI_UNMARK 0

/*=========================================================*
 * XcilMenuItem : メニューバーの各メニューの各項目の構造体
 *=========================================================*/
typedef struct {
  char *label;    /* ラベル／メニューの区切り="-" */
  xcil_callback callback;
  caddr_t client_value;

  long  state;    /* 選択不可状態, MBI_ACTIVE, MBI_INACTIVE, MBI_DELIMITER */
  long  mark;     /* マーク, MBI_MARK, MBI_UNMARK */
  long  blink;    /* 選択された時に点滅する, MBI_BLINK, MBI_NO_BLINK */
  char *shortcut; /* キーボードショートカット */
} XcilMenuItem;

typedef struct {
  char         *label;
  long          item_num;
  XcilMenuItem *item_list;

  /* returned */
  Window window;
  Window parent;
  Window panel;
  long index;
} XcilMenu;


#define MAX_MENU_NUM 32


/*==============================================*
 * XcilCreateMenuBar : メニューバーを作成する。
 *==============================================*/
Window XcilCreateMenuBar
  P5 (( Display * , display   ),
      ( Window    , parent    ),
      ( XContext  , context   ),
      ( long      , menu_num  ),
      ( XcilMenu *, menu_list ))



/**************************************************************************
 * リソースの定義
 **************************************************************************/
/*========================================================================*
 * Xcil_pattern_pixmap : ８×８のパターン
 *========================================================================*/
extern Pixmap Xcil_pattern_pixmap[64];


/*========================================================================*
 * Xcil_icon16_pixmap : １６×１６のアイコンの定義
 *========================================================================*/
extern Pixmap Xcil_icon16_pixmap[];
enum {
  ICN16None = 0,
  ICN16Apple, ICN16Check,
  ICN16Folder, ICN16Doc, ICN16App, ICN16SymLnk,
  ICN16Cmd,
  ICN16Last
};


/*========================================================================*
 * Xcil_icon20_pixmap : ２０×２０のアイコンの定義:主にボタン類
 *========================================================================*/
extern Pixmap Xcil_icon20_pixmap[];
enum {
  ICN20None = 0,
  ICN20Button,
  ICN20CheckBox,
  ICN20CheckedBox,
  ICN20RadioButton,
  ICN20Radio1Button,
  ICN20ScrollBar,
  ICN20ScrollUp,
  ICN20ScrollDown,
  ICN20ScrollLeft,
  ICN20ScrollRight,
  ICN20ScrollSize,
  ICN20Last
};


/*========================================================================*
 * Xcil_icon32_pixmap : ３２×３２のアイコンの定義:アラートなど
 *========================================================================*/
extern Pixmap Xcil_icon32_pixmap[];
enum {
  ICN32None = 0,
  ICN32Bomb,
  ICN32Caution,
  ICN32Note,
  ICN32Stop,
  ICN32Last
};



#endif /* __Xcil_h */
