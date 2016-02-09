/*
 * Usage : gaussian_gradient [input] [output]
 */



#include "Image.h"
#include "Image/Funcs.h"
#include <math.h>
#include <stdio.h>



void main(argc,argv)
     int argc;
     char **argv;
{
  image src,mag,grad_x,grad_y;
  char *input = "-";
  char *output = "-";
  double sigma = 1.0;

  if ((argc == 2) && (strcmp(argv[1],"help") == 0))
    {
      fprintf(stderr,"Usage : gaussian_gradient [input] [output] [sigma]\n");
      exit(0);
    }

  if (argc > 1) input  = argv[1];
  if (argc > 2) output = argv[2];
  if (argc > 3) if (strcmp(argv[3],"-") != 0) sigma = atof(argv[3]);

  if (sigma < 0)
    {
      fprintf(stderr,"warning: sigma: bad value ... sigma = %f\n",sigma);
      sigma = fabs(sigma);
      fprintf(stderr,"                          now sigma = %f\n",sigma);
    }

  src = Image.createFromFilename(input,input);
  mag = Image.create("MAG");
  grad_x = Image.create("GRAD_X");
  grad_y = Image.create("GRAD_Y");

  if (src == 0)
    {
      fprintf(stderr,"gaussian_gradient:can't open file %s\n",input);
      exit(-1);
    }

  image__gaussian_gradient(mag,grad_x,grad_y,src,sigma);
  Image.save(mag,output,"gaussian_gradient");
}
