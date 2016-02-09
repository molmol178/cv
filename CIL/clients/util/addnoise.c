/*
 * 正規分布のノイズを加える
 *
 *	filename : addnoise.c
 *	purpose	 : Adds a Gaussian random noise.
 *	author	 : Takahiro Sugiyama
 *	date	 : 1994/02/18(Thu)
 */



#include "Image.h"
#include "ImageFile.h"
#include "misc/optlib.h"
#include <math.h>
#include <stdio.h>



#define RND_MAX 0x7FFFFFFF
#define uniform_random() ((double)rand() / (double)RND_MAX)

#include <sys/time.h>

/*--------------------------------------------------*
 * set_seed : 乱数のシードを設定する
 *--------------------------------------------------*/
static void set_seed()
{
  double seed;

  seed = time(0);
  if ( optspecified( "seed" ) )
    seed = optvaluefloat( "seed" );

  srand( seed );
}



/*--------------------------------------------------*
 * gauss_random : ガウス分布の乱数を返す
 *--------------------------------------------------*/
static double gauss_random( mean, variance )
     double mean, variance;
{
  double u1, u2, uno, temp, temp1;
  static double temp2;
  static int toggle = 1;

  uno = 2.0 * exp( 0.825 );

  if ( toggle )
    {
      u1 = uniform_random();
      u2 = uniform_random();
      temp = sqrt( ( double )( - uno * variance * log10( ( double )u1 ) ) );
      temp1 = temp * cos( 2.0 * M_PI * u2 ) + mean;
      temp2 = temp * sin( 2.0 * M_PI * u2 ) + mean;
      toggle = 0;
      return temp1;
    }
  toggle = 1;

  return temp2;
}



/*--------------------------------------------------*
 * add_noise : ガウス分布に従うノイズを加える
 *--------------------------------------------------*/
int add_noise( normal, mean, variance )
     image normal;
     double mean;
     double variance;
{
  register int i, n;
  long value, value0, value1, value2;

  n = __AREA( normal );
  switch ( __TYPE( normal ) )
    {
    case UChar:
      {
	register uchar *p;
	p = __RASTER( normal, uchar );
	for ( i = 0; i < n; i++, p++ )
	  {
	    value = (long)*p + gauss_random( mean, variance );
	    if ( value < 0 ) value = 0; else if ( value > 255 ) value = 255;
	    *p = value;
	  }
      }
      break;

    case UChar3:
      {
	register uchar3 *p;
	p = __RASTER( normal, uchar3 );
	for ( i = 0; i < n; i++, p++ )
	  {
	    value0 = (long)p->at[ 0 ] + gauss_random( mean, variance );
	    value1 = (long)p->at[ 1 ] + gauss_random( mean, variance );
	    value2 = (long)p->at[ 2 ] + gauss_random( mean, variance );
	    if ( value0 < 0 ) value0 = 0; else if ( value0 > 255 ) value0 = 255;
	    if ( value1 < 0 ) value1 = 0; else if ( value1 > 255 ) value1 = 255;
	    if ( value2 < 0 ) value2 = 0; else if ( value2 > 255 ) value2 = 255;
	    p->at[ 0 ] = value0;
	    p->at[ 1 ] = value1;
	    p->at[ 2 ] = value2;
	  }
      }
      break;
    }
}



/*--------------------------------------------------*
 * get_variance : オプションから分散を求める
 *--------------------------------------------------*/
static double get_variance()
{
  double SNR;
  double sigma;
  double h;
  double variance;

  if ( optspecified( "variance" ) )
    {
      variance = optvaluefloat( "variance" );
    }
  else
  if ( optspecified( "sigma" ) )
    {
      sigma = optvaluefloat( "sigma" );
      variance = sigma * sigma;
    }
  else
    {
      SNR    = optvaluefloat( "SNR" );
      h      = optvaluefloat( "contrast" );
      variance = h * h / SNR;
    }

  return variance;
}



char *option[] = {
  "The addition of a Gaussian random noise.",
  "input:*:1:(image) name of the input gray or color image",
  "output:*:1:(image) name of the output image",
  "help:-h:print this message",
  "mean:-m:1:0.0:(float) mean of the noise",
  "variance:-v:1:(float) variance of the noise",
  "sigma:-s:1:(float) sigma of the noise",
  "SNR:-sn:1:(float) number of SNR",
  "contrast:-c:1:(int) contrast of the edge",
  "seed:-sd:1:(float) seed of the random",
  "format:-f:1:PNM:(format) file format of the output image",
};



int main( argc, argv )
     int argc;
     char *argv[];
{
  image normal;
  char *input  = "-";
  char *output = "-";
  double mean, variance;

  OPTION_SETUP( option );

  if ( argc == 1 ) optusage( 1 );
  if ( optspecified( "help" ) ) optmanual( 1 );

  input  = optvalue( "input" );
  output = optvalue( "output" );
  mean = optvaluefloat( "mean" );
  variance = get_variance();
  set_seed();

  normal = Image.createFromFilename( "NORMAL", input );
  if ( normal == 0 )
    {
      fprintf( stderr, "can't open file (%s)\n", input );
      exit( -1 );
    }
  printf( "BEGIN { addition of a Gaussian random noise }\n");
  printf( "    input image  = \"%s\"\n", input  );
  printf( "    output image = \"%s\"\n", output );
  printf( "    mean         = %.5f\n",   mean );
  printf( "    variance     = %.5f\n",   variance );
  fflush( stdout );
  add_noise( normal, mean, variance );
  ImageFile.setSaveFormat( optvalue( "format" ) );
  Image.save( normal, output, "NOISE IMAGE" );
  printf( "END { addition of a Gaussian random noise }\n");
  fflush( stdout );

  Image.destroy( normal );
}
