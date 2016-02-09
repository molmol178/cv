/*
 * filename : filtering.c
 * author   : Takahiro Sugiyama
 * date     : Thursday, April 7 1994
 * describe : general filtering
 */



#include "Funcs.h"
#include "Filter.h"
#include <stdio.h>


/*=====================================================================*
 * image__filtering_regular_uchar : 正規化して一般的なフィルタをかける
 *=====================================================================*/
void image__filtering_regular_uchar
  _P3 (( image   , your_output ), /* フィルタの結果(float) */
       ( image   , input       ), /* 入力画像(uchar) */
       ( register t_filter, filter )) /* フィルタ画像(double) */
{
  long x, y, xsize, ysize;
  register uchar **input_data;
  register float **output_data;

  image output;

  xsize = __XSIZE( input );
  ysize = __YSIZE( input );

  if ( your_output == input )
    output = Image.create( "output" );
  else
    output = your_output;

  Image.make( output, Float, xsize, ysize );

  input_data  = __DATA( input , uchar );
  output_data = __DATA( output, float );

  for ( y = 0; y < ysize; y++ )
    for ( x = 0; x < xsize; x++ )
      {
	register int xx, yy, ix, iy;
	double value, weight;

	value = 0;
	for ( yy = filter->top; yy <= filter->bottom; yy++ )
	  {
	    iy = y + yy;
	    if ( iy < 0 || iy >= ysize ) continue;
	    for ( xx = filter->left; xx <= filter->right; xx++ )
	      {
		ix = x - xx;
		if ( ix < 0 || ix >= xsize ) continue;
		weight = filter->data.of_double[ yy ][ xx ];
		value += weight * input_data[ iy ][ ix ];
	      }
	  }
	output_data[ y ][ x ] = value;
      }

  if ( your_output == input )
    {
      Image.copy( your_output, output );
      Image.destroy( output );
    }
}



typedef void (*f_caller) P3 (( image   , output ),
			     ( image   , input  ),
			     ( t_filter, filter ))



static caller_table table[] = {
  { UChar,  image__filtering_regular_uchar },
};



/*===============================================================*
 * image__filtering_regular : 正規化して一般的なフィルタをかける
 *===============================================================*/
void image__filtering_regular
  _P3 (( image   , output ),
       ( image   , input  ),
       ( t_filter, filter ))
{
  f_caller caller;

  caller = ( f_caller )image__caller( "image__filtering_regular",
				      input, Number( table ), table );

  if ( caller ) caller( output, input, filter );
}



/*=============================================*
 * image__filtering : 一般的なフィルタをかける
 *=============================================*/
void image__filtering
  _P5 (( image      , your_output ), /* フィルタリング結果 */
       ( int        , output_type ), /* 出力の画素型 */
       ( image      , input       ), /* 入力画像 */
       ( register t_localfunc, func        ), /* ローカル関数 */
       ( char *     , user        )) /* ユーザ指定値 */
{
  register long x, y, xsize, ysize;
  image output;

  xsize = __XSIZE( input );
  ysize = __YSIZE( input );

  if ( your_output == input )
    output = Image.create( "output" );
  else
    output = your_output;

  Image.make( output, output_type, xsize, ysize );

  for ( y = 0; y < ysize; y++ )
    for ( x = 0; x < xsize; x++ )
      func( output, input, x, y, user );

  if ( your_output == input )
    {
      Image.copy( your_output, output );
      Image.destroy( output );
    }
}
