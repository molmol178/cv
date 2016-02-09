/*
 * filename : item.c
 * author   : Takahiro Sugiyama
 * date     : Saturday, February 8 1997
 * describe : アイテムに対する操作
 */


#include "observe.h"


/*------------------------------------------------------------------------*
 * アイテムを初期化する
 *------------------------------------------------------------------------*/
void item__init(ObItemRec *item, char *filename, int scale)
{
  strcpy(item->name, filename);
  strcpy(item->filename, filename);

  item->transed = 0;
  item->reveresed = 0;

  item->org = Image.create(item->name);
  item->img = Image.create("DISP");

  item->view = 0;
  item->eye = 0;

  item->xsize = 0;
  item->ysize = 0;

  item->region_xoffset = 0;
  item->region_yoffset = 0;
  item->region_xsize = 0;
  item->region_ysize = 0;
  item->pixel_size = scale;
  item->pixel_xsize = scale;
  item->pixel_ysize = scale;
  item->text_xsize = scale;
  item->text_ysize = scale;
  item->value_mode = 0;

  item->region_flag = 0;
  item->do_display_center_coord = 0;
  item->do_display_mouse_coord = 0;
}


/*------------------------------------------------------------------------*
 * 画像ファイルを読み込む
 *------------------------------------------------------------------------*/
int item__load(ObItemRec *item)
{
  if (Image.load(item->org, item->filename) == 0)
    {
      fprintf(stderr, "警告:ファイルをオープンできません (%s).\n",
	      item->filename);
      return 0;
    }

  Image.print(item->org);

  item->xsize = __XSIZE(item->org);
  item->ysize = __YSIZE(item->org);
  item->eye_xsize = item->xsize;
  item->eye_ysize = item->ysize;

  return 1;
}


/*------------------------------------------------------------------------*
 * 表示する
 *------------------------------------------------------------------------*/
void item__disp_view(ObItemRec *item, Display *display)
{
  char *name = item->img->name;
  item->img->name = item->org->name;
  XShowImageFull(display, item->view, Xcil_black_gc, item->img);
  item->img->name = name;
}


/*------------------------------------------------------------------------*
 * 画素のピックスマップを作成する。
 *------------------------------------------------------------------------*/
#define MAX_PIXEL 64
Pixmap black_pixel_pixmap[MAX_PIXEL];
Pixmap white_pixel_pixmap[MAX_PIXEL];

void item__make_pixel_pixmap(Display *display)
{
  int i, psize;
  Pixmap white, black;
  image tmp;

  tmp = Image.create("TMP");
  for (i = 0; i < MAX_PIXEL; i++)
    {
      psize = i + 1;

      black = XCreatePixmap(display, Xcil_root_window, psize, psize, Xcil_depth);
      white = XCreatePixmap(display, Xcil_root_window, psize, psize, Xcil_depth);
      XFillRectangle(display, black, Xcil_white_gc, 0, 0, psize, psize);
      XFillRectangle(display, white, Xcil_white_gc, 0, 0, psize, psize);

      if (i <= 3)
	XFillRectangle(display, black, Xcil_black_gc, 0, 0, psize, psize);
      else
	{
	  int x, y;
	  Image.make(tmp, Bit1, psize, psize);
	  ImageFillArc(tmp, 1, 1, i-2, i-2, 0, 360*64, (char*)1);
	  for (y = 0; y < psize; y++)
	    for (x = 0; x < psize; x++)
	      if (__PIXEL(tmp,x,y,bit1))
		XDrawPoint(display, black, Xcil_black_gc, x, y);
	}
      if (i >= 7)
	XFillRectangle(display, white, Xcil_black_gc,
		       psize/2-1, psize/2-1, 2, 2);

      black_pixel_pixmap[i] = black;
      white_pixel_pixmap[i] = white;
    }
}

void item__draw_eye_buffer_bit1(ObItemRec *item, Display *display,
				int l, int r, int t, int b, int wx0, int wy0)
{
  register int x, y, wx, wy;
  register int pixel_size = item->pixel_size;
  register Pixmap black_pixmap = black_pixel_pixmap[pixel_size-1];
  register Pixmap white_pixmap = white_pixel_pixmap[pixel_size-1];

  for (wy = wy0, y = t; y <= b; y++, wy += pixel_size)
    for (wx = wx0, x = l; x <= r; x++, wx += pixel_size)
      {
	if (__PIXEL(item->img, x, y, bit1))
	  XCopyArea(display, black_pixmap, item->buffer,
		    Xcil_black_gc, 0, 0, pixel_size, pixel_size, wx, wy);
	else
	  XCopyArea(display, white_pixmap, item->buffer,
		    Xcil_black_gc, 0, 0, pixel_size, pixel_size, wx, wy);
      }
}

/*------------------------------------------------------------------------*
 * 表示する
 *------------------------------------------------------------------------*/
void item__disp_eye(ObItemRec *item, Display *display)
{
  register int l, r, t, b, w, h, cx, cy, cw, ch;
  register int wx, wy, wxsize, wysize;

  l = item->region_xoffset;
  r = item->region_xoffset + item->region_xsize;
  t = item->region_yoffset;
  b = item->region_yoffset + item->region_ysize;
  if (l < 0) { wx = -l * item->pixel_xsize; l = 0; } else wx = 0;
  if (t < 0) { wy = -t * item->pixel_ysize; t = 0; } else wy = 0;
  if (r >= item->xsize) r = item->xsize - 1;
  if (b >= item->ysize) b = item->ysize - 1;

  if (l >= item->xsize) l = item->xsize;
  if (t >= item->ysize) t = item->ysize;

  w = r - l + 1;
  h = b - t + 1;
  wxsize = w * item->pixel_xsize;
  wysize = h * item->pixel_ysize;

  if (wx != 0 || wy != 0 || w < item->region_xsize || h < item->region_ysize)
    {
      XFillRectangle(display, item->buffer, Xcil_gray_gc,
		     0, 0, item->eye_xsize, item->eye_ysize);
    }
  else
    {
      XFillRectangle(display, item->buffer, Xcil_white_gc,
		     0, 0, item->eye_xsize, item->eye_ysize);
    }

  if (w > 0 && h > 0)
    {
      if (!item->value_mode)
	{
	  if (__TYPE(item->img) == Bit1)
	    item__draw_eye_buffer_bit1(item, display, l, r, t, b, wx, wy);
	  else
	    {
	      extern int XDrawImage_SetColormap;
	      char *name = item->img->name;
	      item->img->name = item->org->name;
	      XDrawImage_SetColormap = 0;
	      XShowImage(display, item->buffer, Xcil_black_gc, item->img,
			 wx, wy, wxsize, wysize, l, t, w, h);
	      XDrawImage_SetColormap = 1;
	      item->img->name = name;
	    }
	}
      else
	{/* Display in Text */
	  item__draw_eye_buffer_text(item, display, l, r, t, b, wx, wy);
	}
    }
  XCopyArea(display, item->buffer, item->eye, Xcil_black_gc, 0, 0,
	    item->eye_xsize, item->eye_ysize, 0, 0);

  cw = item->pixel_xsize;
  ch = item->pixel_ysize;
  cx = cw*((int)item->region_xsize/2);
  cy = ch*((int)item->region_ysize/2);
  XDrawRectangle(display, item->eye, Xcil_invert_gc, cx, cy, cw-1,ch-1);
}


/*------------------------------------------------------------------------*
 * 表示する
 *------------------------------------------------------------------------*/
void item__disp(ObItemRec *item, Display *display)
{
  item__disp_view(item, display);
  item__disp_eye(item, display);
}


/*------------------------------------------------------------------------*
 * 領域サイズをウィンドウにフィットさせる
 *------------------------------------------------------------------------*/
void item__fit_region_size(ObItemRec *item)
{
  int ox, oy;
  ox = item->region_xsize / 2;
  oy = item->region_ysize / 2;
  item->region_xsize = item->eye_xsize / item->pixel_xsize;
  item->region_ysize = item->eye_ysize / item->pixel_ysize;
  item->region_xoffset = item->region_xoffset + ox - item->region_xsize/2;
  item->region_yoffset = item->region_yoffset + oy - item->region_ysize/2;
}


extern long MAX_WX;
extern long MAX_WY;

/*------------------------------------------------------------------------*
 * ウィンドウを領域サイズにフィットさせる
 *------------------------------------------------------------------------*/
void item__fit_window_size(ObItemRec *item)
{
  item->eye_xsize = item->region_xsize * item->pixel_xsize;
  item->eye_ysize = item->region_ysize * item->pixel_ysize;

  if ((item->eye_xsize > MAX_WX) || (item->eye_ysize > MAX_WY))
    {
      if (item->eye_xsize > MAX_WX) item->eye_xsize = MAX_WX;
      if (item->eye_ysize > MAX_WY) item->eye_ysize = MAX_WY;
      item__fit_region_size(item);
    }
}


/*------------------------------------------------------------------------*
 * X Windowを開いて初期化する
 *------------------------------------------------------------------------*/
void item__open_window(ObItemRec *item, Display *display, XContext context,
		       long xoffset, long yoffset)
{
  char eyename[256];

  item->view = XcilCreateWindow(display, 0, xoffset, yoffset,
				item->xsize, item->ysize,
				5, item->name);

  if (optspecified("観察窓下"))
    yoffset += item->ysize + 25;
  else
    xoffset += item->xsize + 7;
  sprintf(eyename, "eye:%s", item->name);
  item->eye = XcilCreateWindow(display, 0, xoffset, yoffset,
			       item->eye_xsize, item->eye_ysize, 5, eyename);
  XcilManagedWindow(display, item->view, context);
  XcilManagedWindow(display, item->eye, context);
  XcilMapWindow(display, item->view);
  XcilMapWindow(display, item->eye);
  item->store = XCreatePixmap(display, item->view,
			      item->xsize, item->ysize, Xcil_depth);
  item->buffer = XCreatePixmap(display, item->eye,
			       item->eye_xsize, item->eye_ysize, Xcil_depth);
  item->cc_xsize = item->xsize;
  item->cc_ysize = 26;
  item->cc_store = XCreatePixmap(display, item->view, item->xsize, 26, Xcil_depth);
}


/*------------------------------------------------------------------------*
 * 画像を解放する
 *------------------------------------------------------------------------*/
void item__free_image(item)
     ObItemRec *item;
{
  Image.free(item->img);
}


/*------------------------------------------------------------------------*
 * 表示用の画像を作成する
 *------------------------------------------------------------------------*/
void item__make_image(item)
     ObItemRec *item;
{
  Image.copy(item->img, item->org);
  if (item->reveresed) image__reverse(item->img, item->img);
  if (item->transed)  image__liner(item->img, item->img);
}
