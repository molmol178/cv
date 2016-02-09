/*
 * filename : cildel.c
 * author   : Takahiro Sugiyama
 * date     : 1993/10/03
 */



#include "cilserver.h"
#include <stdio.h>



long cil_unlink_more
  P1 (( char *, name ))



int main(argc,argv)
     int argc;
     char *argv[];
{
  int i;

  if ( argc == 1 )
    {
      fprintf( stderr, "Usage : cilrm <name> ...\n");
      fprintf( stderr, "        cilrm <id> ...\n");
      fprintf( stderr, "        cilrm all\n");
      exit( 1 );
    }

  if ( ( argc == 2 ) && ( strcmp( argv[ 1 ], "all" ) == 0 ) )
    {
      cilRec *list = cil_openlist();

      for ( i = 0; i < CIL_LIST_NUM; i++ )
	if ( list[ i ].name[ 0 ] )
	  if ( cil_unlink( list[ i ].name ) == 0 )
	    fprintf( stderr, "deleted #%d\n", i );

      cil_closelist();

      return 0;
    }

  for ( i = 1; i < argc; i++ )
    {
      long state = cil_unlink_more( argv[ i ] );
      switch ( state )
	{
	case -1:
	  fprintf( stderr,"cilrm:server is not alive.\n" );
	  break;
	case 1:
	  fprintf( stderr, "deleted %s\n", argv[ i ] );
	  break;
	}
    }

  return 0;
}



long cil_unlink_more
  _P1 (( char *, name ))
{
  long state;
  cilRec *info;

  state = cil_unlink( name );
  if ( ( state == 0 ) && strisdigit( name ) )
    {
      info = cil_dopen( atol( name ) );
      if ( info ) state = cil_unlink( info->name );
      cil_close( info );
      fprintf(stderr,"specified by ID #%d\n",atol(name));
    }

  return state;
}
