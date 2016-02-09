/*
 * shmls.c
 */



#include "misc/shmemlib.h"
#include <stdio.h>



void print_mode1
  P2 (( shmeminfoRec *, list ),
      ( int           , num  ))
void print_mode2
  P2 (( shmeminfoRec *, list ),
      ( int           , num  ))



void main(argc,argv)
     int argc;
     char *argv[];
{
  shmeminfoRec list[128];
  int i, num, mode;

  mode = 1;
  if ( argc == 2 )
    {
      if ( strcmp( argv[ 1 ], "-a" ) == 0 )
	mode = 2;
      if ( strcmp( argv[ 1 ], "-h" ) == 0 )
	{
	  fprintf(stderr,"Usage : shmls [-a] [-h]\n");
	  exit(0);
	}
    }

  num = shmemgetlist( list );

  switch( mode )
    {
    case 1:
      print_mode1(list,num);
      break;
    case 2:
      print_mode2(list,num);
      break;
    }
}



void print_mode1
  _P2 (( shmeminfoRec *, list ),
       ( int           , num  ))
{
  int i;

  printf("### ID    KEY        OWNER    AT SIZE    CTIME\n");
  for ( i = 0; i < num; i++ )
    {
      shmeminfoRec *info = &list[ i ];
      printf("%3d %-5d 0x%08x %-8s %2d %-7d %s\n",
	     i, info->id, info->key, info->owner, info->attach, info->size,
	     info->ctime );
    }
}



void print_mode2
  _P2 (( shmeminfoRec *, list ),
       ( int           , num  ))
{
  int i;

  printf("## ID    KEY        MODE        OWNER    AT SIZE    CTIME    ATIME    DTIME\n");
  for ( i = 0; i < num; i++ )
    {
      shmeminfoRec *info = &list[ i ];
      printf("%2d %-5d 0x%08x %s %-8s %2d %-7d %8s %8s %8s\n",
	     i, info->id, info->key, info->mode,
	     info->owner, info->attach, info->size,
	     info->ctime, info->atime, info->dtime );
    }
}
