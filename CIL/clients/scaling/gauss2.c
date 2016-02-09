/*
 * filename : scale.c
 * author   : Takahiro Sugiyama
 * date     : Thursday, March 24 1994
 * describe : image scale
 */

/*
 * 領域分割をして、連続領域と分離領域にわけて考える
 */

/*
 * 可視不可視を調べる。flush
 * 対象から一歩下がって見てみるといいらしい。
 * 見えるところをある程度の範囲で調べてみる。
 * 例えば回り４画素から見えるところ調べる。
 */


#include "Image.h"
#include "Image/Funcs.h"
#include "misc/optlib.h"
#include "misc/typelib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



/*=========================================================*
 * image__gaussian_resize : ガウス分布を仮定しリサイズする
 *=========================================================*/
void image__gaussian_resize
  P4 (( image, output ),
      ( image, input  ),
      ( long , xsize  ),
      ( long , ysize  ))



static char *option[] = {
  "input:*:1:(image) name of the input image",
  "output:-o:1:out.scale:(image) name of the output image",
  "scale:-s:1:1.0:(float) value of the image size's scale",
  "x-size:-x:1:(size) width of the image",
  "y-size:-y:1:(size) height of the image",
  "gaussian:-g:resize with the Gaussian",
  "sigma:-sg:1:(float) value of the sigma of Gaussian",
  "help:-h:print this messages",
};



double SPC_SIGMA = 0.0;



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
  optifspecifiedcopyfloat( "sigma", &SPC_SIGMA );

  if ( optspecified( "gaussian" ) )
    image__gaussian_resize( output, input, xsize, ysize );
  else
    Image.resize( output, input, xsize, ysize );

  sprintf( comment, "Scale %.2f of %s image", scale, input );
  Image.save( output, output_name, comment );

  return 0;
}



static void gaussian_resize_bit1
  P4 (( image, output    ),
      ( image, input     ),
      ( long , new_xsize ),
      ( long , new_ysize ))

static void gaussian_resize_uchar
  P4 (( image, output    ),
      ( image, input     ),
      ( long , new_xsize ),
      ( long , new_ysize ))

static void gaussian_resize_uchar3
  P4 (( image, output    ),
      ( image, input     ),
      ( long , new_xsize ),
      ( long , new_ysize ))



double SIGMA_FORM = 2.0;

/*=========================================================*
 * image__gaussian_resize : ガウス分布を仮定しリサイズする
 *=========================================================*/
void image__gaussian_resize
  _P4 (( image, output ),
       ( image, input  ),
       ( long , xsize  ),
       ( long , ysize  ))
{
  switch( __TYPE( input ) )
    {
    case Bit1:
      gaussian_resize_bit1( output, input, xsize, ysize );
      break;
    case UChar:
      gaussian_resize_uchar( output, input, xsize, ysize );
      break;
    case UChar3:
      gaussian_resize_uchar3( output, input, xsize, ysize );
      break;
    default:
      fprintf( stderr, "not supported %s type\n",  typename(__TYPE(input)) );
      exit( -1 );
    }
}



static float **make_bigger_exp_filter
  _P5 (( long    , size         ),
       ( long    , small_size   ),
       ( float **  , opt_data_ptr ),
       ( int **  , spt_data_ptr ),
       ( int **  , ept_data_ptr ))
{
  int n, i, j, spt, ept;
  float **exp_data;
  float *opt_data;
  int *spt_data, *ept_data;
  float *old_data;

  double sigma, sigma2;

  sigma = SPC_SIGMA;
  if ( sigma == 0 )
    sigma = ( (double)size / small_size ) / SIGMA_FORM;
  sigma2 = sigma * sigma;
  n = sigma * 4.2;
  if ( n <= 0 ) n = 1;

  exp_data = typenew2( small_size, size, float );
  opt_data = typenew1( size, float );
  spt_data = typenew1( size, int );
  ept_data = typenew1( size, int );
  for ( i = 0; i < size; i++ )
    {
      double org = ( double )small_size * i / size;
      spt = org - n;
      ept = org + n;
      if ( spt < 0 ) spt = 0;
      if ( ept >= small_size ) ept = small_size - 1;

      opt_data[ i ] = org;
      spt_data[ i ] = spt;
      ept_data[ i ] = ept;

      for ( j = spt; j <= ept; j++ )
	exp_data[ i ][ j ] = exp( - ( org - j ) * ( org - j ) / sigma2 );
    }

  *opt_data_ptr = opt_data;
  *spt_data_ptr = spt_data;
  *ept_data_ptr = ept_data;

  return exp_data;
}



static float **make_smaller_exp_filter
  _P5 (( long    , size         ),
       ( long    , small_size   ),
       ( float **, opt_data_ptr ),
       ( int **  , spt_data_ptr ),
       ( int **  , ept_data_ptr ))
{
  int n, i, j, spt, ept;
  float **exp_data;
  float *opt_data;
  int *spt_data, *ept_data;
  float *old_data;

  double sigma, sigma2;

  sigma = SPC_SIGMA;
  if ( sigma == 0 )
    sigma = ( (double)size / small_size ) / SIGMA_FORM;
  sigma2 = sigma * sigma;
  n = sigma * 4.2;
  if ( n <= 0 ) n = 1;

  exp_data = typenew2( size, small_size, float );
  opt_data = typenew1( small_size, float );
  spt_data = typenew1( small_size, int );
  ept_data = typenew1( small_size, int );
  for ( i = 0; i < small_size; i++ )
    {
      double o = ( double )size * ( i + 0.5 ) / small_size;

      spt = o - n;
      ept = o + n;
      if ( spt < 0 ) spt = 0;
      if ( ept >= size ) ept = size - 1;

      opt_data[ i ] = spt;
      spt_data[ i ] = spt;
      ept_data[ i ] = ept;

      for ( j = spt; j <= ept; j++ )
	exp_data[ i ][ j ] = exp( - ( o - j ) * ( o - j ) / sigma2 );
    }

  *opt_data_ptr = opt_data;
  *spt_data_ptr = spt_data;
  *ept_data_ptr = ept_data;

  return exp_data;
}



static float **make_exp_filter
  _P5 (( long      , old_size     ),
       ( long      , new_size     ),
       ( float **  , opt_data_ptr ),
       ( int **    , spt_data_ptr ),
       ( int **    , ept_data_ptr ))
{
  float **filter;

  if ( old_size <= new_size )
    filter = make_bigger_exp_filter( new_size, old_size,
				     opt_data_ptr, spt_data_ptr, ept_data_ptr );
  else
    filter = make_smaller_exp_filter( old_size, new_size,
				     opt_data_ptr, spt_data_ptr, ept_data_ptr );
  return filter;
}



static void free_exp_filter
  _P4 (( float **, exp_data ),
       ( float * , opt_data ),
       ( int *   , spt_data ),
       ( int *   , ept_data ))
{
  typefree2( exp_data );
  typefree1( opt_data );
  typefree1( spt_data );
  typefree1( ept_data );
}



static void gaussian_resize_bit1
  _P4 (( image, output    ),
       ( image, input     ),
       ( long , new_xsize ),
       ( long , new_ysize ))
{
  int x, y, old_xsize, old_ysize;
  register bit1 **new_data;
  register bit1 **old_data;

  register float **x_exp, **y_exp;
  float *ox_data, *oy_data;
  int *sx_data, *sy_data;
  int *ex_data, *ey_data;

  image label, local;

  label = Image.create( "label" );
  local = Image.create( "local" );

  SIGMA_FORM = 2.0;

  Image.make( output, __TYPE( input ), new_xsize, new_ysize );
  old_xsize = __XSIZE( input );
  old_ysize = __YSIZE( input );

  old_data = __DATA( input , bit1 );
  new_data = __DATA( output, bit1 );

  x_exp = make_exp_filter( old_xsize, new_xsize, &ox_data,&sx_data,&ex_data );
  y_exp = make_exp_filter( old_ysize, new_ysize, &oy_data,&sy_data,&ey_data );

  for ( y = 0; y < new_ysize; y++ )
    {
      double oy;
      int sy, ey;
      int px = -1, py = -1;

      oy = oy_data[ y ];
      sy = sy_data[ y ];
      ey = ey_data[ y ];

      for ( x = 0; x < new_xsize; x++ )
	{
	  double ox;
	  int n, sx, ex;

	  ox = ox_data[ x ];
	  sx = sx_data[ x ];
	  ex = ex_data[ x ];

	  Image.sub( local, input, sx, sy, ex - sx + 1, ey - sy + 1 );
	  n = image__labeling( label, local );
	  if ( n >= 3 )
	    {
	      register int ix, iy, l0, l1, l2, l3, l4, x0, y0;

	      x0 = (int)(0.5+ox) - sx;
	      y0 = (int)(0.5+oy) - sy;

	      l0 = l1 = l2 = l3 = l4 = __PIXEL( label, x0, y0, long );
	      if (! (( x0 == (ox-sx) ) && ( y0 == (oy-sy) )))
		  {
		    if ( (x0+1) < __XSIZE(label) )
		      l1 = __PIXEL( label, x0 + 1, y0, long );
		    if ( (x0-1) >= 0 )
		      l2 = __PIXEL( label, x0 - 1, y0, long );
		    if ( (y0+1) < __YSIZE(label) )
		      l3 = __PIXEL( label, x0, y0 + 1, long );
		    if ( (y0-1) >= 0 )
		      l4 = __PIXEL( label, x0, y0 - 1, long );
		  }
	      for ( iy = 0; iy < __YSIZE(label); iy++ )
		for ( ix = 0; ix < __XSIZE(label); ix++ )
		  if (( __PIXEL( label, ix, iy, long ) != l0 ) &&
		      ( __PIXEL( label, ix, iy, long ) != l1 ) &&
		      ( __PIXEL( label, ix, iy, long ) != l2 ) &&
		      ( __PIXEL( label, ix, iy, long ) != l3 ) &&
		      ( __PIXEL( label, ix, iy, long ) != l4 ))
		    __PIXEL( local, ix, iy, bit1 ) = 1;
	    }

	  {
	    register int ix, iy;
	    double value, sum, weight;
	    value = 0;
	    sum = 0;
	    for ( iy = sy; iy <= ey; iy++ )
	      for ( ix = sx; ix <= ex; ix++ )
		{
		  weight = y_exp[ y ][ iy ] * x_exp[ x ][ ix ];
		  value += weight * __PIXEL( local, ix - sx, iy - sy, bit1 );
		  sum += weight;
		}
	    new_data[ y ][ x ] = (( value / sum ) >= 0.5 ) ? 1 : 0;
	  }
	}
    }

  free_exp_filter( x_exp, ox_data, sx_data, ex_data );
  free_exp_filter( y_exp, oy_data, sy_data, ey_data );
}



static void gaussian_resize_uchar
  _P4 (( image, output    ),
       ( image, input     ),
       ( long , new_xsize ),
       ( long , new_ysize ))
{
  int x, y, old_xsize, old_ysize;
  register uchar **new_data;
  register uchar **old_data;

  register float **x_exp, **y_exp;
  int *sx_data, *sy_data, *ex_data, *ey_data;
  double x_radius_value, y_radius_value;

  SIGMA_FORM = 4.0;

  Image.make( output, __TYPE( input ), new_xsize, new_ysize );
  old_xsize = __XSIZE( input );
  old_ysize = __YSIZE( input );

  old_data = __DATA( input , uchar );
  new_data = __DATA( output, uchar );

  x_exp = make_exp_filter( old_xsize, new_xsize, &sx_data, &ex_data );
  y_exp = make_exp_filter( old_ysize, new_ysize, &sy_data, &ey_data );

  for ( y = 0; y < new_ysize; y++ )
    {
      int sy, ey;

      sy = sy_data[ y ];
      ey = ey_data[ y ];

      for ( x = 0; x < new_xsize; x++ )
	{
	  int sx, ex;

	  sx = sx_data[ x ];
	  ex = ex_data[ x ];

	  {
	    register int ix, iy;
	    double value;
	    double sum, weight;

	    value = 0;
	    sum = 0;
	    for ( iy = sy; iy <= ey; iy++ )
	      for ( ix = sx; ix <= ex; ix++ )
		{
		  weight = y_exp[ y ][ iy ] * x_exp[ x ][ ix ];
		  value += weight * old_data[ iy ][ ix ];
		  sum += weight;
		}
	    new_data[ y ][ x ] = value / sum;
	  }
	}
    }

  free_exp_filter( x_exp, sx_data, ex_data );
  free_exp_filter( y_exp, sy_data, ey_data );
}



static void gaussian_resize_uchar3
  _P4 (( image, output    ),
       ( image, input     ),
       ( long , new_xsize ),
       ( long , new_ysize ))
{
  int x, y, old_xsize, old_ysize;
  register uchar3 **new_data;
  register uchar3 **old_data;

  register float **x_exp, **y_exp;
  int *sx_data, *sy_data, *ex_data, *ey_data;

  SIGMA_FORM = 4.0;

  Image.make( output, __TYPE( input ), new_xsize, new_ysize );
  old_xsize = __XSIZE( input );
  old_ysize = __YSIZE( input );

  old_data = __DATA( input , uchar3 );
  new_data = __DATA( output, uchar3 );

  x_exp = make_exp_filter( old_xsize, new_xsize, &sx_data, &ex_data );
  y_exp = make_exp_filter( old_ysize, new_ysize, &sy_data, &ey_data );

  for ( y = 0; y < new_ysize; y++ )
    {
      int sy, ey;

      sy = sy_data[ y ];
      ey = ey_data[ y ];

      for ( x = 0; x < new_xsize; x++ )
	{
	  int sx, ex;

	  sx = sx_data[ x ];
	  ex = ex_data[ x ];

	  {
	    register int ix, iy;
	    double value[3], sum, weight;
	    value[0] = value[1] = value[2] = 0;
	    sum = 0;
	    for ( iy = sy; iy <= ey; iy++ )
	      for ( ix = sx; ix <= ex; ix++ )
		{
		  weight = y_exp[ y ][ iy ] * x_exp[ x ][ ix ];
		  sum += weight;
		  value[ 0 ] += weight * old_data[ iy ][ ix ].at[ 0 ];
		  value[ 1 ] += weight * old_data[ iy ][ ix ].at[ 1 ];
		  value[ 2 ] += weight * old_data[ iy ][ ix ].at[ 2 ];
		}
	    new_data[ y ][ x ].at[ 0 ] = value[ 0 ] / sum;
	    new_data[ y ][ x ].at[ 1 ] = value[ 1 ] / sum;
	    new_data[ y ][ x ].at[ 2 ] = value[ 2 ] / sum;
	  }
	}
    }

  free_exp_filter( x_exp, sx_data, ex_data );
  free_exp_filter( y_exp, sy_data, ey_data );
}
