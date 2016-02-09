/*
 * local.c
 */



#include "Edit.h"



void long__swap(long a,long b)
{
  long tmp = a; a = b; b = tmp;
}



void string__swap(char *a,char *b)
{
  char tmp[512];
  strcpy(tmp,a);
  strcpy(a,b);
  strcpy(b,tmp);
}



long string__to_metatype(char *name)
{
  if (strcmp(name,"bit1") == 0 || strcmp(name,"Bit1") == 0)
    return Bit1;

  if (strcmp(name,"uchar") == 0 || strcmp(name,"UChar") == 0)
    return UChar;

  if (strcmp(name,"uchar3") == 0 || strcmp(name,"UChar3") == 0)
    return UChar3;

  return Bit1;
}
