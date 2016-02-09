/*
 * fontname : fontlib.c
 * author   : Takahiro Sugiyama
 * date     : 1993/09/17
 */



#include "fontlib.h"



extern long FontNum;
extern struct fontRec FontTable[];



struct fontRec *fontsearch(name)
     char *name;
{
  int i;

  for (i = 0; i < FontNum; i++)
    if (strcmp(name,FontTable[i].name) == 0)
      return &FontTable[i];

  return 0;
}
