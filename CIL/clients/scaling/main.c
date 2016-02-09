/*
 * filename : scale.c
 * author   : Takahiro Sugiyama
 * date     : Thursday, March 24 1994
 * describe : image scale
 */



#include "Image.h"
#include "Image/Funcs.h"
#include "misc/optlib.h"
#include "misc/typelib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



static void image__triangle_resize_bit1
  P4 (( image, output    ),
      ( image, input     ),
      ( long , new_xsize ),
      ( long , new_ysize ))



static char *option[] = {
  "<:Image size scaling (28 May 94):",
  "help:-h:print this messages",
  "input:*:1:(image) name of the input image",
  "output:-o:1:out.scale:(image) name of the output image",
  "scale:-s:1:1.0:(float) value of the image size's scale",
  "x-size:-x:1:(size) width of the image",
  "y-size:-y:1:(size) height of the image",
  "o)Scaling methods:",
  "gaussian:-g:use the method with Gaussian",
  "liner:-l:use the liner complement method",
  "weight:-w:use the weight average complement method",
  "arithmetic:-a:use the arithmetic method",
  "o)The parameter of the method with Gaussian:",
  "sigma:-sg:1:(float) value of the sigma of Gaussian",
};



int main( argc, argv )
     int argc;
     char *argv[];
{
  image input, output;
  char comment[ 512 ];
  long xsize, ysize;

  char *input_name;
  char *output_name;
  double scale;
  double sigma = 0.0;

  OPTION_SETUP( option );

  if ( optspecified( "help" ) ) optmanual( 1 );
  if ( argc == 1 ) optusage( 1 );

  input_name  = optvalue( "input" );
  output_name = optvalue( "output" );
  scale       = optvaluefloat( "scale" );

  input  = Image.createFromFilename( "INPUT", input_name );
  output = Image.create( "OUTPUT" );

  if ( input == 0 )
    {
      fprintf( stderr, "can't open file (%s)\n", input_name );
      exit( -1 );
    }

  xsize = __XSIZE( input ) * scale;
  ysize = __YSIZE( input ) * scale;

  optifspecifiedcopyint( "x-size", &xsize );
  optifspecifiedcopyint( "y-size", &ysize );
  optifspecifiedcopyfloat( "sigma", &sigma );

  if ( optspecified( "gaussian" ) )
    {
      fprintf( stderr, "Use Gaussian\n" );
      image__scaling_gaussian( output, input, xsize, ysize, sigma );
    }
  else
  if ( optspecified( "liner" ) )
    {
      fprintf( stderr, "Use Liner Complement\n" );
      image__scaling_liner_complement( output, input, xsize, ysize );
    }
  else
  if ( optspecified( "weight" ) )
    {
      fprintf( stderr, "Use Weight Average Complement\n" );
      image__scaling_weight_average( output, input, xsize, ysize );
    }
  else
  if ( optspecified( "arithmetic" ) )
    {
      fprintf( stderr, "Use Arithmetic\n" );
      image__scaling_arithmetic( output, input, xsize, ysize );
    }
  else
    Image.resize( output, input, xsize, ysize );

  sprintf( comment, "Scale %.2f of %s image", scale, input_name );
  Image.save( output, output_name, comment );

  return 0;
}
