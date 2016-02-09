/*
 * j4.c
 */



#include "FImage.h"
#include <stdio.h>
#include "../misc/strlib.h"
#include "../misc/filelib.h"



static long J4__load_single(self,fp)
     image self;
     FILE *fp;
{
  char head[80];
  long xsize,ysize,ebit,pbit;
  char c[8];
  char comment[80];
  static ulong mask[8] = {1<<7,1<<6,1<<5,1<<4,1<<3,1<<2,1<<1,1};

  if (fread(head,1,80,fp) != 80) return 0;
  FImageSetHeader(head);
  strncpy(comment,head + 8,60);comment[60] = 0;
  FImageSetComment(comment);

  memcpy(c,head+68,4);c[4] = 0;
  xsize = atol(c);

  memcpy(c,head+72,4);c[4] = 0;
  ysize = atol(c);

  memcpy(c,head+76,2);c[2] = 0;
  pbit = atol(c);

  memcpy(c,head+78,2);c[2] = 0;
  ebit = atol(c);

  if (xsize < 0 || ysize < 0 || ebit <= 0 || pbit <= 0 ||
      ebit > pbit || pbit > 16)
    return 0;

  switch(pbit)
    {
    case 1: 
      {
	int xbyte;
	uchar *buf;

	xbyte = (xsize + 7) / 8;
	buf   = (uchar *)calloc(ysize,xbyte);

	Image.make(self,Bit1,xsize,ysize);

	if (fp_read(fp,(char *)buf,ysize,xbyte) != ysize)
	  {/* extend format */
	    uchar **data = (uchar **)Image.data(self);
	    register int x,y,i = 0;
	    for (y = 0; y < ysize; y++)
	      for (x = 0; x < xsize; x++)
		{
		  if (buf[i / 8] & mask[i % 8]) data[y][x] = 1;
		  i++;
		}
	  }
	else
	  {
	    /* normal format */
	    uchar **data = (uchar **)Image.data(self);
	    register int x,y;
	    for (y = 0; y < ysize; y++)
	      for (x = 0; x < xsize; x++)
		if (buf[y * xbyte + (x / 8)] & mask[x % 8]) data[y][x] = 1;
	  }
	free((char *)buf);
      }
      break;
    case 8:
      {
	if (ebit == 1)
	  Image.make(self,Bit1,xsize,ysize);
	else
	if (ebit == 4)
	  Image.make(self,Bit4,xsize,ysize);
	else
	  Image.make(self,UChar,xsize,ysize);
	if (fp_readbyte(fp,Image.raster(self),Image.byte(self)) != Image.byte(self))
	  fprintf(stderr,"warning:[%s]'s size is wrong.\n",self->name);
      }
      break;
    case 16:
      {
	Image.make(self,UShort,xsize,ysize);
	if (fp_readbyte(fp,Image.raster(self),Image.byte(self)) != Image.byte(self))
	  fprintf(stderr,"Warning:[%s]'s size is wrong.\n",self->name);
	if (ebit <= 8)
	  {
	    register int i;
	    image tmp;
	    register uchar  *dp;
	    register ushort *sp;
	    long num;

	    tmp = Image.create("LoadJ4");
	    if (ebit == 4)
	      Image.make(tmp,Bit4,xsize,ysize);
	    else
	      Image.make(tmp,UChar,xsize,ysize);

	    dp = (uchar *)Image.raster(tmp);
	    sp = (ushort *)Image.raster(self);
	    num = Image.area(self);
	    for (i = 0; i < num; i++)
	      *dp++ = *sp++;
	    Image.copy(self,tmp);
	    Image.destroy(tmp);
	  }
      }
      break;
    default:
      return 0;
      break;
    }
  return 1;
}



static long J4__load_triple(self,rfile,gfile,bfile)
     image self;
     FILE *rfile;
     FILE *gfile;
     FILE *bfile;
{
  char rhead[81],ghead[81],bhead[81];
  long xsize,ysize,ebit,pbit;
  char c[8],comment[80];

  if ((fread(rhead,1,80,rfile) != 80)||
      (fread(ghead,1,80,gfile) != 80)||
      (fread(bhead,1,80,bfile) != 80))
    return 0;
  FImageSetHeader(rhead);
  strncpy(comment,rhead + 8,60);comment[60] = 0;
  FImageSetComment(comment);

  memcpy(c,rhead+68,4);c[4] = 0;
  xsize = atol(c);

  memcpy(c,rhead+72,4);c[4] = 0;
  ysize = atol(c);

  memcpy(c,rhead+76,2);c[2] = 0;
  pbit = atol(c);

  memcpy(c,rhead+78,2);c[2] = 0;
  ebit = atol(c);

  if (xsize < 0 || ysize < 0 || ebit <= 0 || pbit <= 0 ||
      ebit > pbit || pbit > 16)
    return 0;
  
  switch(pbit)
    {
    case 8:
      {
	register int x,y;
	uchar *rbuf, *gbuf, *bbuf;
	uchar3 **data;

	Image.make(self,UChar3,xsize,ysize);
	rbuf = (uchar *)malloc(xsize);
	gbuf = (uchar *)malloc(xsize);
	bbuf = (uchar *)malloc(xsize);
	data = (uchar3 **)Image.data(self);

	for (y = 0; y < ysize; y++)
	  {
	    if ((fread(rbuf,1,xsize,rfile) != xsize)||
		(fread(gbuf,1,xsize,gfile) != xsize)||
		(fread(bbuf,1,xsize,bfile) != xsize))
	      fprintf(stderr,"warning:[%s]'s size is wrong.\n",self->name);
	    for (x = 0; x < xsize; x++)
	      {
		data[y][x].at[0] = rbuf[x];
		data[y][x].at[1] = gbuf[x];
		data[y][x].at[2] = bbuf[x];
	      }
	  }
	free(rbuf);
	free(gbuf);
	free(bbuf);
      }
      break;
    case 16:
      {
	register int x,y;
	long size = xsize * sizeof(ushort);
	ushort *rbuf, *gbuf, *bbuf;

	rbuf = (ushort *)malloc(size);
	gbuf = (ushort *)malloc(size);
	bbuf = (ushort *)malloc(size);

	if (ebit > 8)
	  {
	    ushort3 pixel;
	    ushort3 **data;
	    Image.make(self,UShort3,xsize,ysize);
	    data = (ushort3 **)Image.data(self);

	    for (y = 0; y < ysize; y++)
	      {
		if ((fread(rbuf,1,size,rfile) != size)||
		    (fread(gbuf,1,size,gfile) != size)||
		    (fread(bbuf,1,size,bfile) != size))
		  fprintf(stderr,"warning:[%s]'s size is wrong.\n",self->name);

		for (x = 0; x < xsize; x++)
		  {
		    pixel.at[0] = rbuf[x];
		    pixel.at[1] = gbuf[x];
		    pixel.at[2] = bbuf[x];
		    data[y][x] = pixel;
		  }
	      }
	  }
	else
	  {
	    uchar3 pixel;
	    uchar3 **data;
	    Image.make(self,UChar3,xsize,ysize);
	    data = (uchar3 **)Image.data(self);
	    for (y = 0; y < ysize; y++)
	      {
		if ((fread(rbuf,1,size,rfile) != size)||
		    (fread(gbuf,1,size,gfile) != size)||
		    (fread(bbuf,1,size,bfile) != size))
		  fprintf(stderr,"warning:[%s]'s size is wrong.\n",self->name);

		for (x = 0; x < xsize; x++)
		  {
		    pixel.at[0] = rbuf[x];
		    pixel.at[1] = gbuf[x];
		    pixel.at[2] = bbuf[x];
		    data[y][x] = pixel;
		  }
	      }
	  }
	free(rbuf);
	free(gbuf);
	free(bbuf);
      }
      break;
    default:
      return 0;
      break;
    }
  return 1;
}



static long J4__save_single(self,fp,comment)
     image self;
     FILE *fp;
     char *comment;
{
  int pbit = 8,ebit = 8;
  char head[81];

  if ((self->type == Bit1) || (self->type == Bit1))
    {
      pbit = 1;
    }
  else
  if (typesize(self->type) == 1)
    {
      pbit = 8;
    }
  else
  if (typesize(self->type) == 2)
    {
      pbit = 16;
    }
  ebit = pbit;

  if (self->type == Bit4)
    {
      ebit = 4;
    }

  sprintf(head,
	  "HM                                                                  %4d%4d%2d%2d",
	  Image.xsize(self),Image.ysize(self),pbit,ebit);
  if (comment && *comment)
    {
      int i;
      for (i = 0; (i < 60) && (i < strlen(comment)); i++)
	head[i+8] = comment[i];
    }

  if (fwrite(head,1,80,fp) != 80) return 0;

  switch(pbit)
    {
    case 1:
      if ( self->type != PackedBit1 )
	{
	  static uchar mask[] = {1<<7,1<<6,1<<5,1<<4,1<<3,1<<2,1<<1,1<<0};

	  int y,x;
	  int xbyte = ( self->xsize + 7 ) / 8;
	  uchar *buf = (uchar *)calloc( xbyte, self->ysize );

	  for ( y = 0; y < self->ysize; y++ )
	    for ( x = 0; x < self->xsize; x++ )
	      if ( self->data[ y ][ x ] )
		buf[ y * xbyte + x / 8 ] |= mask[ x % 8 ];

	  if (fp_write(fp,(char *)buf,self->ysize,xbyte) != self->ysize)
	    {
	      free(buf);
	      return 0;
	    }
	  free(buf);
	}
      else
	{
	  int xbyte = ( self->xsize + 7 ) / 8;

	  if (fp_write(fp,self->data[0],self->ysize,xbyte) != self->ysize)
	    return 0;
	}

      break;

    case 8:
    case 16:
      if (fp_writebyte(fp,Image.raster(self),Image.byte(self)) !=  Image.byte(self))
	return 0;
      break;
    }
  return 1;
}



static long J4__save_triple(self,rfile,gfile,bfile,comment)
     image self;
     FILE *rfile;
     FILE *gfile;
     FILE *bfile;
     char *comment;
{
  char rhead[81],ghead[81],bhead[81];

  sprintf(rhead,
	  "HR                                                                  %4d%4d 8 8",
	  self->xsize,self->ysize);
  sprintf(ghead,
	  "HG                                                                  %4d%4d 8 8",
	  self->xsize,self->ysize);
  sprintf(bhead,
	  "HB                                                                  %4d%4d 8 8",
	  self->xsize,self->ysize);

  if (comment && *comment)
    {
      int i;
      for (i = 0; (i < 60) && (i < strlen(comment)); i++)
	{
	  rhead[i+8] = comment[i];
	  ghead[i+8] = comment[i];
	  bhead[i+8] = comment[i];
	}
    }

  if ((fwrite(rhead,1,80,rfile) != 80)||
      (fwrite(ghead,1,80,gfile) != 80)||
      (fwrite(bhead,1,80,bfile) != 80))
    return 0;

  {
    int x,y;
    uchar *r,*g,*b;
    register uchar3 *p = (uchar3 *)Image.raster(self);

    r = (uchar *)malloc(self->xsize);
    g = (uchar *)malloc(self->xsize);
    b = (uchar *)malloc(self->xsize);

    for (y = 0; y < self->ysize; y++)
      {
	for (x = 0; x < self->xsize; x++)
	  {
	    r[x] = p->at[0];
	    g[x] = p->at[1];
	    b[x] = p->at[2];
	    p++;
	  }
	if ((fwrite(r,1,self->xsize,rfile) != self->xsize)||
	    (fwrite(g,1,self->xsize,gfile) != self->xsize)||
	    (fwrite(b,1,self->xsize,bfile) != self->xsize))
	  return 0;
      }
    free(r);
    free(g);
    free(b);
  }
  return 1;
}



long FImageLoad_J4(self,filename)
     image self;
     char *filename;
{
  FILE *fp,*rfile,*gfile,*bfile;
  long err = 0;
  char rfilename[256],gfilename[256],bfilename[256];

  if (fp = fp_open(filename,"r"))
    {
      err = J4__load_single(self,fp);
      fp_close(fp);
      return err;
    }

  sprintf(rfilename,"%s.r",filename);
  sprintf(gfilename,"%s.g",filename);
  sprintf(bfilename,"%s.b",filename);

  if ((rfile = fopen(rfilename,"r")) &&
      (gfile = fopen(gfilename,"r")) &&
      (bfile = fopen(bfilename,"r")))
    {
      err = J4__load_triple(self,rfile,gfile,bfile);
      fclose(rfile);
      fclose(gfile);
      fclose(bfile);
      return err;
    }

  sprintf(rfilename,"%sr",filename);
  sprintf(gfilename,"%sg",filename);
  sprintf(bfilename,"%sb",filename);

  if ((rfile = fopen(rfilename,"r")) &&
      (gfile = fopen(gfilename,"r")) &&
      (bfile = fopen(bfilename,"r")))
    {
      err = J4__load_triple(self,rfile,gfile,bfile);
      fclose(rfile);
      fclose(gfile);
      fclose(bfile);
      return err;
    }

  return 0;
}



long FImageSave_J4(self,filename,comment)
     image self;
     char *filename;
     char *comment;
{
  FILE *fp,*rfile,*gfile,*bfile;
  long err = 0;
  char rfilename[256],gfilename[256],bfilename[256];

  if (self->type == UChar ||
      self->type == UShort ||
      self->type == Short ||
      self->type == Char ||
      self->type == Bit1 ||
      self->type == PackedBit1 ||
      self->type == Bit4)
    if (fp = fp_open(filename,"w"))
      {
	err = J4__save_single(self,fp,comment);
	fp_flush(fp);
	fp_close(fp);
	return err;
      }

  sprintf(rfilename,"%s.r",filename);
  sprintf(gfilename,"%s.g",filename);
  sprintf(bfilename,"%s.b",filename);

  if (self->type == UChar3 || self->type == Char3 ||
      self->type == UShort3 || self->type == Short3)
    if ((rfile = fopen(rfilename,"w")) &&
	(gfile = fopen(gfilename,"w")) &&
	(bfile = fopen(bfilename,"w")))
      {
	err = J4__save_triple(self,rfile,gfile,bfile,comment);
	fflush(rfile);
	fflush(gfile);
	fflush(bfile);
	fclose(rfile);
	fclose(gfile);
	fclose(bfile);
	return err;
      }
    else
      {
	fprintf(stderr,"J4save:can't open file to write (%s)\n",filename);
	return 0;
      }

  fprintf(stderr,"J4save:Save image is wrong.\n"); 

  return 0;
}



long FImageMagic_J4(fp)
     FILE *fp;
{
  char magic_char = getc(fp);

  ungetc(magic_char,fp);

  return ((magic_char == 'H') || (magic_char == 'h') ||
	  (magic_char == 'V') || (magic_char == 'v'));
}
