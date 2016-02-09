/*
 * Usage : rgbtohsv [input] [output]
 */

#include "Image.h"
#include "ColorImage.h"
#include "Image/Funcs.h"
#include "misc/optlib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void image__rgbtohsv(image hsv, image rgb);

char *option[] = {
  "The translater RGB to HSV.",
  "input:*:1:(image) name of the input rgb color image",
  "output:*:1:(image) name of the output hsv color image",
  "help:-h:print help messages",
};

int main(argc, argv)
     int    argc;
     char **argv;
{
  image rgb, hsv;
  char *input;
  char *output;
  int i;

  OPTION_SETUP( option );

  if ( argc == 1 ) optusage( 1 );
  if ( optspecified( "help" ) ) optmanual( 1 );

  input  = optvalue( "input"  );
  output = optvalue( "output" );

  rgb = Image.createFromFilename( "RGB", input );
  hsv = Image.create( "HSV" );
  if ( rgb == 0 )
    {
      fprintf( stderr, "rgbtohsv:can't open file %s\n", input );
      exit( -1 );
    }
  if ( __TYPE( rgb ) != UChar3 )
    {
      fprintf( stderr, "rgbtohsv:image is wrong %s\n", input );
      exit( -1 );
    }

  image__rgbtohsv( hsv, rgb );
  Image.save( hsv, output, "rgbtohsv" );

  Image.destroy( rgb );
  Image.destroy( hsv );

  return 0;
}

void image__rgbtohsv(image hsv, image rgb)
{
  register int x, y, xsize, ysize;
  register uchar3 **hsv_data;
  register uchar3 **rgb_data;
  float3 hsv_pixel;

  xsize = __XSIZE(rgb);
  ysize = __YSIZE(rgb);
  Image.make(hsv, UChar3, xsize, ysize);

  rgb_data = __DATA(rgb, uchar3);
  hsv_data = __DATA(hsv, uchar3);
  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      {
	/* RGB を HSV に変換する関数 各画素のポインタを渡す
	 * void UCharRGB_To_HSV_Abe(rgb_pixel_ptr, hsv_pixel_ptr);
	 * uchar3 *rgb_pixel_ptr;
	 * float3 *hsv_pixel_ptr;
	 */
	UCharRGB_To_HSV_Abe(&rgb_data[y][x], &hsv_pixel);
	hsv_data[y][x].at[0] = 255*hsv_pixel.at[0]/360;
	hsv_data[y][x].at[1] = hsv_pixel.at[1];
	hsv_data[y][x].at[2] = hsv_pixel.at[2];
      }
}
