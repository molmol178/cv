/*
 * filename : ImageDisp.c
 * author   : Takahiro Sugiyama
 * date     : 1993/10/05
 */



#include <stdio.h>
#include "ImageDisp.h"
#include "misc/socketlib.h"
#include "server/cilserver.h"



#define DEBUG 0

long ImageDispExec
  _P2 (( image , self ),
       ( char *, opt  ))
{
  char command[ 512 ];
  char *filename;
  cilRec *info;

  if ( Image.area( self ) == 0 ) return;

  if ( ImageDispOK( self ) ) return;

  info = cil_create( self->name );
  if ( info == 0 ) return 0;
  filename = info->filename;
  if ( opt == 0 ) opt = "";

  if ( ! IS_SHARED( self ) ) Image.save( self, filename, 0 );

  sprintf( command, "imagedisp -k %s %s &", self->name, opt );
  info->imagedispexec = 0;
  info->id = 0;
  system( command );
  while ( info->imagedispexec == 0 ) ;
  cil_close( info );

  fprintf( stderr, "ImageDisp:run \"%s\"\n", command );
}



long ImageDispOK
  _P1 (( image, self ))
{
  cilRec *info;
  long state = 0;

  info = cil_open( self->name );
  if ( info ) state = info->imagedispexec;
  cil_close( info );

  return state;
}



void ImageDispQuit
  _P1 (( image, self ))
{
  cilRec *info;

  info = cil_open( self->name );
  if ( info ) cil_xsend( info, CIL_QUIT, 0 );
  cil_close( info );
}



void ImageDispDraw
  _P1 (( image, self ))
{
  long fd, type;
  cilRec *info;

  if ( Image.area( self ) == 0 ) return;

  info = cil_open( self->name );
  if ( info == 0 )
    {
      ImageDispExec( self, "" );
      return;
    }

  if ( ! IS_SHARED( self ) ) Image.save( self, info->filename, 0 );

  cil_xsend( info, CIL_DRAW, 0 );
  cil_close( info );
}



void ImageDispDrawArea
  _P5 (( image, self  ),
       ( long , x     ),
       ( long , y     ),
       ( long , xsize ),
       ( long , ysize ))
{
  long fd, type;
  cilRec *info;
  long data[CIL_DATA_NUM];

  if ( Image.area( self ) == 0 ) return;

  info = cil_open( self->name );
  if ( info == 0 ) return;

  if ( ! IS_SHARED( self ) ) Image.save( self, info->filename, 0 );

  data[ 0 ] = x;
  data[ 1 ] = y;
  data[ 2 ] = xsize;
  data[ 3 ] = ysize;

  cil_xsend( info, CIL_DRAW_AREA, data );
  cil_close( info );
}



void ImageDispSelectEvent
  _P2 (( image, self ),
       ( long , mask ))
{
  cilRec *info;

  info = cil_open( self->name );
  if ( info )
    {
      info->event.type = NoEvent;
      info->mask = mask;
    }
  cil_close( info );
}



long ImageDispNextEvent
  _P2 (( image     , self  ),
       ( CILEvent *, event ))
{
  cilRec *info;

  info = cil_open( self->name );
  if ( info == 0 ) return 0;

  while ( info->event.type == NoEvent ) ;
  *event = info->event;
  info->event.type = NoEvent;
  cil_close( info );

  return 1;
}



long ImageDispCheckEvent
  _P2 (( image     , self  ),
       ( CILEvent *, event ))
{
  cilRec *info;

  info = cil_open( self->name );
  if ( info == 0 ) return 0;

  *event = info->event;
  info->event.type = NoEvent;
  cil_close( info );

  return ( event->type != NoEvent );
}

