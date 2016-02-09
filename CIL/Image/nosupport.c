/*
 * image-funcs.c
 */



#include "Funcs.h"
#include <stdio.h>



void image__function_nosupport(func_name,src)
     char *func_name;
     image src;
{
  fprintf(stderr,"error = {\n");
  fprintf(stderr,"    library    = image-funcs-lib\n");
  fprintf(stderr,"    message    = function is not supported\n");
  fprintf(stderr,"    function   = %s\n",func_name);
  fprintf(stderr,"    image-name = %s\n",src->name);
  //  fprintf(stderr,"    image-type = %s\n",typename(src->type));
  fprintf(stderr,"    image-type = ");ftypeprintf(stderr,src->type);fprintf(stderr,"\n");
  fprintf(stderr,"}\n");
}
