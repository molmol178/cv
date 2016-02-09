/*
 * filename : cglib.h
 * author   : Takahiro Sugiyama
 * date     : 1993/09/26
 */



#ifndef __cglib_h
#define __cglib_h



#include "../misc/comacros.h"



typedef struct {
  long x;
  long y;
} cg_point;

typedef struct {
  long x1;
  long y1;
  long x2;
  long y2;
} cg_segment;

typedef struct {
  long x1;
  long y1;
  long x2;
  long y2;
} cg_box;

typedef struct {
  long x;
  long y;
  long width;
  long height;
} cg_rectangle;



typedef struct _gcRec gcRec;

typedef void (*cg_func)
     P4 (( char   *, data ),
	 ( gcRec  *, gc   ),
	 ( long    , x    ),
	 ( long    , y    ))

struct _gcRec {
  cg_func func;
};



/*=========================*
 * cgdrawpoint : 点を打つ。
 *=========================*/
#define CGdrawpoint( data, gc, x, y ) ( ( gcRec * )gc )->func( data, gc, x, y )
void cgdrawpoint
  P4 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , x    ),
      ( long    , y    ))

/*==============================*
 * cgdrawpoints : 複数点を打つ。
 *==============================*/
void cgdrawpoints
  P4 (( char     *, data ),
      ( gcRec    *, gc   ),
      ( long      , num  ),
      ( cg_point *, list ))

/*========================*
 * cgdrawline : 線を描く。
 *========================*/
void cgdrawline
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , x1   ),
      ( long    , y1   ),
      ( long    , x2   ),
      ( long    , y2   ))

/*=============================*
 * cgdrawlines : 複数線を描く。
 *=============================*/
void cgdrawlines
  P4 (( char     *, data ),
      ( gcRec    *, gc   ),
      ( long      , num  ),
      ( cg_point *, list ))

/*==================================*
 * cgdrawsegments : 線分を複数描く。
 *==================================*/
void cgdrawsegments
  P4 (( char       *, data ),
      ( gcRec      *, gc   ),
      ( long        , num  ),
      ( cg_segment *, list ))

/*===============================*
 * cgdrawrectangle : 矩形を描く。
 *===============================*/
void cgdrawrectangle
  P6 (( char   *, data   ),
      ( gcRec  *, gc     ),
      ( long    , x      ),
      ( long    , y      ),
      ( long    , width  ),
      ( long    , height ))

/*====================================*
 * cgdrawrectangles : 矩形を複数描く。
 *====================================*/
void cgdrawrectangles
  P4 (( char         *, data ),
      ( gcRec        *, gc   ),
      ( long          , num  ),
      ( cg_rectangle *, list ))

/*=======================*
 * cgdrawbox : 箱を描く。
 *=======================*/
void cgdrawbox
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , x1   ),
      ( long    , y1   ),
      ( long    , x2   ),
      ( long    , y2   ))

/*=============================*
 * cgdrawboxes : 箱を複数描く。
 *=============================*/
void cgdrawboxes
  P4 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , num  ),
      ( cg_box *, list ))

/*==========================*
 * cgdrawcircle : 円を描く。
 *==========================*/
void cgdrawcircle
  P5 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , cx   ),
      ( long    , cy   ),
      ( long    , r    ))

/*===========================================*
 * cgdrawcircle2 : 中心と一点を通る円を描く。
 *===========================================*/
void cgdrawcircle2
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , cx   ),
      ( long    , cy   ),
      ( long    , x    ),
      ( long    , y    ))

/*=====================================*
 * cgdrawcircle3 : 二点を通る円を描く。
 *=====================================*/
void cgdrawcircle3
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , x1   ),
      ( long    , y1   ),
      ( long    , x2   ),
      ( long    , y2   ))

/*=======================================*
 * cgdrawcircle4 : 矩形に接する円を描く。
 *=======================================*/
void cgdrawcircle4
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , x1   ),
      ( long    , y1   ),
      ( long    , x2   ),
      ( long    , y2   ))

/*=========================================*
 * cgdrawellipse : 中心と半径の楕円を描く。
 *=========================================*/
void cgdrawellipse
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , cx   ),
      ( long    , cy   ),
      ( long    , rx   ),
      ( long    , ry   ))

/*==============================================*
 * cgdrawellipse2 : 中心と一点を通る楕円を描く。
 *==============================================*/
void cgdrawellipse2
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , cx   ),
      ( long    , cy   ),
      ( long    , x1   ),
      ( long    , y1   ))

/*============================================*
 * cgdrawellipse3 : 矩形に内接する楕円を描く。
 *============================================*/
void cgdrawellipse3
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , x1   ),
      ( long    , y1   ),
      ( long    , x2   ),
      ( long    , y2   ))

/*============================================*
 * cgdrawellipse4 : 矩形に内接する楕円を描く。
 *============================================*/
void cgdrawellipse4
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , x1   ),
      ( long    , y1   ),
      ( long    , x2   ),
      ( long    , y2   ))

/*===============================*
 * cgdrawpolygon : 多角形を描く。
 *===============================*/
void cgdrawpolygon
  P4 (( char     *, data ),
      ( gcRec    *, gc   ),
      ( long      , num  ),
      ( cg_point *, list ))

/*=============================*
 * cgfillbox : 箱を塗りつぶす。
 *=============================*/
void cgfillbox
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , x1   ),
      ( long    , y1   ),
      ( long    , x2   ),
      ( long    , y2   ))

/*================================*
 * cgfillcircle : 円を塗りつぶす。
 *================================*/
void cgfillcircle
  P5 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , cx   ),
      ( long    , cy   ),
      ( long    , r    ))

/*=================================================*
 * cgfillcircle2 : 中心と一点を通る円を塗りつぶす。
 *=================================================*/
void cgfillcircle2
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , cx   ),
      ( long    , cy   ),
      ( long    , x    ),
      ( long    , y    ))

/*=====================================*
 * cgfillcircle3 : 二点を通る円を描く。
 *=====================================*/
void cgfillcircle3
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , x1   ),
      ( long    , y1   ),
      ( long    , x2   ),
      ( long    , y2   ))

/*=======================================*
 * cgfillcircle4 : 矩形に接する円を描く。
 *=======================================*/
void cgfillcircle4
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , x1   ),
      ( long    , y1   ),
      ( long    , x2   ),
      ( long    , y2   ))

/*===============================================*
 * cgfillellipse : 中心と半径の楕円を塗りつぶす。
 *===============================================*/
void cgfillellipse
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , cx   ),
      ( long    , cy   ),
      ( long    , rx   ),
      ( long    , ry   ))

/*====================================================*
 * cgfillellipse2 : 中心と一点を通る楕円を塗りつぶす。
 *====================================================*/
void cgfillellipse2
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , cx   ),
      ( long    , cy   ),
      ( long    , x1   ),
      ( long    , y1   ))

/*==================================================*
 * cgfillellipse3 : 矩形に内接する楕円を塗りつぶす。
 *==================================================*/
void cgfillellipse3
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , x1   ),
      ( long    , y1   ),
      ( long    , x2   ),
      ( long    , y2   ))

/*==================================================*
 * cgfillellipse4 : 矩形に内接する楕円を塗りつぶす。
 *==================================================*/
void cgfillellipse4
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , x1   ),
      ( long    , y1   ),
      ( long    , x2   ),
      ( long    , y2   ))

/*=====================================*
 * cgfillpolygon : 多角形を塗りつぶす。
 *=====================================*/
void cgfillpolygon
  P4 (( char     *, data ),
      ( gcRec    *, gc   ),
      ( long      , num  ),
      ( cg_point *, list ))



#endif __cglib_h
