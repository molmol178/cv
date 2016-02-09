/*
 *
 */

#include "Image.h"
#include "Voxel.h"
#include "misc/typelib.h"
#include "misc/optlib.h"
#include <stdio.h>



/*============================================*
 * rgb_to_voxel : RGB空間をボクセル型にする。
 *============================================*/
void rgb_to_voxel
  P5 (( voxel, output ),
      ( image, input  ),
      ( long , xsize  ),
      ( long , ysize  ),
      ( long , zsize  ))



static char *option[] = {
  "input:*:1:(image) name of input rgb-image",
  "output:-o:1:output.voxel:(voxel) name of output voxel",
  "size:-s:3:64:64:64:(int) size of output voxel",
  "help:-h:print this messages",
};



void main(argc,argv)
     int argc;
     char *argv[];
{
  image input;
  voxel output;
  char *name_of_output;
  char *name_of_input;
  long xsize, ysize, zsize;
  char comment[512];

  DoCompressVoxelSave = 0;

  OPTION_SETUP( option );

  if ( optspecified( "help" ) ) optmanual( 1 );
  if ( argc == 1 ) optusage( 1 );

  name_of_input = optvalue( "input" );
  name_of_output = optvalue( "output" );

  input = Image.createFromFilename( "input", name_of_input );
  if ( input == 0 )
    {
      fprintf( stderr, "can't open file %s\n", name_of_input );
      exit(-1);
    }
  output = Voxel.create( "output" );

  xsize = optvalueint( "size", 0 );
  ysize = optvalueint( "size", 1 );
  zsize = optvalueint( "size", 2 );

  rgb_to_voxel( output, input, xsize, ysize, zsize );

  sprintf( comment, "rgb2voxel %s", name_of_input );
  Voxel.save( output, name_of_output, comment );

  Image.destroy( input );
  Voxel.destroy( output );
}



/*============================================*
 * rgb_to_voxel : RGB空間をボクセル型にする。
 *============================================*/
void rgb_to_voxel
  _P5 (( voxel, output ),
       ( image, input  ),
       ( long , xsize  ),
       ( long , ysize  ),
       ( long , zsize  ))
{
  long x, y, z, xx, yy, zz;
  uchar3 ***data;

  Voxel.make( output, UChar3, xsize, ysize, zsize );
  data = (uchar3 ***)Voxel.data( output );

  for ( y = 0; y < __YSIZE(input); y++ )
    for ( x = 0; x < __XSIZE(input); x++ )
      {
	xx = (xsize-1) * __PIXEL0( input, x, y, uchar3 ) / 255;
	yy = (ysize-1) * __PIXEL1( input, x, y, uchar3 ) / 255;
	zz = (zsize-1) * __PIXEL2( input, x, y, uchar3 ) / 255;
	data[zz][yy][xx] = __PIXEL( input, x, y, uchar3 );
      }
  for ( x = 0; x < xsize; x++ ) data[0][0][x].at[0] = x / 2 + 127;
  for ( y = 0; y < ysize; y++ ) data[0][y][0].at[1] = y / 2 + 127;
  for ( z = 0; z < zsize; z++ ) data[z][0][0].at[2] = z / 2 + 127;
}
