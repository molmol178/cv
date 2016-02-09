/*
 * filename : main.c
 * author   : Takahiro Sugiyama
 * date     : 1993/02/05
 */



#include "DataPlot.h"



void main(argc,argv)
     long argc;
     char *argv[];
{
  DataPlotRec plot;

  dataPlot_init(&plot,argc,argv);
  dataPlot_open(&plot);
  dataPlot_disp(&plot);
  dataPlot_exec(&plot);
  dataPlot_close(&plot);
  dataPlot_exit(&plot);
}
