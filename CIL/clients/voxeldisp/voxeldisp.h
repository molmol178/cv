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

  int3d origin; /* �濴���� */
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
 * all_draw_voxel_object : �ܥ�����Υ��֥������Ȥ�ɽ�����롣
 *========================================================================*/
void all_draw_voxel_object
  P1 (( t_environment *, environment ))

/*==========================================*
 * get_surface : ����ꥹ�Ȥ�׻����롣
 *==========================================*/
void get_surface
  P1 (( t_environment *, environment ))

/*==================================*
 * rotate_object : ������ž���롣
 *==================================*/
void rotate_object
  P1 (( t_environment *, environment ))

/*====================================*
 * get_planes : ɽ�������̤���롣
 *====================================*/
void get_planes
  P1 (( t_environment *, environment ))

/*================================*
 * paint_planes : �ץ졼����ɤ�
 *================================*/
void paint_planes
  P1 (( t_environment *, environment ))



#define INITIAL_SCALE_INDEX 7


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*
 * xevent.c
 *%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*====================================================*
 * initialize_xevent : X �Υ��٥�ȴط����������롣
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
 * initialize_environment : �Ķ��ν����
 *=======================================*/
void initialize_environment
  P1 (( t_environment *, environment ))



/*============================================*
 * set_wait_cursor : �Ԥ�������������ꤹ�롣
 *============================================*/
void set_wait_cursor
  P1 (( t_environment *, environment ))


/*============================================*
 * reset_wait_cursor : �Ԥ�����������ɤ���
 *============================================*/
void reset_wait_cursor
  P1 (( t_environment *, environment ))



/*==========================================================*
 * write_postscript : �ݥ��ȥ�����ץȥե�����˽񤭽Ф���
 *==========================================================*/
void write_postscript
  P1 (( t_environment *, environment ))
