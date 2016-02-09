/*
 * filename : xevent.c
 * author   : Takahiro Sugiyama
 * date     : Tuesday, January 10 1995
 * describe : X Event 関係
 */



#include "voxeldisp.h"



/*================================*
 * add_panel : パネルを表示する。
 *================================*/
void add_panel
  P1 (( t_environment *, environment ))



#define ANGLE_SKIP_90 90
#define ANGLE_SKIP_45 45
#define ANGLE_SKIP_10 10



GC paint_gc;



static long expose_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  paint_planes( environment );
  return 0;
}



static long open_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  XcilMapWindow( environment->display, environment->open_dialog );
  return 0;
}



static long psout_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  XcilMapWindow( environment->display, environment->save_dialog );
  return 0;
}



static long quit_hdr
  _P4 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ))
{
  return 1;
}



#define MAX_SCALE_INDEX XcilNumber( scale_list )
double scale_list[] = { 1.0/2.0, 2.0/3.0, 1.0, 1.5, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0, 10.0,
			12.0, 14.0, 16.0 };
static long zoom_in_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  long scale_index = environment->scale_index;
  if ( DEBUG ) fprintf( stderr, "command::Zoom in\n" );
  scale_index ++;
  if ( scale_index >= MAX_SCALE_INDEX ) return 0;
  environment->scale_index = scale_index;
  environment->scale = scale_list[scale_index];
  XClearWindow( d, environment->window );
  paint_planes( environment );
  return 0;
}



static long zoom_out_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  long scale_index = environment->scale_index;
  if ( DEBUG ) fprintf( stderr, "command::Zoom out\n" );
  scale_index --;
  if ( scale_index < 0 ) return 0;
  environment->scale_index = scale_index;
  environment->scale = scale_list[scale_index];
  XClearWindow( d, environment->window );
  paint_planes( environment );
  return 0;
}



static long zoom_0_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  environment->scale_index = INITIAL_SCALE_INDEX;
  environment->scale = scale_list[environment->scale_index];
  XClearWindow( d, environment->window );
  paint_planes( environment );
  return 0;
}



static long wireframe_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  environment->wireframe_flag = 1;
  environment->surface_flag = 0;
  XClearWindow( d, environment->window );
  paint_planes( environment );
  return 0;
}



static long wiresurface_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  environment->wireframe_flag = 1;
  environment->surface_flag = 1;
  XClearWindow( d, environment->window );
  paint_planes( environment );
  return 0;
}



static long surface_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  environment->wireframe_flag = 0;
  environment->surface_flag = 1;
  XClearWindow( d, environment->window );
  paint_planes( environment );
  return 0;
}



static long rotate_and_redraw
  _P4 (( t_environment *, environment ),
       ( double, x ),
       ( double, y ),
       ( double, z ))
{
  environment->angle.x = x;
  environment->angle.y = y;
  environment->angle.z = z;
  rotate_object( environment );
  get_planes( environment );
  XClearWindow( environment->display, environment->window );
  paint_planes( environment );

  return 0;
}



static long xangleadd0_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  return rotate_and_redraw( environment, (double)ANGLE_SKIP_10, (double)0, (double)0 );
}



static long xanglesub0_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  return rotate_and_redraw( environment, (double)-ANGLE_SKIP_10, (double)0, (double)0 );
}



static long yangleadd0_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  return rotate_and_redraw( environment, (double)0, (double)ANGLE_SKIP_10, (double)0 );
}



static long yanglesub0_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  return rotate_and_redraw( environment, (double)0, (double)-ANGLE_SKIP_10, (double)0 );
}



static long zangleadd0_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  return rotate_and_redraw( environment, (double)0, (double)0, (double)ANGLE_SKIP_10 );
}



static long zanglesub0_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  return rotate_and_redraw( environment, (double)0, (double)0, (double)-ANGLE_SKIP_10 );
}



static long xangleadd1_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  return rotate_and_redraw( environment, (double)ANGLE_SKIP_45, (double)0, (double)0 );
}



static long xanglesub1_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  return rotate_and_redraw( environment, (double)-ANGLE_SKIP_45, (double)0, (double)0 );
}



static long yangleadd1_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  return rotate_and_redraw( environment, (double)0, (double)ANGLE_SKIP_45, (double)0 );
}



static long yanglesub1_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  return rotate_and_redraw( environment, (double)0, (double)-ANGLE_SKIP_45, (double)0 );
}



static long zangleadd1_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  return rotate_and_redraw( environment, (double)0, (double)0, (double)ANGLE_SKIP_45 );
}



static long zanglesub1_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  return rotate_and_redraw( environment, (double)0, (double)0, (double)-ANGLE_SKIP_45 );
}



static long xangleadd2_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  return rotate_and_redraw( environment, (double)ANGLE_SKIP_90, (double)0, (double)0 );
}



static long xanglesub2_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  return rotate_and_redraw( environment, (double)-ANGLE_SKIP_90, (double)0, (double)0 );
}



static long yangleadd2_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  return rotate_and_redraw( environment, (double)0, (double)ANGLE_SKIP_90, (double)0 );
}



static long yanglesub2_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  return rotate_and_redraw( environment, (double)0, (double)-ANGLE_SKIP_90, (double)0 );
}



static long zangleadd2_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  return rotate_and_redraw( environment, (double)0, (double)0, (double)ANGLE_SKIP_90 );
}



static long zanglesub2_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  return rotate_and_redraw( environment, (double)0, (double)0, (double)-ANGLE_SKIP_90 );
}



static XcilCallback event_list[] = {
  { Expose, (xcil_callback)expose_hdr, 0 },
};



static XcilCallback key_event_list[] = {
  { 'q', (xcil_callback)quit_hdr, 0 },
  { 'o', (xcil_callback)open_hdr, 0 },
  { '+', (xcil_callback)zoom_in_hdr, 0 },
  { '-', (xcil_callback)zoom_out_hdr, 0 },
  { 'w', (xcil_callback)wireframe_hdr, 0 },
  { 'b', (xcil_callback)wiresurface_hdr, 0 },
  { 's', (xcil_callback)surface_hdr, 0 },

  { 'k', (xcil_callback)xangleadd0_hdr, 0 },
  { 'j', (xcil_callback)xanglesub0_hdr, 0 },
  { 'h', (xcil_callback)yangleadd0_hdr, 0 },
  { 'l', (xcil_callback)yanglesub0_hdr, 0 },
  { 'u', (xcil_callback)zangleadd0_hdr, 0 },
  { 'i', (xcil_callback)zanglesub0_hdr, 0 },

  { 'K', (xcil_callback)xangleadd1_hdr, 0 },
  { 'J', (xcil_callback)xanglesub1_hdr, 0 },
  { 'H', (xcil_callback)yangleadd1_hdr, 0 },
  { 'L', (xcil_callback)yanglesub1_hdr, 0 },
  { 'U', (xcil_callback)zangleadd1_hdr, 0 },
  { 'I', (xcil_callback)zanglesub1_hdr, 0 },
};



/*==================================*
 * UChar型の面を表示する関数の変数
 *==================================*/
t_paint_surface_uchar *paint_surface_uchar;

#define RGB_TO_Y(r,g,b) ((30*(long)(r)+59*(long)(g)+11*(long)(b))/100)

void paint_surface_uchar_StaticGray
  _P5 (( Display *, display  ),
       ( Window   , window   ),
       ( XPoint * , points   ),
       ( long     , gray     ),
       ( double   , strength ))
{
  unsigned long pixel;
  pixel = 5 * gray * strength / 256;
  XSetTile( display, paint_gc, Xcil_gray_xpixmap[pixel] );
  XFillPolygon( display, window, paint_gc,
	        points, 5, Convex, CoordModeOrigin );
}

void paint_surface_uchar_PseudoColor
  _P5 (( Display *, display  ),
       ( Window   , window   ),
       ( XPoint * , points   ),
       ( long     , gray     ),
       ( double   , strength ))
{
  unsigned long pixel = gray * strength;

  XSetForeground( display, paint_gc, XLUT_PC_UChar[ pixel ] );
  XFillPolygon( display, window, paint_gc,
	        points, 5, Convex, CoordModeOrigin );
}



void paint_surface_uchar_GrayScale
  _P5 (( Display *, display  ),
       ( Window   , window   ),
       ( XPoint * , points   ),
       ( long     , gray     ),
       ( double   , strength ))
{
  unsigned long pixel = gray * strength;

  XSetForeground( display, paint_gc, XLUT_GS_UChar[ pixel ] );
  XFillPolygon( display, window, paint_gc,
	        points, 5, Convex, CoordModeOrigin );
}

void paint_surface_uchar_TrueColor
  _P5 (( Display *, display  ),
       ( Window   , window   ),
       ( XPoint * , points   ),
       ( long     , gray     ),
       ( double   , strength ))
{
  unsigned long pixel;

  pixel = gray | ( gray << 8 ) | ( gray << 16 );
  XSetForeground( display, paint_gc, pixel );
  XFillPolygon( display, window, paint_gc,
	        points, 5, Convex, CoordModeOrigin );
}


void paint_surface_uchar_TrueColor15
  _P5 (( Display *, display  ),
       ( Window   , window   ),
       ( XPoint * , points   ),
       ( long     , gray     ),
       ( double   , strength ))
{
  unsigned long pixel;
  gray >>= 3;

  pixel = gray | ( gray << 5 ) | ( gray << 10 );
  XSetForeground( display, paint_gc, pixel );
  XFillPolygon( display, window, paint_gc,
	        points, 5, Convex, CoordModeOrigin );
}



/*==================================*
 * UChar3型の面を表示する関数の変数
 *==================================*/
t_paint_surface_uchar3 *paint_surface_uchar3;

#define RGB_TO_Y(r,g,b) ((30*(long)(r)+59*(long)(g)+11*(long)(b))/100)

void paint_surface_uchar3_StaticGray
  _P5 (( Display *, display  ),
       ( Window   , window   ),
       ( XPoint * , points   ),
       ( uchar3 * , pixel_p  ),
       ( double   , strength ))
{
  unsigned long pixel0 = pixel_p->at[0] * strength;
  unsigned long pixel1 = pixel_p->at[1] * strength;
  unsigned long pixel2 = pixel_p->at[2] * strength;
  unsigned long pixel;

  pixel = 5 * RGB_TO_Y( pixel0, pixel1, pixel2 ) / 256;
  XSetTile( display, paint_gc, Xcil_gray_xpixmap[pixel] );
  XFillPolygon( display, window, paint_gc,
	        points, 5, Convex, CoordModeOrigin );
}

void paint_surface_uchar3_PseudoColor
  _P5 (( Display *, display  ),
       ( Window   , window   ),
       ( XPoint * , points   ),
       ( uchar3 * , pixel_p  ),
       ( double   , strength ))
{
  unsigned long pixel0 = pixel_p->at[0] * strength;
  unsigned long pixel1 = pixel_p->at[1] * strength;
  unsigned long pixel2 = pixel_p->at[2] * strength;
  unsigned long r, g, b;

  r = XLUT_PCR[ pixel0 ];
  g = XLUT_PCG[ pixel1 ];
  b = XLUT_PCB[ pixel2 ];
  XSetForeground( display, paint_gc, XLUT_PC_UChar3[ r ][ g ][ b ] );
  XFillPolygon( display, window, paint_gc,
	        points, 5, Convex, CoordModeOrigin );
}



void paint_surface_uchar3_GrayScale
  _P5 (( Display *, display  ),
       ( Window   , window   ),
       ( XPoint * , points   ),
       ( uchar3 * , pixel_p  ),
       ( double   , strength ))
{
  unsigned long pixel0 = pixel_p->at[0] * strength;
  unsigned long pixel1 = pixel_p->at[1] * strength;
  unsigned long pixel2 = pixel_p->at[2] * strength;
  unsigned long pixel;

  pixel = RGB_TO_Y( pixel0, pixel1, pixel2 );
  XSetForeground( display, paint_gc, XLUT_GS_UChar[ pixel ] );
  XFillPolygon( display, window, paint_gc,
	        points, 5, Convex, CoordModeOrigin );
}

void paint_surface_uchar3_TrueColor
  _P5 (( Display *, display  ),
       ( Window   , window   ),
       ( XPoint * , points   ),
       ( uchar3 * , pixel_p  ),
       ( double   , strength ))
{
  unsigned long pixel0 = pixel_p->at[0] * strength;
  unsigned long pixel1 = pixel_p->at[1] * strength;
  unsigned long pixel2 = pixel_p->at[2] * strength;
  unsigned long pixel;

  {
    unsigned long r_mask = DefaultVisual(display,DefaultScreen(display))->red_mask;
    unsigned long g_mask = DefaultVisual(display,DefaultScreen(display))->green_mask;
    unsigned long b_mask = DefaultVisual(display,DefaultScreen(display))->blue_mask;
    if (r_mask & 0xff0000) pixel0 <<= 16; else
      if (r_mask & 0x00ff00) pixel0 <<= 8;
    if (g_mask & 0xff0000) pixel1 <<= 16; else
      if (g_mask & 0x00ff00) pixel1 <<= 8;
    if (b_mask & 0xff0000) pixel2 <<= 16; else
      if (b_mask & 0x00ff00) pixel2 <<= 8;
  }

  pixel = pixel0 | pixel1 | pixel2;
  XSetForeground( display, paint_gc, pixel );
  XFillPolygon( display, window, paint_gc,
	        points, 5, Convex, CoordModeOrigin );
}

void paint_surface_uchar3_TrueColor15
  _P5 (( Display *, display  ),
       ( Window   , window   ),
       ( XPoint * , points   ),
       ( uchar3 * , pixel_p  ),
       ( double   , strength ))
{
  unsigned long pixel0 = (unsigned long)(pixel_p->at[0] * strength)>>3;
  unsigned long pixel1 = (unsigned long)(pixel_p->at[1] * strength)>>3;
  unsigned long pixel2 = (unsigned long)(pixel_p->at[2] * strength)>>3;
  unsigned long pixel;

  {
    unsigned long r_mask = DefaultVisual(display,DefaultScreen(display))->red_mask;
    unsigned long g_mask = DefaultVisual(display,DefaultScreen(display))->green_mask;
    unsigned long b_mask = DefaultVisual(display,DefaultScreen(display))->blue_mask;
    if (r_mask & 0x7c00) pixel0 <<= 10; else
      if (r_mask & 0x03e0) pixel0 <<= 5;
    if (g_mask & 0x7c00) pixel1 <<= 10; else
      if (g_mask & 0x03e0) pixel1 <<= 5;
    if (b_mask & 0x7c00) pixel2 <<= 10; else
      if (b_mask & 0x03e0) pixel2 <<= 5;
  }

  pixel = pixel0 | pixel1 | pixel2;
  XSetForeground( display, paint_gc, pixel );
  XFillPolygon( display, window, paint_gc,
	        points, 5, Convex, CoordModeOrigin );
}



/*================================================*
 * setup_display : ディスプレイ用の設定を行なう。
 *================================================*/
void setup_display
  _P1 (( t_environment *, environment ))
{
  Display *d;
  Window w;
  long visual;

  d = environment->display;
  w = environment->window;
  XImageSetColormap( d, w );
  XImageSetLookupTable();
  visual = DefaultVisual(d,DefaultScreen(d))->class;
  environment->visual = visual;

  switch ( visual )
    {
    case StaticGray: /* 2 Planes */
      XSetFillStyle( d, paint_gc, FillTiled );
      paint_surface_uchar =
	(t_paint_surface_uchar *)paint_surface_uchar_StaticGray;
      paint_surface_uchar3 =
	(t_paint_surface_uchar3 *)paint_surface_uchar3_StaticGray;
      break;
    case PseudoColor: /* 8 Planes */
      paint_surface_uchar =
	(t_paint_surface_uchar *)paint_surface_uchar_PseudoColor;
      paint_surface_uchar3 =
	(t_paint_surface_uchar3 *)paint_surface_uchar3_PseudoColor;
      break;
    case GrayScale: /* 8 Planes */
      paint_surface_uchar =
	(t_paint_surface_uchar *)paint_surface_uchar_GrayScale;
      paint_surface_uchar3 =
	(t_paint_surface_uchar3 *)paint_surface_uchar3_GrayScale;
      break;
    case TrueColor: /* 24 Planes */
      if (DisplayPlanes(d, DefaultScreen(d)) == 15)
	paint_surface_uchar =
	  (t_paint_surface_uchar *)paint_surface_uchar_TrueColor15;
      else
	paint_surface_uchar =
	  (t_paint_surface_uchar *)paint_surface_uchar_TrueColor;

      if (DisplayPlanes(d, DefaultScreen(d)) == 15)
	paint_surface_uchar3 =
	  (t_paint_surface_uchar3 *)paint_surface_uchar3_TrueColor15;
      else
	paint_surface_uchar3 =
	  (t_paint_surface_uchar3 *)paint_surface_uchar3_TrueColor;
      break;
    }
}



/*====================================================*
 * initialize_xevent : X のイベント関係を初期化する。
 *====================================================*/
void initialize_xevent
  _P1 (( t_environment *, environment ))
{
  int i;
  Display *d;
  Window w;
  XContext c;
  static char title[256];

  MEMLIB_ERROR = 0;

  d = XcilOpenDisplay( 0 );
  c = XUniqueContext();
  sprintf( title, "VoxeldispII:%s", environment->name_of_input );
  w = XcilCreateWindow( d, 0, 0, 0, 600, 600, 1, title );
  for ( i = 0; i < 6; i++ )
    environment->gc_list[i] = XCreateGC( d, w, 0, 0 );
  paint_gc = XCreateGC( d, w, 0, 0 );

  XcilManagedWindow( d, w, c );

  for ( i = 0; i < XcilNumber(event_list); i++ )
    event_list[i].client_value = (caddr_t)environment;

  for ( i = 0; i < XcilNumber(key_event_list); i++ )
    key_event_list[i].client_value = (caddr_t)environment;

  XcilAddCallbacks( d, w, c, event_list, XcilNumber( event_list ) );
  XcilAddKeyCallbacks( d, w, c, key_event_list, XcilNumber( key_event_list ) );

  XcilMapWindow( d, w );

  environment->display = d;
  environment->context = c;
  environment->window = w;

  setup_display( environment );
  add_panel( environment );
}



#include "bitmap/open"
#include "bitmap/postscript"
#include "bitmap/quit"

#include "bitmap/zoomin"
#include "bitmap/zoomout"
#include "bitmap/zoom0"

#include "bitmap/wireframe"
#include "bitmap/surface"
#include "bitmap/wiresurface"

#include "bitmap/xanglesub0"
#include "bitmap/xanglesub1"
#include "bitmap/xanglesub2"
#include "bitmap/xangleadd0"
#include "bitmap/xangleadd1"
#include "bitmap/xangleadd2"

#include "bitmap/yanglesub0"
#include "bitmap/yanglesub1"
#include "bitmap/yanglesub2"
#include "bitmap/yangleadd0"
#include "bitmap/yangleadd1"
#include "bitmap/yangleadd2"

#include "bitmap/zanglesub0"
#include "bitmap/zanglesub1"
#include "bitmap/zanglesub2"
#include "bitmap/zangleadd0"
#include "bitmap/zangleadd1"
#include "bitmap/zangleadd2"


static XcilPanelItem panel_list[] = {
  {{"file",__TOP,__TOP,94,0,0,0,1},
     {XCIL_LABEL,"File"},{0,0}},

  {{"open",__TOP,__PRE,32,32},
     {XCIL_BUTTON,".","cc",(char *)open_bits,open_width,open_height},
     {(xcil_callback)open_hdr, 0}},
  {{"prin",__PRE,__SAM,32,32},
     {XCIL_BUTTON,".","cc",(char *)postscript_bits,postscript_width,postscript_height},
     {(xcil_callback)psout_hdr, 0}},
  {{"quit",__PRE,__SAM,32,32},
     {XCIL_BUTTON,".","cc",(char *)quit_bits,quit_width,quit_height},
     {(xcil_callback)quit_hdr, 0}},

  {{"info",__TOP,__PRE,94,0,0,0,1},
     {XCIL_LABEL,"Display"},{0,0}},

  {{"smal",__TOP,__PRE,32,32},
     {XCIL_BUTTON,"-","cc",(char *)zoomout_bits,zoomout_width,zoomout_height},
     {(xcil_callback)zoom_out_hdr, 0}},
  {{"midd",__PRE,__SAM,32,32},
     {XCIL_BUTTON,".","cc",(char *)zoom0_bits,zoom0_width,zoom0_height},
     {(xcil_callback)zoom_0_hdr, 0}},
  {{"larg",__PRE,__SAM,32,32},
     {XCIL_BUTTON,"+","cc",(char *)zoomin_bits,zoomin_width,zoomin_height},
     {(xcil_callback)zoom_in_hdr, 0}},

  {{"wire",__TOP,__PRE,32,32},
     {XCIL_BUTTON,".","cc",(char *)wireframe_bits,wireframe_width,wireframe_height},
     {(xcil_callback)wireframe_hdr, 0}},
  {{"both",__PRE,__SAM,32,32},
     {XCIL_BUTTON,".","cc",(char *)wiresurface_bits,wiresurface_width,wiresurface_height},
     {(xcil_callback)wiresurface_hdr, 0}},
  {{"surf",__PRE,__SAM,32,32},
     {XCIL_BUTTON,".","cc",(char *)surface_bits,surface_width,surface_height},
     {(xcil_callback)surface_hdr, 0}},

  {{"rota",__TOP,__PRE,94,0,0,0,1},
     {XCIL_LABEL,"Rotation"},{0,0}},
  {{"x+0",__TOP,__PRE,32,32},
     {XCIL_BUTTON,".","cc",(char *)xangleadd0_bits,xangleadd0_width,xangleadd0_height},
     {(xcil_callback)xangleadd0_hdr, 0}},
  {{"x+1",__PRE,__SAM,32,32},
     {XCIL_BUTTON,".","cc",(char *)xangleadd1_bits,xangleadd1_width,xangleadd1_height},
     {(xcil_callback)xangleadd1_hdr, 0}},
  {{"x+2",__PRE,__SAM,32,32},
     {XCIL_BUTTON,".","cc",(char *)xangleadd2_bits,xangleadd2_width,xangleadd2_height},
     {(xcil_callback)xangleadd2_hdr, 0}},
  {{"x-0",__TOP,__PRE,32,32},
     {XCIL_BUTTON,".","cc",(char *)xanglesub0_bits,xanglesub0_width,xanglesub0_height},
     {(xcil_callback)xanglesub0_hdr, 0}},
  {{"x-1",__PRE,__SAM,32,32},
     {XCIL_BUTTON,".","cc",(char *)xanglesub1_bits,xanglesub1_width,xanglesub1_height},
     {(xcil_callback)xanglesub1_hdr, 0}},
  {{"x-2",__PRE,__SAM,32,32},
     {XCIL_BUTTON,".","cc",(char *)xanglesub2_bits,xanglesub2_width,xanglesub2_height},
     {(xcil_callback)xanglesub2_hdr, 0}},

  {{"y+0",__TOP,__PRE,32,32},
     {XCIL_BUTTON,".","cc",(char *)yangleadd0_bits,yangleadd0_width,yangleadd0_height},
     {(xcil_callback)yangleadd0_hdr, 0}},
  {{"y+1",__PRE,__SAM,32,32},
     {XCIL_BUTTON,".","cc",(char *)yangleadd1_bits,yangleadd1_width,yangleadd1_height},
     {(xcil_callback)yangleadd1_hdr, 0}},
  {{"y+2",__PRE,__SAM,32,32},
     {XCIL_BUTTON,".","cc",(char *)yangleadd2_bits,yangleadd2_width,yangleadd2_height},
     {(xcil_callback)yangleadd2_hdr, 0}},
  {{"y-0",__TOP,__PRE,32,32},
     {XCIL_BUTTON,".","cc",(char *)yanglesub0_bits,yanglesub0_width,yanglesub0_height},
     {(xcil_callback)yanglesub0_hdr, 0}},
  {{"y-1",__PRE,__SAM,32,32},
     {XCIL_BUTTON,".","cc",(char *)yanglesub1_bits,yanglesub1_width,yanglesub1_height},
     {(xcil_callback)yanglesub1_hdr, 0}},
  {{"y-2",__PRE,__SAM,32,32},
     {XCIL_BUTTON,".","cc",(char *)yanglesub2_bits,yanglesub2_width,yanglesub2_height},
     {(xcil_callback)yanglesub2_hdr, 0}},

  {{"z+0",__TOP,__PRE,32,32},
     {XCIL_BUTTON,".","cc",(char *)zangleadd0_bits,zangleadd0_width,zangleadd0_height},
     {(xcil_callback)zangleadd0_hdr, 0}},
  {{"z+1",__PRE,__SAM,32,32},
     {XCIL_BUTTON,".","cc",(char *)zangleadd1_bits,zangleadd1_width,zangleadd1_height},
     {(xcil_callback)zangleadd1_hdr, 0}},
  {{"z+2",__PRE,__SAM,32,32},
     {XCIL_BUTTON,".","cc",(char *)zangleadd2_bits,zangleadd2_width,zangleadd2_height},
     {(xcil_callback)zangleadd2_hdr, 0}},
  {{"z-0",__TOP,__PRE,32,32},
     {XCIL_BUTTON,".","cc",(char *)zanglesub0_bits,zanglesub0_width,zanglesub0_height},
     {(xcil_callback)zanglesub0_hdr, 0}},
  {{"z-1",__PRE,__SAM,32,32},
     {XCIL_BUTTON,".","cc",(char *)zanglesub1_bits,zanglesub1_width,zanglesub1_height},
     {(xcil_callback)zanglesub1_hdr, 0}},
  {{"z-2",__PRE,__SAM,32,32},
     {XCIL_BUTTON,".","cc",(char *)zanglesub2_bits,zanglesub2_width,zanglesub2_height},
     {(xcil_callback)zanglesub2_hdr, 0}},

};



/*==========================================================*
 * add_open_dialog : ファイルオープンダイアログを設定する。
 *==========================================================*/
void add_open_dialog
  P1 (( t_environment *, environment ))

/*================================*
 * add_panel : パネルを表示する。
 *================================*/
void add_panel
  _P1 (( t_environment *, environment ))
{
  Display *d;
  XContext c;
  int i;
  Window panel;

  XcilPanel_default_border = 0;
  XcilPanel_default_xspace = 0;
  XcilPanel_default_yspace = 0;

  for ( i = 0; i < XcilNumber( panel_list ); i++ )
    panel_list[i].hdr.client_value = (caddr_t)environment;

  d = environment->display;
  c = environment->context;

  panel = XcilCreatePanelWindow( d, 0, c, 100, 100,
				panel_list,XcilNumber(panel_list),"ToolBox");

  environment->panel = panel;
  XImageSetColormap( d, panel );

  XcilMapWindow( d, panel );

  add_open_dialog( environment );
  add_save_dialog( environment );
}
