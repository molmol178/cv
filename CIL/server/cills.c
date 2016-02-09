/*
 * filename : cills.c
 * author   : Takahiro Sugiyama
 * date     : 1993/10/03
 */



#include "cilserver.h"
#include <stdio.h>



void long_print
  P1 (( cilRec *, list ))
void normal_print
  P1 (( cilRec *, list ))
void short_print
  P1 (( cilRec *, list ))



int main(argc,argv)
     int argc;
     char *argv[];
{
  cilRec *list;
  int printall = 0;

  if ( argc == 2 )
    {
      if ( strcmp( argv[1], "-h" ) == 0 )
	{
	  fprintf(stderr,"Usage : cills [-a] [-l] [-h]\n");
	  exit(0);
	}

      if ( strcmp( argv[1], "-a" ) == 0 )
	printall = 1;

      if ( strcmp( argv[1], "-l" ) == 0 )
	printall = 2;
    }

  list = cil_openlist();
  switch ( printall )
    {
    case 0:
      short_print(list);
      break;
    case 1:
      normal_print(list);
      break;
    case 2:
      long_print(list);
      break;
    }
  cil_closelist();
}



void long_print
  _P1 (( cilRec *, list ))
{
  int i, j;
  cilRec *info;

  for ( i = 0; i < CIL_LIST_NUM; i++ )
    {
      info = &list[i];
      if ( info->name[0] == '\0' ) continue;

      printf( "#%d(%s) = {\n", i, info->name );
      printf( "    NAME          : %s\n", info->name );
      printf( "    KEY           : %d\n", info->key );
      printf( "    FILENAME      : %s\n", info->filename );
      printf( "    IMAGEDISP     : %s\n",info->imagedispexec?"exec":"none" );
      printf( "    SHARED MEMORY : %s\n",info->share?"yes":"no" );
      printf( "    DISPLAY       : %s\n",info->displayname );
      printf( "    WINDOW ID     : 0x%08x\n",info->id );
      printf( "    EVENT MASK    : %04x\n",info->mask );
      printf( "    EVENT TYPE    : %d\n",info->event.type );
      printf( "    COMMAND       : %d\n",info->command );
      printf( "    DATA          : [" );
      for ( j = 0; j < CIL_DATA_NUM; j++ )
	printf(" %d", info->data[j] );
      printf(" ]\n}\n");
    }
}



void short_print
  _P1 (( cilRec *, list ))
{
  int i;
  cilRec *info;

  printf( "### NAME            KEY    EX SH DISPLAY\n" );

  for ( i = 0; i < CIL_LIST_NUM; i++ )
    {
      info = &list[i];
      if ( info->name[0] == '\0' ) continue;
	printf( "%3d %-15s %-6d %2d %2d %-10s\n",
	       i, info->name, info->key,
	       info->imagedispexec, info->share,
	       info->displayname);
    }
}



void normal_print
  _P1 (( cilRec *, list ))
{
  int i;
  cilRec *info;

  printf( "### NAME            KEY    EX SH DISPLAY    WID      MASK ET CT FILE\n" );

  for ( i = 0; i < CIL_LIST_NUM; i++ )
    {
      info = &list[i];
      if ( info->name[0] == '\0' ) continue;
	printf( "%3d %-15s %-6d %2d %2d %-10s %08x %04x %2d %2d %s\n",
	       i, info->name, info->key,
	       info->imagedispexec, info->share,
	       info->displayname,
	       info->id, info->mask, info->command,
	       info->event.type, info->filename );
    }

}
