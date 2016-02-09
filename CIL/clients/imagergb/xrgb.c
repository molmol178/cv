/*
 * xrgb
 */



#include "XRGB.h"
#include <stdio.h>


void main(argc,argv)
     int argc;
     char *argv[];
{
  XRGB rgb;

  if (argc < 2) {
    fprintf(stderr,"Usage : xrgb <filename>\n");
    exit(-1);
  }

  rgb.fname = argv[1];
  xrgb_open(&rgb);
  xrgb_disp(&rgb);
  xrgb_exec(&rgb);
  xrgb_close(&rgb);
}
