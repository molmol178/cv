/*
 * Usage : rgbtoy [input] [output]
 *
 * [RGB to YIQ]:
 *      Y = 0.299*R + 0.587*G + 0.114*B;
 *      I = 0.596*R - 0.274*G - 0.322*B;
 *      Q = 0.211*R - 0.523*G + 0.312*B;
 */

#include "Image.h"
#include "Image/Funcs.h"
#include "misc/optlib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void image__rgbtoy(image dest, image src);

char *option[] = {
  "The translater RGB to Y of YIQ.",
  "[ Y = 0.299 * red + 0.587 * green + 0.114 * blue ].",
  "input:*:1:(image) name of the input rgb color image",
  "output:*:1:(image) name of the output gray-scale image",
  "help:-h:print help messages",
};

int main(int argc, char **argv)
{
  image source, yofyiq;
  char *input;
  char *output;
  int i;

  OPTION_SETUP( option );

  if ( argc == 1 ) optusage( 1 );
  if ( optspecified( "help" ) ) optmanual( 1 );

  input  = optvalue( "input"  );
  output = optvalue( "output" );

  source = Image.createFromFilename( "SOURCE", input );
  yofyiq = Image.create( "YOFYIQ" );
  if ( source == 0 )
    {
      fprintf( stderr, "rgbtoy:can't open file %s\n", input );
      exit( -1 );
    }
  if ( __TYPE( source ) != UChar3 )
    {
      fprintf( stderr, "rgbtoy:image is wrong %s\n", input );
      exit( -1 );
    }

  image__rgbtoy( yofyiq, source );
  Image.save( yofyiq, output, "rgbtoy" );

  Image.destroy( source );
  Image.destroy( yofyiq );

  return 0;
}



void image__rgbtoy(image dest, image source)
{
  register int x, y, xsize, ysize;
  register uchar3 **s_data;
  register uchar  **d_data;

  xsize = __XSIZE(source);
  ysize = __YSIZE(source);

  Image.make(dest, UChar, xsize, ysize);

  s_data = __DATA(source, uchar3);
  d_data = __DATA(dest, uchar);
  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      {
	d_data[y][x] = (0.299 * s_data[y][x].at[0] +
			0.587 * s_data[y][x].at[1] +
			0.114 * s_data[y][x].at[2]);
      }
}
