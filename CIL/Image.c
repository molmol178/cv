/*
 * filename : Image.c
 * author   : Takahiro Sugiyama
 * date     : 1992/11/10
 * redate   : 1993/05/19
 * redate   : 1993/10/05
 */



#include "Image.h"
#include "ImageFile.h"

#include "misc/strlib.h"
#include "misc/shmemlib.h"
#include "misc/memlib.h"
//#include "server/cilserver.h"



/*
 * common methods
 */
static image image__create
  P1 (( char *, name ))

static void image__destroy
  P1 (( image, self ))

static void image__print
  P1 (( image, self ))

static long image__load
  P2 (( image , self     ),
      ( char *, filename ))

static long image__save
  P3 (( image , self     ),
      ( char *, filename ),
      ( char *, comment  ))

static image image__createFromImage
  P2 (( char *, name ),
      ( image , src  ))

static image image__createFromFilename
  P2 (( char *, name     ),
      ( char *, filename ))

static image image__createMake
  P4 (( char *, name  ),
      ( long  , type  ),
      ( long  , xsize ),
      ( long  , ysize ))

static image image__createFromBitmapData
  P4 (( char  *, name  ),
      ( uchar *, data  ),
      ( long   , xsize ),
      ( long   , ysize ))



/*
 * access methods
 */
static char *image__name
  P1 ((image, self))

static long image__type
  P1 ((image, self))

static long image__xsize
  P1 ((image, self))

static long image__ysize
  P1 ((image, self))

static char **image__data
  P1 ((image, self))

static long image__area
  P1 ((image, self))

static long image__byte
  P1 ((image, self))

static char *image__raster
  P1 ((image, self))



/*
 * etc
 */
static void image__make
  P4 (( image, self  ),
      ( long , type  ),
      ( long , xsize ),
      ( long , ysize ))

static void image__free
  P1 (( image, self ))

static void image__copy
  P2 (( image, self ),
      ( image,  src ))

static void image__clear
  P1 (( image, self ))

static void image__resize
  P4 (( image, self  ),
      ( image, src   ),
      ( long , xsize ),
      ( long , ysize ))

static void image__sub
  P6 (( image, self  ),
      ( image, src   ),
      ( long , srcx  ),
      ( long , srcy  ),
      ( long , xsize ),
      ( long , ysize ))

static void image__swap
  P2 (( image, src1 ),
      ( image, src2 ))



/*
 * image disp
 */
static void image__display
  P2 (( image , self ),
      ( char *, opt  ))

static void image__undisplay
  P1 (( image, self ))



struct ImageRec Image = {
  image__create,
  image__destroy,
  image__print,
  image__load,
  image__save,

  image__name,
  image__type,
  image__xsize,
  image__ysize,
  image__data,

  image__area,
  image__byte,
  image__raster,

  image__make,
  image__free,
  image__copy,
  image__clear,
  image__resize,
  image__sub,
  image__swap,

  image__createFromImage,
  image__createFromFilename,
  image__createMake,
  image__createFromBitmapData,

  image__display,
  image__undisplay
};

uchar PackedBit1Mask[] = { 1 << 7, 1 << 6, 1 << 5, 1 << 4,
			   1 << 3, 1 << 2, 1 << 1, 1 << 0 };



/*-----------------------------------------------------*
 * image__create : 画像の属性を格納する領域を確保する。
 *-----------------------------------------------------*/
static image image__create
  _P1 (( char *, name ))
{
  image self;

  self = typenew(struct imageRec);
  self->name  = strnew(name);
  self->type  = 0;
  self->xsize = 0;
  self->ysize = 0;
  self->data  = 0;

  return self;
}



/*----------------------------------*
 * image__destroy : 画像を解放する。
 *----------------------------------*/
static void image__destroy
  _P1 (( image, self ))
{
  if ( self == 0 ) return;

  image__free( self );
  strfree( self->name );
  self->name = 0;
  typefree( self );
}



static void image__print
  _P1 (( image, self ))
{
  if ( self == 0 )
    {
      printf( "================================\n" );
      printf( "   THIS IMAGE IS NOT CREATED.   \n" );
      printf( "================================\n" );
    }
  else
  if ( self->name == 0 )
    {
      printf( "================================\n" );
      printf( "   THIS IMAGE WAS DESTROIED.    \n" );
      printf( "================================\n" );
    }
  else
    {
      printf( "================================\n" );
      printf( "             Image              \n" );
      printf( "================================\n" );
      //printf( "    type  : %-8s\n", typename( self->type ) );
      printf( "    type  : "); typeprintf( self->type );printf("\n");
      printf( "    name  : %-16s\n", self->name );
      printf( "    xsize : %d\n", self->xsize );
      printf( "    ysize : %d\n", self->ysize );
      printf( "    data  : 0x%08x\n", self->data );
      printf( "================================\n" );
    }
}



static char *image__name
  _P1 (( image, self ))
{
  return self->name;
}



static long image__type
  _P1 (( image, self ))
{
  return self->type;
}



static long image__xsize
  _P1 (( image, self ))
{
  return self->xsize;
}



static long image__ysize
  _P1 (( image, self ))
{
  return self->ysize;
}



static char **image__data
  _P1 (( image, self ))
{
  return self->data;
}



static long image__area
  _P1 (( image, self ))
{
  return self->xsize * self->ysize;
}



static long image__xbyte
  _P1 (( image, self ))
{
  if ( self->type == PackedBit1 ) return ( self->xsize + 7 ) / 8;
  return self->xsize * typesize( self->type );
}



static long image__byte
  _P1 (( image, self ))
{
  return image__xbyte( self ) * self->ysize;
}



static char *image__raster
  _P1 (( image, self ))
{
  return self->data[ 0 ];
}



static void __make_local
  _P4 (( image, self  ),
       ( long , type  ),
       ( long , xsize ),
       ( long , ysize ))
{
  self->type  = type;
  self->xsize = xsize;
  self->ysize = ysize;
  self->data  = memnew2( image__xbyte( self ), ysize, 1 );
}



static long __shared_header_compile
  _P5 (( char *, ptr   ),
       ( char *, name  ),
       ( long *, type  ),
       ( long *, xsize ),
       ( long *, ysize ))
{
  int i, n;
  char info[ 512 ];
  char *head[16];

  *type  = 0;
  *xsize = 0;
  *ysize = 0;

  for ( i = 0; ( i < 511 ) && ( ptr[ i ] != '\n' ); i++ ) info[ i ] = ptr[ i ];
  info[ i ] = 0;
  if ( i == 512 ) return 0;

  n = strsplit( info, head, " " );

  if ( n >= 4 )
    {
      strcpy( name, head[ 0 ] );
      *type  = typeget( head[ 1 ] );
      *xsize = strtolong( head[ 2 ] );
      *ysize = strtolong( head[ 3 ] );
      return i + 1;
    }

  return 0;
}


/*
static void __make_share
  _P4 (( image, self  ),
       ( long , type  ),
       ( long , xsize ),
       ( long , ysize ))
{
  char head[ 512 ];
  char *ptr, *data;
  long y, xbyte, n;
  cilRec *info;
  long state;

  sprintf( head, "%-16s %-8s %8d %8d\n",
	   self->name, typename( type ), xsize, ysize );

  info = cil_open( self->name );
  if ( info == 0 )
    {
      info = cil_create( self->name );
      if ( info == 0 ) return;
    }

  ptr = shmemget( info->key );

  if ( ptr != 0 )
    {
      char lname[16];
      long ltype, lxsize, lysize;

      __shared_header_compile( ptr, lname, &ltype, &lxsize, &lysize );

      if ( ( xsize != lxsize ) || ( ysize != lysize ) || ( type != ltype ) )
	{
	  info->share = 0;
	  shmemfree( info->key );
	  ptr = 0;
	}
    }

  self->type  = type;
  self->xsize = xsize;
  self->ysize = ysize;

  if ( ptr == 0 )
    {
      ptr = shmemnew( info->key, strlen( head ) + image__byte( self ) );

      if ( ptr == 0 )
	{
	  fprintf( stderr, "error:image__make_share:shmemnew:can't shmat!\n" );
	  cil_close( info );
	  return;
	}
    }
  info->share = 1;
  cil_close( info );

  strcpy( ptr, head );
  data = ptr + strlen( head );

  xbyte = image__xbyte( self );
  self->data = typenew1( ysize + 1, char * );
  for ( y = 0; y < ysize; y++ )
    self->data[ y ] = data + y * xbyte;
  self->data[ y ] = ptr;

  image__clear( self );
}
*/



static void image__make
  _P4 (( image, self  ),
       ( long , type  ),
       ( long , xsize ),
       ( long , ysize ))
{
  if ( xsize == self->xsize && ysize == self->ysize && self->type == type )
    {
      image__clear( self );
      return;
    }

  image__free( self );
  /*  if ( IS_SHARED( self ) )
    __make_share( self, type, xsize, ysize );
    else*/
    __make_local( self, type, xsize, ysize );

  if ( self->data == 0 )
    {
      //fprintf( stderr, "warning:Image.make(%s,%s,%d,%d)\n", self->name, typename( type ), xsize, ysize );
      fprintf( stderr, "warning:Image.make(%s,%d,%d,%d)\n", self->name, type, xsize, ysize );

      fprintf( stderr, "   can't make !\n" );
      self->type  = 0;
      self->xsize = 0;
      self->ysize = 0;
    }
}



static void image__free
  _P1 (( image, self ))
{
  if ( IS_SHARED( self ) )
    {
      if ( self->data )
	{
	  shmdt( self->data[ self->ysize ] );
	  typefree1( self->data );
	}
    }
  else
    if ( self->data ) typefree2( self->data );

  self->type  = 0;
  self->xsize = 0;
  self->ysize = 0;
  self->data  = 0;
}



static void image__copy
  _P2 (( image, self ),
       ( image,  src ))
{
  if ( IS_SHARED( self ) && IS_SHARED( src ) )
    if ( strcmp( __NAME(self), __NAME(src) ) == 0 ) return;

  image__make( self, src->type, src->xsize, src->ysize );
  if ( image__area( src ) && image__area( self ) )
    memcpy( self->data[ 0 ], src->data[ 0 ], image__byte( src ) );
}



static void image__clear
     _P1 (( image, self ))
{
  int n;
  register int i;
  register char *p;

  n = image__byte( self );
  if ( n == 0 ) return;
  p = self->data[ 0 ];
  for ( i = 0; i < n; i++ )
    p[i] = 0;
}


/*
static long __load_shared
  _P2 (( image , self     ),
       ( char *, filename ))
{
  char *ptr,*data;
  long i, xbyte, n;
  char *head[ 20 ];
  cilRec *info;

  image__free( self );

  info = cil_open( filename );
  if ( info == 0 ) return 0;
  ptr = shmemget( info->key );
  cil_close( info );

  if ( ptr == 0 ) return 0;

  strfree( self->name );
  self->name = strnew( filename );
  i = __shared_header_compile( ptr, self->name,
			       &self->type, &self->xsize, &self->ysize );
  if ( i == 0 ) return 0;

  data = ptr + i;
  xbyte = image__xbyte( self );
  self->data = typenew1( self->ysize + 1, char * );

  self->data[ 0 ] = data;
  for ( i = 1; i < self->ysize; i++ )
    self->data[ i ] = self->data[ i - 1 ] + xbyte;
  self->data[ i ] = ptr;

  return 1;
}
*/


static long image__load
  _P2 (( image , self     ),
       ( char *, filename ))
{

  //  if ( filename[ 0 ] == '@' ) /* 共有メモリのキー名 */
  //    return __load_shared( self, filename );

  return ImageFile.load( self, filename );
}



static long __save_shared
  _P3 (( image , self     ),
       ( char *, filename ),
       ( char *, comment  ))
{
  image tmp;

  if ( self->data == 0 ) return 1;

  if ( strcmp( filename, self->name ) == 0 ) return 1;

  tmp = image__createMake( filename, self->type, self->xsize, self->ysize );
  image__copy( tmp, self );
  image__destroy( tmp );

  return 1;
}



static long image__save
  _P3 (( image , self     ),
       ( char *, filename ),
       ( char *, comment  ))
{
  //  if ( filename[ 0 ] == '@' ) /* 共有メモリのキー名 */
  //    return __save_shared( self, filename, comment );

  return ImageFile.save( self, filename, comment );
}



static void image__resize
  _P4 (( image, self  ),
       ( image, src   ),
       ( long , xsize ),
       ( long , ysize ))
{
  int sx,sy;
  int px,py;
  int x,y;
  image temp;
  int size;

  if (src == self)
    temp = image__create(self->name);
  else
    temp = self;
  image__make(temp,src->type,xsize,ysize);
  if (image__area(temp) == 0)
    {
      fprintf(stderr,"error:Image.resize(%s,%s,%d,%d)\n",
	      self->name,src->name,xsize,ysize);
      if (src == self) image__destroy(temp);
      return;
    }
  image__clear(temp);

  size = typesize(src->type);
  for (sy = 0,y = 0; y < ysize; y++)
    {
      py = sy / ysize;
      for (sx = 0,x = 0; x < xsize; x++)
	{
	  px = sx / xsize;
	  COPY_2(temp->data,x,y,src->data,px,py,size);
	  sx += src->xsize;
	}
      sy += src->ysize;
    }

  if (src == self)
    {
      image__copy(self,temp);
      image__destroy(temp);
    }
}



static void image__sub
  _P6 (( image, self  ),
       ( image, src   ),
       ( long , srcx  ),
       ( long , srcy  ),
       ( long , xsize ),
       ( long , ysize ))
{
  int px,py;
  int x,y;
  image temp;
  int size;

  if (src == self)
    temp = image__create(self->name);
  else
    temp = self;
  image__make(temp,src->type,xsize,ysize);
  if (image__area(temp) == 0)
    {
      fprintf(stderr,"error:Image.sub(%s,%s,%d,%d,%d,%d)\n",
	      self->name,src->name,srcx,srcy,xsize,ysize);
      if (src == self) image__destroy(temp);
      return;
    }

  image__clear(temp);

  size = typesize(src->type);
  for (y = 0; y < ysize; y++)
    {
      py = y + srcy;
      if (py < 0 || py >= src->ysize) continue;
      for (x = 0; x < xsize; x++)
	{
	  px = x + srcx;
	  if (px < 0 || px >= src->xsize) continue;
	  COPY_2(temp->data,x,y,src->data,px,py,size);
	}
    }

  if (src == self)
    {
      image__copy(self,temp);
      image__destroy(temp);
    }
}



static image image__createFromFilename
  _P2 (( char *, name     ),
       ( char *, filename ))
{
  image self;

  self = image__create( name );
  if ( image__load( self, filename ) == 0 )
    {
      image__destroy( self );
      return 0;
    }

  return self;
}



static image image__createFromImage
  _P2 (( char *, name ),
       ( image , src  ))
{
  image self;

  self = image__create( name );
  image__copy( self, src );

  return self;
}



static image image__createMake
  _P4 (( char *, name  ),
       ( long  , type  ),
       ( long  , xsize ),
       ( long  , ysize ))
{
  image self;

  self = image__create( name );
  image__make( self, type, xsize, ysize );
  if ( image__area( self ) == 0 )
    {
      //fprintf( stderr, "error:Image.createMake(%s,%s,%d,%d)\n", name, typename( type ), xsize, ysize );
      fprintf( stderr, "error:Image.createMake(%s,%d,%d,%d)\n", name, type, xsize, ysize );
      image__destroy( self );
      return 0;
    }

  return self;
}



static void image__swap
  _P2 (( image, src1 ),
       ( image, src2 ))
{
  long mtype;
  long xsize, ysize;
  char **data;

  if ( IS_SHARED( src1 ) != IS_SHARED( src2 ) )
    {
      char *name;
      name = src1->name;
      src1->name = src2->name;
      src2->name = name;
    }

  mtype = src2->type;
  xsize = src2->xsize;
  ysize = src2->ysize;
  data  = src2->data;

  src2->type  = src1->type;
  src2->xsize = src1->xsize;
  src2->ysize = src1->ysize;
  src2->data  = src1->data;

  src1->type  = mtype;
  src1->xsize = xsize;
  src1->ysize = ysize;
  src1->data  = data;
}



static image image__createFromBitmapData
  _P4 (( char  *, name  ),
       ( uchar *, data  ),
       ( long   , xsize ),
       ( long   , ysize ))
{
  image self;

  self = image__createMake( name, PackedBit1, xsize, ysize );
  memcpy( self->data[ 0 ], data, image__byte( self ) );

  return self;
}



static void image__display
  _P2 (( image , self ),
       ( char *, opt  ))
{
  /*
  if ( ImageDispOK( self ) == 0 )
    ImageDispExec( self, opt );
  else
    ImageDispDraw( self );
  */
}



static void image__undisplay
  _P1 (( image, self ))
{
  //  ImageDispQuit( self );
}
