/*
 * filename : dispbox.c
 * author   : Takahiro Sugiyama
 * date     : Friday, January 6 1995
 * describe : display 3d box
 */


#include "voxeldisp.h"


static char *option[] = {
  "input:*:1:(voxel) name of input voxel image",
  "angles:-a:3:0:0:0:(float) angles of rotation[degree]",
  "light:-l:3:-0.2:-1:1.5:(float) vector of the light",
  "nolight:-n:no use the light effection",
  "scale:-s:1:5:(float) scale of a voxel size",
  "range:-r:1:10:(float) range between object and view point",
  "center:-c:origin by center of object",
  "help:-h:print this messages",
};


int main( argc, argv )
     int argc;
     char *argv[];
{
  t_environment theEnvironment;

  OPTION_SETUP( option );

  if ( optspecified( "help" ) ) optmanual( 1 );
  if ( argc == 1 ) optusage( 1 );

  initialize_environment( &theEnvironment );
  initialize_xevent( &theEnvironment );

  all_draw_voxel_object( &theEnvironment );

  XcilEventLoop( theEnvironment.display, theEnvironment.context );

  return 0;
}
