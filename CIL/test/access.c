#include <unistd.h>
#include <stdio.h>



main(argc,argv)
     int argc;
     char *argv[];
{
  int i;

  if (argc == 1)
    {
      fprintf(stderr,"Usage : access <filename>\n");
      exit(0);
    }

  for (i = 1; i < argc; i++)
    {
      fprintf(stderr,"access check : %s\n",argv[i]);
      if (access(argv[i],R_OK) == 0)
	fprintf(stderr,"    : read OK\n");
      if (access(argv[i],W_OK) == 0)
	fprintf(stderr,"    : write OK\n");
      if (access(argv[i],X_OK) == 0)
	fprintf(stderr,"    : exec or search OK\n");
      if (access(argv[i],F_OK) == 0)
	fprintf(stderr,"    : exist or search OK\n");
    }
}
