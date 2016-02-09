/*
 * filename : main.c
 * author   : Takahiro Sugiyama
 * date     : Saturday, February 8 1997
 * describe : メインルーチン
 */


#include "observe.h"


/*------------------------------------------------------------------------*
 * メイン
 *------------------------------------------------------------------------*/
void main(int argc, char *argv[])
{
  ObserveRec observeRec;

  initialize(&observeRec, argc, argv);
  exec(&observeRec);

  exit(0);
}
