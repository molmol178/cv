/*
 * filename : close.c
 * author   : Takahiro Sugiyama
 * date     : 1993/02/05
 */



#include "DataPlot.h"



void dataPlot_close(self)
     DataPlot self;
{
  XFreeGC(self->world->display,self->graphGC);
  XFreeGC(self->world->display,self->titleGC);
  XwWorld.destroy(self->world);
}
