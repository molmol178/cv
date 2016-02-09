#include "ImageFile.h"



void main(argc,argv)
     int argc;
     char *argv[];
{
  image img;
  int i;

  if (argc < 2)
    {
      fprintf(stderr,"Usage : bin2xw <bit1image>\n");
      exit(-1);
    }

  img = Image.create("img");

  for (i = 1; i < argc; i++)
    if (ImageFile.loadPackedBit1(img,argv[i]))
      {
	printf("extern struct xwBitmapRec %sBM;\n",argv[i]);
      }
  printf("\n");

  for (i = 1; i < argc; i++)
    {
      if (ImageFile.loadPackedBit1(img,argv[i]) == 0)
	{
	  fprintf(stderr,"Bad Image (%s)!!\n",argv[i]);
	  exit(-1);
	}

      {
	bit1 **data;
	long xsize,ysize,xbyte,x,y;

	xsize = Image.xsize(img);
	xbyte = (xsize + 7) / 8;
	ysize = Image.ysize(img);
	data  = (bit1 **)Image.data(img);

	printf("static uchar %s_data[] = {\n",argv[i]);
	for (y = 0; y < ysize; y++)
	  {
	    if ((y % 2) == 0) printf("  ");
	    for (x = 0; x < xbyte; x++)
	      {
		printf("0x%02x",data[y][x]);
		if (x != (xbyte - 1)) printf(",");
	      }
	    if (y != (ysize - 1)) printf(",");
	    if ((y % 2) == 1) printf("\n");
	  }
	printf("};\n");

	printf("struct xwBitmapRec %sBM = {\"%s\", %d, %d, %s_data};\n",
	       argv[i],argv[i],xsize,ysize,argv[i]);

	if (i != (argc-1)) printf("\n");
      }
    }
}
