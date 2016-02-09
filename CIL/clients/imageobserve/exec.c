/*
 * filename : exec.c
 * author   : Takahiro Sugiyama
 * date     : Saturday, February 8 1997
 * describe : イベント処理
 */


#include "observe.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string.h>
#include <stdlib.h>

extern double atof();
int Synchro = 0;
ObserveRec *observe_ptr;

#define MV_LEFT  0
#define MV_DOWN  1
#define MV_UP    2
#define MV_RIGHT 3
#define MV_LEFT_C  4
#define MV_DOWN_C  5
#define MV_UP_C    6
#define MV_RIGHT_C 7

/* Context Key */
#define XDISP_ITEM 0x44495350 /* DISP */

static char *key[] = {
  "●拡大領域関連",
  "\ts ... すべての画像に対して移動の操作を同期します(toggle)",
  "\tv ... 拡大表示において数値をテキストで表示します(toggle)",
  "\t1-9,0,-,=,\\ ... 画像を拡大します",
  "●移動関連",
  "\th/H ... 拡大領域を左に移動します（大文字は大きく移動）",
  "\tj/J ... 拡大領域を下に移動します（大文字は大きく移動）",
  "\tk/K ... 拡大領域を上に移動します（大文字は大きく移動）",
  "\tl/L ... 拡大領域を右に移動します（大文字は大きく移動）",
  "\tButton1 ... 領域の移動（領域をドラッグして移動することも可能）",
  "\tButton3 ... 領域の中心座標を表示する(toggle)",
  "●描写関連",
  "\tr ... 再描写します",
  "\tR ... 濃度値を反転します",
  "\tt ... 濃度値を線形変換します(toggle)",
  "\to ... 原画像に戻します",
  "\tF ... 特徴画像としてスペクトル表示します",
  "\tG ... 濃淡画像として濃淡表示します",
  "\tN ... ラベル画像として境界線表示します",
  "\tC ... カラーラベル画像としてカラー表示します",
  "●その他",
  "\t? ... ヘルプメッセージを表示します",
  "\tQ ... すべて終了します",
  "\tq ... ウィンドウを閉じます",
  "\tI ... 画像の情報を表示します",
  0
};


/*------------------------------------------------------------------------*
 * ヘルプの表示
 *------------------------------------------------------------------------*/
static long print_key()
{
  int i = 0;

  printf("キー操作一覧.\n");
  while (key[i]) {
    puts(key[i]);
    i++;
  }

  return 0;
}


/*------------------------------------------------------------------------*
 * ウィンドウを閉じる
 *------------------------------------------------------------------------*/
long destroy_window(Display *display,
		    Window win,
		    XContext context,
		    XEvent *e,
		    ObserveRec *observe)
{
  int i;
  ObItemRec *item;

  item = (ObItemRec *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  if (item->view) XDestroyWindow(display, item->view);
  if (item->eye) XDestroyWindow(display, item->eye);
  Image.destroy(item->org);
  item->org = 0;
  item->view = 0;
  item->eye = 0;

  for (i = 0; i < observe->num; i++)
    if (observe->list[i].view) break;
  if (i == observe->num) return 1;

  return 0;
}



/*------------------------------------------------------------------------*
 * すべてのウィンドウを閉じて終了する
 *------------------------------------------------------------------------*/
long quit_all(Display *display,
	      Window win,
	      XContext context,
	      XEvent *e,
	      ObserveRec *observe)
{
  int i;

  for (i = 0; i < observe->num; i++)
    {
      ObItemRec *item = &observe->list[i];
      if (item->org) destroy_window(display, item->view, context, e, observe);
    }
  return 1;
}



void item__draw_center_coord(ObItemRec *item, Display *display)
{
  int tx, ty, x, y;
  char buf[256];

  x = item->region_xoffset + item->region_xsize/2;
  y = item->region_yoffset + item->region_ysize/2;
  sprintf(buf, "Center=(%d,%d)", x, y);
  XcilPutStringBoxDeco(display, item->view, Xcil_white_gc, Xcil_black_gc,
		       4, 2, 1, 1, "lt", DECO_FRAME1, buf);
}

static void item__draw_region(ObItemRec *item, Display *display)
{
  int l, r, t, b;
  int cx, cy;

  if (item->region_flag)
    {/* 領域をもとに戻す */
      XCopyArea(display, item->store, item->view, Xcil_black_gc,
		0, 0, item->store_xsize, item->store_ysize,
		item->store_x, item->store_y);
      XCopyArea(display, item->cc_store, item->view, Xcil_black_gc,
		0, 0, item->cc_xsize, item->cc_ysize, 0, 0);
      item->region_flag = 0;
    }
  item->region_flag = 1;

  l = item->region_xoffset;
  r = item->region_xoffset + item->region_xsize;
  t = item->region_yoffset;
  b = item->region_yoffset + item->region_ysize;
  cx = l + item->region_xsize/2;
  cy = t + item->region_ysize/2;

  if (l < 0) l = 0;
  if (t < 0) t = 0;
  if (r >= item->xsize) r = item->xsize - 1;
  if (b >= item->ysize) b = item->ysize - 1;

  item->store_x = l;
  item->store_y = t;
  item->store_xsize = r - l + 1;
  item->store_ysize = b - t + 1;

  XCopyArea(display, item->view, item->store, Xcil_black_gc,
	    item->store_x, item->store_y, item->store_xsize, item->store_ysize,
	    0, 0);
  XCopyArea(display, item->view, item->cc_store, Xcil_black_gc,
	    0, 0, item->cc_xsize, item->cc_ysize, 0, 0);

  if (r-l-2 >= 0 && b-t-2 >= 0)
    XDrawRectangle(display, item->view, Xcil_gray_gc, l+1, t+1, (r-l-2), (b-t-2));
  XDrawRectangle(display, item->view, Xcil_black_gc, l, t, (r-l), (b-t));
  XDrawLine(display, item->view, Xcil_invert_gc, cx, cy-4, cx, cy+4);
  XDrawLine(display, item->view, Xcil_invert_gc, cx-4, cy, cx+4, cy);

  if (item->do_display_center_coord)
    item__draw_center_coord(item, display);
}

static long item__eye_resize(ObItemRec *item, Display *display)
{
  long wx, wy;

  XcilGetWindowSize(display, item->eye, &wx, &wy);

  if ((wx != item->eye_xsize) || (wy != item->eye_ysize))
    {/* Window resize */
      item->eye_xsize = wx;
      item->eye_ysize = wy;
      item__fit_region_size(item);
    }
  else
    {/* others resize */
      wx = item->eye_xsize;
      wy = item->eye_ysize;
    }

  item__fit_window_size(item);
  if ((wx != item->eye_xsize) || (wy != item->eye_ysize))
    {
      XFreePixmap(display, item->buffer);
      XResizeWindow(display, item->eye, item->eye_xsize, item->eye_ysize);
      item->buffer = XCreatePixmap(display, item->eye, item->eye_xsize,
				   item->eye_ysize, Xcil_depth);
      item__fit_region_size(item);
      return 1;
    }
  item__draw_region(item, display);

  return 0;
}


long view_redraw_window(Display *display,
			Window win,
			XContext context,
			XEvent *e)
{
  ObItemRec *item;

  item = (ObItemRec *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  item__make_image(item);
  item__disp_view(item, display);
  item__free_image(item);
  item->region_flag = 0;
  item__draw_region(item, display);

  return 0;
}



long eye_redraw_window(Display *display,
		   Window win,
		   XContext context,
		   XEvent *e)
{
  ObItemRec *item;

  item = (ObItemRec *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  item__make_image(item);

  if (item__eye_resize(item, display) == 1) return 0;
  item__draw_region(item, display);

  item__disp_eye(item, display);
  item__free_image(item);

  return 0;
}



/*------------------------------------------------------------------------*
 * シンクロ画像の再表示
 *------------------------------------------------------------------------*/
void synchro_redraw(Display *display, ObItemRec *item, XContext context,
		    XEvent *event, ObserveRec *observe)
{
  int i, x, y;
  if (Synchro)
    {
      x = item->region_xoffset + item->region_xsize/2;
      y = item->region_yoffset + item->region_ysize/2;
      for (i = 0; i < observe->num; i++)
	{
	  ObItemRec *tmp = &observe->list[i];
	  if (tmp->view == 0) continue;
	  if (tmp == item) continue;
	  tmp->region_xoffset = x - tmp->region_xsize/2;
	  tmp->region_yoffset = y - tmp->region_ysize/2;
	  eye_redraw_window(display, tmp->view, context, event);
	}
    }
}


/*------------------------------------------------------------------------*
 * シンクロフラグ
 *------------------------------------------------------------------------*/
long synchro_image(Display *display,
		   Window win,
		   XContext context,
		   XEvent *event,
		   ObserveRec *observe)
{
  ObItemRec *item;

  item = (ObItemRec *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  Synchro = 1 - Synchro;

  synchro_redraw(display, item, context, event, observe);
  return 0;
}


/*------------------------------------------------------------------------*
 * 値表示フラグ
 *------------------------------------------------------------------------*/
long value_disp_mode(Display *display,
		     Window win,
		     XContext context,
		     XEvent *event,
		     ObserveRec *observe)
{
  ObItemRec *item;

  item = (ObItemRec *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  item->value_mode = 1 - item->value_mode;
  if (item->value_mode == 0)
    {
      item->pixel_xsize = item->pixel_size;
      item->pixel_ysize = item->pixel_size;
    }
  else
    {
      item->pixel_xsize = item->text_xsize;
      item->pixel_ysize = item->text_ysize;
    }
  item__fit_region_size(item);
  eye_redraw_window(display, item->view, context, event);
  synchro_redraw(display, item, context, event, observe);

  return 0;
}


long redraw_window(Display *display,
		   Window win,
		   XContext context,
		   XEvent *e)
{
  ObItemRec *item;

  item = (ObItemRec *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  view_redraw_window(display, win, context, e);
  eye_redraw_window(display, win, context, e);

  return 0;
}


static long reverse_window(Display *display,
			   Window win,
			   XContext context,
			   XEvent *e)
{
  ObItemRec *item;

  item = (ObItemRec *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  item->reveresed = 1 - item->reveresed;
  view_redraw_window(display, win, context, e);
  eye_redraw_window(display, win, context, e);

  return 0;
}

static long trans_window(Display *display,
			 Window win,
			 XContext context,
			 XEvent *e)
{
  long i, befree = 0;
  ObItemRec *item;

  item = (ObItemRec *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  item->transed = 1 - item->transed;
  view_redraw_window(display, win, context, e);
  eye_redraw_window(display, win, context, e);

  return 0;
}

static long original_image(Display *display,
			   Window win,
			   XContext context,
			   XEvent *e)
{
  long befree = 0;
  ObItemRec *item;

  item = (ObItemRec *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  item->reveresed = 0;
  item->transed = 0;
  view_redraw_window(display, win, context, e);
  eye_redraw_window(display, win, context, e);

  return 0;
}



static long scale_image(Display *display,
			Window win,
			XContext context,
			XEvent *e,
			long scale)
{
  ObItemRec *item;

  item = (ObItemRec *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  item->pixel_size = scale;
  if (item->value_mode) return 0;

  item->pixel_xsize = scale;
  item->pixel_ysize = scale;

  item__fit_region_size(item);
  eye_redraw_window(display, win, context, e);
  synchro_redraw(display, item, context, e, observe_ptr);

  return 0;
}



static long property_change(Display *display,
			    Window win,
			    XContext context,
			    XEvent *e,
			    char *prop_str)
{
  static char *type_list[] = {
    0, 0, 0, "CIL_DP_BIT4", "CIL_DP_CHAR", "CIL_DP_SHORT", "CIL_DP_LONG",
    "CIL_DP_LONG", "CIL_DP_UCHAR", "CIL_DP_USHORT", "CIL_DP_ULONG",
    "CIL_DP_ULONG", "CIL_DP_FLOAT", "CIL_DP_DOUBLE"};
  char buf[512], *str;
  ObItemRec *item;

  item = (ObItemRec *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;
  if ((Bit4 > __TYPE(item->org)) || (__TYPE(item->org) > Double))
    return 0;

  sprintf(buf, "%s=%s", type_list[__TYPE(item->org)], prop_str);
  str = strcpy((char*)malloc(strlen(buf)+1), buf);
  putenv(str);
  view_redraw_window(display, win, context, e);
  eye_redraw_window(display, win, context, e);

  return 0;
}



static long information_image(Display *display,
			      Window win,
			      XContext context,
			      XEvent *e)
{
  ObItemRec *item;
  long i, befree = 0;
  Window root;
  int x, y;
  unsigned int width, height, border, depth;

  item = (ObItemRec *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  printf("information of image %s\n", item->name);

  Image.print(item->org);
  printf("Window %s = {\n", item->name);
  XGetGeometry(display, win, &root, &x, &y, &width, &height, &border, &depth);
  printf("    ID     = %d\n", win);
  printf("    x      = %d\n", x);
  printf("    y      = %d\n", y);
  printf("    width  = %d\n", width);
  printf("    height = %d\n", height);
  printf("}\n");

  return 0;
}



static long eye_button_press(Display *display,
			     Window win,
			     XContext context,
			     XEvent *event)
{
  ObItemRec *item;
  int x, y;

  item = (ObItemRec *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  x = item->region_xoffset + event->xbutton.x / item->pixel_xsize;
  y = item->region_yoffset + event->xbutton.y / item->pixel_ysize;
  item->region_xoffset = x - item->region_xsize/2;
  item->region_yoffset = y - item->region_ysize/2;
  eye_redraw_window(display, win, context, event);
  synchro_redraw(display, item, context, event, observe_ptr);

  return 0;
}


static long view_button_release(Display *display,
				Window win,
				XContext context,
				XEvent *e,
				ObserveRec *observe)
{
  XUngrabPointer(display, 0);
  observe->grab = 0;
  observe->state = 0;

  return 0;
}

static long view_button_press(Display *display,
			      Window win,
			      XContext context,
			      XEvent *event,
			      ObserveRec *observe)
{
  ObItemRec *item;
  int x, y;

  item = (ObItemRec *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  if (event->xbutton.button == Button3)
    { item->do_display_center_coord = 1 - item->do_display_center_coord;
      item__draw_region(item, display);
      return 0; }

  if (event->xbutton.button != Button1) return 0;
  x = event->xbutton.x;
  y = event->xbutton.y;
  if (x >= item->region_xoffset && y >= item->region_yoffset &&
      x <= item->region_xsize + item->region_xoffset &&
      y <= item->region_ysize + item->region_yoffset)
    {/* 領域内部…グラブ準備 */
      observe->state = 1;
    }
  else
    {/* 領域外部…領域の移動 */
      item->region_xoffset = x - item->region_xsize/2;
      item->region_yoffset = y - item->region_ysize/2;
      eye_redraw_window(display, win, context, event);
      synchro_redraw(display, item, context, event, observe);
    }

  return 0;
}


static long view_motion_notify(Display *display,
			       Window window,
			       XContext context,
			       XMotionEvent *event,
			       ObserveRec *observe)
{
  ObItemRec *item;
  Window root, child;
  int x_root, y_root;
  unsigned int keys_buttons;
  int x, y, dx, dy;

  if (observe->state == 0) return 0;

  item = (ObItemRec *)XcilGetWindowContext(display, window, context, XDISP_ITEM);
  if (item == 0) return 0;

  XQueryPointer(display, window, &root, &child,
                &x_root, &y_root, &event->x, &event->y, &keys_buttons);

  if (((keys_buttons & Button1MotionMask) == 0) && (observe->state == 0))
    {/* 単に動いた場合 */
      return 0;
    }

  if ((keys_buttons & Button1MotionMask) == 0) return 0;

  x = event->x;
  y = event->y;
  if (x < 0 || x >= item->xsize) return 0;
  if (y < 0 || y >= item->ysize) return 0;

  if (observe->grab == 0)
    {/* マウスをグラブする */
      XGrabPointer(display, window, False,
                   (ButtonReleaseMask|PointerMotionMask),
                   GrabModeAsync, GrabModeSync,
                   None, Xcil_hand2_cursor, 0);
      observe->grab_x = x;
      observe->grab_y = y;
      observe->grab_xoffset = item->region_xoffset;
      observe->grab_yoffset = item->region_yoffset;
      observe->grab = 1;
    }

  dx = x - observe->grab_x;
  dy = y - observe->grab_y;
  if (keys_buttons & ShiftMask)
    if (abs(dx) >= abs(dy)) dy = 0; else dx = 0;
  item->region_xoffset = observe->grab_xoffset + dx;
  item->region_yoffset = observe->grab_yoffset + dy;
  eye_redraw_window(display, window, context, (XEvent*)event);
  synchro_redraw(display, item, context, (XEvent*)event, observe);

  return 0;
}


static long move_image(Display *display,
		       Window win,
		       XContext context,
		       XEvent *e,
		       long wh)
{
  long px, py, ox, oy;
  long xsize, ysize;
  ObItemRec *item;

  item = (ObItemRec *)XcilGetWindowContext(display, win, context, XDISP_ITEM);
  if (item == 0) return 0;

  xsize = Image.xsize(item->org);
  ysize = Image.ysize(item->org);

  ox = item->region_xoffset;
  oy = item->region_yoffset;
  switch (wh)
    {
    case MV_LEFT: ox--; break;
    case MV_RIGHT: ox++; break;
    case MV_UP: oy--; break;
    case MV_DOWN: oy++; break;
    case MV_LEFT_C: ox -= item->region_xsize-1; break;
    case MV_RIGHT_C: ox += item->region_xsize-1; break;
    case MV_UP_C: oy -= item->region_ysize-1; break;
    case MV_DOWN_C: oy += item->region_ysize-1; break;
    }

  if ((ox != item->region_xoffset) || (oy != item->region_yoffset))
    {
      item->region_xoffset = ox;
      item->region_yoffset = oy;
      eye_redraw_window(display, win, context, e);
      synchro_redraw(display, item, context, e, observe_ptr);
    }

  return 0;
}


void exec(ObserveRec *observe)
{
  long i;

  static XcilCallback view_entry[] = {
    {ButtonPress,   (xcil_callback)view_button_press},
    {ButtonRelease, (xcil_callback)view_button_release},
    {MotionNotify,  (xcil_callback)view_motion_notify},
    {Expose,        (xcil_callback)view_redraw_window}
  };

  static XcilCallback eye_entry[] = {
    {ButtonPress,   (xcil_callback)eye_button_press},
    {Expose,        (xcil_callback)eye_redraw_window}
  };

  static XcilCallback key_entry[] = {
    {'q', (xcil_callback)destroy_window},
    {'Q', (xcil_callback)quit_all},
    {'s', (xcil_callback)synchro_image},
    {'v', (xcil_callback)value_disp_mode},
    {'?', (xcil_callback)print_key},
    {'r', (xcil_callback)redraw_window},

    {'I', (xcil_callback)information_image},

    {'R', (xcil_callback)reverse_window},
    {'t', (xcil_callback)trans_window},
    {'o', (xcil_callback)original_image},

    {'1', (xcil_callback)scale_image, (caddr_t)1},
    {'2', (xcil_callback)scale_image, (caddr_t)2},
    {'3', (xcil_callback)scale_image, (caddr_t)3},
    {'4', (xcil_callback)scale_image, (caddr_t)4},
    {'5', (xcil_callback)scale_image, (caddr_t)6},
    {'6', (xcil_callback)scale_image, (caddr_t)8},
    {'7', (xcil_callback)scale_image, (caddr_t)12},
    {'8', (xcil_callback)scale_image, (caddr_t)16},
    {'9', (xcil_callback)scale_image, (caddr_t)20},
    {'0', (xcil_callback)scale_image, (caddr_t)24},
    {'-', (xcil_callback)scale_image, (caddr_t)32},
    {'=', (xcil_callback)scale_image, (caddr_t)48},
    {'\\', (xcil_callback)scale_image, (caddr_t)64},

    {'h', (xcil_callback)move_image, (caddr_t)MV_LEFT},
    {'j', (xcil_callback)move_image, (caddr_t)MV_DOWN},
    {'k', (xcil_callback)move_image, (caddr_t)MV_UP},
    {'l', (xcil_callback)move_image, (caddr_t)MV_RIGHT},
    {'H', (xcil_callback)move_image, (caddr_t)MV_LEFT_C},
    {'J', (xcil_callback)move_image, (caddr_t)MV_DOWN_C},
    {'K', (xcil_callback)move_image, (caddr_t)MV_UP_C},
    {'L', (xcil_callback)move_image, (caddr_t)MV_RIGHT_C},

    {'F', (xcil_callback)property_change, (caddr_t)"feature"},
    {'G', (xcil_callback)property_change, (caddr_t)"gray"},
    {'N', (xcil_callback)property_change, (caddr_t)"label"},
    {'C', (xcil_callback)property_change, (caddr_t)"color-label"},
  };

  view_entry[0].client_value = (caddr_t)observe;
  view_entry[1].client_value = (caddr_t)observe;
  view_entry[2].client_value = (caddr_t)observe;
  view_entry[3].client_value = (caddr_t)observe;

  key_entry[0].client_value = (caddr_t)observe; /* quit */
  key_entry[1].client_value = (caddr_t)observe; /* quit all */
  key_entry[2].client_value = (caddr_t)observe; /* synchro */
  key_entry[3].client_value = (caddr_t)observe; /* value disp */

  for (i = 0; i < observe->num; i++)
    {
      ObItemRec *item = &observe->list[i];

      if (item->view == 0) continue;
      XcilAddCallbacks(observe->display, item->view, observe->context,
		       view_entry, XcilNumber(view_entry));
      XcilAddKeyCallbacks(observe->display, item->view, observe->context,
			  key_entry, XcilNumber(key_entry));

      XcilAddCallbacks(observe->display, item->eye, observe->context,
		       eye_entry, XcilNumber(eye_entry));
      XcilAddKeyCallbacks(observe->display, item->eye, observe->context,
			  key_entry, XcilNumber(key_entry));

      XcilAddWindowContext(observe->display, item->view, observe->context,
			   XDISP_ITEM, (caddr_t)item);
      XcilAddWindowContext(observe->display, item->eye, observe->context,
			   XDISP_ITEM, (caddr_t)item);
    }
  observe_ptr = observe;
  XSetTile(observe->display, Xcil_gray_gc, Xcil_gray_xpixmap[1]);
  XcilEventLoop(observe->display, observe->context);
}
