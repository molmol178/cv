/*
 * edit.h
 */



#define edit__scale(e) (e)->edit.scale
#define edit__grid_on(e) e->edit.grid_on
#define edit__snap_on(e) e->edit.snap_on
#define edit__grid_xsize(e) e->edit.grid_xsize
#define edit__grid_ysize(e) e->edit.grid_ysize



void edit__set_size(Edit e,long xsize,long ysize);
long edit__type_sel(Edit e,xw_object obj,XEvent *event,long type);
long edit__xsize_step(Edit e,xw_object obj,XEvent *event,long type);
long edit__ysize_step(Edit e,xw_object obj,XEvent *event,long type);



long edit__shift_step(Edit e,xw_object obj,XEvent *event,long value);
long edit__scale_step(Edit e,xw_object obj,XEvent *event,long value);
long edit__angle_step(Edit e,xw_object obj,XEvent *event,long value);
long edit__grid_sel(Edit e,xw_object obj,XEvent *event,long value);
long edit__snap_sel(Edit e,xw_object obj,XEvent *event,long value);
long edit__grid_x_step(Edit e,xw_object obj,XEvent *event,long value);
long edit__grid_y_step(Edit e,xw_object obj,XEvent *event,long value);
long edit__gravity_sel(Edit e,xw_object obj,XEvent *event,long value);
void edit__set_grid_size(Edit e,long xsize,long ysize);

long edit__close(Edit e);
void edit__open(Edit e);
long edit__redraw(Edit e);
long edit__undo(Edit e);
long edit__swap(Edit e);
long edit__store(Edit e);
long edit__unzoom(Edit e);
void edit__redraw_scale(Edit e);

void edit__set_grid_state(Edit e,long state);
void edit__set_snap_state(Edit e,long state);
void edit__set_type_state(Edit e,long state);
void edit__set_gravity_state(Edit e,long state);

#define edit__reverse_grid(e)    edit__set_grid_state(e,XW_REVERSE)
#define edit__reverse_snap(e)    edit__set_snap_state(e,XW_REVERSE)
#define edit__reverse_type(e)    edit__set_type_state(e,XW_REVERSE)
#define edit__reverse_gravity(e) edit__set_gravity_state(e,XW_REVERSE)

#define edit__frame_grid(e)    edit__set_grid_state(e,XW_FRAME)
#define edit__frame_snap(e)    edit__set_snap_state(e,XW_FRAME)
#define edit__frame_type(e)    edit__set_type_state(e,XW_FRAME)
#define edit__frame_gravity(e) edit__set_gravity_state(e,XW_FRAME)

long edit__something(Edit e,xw_object obj,XEvent *event,long value);
void edit__fill_all(Edit e,image orig);
void edit__create(Edit e,image orig);
void edit__draw_grid(Edit e);
void edit__draw_grid_area(Edit e,long x1,long y1,long x2,long y2);

void edit__set_scale(Edit e,long scale);



enum {
  SH_UP,
  SH_DOWN,
  SH_LEFT,
  SH_RIGHT,
  ROT_R90,
  ROT_L90,
  ROT_XREV,
  ROT_YREV,
  ROT_XYREV,
  ROT_ANY,
  RESIZE_SZ,
  RESIZE_OBJ,
  FILL_ALL,
  CLEAR_ALL,
  CREATE
};
