/*
 * image-funcs.c
 */



#include "Funcs.h"
#include <stdio.h>



void image__type_check(func_name,src,type)
     char *func_name;
     image src;
     long type;
{
  char *type_name = "none";

  if (src->type != type)
    {
      fprintf(stderr,"error = {\n");
      fprintf(stderr,"    library    = image-funcs-lib\n");
      //fprintf(stderr,"    message    = Type is not %s.type\n",typename(src->type));
      fprintf(stderr,"    message    = Type is not ");ftypeprintf(stderr,src->type);fprintf(stderr,".type\n");
      fprintf(stderr,"    function   = %s\n",func_name);
      fprintf(stderr,"    image-name = %s\n",src->name);
      fprintf(stderr,"    image-type = %s\n",typename(type));
      fprintf(stderr,"}\n");
      exit(-1);
    }
}
