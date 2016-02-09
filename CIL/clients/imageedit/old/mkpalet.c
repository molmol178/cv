#include <stdio.h>


#define NG 8
#define NR 8
#define NB 4

main()
{
  int r,g,b;
  int pr,pg,pb;

  for (g = 0; g < NG; g++)
    for (b = 0; b < NB; b++)
      for (r = 0; r < NR; r++)
	{
	  pr = 255 * g / (NG - 1);
	  pg = 255 * r / (NR - 1);
	  pb = 255 * b / (NB - 1);
	  printf("%3d %3d %3d\n",pr,pg,pb);
	}
}
