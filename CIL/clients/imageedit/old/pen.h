/*
 * pen.h
 */



#include "Edit.h"



#define Pen__id(e) e->pen.menu->id
#define pen__size(e) e->pen.size
#define pen__func(e) e->pen.func
#define pen__store_mode(e) e->pen.store_mode
#define pen__gradation(e) e->pen.gradation


enum {SHAPE_BOX, SHAPE_CIRCLE};

enum {BIN_0, BIN_1};

enum {GRAY_P,GRAY_S,GRAY_M,COLOR_P,COLOR_S,COLOR_M};
enum {GCH1,GCH2,CCH1,CCH2};

enum {DXS_P,DXS_O,DXS_M,DYS_P,DYS_O,DYS_M};
enum {DS_M,DS_O,DS_P};



void pen__open(Edit e);
long pen__close(Edit e,xw_object obj);

void pen__draw(char **data,long x,long y,long value);
void pen__bit1_draw(uchar **data,long x,long y,long value);
void pen__make(Edit e);
void pen__draw_bit1(bit1 **data,long x,long y);
void pen__draw_uchar(uchar **data,long x,long y);
void pen__draw_uchar3(uchar3 **data,long x,long y);

long pen__size_step(Edit e,xw_object obj,XEvent *event,long value);
long pen__shape_sel(Edit e,xw_object obj,XEvent *event,long value);
long pen__func_sel(Edit e,xw_object obj,XEvent *event,long value);
long pen__select_onoff(Edit e,xw_object obj,XEvent *event,long value);
long pen__negative_sel(Edit e,xw_object obj,XEvent *event,long value);
long pen__gradation_onoff(Edit e,xw_object obj,XEvent *event,long value);
long pen__percent_step(Edit e,xw_object obj,XEvent *event,long value);

long pen__binary_sel(Edit e,xw_object obj,XEvent *event,long value);
long pen__gray_step(Edit e,xw_object obj,XEvent *event,long value);
long pen__color_step(Edit e,xw_object obj,XEvent *event,long value);

long pen__gradation_direction_sel(Edit e,xw_object obj,XEvent *event,long value);
long pen__diffusion_size_step(Edit e,xw_object obj,XEvent *event,long value);
long pen__diffusion_strength_step(Edit e,xw_object obj,XEvent *event,long value);

long pen__gray_sel(Edit e,xw_object obj,XEvent *event,long value);
long pen__red_sel(Edit e,xw_object obj,XEvent *event,long value);
long pen__green_sel(Edit e,xw_object obj,XEvent *event,long value);
long pen__blue_sel(Edit e,xw_object obj,XEvent *event,long value);
long pen__gradation_gray_sel(Edit e,xw_object obj,XEvent *event,long value);
long pen__gradation_color_sel(Edit e,xw_object obj,XEvent *event,long value);

long pen__expose(Edit e,xw_object obj,XEvent *event,long value);

long pen__change_select(Edit e,xw_object obj,XEvent *event,long value);
long pen__change_gradation(Edit e,xw_object obj,XEvent *event,long value);

long pen__palet_set_put(Edit e,xw_object obj,XEvent *event,long value);
void pen__redraw_palet(Edit e);
long pen__palet_step(Edit e,xw_object obj,XEvent *event,long value);

void pen__palet_load(Edit e);
void pen__palet_save(Edit e);



/*
 * pen.h
 */
void pen__initialize_pen(Edit e);
void pen__initialize_images(Edit e);
void pen__initialize_palet(Edit e);
void pen__set_size(Edit e,long size);
void pen__set_shape(Edit e,long shape);
void pen__set_func(Edit e,long func);
void pen__set_binary(Edit e,long binary);
void pen__set_select(Edit e,long select);
void pen__set_negative(Edit e,long negative);
void pen__set_gradation(Edit e,long gradation);
void pen__set_gradation_direction(Edit e,long dir);

void pen__set_diffusion_size(Edit e,long xsize,long ysize);
void pen__set_diffusion_strength(Edit e,long strength);
void pen__set_gray(Edit e,long gray);
void pen__set_color(Edit e,uchar3 color);
void pen__set_rgb(Edit e,long r,long g,long b);
void pen__set_reverse(Edit e,long reverse);
void pen__set_percent(Edit e,long percent);
void pen__set_select_first_gray(Edit e,uchar gray);
void pen__set_select_last_gray(Edit e,uchar gray);
void pen__set_select_first_color(Edit e,uchar3 color);
void pen__set_select_last_color(Edit e,uchar3 color);
void pen__set_gradation_first_gray(Edit e,uchar gray);
void pen__set_gradation_last_gray(Edit e,uchar gray);
void pen__set_gradation_first_color(Edit e,uchar3 color);
void pen__set_gradation_last_color(Edit e,uchar3 color);

void pen__set_palet(Edit e,xw_object obj,long i_offset,uchar3 color);
void pen__set_palet_offset(Edit e,long offset);
void pen__put_palet(Edit e,long i_offset);
void pen__get_color(Edit e,long x,long y);
void pen__redraw_binary(Edit e);
void pen__redraw_gray(Edit e);
void pen__redraw_color(Edit e);
void pen__redraw_gray_bar(Edit e);
void pen__redraw_color_bar(Edit e);
void pen__redraw_gradation_gray(Edit e);
void pen__redraw_gradation_color(Edit e);
void pen__redraw_select_gray(Edit e);
void pen__redraw_select_color(Edit e);
void pen__make_gradation_gray(Edit e);
void pen__make_gradation_color(Edit e);
