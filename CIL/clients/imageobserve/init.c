/*
 * filename : initialize.c
 * author   : Takahiro Sugiyama
 * date     : Saturday, February 8 1997
 * describe : 初期化
 */


#include "observe.h"


/*------------------------------------------------------------------------*
 * オプションの設定
 *------------------------------------------------------------------------*/
char *opt_item[] = {
  "入力:*:*:(画像) 入力画像のファイル名",
  "ヘルプ:-h:ヘルプメッセージを表示する",
  "画素サイズ:-s:1:8:(整数) 観察ウィンドウに表示する画素の大きさ",
  "フォント:-f:1:a14:(フォント名) 値表示におけるフォントの指定",

  "*) ウィンドウの表示位置に関するオプション.",
  "タイル状:-T:ウィンドウをタイル状に並べる（初期）",
  "重ねる:-O:ウィンドウを重ねて表示する",
  "中心:-C:ウィンドウを中心に並べる",
  "観察窓右:-R:観察窓を表示ウィンドウの右に並べる（初期）",
  "観察窓下:-B:観察窓を表示ウィンドウの下に並べる",

};


/*------------------------------------------------------------------------*
 * 初期化
 *------------------------------------------------------------------------*/
long MAX_WX;
long MAX_WY;
void initialize(ObserveRec *observe, long argc, char *argv[])
{
  long i;
  char **files;

  int xoff = 10, yoff = 10;
  int xstep = 25, ystep = 25;
  int xoffstart = xoff;
  int ymax;
  int scale;

  int right_ob, bottom_ob;

  OPTION_SETUP(opt_item);
  if (argc == 1) optusage(1);
  if (optspecified("ヘルプ")) optmanual(1);

  scale = optvalueint("画素サイズ");
  observe->num = optvaluenum("入力");
  files = optvaluelist("入力");
  observe->list = typenew1(observe->num, ObItemRec);

  observe->display = XcilOpenDisplay(0);
  observe->context = XcilUniqueContext();
  item__make_pixel_pixmap(observe->display);
  set_value_font(observe->display, optvalue("フォント"));

  XcilGetWindowSize(observe->display,Xcil_root_window,&MAX_WX,&MAX_WY);

  if (optspecified("中心"))
    {
      xoff = MAX_WX;
      yoff = MAX_WY;
    }
  else
  if (optspecified("重ねる"))
    {
       xoff = 10; yoff = 10;
       xstep = 15; ystep = 25;
       xoffstart = xoff;
    }
  else
    {
       xoff = 5; yoff = 5;
       xstep = 5; ystep = 25;
       ymax = 0;
    }

  right_ob = 2;
  bottom_ob = 1;
  if (optspecified("観察窓下"))
    {bottom_ob = 2; right_ob = 1;}

  for (i = 0; i < observe->num; i++)
    {
      ObItemRec *item = &observe->list[i];

      item__init(item, files[i], scale);
      if (item__load(item) == 0) continue;
      text__set_size(item, observe->display);
      item__fit_region_size(item);

      if (optspecified("中心"))
	{
	  xoff = (MAX_WX - right_ob*item->xsize)/2;
	  yoff = (MAX_WY - bottom_ob*item->ysize)/2;
	}

      item__open_window(item, observe->display, observe->context, xoff, yoff);
      item__make_image(item);
      item__disp(item, observe->display);

      if (optspecified("重ねる"))
	{
	  xoff += xstep; yoff += ystep;
	  if ((xoff > (MAX_WX-item->xsize/2)) ||
	      (yoff > (MAX_WY-item->ysize/2)))
	    { xoffstart += 4*xstep; xoff = xoffstart; yoff = 10; }
	}
      else
      if (!optspecified("中心"))
	{
	  xoff += right_ob * (xstep + item->xsize);
	  if (ymax < item->ysize) ymax = item->ysize;
	  if (xoff > (MAX_WX-item->xsize/bottom_ob))
	    {
	      yoff += bottom_ob * (ystep + ymax);
	      xoff = 5;
	      if (yoff > (MAX_WY-ymax/right_ob)) {yoff = 5;}
	      ymax = 0;
	    }
	}
    }
}
