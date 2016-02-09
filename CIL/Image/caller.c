/*
 * caller.c
 */



#include "Funcs.h"
#include <stdio.h>



void *image__caller(func_name,src,table_num,table_list)
     char *func_name;
     image src;
     long table_num;
     caller_table *table_list;
{
  int i;

  for (i = 0; i < table_num; i++)
    if (src->type == table_list[i].type)
      break;
  if (i == table_num || table_list[i].caller == 0)
    {
      image__function_nosupport(func_name,src);
      return 0;
    }

  return table_list[i].caller;
}
