/*
 * filename : gauss.c
 * author   : Takahiro Sugiyama
 * date     : Saturday, May 28 1994
 * describe : Gaussian Complement
 */



#include "Image.h"
#include "misc/memlib.h"
#include <stdio.h>
#include <math.h>



static double SIGMA_FORM = 2.0;
static double SPC_SIGMA = 0.0;



static float **make_bigger_exp_filter
  _P4 (( long    , size         ),
       ( long    , small_size   ),
       ( int **  , spt_data_ptr ),
       ( int **  , ept_data_ptr ))
{
  int n, i, j, spt, ept;
  float **exp_data;
  int *spt_data, *ept_data;
  float *old_data;

  double sigma, sigma2;

  sigma = SPC_SIGMA;
  if ( sigma == 0.0 )
    sigma = ( (double)size / small_size ) / SIGMA_FORM;
  sigma2 = sigma * sigma;
  n = sigma * 4.2;
  if ( n <= 0 ) n = 1;
/*
  fprintf( stderr, "size=%d,smasize=%d,SIF=%.5f,SIGMA=%.5f\n",
	   size, small_size, SIGMA_FORM, sigma );
*/
  exp_data = typenew2( small_size, size, float );
  spt_data = typenew1( size, int );
  ept_data = typenew1( size, int );
  for ( i = 0; i < size; i++ )
    {
      double org = ( double )small_size * i / size;
      spt = org - n;
      ept = org + n;
      if ( spt < 0 ) spt = 0;
      if ( ept >= small_size ) ept = small_size - 1;

      spt_data[ i ] = spt;
      ept_data[ i ] = ept;

      for ( j = spt; j <= ept; j++ )
	exp_data[ i ][ j ] = exp( - ( org - j ) * ( org - j ) / sigma2 );
    }

  *spt_data_ptr = spt_data;
  *ept_data_ptr = ept_data;

  return exp_data;
}



static float **make_smaller_exp_filter
  _P4 (( long    , size         ),
       ( long    , small_size   ),
       ( int **  , spt_data_ptr ),
       ( int **  , ept_data_ptr ))
{
  int n, i, j, spt, ept;
  float **exp_data;
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
  spt_data = typenew1( small_size, int );
  ept_data = typenew1( small_size, int );
  for ( i = 0; i < small_size; i++ )
    {
      double o = ( double )size * ( i + 0.5 ) / small_size;

      spt = o - n;
      ept = o + n;
      if ( spt < 0 ) spt = 0;
      if ( ept >= size ) ept = size - 1;

      spt_data[ i ] = spt;
      ept_data[ i ] = ept;

      for ( j = spt; j <= ept; j++ )
	exp_data[ i ][ j ] = exp( - ( o - j ) * ( o - j ) / sigma2 );
    }

  *spt_data_ptr = spt_data;
  *ept_data_ptr = ept_data;

  return exp_data;
}



static float **make_exp_filter
  _P4 (( long      , old_size     ),
       ( long      , new_size     ),
       ( int **    , spt_data_ptr ),
       ( int **    , ept_data_ptr ))
{
  float **filter;

  if ( old_size <= new_size )
    filter = make_bigger_exp_filter( new_size, old_size,
				     spt_data_ptr, ept_data_ptr );
  else
    filter = make_smaller_exp_filter( old_size, new_size,
				      spt_data_ptr, ept_data_ptr );
  return filter;
}



static void free_exp_filter
  _P3 (( float **, exp_data ),
       ( int *    , spt_data ),
       ( int *    , ept_data ))
{
  typefree2( exp_data );
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
  int *sx_data, *sy_data;
  int *ex_data, *ey_data;

  Image.make( output, __TYPE( input ), new_xsize, new_ysize );
  old_xsize = __XSIZE( input );
  old_ysize = __YSIZE( input );

  old_data = __DATA( input , bit1 );
  new_data = __DATA( output, bit1 );

  SIGMA_FORM = sqrt( 2.0 * log( 2.0 ) );
  x_exp = make_exp_filter( old_xsize, new_xsize, &sx_data, &ex_data );
  y_exp = make_exp_filter( old_ysize, new_ysize, &sy_data, &ey_data );

  for ( y = 0; y < new_ysize; y++ )
    {
      int sy, ey;
      int px, py, nx, ny;
      double rx, ry;

      sy = sy_data[ y ];
      ey = ey_data[ y ];

      ry = (double)y * old_ysize / new_ysize;
      py = ry;
      ny = py + 1;
      if (( ry == (double)py ) || ( ny >= old_ysize )) ny = py;

      for ( x = 0; x < new_xsize; x++ )
	{
	  int sx, ex;

	  sx = sx_data[ x ];
	  ex = ex_data[ x ];

	  rx = (double)x * old_xsize / new_xsize;
	  px = rx;
	  nx = px + 1;
	  if (( rx == (double)px ) || ( nx >= old_xsize )) nx = px;

	  if (( px == nx ) && ( py == ny ))
	    new_data[ y ][ x ] = old_data[ py ][ px ];
	  else
	  if ((( px == nx ) && old_data[py][px] && old_data[ny][px] ) ||
	      (( py == ny ) && old_data[py][px] && old_data[py][nx] ) ||
	      (((px-rx)==(py-ry)) &&  old_data[py][px] && old_data[ny][nx])||
	      (((nx-rx)==(ry-py)) &&  old_data[ny][px] && old_data[py][nx]))
	    new_data[ y ][ x ] = 1;
	  else
	  if (( old_data[py][px] == 0 ) && ( old_data[ny][px] == 0 ) &&
	      ( old_data[py][nx] == 0 ) && ( old_data[ny][nx] == 0 ))
	    new_data[ y ][ x ] = 0;
	  else
	  if (( old_data[py][px] == 1 ) && ( old_data[ny][px] == 1 ) &&
	      ( old_data[py][nx] == 1 ) && ( old_data[ny][nx] == 1 ))
	    new_data[ y ][ x ] = 1;
	  else

	  {
	      register int ix, iy;
	      double value, sum, weight;
	      value = 0;
	      sum = 0;
	      for ( iy = sy; iy <= ey; iy++ )
		for ( ix = sx; ix <= ex; ix++ )
		  {
		    weight = y_exp[ y ][ iy ] * x_exp[ x ][ ix ];
		    value += weight * old_data[ iy ][ ix ];
		    sum += weight;
		  }
	      new_data[ y ][ x ] = value / sum + 0.5;
	    }
	}
    }

  free_exp_filter( x_exp, sx_data, ex_data );
  free_exp_filter( y_exp, sy_data, ey_data );
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

  SIGMA_FORM = sqrt( 2.0 * log( 2.0 ) );

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

  SIGMA_FORM = sqrt( 2.0 * log( 2.0 ) );

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



/*=========================================================*
 * image__scaling_gaussian : ガウス分布を仮定しリサイズする
 *=========================================================*/
void image__scaling_gaussian
  _P5 (( image , output ),
       ( image , input  ),
       ( long  , xsize  ),
       ( long  , ysize  ),
       ( double, sigma ))
{
  SPC_SIGMA = sigma;

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
