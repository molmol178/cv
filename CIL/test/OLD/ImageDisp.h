/*
 * filename : ImageDisp.h
 * author   : Takahiro Sugiyama
 * date     : 1993/10/05
 */



#ifndef __ImageDisp_h
#define __ImageDisp_h



#include "Image.h"



/*================*
 * CIL Event Type 
 *================*/
enum {
  NoEvent = 0,
  ImageExpose = 1,
  MouseMove,
  MousePress,
  MouseRelease,
  MouseEnter,
  MouseLeave,
  KeyboardPress,
  KeyboardRelease,
};
#define ImageExposeMask     ( 1 << ImageExpose     )
#define MouseMoveMask       ( 1 << MouseMove       )
#define MousePressMask      ( 1 << MousePress      )
#define MouseReleaseMask    ( 1 << MouseRelease    )
#define MouseEnterMask      ( 1 << MouseEnter      )
#define MouseLeaveMask      ( 1 << MouseLeave      )
#define KeyboardPressMask   ( 1 << KeyboardPress   )
#define KeyboardReleaseMask ( 1 << KeyboardRelease )



/*================================*
 * CILEvent : CIL Event structure
 *================================*/
typedef struct {
  long type;
  long detail;
  long x, y;
  long window_x, window_y;
} CILAnyEvent;

typedef struct {
  long type;
  long button;
  long x, y;
  long window_x, window_y;
} CILMouseEvent;

typedef struct {
  long type;
  long ascii;
  long x, y;
  long window_x, window_y;
  long keysym;
} CILKeyboardEvent;

typedef union {
  long type;
  CILAnyEvent any;
  CILMouseEvent mouse;
  CILKeyboardEvent keyboard;
} CILEvent;



long ImageDispExec
  P2 (( image , self ),
      ( char *, opt  ))

long ImageDispOK
  P1 (( image, self ))

void ImageDispQuit
  P1 (( image, self ))

void ImageDispDraw
  P1 (( image, self ))

long ImageDispSelectEvent
  P2 (( image, self ),
      ( long , mask ))

long ImageDispNextEvent
  P2 (( image     , self  ),
      ( CILEvent *, event ))

long ImageDispCheckEvent
  P2 (( image     , self  ),
      ( CILEvent *, event ))



/*=========================*
 * ImageDisp request types
 *=========================*/
enum {
  XCIL_OK = 1,
  XCIL_QUIT,
  XCIL_DRAW,
  XCIL_SELECT_EVENT
};



#endif /* __ImageDisp_h */
