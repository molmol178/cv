/*
 * filename : DataPlot.h
 * author   : Takahiro Sugiyama
 * date     : 1993/02/05
 */



#ifndef _DataPlot_h



#include "Xw/XwObject.h"
#include "Xw/XwHard.h"
#include "Xw/XwSoft.h"
#include "misc/strlib.h"
#include "misc/memlib.h"
#include "misc/optlib.h"
#include "misc/filelib.h"



typedef struct {
  xw_world world;
  xw_object graph;

  GC labelGC;
  GC titleGC;
  GC graphGC;

  char *config_name;

  char *label_font_name;
  char *title_font_name;

  char *title;
  char *ylabel;
  char *xlabel;
  double ymin;
  double ymax;

  long dot_width;
  long moat_width;
  long line_width;
  long line_length;
  long string_space;

  long data_num;
  struct adata {
    char *  filename;
    char *  title;
    char *  shape_name;
    xw_shape shape;
    long     fill;

    long dot_num;
    char *dot;

    long coord_num;
    struct acoord {
      char *x;
      double  y;
    } *coord;
  } *data;

  long xsize;
  long ysize;

  long xsizegraph;
  long ysizegraph;

  long xcoord_num;
  char **xcoord;

  long ycoord_num;
  char **ycoord;

  long xlabel_width;
  long xlabel_height;

  long ylabel_width;
  long ylabel_height;

  long xcoord_width;
  long xcoord_height;

  long ycoord_width;
  long ycoord_height;

  long file_width;
  long file_height;

  long xlabel_x;
  long xlabel_y;
  long xlabel_xsize;
  long xlabel_ysize;

  long ylabel_x;
  long ylabel_y;
  long ylabel_xsize;
  long ylabel_ysize;

  long xcoord_x;
  long xcoord_y;
  long xcoord_xsize;
  long xcoord_ysize;

  long ycoord_x;
  long ycoord_y;
  long ycoord_xsize;
  long ycoord_ysize;

  long file_x;
  long file_y;
  long file_xsize;
  long file_ysize;

  long graph_x;
  long graph_y;
  long graph_xsize;
  long graph_ysize;

  long left_pool;
  long right_pool;
  long top_pool;
  long bot_pool;

  long win_width;
  long win_height;
} DataPlotRec,*DataPlot;



void dataPlot_init  P ((DataPlot self,long argc,char *argv[]));
void dataPlot_load  P ((DataPlot self));
void dataPlot_open  P ((DataPlot self));
void dataPlot_disp  P ((DataPlot self));
void dataPlot_exec  P ((DataPlot self));
void dataPlot_close P ((DataPlot self));
void dataPlot_exit  P ((DataPlot self));
void dataPlot_print P ((DataPlot self));



#endif _DataPlot_h
