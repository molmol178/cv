/*
 * exec.c
 */



#include "Edit.h"



void Edit__exec(Edit e)
{
  XwWorld.mainLoop(e->world);
}
