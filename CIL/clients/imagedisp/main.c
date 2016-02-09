/*
 * filename : main.c
 * author   : Takahiro Sugiyama
 * date     : from 1993
 * describe : main routine
 */


#include "Xdisp.h"


void main(int argc, char *argv[])
{
  struct XdispAppRec app;

  initialize(&app, argc, argv);
  load(&app);
  xopen(&app);
  disp(&app);
  exec(&app);
  destrize(&app);

  exit(0);
}
