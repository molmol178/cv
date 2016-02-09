/*
 * filename : initialize.c
 * author   : Takahiro Sugiyama
 * date     : Tuesday, January 10 1995
 * describe : voxeldisp initialization
 */



#include "voxeldisp.h"



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
}
