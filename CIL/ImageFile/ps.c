/*
 * ps.c
 */



#include "FImage.h"
#include <stdio.h>
#include "misc/filelib.h"


long FImageLoad_PS_OLD(self,filename)
     image self;
     char *filename;
{
  char buf[1024];
  char bufopt[ 256 ];

  fprintf( stderr, "INPUT \"WIDTH HEIGHT BITS/SAMPLE\" ? " );
  fgets( bufopt, sizeof( bufopt ), stdin );
  bufopt[ strlen(bufopt)-1 ] = '\0';

  if (filename == 0 || strlen(filename) == 0 ||
      strequal("-",filename) || strequal("stdin",filename))
    sprintf(buf,"psidtopgm %s",bufopt);
  else
    sprintf(buf,"psidtopgm %s %s",bufopt,filename);

  return FImageLoadPipe_PNM(self,buf);
}



#define PNMTOPS_OPTION "-scale 1 -noturn"
long FImageSave_PS(self,filename,comment)
     image self;
     char *filename;
     char *comment;
{
  char buf[1024];
  char *pnmtops_option;

  pnmtops_option = getenv( "CIL_OPTION_pnmtops" );
  if ( pnmtops_option == 0 ) pnmtops_option = PNMTOPS_OPTION;

  if (filename == 0 || strlen(filename) == 0 ||
      strequal(filename,"-") || strequal(filename,"stdout"))
    sprintf(buf,"pnmtops %s",pnmtops_option);
  else
    sprintf(buf,"pnmtops %s > %s",pnmtops_option,filename);

  return FImageSavePipe_PNM(self,buf);
}



long FImageMagic_PS(fp)
     FILE *fp;
{
  char magic_char = getc(fp);
  ungetc(magic_char,fp);
  return magic_char == '%';
}



static long __char_to_hex( c )
  int c;
{
  if ( c >= '0' && c <= '9' )
    return c - '0';
  else if ( c >= 'A' && c <= 'F' )
    return c - 'A' + 10;
  else if ( c >= 'a' && c <= 'f' )
    return c - 'a' + 10;
  return -1;
}

static long __gethex1( fp )
  FILE *fp;
{
  long c, c1;

  do
    {
      c = fp_getc( fp );
      if ( c == -1 ) return 0;
      c1 = __char_to_hex( c );
    }
  while ( c1 == -1 );

  return c1;
}

static long __gethex2( fp )
  FILE *fp;
{
  long c1, c2;

  c1 = __gethex1( fp );
  c2 = __gethex1( fp );

  return ( c1 << 4 ) | ( c2 );
}

long FImageLoad_PS(self,filename)
     image self;
     char *filename;
{
  FILE *fp;
  char buf[ 1024 ];
  long num;
  char *head[ 64 ];
  long top_of_image = 0;

  long xsize, ysize, bits, type;

  if ( self == 0 )
    {
      fprintf( stderr, "error:PS.load:image is wrong.\n" );
      return 0;
    }

  fp = fp_open( filename, "r" );
  if ( fp == 0 ) return 0;

  /*
   * read xsize, ysize and bits/pixel
   */
  bits = 0;
  xsize = ysize = 0;
  while ( 1 )
    {
      if ( fp_gets( fp, buf, 1024 ) == 0 ) return 0;
      num = strsplit( buf, head, " \t\n" );
      if (( num == 3 ) && ( strisdigit( head[ 0 ] ) &&
			    strisdigit( head[ 1 ] ) &&
			    strisdigit( head[ 2 ] )))
	{
	  xsize = atol( head[ 0 ] );
	  ysize = atol( head[ 1 ] );
	  bits  = atol( head[ 2 ] );
	  break;
	}
    }

  if ( bits == 0 ) { fp_close( fp ); return 0; }

  /*
   * check pixel type and seek image head.
   */
  type = 0;
  while ( 1 )
    {
      if ( fp_gets( fp, buf, 1024 ) == 0 ) return 0;
      num = strsplit( buf, head, " \t\n" );
      if ( num == 1 )
	{
	  if ( strequal( head[ 0 ], "image" ) )
	    {
	      if ( bits == 1 ) type = Bit1; else
	      if ( bits == 4 ) type = Bit4; else
	      if ( bits == 8 ) type = UChar;
	    }
	  else
	  if ( strequal( head[ 0 ], "colorimage" ) )
	    {
	      type = UChar3;
	    }
	  break;
	}
    }

  if ( type == 0 ) { fp_close( fp ); return 0; }

  Image.make( self, type, xsize, ysize );

  switch ( type )
    {
    case Bit1:
      {
	register int x, y, xsize, ysize;
	register bit1 **p = __DATA( self, bit1 );
	static int bitmask[ 8 ] = { 1<<7, 1<<6, 1<<5, 1<<4,
				    1<<3, 1<<2, 1<<1, 1<<0 };
	int pixels;

	xsize = __XSIZE( self );
	ysize = __YSIZE( self );
	for ( y = 0; y < ysize; y++ )
	  for ( x = 0; x < xsize; x++ )
	    {
	      if (( x % 8 ) == 0 ) pixels = __gethex2( fp );
	      p[ y ][ x ] = ! ( bitmask[ x % 8 ] & pixels );
	    }
      }
      break;
    case Bit4:
      {
	register int i, n = __AREA( self );
	register bit4 *p = __RASTER( self, bit4 );
	for ( i = 0; i < n; i++ ) *p++ = __gethex1( fp );
      }
      break;
    case UChar:
      {
	register int i, n = __AREA( self );
	register uchar *p = __RASTER( self, uchar );
	for ( i = 0; i < n; i++ ) *p++ = __gethex2( fp );
      }
      break;
    case UChar3:
      {
	register int x, y, xsize, ysize;
	register uchar3 **p = __DATA( self, uchar3 );

	xsize = __XSIZE( self );
	ysize = __YSIZE( self );
	for ( y = 0; y < ysize; y++ )
	  {
	    for ( x = 0; x < xsize; x++ )
	      p[ y ][ x ].at[ 0 ] = __gethex2( fp );
	    for ( x = 0; x < xsize; x++ )
	      p[ y ][ x ].at[ 1 ] = __gethex2( fp );
	    for ( x = 0; x < xsize; x++ )
	      p[ y ][ x ].at[ 2 ] = __gethex2( fp );
	  }
      }
      break;
    }
  fp_close( fp );

  return 1;
}
