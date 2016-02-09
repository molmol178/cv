/*
 * Usage : gaussian [input] [output]
 */



#include "Image.h"
#include "Image/Funcs.h"
#include <math.h>
#include <stdio.h>



void main(argc,argv)
     int argc;
     char **argv;
{
  image src,dst;
  char *input = "-";
  char *output = "-";
  double sigma = 1.0;

  if ((argc == 2) && (strcmp(argv[1],"help") == 0))
    {
      fprintf(stderr,"Usage : gaussian [sigma] [input] [output]\n");
      exit(0);
    }

  if (argc > 1) if (strcmp(argv[1],"-") != 0) sigma = atof(argv[1]);
  if (argc > 2) input  = argv[2];
  if (argc > 3) output = argv[3];

  if (sigma < 0)
    {
      fprintf(stderr,"warning: sigma: bad value ... sigma = %f\n",sigma);
      sigma = fabs(sigma);
      fprintf(stderr,"                          now sigma = %f\n",sigma);
    }

  src = Image.createFromFilename(input,input);
  dst = Image.create("DST");

  if (src == 0)
    {
      fprintf(stderr,"gaussian:can't open file %s\n",input);
      exit(-1);
    }

  image__gaussian(dst,src,sigma);
  Image.save(dst,output,"gaussian");
}
