/*
 * strings histroy
 */



#include "Xcil.h"
#include <string.h>



#define MAX_HISTORY 128
static long  history_offset = 0;
static long  history_last   = 0;
static long  history_num    = 0;
static char *history_list[MAX_HISTORY] = {0};



void __add_history
  _P1 (( char *, string ))
{
  char *old = history_list[ history_last % MAX_HISTORY ];
  int i;

  for ( i = 0; i < history_num; i++ )
    if ( strcmp( history_list[ i ], string ) == 0 ) return;

  if ( old ) free( old );
  old = ( char * )calloc( strlen( string ) + 1, sizeof( char ) );
  strcpy( old, string );
  history_list[ history_last % MAX_HISTORY ] = old;
  history_last++;
  history_num++;
  history_offset = history_last % MAX_HISTORY;
  if ( history_num > MAX_HISTORY ) history_num = MAX_HISTORY;
}



void __next_history
  _P1 (( char *, string ))
{
  char *old;

  if ( history_num <= 0 ) return;

  history_offset++;
  if ( history_offset >= history_num ) history_offset = 0;
  old = history_list[ history_offset ];
  if ( old ) strcpy( string, old );
}



void __prev_history
  _P1 (( char *, string ))
{
  char *old;

  if ( history_num <= 0 ) return;

  history_offset--;
  if ( history_offset < 0 ) history_offset = history_num - 1;
  old = history_list[ history_offset ];
  if ( old ) strcpy( string, old );
}



