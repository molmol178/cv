/*
 * filename : open.c
 * author   : Takahiro Sugiyama
 * date     : 1993/02/05
 */



#include "DataPlot.h"



void dataPlot_open(self)
     DataPlot self;
{
  self->world = XwWorld.create(self,"DataPlot");
  self->graph = XwObject.create(self->world,XwHard.class,0,self->title);
  XwObject.configure(self->graph,0,0,self->xsize,self->ysize,1);
  XwObject.map(self->graph);

  self->labelGC = self->world->gc;
  self->graphGC = XCreateGC(self->world->display,self->graph->id,0,0);
  self->titleGC = XCreateGC(self->world->display,self->graph->id,0,0);

  self->world->gc = self->labelGC;
  XwSetForeground(self->world,XwWorld.blackPixel(self->world));
  XwSetFont(self->world,XwLoadFont(self->world,self->label_font_name));

  self->world->gc = self->titleGC;
  XwSetForeground(self->world,XwWorld.blackPixel(self->world));
  XwSetFont(self->world,XwLoadFont(self->world,self->title_font_name));

  self->world->gc = self->graphGC;
  XwSetForeground(self->world,XwWorld.blackPixel(self->world));
}
