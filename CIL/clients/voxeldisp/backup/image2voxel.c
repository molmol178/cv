/*
 *
 */

#include "Image.h"
#include "Voxel.h"
#include "misc/typelib.h"
#include <stdio.h>



void voxel_copy_image P ((voxel dest,image src,long z));

void bit1_to_voxel   P ((voxel dest,image src));
void uchar_to_voxel  P ((voxel dest,image src));
void uchar3_to_voxel  P ((voxel dest,image src));



void main(argc,argv)
     int argc;
     char *argv[];
{
  image src;
  voxel dest;
  char *outname = "out.voxel";

  if (argc < 2)
    {
      fprintf(stderr,"Usage : image2voxel <image> [voxel]\n");
      exit(-1);
    }
  if (argc > 2) outname = argv[2];

  src = Image.createFromFilename("src",argv[1]);
  if (src == 0)
    {
      fprintf(stderr,"can't open file\n");
      exit(-1);
    }
  dest = Voxel.createMake("dest",Bit1,Image.xsize(src),Image.ysize(src),Image.xsize(src));

  fprintf(stderr,"Image2Voxel = {\n");
  fprintf(stderr,"    image  = %s\n",argv[1]);
  fprintf(stderr,"    output = %s\n",outname);
  fprintf(stderr,"    type   = %s\n",typename(Image.type(src)));
  fprintf(stderr,"    xsize  = %d\n",Image.xsize(src));
  fprintf(stderr,"    ysize  = %d\n",Image.ysize(src));
  fprintf(stderr,"}\n");

  if (Image.type(src) == Bit1)
    bit1_to_voxel(dest,src);
  else
  if (Image.type(src) == UChar)
    uchar_to_voxel(dest,src);
  else
  if (Image.type(src) == UChar3)
    uchar3_to_voxel(dest,src);
  else
    exit(-1);

  {
    char comment[512];
    sprintf(comment,"image2voxel %s[%s.type]",
	    argv[1],typename(Image.type(src)));
    Voxel.save(dest,outname,comment);
  }

  Image.destroy(src);
  Voxel.destroy(dest);
}



void uchar3_to_voxel(dest,src)
     voxel dest;
     image src;
{
  long z,n;
  register long i;
  register uchar3 *data;

  n = Image.area(src);
  data = Image.raster(src);
  for (i = 0 ; i < n; i++,data++)
    data->at[0] = (data->at[0]+data->at[1]+data->at[2]) / 3;

  for (z = 0; z < 256; z++)
    {
      register bit1  *data3;

      data = Image.raster(src);
      data3 = Voxel.data2D(dest,z)[0];
      for (i = 0 ; i < n; i++,data++,data3++)
	if (z >= data->at[0]) *data3 = 1;
    }
}



void uchar_to_voxel(dest,src)
     voxel dest;
     image src;
{
  long z,n,zsize;

  n = Image.area(src);
  zsize = Voxel.zsize(dest);
  for (z = 0; z < zsize; z++)
    {
      register long i;
      register uchar *data2;
      register bit1  *data3;

      data2 = Image.raster(src);
      data3 = Voxel.data2D(dest,z)[0];
      for (i = 0 ; i < n; i++,data2++,data3++)
	if (z <= (zsize * (*data2) / 255)) *data3 = 1;
    }
}



void bit1_to_voxel(dest,src)
     voxel dest;
     image src;
{
  int i,xsize;

  xsize = Image.xsize(src);
  for (i = xsize/4 ; i < 3*xsize/4; i++)
    voxel_copy_image(dest,src,i);
}



void voxel_copy_image(dest,src,z)
     voxel dest;
     image src;
     long z;
{
  register bit1 **dest_data;
  register bit1 **src_data;
  register int x,y;
  long xsize,ysize;

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  dest_data = Voxel.data2D(dest,z);
  src_data = Image.data(src);
  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      dest_data[y][x] = src_data[y][x];
}

