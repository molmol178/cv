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

  ciladd( self->name );
  info = cilinfo( self->name );
  filename = info->filename;
  if ( opt == 0 ) opt = "";

  if ( ! IS_SHARED( self ) ) Image.save( self, filename, 0 );

  sprintf( command, "imagedisp -k %s %s &", self->name, opt );
  info->imagedispexec = 0;
  system( command );
  while ( info->imagedispexec == 0 ) ;
  while ( ! ImageDispOK( self ) ) ;
  fprintf( stderr, "ImageDisp:run \"%s\"\n", command );
}



long ImageDispOK
  _P1 (( image, self ))
{
  long fd, type, state;
  cilRec *info;

  type  = XCIL_OK;
  state = 0;
  info = cilinfo( self->name );
  if ( info == 0 ) return 0;

  fd = socketconnect( info->socketname );
  if ( fd < 0 ) return 0;

  state += socketwrite( fd, ( char * )&type, sizeof( long ) );
  state += socketread( fd, ( char * )&type, sizeof( long ) );
  socketclose( fd );

  if ( state == 8 ) return 1;

  return 0;
}



void ImageDispQuit
  _P1 (( image, self ))
{
  long fd, type;
  cilRec *info;

  type  = XCIL_QUIT;
  info = cilinfo( self->name );
  if ( info == 0 ) return;
  fd = socketconnect( info->socketname );
  if ( fd < 0 ) return;

  socketwrite( fd, ( char * )&type, 4 );
  socketclose( fd );
}



void ImageDispDraw
  _P1 (( image, self ))
{
  long fd, type;
  cilRec *info;

  if ( Image.area( self ) == 0 ) return;

  info = cilinfo( self->name );
  if ( info == 0 )
    {
      ImageDispExec( self, "" );
      return;
    }

  if ( ! IS_SHARED( self ) ) Image.save( self, info->filename, 0 );

  type = XCIL_DRAW;
  fd = socketconnect( info->socketname );
  if ( fd < 0 )
    {
      ImageDispExec( self, "" );
      return;
    }

  if ( fd < 0 ) return;
  socketwrite( fd, ( char * )&type, 4 );

  socketclose( fd );
}



long ImageDispSelectEvent
  _P2 (( image, self ),
       ( long , mask ))
{
  long fd, type;
  cilRec *info;

  while ( ImageDispOK( self ) != 1 ) ;

  if (DEBUG>2) fprintf(stderr,"(*setmask(%d)",mask);

  type = XCIL_SELECT_EVENT;
  info = cilinfo( self->name );
  if ( info == 0 )
    {
      fprintf(stderr,"ImageDispSelectEvent:no information\n");
      return 0;
    }

  info->event.type = NoEvent;

  fd = socketconnect( info->socketname );
  if ( fd < 0 )
    {
      fprintf(stderr,"ImageDispSelectEvent:not yet connect\n");
      return -1;
    }

  socketwrite( fd, ( char * )&type, sizeof( long ) );
  socketwrite( fd, ( char * )&mask, sizeof( long ) );
  socketread( fd, ( char * )&mask, sizeof( long ) );
  socketclose( fd );
  if (DEBUG>2) fprintf(stderr,"endsetmask*)\n");

  return 1;
}



long ImageDispNextEvent
  _P2 (( image     , self  ),
       ( CILEvent *, event ))
{
  cilRec *info;

  info = cilinfo( self->name );
  if ( info == 0 ) return 0;

  while ( info->event.type == NoEvent ) ;
  *event = info->event;
  info->event.type = NoEvent;

  return 1;
}



long ImageDispCheckEvent
  _P2 (( image     , self  ),
       ( CILEvent *, event ))
{
  cilRec *info;

  info = cilinfo( self->name );
  if ( info == 0 ) return 0;

  *event = info->event;
  info->event.type = NoEvent;

  return ( event->type != NoEvent );
}
