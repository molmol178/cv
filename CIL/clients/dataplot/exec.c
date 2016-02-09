/*
 * filename : exec.c
 * author   : Takahiro Sugiyama
 * date     : 1993/02/05
 */



#include "DataPlot.h"



static long quit()
{
  return 1;
}



static long redraw(self)
     DataPlot self;
{
  dataPlot_disp(self);
  return 0;
}



static xw_event key[] = {
  {'q',(xw_handler)quit},
  {'r',(xw_handler)redraw}
};



static xw_event event[] = {
  {Expose,(xw_handler)redraw}
};



void dataPlot_exec(self)
     DataPlot self;
{
  XwObject.addKeyEvents(self->graph,XwNumber(key),key);
  XwObject.addEvents(self->graph,XwNumber(event),event);

  XwWorld.mainLoop(self->world);
}
