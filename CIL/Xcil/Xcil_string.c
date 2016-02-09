/*
 * filename : Xcil_string.c
 * project  : Xcil Project
 * author   : Takahiro Sugiyama
 * date     : 1993/10/19
 */



#include "Xcil.h"



static long __is_kanji
  _P1 (( register char *, string ))
{
  register int i, n;

  if (( string == 0 ) || ( *string == '\0' )) return 0;

  n = strlen( string );
  for ( i = 0; i < n; i++)
    if ( string[ i ] & 0x80 ) return 1;

  return 0;
}



void XcilPutString
  _P6 (( Display *      , display ),
       ( Window         , window  ),
       ( GC             , gc      ),
       ( long           , x       ),
       ( long	        , y       ),
       ( register char *, string  ))
{
  register long i, j;

  if ( string == 0 || *string == '\0' ) return;

  for ( i = 0; string[ i ]; )
    {
      if ( string[ i ] & 0x80 )
	{/* EUC code */
	  XChar2b c[ 512 ];

	  for ( j = 0; string[ i ] & 0x80; j++, i += 2 )
	    {
	      c[ j ].byte1 = 0177 & string[ i ];
	      c[ j ].byte2 = 0177 & string[ i + 1 ];
	    }

	  XSetFont( display, gc, Xcil_kanji_struct->fid );
	  XDrawString16( display, window, gc, x, y, c, j );
	  x += XTextWidth16( Xcil_kanji_struct, c, j );
	}
      else
	{/* ASCII code */
	  char *c;

	  c = string + i;
	  for ( j = 0; (( c[ j ] != '\0' ) && (! ( c[ j ] & 0x80 ))); j++ ) ;

	  XSetFont( display, gc, Xcil_font_struct->fid );
	  XDrawString( display, window, gc, x, y, c, j );
	  x += XTextWidth( Xcil_font_struct, c, j );
	  i += j;
	}
    }
}



void XcilPutStringBox
  _P9 (( Display *, display  ),
       ( Window   , window   ),
       ( GC	  , gc	     ),
       ( long	  , x	     ),
       ( long	  , y 	     ),
       ( long	  , xsize    ),
       ( long	  , ysize    ),
       ( char *	  , position ),
       ( char *	  , string   ))
{
  XCharStruct over;
  int ascent, descent;
  long str_ysize, str_xsize;
  long ox, oy;

  if ( string == 0 || *string == 0 ) return;

  XcilTextExtents( string, &ascent, &descent, &over );

  str_ysize = ascent + descent;
  str_xsize = over.rbearing + over.lbearing;

  XcilPositionCoord( position, xsize, ysize, str_xsize, str_ysize, &ox, &oy );

  XcilPutString( display, window, gc, ox + x, oy + y + ascent, string );
}



long XcilTextHeight
  _P1 (( char *, string ))
{
  int kanji_height, font_height;

  if ( __is_kanji( string ) )
    kanji_height = Xcil_kanji_struct->ascent + Xcil_kanji_struct->descent;
  else
    kanji_height = 0;

  font_height = Xcil_font_struct->ascent + Xcil_font_struct->descent;

  return ( font_height > kanji_height ) ? font_height : kanji_height;
}



long XcilTextWidth
  _P1 (( char *, string ))
{
  long j, i, width;

  if ( string == 0 || *string == 0 )
    {
      long kanji_width = 0;
      long font_width = 0;

      if ( __is_kanji( string ) )
	kanji_width = Xcil_kanji_struct->max_bounds.width;
      font_width = Xcil_font_struct->max_bounds.width;
      return ( font_width > kanji_width ) ? font_width : kanji_width;
    }

  for ( i = width = 0; string[ i ]; )
    {
      if ( string[ i ] & 0x80 )
	{/* EUC code */
	  XChar2b c[ 512 ];

	  for ( j = 0; string[ i ] & 0x80; j++, i += 2 )
	    {
	      c[ j ].byte1 = 0177 & string[ i ];
	      c[ j ].byte2 = 0177 & string[ i + 1 ];
	    }

	  width += XTextWidth16( Xcil_kanji_struct, c, j );
	}
      else
	{/* ASCII code */
	  char *c;

	  c = string + i;
	  for ( j = 0; (( c[ j ] != '\0' ) && ( ! ( c[ j ] & 0x80 ))); j++ ) ;
	  i += j;
	  width += XTextWidth( Xcil_font_struct, c, j );
	}
    }

  return width;
}



void XcilTextExtents
  _P4 (( char *	      , string	),
       ( int *	      , ascent	),
       ( int *	      , descent ),
       ( XCharStruct *, over	))
{
  long i, j;
  int directionR,ascentR,descentR;
  XCharStruct overR;

  int ldirection,lascent,ldescent;
  XCharStruct lover;

  long first = 1;

  if ( ascent  == 0 ) ascent  = &ascentR;
  if ( descent == 0 ) descent = &descentR;
  if ( over    == 0 ) over    = &overR;

  over->lbearing = 0;
  over->rbearing = 0;
  over->width = 0;
  over->ascent = 0;
  over->descent = 0;

  *ascent  = Xcil_font_struct->ascent;
  *descent = Xcil_font_struct->descent;

  if ( *ascent < Xcil_kanji_struct->ascent )
    *ascent = Xcil_kanji_struct->ascent;
  if ( *descent < Xcil_kanji_struct->descent )
    *descent = Xcil_kanji_struct->descent;

  if ( string == 0 || *string == 0 ) return;

  for ( i = 0; string[ i ]; )
    {
      if ( string[ i ] & 0x80 )
	{/* EUC code */
	  XChar2b c[ 512 ];

	  for ( j = 0; string[ i ] & 0x80; j++, i += 2 )
	    {
	      c[ j ].byte1 = 0177 & string[ i ];
	      c[ j ].byte2 = 0177 & string[ i + 1 ];
	    }

	  XTextExtents16( Xcil_kanji_struct, c, j,
			  &ldirection, &lascent, &ldescent, &lover );
	  over->width += lover.width;
	  if ( first ) { over->lbearing = lover.lbearing; first = 0; }
	  if ( over->ascent < lover.ascent ) over->ascent = lover.ascent;
	  if ( over->descent < lover.descent ) over->descent = lover.descent;
	}
      else
	{/* ASCII code */
	  char *c;

	  c = string + i;
	  for ( j = 0; (( c[ j ] != '\0' ) && ( ! ( c[ j ] & 0x80 ))); j++ ) ;
	  i += j;

	  XTextExtents( Xcil_font_struct, c, j,
		        &ldirection, &lascent, &ldescent, &lover );
	  over->width += lover.width;
	  if ( first ) { over->lbearing = lover.lbearing; first = 0; }
	  if ( over->ascent < lover.ascent ) over->ascent = lover.ascent;
	  if ( over->descent < lover.descent ) over->descent = lover.descent;
	}
    }

  over->rbearing = over->width + lover.rbearing - lover.width;
}



void XcilPutStringDeco
  _P8 (( Display *, d	     ),
       ( Window   , w	     ),
       ( GC	  , body_gc  ),
       ( GC	  , frame_gc ),
       ( long	  , x	     ),
       ( long	  , y 	     ),
       ( char *   , deco     ),
       ( char *	  , string   ))
{
  int j, i, n;
  static struct {
    int x, y;
  } p[] = {{ -1,  1 }, {  0,  1 }, {  1,  1 },
	   { -1,  0 }, {  0,  0 }, {  1,  0 },
	   { -1, -1 }, {  0, -1 }, {  1, -1 }};

  if ( deco ) n = strlen( deco ); else n = 0;
  for ( i = 0; i < n; i ++ )
    {
      j = deco[ i ] - '1';
      XcilPutString( d, w, frame_gc, x + p[j].x, y + p[j].y, string );
    }
  XcilPutString( d, w, body_gc, x, y, string );
}



void XcilPutStringBoxDeco
  _P11 (( Display *, d        ),
	( Window   , w        ),
	( GC	   , body_gc  ),
	( GC	   , frame_gc ),
	( long	   , x        ),
	( long	   , y        ),
	( long	   , xsize    ),
	( long	   , ysize    ),
	( char *   , position ),
	( char *   , deco     ),
	( char *   , string   ))
{
  long rx, ry;
  long string_xsize;
  long string_ysize;

  int ascent, descent;
  XCharStruct over;

  if ( string == 0 || *string == 0 ) return;

  XcilTextExtents( string, &ascent, &descent, &over );

  string_xsize = XcilTextWidth( string );
  string_ysize = XcilTextHeight( string );

  XcilPositionCoord( position, xsize, ysize,
		     string_xsize, string_ysize, &rx, &ry );

  XcilPutStringDeco( d, w, body_gc, frame_gc,
		     x + rx, y + ry + Xcil_font_struct->ascent,
		     deco, string );
  XFlush( d );
}



void XcilTextSize
  _P3 (( char **, text  ),
       ( long * , xsize ),
       ( long * , ysize ))
{
  int i;
  long max_xsize, max_ysize, font_height;

  max_xsize = max_ysize = 0;
  for ( i = 0; text[i]; i++ )
    {
      int width = XcilTextWidth( text[ i ] );
      if ( max_xsize < width ) max_xsize = width;
    }
  font_height = XcilTextHeight( 0 );
  max_ysize = ( font_height + 2 ) * i + font_height;
  max_xsize += font_height;

  *xsize = max_xsize;
  *ysize = max_ysize;
}
