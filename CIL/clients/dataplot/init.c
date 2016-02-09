/*
 * filename : init.c
 * author   : Takahiro Sugiyama
 * date     : 1993/02/05
 */



#include "DataPlot.h"



static char *item[] = {
  "datafiles:*:*:input data files",
  "Options",
  "config:-f:1:config file specified",
  "help:-h:print this message",
  "labelFontName:-lf:1:a14:font name of label",
  "titleFontName:-tf:1:a14:font name of title",
  "title:-t:1:DataPlot:title",
  "ylabel:-yl:1:Yaxis:y axis label",
  "xlabel:-xl:1:Xaxis:x axis label",
  "ymin:-ymin:1:minimum of y value",
  "ymax:-ymax:1:maxmum of x value",
  "xsize:-xs:1:400:window xsize",
  "ysize:-ys:1:400:window ysize",
  "dotWidth:-dw:1:5:dot width",
  "moatWidth:-mw:1:10:moat width",
  "lineWidth:-lw:1:0:line width",
  "lineLength:-ll:1:60:line length",
  "graphXSize:-gx:1:0:graph xsize",
  "graphYSize:-gy:1:0:graph ysize",
  "stringSpace:-ss:1:2:space between strings",
  "Config Parameters",
  "    label-font-name, title-font-name, title, ylabel, xlabel,",
  "    ymin, ymax, xsize, ysize, graph-xsize, graph-ysize,",
  "    dot-width, moat-width, string-space, line-width, line-length,",
  "",
  "    xcoord <labels>",
  "    ycoord <labels>",
  "    <file> <title> <dot-shape> <dashes>",
  "<dot-shapes>",
  "    oval, oval-fill, box, box-fill, triangle, triangle-fill",
  "    dia, dia-fill, cross"
};



void config_load P ((DataPlot self));

static double calc_ymin   P ((DataPlot self));
static double calc_ymax   P ((DataPlot self));
static void calc_xcoord P ((DataPlot self));
static void calc_ycoord P ((DataPlot self));



void dataPlot_init(self,argc,argv)
     DataPlot self;
     long argc;
     char *argv[];
{
  long i;
  char **datafiles;

  OPTION_SETUP( item );

  self->label_font_name = optvalue("labelFontName");
  self->title_font_name = optvalue("titleFontName");

  self->title = optvalue("title");
  self->ylabel = optvalue("ylabel");
  self->xlabel = optvalue("xlabel");
  self->xsize = optvalueint("xsize");
  self->ysize = optvalueint("ysize");
  self->dot_width = optvalueint("dotWidth");
  self->moat_width = optvalueint("moatWidth");
  self->line_width = optvalueint("lineWidth");
  self->line_length = optvalueint("lineLength");
  self->xsizegraph = optvalueint("graphXSize");
  self->ysizegraph = optvalueint("graphYSize");
  self->string_space = optvalueint("stringSpace");

  if ( optspecified( "config" ) )
    self->config_name = optvalue("config");
  else
    self->config_name = 0;

  self->ymax            = -1000;
  self->ymin            = -1000;

  self->xcoord_num      = 0;
  self->ycoord_num      = 0;
  self->xcoord          = 0;
  self->ycoord          = 0;

  if (optvaluenum("datafiles"))
    {
      self->data_num = optvaluenum("datafiles");
      self->data = typenew1( self->data_num, struct adata);
    }
  else
    {
      self->data_num = 0;
      self->data = 0;
    }

  datafiles = optvaluelist("datafiles");
  for (i = 0; i < self->data_num; i++)
    {
      self->data[i].filename  = datafiles[i];
      self->data[i].title     = datafiles[i];
      self->data[i].dot_num   = 0;
      self->data[i].dot       = 0;
      self->data[i].coord_num = 0;
      self->data[i].coord     = 0;
    }

  if (self->config_name) config_load(self);
  dataPlot_load(self);

  if (optspecified("help")) optmanual( 1 );

  if (self->data_num == 0) optusage( 1 );

  if (self->data_num == 1 && self->data[0].coord_num == 0)
    {
      self->config_name = self->data[0].filename;
      self->data_num = 0;
      typefree1(self->data);
      self->data = 0;
      if (self->config_name) config_load(self);
      dataPlot_load(self);
    }

  if (optspecified("help")) optmanual( 1 );

  if (self->data_num == 0) optusage( 1 );
  for (i = 0; i < self->data_num; i++)
    if (self->data[i].coord_num != 0) break;
  if (i == self->data_num) optmanual( 1 );

  if (optspecified("labelFontName"))
    self->label_font_name = optvalue("labelFontName");
  if (optspecified("titleFontName"))
    self->title_font_name = optvalue("titleFontName");

  if (optspecified("title")) self->title = optvalue("title");
  if (optspecified("ylabel")) self->ylabel = optvalue("ylabel");
  if (optspecified("xlabel")) self->xlabel = optvalue("xlabel");
  if (self->ymin == -1000)
    if (optspecified("ymin"))
      self->ymin = optvaluefloat("ymin");
    else
      self->ymin = calc_ymin(self);
  if (self->ymax == -1000)
    if (optspecified("ymax"))
      self->ymax = optvaluefloat("ymax");
    else
      self->ymax = calc_ymax(self);
  if (optspecified("xsize"))
    self->xsize = optvalueint("xsize");
  if (optspecified("ysize"))
    self->ysize = optvalueint("ysize");
  if (optspecified("dotWidth"))
    self->dot_width = optvalueint("dotWidth");
  if (optspecified("moatWidth"))
    self->moat_width = optvalueint("moatWidth");
  if (optspecified("lineWidth"))
    self->line_width = optvalueint("lineWidth");
  if (optspecified("lineLength"))
    self->line_length = optvalueint("lineLength");
  if (optspecified("graphXSize"))
    self->xsizegraph = optvalueint("graphXSize");
  if (optspecified("graphYSize"))
    self->ysizegraph = optvalueint("graphYSize");
  if (optspecified("stringSpace"))
    self->string_space = optvalueint("stringSpace");

  calc_xcoord(self);
  calc_ycoord(self);
}



void config_load(self)
     DataPlot self;
{
  FILE *fp;
  long n,i;
  char *head[256];
  char buf[512];
  char orig[512];

  long data_num;
  struct adata data[32];

  fp = fp_open(self->config_name,"r");
  if (fp == 0)
    {
      fprintf(stderr,"can't open config file (%s)\n",self->config_name);
      exit(1);
    }

  data_num = 0;
  while (fp_gets(fp,orig,512))
    {
      strcpy(buf,orig);
      if (*buf == '\n' || *buf == '%') continue;
      n = strsplit(buf,head,"\n :=\t");
      if (n == 0) continue;

      if (strequal(head[0],"label-font-name"))
	{
	  if (n >= 2)
	    self->label_font_name = strnew(head[1]);
	}
      else
      if (strequal(head[0],"title-font-name"))
	{
	  if (n >= 2)
	    self->title_font_name = strnew(head[1]);
	}
      else
      if (strequal(head[0],"title"))
	{
	  if (n >= 2)
	    self->title = strnew(head[1]);
	}
      else
      if (strequal(head[0],"ylabel"))
	{
	  if (n >= 2)
	    self->ylabel = strnew(head[1]);
	}
      else
      if (strequal(head[0],"xlabel"))
	{
	  if (n >= 2)
	    self->xlabel = strnew(head[1]);
	}
      else
      if (strequal(head[0],"ymin"))
	{
	  if (n >= 2)
	    self->ymin = strtodouble(head[1]);
	}
      else
      if (strequal(head[0],"ymax"))
	{
	  if (n >= 2)
	    self->ymax = strtodouble(head[1]);
	}
      else
      if (strequal(head[0],"xsize"))
	{
	  if (n >= 2)
	    self->xsize = strtolong(head[1]);
	}
      else
      if (strequal(head[0],"ysize"))
	{
	  if (n >= 2)
	    self->ysize = strtolong(head[1]);
	}
      else
      if (strequal(head[0],"graph-xsize"))
	{
	  if (n >= 2)
	    self->xsizegraph = strtolong(head[1]);
	}
      else
      if (strequal(head[0],"graph-ysize"))
	{
	  if (n >= 2)
	    self->ysizegraph = strtolong(head[1]);
	}
      else
      if (strequal(head[0],"moat-width"))
	{
	  if (n >= 2)
	    self->moat_width = strtolong(head[1]);
	}
      else
      if (strequal(head[0],"line-width"))
	{
	  if (n >= 2)
	    self->line_width = strtolong(head[1]);
	}
      else
      if (strequal(head[0],"dot-width"))
	{
	  if (n >= 2)
	    self->dot_width = strtolong(head[1]);
	}
      else
      if (strequal(head[0],"line-length"))
	{
	  if (n >= 2)
	    self->line_length = strtolong(head[1]);
	}
      else
      if (strequal(head[0],"string-space"))
	{
	  if (n >= 2)
	    self->string_space = strtolong(head[1]);
	}
      else
      if (strequal(head[0],"xcoord"))
	{
	  if (n < 2) continue;
	  self->xcoord_num = n - 1;
	  self->xcoord = typenew1(self->xcoord_num,char *);
	  for (i = 0; i < self->xcoord_num; i++)
	    self->xcoord[i] = strnew(head[i+1]);
	}
      else
      if (strequal(head[0],"ycoord"))
	{
	  if (n < 2) continue;
	  self->ycoord_num = n - 1;
	  self->ycoord = typenew1(self->ycoord_num,char *);
	  for (i = 0; i < self->ycoord_num; i++)
	    self->ycoord[i] = strnew(head[i+1]);
	}
      else
	{
	  data[data_num].filename = strnew(head[0]);

	  if (n >= 2)
	    data[data_num].title = strnew(head[1]);
	  else
	    data[data_num].title = data[data_num].filename;

	  if (n >= 3)
	    data[data_num].shape_name = strnew(head[2]);
	  else
	    data[data_num].shape_name = 0;

	  if (n >= 4)
	    {
	      data[data_num].dot_num = n - 3;
	      data[data_num].dot = memnew(1,n - 3);
	      for (i = 3; i < n; i++)
		data[data_num].dot[i-3] = strtolong(head[i]);
	    }
	  else
	    {
	      data[data_num].dot_num = 0;
	      data[data_num].dot = 0;
	    }
	  data[data_num].coord_num = 0;
	  data[data_num].coord     = 0;
	  data_num++;
	}
    }
  if (data_num)
    {
      self->data_num += data_num;
      self->data = typerenew1(self->data,self->data_num,struct adata);
      for (i = 0; i < data_num; i++)
	self->data[i] = data[i];
    }
}



static double calc_ymin(self)
     DataPlot self;
{
  long i,j;
  double ymax,ymin;
  struct adata *data;

  ymax = -100000;
  ymin = 100000;
  for (i = 0; i < self->data_num; i++)
    {
      data = &self->data[i];
      for (j = 0; j < data->coord_num; j++)
	{
	  if (data->coord[j].y > ymax) ymax = data->coord[j].y;
	  if (data->coord[j].y < ymin) ymin = data->coord[j].y;
	}
    }

  if (ymin <= 0.5)
    ymin = (double)((long)(10 * ymin))/10.0;
  else
  if (ymin < 1.0) ymin = 0;
  else
  if (ymin < 10.0) ymin = 10 * (((long)ymin) / 10);
  else
  if (ymin < 100.0) ymin = 100 * (((long)ymin) / 100);
  else
  if (ymin < 1000.0) ymin = 1000 * (((long)ymin) / 1000);
  else
  if (ymin < 10000.0) ymin = 10000 * (((long)ymin) / 10000);
  else
  if (ymin < 100000.0) ymin = 100000 * (((long)ymin) / 100000);
  else
  if (ymin < 1000000.0) ymin = 1000000 * (((long)ymin) / 1000000);

  return ymin;
}



static double calc_ymax(self)
     DataPlot self;
{
  long i,j;
  double ymax,ymin;
  struct adata *data;

  ymax = -100000;
  ymin = 100000;
  for (i = 0; i < self->data_num; i++)
    {
      data = &self->data[i];
      for (j = 0; j < data->coord_num; j++)
	{
	  if (data->coord[j].y > ymax) ymax = data->coord[j].y;
	  if (data->coord[j].y < ymin) ymin = data->coord[j].y;
	}
    }

  if (ymax <= 0.5) ymax = ((double)(((long)(10 * ymax)) + 1)) / 10.0;
  else
  if (ymax <= 1.0) ymax = 1;
  else
  if (ymax < 10.0) ymax = 10 * (((long)ymax) / 10) + 10;
  else
  if (ymax < 100.0) ymax = 100 * (((long)ymax) / 100) + 100;
  else
  if (ymax < 1000.0) ymax = 1000 * (((long)ymax) / 1000) + 1000;
  else
  if (ymax < 10000.0) ymax = 10000 * (((long)ymax) / 10000) + 10000;
  else
  if (ymax < 100000.0) ymax = 100000 * (((long)ymax) / 100000) + 100000;
  else
  if (ymax < 1000000.0) ymax = 1000000 * (((long)ymax) / 1000000) + 1000000;

  return ymax;
}



static void calc_xcoord(self)
     DataPlot self;
{
  long i;
  char *temp = "(a)";

  if (self->xcoord_num) return;

  self->xcoord_num = self->data[0].coord_num;
  self->xcoord = typenew1(self->xcoord_num,char *);
  for (i = 0; i < self->xcoord_num; i++)
    {
      temp[1] = 'a' + i;
      self->xcoord[i] = strnew(temp);
    }
}



static void calc_ycoord(self)
     DataPlot self;
{
  long i;
  double range;
  char buf[100];

  if (self->ycoord_num) return;

  range = self->ymax - self->ymin;
  if (range <= 0.5) self->ycoord_num = range / 0.02 + 1;
  else
  if (range <= 1.0) self->ycoord_num = range / 0.1 + 1;
  else
  if (range < 10.0) self->ycoord_num = range / 1 + 1;
  else
  if (range < 100.0) self->ycoord_num = range / 10 + 1;
  else
  if (range < 1000.0) self->ycoord_num = range / 100 + 1;
  else
  if (range < 10000.0) self->ycoord_num = range / 1000 + 1;
  else
  if (range < 100000.0) self->ycoord_num = range / 10000 + 1;
  else
  if (range < 1000000.0) self->ycoord_num = range / 100000 + 1;

  self->ycoord = typenew1(self->ycoord_num,char *);

  for (i = 0; i < self->ycoord_num; i++)
    {
      if (range <= 1.0)
	sprintf(buf,"%.2f",self->ymin + i * range / (self->ycoord_num-1));
      else
	sprintf(buf,"%.0f",self->ymin + i * range / (self->ycoord_num-1));
      self->ycoord[i] = strnew(buf);
    }
}
