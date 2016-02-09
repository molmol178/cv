/*
 * filename : cilclient.c
 * author   : Takahiro Sugiyama
 * date     : 1993/10/03
 */



#include "cilserver.h"
#include "misc/typelib.h"
#include "misc/socketlib.h"
#include <stdio.h>



static cilRec *cil_list = 0;
static long cil_attach = 0;



static void __cil_make_keyname
  _P2 (( char *, keyname ),
       ( char *, name    ))
{
  int i;

  if ( name[0] == '@' )
    {
      strncpy( keyname, name, NAME_MAX );
      keyname[ NAME_MAX - 1 ] = '\0';
      return;
    }

  for ( i = strlen( name ) - 1; i >= 0; i-- )
    if ( name[i] == '/' ) break;
  if ( i < 0 )
    {
      strncpy( keyname, name, NAME_MAX );
      keyname[ NAME_MAX - 1 ] = '\0';
      return;
    }

  strncpy( keyname, name + i + 1, NAME_MAX );
  keyname[ NAME_MAX - 1 ] = '\0';
}



cilRec *cil_openlist
  _P0 (void)
{
  long key, id, i;

  if ( cil_list != 0 ) return cil_list;

  strncpy( ( char * )&key, "NM", 2 );
  key = 'N' * 0x10000 + 'M' * 0x100 + getuid();
  id = shmget( key, 0, 0666 );
  if ( id < 0 )
    {
      perror( "cil_openlist:shmget" );
      fprintf( stderr, "please execute \"cilserver\"\n" );
      fprintf( stderr, "sorry ... bye\n" );
      exit( -1 );
    }

  cil_list = ( cilRec * )shmat( id, 0, 0 );
  if ( cil_list == ( cilRec * )-1 )
    {
      perror( "cil_openlist:shmat" );
      exit( -1 );
    }

  return cil_list;
}



void cil_closelist
  _P0 (void)
{
  if ( cil_list == 0 ) return;
  if ( cil_attach > 0 ) return;
  if ( shmdt( cil_list ) == -1 ) perror( "cil_closelist:shmdt" );
  cil_list = 0;
  cil_attach = 0;
}


#define DEBUG 0
cilRec *cil_open
  _P1 (( char *, name ))
{
  int i;
  cilRec *list;
  char keyname[16];

  list = cil_openlist();
  __cil_make_keyname( keyname, name );
  for ( i = 0; i < CIL_LIST_NUM; i++ )
    {
      cilRec *info = &list[i];
      if ( strcmp( keyname, info->name ) == 0 )
	{
	  cil_attach ++;
	  return info;
	}
    }
  if ( DEBUG > 0 ) fprintf(stderr,"cil_open:not found key %s (%s)\n", keyname,name);
  cil_closelist();

  return 0;
}



void cil_close
  _P1 (( cilRec *, info ))
{
  if ( info ) cil_attach --;
  if ( cil_attach <= 0 ) cil_closelist();
}



cilRec *cil_create
  _P1 (( char *, name ))
{
  long fd;
  char servername[64];
  long type;
  long state;
  char keyname[16];

  __cil_make_keyname( keyname, name );

  sprintf( servername, "/tmp/CIL%d", getuid() );
  fd = socketconnect( servername );
  if ( fd < 0 )
    {
      fprintf(stderr,"server not responding...\n");
      return 0;
    }
  type = CIL_ADD;

  socketwrite( fd, ( char * )&type, sizeof( long ) );
  socketwrite( fd, keyname, NAME_MAX );
  socketread( fd, ( char * )&state, sizeof( long ) );
  socketclose( fd );

  return cil_open( name );
}



long cil_unlink
  _P1 (( char *, name ))
{
  long fd;
  char servername[64];
  long type;
  long state;
  char keyname[16];

  __cil_make_keyname( keyname, name );

  sprintf( servername, "/tmp/CIL%d", getuid() );

  fd = socketconnect( servername );
  if ( fd < 0 ) return -1;
  type = CIL_DEL;

  socketwrite( fd, ( char * )&type, sizeof( long ) );
  socketwrite( fd, keyname, NAME_MAX );
  socketread( fd, ( char * )&state, sizeof( long ) );
  socketclose( fd );

  return state;
}



cilRec *cil_dopen
  _P1 (( long, d ))
{
  cilRec *list;
  cilRec *info;

  if ( d < 0 || d >= CIL_LIST_NUM )
    {
      fprintf( stderr, "cil_dopen:index over %d\n", d );
      return 0;
    }

  list = cil_openlist();
  if ( list[ d ].name[ 0 ] != '\0' )
    {
      cil_attach ++;
      info = &list[ d ];
    }
  cil_closelist();

  return info;
}



#define DEBUG 0

long cil_xsend
  _P3 (( char *, name ),
       ( long  , type ),
       ( long *, data ))
{
  long i;
  long fd;
  char servername[64];
  long state;
  char keyname[16];
  cilRec *info;
  long dataR[CIL_DATA_NUM];

  __cil_make_keyname( keyname, name );

  sprintf( servername, "/tmp/XCIL%d", getuid() );

  if ( DEBUG > 1 ) fprintf(stderr,"cil_xsend:%s,%d\n",name,type);

  fd = socketconnect( servername );
  if ( fd < 0 )
    {
      fprintf(stderr,"server not responding...\n");
      return -1;
    }

  info = cil_open( name );
  info->command = type;
  if ( data )
    for ( i = 0; i < CIL_DATA_NUM; i++ )
      info->data[i] = data[i];

  socketwrite( fd, ( char * )&type, sizeof( long ) );
  socketwrite( fd, keyname, NAME_MAX );
  socketclose( fd );

  return 0;
}
