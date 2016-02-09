/*
 * filename : disp.c
 * author   : Takahiro Sugiyama
 * date     : 1993/02/05
 */



#include "DataPlot.h"



static void get_geometry    P ((DataPlot self));
static void calc_geometry   P ((DataPlot self));
static void resize_window   P ((DataPlot self));

static long max_title_width P ((DataPlot self));
static long max_title_height P ((DataPlot self));
static long xover_width  P ((DataPlot self));
static long xover_height P ((DataPlot self));
static long yover_width  P ((DataPlot self));
static long yover_height P ((DataPlot self));

static void data_plot P ((DataPlot self,struct adata *data));
static void dot_draw  P ((DataPlot self,struct adata *data,long x,long y));



static char dash[2] = {1,1};



void dataPlot_disp(self)
     DataPlot self;
{
  long i,x,y;
  long xoffset,yoffset;
  XEvent e;

  get_geometry(self);
  if (self->xsizegraph || self->ysizegraph)
    resize_window(self);
  calc_geometry(self);

  while (XwCheckTypedEvent(self->world,Expose,&e));
  XwObject.clear(self->graph);

  self->world->gc = self->titleGC;
  XwPutStringBox(self->graph,self->xlabel_x,self->xlabel_y,
		 self->xlabel_xsize,self->xlabel_ysize,"cc",self->xlabel);
  XwPutStringBox(self->graph,self->ylabel_x,self->ylabel_y,
		 self->ylabel_xsize,self->ylabel_ysize,"cc",self->ylabel);


  self->world->gc = self->labelGC;
  XwSetLineAttributes(self->world,0,LineOnOffDash,CapButt,JoinRound);
  XwSetDashes(self->world,0,dash,2);
  xoffset = self->graph_xsize / self->xcoord_num / 2;
  for (i = 0; i < self->xcoord_num; i++)
    {
      x = self->xcoord_x + xoffset + i * self->graph_xsize / self->xcoord_num;
      XwPutStringPoint(self->graph,x,self->xcoord_y+self->string_space,
		     "tc",self->xcoord[i]);
      XwDrawLine(self->graph,
		 x,self->graph_y,x,self->graph_y+self->graph_ysize-1);
    }

  for (i = 0; i < self->ycoord_num; i++)
    {
      y = self->ycoord_y + i * self->graph_ysize / (self->ycoord_num-1);
      XwPutStringPoint(self->graph,self->xcoord_x-self->string_space,y,"rc",
		       self->ycoord[self->ycoord_num-i-1]);
      XwDrawLine(self->graph,
		 self->graph_x,y,self->graph_x+self->graph_xsize-1,y);
    }
  XwSetLineAttributes(self->world,0,LineSolid,CapButt,JoinRound);

  for (i = 0; i < self->data_num; i++)
    {
      self->world->gc = self->titleGC;
      y = self->file_y + i * self->xlabel_ysize;
      XwPutStringPoint(self->graph,self->file_x+self->line_length+2,
		       y+self->string_space,"lt",self->data[i].title);

      self->world->gc = self->graphGC;
      if (self->data[i].dot_num == 0)
	XwSetLineAttributes(self->world,self->line_width,
			    LineSolid,CapButt,JoinRound);
      else
	{
	  XwSetLineAttributes(self->world,self->line_width,
			      LineOnOffDash,CapButt,JoinRound);
	  XwSetDashes(self->world,0,self->data[i].dot,self->data[i].dot_num);
	}
      XwDrawLine(self->graph,self->file_x+5,y+self->xlabel_ysize/2,
		 self->file_x+self->line_length,y+self->xlabel_ysize/2);
      dot_draw(self,&self->data[i],self->file_x+(self->line_length+5)/2,
	       y+self->xlabel_ysize/2);
    }
  XwSetLineAttributes(self->world,0,LineSolid,CapButt,JoinRound);

  XwDrawRectangle(self->graph,self->graph_x,self->graph_y,
		  self->graph_xsize+1,self->graph_ysize+1);

  for (i = 0; i < self->data_num; i++)
    {
      data_plot(self,&self->data[i]);
    }

  XwFlush(self->world);
}



static void calc_geometry(self)
     DataPlot self;
{
  self->xlabel_x = self->win_width - self->right_pool;
  self->xlabel_y = self->win_height - self->bot_pool;
  self->xlabel_xsize = self->xlabel_width;
  self->xlabel_ysize = self->xlabel_height;

  self->ylabel_x = self->left_pool - self->ylabel_width;
  self->ylabel_y = 0;
  self->ylabel_xsize = self->ylabel_width;
  self->ylabel_ysize = self->ylabel_height;

  self->xcoord_x = self->left_pool;
  self->xcoord_y = self->win_height - self->bot_pool;
  self->xcoord_xsize = self->win_width - self->left_pool - self->right_pool;
  self->xcoord_ysize = self->xcoord_height;

  self->ycoord_x = self->left_pool - self->ycoord_width;
  self->ycoord_y = self->top_pool;
  self->ycoord_xsize = self->ycoord_width;
  self->ycoord_ysize = self->win_height - self->top_pool - self->bot_pool;

  self->file_x = self->win_width - self->right_pool;
  self->file_y = self->top_pool;
  self->file_xsize = self->right_pool;
  self->file_ysize = self->win_height - self->top_pool - self->bot_pool;

  self->graph_x = self->left_pool;
  self->graph_y = self->top_pool;
  self->graph_xsize = self->win_width - self->right_pool - self->left_pool;
  self->graph_ysize = self->win_height - self->top_pool - self->bot_pool;

  self->xlabel_x += self->moat_width;
  self->xlabel_y += self->moat_width;
  self->ylabel_x += self->moat_width;
  self->ylabel_y += self->moat_width;
  self->xcoord_x += self->moat_width;
  self->xcoord_y += self->moat_width;
  self->ycoord_x += self->moat_width;
  self->ycoord_y += self->moat_width;
  self->file_x   += self->moat_width;
  self->file_y   += self->moat_width;
  self->graph_x  += self->moat_width;
  self->graph_y  += self->moat_width;
}



static long max_title_width(self)
     DataPlot self;
{
  long i;
  XCharStruct over;
  long width = 0;

  self->world->gc = self->titleGC;
  for (i = 0; i < self->data_num; i++)
    {
      XwTextExtents(self->graph,self->data[i].title,0,0,0,&over);
      if (width < (over.lbearing + over.width))
	width = over.lbearing + over.width;
    }

  return width;
}



static long max_title_height(self)
     DataPlot self;
{
  XCharStruct over;

  self->world->gc = self->titleGC;
  XwTextExtents(self->graph,self->data[0].title,0,0,0,&over);

  return over.ascent + over.descent;
}



static long xover_width(self)
     DataPlot self;
{
  long i;
  long xmax = 0;
  XCharStruct over;

  self->world->gc = self->labelGC;
  for (i = 0; i < self->xcoord_num; i++)
    {
      XwTextExtents(self->graph,self->xcoord[i],0,0,0,&over);
      if (xmax < (over.lbearing + over.width))
	xmax = over.lbearing + over.width;
    }

  return xmax;
}



static long xover_height(self)
     DataPlot self;
{
  long i;
  long ymax = 0;
  XCharStruct over;

  self->world->gc = self->labelGC;
  for (i = 0; i < self->xcoord_num; i++)
    {
      XwTextExtents(self->graph,self->xcoord[i],0,0,0,&over);
      if (ymax < (over.ascent + over.descent))
	ymax = over.ascent + over.descent;
    }

  return ymax;
}



static long yover_width(self)
     DataPlot self;
{
  long i;
  long xmax = 0;
  XCharStruct over;

  self->world->gc = self->labelGC;
  for (i = 0; i < self->ycoord_num; i++)
    {
      XwTextExtents(self->graph,self->ycoord[i],0,0,0,&over);
      if (xmax < (over.lbearing + over.width))
	xmax = over.lbearing + over.width;
    }

  return xmax;
}



static long yover_height(self)
     DataPlot self;
{
  XCharStruct over;

  self->world->gc = self->labelGC;
  XwTextExtents(self->graph,self->ycoord[self->ycoord_num-1],0,0,0,&over);

  return over.ascent + over.descent;
}



void dataPlot_print(self)
     DataPlot self;
{
  printf("config_name = %d\n",self->config_name);
  printf("label_font_name = %s\n",self->label_font_name);
  printf("title_font_name = %s\n",self->title_font_name);

  printf("title = %s\n",self->title);
  printf("ylabel = %s\n",self->ylabel);
  printf("xlabel = %s\n",self->xlabel);
  printf("ymin = %f\n",self->ymin);
  printf("ymax = %f\n",self->ymax);

  printf("data_num = %d\n",self->data_num);

  printf("xsize = %d\n",self->xsize);
  printf("ysize = %d\n",self->ysize);

  printf("xcoord_num = %d\n",self->xcoord_num);
  printf("xcoord = %d\n",self->xcoord);

  printf("ycoord_num = %d\n",self->ycoord_num);
  printf("ycoord = %d\n",self->ycoord);

  printf("xlabel_width = %d\n",self->xlabel_width);
  printf("xlabel_height = %d\n",self->xlabel_height);

  printf("ylabel_width = %d\n",self->ylabel_width);
  printf("ylabel_height = %d\n",self->ylabel_height);

  printf("xcoord_width = %d\n",self->xcoord_width);
  printf("xcoord_height = %d\n",self->xcoord_height);

  printf("ycoord_width = %d\n",self->ycoord_width);
  printf("ycoord_height = %d\n",self->ycoord_height);

  printf("file_width = %d\n",self->file_width);
  printf("file_height = %d\n",self->file_height);

  printf("xlabel_x = %d\n",self->xlabel_x);
  printf("xlabel_y = %d\n",self->xlabel_y);
  printf("xlabel_xsize = %d\n",self->xlabel_xsize);
  printf("xlabel_ysize = %d\n",self->xlabel_ysize);

  printf("ylabel_x = %d\n",self->ylabel_x);
  printf("ylabel_y = %d\n",self->ylabel_y);
  printf("ylabel_xsize = %d\n",self->ylabel_xsize);
  printf("ylabel_ysize = %d\n",self->ylabel_ysize);

  printf("xcoord_x = %d\n",self->xcoord_x);
  printf("xcoord_y = %d\n",self->xcoord_y);
  printf("xcoord_xsize = %d\n",self->xcoord_xsize);
  printf("xcoord_ysize = %d\n",self->xcoord_ysize);

  printf("ycoord_x = %d\n",self->ycoord_x);
  printf("ycoord_y = %d\n",self->ycoord_y);
  printf("ycoord_xsize = %d\n",self->ycoord_xsize);
  printf("ycoord_ysize = %d\n",self->ycoord_ysize);

  printf("file_x = %d\n",self->file_x);
  printf("file_y = %d\n",self->file_y);
  printf("file_xsize = %d\n",self->file_xsize);
  printf("file_ysize = %d\n",self->file_ysize);

  printf("graph_x = %d\n",self->graph_x);
  printf("graph_y = %d\n",self->graph_y);
  printf("graph_xsize = %d\n",self->graph_xsize);
  printf("graph_ysize = %d\n",self->graph_ysize);
}



static void data_plot(self,data)
     DataPlot self;
     struct adata *data;
{
  long i,j;
  long x,y,pre_x,pre_y,xoffset,yoffset;

  self->world->gc = self->graphGC;
  XwSetLineAttributes(self->world,self->line_width,
		      LineOnOffDash,CapButt,JoinRound);
  if (data->dot_num)
    XwSetDashes(self->world,0,data->dot,data->dot_num);
  else
    XwSetLineAttributes(self->world,self->line_width,
			LineSolid,CapButt,JoinRound);

  for (j = 0; j < data->coord_num; j++)
    if (strequal(data->coord[j].x,self->xcoord[0]))
      break;
  if (j == data->coord_num) j = 0;

  xoffset = self->graph_xsize / self->xcoord_num / 2;
  pre_x = (self->graph_x + xoffset);
  pre_y = (self->graph_y + (self->graph_ysize - self->graph_ysize * 
	    (data->coord[j].y - self->ymin) / (self->ymax - self->ymin)));

  for (i = 1; i < self->xcoord_num; i++)
    {
      for (j = 0; j < data->coord_num; j++)
	if (strequal(data->coord[j].x,self->xcoord[i]))
	  break;
      if (j == data->coord_num) j = i;

      x = (self->graph_x + xoffset + 
	   i * self->graph_xsize / self->xcoord_num);
      y = (self->graph_y + (self->graph_ysize - self->graph_ysize *
	   (data->coord[j].y - self->ymin) / (self->ymax - self->ymin)));
      XwDrawLine(self->graph,pre_x,pre_y,x,y);
      pre_x = x;
      pre_y = y;
    }

  for (i = 0; i < self->xcoord_num; i++)
    {
      for (j = 0; j < data->coord_num; j++)
	if (strequal(data->coord[j].x,self->xcoord[i]))
	  break;
      if (j == data->coord_num) j = i;
      x = (self->graph_x + xoffset + 
	   i * self->graph_xsize / self->xcoord_num);
      y = (self->graph_y + (self->graph_ysize - self->graph_ysize *
	 (data->coord[j].y - self->ymin) / (self->ymax - self->ymin)));
      dot_draw(self,data,x,y);
    }
}


static void resize_window(self)
     DataPlot self;
{
  long width,height;

  width = self->left_pool + self->right_pool + self->xsizegraph;
  height = self->top_pool + self->bot_pool + self->ysizegraph;

  XwObject.resize(self->graph,width,height);
}



static void get_geometry(self)
     DataPlot self;
{
  XCharStruct over;

  XwObject.getGeometry(self->graph,0,0,&self->win_width,&self->win_height);
  self->win_width -= self->moat_width * 2;
  self->win_height -= self->moat_width * 2;

  self->world->gc = self->titleGC;
  XwTextExtents(self->graph,self->xlabel,0,0,0,&over);
  self->xlabel_width = over.lbearing + over.width + 2*self->string_space;
  self->xlabel_height = over.ascent + over.descent + 2*self->string_space;

  XwTextExtents(self->graph,self->ylabel,0,0,0,&over);
  self->ylabel_width = over.lbearing + over.width + 2*self->string_space;
  self->ylabel_height = over.ascent + over.descent + 2*self->string_space;

  self->xcoord_width  = xover_width(self) + 2*self->string_space;
  self->xcoord_height = xover_height(self) + 2*self->string_space;
  self->ycoord_width  = yover_width(self) + 2*self->string_space;
  self->ycoord_height = yover_height(self) + 2*self->string_space;
  self->file_width    = max_title_width(self) + self->line_length + 2*self->string_space;
  self->file_height   = max_title_height(self) + 2*self->string_space;

  if (self->ycoord_width > self->ylabel_width)
    self->left_pool = self->ycoord_width;
  else
    self->left_pool = self->ylabel_width;

  if (self->file_width > self->xlabel_width)
    self->right_pool = self->file_width;
  else
    self->right_pool = self->xlabel_width;

  self->top_pool = self->ylabel_height + self->ycoord_height / 2;

  if (self->xlabel_height > self->xcoord_height)
    self->bot_pool = self->xlabel_height;
  else
    self->bot_pool = self->xcoord_height;
}



static void dot_draw(self,data,x,y)
     DataPlot self;
     struct adata *data;
     long x,y;
{
  long rx,ry,lx,ly,w;

  w = self->dot_width;
  rx = x-self->dot_width/2;
  ry = y-self->dot_width/2;
  lx = rx + w;
  ly = ry + w;

  self->world->gc = self->titleGC;

  if (self->dot_width) {
    if (data->shape == 1) {
      XwSetForeground(self->world,self->world->white_pixel);
      XwFillRectangle(self->graph,rx,ry,w,w);
      XwSetForeground(self->world,self->world->black_pixel);
      XwDrawLine(self->graph,rx,ry,lx,ly);
      XwDrawLine(self->graph,lx,ry,rx,ly);
    } else
    if (data->shape) {
      if (!data->fill) {
	XwSetForeground(self->world,self->world->white_pixel);
	data->shape->fill(self->graph,rx,ry,w,w);
	XwSetForeground(self->world,self->world->black_pixel);
	data->shape->draw(self->graph,rx,ry,w,w);
      } else {
	data->shape->fill(self->graph,rx,ry,w,w);
	data->shape->draw(self->graph,rx,ry,w,w);
      }
    }
  }
  self->world->gc = self->graphGC;
}
