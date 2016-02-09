/*
 * c2d.c
 */



#include "FImage.h"
#include <stdio.h>
#include "../misc/strlib.h"
#include "../misc/filelib.h"


static void __cil_fp_gets(FILE *fp, char *buf, int size)
{
  int i;
  char ch;

  i = 0;
  do
    {
      ch = getc(fp);
      buf[i] = ch;
      i++;
      if (i >= size) break;
    }
  while (ch != '\n' && ch != EOF && ch != 0x0a && ch != 0x0d);
}


static long C2D__loadBit1Data
  _P2 (( image , self ),
       ( FILE *, fp   ))
{
  long xbyte,x,y;
  register uchar *buf;
  static ulong mask[] = { 1<<7, 1<<6, 1<<5, 1<<4, 1<<3, 1<<2, 1<<1, 1<<0 };

  xbyte = ( self->xsize + 7 ) / 8;
  buf = ( uchar * )calloc( self->ysize, xbyte );

  if ( fp_read( fp, ( char * )buf, self->ysize, xbyte ) != self->ysize )
    fprintf( stderr, "warning:C2D__loadBit1Data:can't read all\n" );

  for ( y = 0; y < self->ysize; y++ )
    for ( x = 0; x < self->xsize; x++ )
      if ( buf[ y * xbyte + x / 8 ] & mask[ x % 8 ] )
	self->data[ y ][ x ] = 1;

  free( ( char * )buf );

  return 1;
}



static long C2D__saveBit1Data
  _P2 (( image , self ),
       ( FILE *, fp   ))
{
  long xbyte, x, y;
  register uchar *buf;
  static ulong mask[ 8 ] = { 1<<7, 1<<6, 1<<5, 1<<4, 1<<3, 1<<2, 1<<1, 1<<0 };

  xbyte = ( self->xsize + 7 ) / 8;
  buf = ( uchar * )calloc( self->ysize, xbyte );

  for ( y = 0; y < self->ysize; y++ )
    for ( x = 0; x < self->xsize; x++ )
      if ( self->data[ y ][ x ] ) buf[ y * xbyte + x / 8 ] |= mask[ x % 8 ];

  if ( fp_write( fp, ( char * )buf, self->ysize, xbyte ) != self->ysize )
    fprintf( stderr, "warning:C2D__saveBit1Data:can't write all\n" );

  free( ( char * )buf );

  return 1;
}


static void swap_bytes
  _P1 (( image , self ))
{
  if ((self->type != UChar) && (self->type != Bit1) && (self->type != UChar3))
    {
      register long i, n = self->xsize * self->ysize;
      register unsigned char *b_data = (unsigned char*)(self->data[0]);
      register unsigned short *w_data = (unsigned short*)(self->data[0]);

      switch (self->type)
	{
	case Double:
	  n = 2 * n;
	case Float: case Long: case ULong:
	  for (i = 0; i < n; i++)
	    {
	      unsigned short w_tmp = w_data[i*2];
	      w_data[i*2] = w_data[i*2 + 1];
	      w_data[i*2 + 1] = w_tmp;
	    }
	  n = 2 * n;
	case Short: case UShort:
	  for (i = 0; i < n; i++)
	    {
	      unsigned char tmp = b_data[i*2];
	      b_data[i*2] = b_data[i*2 + 1];
	      b_data[i*2 + 1] = tmp;
	    }
	  break;

	case Double2:
	  n = 2 * n;
	case Float2: case Long2: case ULong2:
	  for (i = 0; i < 2*n; i++)
	    {
	      unsigned short w_tmp = w_data[i*2];
	      w_data[i*2] = w_data[i*2 + 1];
	      w_data[i*2 + 1] = w_tmp;
	    }
	  n = 2 * n;
	case Short2: case UShort2:
	  for (i = 0; i < 2*n; i++)
	    {
	      unsigned char tmp = b_data[i*2];
	      b_data[i*2] = b_data[i*2 + 1];
	      b_data[i*2 + 1] = tmp;
	    }
	  break;

	case Double3:
	  n = 2 * n;
	case Float3: case Long3: case ULong3:
	  for (i = 0; i < 3*n; i++)
	    {
	      unsigned short w_tmp = w_data[i*2];
	      w_data[i*2] = w_data[i*2 + 1];
	      w_data[i*2 + 1] = w_tmp;
	    }
	  n = 2 * n;
	case Short3: case UShort3:
	  for (i = 0; i < 3*n; i++)
	    {
	      unsigned char tmp = b_data[i*2];
	      b_data[i*2] = b_data[i*2 + 1];
	      b_data[i*2 + 1] = tmp;
	    }
	  break;
	}
    }
}


long FImageLoad_C2D
  _P2 (( image , self     ),
       ( char *, filename ))
{
  FILE *fp;
  char buf[ 1024 ];
  long n;
  char *head[ 64 ];
  long size, xsize, ysize;
  long type;

  //  fprintf(stderr, "a.");

  if ( self == 0 )
    {
      fprintf( stderr, "error:C2D.load:image is wrong.\n" );
      return 0;
    }

  fp = fp_open( filename, "r" );

  //  fprintf(stderr, "b.");

  __cil_fp_gets( fp, buf, 1024 );
  FImageSetHeader( buf );

  //  fprintf(stderr, "c.");

  n = strsplit( buf, head, " \n" );
  if ( ! strequal( "C2D", head[0] ) ) return 0;

  //  fprintf(stderr, "d(%s).", head[1]);

  type = typeget( head[ 1 ] );

  //  fprintf(stderr, "d1.");

  if ( type == 0 )
    {
      fprintf( stderr,
	       "warning:C2D.load:image type is unknown.(%s)\n", head[ 1 ] );
      size = atol( head[ 2 ] );
      type = typeenter( head[ 1 ], size );
    }
  //  fprintf(stderr, "e.");

  xsize = atol( head[ 3 ] );
  ysize = atol( head[ 4 ] );

  //  fprintf(stderr, "f.");

  if ( n > 5 )
    FImageSetComment( FImageGetHeader() + ( head[ 5 ] - head[ 0 ] ));

  //  fprintf(stderr, "g.");

  Image.make( self, type, xsize, ysize );

  //  fprintf(stderr, "h.");

  if ( type == Bit1 )
    C2D__loadBit1Data( self, fp );
  else
    if (fp_readbyte( fp, Image.raster( self ), Image.byte( self ) ) != Image.byte( self ) )
      fprintf(stderr,"warning:can't write all\n");

  //  fprintf(stderr, "i.");

  fp_close( fp );

  //  fprintf(stderr, "j.");

#ifdef __CIL_BIG_EDIAN__ /* Swap Word, Long: STUPIT! */
  swap_bytes(self);
#endif

  //  fprintf(stderr, "k.");

  return 1;
}



long FImageSave_C2D
  _P3 (( image , self     ),
       ( char *, filename ),
       ( char *, comment  ))
{
  FILE *fp;

  if (self == 0)
    {
      fprintf(stderr,"error:C2D.save:image is wrong.\n");
      return 0;
    }

  fp = fp_open( filename, "w" );
  if ( fp == 0 )
    {
      fprintf( stderr, "can't open file to write (%s)\n", filename );
      return 0;
    }

  if ( comment != 0 )
    {
      if (comment[strlen(comment)-1] == '\n') {
	// fprintf( fp, "C2D %s %d %d %d %s", typename( self->type ), typesize( self->type ), self->xsize, self->ysize, comment );
	fprintf( fp, "C2D ");
	ftypeprintf( fp, self->type );
	fprintf(fp, " %d %d %d %s", typesize( self->type ), self->xsize, self->ysize, comment );
      } else {
	//fprintf( fp, "C2D %s %d %d %d %s\n", typename( self->type ), typesize( self->type ), self->xsize, self->ysize, comment );
	fprintf( fp, "C2D ");
	ftypeprintf( fp, self->type );
	fprintf(fp, " %d %d %d %s\n", typesize( self->type ), self->xsize, self->ysize, comment );
      }
    }
  else
    {
      //fprintf( fp, "C2D %s %d %d %d\n", typename( self->type ), typesize( self->type ), self->xsize, self->ysize );
      fprintf( fp, "C2D ");
      ftypeprintf( fp, self->type );
      fprintf(fp, " %d %d %d\n", typesize( self->type ), self->xsize, self->ysize );
    }

#ifdef __CIL_BIG_EDIAN__ /* Swap Word, Long: STUPIT! */
  swap_bytes(self);
#endif

  if ( self->type == Bit1 )
    C2D__saveBit1Data( self, fp );
  else
    if (fp_writebyte( fp, Image.raster( self ), Image.byte( self ) ) != Image.byte( self ))
      fprintf(stderr,"warning:can't write all\n");
  fp_close( fp );

#ifdef __CIL_BIG_EDIAN__ /* Swap Word, Long: STUPIT! */
  swap_bytes(self);
#endif

  return 1;
}



long FImageMagic_C2D
  _P1 (( FILE *, fp ))
{
  char magic[ 4 ];

  fread( magic, 1, 3, fp );
  magic[ 3 ] = '\0';

  rewind( fp );

  return ( strcmp( magic, "C2D" ) == 0 );
}
