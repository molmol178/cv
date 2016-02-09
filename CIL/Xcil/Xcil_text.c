/*
 * filename : Xcil_text.c
 * project  : Xcil Project
 * author   : Takahiro Sugiyama
 * date     : 1993/10/25
 */



#include "Xcil.h"
#include "misc/memlib.h"
//#include <varargs.h>
#include <stdarg.h>



typedef struct {

  long x_dot;

  long y;
  long y_size;
  long font_height;
  long font_width;

  long x_size_dot;

  long x_offset;
  long y_offset;

  char **text;

/* for gets */
  long	       pos;
  long	       y_pos;
  xcil_callback callback;
  caddr_t       client_value;
  char	       log[1024];
  long	       last_key;

  long cut_y1;
  long cut_y2;
  long cut_x1;
  long cut_x2;

} XcilTextItem;



void __add_history
  P1 (( char *, string ))
void __next_history
  P1 (( char *, string ))
void __prev_history
  P1 (( char *, string ))



static long __Xcil_cut_buffer_output_only
  P4 (( Display *     , display ),
      ( Window        , window  ),
      ( XContext      , context ),
      ( XEvent *      , event   ))



void __XcilText_size_set
  _P3 (( Display *     , display ),
       ( Window        , window  ),
       ( XcilTextItem *, item    ))
{
  long xsize, ysize;

  XcilGetWindowSize( display, window, &xsize, &ysize );
  item->font_height = XcilTextHeight( 0 ) + 2;
  item->font_width  = XcilTextWidth( 0 );
  item->y_size = ( ysize - 4 ) / item->font_height;
  if ( item->y_size == 0 ) item->y_size = 1;
  item->y_offset = ( ysize - item->font_height * item->y_size ) / 2;
  item->x_offset = 4;
  item->x_size_dot = xsize - item->x_offset * 2;

  if ( item->text ) free( item->text );
  item->text = ( char ** )calloc( item->y_size, sizeof( char *) );
}



XcilTextItem *__XcilText_item
  _P3 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ))
{
  XcilTextItem *item;

  item = ( XcilTextItem * )XcilGetWindowContext
			( display, window, context, XCIL_TEXT_ITEM );
  if ( item == 0 )
    {
      fprintf( stderr,"error:__XcilText:This window can not use text.\n" );
      fprintf( stderr,"    You should use \"XcilTextSet\".\n" );
      exit( -1 );
    }

  return item;
}



void __XcilText_free_line
  _P2 (( XcilTextItem *, item ),
       ( long          , y    ))
{
  if ( item->text[ y ] ) free( item->text[ y ] );
  item->text[ y ] = 0;
}



void __XcilText_scrollup
  _P3 (( Display *     , display ),
       ( Window        , window  ),
       ( XcilTextItem *, item    ))
{
  long i, height;

  item->x_dot = 0;
  item->y ++;
  if ( item->y >= item->y_size )
    {
      item->y = item->y_size - 1;
      XCopyArea( display, window, window, Xcil_black_gc,
		item->x_offset, item->y_offset + item->font_height,
		item->x_size_dot, item->font_height*( item->y_size - 1 ),
		item->x_offset, item->y_offset );
      XFillRectangle( display, window, Xcil_white_gc,
		     item->x_offset,
		     item->y_offset + item->font_height*(item->y_size-1),
		     item->x_size_dot, item->font_height );
      __XcilText_free_line( item, 0 );
      for ( i = 1; i < item->y_size; i++ )
	item->text[ i - 1 ] = item->text[ i ];
      item->text[ item->y ] = 0;
    }
  XFlush( display );
}



void __XcilText_add_string
  _P2 (( XcilTextItem *, item      ),
       ( char *        , newstring ))
{
  int n, i, j;
  char *string;

  string = item->text[ item->y ];

  n = ( newstring ) ? strlen( newstring ) : 0;
  i = ( string ) ? strlen( string ) : 0;

  string = typerenew1( string, i + n + 1, char );
  for ( j = 0; j < n; j++ )
    string[ i + j ] = newstring[ j ];
  string[ i + j ] = '\0';

  item->text[ item->y ] = string;
}



static long expose_hdr
  _P5 (( Display *     , display ),
       ( Window        , window  ),
       ( XContext      , context ),
       ( XEvent *      , event   ),
       ( XcilTextItem *, item    ))
{
  int i;
  long x, y;

  x = item->x_offset;
  for ( i = 0; i < item->y_size; i++ )
    {
      if ( item->text[ i ] == 0 ) continue;
      y = item->y_offset + i * item->font_height;
      XcilPutStringBox( display, window, Xcil_black_gc, x, y, 10, item->font_height, "lc", item->text[ i ] );
    }

  return 0;
}



void XcilTextSet
  _P3 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ))
{
  XcilTextItem *item;
  static Cursor cursor = 0;

  item = ( XcilTextItem * )calloc( 1, sizeof( XcilTextItem ) );

  if ( cursor == 0 ) cursor = XCreateFontCursor( display, XC_xterm );
  XDefineCursor( display, window, cursor );

  XcilAddWindowContext( display, window, context, XCIL_TEXT_ITEM, (caddr_t)item );

  XcilAddCallback( display, window, context, Expose, (xcil_callback)expose_hdr, (caddr_t)item );
  XcilAddCallback( display, window, context, ButtonPress, (xcil_callback)__Xcil_cut_buffer_output_only, (caddr_t)item );

  __XcilText_size_set( display, window, item );
}



void XcilTextNextLine
  _P3 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ))
{
  XcilTextItem *item;
  item = __XcilText_item( display, window, context );
  __XcilText_scrollup( display, window, item );
}



long XcilTextLocate
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( long     , x_dot   ),
       ( long     , y       ))
{
  XcilTextItem *item;
  item = __XcilText_item( display, window, context );
  item->x_dot = x_dot;
  item->y = y;
  __XcilText_free_line( item, y );
}



void XcilTextClear
  _P3 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ))
{
  XcilTextItem *item;
  int i;

  item = __XcilText_item( display, window, context );

  for ( i = 0; i < item->y_size; i++ )
    __XcilText_free_line( item, i );

  item->x_dot = 0;
  item->y = 0;
}



void XcilTextPuts
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( GC       , gc      ),
       ( char *   , string  ))
{
  XcilTextItem *item;
  long x, y;

  item = __XcilText_item( display, window, context );

  x = item->x_offset;
  y = item->y_offset + item->y * item->font_height;

  __XcilText_add_string( item, string );
  XcilPutStringBox( display, window, gc, x, y, 10, item->font_height, "lc", item->text[ item->y ] );
  XcilTextNextLine( display, window, context );
}



void XcilText_nputs
  _P6 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( GC       , gc      ),
       ( char *   , string  ),
       ( long     , n       ))
{
  char buf[ 1024 ] ;
  XcilTextItem *item;
  long x, y;

  item = __XcilText_item( display, window, context );

  x = item->x_offset;
  y = item->y_offset + item->y * item->font_height;

  strncpy( buf, string, n );
  buf[ n ] = '\0';
  __XcilText_add_string( item, buf );
  XcilPutStringBox( display, window, gc, x, y, 10, item->font_height, "lc", item->text[ item->y ] );
}



void XcilTextPrintf( Display *display, Window window, XContext context, GC gc, char *format, ... )
{
  char string[ 1024 ];
  va_list args;
  char *head[ 64 ];
  int j, i, n;

  va_start( args, format );
  vsprintf( string, format, args );
  va_end( args );

  n = strlen( string );
  for ( i = 0; i < n; )
    {
      for ( ; string[ i ] == '\n'; i++ )
	XcilTextNextLine( display, window, context );
      for ( j = 0; string[ i + j ] && ( string[ i + j ] != '\n' ); j++ ) ;
      XcilText_nputs( display, window, context, gc, string + i, j );
      i += j;
    }
}



void XcilTextPutc
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( GC       , gc      ),
       ( char     , c       ))
{
  XcilTextItem *item;
  long n, x, y;
  char string[ 3 ];
  char *log;

  item = __XcilText_item( display, window, context );

  x = item->x_offset;
  y = item->y_offset + item->y * item->font_height;
  log = item->text[ item->y ];
  n = ( log ) ? strlen( log ) : 0;

  switch( c )
    {
    case '\n':
    case 0x0e:/* C-n */
    case 0x0d:/* return */
      XcilTextNextLine( display, window, context );
      break;

    case 0x08:/* backspace */
    case 0x7f:/* delete */
      if ( n >= 1 )
	{
	  XFillRectangle( display, window, Xcil_white_gc, x, y, XcilTextWidth( log ), item->font_height );
	  log[ n - 1 ] = '\0';
	  XcilPutStringBox( display, window, gc, x, y, 10, item->font_height, "lc", item->text[ item->y ] );
	}
      break;

    default:
      if ( ! isprint( c ) ) break;
      string[ 0 ] = c;
      string[ 1 ] = '\0';
      __XcilText_add_string( item, string );
      if ( n )
	XFillRectangle( display, window, Xcil_white_gc, x, y, XcilTextWidth( log ), item->font_height );
      XcilPutStringBox( display, window, gc, x, y, 10, item->font_height, "lc", item->text[ item->y ] );
      break;
    }
}



void XcilTextPutIcon16
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( GC       , gc      ),
       ( long     , icnID   ))
{
  XcilTextItem *item;
  long x, y;

  item = __XcilText_item( display, window, context );

  x = item->x_offset;
  y = item->y_offset + item->y * item->font_height;

  __XcilText_add_string( item, "   " );
  XFillRectangle( display, window, Xcil_white_gc, x, y, 16, 16 );
  XcilPutIcon16Box( display, window, gc, x, y, 16, 16, "lc", icnID );
}



void XcilTextPutIcon32
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( GC       , gc      ),
       ( long     , icnID   ))
{
  XcilTextItem *item;
  long x, y;

  item = __XcilText_item( display, window, context );

  x = item->x_offset;
  y = item->y_offset + item->y * item->font_height;

  __XcilText_add_string( item, "      " );
  XFillRectangle( display, window, Xcil_white_gc, x, y, 32, 32 );
  XcilPutIcon32Box( display, window, gc, x, y, 32, 32, "lc", icnID );
}



void XcilTextPutIcon20
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( GC       , gc      ),
       ( long     , icnID   ))
{
  XcilTextItem *item;
  long x, y;

  item = __XcilText_item( display, window, context );

  x = item->x_offset;
  y = item->y_offset + item->y * item->font_height;

  __XcilText_add_string( item, "    " );
  XFillRectangle( display, window, Xcil_white_gc, x, y, 20, 20 );
  XcilPutIcon20Box( display, window, gc, x, y, 20, 20, "lc", icnID );
}



#define XCIL_ES_KEYIN ( 1 << 16 )

static long __Xcil_getc
  _P6 (( Display *     , display ),
       ( Window        , window  ),
       ( XContext      , context ),
       ( XEvent *      , event   ),
       ( XcilTextItem *, item    ),
       ( long          , ascii   ))
{
  XcilTextPutc( display, window, context, Xcil_black_gc, ascii );
  if ( ascii == 0 ) return 0;
  
  return XCIL_ES_KEYIN | ascii;
}



long XcilTextGetc
  _P3 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ))
{
  XcilTextItem *item;
  long state;

  item = __XcilText_item( display, window, context );

  XcilAddCallback( display, window, context, KeyPress, (xcil_callback)__Xcil_getc, (caddr_t)item );

  state = XcilEventLoop( display, context );

  XcilDelCallback( display, window, context, KeyPress, (xcil_callback)__Xcil_getc );

  if ( state & XCIL_ES_KEYIN )
    return state & 0x00ff;

  return -state;
}



static long __Xcil_getkeysym
  _P6 (( Display *     , display ),
       ( Window        , window  ),
       ( XContext      , context ),
       ( XEvent *      , event   ),
       ( XcilTextItem *, item    ),
       ( long          , ascii   ))
{
  KeySym keysym;

  if ( isprint( ascii ) )
    XcilTextPutc( display, window, context, Xcil_black_gc, ascii );

  keysym = XLookupKeysym( event, 0 );
  
  if ( keysym == 0 ) return 0;

  return XCIL_ES_KEYIN | keysym;
}



long XcilTextGetKeysym
  _P3 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ))
{
  XcilTextItem *item;
  long state;

  item = __XcilText_item( display, window, context );

  XcilAddCallback( display, window, context, KeyPress, (xcil_callback)__Xcil_getkeysym, (caddr_t)item );

  state = XcilEventLoop( display, context );

  XcilDelCallback( display, window, context, KeyPress, (xcil_callback)__Xcil_getkeysym );

  if ( state & XCIL_ES_KEYIN )
    return state & 0xffff;

  return -state;
}



void __Xcil_redraw_line_gets
  _P4 (( Display *     , display ),
       ( Window        , window  ),
       ( XcilTextItem *, item    ),
       ( long          , top_pos ))
{
  long x, y;

  x = item->x_offset;
  y = item->y_offset + item->y * item->font_height;

  XFillRectangle( display, window, Xcil_white_gc, x, y,
		  XcilTextWidth( item->text[ item->y ] ) + 8,
		  item->font_height );

  item->text[ item->y ] = typerenew1( item->text[ item->y ], top_pos + strlen( item->log ) + 1, char );
  strcpy( item->text[ item->y ] + top_pos, item->log );

  XcilPutStringBox( display, window, Xcil_black_gc, x, y, 10, item->font_height, "lc", item->text[ item->y ] );
}



static __cursor_on
  _P4 (( Display *     , display ),
       ( Window        , window  ),
       ( XcilTextItem *, item    ),
       ( long          , pos     ))
{
  char *string;
  long x, y, xsize, ysize;
  long ox;

  x = item->x_offset;
  y = item->y_offset + item->y * item->font_height;

  string = item->text[ item->y ];
  ox = ( string ) ? XTextWidth( Xcil_font_struct, string, pos ) : 0;

  x = x + ox + 1;
  y = y + 1;
  xsize = 7;
  ysize = item->font_height - 2;

  if ( item->last_key == 0x1b )
    {
      XSetTile( display, Xcil_or_gc, Xcil_gray_xpixmap[ 1 ] );
      XFillRectangle( display, window, Xcil_or_gc, x, y, xsize, ysize );
    }
  else
  if ( item->last_key < 0 )
    {
      XDrawRectangle( display, window, Xcil_black_gc, x, y, xsize - 1, ysize - 1 );
    }
  else
    {
      XFillRectangle( display, window, Xcil_invert_gc, x, y, xsize, ysize );
    }
}



static __cursor_clear
  _P4 (( Display *     , display ),
       ( Window        , window  ),
       ( XcilTextItem *, item    ),
       ( long          , pos     ))
{
  char *string, buf[ 2 ];
  long x, y, xsize, ysize;
  long ox;

  x = item->x_offset;
  y = item->y_offset + item->y * item->font_height;

  string = item->text[ item->y ];
  ox = ( string ) ? XTextWidth( Xcil_font_struct, string, pos ) : 0;

  x = x + ox + 1;
  y = y + 1;
  xsize = 7;
  ysize = item->font_height - 2;

  XFillRectangle( display, window, Xcil_white_gc, x, y, xsize, ysize );

  /*
   * カーソルを消したところに文字があった場合
   */
  if ( string && ( string[ pos ] != '\0' ))
    {
      buf[ 0 ] = string[ pos ]; buf[ 1 ] = '\0';
      XcilPutStringBox( display, window, Xcil_black_gc, x - 1, y - 1, 10, item->font_height, "lc", buf );
    }
}



static long enter_hdr
  _P5 (( Display *     , display ),
       ( Window        , window  ),
       ( XContext      , context ),
       ( XEvent *      , event   ),
       ( long          , top_pos ))
{
  XcilTextItem *item;

  item = __XcilText_item( display, window, context );

  __cursor_clear( display, window, item, top_pos + item->pos  );
  item->last_key = 0;
  __cursor_on( display, window, item, top_pos + item->pos  );

  return 0;
}



static long leave_hdr
  _P5 (( Display *     , display ),
       ( Window        , window  ),
       ( XContext      , context ),
       ( XEvent *      , event   ),
       ( long          , top_pos ))
{
  XcilTextItem *item;

  item = __XcilText_item( display, window, context );

  __cursor_clear( display, window, item, top_pos + item->pos  );
  item->last_key = -1;
  __cursor_on( display, window, item, top_pos + item->pos  );

  return 0;
}



long forward_char(str,pos)
     char *str;
     long pos;
{
  long n = strlen(str);
  if (pos >= n) return n;
  pos++;
  return pos;
}



long backward_char(str,pos)
     char *str;
     long pos;
{
  if (pos < 1) return 0;
  pos--;
  return pos;
}



long forward_word(str,pos)
     char *str;
     long pos;
{
  long num = strlen(str);

  if (! isalnum(str[pos]))
    while ((pos < num) && (! isalnum(str[pos])))
      pos++;

  while ((pos < num) && (isalnum(str[pos]))) pos++;

  return pos;
}



long backward_word(str,pos)
     char *str;
     long pos;
{
  if (pos < 1) return 0;
  if ( pos == 1 ) return 0;

  pos--;
  if (! isalnum(str[pos]))
    while ((pos > 0) && (! isalnum(str[pos])))
      pos--;
  if ( pos <= 0 ) return 0;

  while ((pos > 0) && isalnum(str[pos]))
    pos--;
  if ( ! isalnum( str[ pos ] )) pos++;

  return pos;
}



static long __Xcil_gets
  _P6 (( Display *     , display ),
       ( Window        , window  ),
       ( XContext      , context ),
       ( XEvent *      , event   ),
       ( long          , top_pos ),
       ( long          , ascii   ))
{
  long state = 0;
  XcilTextItem *item;
  char *text;
  char *log;
  long nlog;
  char old[1024];
  long i, x, y;
  long old_pos, pos, tmp;
  static char yank[1024] = "";
  static long nyank = 0;
  KeySym keysym;

  item = __XcilText_item( display, window, context );
  log = item->log;
  nlog = strlen( log );
  strcpy( old, log );
  old_pos = pos = item->pos;

  nyank = strlen( yank );

  keysym = XLookupKeysym( event, 0 );

  x = item->x_offset;
  y = item->y_offset + item->y * item->font_height;

  if ( item->last_key == 0x1b )
    {
      switch ( keysym )
	{
	case 0xff51:/*XK_Left:*/
	  pos = backward_word( log, pos );
	  break;

	case 0xff53:/*XK_Right:*/
	  pos = forward_word( log, pos );
	  break;

	case 0xff52: /* XK_up */
	  __add_history( log );
	  __next_history( log );
	  nlog = strlen( log );
	  if ( pos > nlog ) pos = nlog;
	  break;

	case 0xff54: /* XK_down */
	  __add_history( log );
	  __prev_history( log );
	  nlog = strlen( log );
	  if ( pos > nlog ) pos = nlog;
	  break;
	}

      switch ( ascii )
	{
	case 'f':/* ESC-f */
	  pos = forward_word( log, pos );
	  break;

	case 'b':/* ESC-b */
	  pos = backward_word( log, pos );
	  break;

	case 0x08: /* ESC-backspace */
	case 0x7f:
	  if (pos == 0) break;
	  pos = backward_word( log, pos );
	  for ( i = pos; i < old_pos; i++ )
	    yank[i-pos] = log[i];
	  yank[i-pos] = 0;
	  for (i = old_pos; i < nlog; i++)
	    log[pos + i - old_pos] = log[i];
	  log[pos + i - old_pos] = 0;
	  nlog = strlen(log);
	  break;

	case 'd':/* ESC-d */
	case 0x04:/* delete back */
	  if (pos == nlog) break;
	  pos = forward_word(log,pos);
	  for (i = old_pos; i < pos; i++)
	    yank[i-old_pos] = log[i];
	  yank[i-old_pos] = 0;
	  for (i = pos; i < nlog; i++)
	    log[old_pos + i - pos] = log[i];
	  log[old_pos + i - pos] = 0;
	  nlog = strlen(log);
	  pos = old_pos;
	  break;

	case 'c':/* ESC-c */
	  if (pos == nlog) break;
	  pos = forward_word(log,pos);
	  for (i = old_pos; i < pos; i++)
	    if (isupper(log[i])) log[i] = tolower(log[i]);
	  i = backward_word(log,pos);
	  if (islower(log[i])) log[i] = toupper(log[i]);
	  break;

	case 'l':/* ESC-l */
	  if (pos == nlog) break;
	  pos = forward_word(log,pos);
	  for (i = old_pos; i < pos; i++)
	    if (isupper(log[i])) log[i] = tolower(log[i]);
	  break;

	case 'u':/* ESC-u */
	  if (pos == nlog) break;
	  pos = forward_word(log,pos);
	  for (i = old_pos; i < pos; i++)
	    if (islower(log[i])) log[i] = toupper(log[i]);
	  break;

	default:
	case 0x1b: ascii = 0;  break;
	}
    }
  else
    {
      switch ( keysym )
	{
	case 0xff51:/*XK_Left:*/
	  pos = ( pos <= 0 ) ? 0 : pos - 1;
	  break;

	case 0xff53:/*XK_Right:*/
	  pos = ( pos == nlog ) ? pos : pos + 1;
	  break;

	case 0xff52: /* XK_up */
	  __add_history( log );
	  __next_history( log );
	  nlog = strlen( log );
	  if ( pos > nlog ) pos = nlog;
	  break;

	case 0xff54: /* XK_down */
	  __add_history( log );
	  __prev_history( log );
	  nlog = strlen( log );
	  if ( pos > nlog ) pos = nlog;
	  break;
	}

      switch ( ascii )
	{
	case '\n': 
	case 0x0d: /* return */
	  {
	    xcil_callback callback;
	    
	    __add_history( log );
	    callback = item->callback;
	    if ( callback == 0 ) return XCIL_ES_KEYIN;
	    state = callback( display, window, context,
			      event, item->client_value, (caddr_t)log );
	  }
	  return state;
	  break;

	case 0x0e: /* C-n */
	  __add_history( log );
	  __next_history( log );
	  nlog = strlen( log );
	  if ( pos > nlog ) pos = nlog;
	  break;

	case 0x10: /* C-p */
	  __add_history( log );
	  __prev_history( log );
	  nlog = strlen( log );
	  if ( pos > nlog ) pos = nlog;
	  break;

	case 0x02:/* back */
	  pos = ( pos <= 0 ) ? 0 : pos - 1;
	  break;
	  
	case 0x06:/* for */
	  pos = ( pos == nlog ) ? pos : pos + 1;
	  break;
	  
	case 0x14: /* C-t */
	  if (pos <= 1) break;
	  if (pos == nlog)
	    { tmp = log[pos-1]; log[pos-1] = log[pos-2]; log[pos-2] = tmp; }
	  else
	    { tmp = log[pos-1]; log[pos-1] = log[pos]; log[pos] = tmp; pos++; }
	  break;
	  
	case 0x01:/* C-a */
	  pos = 0;
	  break;
	  
	case 0x05:/* C-e */
	  pos = nlog;
	  break;
	  
	case 0x19:/* C-y */
	  nyank = strlen(yank);
	  for (i = nlog; i >= pos; i--)
	    log[i+nyank] = log[i];
	  for (i = 0; i < nyank; i++)
	    log[pos+i] = yank[i];
	  nlog = strlen(log);
	  pos += strlen(yank);
	  break;
	  
	case 0x0b:/* C-k */
	  for (i = pos; i < nlog; i++)
	    yank[i-pos] = log[i];
	  yank[i-pos] = 0;
	  nlog = pos;
	  break;
	  
	case 0x08: /* backspace */
	case 0x7f: /* delete */
	  if (pos <= 0) break;
	  for (i = pos; i < nlog; i++)
	    log[i-1] = log[i];
	  nlog--;
	  pos--;
	  break;
	  
	case 0x04: /* C-d, delete back */
	  if ( nlog == 0 || nlog <= pos ) break;
	  for ( i = pos + 1; i < nlog; i++ )
	    log[ i - 1 ] = log[ i ];
	  nlog--;
	  break;
	  
	case 0x15: /* C-u */
	  strcpy( yank, log );
	  nlog = 0;
	  pos = 0;
	  break;
	  
	default:
	  if ( ascii >= 0x80 ) break;
	  if ( ! isprint( ascii ) ) break;
	  for ( i = nlog; i > pos; i--)
	    log[i] = log[i-1];
	  log[pos] = ascii;
	  pos++;
	  nlog++;
	  break;
	}
    }

  __cursor_clear( display, window, item, top_pos + old_pos );

  if ( nlog < 0 ) nlog = 0;
  log[nlog] = '\0';

  item->pos = pos;

  if ( strcmp( log, old ) != 0 )
    __Xcil_redraw_line_gets( display, window, item, top_pos );

  item->last_key = ascii;
  __cursor_on( display, window, item, top_pos + pos );
  XFlush( display );

  return 0;
}



static void __coord_to_locate
  _P7 (( Display *     , display ),
       ( Window        , window  ),
       ( XcilTextItem *, item    ),
       ( long          , x       ),
       ( long          , y       ),
       ( long *        , lx      ),
       ( long *        , ly      ))
{
  long locy,locx;

  locy = ( y - item->y_offset ) / item->font_height;
  if (( locy < 0 ) || ( locy >= item->y_size ))
    {
      *lx = -1, *ly = -1;
      return;
    }
  *ly = locy;

  locx = ( x - item->x_offset ) / item->font_width;
  if ( locx < 0 )
    {
      *lx = -1;
      return;
    }
  *lx = locx;
}



void __Xcil_store_left
  _P4 (( Display *     , display ),
       ( Window        , window  ), 
       ( XEvent *      , event   ),
       ( XcilTextItem *, item    ))
{
  long x, y;

  x = event->xbutton.x;
  y = event->xbutton.y;

  __coord_to_locate( display, window, item, x, y,
		     &item->cut_x1, &item->cut_y1 );
}



void __Xcil_store_right
  _P4 (( Display *     , display ),
       ( Window        , window  ), 
       ( XEvent *      , event   ),
       ( XcilTextItem *, item    ))
{
  char *buf, *string;
  long nbuf, ntext;
  long i, x, y, tmp;
  long last_return = 0;

  x = event->xbutton.x;
  y = event->xbutton.y;

  __coord_to_locate( display, window, item, x, y,
		     &item->cut_x2, &item->cut_y2 );
  if (( item->cut_y1 < 0 ) || ( item->cut_y2 < 0 )) return;

  if ( item->cut_y1 > item->cut_y2 )
    {
      tmp = item->cut_y1; item->cut_y1 = item->cut_y2; item->cut_y2 = tmp;
      tmp = item->cut_x1; item->cut_x1 = item->cut_x2; item->cut_x2 = tmp;
    }

  if ( item->cut_y1 == item->cut_y2 )
    {
      if ( item->cut_x2 < item->cut_x1 )
	{
	  tmp = item->cut_x1;
	  item->cut_x1 = item->cut_x2;
	  item->cut_x2 = tmp;
	}
      if ( item->cut_x2 < 0 ) return;
      if ( item->cut_x1 < 0 ) item->cut_x1 = 0;
      string = item->text[ item->cut_y1 ];
      ntext = ( string ) ? strlen( string ) : 0;
      if ( item->cut_x2 > ntext )
	{ item->cut_x2 = ntext; if ( ntext ) last_return = 1; }
      if ( item->cut_x2 < 0 ) item->cut_x2 = 0;
      buf = calloc( item->cut_x2 - item->cut_x1 + 2, sizeof( char ) );
      for ( i= 0, x = item->cut_x1; x < item->cut_x2; x++, i++ )
	buf[ i ] = string[ x ];
      if ( last_return ) buf[i++] = '\n';
      buf[ i ] = '\0';
      XStoreBuffer( display, buf, i, 0 );
      free( buf );
    }
  else
    {
      y = item->cut_y1;
      string = item->text[ y ];
      ntext = ( string ) ? strlen( string ) : 0;
      nbuf = ntext - item->cut_x1 + 2;
      if ( nbuf <= 0 ) nbuf = 1;
      for ( y = y + 1; y < item->cut_y2; y++ )
	{
	  string = item->text[ y ];
	  nbuf += ( string ) ? strlen( string ) + 1: 1;
	}
      string = item->text[ y ];
      ntext = ( string ) ? strlen( string ) : 0;
      if ( item->cut_x2 > ntext )
	{ item->cut_x2 = ntext; if ( ntext ) last_return = 1; }
      if ( item->cut_x2 < 0 ) item->cut_x2 = 0;
      nbuf += item->cut_x2;
      nbuf++;

      buf = calloc( nbuf, sizeof( char ) );

      i = 0;
      y = item->cut_y1;
      string = item->text[ y ];
      ntext = ( string ) ? strlen( string ) : 0;
      for ( x = item->cut_x1; x < ntext; x++, i++ ) buf[i] = string[x];
      buf[i++] = '\n';

      for ( y = y + 1; y < item->cut_y2; y++ )
	{
	  string = item->text[ y ];
	  ntext = ( string ) ? strlen( string ) : 0;
	  for ( x = 0; x < ntext; x++, i++ ) buf[i] = string[x];
	  buf[i++] = '\n';
	}
      string = item->text[ y ];
      for ( x = 0; x < item->cut_x2; x++, i++ ) buf[i] = string[x];
      if ( last_return ) buf[i++] = '\n';
      buf[i] = '\0';
      XStoreBuffer( display, buf, strlen( buf ), 0 );
      free( buf );
    }
}



static long __Xcil_cut_buffer
  _P5 (( Display *     , display ),
       ( Window        , window  ),
       ( XContext      , context ),
       ( XEvent *      , event   ),
       ( long          , top_pos ))
{
  char *buf;
  int i, nlog, nbuf;
  XcilTextItem *item;
  long do_exec = 0, state = 0;
  long x, y;

  item = __XcilText_item( display, window, context );

  x = event->xbutton.x;
  y = event->xbutton.y;

  switch( event->xbutton.button )
    {
    case Button1: /* 左ポジションを入れる */
      __Xcil_store_left( display, window, event, item );
      if ( item->cut_y1 == item->y )
	{
	  __cursor_clear(display, window, item, top_pos + item->pos );
	  nbuf = strlen( item->log );
	  if ( item->cut_x1 >= 0 ) item->pos = item->cut_x1 - top_pos;
	  if ( item->pos < 0 ) item->pos = 0;
	  if ( nbuf < item->pos ) item->pos = nbuf;
	  __cursor_on( display, window, item, top_pos + item->pos );
	}

      break;
    case Button3: /* CUT_BUFFER0 にコピーする */
      __Xcil_store_right( display, window, event, item );
      break;

    case Button2:/* CUT_BUFFER0 からコピーしてくる。 */
      __cursor_clear( display, window, item, top_pos + item->pos );
      buf = XFetchBuffer( display, &nbuf, 0 );
      if ( nbuf == 0 || buf == 0 ) break;
      nlog = strlen( item->log );
      for ( i = 0; ( buf[i] != '\n') && ( i < nbuf ); i++ ) ;
      if ( buf[i] == '\n' ) do_exec = 1;
      nbuf = i;
      for ( i = nlog; i >= item->pos; i--)
	item->log[i+nbuf] = item->log[i];
      for (i = 0; i < nbuf; i++)
	item->log[item->pos+i] = buf[i];
      item->pos += i;
      __Xcil_redraw_line_gets( display, window, item, top_pos );
      __cursor_on( display, window, item, top_pos + item->pos );
      XFree( buf );

      break;
    }

  if ( do_exec )
    {
      xcil_callback callback = item->callback;
      __add_history( item->log );
      if ( callback == 0 ) return XCIL_ES_KEYIN;
      state = callback( display, window, context,  event, item->client_value, (caddr_t)item->log );
    }

  return state;
}



static long __Xcil_cut_buffer_output_only
  _P4 (( Display *     , display ),
       ( Window        , window  ),
       ( XContext      , context ),
       ( XEvent *      , event   ))
{
  XcilTextItem *item;

  item = __XcilText_item( display, window, context );

  switch( event->xbutton.button )
    {
    case Button1: /* 左ポジションを入れる */
      __Xcil_store_left( display, window, event, item );
      break;

    case Button3: /* CUT_BUFFER0 にコピーする */
      __Xcil_store_right( display, window, event, item );
      break;
    }

  return 0;
}


static long __Xcil_gets_expose
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( XEvent * , event   ),
       ( long     , pos     ))
{
  XcilTextItem *item;

  item = __XcilText_item( display, window, context );

  __cursor_clear( display, window, item, pos + item->pos );

  expose_hdr( display, window, context, event, item );

  __cursor_on( display, window, item, pos + item->pos );

  return 0;
}



long XcilTextGets
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( char *   , buffer  ),
       ( long     , n       ))
{
  XcilTextItem *item;
  long state;
  long pos, nlog;
  char *log;

  item = __XcilText_item( display, window, context );

  log = item->text[ item->y ];
  pos = ( log ) ? strlen( log ) : 0;
  item->pos = strlen( buffer );
  strcpy( item->log, buffer );
  item->last_key = XcilMouseInWindow( display, window ) ? 0 : -1;

  XcilAddCallback( display, window, context, KeyPress,    (xcil_callback)__Xcil_gets,        (caddr_t)pos );
  XcilAddCallback( display, window, context, ButtonPress, (xcil_callback)__Xcil_cut_buffer,  (caddr_t)pos );
  XcilAddCallback( display, window, context, Expose,      (xcil_callback)__Xcil_gets_expose, (caddr_t)pos );
  XcilAddCallback( display, window, context, EnterNotify, (xcil_callback)enter_hdr,          (caddr_t)pos );
  XcilAddCallback( display, window, context, LeaveNotify, (xcil_callback)leave_hdr,          (caddr_t)pos );

  __Xcil_redraw_line_gets( display, window, item, pos );

  __cursor_on( display, window, item, pos + item->pos );
  state = XcilEventLoop( display, context );
  __cursor_clear( display, window, item, pos + item->pos );

  XcilDelCallback( display, window, context, KeyPress,    (xcil_callback)__Xcil_gets );
  XcilDelCallback( display, window, context, ButtonPress, (xcil_callback)__Xcil_cut_buffer );
  XcilDelCallback( display, window, context, Expose,      (xcil_callback)__Xcil_gets_expose );
  XcilDelCallback( display, window, context, EnterNotify, (xcil_callback)enter_hdr );
  XcilDelCallback( display, window, context, LeaveNotify, (xcil_callback)leave_hdr );

  if ( state & XCIL_ES_KEYIN )
    {
      strcpy( buffer, item->log );
      return 0;
    }

  return -state;
}


long dummy_hdr()
{
  return 0;
}

void XcilTextGetsReady
  _P5 (( Display *    , display  ),
       ( Window       , window   ),
       ( XContext     , context  ),
       ( xcil_callback, callback ),
       ( caddr_t      , value    ))
{
  XcilTextItem *item;
  long state;
  long pos, nlog;
  char *log;

  item = __XcilText_item( display, window, context );

  log = item->text[ item->y ];
  pos = ( log ) ? strlen( log ) : 0;

  item->pos          = 0;
  item->client_value = value;
  item->callback     = ( callback == 0 ) ? (xcil_callback)dummy_hdr : callback;
  item->log[ 0 ] = '\0';
  item->last_key = XcilMouseInWindow( display, window ) ? 0 : -1;

  XcilAddCallback( display, window, context, KeyPress   , (xcil_callback)__Xcil_gets,        (caddr_t)pos );
  XcilAddCallback( display, window, context, ButtonPress, (xcil_callback)__Xcil_cut_buffer,  (caddr_t)pos );
  XcilAddCallback( display, window, context, Expose     , (xcil_callback)__Xcil_gets_expose, (caddr_t)pos );
  XcilAddCallback( display, window, context, EnterNotify, (xcil_callback)enter_hdr,          (caddr_t)pos );
  XcilAddCallback( display, window, context, LeaveNotify, (xcil_callback)leave_hdr,          (caddr_t)pos );
  __cursor_on( display, window, item, pos );
}



long XcilTextGrabGets
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( char *   , buffer  ),
       ( long     , n       ))
{
  XcilTextItem *item;
  long state;
  long pos, nlog;
  char *log;
  static XContext main_context = 0;

  if ( main_context == 0 ) main_context = XUniqueContext();

  item = __XcilText_item( display, window, context );

  log = item->text[ item->y ];
  pos = ( log ) ? strlen( log ) : 0;
  item->pos = strlen( buffer );
  strcpy( item->log, buffer );
  item->last_key = XcilMouseInWindow( display, window ) ? 0 : -1;

  XcilManagedWindow( display, window, main_context );

  XcilAddCallback( display, window, main_context, KeyPress,    (xcil_callback)__Xcil_gets,        (caddr_t)pos );
  XcilAddCallback( display, window, main_context, ButtonPress, (xcil_callback)__Xcil_cut_buffer,  (caddr_t)pos );
  XcilAddCallback( display, window, main_context, Expose,      (xcil_callback)__Xcil_gets_expose, (caddr_t)pos );
  XcilAddCallback( display, window, main_context, EnterNotify, (xcil_callback)enter_hdr,          (caddr_t)pos );
  XcilAddCallback( display, window, main_context, LeaveNotify, (xcil_callback)leave_hdr,          (caddr_t)pos );

  XcilAddWindowContext( display, window, main_context, XCIL_TEXT_ITEM, (caddr_t)item );

  __Xcil_redraw_line_gets( display, window, item, pos );

  __cursor_on( display, window, item, pos + item->pos );
  state = XcilModalLoop( display, main_context, context );
  __cursor_clear( display, window, item, pos + item->pos );

  XcilDelCallback( display, window, main_context, KeyPress,    (xcil_callback)__Xcil_gets );
  XcilDelCallback( display, window, main_context, ButtonPress, (xcil_callback)__Xcil_cut_buffer );
  XcilDelCallback( display, window, main_context, Expose,      (xcil_callback)__Xcil_gets_expose );
  XcilDelCallback( display, window, main_context, EnterNotify, (xcil_callback)enter_hdr );
  XcilDelCallback( display, window, main_context, LeaveNotify, (xcil_callback)leave_hdr );

  XcilUnmanagedWindow( display, window, main_context );

  if ( state & XCIL_ES_KEYIN )
    {
      strcpy( buffer, item->log );
      return strlen( buffer );
    }

  return 0;
}



long XcilTextGetInput
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( char *   , buffer  ),
       ( long     , n       ))
{
  XcilTextItem *item;

  item = __XcilText_item( display, window, context );

  strcpy( buffer, item->log );

  return strlen( buffer );
}



void XcilTextSetInput
  _P4 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( char *   , buffer  ))
{
  XcilTextItem *item;
  char *log;
  long pos;

  item = __XcilText_item( display, window, context );

  strcpy( item->log, buffer );

  log = item->text[ item->y ];
  pos = ( log ) ? strlen( log ) : 0;
  item->pos = strlen( buffer );
  strcpy( item->log, buffer );

  __Xcil_gets_expose( display, window, context, 0, pos );
  __Xcil_redraw_line_gets( display, window, item, pos );
}
