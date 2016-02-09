/*
 * exit.c
 */



#include "Edit.h"



void Edit__exit(Edit e)
{
  XwWorld.destroy(e->world);
  if (optspecified("palet-save")) pen__palet_save(e);
}
