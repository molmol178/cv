/*
 * imgfont.c
 */



#include "ImageCG.h"
#include <stdio.h>
#include <X11/Xlib.h>


#define DEFAULT_FONT_NAME "a14"


static Display *display = 0;
static long screen;
static Window root;
static GC Clear_gc;
static GC Font_gc;
static GC Kanji_gc;
static char *Font_name = DEFAULT_FONT_NAME;
static char *Kanji_name = "k14";
static XFontStruct *Font_struct = 0;
static XFontStruct *Kanji_struct = 0;
static unsigned long White_pixel;
static unsigned long Black_pixel;
static long plane;
static long depth;



static void __ImageInit()
{
  if ( display ) return;

  if ( display == 0 )
    {
      display = XOpenDisplay( ( char * )getenv( "DISPLAY" ) );
      if ( display == 0 )
	{
	  fprintf( stderr, "error:__XImageInit:can't open display\n" );
	  exit( -1 );
	}
    }
  screen  = DefaultScreen( display );
  root    = DefaultRootWindow( display );
  plane   = DisplayPlanes( display, screen );
  depth   = DefaultDepth( display, screen );

  Font_gc  = XCreateGC( display, root, 0, 0 );
  Kanji_gc = XCreateGC( display, root, 0, 0 );
  Clear_gc = XCreateGC( display, root, 0, 0 );

  Black_pixel = BlackPixel( display, screen );
  White_pixel = WhitePixel( display, screen );

  Font_struct  = XLoadQueryFont( display, Font_name  );
  if ( Font_struct == 0 )
    {
      fprintf( stderr, "error:__ImageInit:can't load font \"%s\n\"",
	      Font_name );
      return;
    }

  Kanji_struct = XLoadQueryFont( display, Kanji_name );
  if ( Font_struct == 0 )
    {
      fprintf( stderr, "error:__ImageInit:can't load font \"%s\n\"",
	      Kanji_name );
      return;
    }

  XSetFont( display, Font_gc,  Font_struct->fid  );
  XSetFont( display, Kanji_gc, Kanji_struct->fid );

  XSetForeground( display, Clear_gc, White_pixel );
  XSetForeground( display, Font_gc , Black_pixel );
  XSetForeground( display, Kanji_gc, Black_pixel );
}



void ImageSetStringFont
  _P1 (( char *, name ))
{
  XFontStruct *font_struct = 0;

  if ( name == 0 ) name = DEFAULT_FONT_NAME;
  if ( strcmp( name, Font_name ) == 0) return;

  __ImageInit();

  font_struct = XLoadQueryFont( display, name );
  if ( font_struct == 0 )
    {
      fprintf( stderr,
	       "error:ImageSetStringFont:can't open font \"%s\"", name );
      return;
    }

  XFreeFont( display, Font_struct );
  Font_struct = font_struct;
  Font_name = name;
  XSetFont( display, Font_gc, Font_struct->fid );
}



void ImageSetStringFont16
  _P1 (( char *, name ))
{
  XFontStruct *kanji_struct = 0;

  if ( strcmp( name, Kanji_name ) == 0) return;

  __ImageInit();

  kanji_struct = XLoadQueryFont( display, name );
  if ( kanji_struct == 0 )
    {
      fprintf( stderr,
	       "error:ImageSetStringFont16:can't open font \"%s\"", name );
      return;
    }

  XFreeFont( display, Kanji_struct );
  Kanji_struct = kanji_struct;
  Kanji_name = name;
  XSetFont( display, Kanji_gc, Kanji_struct->fid );
}



void ImageDrawString
  _P6 (( image , img    ),
       ( long  , ox     ),
       ( long  , oy     ),
       ( char *, str    ),
       ( long  , length ),
       ( char *, pixel  ))
{
  Pixmap pixmap;
  XImage *text;
  int dir, ascent, descent;
  XCharStruct over;
  long x, y, xsize, ysize;

  __ImageInit();

  XTextExtents( Font_struct, str, length, &dir, &ascent, &descent, &over );

  xsize = over.lbearing + over.rbearing;
  ysize = ascent + descent;

  pixmap = XCreatePixmap( display, root, xsize, ysize, depth );

  XFillRectangle( display, pixmap, Clear_gc, 0, 0, xsize, ysize );
  XDrawString( display, pixmap, Font_gc, over.lbearing, ascent, str, length );
  XFlush( display );

  if ( plane == 1 )
    text = XGetImage( display, pixmap, 0, 0, xsize, ysize, AllPlanes, XYPixmap );
  else
    text = XGetImage( display, pixmap, 0, 0, xsize, ysize, AllPlanes, ZPixmap );

  ox -= over.lbearing;
  oy -= ascent;
  for ( y = 0; y < ysize; y++ )
    for ( x = 0; x < xsize; x++ )
      if ( XGetPixel( text, x, y ) == Black_pixel )
	ImageDrawPoint( img, ox + x, oy + y, pixel );

  XFreePixmap( display, pixmap );
  XDestroyImage( text );
}



image ImageCreateString
  _P2 (( char *, str    ),
       ( long  , length ))
{
  image img;
  Pixmap pixmap;
  XImage *text;
  int dir, ascent, descent;
  XCharStruct over;
  long x, y, xsize, ysize;

  __ImageInit();

  XTextExtents( Font_struct, str, length, &dir, &ascent, &descent, &over );

  xsize = over.lbearing + over.rbearing;
  ysize = ascent + descent;

  pixmap = XCreatePixmap( display, root, xsize, ysize, depth );

  XFillRectangle( display, pixmap, Clear_gc, 0, 0, xsize, ysize );
  XDrawString( display, pixmap, Font_gc, over.lbearing, ascent, str, length );
  XFlush( display );

  if ( plane == 1 )
    text = XGetImage( display, pixmap, 0, 0, xsize, ysize, AllPlanes, XYPixmap );
  else
    text = XGetImage( display, pixmap, 0, 0, xsize, ysize, AllPlanes, ZPixmap );

  img = Image.createMake(str, Bit1, xsize, ysize);
  for ( y = 0; y < ysize; y++ )
    for ( x = 0; x < xsize; x++ )
      if ( XGetPixel( text, x, y ) == Black_pixel )
	ImageDrawPoint( img, x, y, (char*)1 );

  XFreePixmap( display, pixmap );
  XDestroyImage( text );

  return img;
}



void ImageDrawString16
  _P6 (( image    , img    ),
       ( long     , ox     ),
       ( long     , oy     ),
       ( XChar2b *, str    ),
       ( long     , length ),
       ( char *   , pixel  ))
{
  Pixmap pixmap;
  XImage *text;
  int dir, ascent, descent;
  XCharStruct over;
  long x, y, xsize, ysize;

  __ImageInit();

  XTextExtents16( Kanji_struct, str, length, &dir, &ascent, &descent, &over );

  xsize = over.lbearing + over.rbearing;
  ysize = ascent + descent;

  pixmap = XCreatePixmap( display, root, xsize, ysize, depth );

  XFillRectangle( display, pixmap, Clear_gc, 0, 0, xsize, ysize );
  XDrawString16( display, pixmap, Kanji_gc, over.lbearing, ascent, str, length );
  XFlush( display );

  if ( plane == 1 )
    text = XGetImage( display, pixmap, 0, 0, xsize, ysize, AllPlanes, XYPixmap );
  else
    text = XGetImage( display, pixmap, 0, 0, xsize, ysize, AllPlanes, ZPixmap );

  ox -= over.lbearing;
  oy -= ascent;
  for ( y = 0; y < ysize; y++ )
    for ( x = 0; x < xsize; x++ )
      if ( XGetPixel( text, x, y ) == Black_pixel )
	ImageDrawPoint( img, ox + x, oy + y, pixel );

  XFreePixmap( display, pixmap );
  XDestroyImage( text );
}



void ImageDrawImageString
  _P7 (( image , img       ),
       ( long  , ox        ),
       ( long  , oy        ),
       ( char *, str       ),
       ( long  , length    ),
       ( char *, forepixel ),
       ( char *, backpixel ))
{
  Pixmap pixmap;
  XImage *text;
  int dir, ascent, descent;
  XCharStruct over;
  long x, y, xsize, ysize;

  __ImageInit();

  XTextExtents( Font_struct, str, length, &dir, &ascent, &descent, &over );

  xsize = over.lbearing + over.rbearing;
  ysize = ascent + descent;

  pixmap = XCreatePixmap( display, root, xsize, ysize, depth );

  XFillRectangle( display, pixmap, Clear_gc, 0, 0, xsize, ysize );
  XDrawString( display, pixmap, Font_gc, over.lbearing, ascent, str, length );
  XFlush( display );

  if ( plane == 1 )
    text = XGetImage( display, pixmap, 0, 0, xsize, ysize, AllPlanes, XYPixmap );
  else
    text = XGetImage( display, pixmap, 0, 0, xsize, ysize, AllPlanes, ZPixmap );

  ox -= over.lbearing;
  oy -= ascent;
  for ( y = 0; y < ysize; y++ )
    for ( x = 0; x < xsize; x++ )
      if ( XGetPixel( text, x, y ) == Black_pixel )
	ImageDrawPoint( img, ox + x, oy + y, forepixel );
      else
	ImageDrawPoint( img, ox + x, oy + y, backpixel );

  XFreePixmap( display, pixmap );
  XDestroyImage( text );
}



void ImageDrawImageString16
  _P7 (( image    , img       ),
       ( long     , ox        ),
       ( long     , oy        ),
       ( XChar2b *, str       ),
       ( long     , length    ),
       ( char *   , forepixel ),
       ( char *   , backpixel ))
{
  Pixmap pixmap;
  XImage *text;
  int dir, ascent, descent;
  XCharStruct over;
  long x, y, xsize, ysize;

  __ImageInit();

  XTextExtents16( Kanji_struct, str, length, &dir, &ascent, &descent, &over );

  xsize = over.lbearing + over.rbearing;
  ysize = ascent + descent;

  pixmap = XCreatePixmap( display, root, xsize, ysize, depth );

  XFillRectangle( display, pixmap, Clear_gc, 0, 0, xsize, ysize );
  XDrawString16( display, pixmap, Kanji_gc, over.lbearing, ascent, str, length );
  XFlush( display );

  if ( plane == 1 )
    text = XGetImage( display, pixmap, 0, 0, xsize, ysize, AllPlanes, XYPixmap );
  else
    text = XGetImage( display, pixmap, 0, 0, xsize, ysize, AllPlanes, ZPixmap );

  ox -= over.lbearing;
  oy -= ascent;
  for ( y = 0; y < ysize; y++ )
    for ( x = 0; x < xsize; x++ )
      if ( XGetPixel( text, x, y ) == Black_pixel )
	ImageDrawPoint( img, ox + x, oy + y, forepixel );
      else
	ImageDrawPoint( img, ox + x, oy + y, backpixel );

  XFreePixmap( display, pixmap );
  XDestroyImage( text );
}



/*------------------------------------------------------------------------*
 * 指定した位置にテキストを表示する。
 *------------------------------------------------------------------------*/
void ImagePutString
  _P6 (( image , img   ),
       ( long  , ox    ),
       ( long  , oy    ),
       ( char *, pos   ),
       ( char *, str   ),
       ( char *, pixel ))
{
  image text;

  text = ImageCreateString(str, strlen(str));
  ImageDrawPointImage(img, ox, oy, text, pos, pixel);
  Image.destroy(text);
}
