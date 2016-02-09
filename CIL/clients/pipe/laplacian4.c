/*
 * Usage : laplacian4 [input] [output]
 */



#include "Image.h"
#include "Image/Funcs.h"
#include <math.h>
#include <stdio.h>



void main(argc,argv)
     int argc;
     char **argv;
{
  image src,lap4;
  char *input = "-";
  char *output = "-";

  if ((argc == 2) && (strcmp(argv[1],"help") == 0))
    {
      fprintf(stderr,"Usage : laplacian4 [input] [output]\n");
      exit(0);
    }

  if (argc > 1) input  = argv[1];
  if (argc > 2) output = argv[2];

  src = Image.createFromFilename(input,input);
  lap4 = Image.create("LAP4");

  if (src == 0)
    {
      fprintf(stderr,"laplacian4:can't open file %s\n",input);
      exit(-1);
    }

  image__laplacian4(lap4,src,0);
  Image.save(lap4,output,"laplacian4");
}
