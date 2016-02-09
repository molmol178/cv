/*
 * filename : ps.c
 * author   : Takahiro Sugiyama
 * date     : Wednesday, January 18 1995
 * describe : Write PostScript File.
 * Special Thanks : Andreas Held
 */



#include "voxeldisp.h"
#include "misc/timelib.h"



/*==========================================================*
 * ps_write_plane : ポストスクリプトファイルに書き出す。
 *==========================================================*/
void ps_write_plane
  P2 (( FILE *, fp ),
      ( t_environment *, environment ))



/*==========================================================*
 * write_postscript : ポストスクリプトファイルに書き出す。
 *==========================================================*/
void write_postscript
  _P1 (( t_environment *, environment ))
{
  FILE *fp;
  long xsize, ysize;
  double *strength_list = environment->strength_list;

  long paper_width, paper_height;
  long object_x, object_y;
  long object_width, object_height;

  paper_width = 72 * 210 / 25.4;
  paper_height = 72 * 297 / 25.4;
  paper_width = 400;
  paper_height = 400;
  object_x = (paper_width - object_width) / 2;
  object_y = (paper_height - object_height) / 2;

  fp = fp_open( environment->name_of_output, "w" );

  fp_puts( fp, "%!PS-Adobe-2.0 EPSF-2.0\n" );
  fp_puts( fp, "%%Title:" );fp_printf( fp,"%s\n",environment->name_of_input );
  fp_puts( fp, "%%Creator: voxeldisp\n" );
  fp_puts( fp, "%%CreationDate: " );fp_printf(fp,"%s\n",timedate(0));
  fp_puts( fp, "%%Pages: 1\n" );
  fp_puts( fp, "%%BoundingBox: 97 221 497 621\n" );
  fp_puts( fp, "%%EndProlog\n" );
  fp_puts( fp, "%%Pages: 1 1\n" );

  XcilGetWindowSize( environment->display, environment->window,
		    &xsize, &ysize );
  fp_printf( fp, "97 221 translate\n" );
  fp_printf( fp, "%.3f %.3f scale\n", 400.0/xsize, 400.0/ysize );
  fp_printf( fp, "/g {gsave translate newpath 0 0 moveto lineto lineto lineto closepath " );
  if ( environment->wireframe_flag && environment->surface_flag )
    fp_printf( fp, "gsave setgray fill grestore 0 setgray stroke grestore} def\n" );
  else
  if ( environment->wireframe_flag )
    fp_printf( fp, "stroke grestore} def\n" );
  else
    fp_printf( fp, "setgray fill grestore} def\n" );

  fp_printf( fp, "0 setgray\n" );
  fp_printf( fp, "0 setlinewidth\n" );

  ps_write_plane( fp, environment );

  fp_puts( fp, "showpage\n" );
  fp_puts( fp, "%%Trailer\n" );

  fp_close( fp );
}



/*==========================================================*
 * ps_write_plane : ポストスクリプトファイルに書き出す。
 *==========================================================*/
void ps_write_plane
  _P2 (( FILE *, fp ),
       ( t_environment *, environment ))
{
  int i;
  long wxsize, wysize;
  long nplane;
  t_plane *plane_list;
  char gray_list[] = { 'a', 'b', 'c', 'd', 'e', 'f' };

  double x, y, z;
  double scale, range, wide;
  double D;

  scale = environment->scale;
  XcilGetWindowSize( environment->display, environment->window,
		     &wxsize, &wysize );

  nplane = environment->nplane;
  plane_list = environment->plane_list;

  range = Voxel.xsize( environment->object ) * environment->range;
  for ( i = 0; i < nplane; i++ )
    {
      t_plane *a_plane = &plane_list[i];
      double x0, y0, x1, y1, x2, y2, x3, y3;

      z = a_plane->at[0].z;
      wide = range / ( z + range );
      x = a_plane->at[0].x * wide;
      y = a_plane->at[0].y * wide;
      x0 = scale * x + wxsize/2;
      y0 = scale * y + wysize/2;

      z = a_plane->at[1].z;
      wide = range / ( z + range );
      x = a_plane->at[1].x * wide;
      y = a_plane->at[1].y * wide;
      x1 = scale * x + wxsize/2 - x0;
      y1 = scale * y + wysize/2 - y0;

      z = a_plane->at[2].z;
      wide = range / ( z + range );
      x = a_plane->at[2].x * wide;
      y = a_plane->at[2].y * wide;
      x2 = scale * x + wxsize/2 - x0;
      y2 = scale * y + wysize/2 - y0;

      z = a_plane->at[3].z;
      wide = range / ( z + range );
      x = a_plane->at[3].x * wide;
      y = a_plane->at[3].y * wide;
      x3 = scale * x + wxsize/2 - x0;
      y3 = scale * y + wysize/2 - y0;

      if (environment->surface_flag)
	fp_printf( fp, "%.3f ", environment->strength_list[a_plane->no] );

      fp_printf( fp, "%.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f g\n",
		x3, y3, x2, y2, x1, y1, x0, y0);
    }
}
