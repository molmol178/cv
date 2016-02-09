/*
 * filename : cilcp.c
 * author   : Takahiro Sugiyama
 * date     : 1993/10/01
 */



#include "Image.h"
#include <stdio.h>



int main(argc,argv)
     int argc;
     char **argv;
{
  image src;
  image dest;
  char *output = "-";
  char *input = "-";

  if ((argc == 2) && (strcmp(argv[1],"-h") == 0))
    {
      fprintf(stderr,"Usage : cilcp [input] [output]\n");
      return 1;
    }

  if (argc > 1) input  = argv[1];
  if (argc > 2) output = argv[2];

  dest = Image.createFromFilename(input,input);
  if (dest == 0)
    {
      fprintf(stderr,"cilcp:can't get input (%s)\n",input);
      return -1;
    }

  Image.save(dest,output,input);
  Image.destroy(dest);

  return 0;
}
