/*
 * filename : main.c
 * author   : Takahiro Sugiyama
 * date     : Saturday, February 8 1997
 * describe : $B%a%$%s%k!<%A%s(B
 */


#include "observe.h"


/*------------------------------------------------------------------------*
 * $B%a%$%s(B
 *------------------------------------------------------------------------*/
void main(int argc, char *argv[])
{
  ObserveRec observeRec;

  initialize(&observeRec, argc, argv);
  exec(&observeRec);

  exit(0);
}
