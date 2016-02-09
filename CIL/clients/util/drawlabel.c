/*
 * filename : writeno.c
 * author   : Takahiro Sugiyama
 * date     : Monday, April 1 1996
 * describe : Write Label No
 */


#include <stdio.h>
#include "Image.h"
#include "Xcil/ImageCG.h"
#include "Image/Funcs.h"
#include "misc/optlib.h"
#include "misc/strlib.h"


static char *option[] = {
  "input:*:1:(image) name of input label image",
  "output:-o:1:labelno.out:(image) name of output labeled image",
  "lookup-table:-lut:1:(text) name of lookup table file",
  "scale:-s:1:1:(float) scale for output image",
  "font-name:-fn:1:a14:(font) name of label font",
  "area:-a:1:200:(int) threshold for region area",
  "gray:-g:do gray-scale image",
  "help:-h:print this messages",
};



typedef struct RegionRec {
  long area;
  long label;
  short x, y;
  short distance;
  char *str;

  short n;
  short x_l[257], y_l[257];

} RegionRec;



/*------------------------------------------------------------------------*
 * ルックアップテーブルを読み込む。
 *------------------------------------------------------------------------*/
#include <unistd.h>
void set_label(RegionRec *list, long num)
{
  long i, n, nw, index;
  char **lut;
  char *name_of_lut;
  char buf[256];
  char *head[16];
  FILE *fp;

  /* 初期ラベル */
  for (i = 1; i <= num; i++)
    if (list[i].area)
      {
	sprintf(buf, "%d", list[i].label);
	list[i].str = (char *)strcpy((char *)malloc(strlength(buf)+1), buf);
      }

  if (! optspecified("lookup-table")) return;

  name_of_lut = optvalue("lookup-table");
  if (access(name_of_lut,R_OK) != 0)
    {
      fprintf(stderr,"cannot read lookup tabel %s\n", name_of_lut);
      fprintf(stderr,"\tignore this file\n");
      return;
    }

  fp = fopen(name_of_lut,"r");
  if (fp == 0)
    {
      fprintf(stderr,"cannot open lookup tabel %s\n", name_of_lut);
      fprintf(stderr,"\tignore this file\n");
      return;
    }

  while (fgets(buf, sizeof(buf), fp))
    {
      if (buf[0] == '#') continue;
      nw = strsplit(buf, head, "\t\n ");
      if (nw != 2) continue;
      index = strtolong(head[0]);
      for (i = 1; i <= num; i++)
	if (index == list[i].label)
	  {
	    strfree(list[i].str);
	    list[i].str = strnew(head[1]);
	  }
    }
  fclose(fp);

  return;
}



/*------------------------------------------------------------------------*
 * 画像を拡大縮小する。
 *------------------------------------------------------------------------*/
void scaling_label(image input, double scale)
{
  long x, y, xx, yy, xsize, ysize;
  long ex_xsize, ex_ysize;
  image tmp;

  if (scale == 1.0) return;

  tmp = Image.createFromImage("TMP", input);
  ex_xsize = __XSIZE(input);
  ex_ysize = __YSIZE(input);
  xsize = ex_xsize * scale;
  ysize = ex_ysize * scale;
  fprintf(stderr,"image scaling ... %.2f[%dx%d -> %dx%d]\n",
	  scale, ex_xsize, ex_ysize, xsize, ysize);
  Image.make(input, Long, xsize, ysize);
  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      {
	xx = ex_xsize * x / xsize;
	yy = ex_ysize * y / ysize;
	__PIXEL(input, x, y, long) = __PIXEL(tmp, xx, yy, long);
      }
  Image.destroy(tmp);
}


/*------------------------------------------------------------------------*
 * 境界線を描く。
 *------------------------------------------------------------------------*/
void draw_boundary(image output, image input)
{
  int x, y, xsize, ysize;
  long label;
  long **data;

  xsize = __XSIZE(input);
  ysize = __YSIZE(input);
  Image.make(output, Bit1, xsize, ysize);
  data = __DATA(input, long);
  for ( y = 0; y < ysize; y++ )
    for ( x = 0; x < xsize; x++ )
      {
	label = data[y][x];
	if (( x == 0 || y == 0 ||
	      x == ( xsize - 1 ) || y == ( ysize - 1 )) ||
	    ( data[ y ][ x-1 ] != label ) ||
	    ( data[ y-1 ][ x ] != label ) ||
	    ( data[ y-1 ][ x-1 ] != label ))
	  __PIXEL(output, x, y, bit1) = 1;
      }
}

/*------------------------------------------------------------------------*
 * 距離画像を作る。
 *------------------------------------------------------------------------*/
void calc_label_distance(image distance, image input)
{
  int x, y, xsize, ysize;
  long label;
  long **data;
  short **dis_data;
  long min_dis;

  xsize = __XSIZE(input);
  ysize = __YSIZE(input);
  data = __DATA(input, long);

  Image.make(distance, Short, xsize, ysize);
  dis_data = __DATA(distance, short);

  for ( y = 0; y < ysize; y++ )
    for ( x = 0; x < xsize; x++ )
      {
	if (x == 0 || y == 0) { dis_data[y][x] = 1; continue; }
	label = data[y][x];

	min_dis = 1000000;

	if (data[y][x-1] == label)
	  min_dis = (min_dis > dis_data[y][x-1]) ? dis_data[y][x-1] : min_dis;
	else { dis_data[y][x] = 1; continue; }

	if (data[y-1][x] == label)
	  min_dis = (min_dis > dis_data[y-1][x]) ? dis_data[y-1][x] : min_dis;
	else { dis_data[y][x] = 1; continue; }

	if (data[y-1][x-1] == label)
	  min_dis = (min_dis > dis_data[y-1][x-1]) ? dis_data[y-1][x-1] : min_dis;
	else { dis_data[y][x] = 1; continue; }

	dis_data[y][x] = min_dis + 1;
      }

  for ( y = ysize-1; y > 0; y-- )
    for ( x = xsize-1; x > 0; x-- )
      {
	if (x == (xsize-1) || y == (ysize-1)) { dis_data[y][x] = 1; continue; }
	label = data[y][x];

	min_dis = dis_data[y][x];

	if (data[y][x+1] == label)
	  min_dis = (min_dis > dis_data[y][x+1]+1) ? dis_data[y][x+1]+1 : min_dis;
	else { dis_data[y][x] = 1; continue; }

	if (data[y+1][x] == label)
	  min_dis = (min_dis > dis_data[y+1][x]+1) ? dis_data[y+1][x]+1 : min_dis;
	else { dis_data[y][x] = 1; continue; }

	if (data[y+1][x+1] == label)
	  min_dis = (min_dis > dis_data[y+1][x+1]+1) ? dis_data[y+1][x+1]+1 : min_dis;
	else { dis_data[y][x] = 1; continue; }

	dis_data[y][x] = min_dis;
      }
}


/*------------------------------------------------------------------------*
 * 文字を書く。
 *------------------------------------------------------------------------*/
void image_draw_label(image output, long ox, long oy, char *label)
{
  long d, x, y, xx, yy;
  long xsize, ysize;
  long room_xsize, room_ysize;
  long max_distance;
  image str;

  xsize = __XSIZE(output);
  ysize = __YSIZE(output);

  str = ImageCreateString(label, strlength(label));

  fprintf(stderr,"DrawLabel at (%d, %d) \"%s\"\n", ox, oy, label);

  room_xsize = __XSIZE(str);
  room_ysize = __YSIZE(str);

  ox = ox - room_xsize / 2;
  oy = oy - room_ysize / 2;
  for (y = 0; y < room_ysize; y++)
    for (x = 0; x < room_xsize; x++)
      {
	xx = ox + x;
	yy = oy + y;
	if (xx < 0 || xx >= xsize || yy < 0 || yy >= ysize) continue;
	if (__PIXEL(str, x, y, bit1))
	  __PIXEL(output, xx, yy, bit1) = 1;
      }

  Image.destroy(str);
}


/*------------------------------------------------------------------------*
 * 領域情報を計算する。
 *------------------------------------------------------------------------*/
void calc_region_info(RegionRec *list, long num,
		      image label, image distance, image input)
{
  int i, x, y, xsize, ysize;

  xsize = __XSIZE(input);
  ysize = __YSIZE(input);

  for (i = 0; i <= num; i++)
    list[i].area = 0;

  for ( y = 0; y < ysize; y++ )
    for ( x = 0; x < xsize; x++ )
      {
	i = __PIXEL(label, x, y, long);
	list[i].area++;
	list[i].label = __PIXEL(input, x, y, long);
	if (list[i].distance < __PIXEL(distance, x, y, short))
	  list[i].distance = __PIXEL(distance, x, y, short);
      }
  for ( y = 0; y < ysize; y++ )
    for ( x = 0; x < xsize; x++ )
      {
	i = __PIXEL(label, x, y, long);
	if (list[i].distance == __PIXEL(distance, x, y, short) && list[i].n < 257)
	  {
	    list[i].x_l[list[i].n] = x;
	    list[i].y_l[list[i].n] = y;
	    list[i].n++;
	  }
      }

  for (i = 1; i <= num; i++)
    if (list[i].area)
      {
	list[i].x = list[i].x_l[list[i].n/2];
	list[i].y = list[i].y_l[list[i].n/2];
      }

  set_label(list, num);
}


/*------------------------------------------------------------------------*
 * ラベルを書き込む。
 *------------------------------------------------------------------------*/
void draw_label(image output, image input,
		char *name_of_font, double scale, long area)
{
  long i;
  long *area_list;
  image distance, label;
  long region_num;
  RegionRec *region_list;

  ImageSetStringFont(name_of_font);
  scaling_label(input, scale);
  draw_boundary(output, input);

  distance = Image.create("Distance");
  calc_label_distance(distance, input);

  label = Image.create("Label");
  region_num = image__labeling(label, input);
  fprintf(stderr,"a number of labels = %d\n", region_num);
  region_list = typenew1(region_num + 1, RegionRec);
  calc_region_info(region_list, region_num, label, distance, input);

  for (i = 1; i <= region_num; i++)
    {
      if (region_list[i].area < area) continue;
      image_draw_label(output, region_list[i].x, region_list[i].y,
		       region_list[i].str);
    }
}


/*------------------------------------------------------------------------*
 * 濃淡画像に変更する。
 *------------------------------------------------------------------------*/
void paint_gray_scale(image output, image input)
{
  long x, y;
  long xsize, ysize;

  xsize = __XSIZE(output);
  ysize = __YSIZE(output);
  __TYPE(output) = UChar;

  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      {
	if (__PIXEL(output, x, y, uchar) == 1)
	  __PIXEL(output, x, y, uchar) = 0;
	else
	  {
	    if (__PIXEL(input, x, y, long) == 0)
	      __PIXEL(output, x, y, uchar) = (((x + y) % 5) == 0) ? 0 : 255;
	    else
	    if (__PIXEL(input, x, y, long) > 0)
	      __PIXEL(output, x, y, uchar) = 255;
	    else
	    if (__PIXEL(input, x, y, long) < 0)
	      __PIXEL(output, x, y, uchar) = 200;
	  }
      }
}


/*------------------------------------------------------------------------*
 * メインだよ。
 *------------------------------------------------------------------------*/
int main(argc, argv)
     int argc;
     char *argv[];
{
  char *name_of_input;
  char *name_of_output;
  char *name_of_font;
  double scale;
  long area;
  image input, output;

  OPTION_SETUP( option );

  if (optspecified("help")) optmanual(1);
  if (argc == 1) optusage(1);

  name_of_input = optvalue("input");
  name_of_output = optvalue("output");
  name_of_font = optvalue("font-name");
  area = optvalueint("area");
  scale = optvaluefloat("scale");

  input = Image.createFromFilename("Input", name_of_input);
  if (input == 0)
    {
      fprintf(stderr,"error: cannot read \"%s\"\n", name_of_input);
      exit(-1);
    }
  output = Image.create("Output");

  if (__TYPE(input) != Long)
    {
      image__long_type(input, input);
      fprintf(stderr, "change to long type image\n");
    }

  draw_label(output, input, name_of_font, scale, area);

  if (optspecified("gray"))
    paint_gray_scale(output, input);

  Image.save(output, name_of_output, "Label Image");
  fprintf(stderr,"\n");

  Image.destroy(output);
  Image.destroy(input);

  return 0;
}
