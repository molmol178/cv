/*
 * filename : observe.h
 * author   : Takahiro Sugiyama
 * date     : Sunday, February 9 1997
 * describe : Observer Header File
 */


#ifndef __observe_h
#define __observe_h

#ifndef __STDC__
#include "X11/Xlib.h"
#include "../Image.h"
#else
#include "Image.h"
#endif __STDC__

#include "Image/Funcs.h"
#include "Xcil/XImage.h"
#include "Xcil/Xcil.h"
#include <stdio.h>
#include <X11/Xatom.h>

#include "misc/strlib.h"
#include "misc/memlib.h"
#include "misc/optlib.h"


typedef struct {
  char name[128];
  char filename[128];

  long reveresed;
  long transed;

  image org;
  image img;

  Window view;
  long xsize, ysize;		/* 表示用ウィンドウのサイズ（画像サイズ） */

  Window eye;
  long eye_xsize, eye_ysize;	/* Eye Window Size */
  long region_xoffset, region_yoffset; /* Image Offset */
  long region_xsize, region_ysize; /* Image Window Size */
  long pixel_size;		/* Pixel Size */
  long text_xsize, text_ysize;	/* Text Size */
  long pixel_xsize, pixel_ysize; /* Real Pixel Size */

  int region_flag;		/* Region display flag */
  Pixmap store;
  Pixmap buffer;
  long store_x, store_y;
  long store_xsize, store_ysize;
  int value_mode;

  int do_display_center_coord;
  int do_display_mouse_coord;

  int cc_xsize, cc_ysize;
  Pixmap cc_store;

} ObItemRec;

typedef struct {
  Display *display;
  XContext context;

  long num;
  ObItemRec *list;

  /* Grab */
  int state, grab;
  int grab_x, grab_y;
  int grab_xoffset, grab_yoffset;

} ObserveRec;


void initialize(ObserveRec *observe,long argc,char *argv[]);
void load(ObserveRec *observe);
void xopen(ObserveRec *observe);
void disp(ObserveRec *observe);
void exec(ObserveRec *observe);
void destrize(ObserveRec *observe);


#define SCALE_WINDOW 0
#define SCALE_PIXEL  1
#define SCALE_IMAGE  2
#define SCALE_DEFINE 3


#endif  __observe_h
