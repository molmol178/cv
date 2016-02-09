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
 * cgdrawpoint : $@E@$rBG$D!#(J
 *=========================*/
#define CGdrawpoint( data, gc, x, y ) ( ( gcRec * )gc )->func( data, gc, x, y )
void cgdrawpoint
  P4 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , x    ),
      ( long    , y    ))

/*==============================*
 * cgdrawpoints : $@J#?tE@$rBG$D!#(J
 *==============================*/
void cgdrawpoints
  P4 (( char     *, data ),
      ( gcRec    *, gc   ),
      ( long      , num  ),
      ( cg_point *, list ))

/*========================*
 * cgdrawline : $@@~$rIA$/!#(J
 *========================*/
void cgdrawline
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , x1   ),
      ( long    , y1   ),
      ( long    , x2   ),
      ( long    , y2   ))

/*=============================*
 * cgdrawlines : $@J#?t@~$rIA$/!#(J
 *=============================*/
void cgdrawlines
  P4 (( char     *, data ),
      ( gcRec    *, gc   ),
      ( long      , num  ),
      ( cg_point *, list ))

/*==================================*
 * cgdrawsegments : $@@~J,$rJ#?tIA$/!#(J
 *==================================*/
void cgdrawsegments
  P4 (( char       *, data ),
      ( gcRec      *, gc   ),
      ( long        , num  ),
      ( cg_segment *, list ))

/*===============================*
 * cgdrawrectangle : $@6k7A$rIA$/!#(J
 *===============================*/
void cgdrawrectangle
  P6 (( char   *, data   ),
      ( gcRec  *, gc     ),
      ( long    , x      ),
      ( long    , y      ),
      ( long    , width  ),
      ( long    , height ))

/*====================================*
 * cgdrawrectangles : $@6k7A$rJ#?tIA$/!#(J
 *====================================*/
void cgdrawrectangles
  P4 (( char         *, data ),
      ( gcRec        *, gc   ),
      ( long          , num  ),
      ( cg_rectangle *, list ))

/*=======================*
 * cgdrawbox : $@H"$rIA$/!#(J
 *=======================*/
void cgdrawbox
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , x1   ),
      ( long    , y1   ),
      ( long    , x2   ),
      ( long    , y2   ))

/*=============================*
 * cgdrawboxes : $@H"$rJ#?tIA$/!#(J
 *=============================*/
void cgdrawboxes
  P4 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , num  ),
      ( cg_box *, list ))

/*==========================*
 * cgdrawcircle : $@1_$rIA$/!#(J
 *==========================*/
void cgdrawcircle
  P5 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , cx   ),
      ( long    , cy   ),
      ( long    , r    ))

/*===========================================*
 * cgdrawcircle2 : $@Cf?4$H0lE@$rDL$k1_$rIA$/!#(J
 *===========================================*/
void cgdrawcircle2
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , cx   ),
      ( long    , cy   ),
      ( long    , x    ),
      ( long    , y    ))

/*=====================================*
 * cgdrawcircle3 : $@FsE@$rDL$k1_$rIA$/!#(J
 *=====================================*/
void cgdrawcircle3
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , x1   ),
      ( long    , y1   ),
      ( long    , x2   ),
      ( long    , y2   ))

/*=======================================*
 * cgdrawcircle4 : $@6k7A$K@\$9$k1_$rIA$/!#(J
 *=======================================*/
void cgdrawcircle4
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , x1   ),
      ( long    , y1   ),
      ( long    , x2   ),
      ( long    , y2   ))

/*=========================================*
 * cgdrawellipse : $@Cf?4$HH>7B$NBJ1_$rIA$/!#(J
 *=========================================*/
void cgdrawellipse
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , cx   ),
      ( long    , cy   ),
      ( long    , rx   ),
      ( long    , ry   ))

/*==============================================*
 * cgdrawellipse2 : $@Cf?4$H0lE@$rDL$kBJ1_$rIA$/!#(J
 *==============================================*/
void cgdrawellipse2
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , cx   ),
      ( long    , cy   ),
      ( long    , x1   ),
      ( long    , y1   ))

/*============================================*
 * cgdrawellipse3 : $@6k7A$KFb@\$9$kBJ1_$rIA$/!#(J
 *============================================*/
void cgdrawellipse3
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , x1   ),
      ( long    , y1   ),
      ( long    , x2   ),
      ( long    , y2   ))

/*============================================*
 * cgdrawellipse4 : $@6k7A$KFb@\$9$kBJ1_$rIA$/!#(J
 *============================================*/
void cgdrawellipse4
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , x1   ),
      ( long    , y1   ),
      ( long    , x2   ),
      ( long    , y2   ))

/*===============================*
 * cgdrawpolygon : $@B?3Q7A$rIA$/!#(J
 *===============================*/
void cgdrawpolygon
  P4 (( char     *, data ),
      ( gcRec    *, gc   ),
      ( long      , num  ),
      ( cg_point *, list ))

/*=============================*
 * cgfillbox : $@H"$rEI$j$D$V$9!#(J
 *=============================*/
void cgfillbox
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , x1   ),
      ( long    , y1   ),
      ( long    , x2   ),
      ( long    , y2   ))

/*================================*
 * cgfillcircle : $@1_$rEI$j$D$V$9!#(J
 *================================*/
void cgfillcircle
  P5 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , cx   ),
      ( long    , cy   ),
      ( long    , r    ))

/*=================================================*
 * cgfillcircle2 : $@Cf?4$H0lE@$rDL$k1_$rEI$j$D$V$9!#(J
 *=================================================*/
void cgfillcircle2
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , cx   ),
      ( long    , cy   ),
      ( long    , x    ),
      ( long    , y    ))

/*=====================================*
 * cgfillcircle3 : $@FsE@$rDL$k1_$rIA$/!#(J
 *=====================================*/
void cgfillcircle3
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , x1   ),
      ( long    , y1   ),
      ( long    , x2   ),
      ( long    , y2   ))

/*=======================================*
 * cgfillcircle4 : $@6k7A$K@\$9$k1_$rIA$/!#(J
 *=======================================*/
void cgfillcircle4
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , x1   ),
      ( long    , y1   ),
      ( long    , x2   ),
      ( long    , y2   ))

/*===============================================*
 * cgfillellipse : $@Cf?4$HH>7B$NBJ1_$rEI$j$D$V$9!#(J
 *===============================================*/
void cgfillellipse
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , cx   ),
      ( long    , cy   ),
      ( long    , rx   ),
      ( long    , ry   ))

/*====================================================*
 * cgfillellipse2 : $@Cf?4$H0lE@$rDL$kBJ1_$rEI$j$D$V$9!#(J
 *====================================================*/
void cgfillellipse2
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , cx   ),
      ( long    , cy   ),
      ( long    , x1   ),
      ( long    , y1   ))

/*==================================================*
 * cgfillellipse3 : $@6k7A$KFb@\$9$kBJ1_$rEI$j$D$V$9!#(J
 *==================================================*/
void cgfillellipse3
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , x1   ),
      ( long    , y1   ),
      ( long    , x2   ),
      ( long    , y2   ))

/*==================================================*
 * cgfillellipse4 : $@6k7A$KFb@\$9$kBJ1_$rEI$j$D$V$9!#(J
 *==================================================*/
void cgfillellipse4
  P6 (( char   *, data ),
      ( gcRec  *, gc   ),
      ( long    , x1   ),
      ( long    , y1   ),
      ( long    , x2   ),
      ( long    , y2   ))

/*=====================================*
 * cgfillpolygon : $@B?3Q7A$rEI$j$D$V$9!#(J
 *=====================================*/
void cgfillpolygon
  P4 (( char     *, data ),
      ( gcRec    *, gc   ),
      ( long      , num  ),
      ( cg_point *, list ))



#endif __cglib_h
