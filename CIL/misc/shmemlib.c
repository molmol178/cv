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
 * shmemnew  : キーで指定されたサイズの領域を共有メモリで確保する。
 *==================================================================*/
char *shmemnew /* メモリポインタ */
  _P2 (( long, key  ), /* キー   */
       ( long, size )) /* サイズ */
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
 * shmemfree : キーを持つ共有メモリを開放する。
 *==============================================*/
void shmemfree
  _P1 (( long, key ))  /* キー */
{
  long shm_id;

  shm_id = shmget( key, 0, 0666 );
  if ( shm_id < 0 ) return;

  shmctl( shm_id, IPC_RMID, 0 );
}



/*=============================================================*
 * shmemget : キーで指定された共有メモリのポインタを獲得する。
 *=============================================================*/
char *shmemget
  _P1 (( long, key ))  /* キー */
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
 * shmemgetinfo : キーで指定された共有メモリの情報を獲得する。
 *============================================================*/
long shmemgetinfo /* 情報が獲得できたかどうかの真理値 */
  _P2 (( long          , key ),  /* キー       */
       ( shmeminfoRec *, info )) /* 情報が入る */
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
 * shmemgetlist : 共有メモリの情報を取ってくる。
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
