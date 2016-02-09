#include "Image.h"
#include "misc/memlib.h"
#include "misc/optlib.h"
#include <math.h>

/*------------------------------------------------*
 * ラプラシアン・ガウシアン・フィルタを作成する。
 *------------------------------------------------*/
static double **make_LOG_filter(double sigma, long *n_ptr)
{
  int j,i,n;
  double **filter;
  register double *p;

  n = sigma * 4.2;
  filter = typenew2( 2 * n + 1, 2 * n + 1, double );

  for (i = -n; i <= n; i++)
    for (j = -n; j <= n; j++)
      filter[j+n][i+n] = (- 1 + (i*i+j*j) / (2.0 * sigma * sigma))
	* exp(- (i*i+j*j) / (2.0 * sigma * sigma))
	  / (M_PI * sigma * sigma * sigma * sigma);

  *n_ptr = n;

  return filter;
}

/*----------------------*
 * フィルタを適用する。
 *----------------------*/
static void LOG_filter(image out, image src, double sigma)
{
  register long x, y, xsize, ysize;
  register double **filter;
  register float **out_data;
  register uchar  **src_data;
  long n;

  xsize = __XSIZE( src );
  ysize = __YSIZE( src );
  filter = make_LOG_filter(sigma, &n);
  Image.make(out, Float, xsize, ysize);

  src_data = __DATA(src, uchar);
  out_data = __DATA(out, float);

  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      {
	register long lx, ly;
	double v = 0;
	for (ly = -n; ly <= n; ly++)
	  for (lx = -n; lx <= n; lx++)
	    {
	      register long xx = x + lx;
	      register long yy = y + ly;
	      if (yy < 0) yy = 0; else if (yy >= ysize) yy = ysize -1;
	      if (xx < 0) xx = 0; else if (xx >= xsize) xx = xsize -1;
	      v += src_data[yy][xx] * filter[ly+n][lx+n];
	    }
	out_data[y][x] = v;
      }
  typefree2(filter);
}

/* オプションの定義 */
char *option[] = {
  "The LOG filtering.",
  "input:*:1:(image) UChar型の入力画像ファイル名",
  "output:-o:1:Output:(image) Float型の出力画像ファイル名",
  "sigma:-s:1:1:(real) ガウス関数の標準偏差",
  "help:-h:print help messages",
};


int main(int argc, char *argv[])
{
  image input, output;
  char *input_name;
  char *output_name;
  double sigma;

  OPTION_SETUP(option);

  if (argc == 1) optusage(1);
  if (optspecified("help")) optmanual(1);

  input_name  = optvalue("input");
  output_name = optvalue("output");
  sigma = optvaluefloat("sigma");

  input = Image.createFromFilename("INPUT", input_name);
  output  = Image.create("LOG");

  if (input == 0)
    {
      fprintf(stderr, "output:can't open file %s\n", input_name);
      exit(-1);
    }

  LOG_filter(output, input, sigma);
  Image.save(output, output_name, "LOG");

  Image.destroy(input);
  Image.destroy(output);

  return 0;
}
