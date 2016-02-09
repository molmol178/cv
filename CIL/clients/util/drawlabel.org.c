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
  "help:-h:print this messages",
};



/*------------------------------------------------------------------------*
 * ラベルの最大値を求める。
 *------------------------------------------------------------------------*/
long label_max(image input)
{
  int i;
  long label_max;
  long area = __AREA(input);

  label_max = 0;
  for (i = 0; i < area; i++)
    {
      if (label_max < __RAXEL(input,i,long))
	label_max = __RAXEL(input,i,long);
    }
  return label_max;
}

/*------------------------------------------------------------------------*
 * ルックアップテーブルを読み込む。
 *------------------------------------------------------------------------*/
#include <unistd.h>
char **make_label_lookup_table(image input, long *n_ptr)
{
  long i, n, nw, index;
  char **lut;
  char *name_of_lut;
  char buf[256];
  char *head[16];
  FILE *fp;

  n = label_max(input);
  *n_ptr = n;
  fprintf(stderr,"the number of label is %d\n", n);

  lut = typenew1(n+2, char *);
  /* 初期ラベル */
  for (i = 0; i <= n; i++)
    {
      sprintf(buf, "%d", i);
      lut[i] = (char *)strcpy((char *)malloc(strlength(buf)+1), buf);
    }

  if (! optspecified("lookup-table")) return lut;

  name_of_lut = optvalue("lookup-table");
  if (access(name_of_lut,R_OK) != 0)
    {
      fprintf(stderr,"cannot read lookup tabel %s\n", name_of_lut);
      fprintf(stderr,"\tignore this file\n");
      return lut;
    }

  fp = fopen(name_of_lut,"r");
  if (fp == 0)
    {
      fprintf(stderr,"cannot open lookup tabel %s\n", name_of_lut);
      fprintf(stderr,"\tignore this file\n");
      return lut;
    }

  while (fgets(buf, sizeof(buf), fp))
    {
      if (buf[0] == '#') continue;
      nw = strsplit(buf, head, "\t\n ");
      if (nw != 2) continue;
      index = strtolong(head[0]);
      if (index > n)
	{
	  fprintf(stderr,"illegal index value %s, ignore it\n", head[0]);
	  continue;
	}
      strfree(lut[index]);
      lut[index] = strnew(head[1]);
    }
  fclose(fp);

  return lut;
}


/*------------------------------------------------------------------------*
 * 面積を測る。
 *------------------------------------------------------------------------*/
void count_area(long *area_list, long n, image input)
{
  int i;
  long area = __AREA(input);

  for (i = 0; i <= n; i++)
    area_list[i] = 0;
  for (i = 0; i < area; i++)
    if (__RAXEL(input,i,long) >= 0)
      area_list[__RAXEL(input,i,long)]++;
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
 * 境界から最も離れた位置を探す。
 *------------------------------------------------------------------------*/
long find_central_coord(image input, image distance, long index,
			long *ox_ptr, long *oy_ptr, long *d_ptr)
{
  long d, ox, oy, n;
  int x, y, xsize, ysize;
  long ox_l[100], oy_l[100];

  xsize = __XSIZE(input);
  ysize = __YSIZE(input);

  d = 0;
  ox = oy = 0;
  for ( y = 0; y < ysize; y++ )
    for ( x = 0; x < xsize; x++ )
      if ((__PIXEL(input, x, y, long) == index) &&
	  (d < __PIXEL(distance, x, y, short)))
	{
	  d = __PIXEL(distance, x, y, short);
	  ox = x; oy = y;
	}

  n = 0;
  for ( y = 0; y < ysize; y++ )
    for ( x = 0; x < xsize; x++ )
      if ((__PIXEL(input, x, y, long) == index) &&
	  (d == __PIXEL(distance, x, y, short)))
	{
	  if (n < 100)
	    ox_l[n] = x; oy_l[n] = y; n++;
	}

  if (n == 0) return 0;
  ox = ox_l[n/2];
  oy = oy_l[n/2];

  *d_ptr = d;
  *ox_ptr = ox;
  *oy_ptr = oy;
  return 1;
}

/*------------------------------------------------------------------------*
 * 空いているスペースを見つける。
 *------------------------------------------------------------------------*/
void find_and_draw_label(image output, image input, image distance,
			 long index, char *label)
{
  long d, ox, oy, x, y, xx, yy;
  long xsize, ysize;
  long room_xsize, room_ysize;
  long max_distance;
  image str;

  xsize = __XSIZE(input);
  ysize = __YSIZE(input);

  str = ImageCreateString(label, strlength(label));
  if (find_central_coord(input, distance, index, &ox, &oy, &d) == 0)
    {
      return;
    }

  room_xsize = __XSIZE(str);
  room_ysize = __YSIZE(str);
  max_distance = (room_xsize > room_ysize) ? room_xsize/2 : room_ysize/2;

  if (max_distance > d)
    {
      fprintf(stderr," [maybe overlapped \"%s\"]", label);
      /* return; */
    }

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
 * ラベルを書き込む。
 *------------------------------------------------------------------------*/
void draw_label(image output, image input, char **lut, long n,
		char *name_of_font, double scale, long area)
{
  long i;
  long *area_list;
  image distance;

  area_list = typenew1(n+2, long);
  count_area(area_list, n, input);
  ImageSetStringFont(name_of_font);
  scaling_label(input, scale);
  draw_boundary(output, input);

  distance = Image.create("Distance");
  calc_label_distance(distance, input);
  for (i = 0; i <= n; i++)
    {
      if (area_list[i] < area) continue;
      find_and_draw_label(output, input, distance, i, lut[i]);
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
  char **lut;
  long n;

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

  lut = make_label_lookup_table(input, &n);
  draw_label(output, input, lut, n, name_of_font, scale, area);
  Image.save(output, name_of_output, "Label Image");
  fprintf(stderr,"\n");

  Image.destroy(output);
  Image.destroy(input);

  return 0;
}
