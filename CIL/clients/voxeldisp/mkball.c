/*
 *
 */

#include "Image.h"
#include "Voxel.h"
#include "misc/typelib.h"
#include "misc/optlib.h"
#include <math.h>
#include <stdio.h>



/*==========================*
 * make_ball : ボールを作る
 *==========================*/
void make_ball
  P5 (( voxel, output ),
      ( long , ox ),
      ( long , oy ),
      ( long , oz ),
      ( long , r  ))



static char *option[] = {
  "input:*:1:(image) name of input rgb-image",
  "output:-o:1:output.voxel:(voxel) name of output voxel",
  "size:-s:3:64:64:64:(int) size of output voxel",
  "radius:-r:1:20:(int) radius of ball",
  "center:-c:3:32:32:32:(int) center of ball",
  "help:-h:print this messages",
};



void main(argc,argv)
     int argc;
     char *argv[];
{
  voxel input;
  char *name_of_output;
  char *name_of_input = 0;
  long xsize, ysize, zsize;
  long ox, oy, oz;
  long radius;
  char comment[512];

/*  DoCompressVoxelSave = 0;*/

  OPTION_SETUP( option );

  if ( optspecified( "help" ) ) optmanual( 1 );
  if ( argc == 1 ) optusage( 1 );

  if ( optspecified( "input" ) ) name_of_input = optvalue( "input" );
  name_of_output = optvalue( "output" );
  xsize = optvalueint( "size", 0 );
  ysize = optvalueint( "size", 1 );
  zsize = optvalueint( "size", 2 );
  ox = optvalueint( "center", 0 );
  oy = optvalueint( "center", 1 );
  oz = optvalueint( "center", 2 );
  radius = optvalueint( "radius", 2 );

  if ( name_of_input )
    input = Voxel.createFromFilename( "input", name_of_input );
  if ( input == 0 )
    input = Voxel.createMake( "input", Bit1, xsize, ysize, zsize );

  make_ball( input, ox, oy, oz, radius );

  sprintf( comment, "mkball %s", name_of_input );
  Voxel.save( input, name_of_output, comment );

  Voxel.destroy( input );
}



/*==========================*
 * make_ball : ボールを作る
 *==========================*/
void make_ball
  _P5 (( voxel, output ),
       ( long , ox ),
       ( long , oy ),
       ( long , oz ),
       ( long , r  ))
{
  long x, y, z, xx, yy, zz, rr;
  bit1 ***data;
  long xsize, ysize, zsize;

  xsize = Voxel.xsize( output );
  ysize = Voxel.ysize( output );
  zsize = Voxel.zsize( output );
  data = (bit1 ***)Voxel.data( output );
  rr = r * r;

  for ( z = 0; z < zsize; z++ )
    {
      zz = (z - oz)*(z - oz);
      for ( y = 0; y < ysize; y++ )
	{
	  yy = (y - oy)*(y - oy);
	  for ( x = 0; x < xsize; x++ )
	    {
	      xx = (x - ox)*(x - ox);
	      if (( xx + yy + zz ) < rr )
		data[z][y][x] = 1;
	    }
	}
    }
}
