/*
 * filename : voxeldisp.h
 * author   : Takahiro Sugiyama
 * date     : Tuesday, January 10 1995
 * describe : voxeldisp header file
 */



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "misc/optlib.h"
#include "misc/typelib.h"
#include "misc/memlib.h"
#include "misc/strlib.h"
#include "misc/filelib.h"
#include "Xcil/Xcil.h"
#include "Voxel.h"
#include "Xcil/XImage.h"


#define DEBUG 0


typedef struct {
  int x, y, z;
} int3d;

typedef struct {
  float x, y, z;
} float3d;

typedef struct {
  float3d at[4];
  short no;
  float gravity;
  int3d *pindex;
} t_plane;

#define X0 (1<<0)
#define X1 (1<<1)
#define Y0 (1<<2)
#define Y1 (1<<3)
#define Z0 (1<<4)
#define Z1 (1<<5)

typedef struct {
  int3d index;
  float3d coord;
  ulong plane_bits;
} t_cell;

typedef struct {
  char name_of_input[512];
  char name_of_output[512];

  XcilFileDialogAttributes attribute;

  voxel object;
  Display *display;
  XContext context;
  Window window;
  Window panel;
  Window open_dialog;
  GC gc_list[6];
  long visual; /* PseudoColor, GrayScale, StaticGray(2bit), TrueColor */

  XcilFileDialogAttributes save_attribute;
  Window save_dialog;

  int3d origin; /* 中心位置 */
  float3d light;
  float3d angle;

  long    ncell;
  t_cell *cell_list;

  ulong    plane_bits;
  double   strength_list[6];
  long     nplane;
  t_plane *plane_list;

  long scale_index;
  double scale;
  double range;

  long wireframe_flag;
  long surface_flag;
  long light_flag;
  long center_flag;
} t_environment;



/*========================================================================*
 * all_draw_voxel_object : ボクセルのオブジェクトを表示する。
 *========================================================================*/
void all_draw_voxel_object
  P1 (( t_environment *, environment ))

/*==========================================*
 * get_surface : セルリストを計算する。
 *==========================================*/
void get_surface
  P1 (( t_environment *, environment ))

/*==================================*
 * rotate_object : セルを回転する。
 *==================================*/
void rotate_object
  P1 (( t_environment *, environment ))

/*====================================*
 * get_planes : 表示する面を求める。
 *====================================*/
void get_planes
  P1 (( t_environment *, environment ))

/*================================*
 * paint_planes : プレーンを塗る
 *================================*/
void paint_planes
  P1 (( t_environment *, environment ))



#define INITIAL_SCALE_INDEX 7


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*
 * xevent.c
 *%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*====================================================*
 * initialize_xevent : X のイベント関係を初期化する。
 *====================================================*/
void initialize_xevent
  P1 (( t_environment *, environment ))

typedef t_paint_surface_uchar
  P5 (( Display *, display  ),
      ( Window   , window   ),
      ( XPoint * , points   ),
      ( long     , gray     ),
      ( double   , strength ))

extern t_paint_surface_uchar *paint_surface_uchar;

typedef t_paint_surface_uchar3
  P5 (( Display *, display  ),
      ( Window   , window   ),
      ( XPoint * , points   ),
      ( uchar3 * , pixel_p  ),
      ( double   , strength ))

extern t_paint_surface_uchar3 *paint_surface_uchar3;




/*=======================================*
 * initialize_environment : 環境の初期化
 *=======================================*/
void initialize_environment
  P1 (( t_environment *, environment ))



/*============================================*
 * set_wait_cursor : 待ちカーソルを設定する。
 *============================================*/
void set_wait_cursor
  P1 (( t_environment *, environment ))


/*============================================*
 * reset_wait_cursor : 待ちカーソルをもどす。
 *============================================*/
void reset_wait_cursor
  P1 (( t_environment *, environment ))



/*==========================================================*
 * write_postscript : ポストスクリプトファイルに書き出す。
 *==========================================================*/
void write_postscript
  P1 (( t_environment *, environment ))
