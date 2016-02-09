/*
 * Usage : average [input] [output]
 */



#include "Image.h"
#include "Image/Funcs.h"
#include <math.h>
#include <stdio.h>



void main(argc,argv)
     int argc;
     char **argv;
{
  image src,del;
  char *input = "-";
  char *output = "-";

  if ((argc == 2) && (strcmp(argv[1],"help") == 0))
    {
      fprintf(stderr,"Usage : average [input] [output]\n");
      exit(0);
    }

  if (argc > 1) input  = argv[1];
  if (argc > 2) output = argv[2];

  src = Image.createFromFilename(input,input);
  del = Image.create("DEL");

  if (src == 0)
    {
      fprintf(stderr,"average:can't open file %s\n",input);
      exit(-1);
    }

  image__average(del,src);
  Image.save(del,output,"average");
}
