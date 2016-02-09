/*
 * pnm.c
 */



#include "FImage.h"
#include <stdio.h>
#include "../misc/strlib.h"
#include "../misc/filelib.h"



static int  PNM__getChar(/* FILE *fp */);
static int  PNM__getInteger(/* FILE *fp */);
static long PNM__loadFile(/* image self,FILE *file */);
static long PNM__saveFile(/* image self,FILE *file */);



static int PNM__getChar(fp)
     FILE *fp;
{
  register int ch;

  ch = getc(fp);
  if (ch == '#')
    {
      do
	{
	  ch = getc(fp);
	}
      while (ch != '\n' && ch != EOF);
    }
  return ch;
}



static int PNM__getInteger(fp)
     FILE *fp;
{
  register int ch;
  register int val;
  
  do
    {
      ch = PNM__getChar(fp);
      if (ch == EOF)
	{
	  fprintf(stderr,"PNM.load:premature eof in pnm File.\n");
	  exit(1);
	}
    }
  while (ch == ' ' || ch == '\t' || ch == '\n');

  if (ch < '0' || ch > '9')
    {
      fprintf(stderr,"bogus data in pnm File.\n");
      exit(1);
    }
  
  val = ch - '0';
  while ((ch = PNM__getChar(fp)) >= '0' && ch <= '9')
    {
      val *= 10;
      val += ch - '0';
    }
  return val;
}



static long PNM__loadFile(self,fp)
     image self;
     FILE *fp;
{
  int c;
  uint x_size, y_size, mval;
  uint maxval;        /* maxval value from input file */
  uint half_maxval;	/* half of same -- for rounding */

  if (getc(fp) != 'P') return 0;

  c = getc(fp);

  x_size = PNM__getInteger(fp);
  y_size = PNM__getInteger(fp);

  if ( c != '4' )
    mval = PNM__getInteger(fp);
  else
    mval = 1;

  if (x_size <= 0 || y_size <= 0 || mval <= 0) return 0;

  maxval = mval;
  half_maxval = mval / 2;

  switch (c)
    {
    case '2':			/* it's a text-format pgm FILE **/
      Image.make(self,UChar,x_size,y_size);
      {
	register uchar *p;
	register long i;

	p = (uchar *)Image.raster(self);
	for (i = 0; i < Image.area(self); i++)
	  *p++ = (uchar)((PNM__getInteger(fp) * 255 + half_maxval) / maxval);
      }
      break;

    case '3':			/* it's a text-format pnm file */
      Image.make(self,UChar3,x_size,y_size);
      {
	register uchar3 *p;
	register long i;
  
	p = (uchar3 *)Image.raster(self);
	for (i = 0; i < Image.area(self); i++,p++)
	  {
	    p->at[0] = (uchar)((PNM__getInteger(fp)*255 + half_maxval)/maxval);
	    p->at[1] = (uchar)((PNM__getInteger(fp)*255 + half_maxval)/maxval);
	    p->at[2] = (uchar)((PNM__getInteger(fp)*255 + half_maxval)/maxval);
	  }
      }
      break;

    case '4':			/* it's a raw-format pbm file */
      Image.make(self,Bit1,x_size,y_size);
      {
	uchar *buf;
	uchar **data = (uchar **)Image.data(self);
	register int x,y;
	long xbyte = ( x_size + 7 ) /  8;
	static uchar mask[8] = {1<<7,1<<6,1<<5,1<<4,1<<3,1<<2,1<<1,1<<0};

	buf = ( uchar * )malloc( xbyte * y_size );

	if ( fp_read( fp, ( char * )buf, y_size, xbyte ) != y_size )
	  fprintf(stderr,"waning:PNM__loadFile:can't read completely\n");

	/* strippled(1)
	for (y = 0; y < y_size; y++) {
	  for (x = 0; x < 8; x++)
	    if (buf[y * xbyte + ((x_size-1) / 8)] & mask[x % 8]) 
	      data[y][x] = 0; else data[y][x] = 1;
	  for (x = 8; x < x_size; x++)
	    if (buf[y * xbyte + ((x-8) / 8)] & mask[x % 8])
	      data[y][x] = 1; else data[y][x] = 0;
	      }*/

	for (y = 0; y < y_size; y++) 
	  for (x = 0; x < x_size; x++)
	    if (buf[y * xbyte + (x / 8)] & mask[x % 8]) data[y][x] = 1;

	free( ( char * )buf );
      }
      break;

    case '5':			/* it's a raw-format pgm file */
      Image.make(self,UChar,x_size,y_size);
      if (mval == 255)
	{
	  long n;
	  n = fp_readbyte( fp, Image.raster(self), Image.byte(self) );
	  if ( n != Image.byte(self) )
	    fprintf(stderr,"waning:PNM__loadFile:can't read completely\n");
	}
      else
	{
	  register FILE *infile = fp;
	  register uchar *p;
	  register long i;
  
	  p = (uchar *)Image.raster(self);
	  for (i = 0; i < Image.area(self); i++)
	    *p++ = (uchar)
	      ((((uint)getc(infile))*255 + half_maxval)/maxval);
	}
      break;

    case '6':			/* it's a raw-format pnm file */
      Image.make(self,UChar3,x_size,y_size);
      if (mval == 255)
	{
	  long n;
	  n = fp_readbyte( fp, Image.raster(self), Image.byte(self) );
	  if ( n != Image.byte(self) )
	    fprintf(stderr,"waning:PNM__loadFile:can't read all\n");
	}
      else
	{
	  register FILE *infile = fp;
	  register uchar3 *p;
	  register long i;

	  p = (uchar3 *)Image.raster(self);
	  for (i = 0; i < Image.area(self); i++,p++)
	    {
	      p->at[0] = (uchar)
		((((uint)getc(infile))*255 + half_maxval)/maxval);
	      p->at[1] = (uchar)
		((((uint)getc(infile))*255 + half_maxval)/maxval);
	      p->at[2] = (uchar)
		((((uint)getc(infile))*255 + half_maxval)/maxval);
	    }
	}
      break;

    default:
      fprintf(stderr,"warning:PNM.load:I can't find format(%c).\n",c);
      return 0;
      break;
    }
  return 1;
}



static long PNM__saveFile_Bit1(self,fp)
     image self;
     FILE *fp;
{
  long xbyte;
  long x,y,i;
  uchar *buf;
  static uchar mask[8] = {1<<7,1<<6,1<<5,1<<4,1<<3,1<<2,1<<1,1<<0};

  fprintf(fp,"P4\n%ld %ld\n",self->xsize,self->ysize);

  xbyte = ( self->xsize + 7 ) / 8;
  buf = ( uchar * )calloc( xbyte, self->ysize );

  for ( y = 0; y < self->ysize; y++ )
    for ( x = 0; x < self->xsize; x++ )
      if ( self->data[ y ][ x ] )
	buf[ y * xbyte + x / 8 ] |= mask[ x % 8 ];

  if ( fp_write( fp, ( char * )buf, self->ysize, xbyte ) != self->ysize )
    fprintf(stderr,"warning:can't write all\n");

  free( ( char * )buf );
  return 1;
}



static long PNM__saveFile_PackedBit1(self,fp)
     image self;
     FILE *fp;
{
  long xbyte;

  fprintf(fp,"P4\n%ld %ld\n",self->xsize,self->ysize);

  xbyte = ( self->xsize + 7 ) / 8;
  if ( fp_write(fp,Image.raster(self),self->ysize,xbyte) != self->ysize )
    fprintf(stderr,"warning:can't write all\n");

  return 1;
}



static long PNM__saveFile(self,fp)
     image self;
     FILE *fp;
{
  if (self->type == Bit1) return PNM__saveFile_Bit1(self,fp);
  if (self->type == PackedBit1) return PNM__saveFile_PackedBit1(self,fp);

  if (self->type == UChar)
    fprintf(fp,"P5\n%ld %ld\n%d\n",self->xsize,self->ysize,255);
  else
  if (self->type == UChar3)
    fprintf(fp,"P6\n%ld %ld\n%d\n",self->xsize,self->ysize,255);

  if (fp_writebyte(fp,Image.raster(self),Image.byte(self)) != Image.byte(self))
    fprintf(stderr,"warning:can't write all\n");

  fp_flush(fp);

  return 1;
}



long FImageLoad_PNM(self, filename)
     image self;
     char *filename;
{
  FILE *fp;
  long err;

  if (self == 0) fprintf(stderr,"error:PNM.load:image is wrong.\n");

  fp = fp_open(filename,"r");

  err = PNM__loadFile(self,fp);

  fp_close(fp);

  return err;
}



long FImageSave_PNM(self, filename, comment)
     image self;
     char *filename;
     char *comment;
{
  FILE *fp;
  long err;

  if (self == 0)
    {
      fprintf(stderr,"error:PNM.save:image is wrong.\n");
      return 0;
    }

  if ((self->type != UChar) && (self->type != UChar3) &&
      (self->type != Bit1) &&  ( self->type != PackedBit1))
    {
      fprintf(stderr,"error:PNM.save:Image type is wrong.\n");
      return 0;
    }

  fp = fp_open(filename,"w");
  if (fp == 0)
    {
      fprintf(stderr,"can't open file to write (%s)\n",filename);
      return 0;
    }

  err = PNM__saveFile(self,fp);
  fp_flush(fp);
  fp_close(fp);

  return err;
}



long FImageMagic_PNM(fp)
     FILE *fp;
{
  char magic_char = getc(fp);
  ungetc(magic_char,fp);
  return (magic_char == 'P');
}



long FImageLoadPipe_PNM(self,pipename)
     image self;
     char *pipename;
{
  FILE *fp;
  long err = 0;

  if (self == 0) fprintf(stderr,"error:PNM.loadPipe:image is wrong.\n");

  if (fp = popen(pipename,"r"))
    {
      err = PNM__loadFile(self,fp);
      pclose(fp);
    }

  return err;
}



long FImageSavePipe_PNM(self,pipename)
     image self;
     char *pipename;
{
  FILE *fp;
  long err = 0;

  if (self == 0) fprintf(stderr,"error:PNM.savePipe:image is wrong.\n");

  if (self->type == UChar || self->type == UChar3 ||
      self->type == PackedBit1 || self->type == Bit1)
    if (fp = popen(pipename,"w"))
      {
	err = PNM__saveFile(self,fp);
	fflush(fp);
	pclose(fp);
      }

  return err;
}
