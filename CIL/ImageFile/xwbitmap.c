/*
 * xwbitmap.c
 */



#include "FImage.h"
#include <stdio.h>



static void bitmap_get_name(name,orig)
     char *name;
     char *orig;
{
  long n,i;
  char filename[1024];

  strcpy(filename,orig);
  n = strlen(filename);
  if ((filename[n-2] == '.') && (filename[n-1] == 'c'))
    {filename[n-2] = '\0'; n -= 2;}

  for (i = n - 1; i > 0; i--)
    if (filename[i] == '/') break;

  if (filename[i] == '/') filename[i++] = '\0';

  strcpy(name,filename + i);
}



long FImageLoad_XwBitmap(self,filename)
     image self;
     char *filename;
{
  FILE *fp;
  char buf[1024];
  long i,j,n;
  char *head[512];
  long byte,xbyte,xsize,ysize;
  long x,y;
  uchar *bitmap_data;
  uchar *raster;
  static ulong bitmask[8] = {1<<7,1<<6,1<<5,1<<4,1<<3,1<<2,1<<1,1};

  if (self == 0)
    {
      fprintf(stderr,"error:XwBitmap.load:image is wrong.\n");
      return 0;
    }

  if (strequal(filename,"-") || strequal(filename,"stdin"))
    fp = stdin;
  else
    fp = fopen(filename,"r");
  if (fp == 0) return 0;

  fgets(buf,1024,fp);
  FImageSetHeader(buf);

  n = strsplit(buf,head,"/*/ \n");
  if (n < 4)
    {
      fprintf(stderr,"error:XwBitmap.load:It(%s) must be C program source !\n",
	      filename);
      return 0;
    }
  if (strcmp("XwBitmap",head[0]) != 0) return 0;
  xsize = atol(head[2]);
  ysize = atol(head[3]);
  if (n > 4) FImageSetComment(head[4]);

  Image.make(self,Bit1,xsize,ysize);
  xbyte = (xsize + 7) / 8;
  byte = xbyte * ysize;
  while (fgets(buf,1024,fp))
    {
      n = strsplit(buf,head,",\n ");
      if (n == 0) continue;
      if (strcmp(head[0],"static") == 0) break;
    }

  j = 0;
  bitmap_data = (uchar *)calloc(byte,sizeof(uchar));
  while (fgets(buf,1024,fp))
    {
      n = strsplit(buf,head,",\n ");
      for (i = 0; i < n; i++)
	{
	  bitmap_data[j++] = atol(head[i]);
	  if (j >= byte) break;
	}
      if (j >= byte) break;
    }

  i = 0;
  raster = (uchar *)Image.raster(self);
  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      raster[i++] = bitmap_data[y * xbyte + x / 8] & bitmask[x % 8];

  free(bitmap_data);

  if (fp != stdin) fclose(fp);

  return 1;
}



long FImageSave_XwBitmap(self,filename,comment)
     image self;
     char *filename;
     char *comment;
{
  long xsize,ysize,xbyte,x,y,i,byte;
  register uchar *buf;
  register bit1 **data;
  static ulong mask[8] = {1<<7,1<<6,1<<5,1<<4,1<<3,1<<2,1<<1,1};
  char name[100];
//  static void bitmap_get_name (char *name,char *filename);
  FILE *fp;

  if (self == 0)
    {
      fprintf(stderr,"error:XwBitmap.save:image is wrong.\n");
      return 0;
    }

  if (self->type != Bit1)
    {
      fprintf(stderr,"error:XwBitmap.save:image type is wrong(only Bit1).\n");
      return 0;
    }

  if (strequal(filename,"-") || strequal(filename,"stdout"))
    {
      fp = stdout;
      if (comment && comment[0]) bitmap_get_name(name,comment);
      else strcpy(name,"foo");
    }
  else
    {
      fp = fopen(filename,"w");
      bitmap_get_name(name,filename);
      if (name[0] == '\0') strcpy(name,"foo");
    }

  if (fp == 0) return 0;

  xsize = Image.xsize(self);
  ysize = Image.ysize(self);
  data  = (bit1 **)Image.data(self);
  xbyte = (xsize + 7) / 8;
  byte  = xbyte * ysize;
  buf   = (uchar *)malloc(byte);

  i = -1;
  for (y = 0; y < ysize; y++)
    {
      long offset;

      for (x = 0; x < xsize; x++)
	{
	  offset = x % 8;
	  if (offset == 0) {i++;buf[i] = 0;}
	  if (data[y][x]) buf[i] |= mask[offset];
	}
    }

  fprintf(fp,"/*XwBitmap %s %d %d \"%s\"*/\n",name,xsize,ysize,comment);
  fprintf(fp,"extern struct xwBitmapRec %sBM;\n",name);
  fprintf(fp,"static unsigned char %s_data[] = {",name);
  for (i = 0; i < byte; i++)
    {
      if ((i % 18) == 0) fprintf(fp,"\n  ");
      fprintf(fp,"%3d",buf[i]);
      if (i != (byte-1)) fprintf(fp,",");
    }
  fprintf(fp,"\n};\n");
  fprintf(fp,"struct xwBitmapRec %sBM = {\"%s\", %d, %d, %s_data};\n",
	  name,name,xsize,ysize,name);
  free(buf);

  if (fp != stdin) fclose(fp);

  return 1;
}



long FImageMagic_XwBitmap(fp)
     FILE *fp;
{
  char magic_char = getc(fp);

  ungetc(magic_char,fp);

  return (magic_char == '/');
}



