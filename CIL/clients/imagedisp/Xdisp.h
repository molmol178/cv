/*
 * filename : Xdisp.h
 * author   : Takahiro Sugiyama
 * date     : from 1993
 * describe : header file
 */


#ifndef __imagedisp_h
#define __imagedisp_h

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

#include "server/cilserver.h"


typedef struct XdispAppRec {
  Display *display;
  XContext context;

  long debug;
  long scaling;
  long max_wx,max_wy;

  long   wx,wy;  /* Window Size */
  long   ox,oy;  /* Image Offset */
  long   rx,ry;  /* Image Window Size */
  double px,py;  /* Pixel Size */

  long num;
  struct ITEM {
    char     name[128];
    char     filename[128];
    cilRec   *info;

    long      reveresed;
    long      transed;
    long      histflat;
    image     org;
    image     img;

    Window win;

    long   wx,wy;  /* Window Size */
    long   ox,oy;  /* Image Offset */
    long   rx,ry;  /* Image Window Size */
    double px,py;  /* Pixel Size */
  } *list;
} *XdispApp;


void initialize P ((XdispApp app,long argc,char *argv[]));
void load       P ((XdispApp app));
void xopen      P ((XdispApp app));
void disp       P ((XdispApp app));
void exec       P ((XdispApp app));
void destrize   P ((XdispApp app));


void bestmatch_rxry P ((struct ITEM *item));
void bestmatch_wxwy P ((struct ITEM *item));

#define SCALE_WINDOW 0
#define SCALE_PIXEL  1
#define SCALE_IMAGE  2
#define SCALE_DEFINE 3


long server_exec
  P2 (( XdispApp     , app  ),
      ( struct ITEM *, item ))


#endif  __imagedisp_h
