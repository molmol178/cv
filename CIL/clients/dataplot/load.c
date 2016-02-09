/*
 * filename : load.c
 * author   : Takahiro Sugiyama
 * date     : 1993/02/05
 */



#include "DataPlot.h"



static void load_data P ((struct adata *data));
static void string2shape P ((struct adata *data));



static long dot_num[] = {0,2,4,6};
static char dot1[] = {0};
static char dot2[] = {2,2};
static char dot3[] = {6,2,2,2};
static char dot4[] = {6,2,2,2,2,2};
static char *dot[] = {dot1,dot2,dot3,dot4};

static char *shape_name[] = {
  "oval","oval-fill",
  "box","box-fill",
  "triangle","triangle-fill",
  "dia","dia-fill",
  "cross"
};


void dataPlot_load(self)
     DataPlot self;
{
  long i,ok = 1,ok2 = 1;

  for (i = 0; i < self->data_num; i++)
    {
      if (self->data[i].title == 0)
	self->data[i].title = self->data[i].filename;

      if (self->data[i].dot_num == 0 && ok == 1)
	{
	  self->data[i].dot_num = dot_num[i%4];
	  self->data[i].dot = dot[i%4];
	} else ok = 0;

      if (self->data[i].shape_name == 0 && ok2 == 1)
	self->data[i].shape_name = shape_name[i%9];
      else
	ok2 = 0;
      string2shape(&self->data[i]);

      load_data(&(self->data[i]));
    }
}



static void load_data(data)
     struct adata *data;
{
  FILE *fp;
  long n,i;
  char *head[256];
  char buf[512];
  long coord_num;
  struct acoord coord[128];

  fp = fp_open(data->filename,"r");
  if (fp == 0)
    {
      fprintf(stderr,"warning:can't open file (%s)\n",data->filename);
      return;
    }

  coord_num = 0;
  while (fp_gets(fp,buf,512))
    {
      if (*buf == '\n') continue;
      n = strsplit(buf,head,"\n \t");
      if (n == 0) continue;
      coord[coord_num].x = strnew(head[0]);
      if (n >= 2)
	coord[coord_num].y = strtodouble(head[1]);
      else
	coord[coord_num].y = strtodouble(head[0]);
      coord_num++;
    }

  for (i = 0; i < coord_num; i++)
    if (coord[i].y != 0) break;
  if (i == coord_num) return;

  data->coord = typenew1(coord_num,struct acoord);

  data->coord_num = coord_num;
  for (i = 0; i < coord_num; i++)
    data->coord[i] = coord[i];
}



static void string2shape(data)
     struct adata *data;
{
  char buf[512];
  long n;
  char *head[12];

  if (data->shape_name == 0)
    {
      data->fill = 0;
      data->shape = 0;
      return;
    }

  strcpy(buf,data->shape_name);
  n = strsplit(buf,head,"-");

  if (n == 0)
    {
      data->fill = 0;
      data->shape = 0;
      return;
    }

  if (n == 1)
    data->fill = 0;
  else
    data->fill = 1;

  if (strequal("oval",head[0]))
    data->shape = Oval.shape;
  else
  if (strequal("box",head[0]))
    data->shape = Box.shape;
  else
  if (strequal("triangle",head[0]))
    data->shape = UpTriangle.shape;
  else
  if (strequal("dia",head[0]))
    data->shape = Dia.shape;
  else
    data->shape = 1;
}
