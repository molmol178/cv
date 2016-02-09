/*
 * filename : initialize.c
 * author   : Takahiro Sugiyama
 * date     : Tuesday, January 10 1995
 * describe : voxeldisp initialization
 */



#include "voxeldisp.h"



static void shift_object(t_environment *environment)
{
  voxel tmp_object;
  register int x, y, z;
  int xsize, ysize, zsize, n;
  register bit1 ***data, ***tmp_data;
  voxel the_object;
  double sum_x, sum_y, sum_z;

  fprintf(stderr, "shift object\n");

  the_object = environment->object;
  data  = (bit1 ***)Voxel.data( the_object );
  xsize = Voxel.xsize( the_object );
  ysize = Voxel.ysize( the_object );
  zsize = Voxel.zsize( the_object );

  tmp_object = Voxel.create("tmp");
  Voxel.copy(tmp_object, the_object);
  tmp_data  = (bit1 ***)Voxel.data( tmp_object );

  n = 0;
  sum_x = sum_y = sum_z = 0;
  for ( z = 0; z < zsize; z++ )
    for ( y = 0; y < ysize; y++ )
      for ( x = 0; x < xsize; x++ )
	if (data[z][y][x])
	  {
	    sum_x += x;
	    sum_y += y;
	    sum_z += z;
	    n++;
	  }
  sum_x /= n;
  sum_y /= n;
  sum_z /= n;

  fprintf(stderr, "center = (%.1f, %.1f, %.1f)\n", sum_x, sum_y, sum_z);

  for ( z = 0; z < zsize; z++ )
    for ( y = 0; y < ysize; y++ )
      for ( x = 0; x < xsize; x++ )
	{
	  int xx = x + sum_x - xsize/2;
	  int yy = y + sum_y - ysize/2;
	  int zz = z + sum_z - zsize/2;
	  data[z][y][x] = 0;
	  if (xx < 0 || xx >= xsize) continue;
	  if (yy < 0 || yy >= ysize) continue;
	  if (zz < 0 || zz >= zsize) continue;
	  data[z][y][x] = tmp_data[zz][yy][xx];
	}
  Voxel.destroy(tmp_object);
}


/*=======================================*
 * initialize_environment : 環境の初期化
 *=======================================*/
void initialize_environment
  _P1 (( t_environment *, environment ))
{
  strcopy( environment->name_of_input, optvalue( "input" ) );
  environment->object = Voxel.createFromFilename( "Input", environment->name_of_input );

  environment->angle.x = optnvaluefloat( "angles", 0 );
  environment->angle.y = optnvaluefloat( "angles", 1 );
  environment->angle.z = optnvaluefloat( "angles", 2 );
  environment->scale = optvaluefloat( "scale" );
  environment->scale_index = INITIAL_SCALE_INDEX;
  environment->range = optvaluefloat( "range" );
  environment->light.x = optnvaluefloat( "light", 0 );
  environment->light.y = optnvaluefloat( "light", 1 );
  environment->light.z = optnvaluefloat( "light", 2 );

  environment->ncell = 0;
  environment->cell_list = 0;
  environment->nplane = 0;
  environment->plane_list = 0;
  environment->wireframe_flag = 1;
  environment->surface_flag = 0;
  environment->light_flag = 1;
  environment->center_flag = optspecified("center");

  if (environment->center_flag)
    shift_object(environment);

  if (optspecified("nolight"))
    environment->light_flag = 0;
}
