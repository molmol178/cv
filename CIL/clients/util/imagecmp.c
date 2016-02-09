/*
 * filename : imagecmp.c
 * author   : Takahiro Sugiyama
 * date     : Wednesday, February 5 1997
 * describe : 画像の比較
 */


#include "Image.h"
#include "misc/optlib.h"
#include "misc/memlib.h"
#include <stdio.h>

#define UndefLabel -1

void image__compare_uchar(image input, image label, float times)
{
  register long x, y, xsize, ysize;
  register uchar **src_data;
  register uchar **dst_data;
  int n, err;
  char *dir;
  long max_label;
  long *label_tbl;
  long i, label_num;

  xsize = __XSIZE(input);
  ysize = __YSIZE(input);

  src_data = __DATA(input, uchar);
  dst_data = __DATA(label, uchar);

  n = 0;
  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      {
	float tmp;
	if (src_data[y][x] != dst_data[y][x])
	  {
	    fprintf(stderr,"(%d,%d): %d <> %d\n", x,y,src_data[y][x],dst_data[y][x]);
	    n++;
	  }
	tmp = times * abs(dst_data[y][x] - src_data[y][x]);
	if (tmp > 255) tmp = 255;
	dst_data[y][x] = tmp;
      }

  Image.save(label, optvalue("output"), "AREA");
}


void image__compare_short(image input, image label)
{
  register long x, y, xsize, ysize;
  register short **src_data;
  register short **dst_data;
  int n, err;
  char *dir;
  long max_label;
  long *label_tbl;
  long i, label_num;

  xsize = __XSIZE(input);
  ysize = __YSIZE(input);

  src_data = __DATA(input, short);
  dst_data = __DATA(label, short);

  n = 0;
  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      {
	if (src_data[y][x] != dst_data[y][x])
	  {
	    fprintf(stderr,"(%d,%d): %d <> %d", x,y,src_data[y][x],dst_data[y][x]);
	    n++;
	  }
	dst_data[y][x] -= src_data[y][x];
      }

  Image.save(label, optvalue("output"), "AREA");
}

static char *option[] = {
  "input:*:1:(image) name of input short/uchar image",
  "label:*:1:Label:(image) name of input short/uchar image",
  "times:-t:1:1:(float) times for diff",
  "output:-o:1:AREA:(image) name of input label image",
  "help:-h:print this messages"
};


/*========================================================================*
 * メイン
 *========================================================================*/
int main(int argc, char *argv[])
{
  char *name_of_input;
  char *name_of_label;
  image input, label;
  float times;

  OPTION_SETUP(option);
  if (optspecified("help")) optmanual(1);
  if (argc == 1) optusage(1);

  /* オプションの読みとり */
  name_of_input = optvalue("input");
  name_of_label = optvalue("label");
  times = optvaluefloat("times");

  /* 原画像の読み込み */
  input = Image.createFromFilename("Input", name_of_input);
  if (input == 0)
    {
      fprintf(stderr, "cannot open file %s\n", name_of_input);
      exit(-1);
    }
  /* 型チェック */
  if ((__TYPE(input) != Short) && (__TYPE(input) != UChar))
    {
      fprintf(stderr, "wrong image type of the image %s\n", name_of_input);
      exit(-1);
    }

  label = Image.createFromFilename("Label", name_of_label);
  if (label == 0)
    {
      fprintf(stderr, "cannot open file %s\n", name_of_label);
      exit(-1);
    }
  /* 型チェック */
  if ((__TYPE(label) != Short) && (__TYPE(label) != UChar))
    {
      fprintf(stderr, "wrong image type of the image %s\n", name_of_label);
      exit(-1);
    }

  /* エッジ特徴を求める */
  fprintf(stderr, "画像の比較…\n");
  if (__TYPE(input) == UChar)
    image__compare_uchar(input, label, times);
  else
    image__compare_short(input, label);

  Image.destroy(input);
  Image.destroy(label);

  return 0;
}
