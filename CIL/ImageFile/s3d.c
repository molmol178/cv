/*
 * tiff.c
 */



#include "FImage.h"
#include <stdio.h>



static long S3D__loadFile P ((image self,FILE *fp));
static long S3D__saveFile P ((image self,FILE *fp));



static ulong S3D__getLong(fp)
     FILE *fp;
{
  ulong c1,c2,c3,c4;
  c1 = (uchar)getc(fp);
  c2 = (uchar)getc(fp);
  c3 = (uchar)getc(fp);
  c4 = (uchar)getc(fp);
  return (c4<<24) | (c3<<16) | (c2<<8) | (c1);
}



static ulong S3D__getWord(fp)
     FILE *fp;
{
  ulong c1,c2;
  c1 = (uchar)getc(fp);
  c2 = (uchar)getc(fp);
  return (c2<<8) | (c1);
}



static ulong S3D__getBit(fp)
     FILE *fp;
{
  static long nbyte = 0;
  static ulong byte;
  ulong bit;

  if (nbyte == 0) {nbyte = 8;byte = (ulong)getc(fp);}
  bit = (byte & 0x80) >> 7;
  byte <<= 1; nbyte--;

  return bit;
}



static long S3D__getLength(fp)
     FILE *fp;
{
  long i,len = 0;

  if (S3D__getBit(fp))
    {
      len = S3D__getBit(fp);
    }
  else
    {
      long zeronum = 1;

      while (S3D__getBit(fp) == 0) zeronum ++;
      len = 1;
      for (i = 0; i < zeronum; i++)
	len = (len << 1) + S3D__getBit(fp);
    }
  return len + 1;
}



static long S3D__loadFile(self,fp)
     image self;
     FILE *fp;
{
  long i;
  long sc,xs,ys,xe,ye,dn,bn,data_type,compbyte;
  long mmdot,xbyte,xsize,ysize,compress,gray;

  sc = S3D__getLong(fp);
  xs = S3D__getWord(fp);
  ys = S3D__getWord(fp);
  xe = S3D__getWord(fp);
  ye = S3D__getWord(fp);
  dn = S3D__getWord(fp);
  bn = S3D__getLong(fp);
  data_type = S3D__getWord(fp);
  compbyte  = S3D__getLong(fp);

  for (i = 0x0018; i <= 0x7f; i++)
    getc(fp);

  mmdot    = dn / 25;
  xbyte    = ((xe - xs) * 8 + 15) / 16 * 2;
  ysize    = ((ye - ys) * 8 + 15) / 16 * 2;
  compress = data_type & 1;
  gray     = data_type & 0x0100;

  fprintf(stderr,"S3D.load\n");
  fprintf(stderr,"sec = %d\n",sc);
  fprintf(stderr,"xs = %d\n",xs);
  fprintf(stderr,"ys = %d\n",ys);
  fprintf(stderr,"xe = %d\n",xe);
  fprintf(stderr,"ye = %d\n",ye);
  fprintf(stderr,"dn = %d\n",dn);
  fprintf(stderr,"sum = %d\n",bn);
  fprintf(stderr,"data type = 0x%04x\n",data_type);
  fprintf(stderr,"compyte = %d\n",compbyte);
  fprintf(stderr,"\n");

  if (gray)
    {/* 16 */
      xsize = xbyte * 2;
      Image.make(self,Bit4,xsize,ysize);
    }
  else
    {/* 2 */
      xsize = xbyte * 8;
      Image.make(self,Bit1,xsize,ysize);
    }

  fprintf(stderr,"mmdot = %d\n",mmdot);
  fprintf(stderr,"xbyte = %d\n",xbyte);
  fprintf(stderr,"xsize = %d\n",xsize);
  fprintf(stderr,"ysize = %d\n",ysize);
  fprintf(stderr,"compress = %d\n",compress);
  fprintf(stderr,"gray = %d\n",gray);

  if (compress)
    {/* compress */
      if (gray)
	{
	  long num,len,npix,pix;
	  register uchar *p = (uchar *)Image.raster(self);
	  uchar col = S3D__getBit(fp);

	  npix = 0;
	  pix = 0;
	  for (num = 0; num < Image.area(self);)
	    {
	      len = S3D__getLength(fp);
	      for (i = 0; i < len; i++) {
		if (npix == 4) {
		  *p++ = pix;
		  pix = 0;
		  npix = 0;
		  num++;
		}
		pix = (pix << 1) + col;
		npix++;
	      }
	      col = 1 - col;
	    }
	}
      else
	{
	  long num,len;
	  register uchar *p = (uchar *)Image.raster(self);
	  uchar col = S3D__getBit(fp);
	  long n;

	  n = Image.area(self);
	  for (num = 0; num < n;)
	    {
	      len = S3D__getLength(fp);
	      for (i = 0; (i < n) && (i < len); i++,num++,p++)
		*p = col;
	      if ( i < len )
		fprintf(stderr,"warning:S3D__loadFile:check sum error? length=%d\n",len);
	      col = 1 - col;
	    }
	}
    }
  else
    {/* uncompress */
      if (gray)
	{
	  long x,y,pack;
	  uchar *buf = (uchar *)calloc(xbyte,sizeof(uchar));
	  uchar **data = (uchar **)Image.data(self);

	  for (y = 0; y < ysize; y++)
	    {
	      if (fread(buf,1,xbyte,fp) != xbyte)
		fprintf(stderr,"warning:can't read data.\n");
	      for (x = 0; x < xsize; x++)
		{
		  pack = buf[x/2];
		  if ((x % 2) == 0)
		    data[y][x] = (pack >> 4) & 0x0f;
		  else
		    data[y][x] = pack & 0x0f;
		}
	    }

	  free(buf);
	}
      else
	{
	  long x,y,wherebit;
	  uchar *buf = (uchar *)calloc(xbyte,sizeof(uchar));
	  uchar **data = (uchar **)Image.data(self);

	  for (y = 0; y < ysize; y++)
	    {
	      if (fread(buf,1,xbyte,fp) != xbyte)
		fprintf(stderr,"warning:can't read data.\n");
	      for (x = 0; x < xsize; x++)
		{
		  if ((x % 8) == 0) wherebit = 0x80;
		  if (wherebit & buf[x/8])
		    data[y][x] = 1;
		  else
		    data[y][x] = 0;
		  wherebit >>= 1;
		}
	    }

	  free(buf);
	}
    }
  return 1;
}



static long S3D__saveFile(self,fp)
     image self;
     FILE *fp;
{
  fprintf(stderr,"S3Dsave:not yet supported\n");
  return 0;
}



long FImageLoad_S3D(self,filename)
     image self;
     char *filename;
{
  FILE *fp;
  long err = 0;

  if (self == 0) fprintf(stderr,"error:S3D.load:image is wrong.\n");

  if (fp = fopen(filename,"r"))
    {
      err = S3D__loadFile(self,fp);
      fclose(fp);
    }

  return err;
}



long FImageSave_S3D(self,filename,comment)
     image self;
     char *filename;
     char *comment;
{
  FILE *fp;
  long err = 0;

  if (self == 0) fprintf(stderr,"error:S3D.save:image is wrong.\n");

  if (self->type == Bit1 || self->type == Bit4)
    if (fp = fopen(filename,"w"))
      {
	err = S3D__saveFile(self,fp);
	fflush(fp);
	fclose(fp);
      }

  return err;
}



long FImageMagic_S3D(fp)
     FILE *fp;
{
  long sc,xs,ys,xe,ye,dn,sum;
  long xbyte,ysize;

  sc = S3D__getLong(fp);
  xs = S3D__getWord(fp);
  ys = S3D__getWord(fp);
  xe = S3D__getWord(fp);
  ye = S3D__getWord(fp);
  dn = S3D__getWord(fp);
  sum = S3D__getLong(fp);

  xbyte    = ((xe - xs) * 8 + 15) / 16 * 2;
  ysize    = ((ye - ys) * 8 + 15) / 16 * 2;

  rewind(fp);

  if ((sum != 0) && ((xbyte * ysize) == sum)) return 1;

  return 0;
}
