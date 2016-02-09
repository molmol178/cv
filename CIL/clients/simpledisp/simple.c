/*
 * filename : simple.c
 * author   : Takahiro Sugiyama
 * date     : ?
 * update   : Tuesday February 15 1994 by Takahiro Sugiyama
 *	      タイトルを指定できるようにする.
 */



#include "Image.h"
#include "Image/Funcs.h"
#include "Xcil/Xcil.h"
#include "misc/optlib.h"
#include "misc/memlib.h"



#define SID_KEY 100



static char *opt_list[] = {
  "input:*:*:(image) name of the input image",
  "help:-h:print this message",
  "title:-t:*:(title) name of the window",
  "geometry:-g:1:(geometry) geometry of the top window <XSIZE>x<YSIZE>[+-]<X>[+-]<Y>",
  "column:-c:1:(int) number of the window column",
  "space:-s:1:5x5:(int*2) x,y space of the window <XSPACE>x<YSPACE>",
  "reverse:-rev:reverse image",
  "root:-r:display on the root window",
  "ROOT:-R:display on the whole root window",
};



typedef struct {
  image img;
  Window w;
  char *filename;
  char *title;
  long x, y;
  long xsize, ysize;
} SIDRec;



void xsimple_disp
  P3 (( Display *, d   ),
      ( XContext , c   ),
      ( SIDRec * , sid ))

void display_on_root_window
  P6 (( Display *, display  ),
      ( char *   , filename ),
      ( long     , x        ),
      ( long     , y        ),
      ( long     , xsize    ),
      ( long     , ysize    ))



/*
 * Event Handlers
 */
long expose_hdr
  P4 (( Display *, d ),
      ( Window   , w ),
      ( XContext , c ),
      ( XEvent * , e ))

long all_quit_hdr
  P4 (( Display *, d ),
      ( Window   , w ),
      ( XContext , c ),
      ( XEvent * , e ))

long quit_hdr
  P4 (( Display *, d ),
      ( Window   , w ),
      ( XContext , c ),
      ( XEvent * , e ))



int main( argc, argv )
     int argc;
     char *argv[];
{
  long sem;
  int i;
  char **files;
  int nfiles;

  char **titles;
  int ntitles;

  long ox, oy, x, y, xsize, ysize;
  long col, column;
  long xspace, yspace;
  long ymax;

  Display *d;
  XContext c;
  SIDRec *sid_list;

  OPTION_SETUP( opt_list );

  if ( argc == 1 ) optusage( 1 );
  if ( optspecified( "help" ) ) optmanual( 1 );

  d = XcilOpenDisplay(0);

  c = XUniqueContext();

  column = 0;
  if ( optspecified( "column" ) ) column = optvalueint( "column" );

  {
    char *head[10];
    long n = strsplit( optvalue( "space" ), head, "x" );
    if ( n > 0 ) xspace = yspace = atol( head[ 0 ] );
    if ( n > 1 ) yspace = atol( head[ 1 ] );
  }

  ox = 0;
  oy = 0;
  xsize = 0;
  ysize = 0;
  if ( optspecified( "geometry" ) )
    XcilTransGeometry( optvalue( "geometry" ), &ox, &oy, &xsize, &ysize );

  nfiles = optvaluenum("input");
  while ( nfiles == 0 )
    {
      optinput("input");
      nfiles = optvaluenum("input");
    }
  files = optvaluelist("input");

  if ( optspecified( "root" ) )
    {
      display_on_root_window( d, files[0], ox, oy, xsize, ysize );
    }

  if ( optspecified( "ROOT" ) )
    {
      ox = 0; oy = 0;
      xsize = DisplayWidth( d, 0 );
      ysize = DisplayHeight( d, 0 );
      display_on_root_window( d, files[0], ox, oy, xsize, ysize );
    }

  x = ( ox > 0 ) ? ox : xspace;
  y = ( oy > 0 ) ? oy : yspace;
  sem = 0;
  col = 0;
  ymax = 0;
  sid_list = typenew1( nfiles, SIDRec );

  ntitles = 0;
  titles = 0;

  if ( optspecified( "title" ) )
    {
      ntitles = optvaluenum( "title" );
      titles = optvaluelist( "title" );
    }

  for ( i = 0; i < nfiles; i++ )
    {
      SIDRec *sid = &sid_list[ i ];

      sid->filename = files[ i ];
      sid->img = Image.createFromFilename( sid->filename, sid->filename );
      if ( sid->img == 0 )
	{
	  fprintf( stderr, "can't open file %s\n", sid->filename );
	  continue;
	}

      if ((optspecified("reverse")) &&
	  ((__TYPE(sid->img) == Bit1) ||
	   (__TYPE(sid->img) == Float) ||
	   (__TYPE(sid->img) == UChar) ||
	   (__TYPE(sid->img) == UChar3)))
	{
	  image__reverse(sid->img, sid->img);
	}

      if ( i < ntitles )
	sid->title = titles[ i ];
      else
	sid->title = sid->filename;

      sid->x = x;
      sid->y = y;
      sid->xsize = ( xsize == 0 ) ? __XSIZE( sid->img ) : xsize;
      sid->ysize = ( ysize == 0 ) ? (sid->xsize * __YSIZE( sid->img ) / __XSIZE( sid->img )) : ysize;
      if ( ymax < sid->ysize ) ymax = sid->ysize;

      x += sid->xsize + xspace + Xcil_wm_xsize;
      col ++;
      if ( ( x >= ( DisplayWidth( d, 0 ) - (sid->xsize + Xcil_wm_xsize) ) ) ||
	   ( ( column > 0 ) && ( col >= column ) ) )
	{
	  col = 0;
	  x = ( ox > 0 ) ? ox : xspace;
	  y += ymax + yspace + Xcil_wm_ysize;
	  ymax = 0;
	  if ( y >= ( DisplayHeight( d, 0 ) - (sid->ysize + Xcil_wm_ysize) ))
	    y = ( oy > 0 ) ? oy : yspace;
	}

      xsimple_disp( d, c, sid );
      sem++;
    }

  while ( sem > 0 ) sem -= XcilEventLoop( d, c );

  return 0;
}



static XcilCallback event_list[] = {
  { Expose, (xcil_callback)expose_hdr, 0 },
};



static XcilCallback key_event_list[] = {
  { 'q', quit_hdr, 0 },
  { 'Q', all_quit_hdr, 0 },
};



void xsimple_disp
  _P3 (( Display *, d ),
       ( XContext , c ),
       ( SIDRec * , s ))
{
  s->w = XcilCreateWindow( d, 0,
			   s->x, s->y, s->xsize, s->ysize, 1, s->title );

  XcilManagedWindow( d, s->w, c );

  XcilAddWindowContext( d, s->w, c, SID_KEY, (caddr_t)s );
  XcilAddCallbacks( d, s->w, c, event_list, XcilNumber( event_list ) );
  XcilAddKeyCallbacks( d, s->w, c, key_event_list, XcilNumber( key_event_list ) );

  XMapWindow( d, s->w );
}



long expose_hdr
  _P4 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ))
{
  SIDRec *sid = (SIDRec *)XcilGetWindowContext( d, w, c, SID_KEY );

  XShowImageFull( d, w, Xcil_black_gc, sid->img );
  return 0;
}



long quit_hdr
  _P4 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ))
{
  SIDRec *sid = (SIDRec *)XcilGetWindowContext( d, w, c, SID_KEY );

  Image.destroy( sid->img );
  XcilUnmanagedWindow( d, w, c );
  XDestroyWindow( d, w );

  return 1;
}



long all_quit_hdr
  _P4 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ))
{
  return 100;
}



void display_on_root_window
  _P6 (( Display *, display  ),
       ( char *   , filename ),
       ( long     , x        ),
       ( long     , y        ),
       ( long     , xsize    ),
       ( long     , ysize    ))
{
  image img;
  Window window;
  XSetWindowAttributes xswa;

  img = Image.createFromFilename( "ROOT", filename );
  if ( img == 0 )
    {
      fprintf( stderr, "can't open file(%s)", filename );
      exit( -1 );
    }

  x = ( x > 0 ) ? x : 0;
  y = ( y > 0 ) ? y : 0;
  xsize = ( xsize == 0 ) ? __XSIZE( img ) : xsize;
  ysize = ( ysize == 0 ) ? (xsize * __YSIZE( img ) / __XSIZE( img )) : ysize;

  xswa.backing_store = Always;
  window = DefaultRootWindow( display );
  XChangeWindowAttributes( display, window, CWBackingStore, &xswa );

  XShowImagePartOfWindow( display, window, Xcil_black_gc,
			  img, x, y, xsize, ysize );

  Image.destroy( img );

  exit( 0 );
}
