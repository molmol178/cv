/*
 * labelinfo.c
 */



#include "Image.h"
#include "Image/Funcs.h"
#include <stdio.h>
#include "misc/filelib.h"
#include "misc/memlib.h"
#include "misc/optlib.h"



typedef struct labelTABLE {
  short area;
  short len;
  long x,y;
  long neg_num;
  long *neg_list;
} LabelTABLE;



long table_num = 0;
LabelTABLE *table_list = 0;
char *out = "-";

void labelinfo_calc   P ((image src));
void image_label_disp P ((image src));



long areamax = 1024 * 1024;
long areamin = 1;
long neighbor = 0;
long detail = 0;


static char *item[] = {
  "output:-o:1:stdout:output filename",
  "area-max:-amax:1:maximun of region area",
  "area-min:-amin:1:1:minumum of region area",
  "neighbor:-n:neighbor infomation",
  "detail:-d:in detail"
};



void main(argc,argv)
     int argc;
     char **argv;
{
  image src;

  optinit(Number(item),item,argc,argv);

  if (optargc < 1)
    {
      optmanual("Usage : labelinfo [options] <label-image> [options]");
      exit(-1);
    }
  src = Image.createFromFilename("original",argv[1]);

  if (optspecified("output")) out = optvalue("output");
  if (optspecified("area-max"))
    areamax = atol(optvalue("area-max"));
  else
    areamax = Image.xsize(src) * Image.ysize(src);
  if (optspecified("area-min")) areamin = atol(optvalue("area-min"));
  if (optspecified("neighbor")) {neighbor = 1;detail = 1;}
  if (optspecified("detail")) detail = 1;

  image__long_type(src,src);

  labelinfo_calc(src);
  image_label_disp(src);
}



void addneg(l,p)
     LabelTABLE *l;
     long p;
{
  int i;

  for (i = 0; i < l->neg_num; i++)
    if (l->neg_list[i] == p) return;

  l->neg_num++;

  l->neg_list = typerenew1( l->neg_list, l->neg_num, long );

  l->neg_list[i] = p;
}



void labelinfo_calc(src)
     image src;
{
  long xsize,ysize;
  register long i,x,y;
  register long **p;

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  p     = (long **)Image.data(src);

  table_num = 0;
  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      if (p[y][x] > table_num) table_num = p[y][x];

  table_list = typenew1(table_num + 1, LabelTABLE);
  if (table_list == 0)
    {
      fprintf(stderr,"can't alloc memory. It's too large.\n");
      exit(-1);
    }

  for (i = 1; i <= table_num; i++) {
    table_list[i].area = 0;
    table_list[i].len = 0;
    table_list[i].x = 0;
    table_list[i].y = 0;
    table_list[i].neg_num = 0;
    table_list[i].neg_list = 0;
  };

  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      {
	long i = p[y][x];
	LabelTABLE *l = &table_list[i];

	l->area ++;
	l->x += x;
	l->y += y;

	if (y == 0 || x == 0 || x == (xsize-1) || y == (ysize-1))
	  l->len++;
	if ((y != 0) && (p[y-1][x] != i)) {l->len++;addneg(l,p[y-1][x]);}
	if ((x != 0) && (p[y][x-1] != i)) {l->len++;addneg(l,p[y][x-1]);}
	if ((y != (ysize-1)) && (p[y+1][x] != i))
	  {l->len++;addneg(l,p[y+1][x]);}
	if ((x != (xsize-1)) && (p[y][x+1] != i))
	  {l->len++;addneg(l,p[y][x+1]);}
      }

  for (i = 1; i <= table_num; i++)
    if (table_list[i].area)
      {
	table_list[i].x /= table_list[i].area;
	table_list[i].y /= table_list[i].area;
      }
}



void image_label_disp(src)
     image src;
{
  FILE *fp;
  long i,j;
  long c = 0;

  fp = fp_open(out,"w");
  fprintf(fp,"number of table = %d\n",table_num);

  for (i = 1; i <= table_num; i++)
    {
      LabelTABLE *l = &table_list[i];

      if ((areamax < l->area) || (l->area < areamin)) continue;
      c++;

      if (detail)
	{
	  fprintf(fp,"%5d = { (Gx,Gy)=(%3d,%3d), length=%3d, area=%3d}",
		  i,l->x,l->y,l->len,l->area);
	  if (neighbor && l->neg_num)
	    {
	      fprintf(fp,"{neg=");
	      for (j = 0; j < l->neg_num; j++)
		{
		  fprintf(fp,"%d",l->neg_list[j]);
		  if (j != (l->neg_num-1)) fprintf(fp,",");
		}
	      fprintf(fp,"}\n");
	    }
	  else
	    fprintf(fp,"\n");
	}
    }
  fprintf(fp,"number of (%d <= area <= %d) = %d\n",areamin,areamax,c);
  fp_close(fp);
}
