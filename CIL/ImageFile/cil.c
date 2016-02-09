/*
 * filename : cil.c
 * author   : Takahiro Sugiyama
 * date     : Friday, April 15 1994
 * describe : CIL IMAGE FORMAT
 *
 * CIL <typename> <typesize> <xsize> <ysize>
 * <comment lines>
 * @BEGIN
 * <data>
 * @END
 *
 */



#include "FImage.h"
#include <stdio.h>
#include "../misc/strlib.h"
#include "../misc/filelib.h"



static long CIL__loadBit1Data
  _P2 (( image , self ),
       ( FILE *, fp   ))
{
  long xbyte,x,y;
  register uchar *buf;
  static ulong mask[] = { 1<<7, 1<<6, 1<<5, 1<<4, 1<<3, 1<<2, 1<<1, 1<<0 };

  xbyte = ( self->xsize + 7 ) / 8;
  buf = ( uchar * )calloc( self->ysize, xbyte );

  if ( fp_read( fp, ( char * )buf, self->ysize, xbyte ) != self->ysize )
    fprintf( stderr, "warning:CIL__loadBit1Data:can't read all\n" );

  for ( y = 0; y < self->ysize; y++ )
    for ( x = 0; x < self->xsize; x++ )
      if ( buf[ y * xbyte + x / 8 ] & mask[ x % 8 ] )
	self->data[ y ][ x ] = 1;

  free( ( char * )buf );

  return 1;
}



static long CIL__saveBit1Data
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
    fprintf( stderr, "warning:CIL__saveBit1Data:can't write all\n" );

  free( ( char * )buf );

  return 1;
}



long FImageLoad_CIL
  _P2 (( image , self     ),
       ( char *, filename ))
{
  FILE *fp;
  char buf[ 1024 ];
  long n;
  char *head[ 64 ];
  long size, xsize, ysize;
  long type;

  if ( self == 0 )
    {
      fprintf( stderr, "error:CIL.load:image is wrong.\n" );
      return 0;
    }

  fp = fp_open( filename, "r" );

  fp_gets( fp, buf, 1024 );
  FImageSetHeader( buf );

  n = strsplit( buf, head, " \n" );
  if ( ! strequal( "CIL", head[0] ) ) return 0;

  type = typeget( head[ 1 ] );
  if ( type == 0 )
    {
      fprintf( stderr,
	       "warning:CIL.load:image type is unknown.(%s)\n", head[ 1 ] );
      size = atol( head[ 2 ] );
      type = typeenter( head[ 1 ], size );
    }
  xsize = atol( head[ 3 ] );
  ysize = atol( head[ 4 ] );

  cil_read_comment( fp );

  Image.make( self, type, xsize, ysize );
  if ( type == Bit1 )
    CIL__loadBit1Data( self, fp );
  else
    if (fp_readbyte( fp, Image.raster( self ), Image.byte( self ) ) != Image.byte( self ) )
      fprintf(stderr,"warning:can't write all\n");

  fp_close( fp );

  return 1;
}



long FImageSave_CIL
  _P3 (( image , self     ),
       ( char *, filename ),
       ( char *, comment  ))
{
  FILE *fp;

  if (self == 0)
    {
      fprintf(stderr,"error:CIL.save:image is wrong.\n");
      return 0;
    }

  fp = fp_open( filename, "w" );
  if ( fp == 0 )
    {
      fprintf( stderr, "can't open file to write (%s)\n", filename );
      return 0;
    }

  if ( comment != 0 ) {
    //fprintf( fp, "CIL %s %d %d %d %s\n", typename( self->type ), typesize( self->type ), self->xsize, self->ysize, comment );
    fprintf( fp, "CIL ");
    ftypeprintf( fp, self->type );
    fprintf(fp, " %d %d %d %s\n", typesize( self->type ), self->xsize, self->ysize, comment );
  } else {
    //fprintf( fp, "CIL %s %d %d %d\n", typename( self->type ), typesize( self->type ), self->xsize, self->ysize );
    fprintf( fp, "CIL ");
    ftypeprintf( fp, self->type );
    fprintf(fp, " %d %d %d\n", typesize( self->type ), self->xsize, self->ysize );
  }

  if ( self->type == Bit1 )
    CIL__saveBit1Data( self, fp );
  else
    if (fp_writebyte( fp, Image.raster( self ), Image.byte( self ) ) != Image.byte( self ))
      fprintf(stderr,"warning:can't write all\n");

  fp_close( fp );

  return 1;
}



long FImageMagic_CIL
  _P1 (( FILE *, fp ))
{
  char magic[ 4 ];

  fread( magic, 1, 3, fp );
  magic[ 3 ] = '\0';

  rewind( fp );

  return ( strcmp( magic_char, "CIL" ) == 0 );
}
