/*
 * shmls.c
 */



#include "misc/shmemlib.h"
#include <stdio.h>
#include <stdlib.h>



void remove_mode0
  P4 (( shmeminfoRec *, list ),
      ( int           , num  ),
      ( char **       , argv ),
      ( int           , argc ))

void remove_mode1
  P4 (( shmeminfoRec *, list ),
      ( int           , num  ),
      ( char **       , argv ),
      ( int           , argc ))

void remove_mode2
  P2 (( shmeminfoRec *, list ),
      ( int           , num  ))


void print_man()
{
  fprintf(stderr,"Usage : shmrm #1 #2 ...\n");
  fprintf(stderr,"        shmrm -id <IDs> ...\n");
  fprintf(stderr,"        shmrm all\n");
  exit( 0 );
}



void main(argc,argv)
     int argc;
     char *argv[];
{
  shmeminfoRec list[128];
  int i, num, mode;

  if ( argc == 1 ) print_man();

  mode = 0;
  if (( argc >= 2 ) && ( strcmp( argv[ 1 ], "-id" ) == 0 )) mode = 1;
  if (( argc >= 2 ) && ( strcmp( argv[ 1 ], "all" ) == 0 )) mode = 2;

  num = shmemgetlist( list );

  switch( mode )
    {
    case 0:
      remove_mode0(list,num,argv,argc);
      break;
    case 1:
      remove_mode1(list,num,argv,argc);
      break;
    case 2:
      remove_mode2(list,num);
      break;
    }
}



void remove_mode0
  _P4 (( shmeminfoRec *, list ),
       ( int           , num  ),
       ( char **       , argv ),
       ( int           , argc ))
{
  int i, no;
  shmeminfoRec *info;

  for ( i = 1; i < argc; i++ )
    {
      no = atol( argv[ i ] );
      if ( ( no < 0 ) || ( no >= num ) ) continue;
      info = &list[ no ];
      shmemfree( info->key );
      printf("removed %d\n",info->id);
    }
}



void remove_mode1
  _P4 (( shmeminfoRec *, list ),
       ( int           , num  ),
       ( char **       , argv ),
       ( int           , argc ))
{
  int i, id, no;
  shmeminfoRec *info;

  for ( i = 2; i < argc; i++ )
    {
      id = atol( argv[ i ] );
      for ( no = 0; no < num; no++ )
	{
	  info = &list[ no ];
	  if ( info->id == id ) break;
	}
      if ( no == num ) continue;
      shmemfree( info->key );
      printf("removed %d\n",info->id);
    }
}



void remove_mode2
  _P2 (( shmeminfoRec *, list ),
       ( int           , num  ))
{
  int i;
  shmeminfoRec *info;
  char *user = getenv("USER");

  for ( i = 0; i < num; i++ )
    {
      info = &list[ i ];
      if ( strcmp( user, info->owner ) == 0 )
	{
	  shmemfree( info->key );
	  printf("removed %d\n",info->id);
	}
    }
}

