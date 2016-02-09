/*
 * filename : calculate.c
 * author   : Takahiro Sugiyama
 * date     : Tuesday, January 10 1995
 * describe : calculate object position, rotation, ...
 */



#include "voxeldisp.h"


extern GC paint_gc;

ulong plane_bits_list[6] = { Z0, Z1, Y0, Y1, X0, X1 };

/*--------------------*
 * 標準セルの面データ
 *--------------------*/
static float3d temp_planes[6][4] = {
  /* plane z0 */
  {{ 1, 1,-1},{-1, 1,-1},{-1,-1,-1},{ 1,-1,-1}},
  /* plane z1 */
  {{ 1, 1, 1},{ 1,-1, 1},{-1,-1, 1},{-1, 1, 1}},
  /* plane y0 */
  {{ 1,-1, 1},{ 1,-1,-1},{-1,-1,-1},{-1,-1, 1}},
  /* plane y1 */
  {{ 1, 1, 1},{-1, 1, 1},{-1, 1,-1},{ 1, 1,-1}},
  /* plane x0 */
  {{-1, 1, 1},{-1,-1, 1},{-1,-1,-1},{-1, 1,-1}},
  /* plane x1 */
  {{ 1, 1, 1},{ 1, 1,-1},{ 1,-1,-1},{ 1,-1, 1}},
};

/*------------------------*
 * 回転したセルの面データ
 *------------------------*/
static float3d coord_planes[6][4] = {
  /* plane z0 */
  {{ 0.5, 0.5,-0.5},{-0.5, 0.5,-0.5},{-0.5,-0.5,-0.5},{ 0.5,-0.5,-0.5}},
  /* plane z1 */
  {{ 0.5, 0.5, 0.5},{ 0.5,-0.5, 0.5},{-0.5,-0.5, 0.5},{-0.5, 0.5, 0.5}},
  /* plane y0 */
  {{ 0.5,-0.5, 0.5},{ 0.5,-0.5,-0.5},{-0.5,-0.5,-0.5},{-0.5,-0.5, 0.5}},
  /* plane y1 */
  {{ 0.5, 0.5, 0.5},{-0.5, 0.5, 0.5},{-0.5, 0.5,-0.5},{ 0.5, 0.5,-0.5}},
  /* plane x0 */
  {{-0.5, 0.5, 0.5},{-0.5,-0.5, 0.5},{-0.5,-0.5,-0.5},{-0.5, 0.5,-0.5}},
  /* plane x1 */
  {{ 0.5, 0.5, 0.5},{ 0.5, 0.5,-0.5},{ 0.5,-0.5,-0.5},{ 0.5,-0.5, 0.5}},
};

static float3d original_coord_planes[6][4] = {
  /* plane z0 */
  {{ 0.5, 0.5,-0.5},{-0.5, 0.5,-0.5},{-0.5,-0.5,-0.5},{ 0.5,-0.5,-0.5}},
  /* plane z1 */
  {{ 0.5, 0.5, 0.5},{ 0.5,-0.5, 0.5},{-0.5,-0.5, 0.5},{-0.5, 0.5, 0.5}},
  /* plane y0 */
  {{ 0.5,-0.5, 0.5},{ 0.5,-0.5,-0.5},{-0.5,-0.5,-0.5},{-0.5,-0.5, 0.5}},
  /* plane y1 */
  {{ 0.5, 0.5, 0.5},{-0.5, 0.5, 0.5},{-0.5, 0.5,-0.5},{ 0.5, 0.5,-0.5}},
  /* plane x0 */
  {{-0.5, 0.5, 0.5},{-0.5,-0.5, 0.5},{-0.5,-0.5,-0.5},{-0.5, 0.5,-0.5}},
  /* plane x1 */
  {{ 0.5, 0.5, 0.5},{ 0.5, 0.5,-0.5},{ 0.5,-0.5,-0.5},{ 0.5,-0.5, 0.5}},
};


/*========================================================================*
 * all_draw_voxel_object : ボクセルのオブジェクトを表示する。
 *========================================================================*/
void all_draw_voxel_object
  _P1 (( t_environment *, environment ))
{
  int i, j;
  for ( i = 0; i < 6; i++ )
    for ( j = 0; j < 4; j++ )
      coord_planes[i][j] = original_coord_planes[i][j];
  get_surface( environment );
  rotate_object( environment );
  get_planes( environment );
  paint_planes( environment );
}



/*===================================================================*
 * get_gray_core_object : 濃淡ボクセルの占有ピクセルだけを抽出する。
 *===================================================================*/
voxel get_gray_core_object
  _P1 (( voxel, the_object ))
{
  voxel core_voxel;
  long x, y, z;
  long xsize, ysize, zsize;
  bit1 ***core_data;
  uchar ***data;

  xsize = Voxel.xsize( the_object );
  ysize = Voxel.ysize( the_object );
  zsize = Voxel.zsize( the_object );
  data  = (uchar ***)Voxel.data( the_object );

  core_voxel = Voxel.createMake( "Core", Bit1, xsize, ysize, zsize );
  core_data = (bit1 ***)Voxel.data( core_voxel );

  for ( z = 0; z < zsize; z++ )
    for ( y = 0; y < ysize; y++ )
      for ( x = 0; x < xsize; x++ )
	core_data[z][y][x] = ( data[z][y][x] ) ? 1 : 0;

  return core_voxel;
}

/*===================================================================*
 * get_rgb_core_object : RGBボクセルの占有ピクセルだけを抽出する。
 *===================================================================*/
voxel get_rgb_core_object
  _P1 (( voxel, the_object ))
{
  voxel core_voxel;
  long x, y, z;
  long xsize, ysize, zsize;
  bit1 ***core_data;
  uchar3 ***data;

  xsize = Voxel.xsize( the_object );
  ysize = Voxel.ysize( the_object );
  zsize = Voxel.zsize( the_object );
  data  = (uchar3 ***)Voxel.data( the_object );

  core_voxel = Voxel.createMake( "Core", Bit1, xsize, ysize, zsize );
  core_data = (bit1 ***)Voxel.data( core_voxel );

  for ( z = 0; z < zsize; z++ )
    for ( y = 0; y < ysize; y++ )
      for ( x = 0; x < xsize; x++ )
	{
	  uchar3 *ptr = &data[z][y][x];
	  core_data[z][y][x] = ( ptr->at[0] || ptr->at[1] || ptr->at[2] ) ? 1 : 0;
	}

  return core_voxel;
}

/*==============================================================*
 * get_only_object : オブジェクトの占有ピクセルだけを抽出する。
 *==============================================================*/
voxel get_only_object
  _P2 (( voxel, the_object ),
       ( int *, do_free    ))
{
  long type;
  voxel core_object;

  type = Voxel.type( the_object );
  switch ( type )
    {
    case Bit1:
      core_object = the_object;
      *do_free = 0;
      break;
    case UChar:
      core_object = get_gray_core_object( the_object );
      *do_free = 1;
      break;
    case UChar3:
      core_object = get_rgb_core_object( the_object );
      *do_free = 1;
      break;
    default:
      fprintf( stderr, "not yet supported\n" );
      exit( -1 );
    }

  return core_object;
}




/*====================================*
 * get_surface : 表面セルを求める
 *====================================*/
void get_surface
  _P1 (( t_environment *, environment ))
{
  int i;

  long nplane;

  long ncell;
  t_cell *cell_list;
  double center_x, center_y, center_z;

  voxel the_object, surface;
  register int x, y, z;
  int xsize, ysize, zsize;
  register bit1 ***data, ***surface_data;
  int do_free;

  if ( DEBUG ) fprintf( stderr, "begin of procedure `get_surface'\n" );

  the_object = get_only_object( environment->object, &do_free );

  cell_list = environment->cell_list;

  data  = (bit1 ***)Voxel.data( the_object );
  xsize = Voxel.xsize( the_object );
  ysize = Voxel.ysize( the_object );
  zsize = Voxel.zsize( the_object );

  surface = Voxel.createMake( "surface", Bit1, xsize, ysize, zsize );
  surface_data = (bit1 ***)Voxel.data( surface );

  /* 表示できるセルだけを抽出する */
  ncell = 0;
  for ( z = 0; z < zsize; z++ )
    for ( y = 0; y < ysize; y++ )
      for ( x = 0; x < xsize; x++ )
	{
	  if ( data[z][y][x] == 0 ) continue;
	  if ((z == 0) || (y == 0) || (x == 0) ||
	      ( z == (zsize-1)) || ( y == (ysize-1)) || ( x == (xsize-1)))
	    {
	      surface_data[z][y][x] = 1;
	      ncell++;
	      continue;
	    }
	  if ( data[z][y][x] && (data[z-1][y][x]==0 || data[z+1][y][x]==0 ||
				 data[z][y-1][x]==0 || data[z][y+1][x]==0 ||
				 data[z][y][x-1]==0 || data[z][y][x+1]==0 ))
	    {
	      surface_data[z][y][x] = 1;
	      ncell++;
	    }
	}

  if ( DEBUG ) fprintf( stderr, "    number of cell = %d\n", ncell );

  /* セルのリストを作る */
  i = 0; nplane = 0;
  cell_list = typerenew1( cell_list, ncell, t_cell );
  center_x = xsize / 2;
  center_y = ysize / 2;
  center_z = zsize / 2;
  for ( z = 0; z < zsize; z++ )
    for ( y = 0; y < ysize; y++ )
      for ( x = 0; x < xsize; x++ )
	if ( surface_data[z][y][x] )
	  {
	    register t_cell *a_cell = &cell_list[i++];
	    ulong bits;

	    a_cell->index.x = x;
	    a_cell->index.y = y;
	    a_cell->index.z = z;
	    a_cell->coord.x = x - center_x;
	    a_cell->coord.y = y - center_y;
	    a_cell->coord.z = z - center_z;

	    /* 表面の表示可能面の計算 */
	    bits = 0x3f; nplane += 6;
	    if (( z > 0 ) && data[z-1][y][x]) { bits &= ~Z0; nplane--;}
	    if (( z < (zsize-1)) && data[z+1][y][x]) { bits &= ~Z1; nplane--; }
	    if (( y > 0 ) && data[z][y-1][x]) { bits &= ~Y0; nplane--; }
	    if (( y < (ysize-1)) && data[z][y+1][x]) { bits &= ~Y1; nplane--; }
	    if (( x > 0 ) && data[z][y][x-1]) { bits &= ~X0; nplane--; }
	    if (( x < (xsize-1)) && data[z][y][x+1]) { bits &= ~X1; nplane--; }
	    a_cell->plane_bits = bits;
	  }
  if ( do_free ) Voxel.destroy( the_object );
  Voxel.destroy( surface );

  environment->ncell = ncell;
  environment->cell_list = cell_list;

  environment->nplane = nplane;
  if ( DEBUG ) fprintf( stderr, "    number of plane = %d\n", nplane );

  if ( DEBUG ) fprintf( stderr, "end of procedure `get_surface'\n" );
}



/*---------------------------*
 * compare_cell : セルの比較
 *---------------------------*/
static int compare_cell
  _P2 (( t_cell *, a ),
       ( t_cell *, b ))
{
  return (int)(b->coord.z - a->coord.z);
}

/*==================================*
 * rotate_object : セルを回転する。
 *==================================*/
void rotate_object
  _P1 (( t_environment *, environment ))
{
  int i, j;

  float3d *angle;
  float3d radian;
  long ncell;
  t_cell *cell_list;

  double sx,cx,sy,cy,sz,cz;
  double t00,t01,t02;
  double t10,t11,t12;
  double t20,t21,t22;
  double x,y,z;

  angle = &environment->angle;
  ncell = environment->ncell;
  cell_list = environment->cell_list;

  radian.x = M_PI * angle->x / 180.0;
  radian.y = M_PI * angle->y / 180.0;
  radian.z = M_PI * angle->z / 180.0;

  sx = sin(radian.x); cx = cos(radian.x);
  sy = sin(radian.y); cy = cos(radian.y);
  sz = sin(radian.z); cz = cos(radian.z);

  t00 = cz * cy;
  t01 = -sz*cx+cz*sy*sx;
  t02 = sz*sx+cx*cz*sy;

  t10 = sz*cy;
  t11 = cz*cx+sz*sy*sx;
  t12 = -sx*cz+sz*sy*cx;

  t20 = -sy;
  t21 = cy*sx;
  t22 = cy*cx;

  for ( i = 0; i < ncell; i++ )
    {
      register t_cell *a_cell = &cell_list[i];
      x = a_cell->coord.x;
      y = a_cell->coord.y;
      z = a_cell->coord.z;
      a_cell->coord.x = t00*x + t01*y + t02*z;
      a_cell->coord.y = t10*x + t11*y + t12*z;
      a_cell->coord.z = t20*x + t21*y + t22*z;
    }

  qsort( cell_list, ncell, sizeof(t_cell), compare_cell );

  /*----------------------------*
   * 標準セルの回転した面を計算
   *----------------------------*/
  for ( j = 0; j < 6; j++ )
    for ( i = 0; i < 4; i++ )
      {
	x = coord_planes[j][i].x;
	y = coord_planes[j][i].y;
	z = coord_planes[j][i].z;
	coord_planes[j][i].x = ( t00*x + t01*y + t02*z );
	coord_planes[j][i].y = ( t10*x + t11*y + t12*z );
	coord_planes[j][i].z = ( t20*x + t21*y + t22*z );
      }

  /*------------------*
   * 表示可能面の計算
   *------------------*/
  environment->plane_bits = 0;
  for ( i = 0; i < 6; i++ )
    {
      double x0, y0, x1, y1, x2, y2;
      double D;
      x0 = coord_planes[i][0].x; y0 = coord_planes[i][0].y;
      x1 = coord_planes[i][1].x; y1 = coord_planes[i][1].y;
      x2 = coord_planes[i][2].x; y2 = coord_planes[i][2].y;
      D = ( x1 - x0 ) * ( y2 - y0 ) - ( x2 - x0 ) * ( y1 - y0 );
      if ( D > 0 ) environment->plane_bits |= plane_bits_list[i];
    }
  angle->x = 0;
  angle->y = 0;
  angle->z = 0;
}



/*---------------------------------*
 * compare_plane : プレーンの比較
 *---------------------------------*/
static int compare_plane
  _P2 (( register t_plane *, a ),
       ( register t_plane *, b ))
{
  return (int)(20 * (b->gravity - a->gravity));
}

/*====================================*
 * get_planes : 表示する面を求める。
 *====================================*/
void get_planes
  _P1 (( t_environment *, environment ))
{
  register int i, j;
  ulong all_planes;

  long icell, ncell;
  t_cell *cell_list;

  long iplane, nplane;
  t_plane *plane_list;

  if ( DEBUG ) fprintf( stderr, "begin of procedure `get_planes'\n" );

  nplane = environment->nplane;
  ncell = environment->ncell;
  cell_list = environment->cell_list;
  plane_list = environment->plane_list;

  nplane = 0;
  for ( icell = 0; icell < ncell; icell++ )
    {
      all_planes = environment->plane_bits & cell_list[icell].plane_bits;
      for ( j = 0; j < 6; j++ )
	if ( all_planes & plane_bits_list[j] ) nplane++;
    }

  plane_list = typerenew1( plane_list, nplane, t_plane );

  if ( DEBUG ) fprintf( stderr, "    number of plane = %d\n", nplane );

  iplane = 0;
  for ( icell = 0; icell < ncell; icell++ )
    {
      register t_cell *a_cell = &cell_list[icell];

      all_planes = environment->plane_bits & a_cell->plane_bits;
      for ( j = 0; j < 6; j++ )
	if ( all_planes & plane_bits_list[j] )
	  {
	    register t_plane *a_plane = &plane_list[iplane++];
	    double grav_sum = 0;
	    for ( i = 0; i < 4; i++ )
	      {
		a_plane->at[i].x = coord_planes[j][i].x + a_cell->coord.x;
		a_plane->at[i].y = coord_planes[j][i].y + a_cell->coord.y;
		a_plane->at[i].z = coord_planes[j][i].z + a_cell->coord.z;
		grav_sum += a_plane->at[i].z;
	      }
	    a_plane->no = j;
	    a_plane->pindex = &a_cell->index;
	    a_plane->gravity = grav_sum / 4;
	  }
    }
  environment->plane_list = plane_list;
  environment->nplane = nplane;

  qsort( plane_list, nplane, sizeof(t_plane), compare_plane );

  if ( DEBUG ) fprintf( stderr, "end of procedure `get_planes'\n" );
}



/*==============================================*
 * calc_strength : 面の明るさの強度をもとめる。
 *==============================================*/
#define STRENGTH_BIAS 0.05
void calc_strength
  _P1 (( t_environment *, environment ))
{
  register int i;
  double x0, y0, z0, x1, y1, z1, x3, y3, z3;
  double strength;
  double lx, ly, lz, ll;
  double dx, dy, dz, dd;
  unsigned long foreground;

  if ( DEBUG ) fprintf( stderr, "begin of procedure `calc_strength'\n" );

  lx = -environment->light.x;
  ly = environment->light.y;
  lz = environment->light.z;
  ll = sqrt( lx*lx + ly*ly + lz*lz );

  if ( DEBUG ) fprintf( stderr, "    vector of light = {%.2f, %.2f, %.2f}\n", lx, ly, lz );

  for ( i = 0; i < 6; i++ )
    {
      x0 = coord_planes[i][0].x; y0 = coord_planes[i][0].y; z0 = coord_planes[i][0].z;
      x1 = coord_planes[i][1].x; y1 = coord_planes[i][1].y; z1 = coord_planes[i][1].z;
      x3 = coord_planes[i][3].x; y3 = coord_planes[i][3].y; z3 = coord_planes[i][3].z;
      dx = ( y1 - y0 ) * ( z3 - z0 ) - ( y3 - y0 ) * ( z1 - z0 );
      dy = ( z1 - z0 ) * ( x3 - x0 ) - ( z3 - z0 ) * ( x1 - x0 );
      dz = ( x1 - x0 ) * ( y3 - y0 ) - ( x3 - x0 ) * ( y1 - y0 );
      dd = sqrt( dx*dx + dy*dy + dz*dz );
      strength = ( dx*lx + dy*ly + dz*lz ) / ( dd * ll );

      strength = (1 - STRENGTH_BIAS) * strength + STRENGTH_BIAS;
      if ( strength < 0 ) strength = 0;
      if ( strength >= 1 ) strength = 0.999;

      environment->strength_list[i] = (environment->light_flag) ? strength : 1;

      switch ( environment->visual )
	{
	case StaticGray: /* 2 Planes */
	  XSetFillStyle( environment->display, environment->gc_list[i], FillTiled );
	  foreground = 5 * strength;
	  XSetTile( environment->display, environment->gc_list[i], Xcil_gray_xpixmap[ foreground ] );
	  break;
	case PseudoColor: /* 8 Planes */
	  XSetFillStyle( environment->display, environment->gc_list[i], FillSolid );
	  foreground = 255 * strength;
	  if (DEBUG) fprintf( stderr, "strength[%d] = %.3f[%d] -> %d\n",
			     i, strength, foreground, XLUT_PC_UChar[ foreground ] );
	  XSetForeground( environment->display, environment->gc_list[i],
			  XLUT_PC_UChar[ foreground ] );
	  break;
	case GrayScale: /* 8 Planes */
	  XSetFillStyle( environment->display, environment->gc_list[i], FillSolid );
	  foreground = 255 * strength;
	  XSetForeground( environment->display, environment->gc_list[i],
			  XLUT_GS_UChar[ foreground ] );
	  break;
	case TrueColor: /* 24 Planes */
	  XSetFillStyle( environment->display, environment->gc_list[i], FillSolid );
	  foreground = 255 * strength;
	  if (DisplayPlanes(environment->display, DefaultScreen(environment->display)) == 15)
	    {
	      foreground >>= 3;
	      XSetForeground( environment->display, environment->gc_list[i],
			      (unsigned long)(foreground | ( foreground << 5 ) | ( foreground << 10 )) );
	    }
	  else
	    {
	      XSetForeground( environment->display, environment->gc_list[i],
			      (unsigned long)(foreground | ( foreground << 8 ) | ( foreground << 16 )) );
	    }
	  break;
	}
    }
  if ( DEBUG ) fprintf( stderr, "end of procedure `calc_strength'\n" );
}



/*====================================*
 * paint_planes_bit1 : プレーンを塗る
 *====================================*/
void paint_planes_bit1
  _P1 (( t_environment *, environment ))
{
  int i;

  Window w;
  Display *d;
  GC *gc_list;

  long wxsize, wysize;

  long nplane;
  t_plane *plane_list;

  double x, y, z;
  double scale, range, wide;
  double D;

  d = environment->display;
  w = environment->window;
  gc_list = environment->gc_list;
  scale = environment->scale;

  XcilGetWindowSize( d, w, &wxsize, &wysize );

  nplane = environment->nplane;
  plane_list = environment->plane_list;

  if ( DEBUG ) fprintf( stderr, "    number of plane = %d\n", nplane );

  range = Voxel.xsize( environment->object ) * environment->range;
  for ( i = 0; i < nplane; i++ )
    {
      register t_plane *a_plane = &plane_list[i];
      XPoint points[5];

      z = a_plane->at[0].z;
      wide = range / ( z + range );
      x = a_plane->at[0].x * wide;
      y = a_plane->at[0].y * wide;
      points[0].x = points[4].x = scale * x + wxsize/2;
      points[0].y = points[4].y = wysize/2 - scale * y;

      z = a_plane->at[1].z;
      wide = range / ( z + range );
      x = a_plane->at[1].x * wide;
      y = a_plane->at[1].y * wide;
      points[1].x = scale * x + wxsize/2;
      points[1].y = wysize/2 - scale * y;

      z = a_plane->at[2].z;
      wide = range / ( z + range );
      x = a_plane->at[2].x * wide;
      y = a_plane->at[2].y * wide;
      points[2].x = scale * x + wxsize/2;
      points[2].y = wysize/2 - scale * y;

      z = a_plane->at[3].z;
      wide = range / ( z + range );
      x = a_plane->at[3].x * wide;
      y = a_plane->at[3].y * wide;
      points[3].x = scale * x + wxsize/2;
      points[3].y = wysize/2 - scale * y;

      if ( environment->surface_flag )
	XFillPolygon( d, w, gc_list[a_plane->no], points, 5, Convex, CoordModeOrigin );
      if ( environment->wireframe_flag )
	XDrawLines( d, w, Xcil_black_gc, points, 5, CoordModeOrigin );
    }
  if ( DEBUG ) fprintf( stderr, "end of procedure `paint_planes'\n" );
}



/*======================================*
 * paint_planes_uchar : プレーンを塗る
 *======================================*/
void paint_planes_uchar
  _P1 (( t_environment *, environment ))
{
  int i;

  Window w;
  Display *d;
  GC *gc_list;

  uchar ***data;
  double *strength_list;
  long wxsize, wysize;

  long nplane;
  t_plane *plane_list;

  double x, y, z;
  double scale, range, wide;
  double D;

  d = environment->display;
  w = environment->window;
  gc_list = environment->gc_list;
  scale = environment->scale;
  strength_list = environment->strength_list;

  XcilGetWindowSize( d, w, &wxsize, &wysize );

  nplane = environment->nplane;
  plane_list = environment->plane_list;

  if ( DEBUG ) fprintf( stderr, "    number of plane = %d\n", nplane );

  range = Voxel.xsize( environment->object ) * environment->range;
  data = (uchar ***)Voxel.data( environment->object );
  for ( i = 0; i < nplane; i++ )
    {
      register t_plane *a_plane = &plane_list[i];
      XPoint points[5];

      z = a_plane->at[0].z;
      wide = range / ( z + range );
      x = a_plane->at[0].x * wide;
      y = a_plane->at[0].y * wide;
      points[0].x = points[4].x = scale * x + wxsize/2;
      points[0].y = points[4].y = wysize/2 - scale * y;

      z = a_plane->at[1].z;
      wide = range / ( z + range );
      x = a_plane->at[1].x * wide;
      y = a_plane->at[1].y * wide;
      points[1].x = scale * x + wxsize/2;
      points[1].y = wysize/2 - scale * y;

      z = a_plane->at[2].z;
      wide = range / ( z + range );
      x = a_plane->at[2].x * wide;
      y = a_plane->at[2].y * wide;
      points[2].x = scale * x + wxsize/2;
      points[2].y = wysize/2 - scale * y;

      z = a_plane->at[3].z;
      wide = range / ( z + range );
      x = a_plane->at[3].x * wide;
      y = a_plane->at[3].y * wide;
      points[3].x = scale * x + wxsize/2;
      points[3].y = wysize/2 - scale * y;

      if ( environment->surface_flag )
	{
	  int3d *pindex = a_plane->pindex;
	  long gray = data[pindex->z][pindex->y][pindex->x];
	  paint_surface_uchar(d,w,points,gray,strength_list[a_plane->no]);
	}
      if ( environment->wireframe_flag )
	XDrawLines( d, w, Xcil_black_gc, points, 5, CoordModeOrigin );
    }
  if ( DEBUG ) fprintf( stderr, "end of procedure `paint_planes'\n" );
}



/*======================================*
 * paint_planes_uchar3 : プレーンを塗る
 *======================================*/
void paint_planes_uchar3
  _P1 (( t_environment *, environment ))
{
  int i;

  Window w;
  Display *d;

  register uchar3 ***data;
  double *strength_list;
  long wxsize, wysize;

  long nplane;
  t_plane *plane_list;

  double x, y, z;
  double scale, range, wide;
  double D;

  d = environment->display;
  w = environment->window;
  scale = environment->scale;
  strength_list = environment->strength_list;

  XcilGetWindowSize( d, w, &wxsize, &wysize );

  nplane = environment->nplane;
  plane_list = environment->plane_list;

  if ( DEBUG ) fprintf( stderr, "    number of plane = %d\n", nplane );

  range = Voxel.xsize( environment->object ) * environment->range;
  data = (uchar3 ***)Voxel.data( environment->object );
  for ( i = 0; i < nplane; i++ )
    {
      register t_plane *a_plane = &plane_list[i];
      XPoint points[5];

      z = a_plane->at[0].z;
      wide = range / ( z + range );
      x = a_plane->at[0].x * wide;
      y = a_plane->at[0].y * wide;
      points[0].x = points[4].x = scale * x + wxsize/2;
      points[0].y = points[4].y = wysize/2 - scale * y;

      z = a_plane->at[1].z;
      wide = range / ( z + range );
      x = a_plane->at[1].x * wide;
      y = a_plane->at[1].y * wide;
      points[1].x = scale * x + wxsize/2;
      points[1].y = wysize/2 - scale * y;

      z = a_plane->at[2].z;
      wide = range / ( z + range );
      x = a_plane->at[2].x * wide;
      y = a_plane->at[2].y * wide;
      points[2].x = scale * x + wxsize/2;
      points[2].y = wysize/2 - scale * y;

      z = a_plane->at[3].z;
      wide = range / ( z + range );
      x = a_plane->at[3].x * wide;
      y = a_plane->at[3].y * wide;
      points[3].x = scale * x + wxsize/2;
      points[3].y = wysize/2 - scale * y;

      if ( environment->surface_flag )
	{
	  int3d *pindex = a_plane->pindex;
	  uchar3 *ptr = &data[pindex->z][pindex->y][pindex->x];
	  paint_surface_uchar3(d,w,points,ptr,strength_list[a_plane->no]);
	}
      if ( environment->wireframe_flag )
	XDrawLines( d, w, Xcil_black_gc, points, 5, CoordModeOrigin );
    }
  XSetForeground( d, paint_gc, WhitePixel( d, 0 ) );
  if ( DEBUG ) fprintf( stderr, "end of procedure `paint_planes'\n" );
}



/*================================*
 * paint_planes : プレーンを塗る
 *================================*/
void paint_planes
  _P1 (( t_environment *, environment ))
{
  long type;

  set_wait_cursor( environment );
  if ( DEBUG ) fprintf( stderr, "begin of procedure `paint_planes'\n" );

  calc_strength( environment );

  type = Voxel.type( environment->object );
  switch ( type )
    {
    case Bit1:
      paint_planes_bit1( environment );
      break;
    case UChar:
      paint_planes_uchar( environment );
      break;
    case UChar3:
      paint_planes_uchar3( environment );
      break;
    default:
      fprintf( stderr, "not yet supported.\n" );
      exit( -1 );
    }
  reset_wait_cursor( environment );
}



/*============================================*
 * set_wait_cursor : 待ちカーソルを設定する。
 *============================================*/
void set_wait_cursor
  _P1 (( t_environment *, environment ))
{
  Display *display;
  Window window;

  display = environment->display;
  window = environment->window;
  XDefineCursor( display, window, Xcil_wait_cursor );

  window = environment->panel;
  XDefineCursor( display, window, Xcil_wait_cursor );

  window = environment->open_dialog;
  XDefineCursor( display, window, Xcil_wait_cursor );
}



/*============================================*
 * reset_wait_cursor : 待ちカーソルをもどす。
 *============================================*/
void reset_wait_cursor
  _P1 (( t_environment *, environment ))
{
  Display *display;
  Window window;

  display = environment->display;
  window = environment->window;
  XDefineCursor( display, window, Xcil_pointer_cursor );

  window = environment->panel;
  XDefineCursor( display, window, Xcil_pointer_cursor );

  window = environment->open_dialog;
  XDefineCursor( display, window, Xcil_pointer_cursor );
}
