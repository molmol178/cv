/*
 * filename : shmemlib.h
 * author   : Takahiro Sugiyama
 * date     : 1993/09/13
 */



#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shmemlib.h"



/*==================================================================*
 * shmemnew  : $B%-!<$G;XDj$5$l$?%5%$%:$NNN0h$r6&M-%a%b%j$G3NJ]$9$k!#(J
 *==================================================================*/
char *shmemnew /* $B%a%b%j%]%$%s%?(J */
  _P2 (( long, key  ), /* $B%-!<(J   */
       ( long, size )) /* $B%5%$%:(J */
{
  char *ptr;
  long shm_id;

  shm_id = shmget( key, size, IPC_CREAT | 0666 );
  if ( shm_id < 0 )
    {
      perror("error:shmemnew:shmget:");
      return 0;
    }

  ptr = ( char * )shmat( shm_id, 0, 0 );
  if ( ptr == ( char * )-1 )
    {
      perror("error:shmemnew:shmat:");
      return 0;
    }

  return ptr;
}



/*==============================================*
 * shmemfree : $B%-!<$r;}$D6&M-%a%b%j$r3+J|$9$k!#(J
 *==============================================*/
void shmemfree
  _P1 (( long, key ))  /* $B%-!<(J */
{
  long shm_id;

  shm_id = shmget( key, 0, 0666 );
  if ( shm_id < 0 ) return;

  shmctl( shm_id, IPC_RMID, 0 );
}



/*=============================================================*
 * shmemget : $B%-!<$G;XDj$5$l$?6&M-%a%b%j$N%]%$%s%?$r3MF@$9$k!#(J
 *=============================================================*/
char *shmemget
  _P1 (( long, key ))  /* $B%-!<(J */
{
  char *ptr;
  long shm_id;

  shm_id = shmget( key, 0, 0666 );
  if ( shm_id < 0 ) return 0;

  ptr = ( char * )shmat( shm_id, 0, 0 );
  if ( ptr == ( char * )-1 )
    {
      perror( "shmemget:shmat" );
      return 0;
    }

  return ptr;
}



/*============================================================*
 * shmemgetinfo : $B%-!<$G;XDj$5$l$?6&M-%a%b%j$N>pJs$r3MF@$9$k!#(J
 *============================================================*/
long shmemgetinfo /* $B>pJs$,3MF@$G$-$?$+$I$&$+$N??M}CM(J */
  _P2 (( long          , key ),  /* $B%-!<(J       */
       ( shmeminfoRec *, info )) /* $B>pJs$,F~$k(J */
{
  shmeminfoRec list[ 100 ];
  int i, num;

  num = shmemgetlist( list );
  for ( i = 0; i < num; i++ )
    if ( list[ i ].key == key ) break;
  if ( i == num ) return 0;

  *info = list[ i ];

  return 1;
}



/*==============================================*
 * shmemgetlist : $B6&M-%a%b%j$N>pJs$r<h$C$F$/$k!#(J
 *==============================================*/
long shmemgetlist
  _P1 (( shmeminfoRec *, list ))
{
  FILE *fp;
  char buf[ 1024 ];
  char *head[ 64 ];
  int i;

  fp = popen( "ipcs -m -a", "r" );

  i = 0;
  while( fgets( buf, 1024, fp ) )
    {
      if ( buf[ 0 ] == 'm' )
	{
	  long n = strsplit( buf, head, " \t\n" );

	  list[ i ].id = atol( head[ 1 ] );
	  list[ i ].key = strtol( head[ 2 ], 0, 16 );
	  strcpy( list[ i ].mode, head[ 3 ] );
	  strcpy( list[ i ].owner, head[ 4 ] );
	  strcpy( list[ i ].group, head[ 5 ] );
	  list[ i ].attach = atol( head[ 8 ] );
	  list[ i ].size = atol( head[ 9 ] );

	  strcpy( list[ i ].atime, head[ 12 ] );
	  strcpy( list[ i ].dtime, head[ 13 ] );
	  strcpy( list[ i ].ctime, head[ 14 ] );
	  i++;
	}
    }

  pclose( fp );

  return i;
}
