/*
 * filename : cilserver.c
 * author   : Takahiro Sugiyama
 * date     : 1993/10/03
 */



#include "cilserver.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#include "ImageDisp.h"
#include "misc/shmemlib.h"
#include "misc/socketlib.h"



static void ciltest_server
  P1 (( long, fd  ))

static void cilexit_server
  P1 (( long, fd  ))

static void cilfree_server
  P1 (( long, fd  ))

static void ciladd_server
  P1 (( long, fd  ))

static void cildel_server
  P1 (( long, fd  ))

static void cildraw_server
  P1 (( long, fd  ))

static void cilquit_server
  P1 (( long, fd  ))

static void cilok_server
  P1 (( long, fd  ))

static void xcil_server
  P2 (( long, fd   ),
      ( long, type ))


static cilRec *cil_list = 0;
static long server;
static long xserver;
static char servername[64];
static char xservername[64];



static void __cilinit
  P0 (void)

static void __cilfree
  P0 (void)

static cilRec *__find_cil
  P1 (( char * , name ))

static cilRec *__find_free
  P0 (void)

static char *__cil_keyname
  P1 (( char *, fullname ))

static cilRec *__cil_create
  P0 (void)

static cilRec *__cil_list
  P0 (void)

static void __cil_clear
  P1 (( cilRec *, list ))

static void __cil_delete
  P1 (( cilRec *, info ))

static long __cil_xsend
  P2 (( cilRec *, info ),
      ( long    , type ))



static void cilservertest()
{
  long fd;
  long type;

  type = CIL_TEST;

  fd = socketconnect( servername );
  if ( fd >= 0 )
    {
      socketwrite( fd, (char *)&type, sizeof( long ) );
      socketclose( fd );
      fprintf( stderr, "CIL-SERVER may be running.\n" );
      exit( 1 );
    }
  fprintf( stderr, "CIL-SERVER is starting.\n" );

  fd = socketconnect( xservername );
  if ( fd >= 0 )
    {
      socketwrite( fd, (char *)&type, sizeof( long ) );
      socketclose( fd );
      fprintf( stderr, "XCIL-SERVER may be running.\n" );
      exit( 1 );
    }
  fprintf( stderr, "XCIL-SERVER is starting.\n" );

  if ( fork() != 0 ) exit( 0 );
}



static void cilserverexit
  _P0 (void)
{
  long fd;

  fd = socketconnect( servername );
  if ( fd >= 0 )
    {
      long type = CIL_EXIT;
      fprintf( stderr, "CIL-SERVER exit.\n" );
      socketwrite( fd, (char *)&type, sizeof( long ) );
      socketclose( fd );
    }
  else
    {
      fprintf( stderr, "CIL-SERVER wasn't running.\n" );
    }

  fd = socketconnect( xservername );
  if ( fd >= 0 )
    {
      long type = CIL_EXIT;
      fprintf( stderr, "XCIL-SERVER exit.\n" );
      socketwrite( fd, (char *)&type, sizeof( long ) );
      socketclose( fd );
    }
  else
    {
      fprintf( stderr, "XCIL-SERVER wasn't running.\n" );
    }

  exit( 1 );
}



static long __cilkey()
{
  long key = 0;

  strncpy( ( char * )&key, "NM", 2 );
  key = 'N' * 0x10000 + 'M' * 0x100 + getuid();

  return key;
}



static void __cilinit()
{
  long i;
  long doinit = 0;
  cilRec *list;

  list = __cil_list();
  if ( list == 0 )
    {
      doinit = 1;
      list = __cil_create();
    }

  if ( doinit ) __cil_clear( list );
}



static void cilserverclear()
{
  __cil_clear( cil_openlist() );
  exit( 0 );
}



static void cilserverfree()
{
  __cilfree();
  cilserverexit();
}



static void cilserverhelp()
{
  fprintf( stderr, "Usage : cilserver [-uid <uid>] [-r] [-c] [-h] [-f]\n" );
  fprintf( stderr, "        -h   : print this messaged\n" );
  fprintf( stderr, "        -uid : user ID\n" );
  fprintf( stderr, "        -r   : reset server\n" );
  fprintf( stderr, "        -c   : clear cil table\n" );
  fprintf( stderr, "        -f   : free cil table\n" );
}



/*----------------------*
 * サーバのソケットＦＤ
 *----------------------*/
int main( argc, argv )
     long argc;
     char **argv;
{
  int i;

  sprintf( servername, "/tmp/CIL%d", getuid() );
  sprintf( xservername, "/tmp/XCIL%d", getuid() );

  for ( i = 1; i < argc; i++ )
    {
      if ( strcmp( argv[i], "-uid" ) == 0 )
	if ( ++i < argc )
	  {
	    sprintf( servername,  "/tmp/CIL%s", argv[i] );
	    sprintf( xservername, "/tmp/XCIL%s", argv[i] );
	  }

      if ( strcmp( argv[i], "-r" ) == 0 )
	cilserverexit();
      if ( strcmp( argv[i], "-c" ) == 0 )
	cilserverclear();
      if ( strcmp( argv[i], "-h" ) == 0 )
	cilserverhelp();
      if ( strcmp( argv[i], "-f" ) == 0 )
	cilserverfree();
    }

  cilservertest();

  __cilinit();

  server = socketinit( servername );
  xserver = socketinit( xservername );

  while ( 1 )
    {
      long client;
      long type;
      fd_set mask;

      socketfdzero( mask );
      socketfdset( mask, server );
      socketfdset( mask, xserver );
      if ( socketselect( xserver + 1, &mask, 0.0 ) <= 0 ) continue;

      if ( socketready( mask, server ) )
	{
	  client = socketaccept( server );
	  if ( client < 0 )
	    {
	      fprintf( stderr, "cilserver:can't accept client\n" );
	      exit( -1 );
	    }
	  socketread( client, ( char * )&type, sizeof( long ) );

	  switch ( type )
	    {
	    case CIL_ADD:
	      ciladd_server( client ); break;
	    case CIL_DEL:
	      cildel_server( client ); break;
	    case CIL_TEST:
	      ciltest_server( client ); break;
	    case CIL_EXIT:
	      cilexit_server( client ); break;
	    case CIL_FREE:
	      cilfree_server( client ); break;
	    default:
	      fprintf( stderr, "unknown message type(%d)\n", type );break;
	    }
	  socketclose( client );
	}

      if ( socketready( mask, xserver ) )
	{
	  client = socketaccept( xserver );
	  if ( client < 0 )
	    {
	      fprintf( stderr, "cilserver:can't accept client\n" );
	      exit( -1 );
	    }
	  socketread( client, ( char * )&type, sizeof( long ) );
	  xcil_server( client, type );
	  socketclose( client );
	}
    }
}



static void ciltest_server
  _P1 (( long, fd  ))
{
}



static void cilexit_server
  _P1 (( long, fd  ))
{
  socketclose( fd );
  socketexit( servername, server );
  socketexit( xservername, xserver );

  fprintf(stderr,"bye\n");
  exit( 1 );
}



static void cilfree_server
  _P1 (( long, fd  ))
{
  __cilfree();
  cilexit_server( fd );
}



static void ciladd_server
  _P1 (( long, fd  ))
{
  char name[NAME_MAX];
  cilRec *info;
  long uid;
  char *display;
  char *keyname;
  long state = 1;

  socketread( fd, name, NAME_MAX );
  keyname = __cil_keyname( name );

  info = __find_cil( keyname );
  if ( info == 0 )
    {
      info = __find_free( );
      if ( info == 0 )
	{
	  state = 0;
	  socketwrite( fd, ( char * )&state, sizeof( long ) );
	  return;
	}
      info->imagedispexec = 0;
      info->share = 0;
      info->mask = 0;
      info->id = 0;
      info->command = 0;
    }

  uid = getuid();
  display = getenv( "DISPLAY" );
  if ( display == 0 ) display = ":0.0";

  strcpy( info->name, keyname );
  if ( name[0] == '@' )
    strcpy( info->filename, keyname );
  else
    sprintf( info->filename, "/tmp/#%d%s#", uid, keyname );

  strcpy( info->displayname, display );

  socketwrite( fd, ( char * )&state, sizeof( long ) );
}



static void cildel_server
  _P1 (( long, fd  ))
{
  char name[NAME_MAX];
  cilRec *info;
  char *keyname;
  long state = 1;

  socketread( fd, name, NAME_MAX );
  keyname = __cil_keyname( name );

  info = __find_cil( keyname );
  if ( info == 0 )
    {
      state = 0;
      socketwrite( fd, ( char * )&state, sizeof( long ) );
      return;
    }
  __cil_delete( info );

  socketwrite( fd, ( char * )&state, sizeof( long ) );
}



static cilRec *__find_cil
  _P1 (( char * , name ))
{
  long i;

  for ( i = 0; i < CIL_LIST_NUM; i++)
    {
      cilRec *info = &cil_list[i];
      if ( strcmp( name, info->name ) == 0 )
	return info;
    }

  return 0;
}



static cilRec *__find_free()
{
  long i;

  for ( i = 0; i < CIL_LIST_NUM; i++ )
    {
      cilRec *info = &cil_list[i];
      if ( info->name[0] == '\0' )
	return info;
    }

  return 0;
}



static char *__cil_keyname
  _P1 (( char *, fullname ))
{
  int i;

  if ( fullname[0] == '@' ) return fullname;
  fullname[ NAME_MAX - 1 ] = '\0';

  for ( i = strlen( fullname ) - 1; i >= 0; i-- )
    if ( fullname[i] == '/' ) break;
  if ( i < 0 ) return fullname;

  return fullname + i + 1;
}



static cilRec *__cil_list()
{
  long key;
  long id;

  if ( cil_list ) return cil_list;

  key = __cilkey();
  id = shmget( key, 0, 0666 );
  if ( id < 0 ) return 0;

  cil_list = ( cilRec * )shmat( id, 0, 0 );
  if ( cil_list == ( cilRec * )-1 ) return 0;

  return cil_list;
}



static cilRec *__cil_create()
{
  long id, key;

  key = __cilkey();
  id = shmget( key, sizeof( cilRec ) * CIL_LIST_NUM, IPC_CREAT | 0666 );
  if ( id < 0 )
    {
      perror( "__cil_create:shmget" );
      exit( -1 );
    }

  cil_list = ( cilRec * )shmat( id, 0, 0 );
  if ( cil_list == ( cilRec * )-1 )
    {
      perror( "__cil_create:shmat" );
      exit( -1 );
    }

  return cil_list;
}



static void __cilfree()
{
  long key;
  long id;

  key = __cilkey();
  id = shmget( key, 0, 0666 );
  if ( id < 0 ) return;

  shmctl( id, IPC_RMID, 0 );
}



static void __cil_clear
  _P1 (( cilRec *, list ))
{
  int i;
  long uid = getuid();

  if ( list == 0 ) return;

  for ( i = 0; i < CIL_LIST_NUM; i++ )
    {
      cilRec *info = &list[i];

      info->key = i + uid * 256 + 1000;

      __cil_delete( info );
    }
}



static void __cil_delete
  _P1 (( cilRec *, info ))
{
  if ( info == 0 ) return;
  if ( info->name[ 0 ] == '\0' ) return;

  if (( info->share == 1 ) || ( info->name[ 0 ] == '@' ))
    {
      shmemfree( info->key );
    }
  if (( info->imagedispexec == 1 ) && ( info->id ))
    {
      info->command = CIL_QUIT;
      __cil_xsend( info, CIL_QUIT );
    }
  info->name[ 0 ] = '\0';
}



static void xcil_server
  _P2 (( long, fd   ),
       ( long, type ))
{
  char name[NAME_MAX];
  cilRec *info;
  char *keyname;

  socketread( fd, name, NAME_MAX );
  keyname = __cil_keyname( name );

  info = __find_cil( keyname );
  if ( info == 0 ) return;

  __cil_xsend( info, type );
}



static cilRec *err_info = 0;
static int xcil_error
  _P2 (( Display *    , d   ),
       ( XErrorEvent *, err ))
{
  char err_msg[80];

  XGetErrorText( d, err->error_code, err_msg, 80 );

  fprintf( stderr, "XError:%s:Now Recovering ...\n", err_msg );
  err_info->id = 0;
  err_info->imagedispexec = 0;
  return 0;
}

#define DEBUG 0

static long __cil_xsend
  _P2 (( cilRec *, info ),
       ( long    , type ))
{
  XEvent event;
  Display *display;

  XSetErrorHandler( xcil_error );

  if ( DEBUG > 1 ) fprintf(stderr,"__cil_xsend:%d\n",type);

  if ( info->id == 0 ) return 0;

  display = XOpenDisplay(getenv("DISPLAY"));
  if ( display == 0 )
    {
      fprintf(stderr,"can't open display\n");
      return 0;
    }

  event.type = ClientMessage;
  event.xany.serial = 0;
  event.xany.send_event = True;
  event.xany.display = display;
  event.xclient.window = info->id;
  event.xclient.message_type = 0;
  event.xclient.format = 32;
  event.xclient.data.l[0] = 0;

  info->id = 0;
  err_info = info;
  XSendEvent( display, event.xclient.window, False, 0, &event );
  XSync( display, True );

  XCloseDisplay( display );

  return 1;
}
