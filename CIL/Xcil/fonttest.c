#include "XImageCG.h"
#include <stdio.h>


main(argc,argv)
     int argc;
     char *argv[];
{
  XImage *src;
  int i;
  Display *d;
  Window root;
  Window w;
  GC gc;

  d = XOpenDisplay( ( char * )getenv( "DISPLAY" ) );
  root = DefaultRootWindow( d );

  w = XCreateSimpleWindow( d, root, 100, 100, 600, 400, 2,
			  BlackPixel( d, 0 ), WhitePixel( d, 0 ) );
  XMapWindow( d, w );

  gc = XCreateGC( d, w, 0, 0 );
  XSetForeground( d, gc, BlackPixel( d, 0 ) );

  XFlush( d );

  fprintf( stderr, "push return key !" ); getchar();

  src = XGetImage( d, w, 0, 0, 600, 400, AllPlanes, ZPixmap );

  fprintf( stderr, "getimage:push return key !" ); getchar();

  for ( i = 1; i < argc; i++ )
    {
      XImageDrawString( src, 100, 100 + i * 20,
		       argv[i], strlen(argv[i]), BlackPixel( d, 0 ) );

      XImageDrawLine( src, 100, 100 + i * 20, 120, 100, BlackPixel( d, 0 ));
    }

  XPutImage( d, w, gc, src, 0, 0, 0, 0, 600, 400 );
  XFlush( d );

  fprintf( stderr, "push return key !" ); getchar();
}
