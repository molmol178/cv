/*
 * dib.c
 */



#include "FImage.h"
#include "../misc/filelib.h"
#include <stdio.h>



static long getdword
  _P1 ((FILE *, fp))
{
  long hh,hl,lh,ll,result;

  ll = fgetc(fp);
  lh = fgetc(fp);
  hl = fgetc(fp);
  hh = fgetc(fp);
  result = ll + (lh << 8) + (hl << 16) + (hh << 24);

  return(result);
}



static long getword
  _P1 ((FILE *, fp))
{
  long h,l,result;

  l = fgetc(fp);
  h = fgetc(fp);
  result = l + (h << 8);

  return(result);
}



long FImageLoad_DIB(self,filename)
     image self;
     char *filename;
{
  register uchar3 **data;
  long i,x,y,xsize,ysize,byte,offset,bitnum,palnum,onedata;
  FILE *fp;
  struct RGB {
    uchar B,G,R,dummy;
  } *pallete;

  fp = fp_open( filename, "r" );
  if ( fp == 0 )
    {
      fprintf( stderr, "can't open file to read (%s)\n", filename );
      return 0;
    }

  if ((fgetc(fp) != 'B') || (fgetc(fp) != 'M'))
    {
      fprintf(stderr,"error:%s is not DIB file\n",filename);
      return 0;
    }

  getdword(fp); getword(fp);  getword(fp);
  offset = getdword(fp);
  getdword(fp);
  xsize = getdword(fp);
  ysize = getdword(fp);
  getword(fp);
  bitnum = getword(fp);
  if (getdword(fp) != 0)
    {
      fprintf(stderr,"error:This is packed file.\n");
      return 0;
    }
  getdword(fp); getdword(fp);  getdword(fp);
  palnum = getdword(fp);
  getdword(fp);

  if (palnum == 0)
    if (bitnum != 24)
      palnum = 1 << bitnum;
    else
      palnum = 0;

  pallete = ( struct RGB * )malloc( ( palnum + 1 ) * sizeof( struct RGB ) );
  fread( pallete, sizeof( struct RGB ), palnum, fp );

  fprintf(stderr,"%s : xsize = %d   ysize = %d   %d colors\n",
	  filename,xsize,ysize,palnum);
  fprintf(stderr,"paletenum = %d, bitnum = %d\n", palnum, bitnum);

  Image.make( self, UChar3, xsize, ysize );
  data = (uchar3 **)Image.data( self );

  fseek(fp, offset, SEEK_SET);

  switch (bitnum)
    {
    case 1:
      i = 0;
      for ( y = ysize - 1; y >= 0; y-- )
	for ( x = 0; x < xsize; x++ )
	  {
	    if (( x % 8 ) == 0 ) byte = fgetc( fp );
	    onedata = (( byte >> 7 ) & 1 ); byte <<= 1;
	    data[y][x].at[0] = pallete[onedata].R;
	    data[y][x].at[1] = pallete[onedata].G;
	    data[y][x].at[2] = pallete[onedata].B;
	  }
      break;

    case 4:
      for (y = ysize - 1; y >= 0; y--)
	for ( x = 0; x < xsize; x++)
	  {
	    if (( x % 2 ) == 0 ) byte = fgetc( fp );
	    onedata = (( byte >> 4 ) & 15 ); byte <<= 4;
	    data[y][x].at[0] = pallete[onedata].R;
	    data[y][x].at[1] = pallete[onedata].G;
	    data[y][x].at[2] = pallete[onedata].B;
	  }
      break;

    case 8:
      for (y = ysize - 1; y >= 0; y--)
	for ( x = 0; x < xsize; x++)
	  {
	    onedata = fgetc( fp );
	    data[y][x].at[0] = pallete[onedata].R;
	    data[y][x].at[1] = pallete[onedata].G;
	    data[y][x].at[2] = pallete[onedata].B;
	  }
      break;
    }

  fp_close(fp);
  free( pallete );

  return 1;
}



long FImageSave_DIB(self,filename,comment)
     image self;
     char *filename;
     char *comment;
{
  fprintf(stderr,"FImageSave_DIB:not yet supported\n");
  return 0;
}



long FImageMagic_DIB(fp)
     FILE *fp;
{
  char magic_char1 = getc(fp);
  char magic_char2 = getc(fp);

  rewind(fp);

  return (magic_char1 == 'B' || magic_char2 == 'M');
}
