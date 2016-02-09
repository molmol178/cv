/*
 * filename : optlib.c
 * author   : Takahiro Sugiyama
 * date     : 1993/09/18
 */



#include "optlib.h"
#include "strlib.h"
#include "memlib.h"
#include "filelib.h"
#include "timelib.h"
#include <stdio.h>
#include <stdlib.h>



/*--------------------*
 * ²ÄÊÑ°ú¿ô¤ÎºÇÂçÃÍ
 *--------------------*/
#define OPT_INFINITE 1024



/*-------------------------*
 * ÃÍ¤¬ÄÉ²Ã¤µ¤ì¤¿¤«¤É¤¦¤«
 *-------------------------*/
enum {
  OPT_VALUE_NOT_ADDED,
  OPT_VALUE_ADDED,
};



/*----------------------------------------*
 * ¥ª¥×¥·¥ç¥ó¥¢¥¤¥Æ¥à¤Î¼ïÎà
 *----------------------------------------*/
enum {
  TYPE_OPTION,
  TYPE_ARGUMENT,
  TYPE_DESCRIPTION,
};



/*--------------------*
 * description ¤Î¼ïÎà
 *--------------------*/
enum {
  DESCRIPTION_FIRST,
  DESCRIPTION_HERE,
  DESCRIPTION_TAB,
  DESCRIPTION_TAB_SPECIFIED,
};



/*------------------------------------*
 * optitem : ¥ª¥×¥·¥ç¥ó¥¢¥¤¥Æ¥à¤ÎÀë¸À
 *------------------------------------*/
struct optitem {
  long   type;
  char  *description;
  long   state;

  char  *name;
  char  *specifier;
  long   specified;

  long   value_num;
  char **default_value;
  char **value;

  char *value_type;
  long   tab;
};



/*=======================================================*
 * optcommand : ¥³¥Þ¥ó¥ÉÌ¾ (argv[0])
 * optargc    : ¥ª¥×¥·¥ç¥ó¤Ç²òÀÏ¤µ¤ì¤Ê¤«¤Ã¤¿°ú¿ô¤Î¿ô
 * optargv    : ¥ª¥×¥·¥ç¥ó¤Ç²òÀÏ¤µ¤ì¤Ê¤«¤Ã¤¿°ú¿ô¤Î¥ê¥¹¥È
 *=======================================================*/
char  *optcommand = "";
long   optargc;
char **optargv;



/*--------------------------------------*
 * optnum  : ¥ª¥×¥·¥ç¥ó¥¢¥¤¥Æ¥à¤Î¿ô
 * optlist : ¥ª¥×¥·¥ç¥ó¥¢¥¤¥Æ¥à¤Î¥ê¥¹¥È
 *--------------------------------------*/
static long            optnum  = 0;
static struct optitem *optlist = 0;



/*--------------------------------------------------*
 * __opt_argument_append : ¥ª¥×¥·¥ç¥ó¤ËÃÍ¤òÄÉ²Ã¤¹¤ë
 *--------------------------------------------------*/
static long __opt_argument_append
  P1 (( char *, value ))

/*------------------------------------------------*
 * __opt_value_append : ¥ª¥×¥·¥ç¥ó¤ËÃÍ¤òÄÉ²Ã¤¹¤ë
 *------------------------------------------------*/
static long __opt_value_append
  P2 (( struct optitem *, item  ),
      ( char *          , value ))

/*------------------------------------------------*
 * __opt_value_delete : ¥ª¥×¥·¥ç¥ó¤ËÃÍ¤òºï½ü¤¹¤ë
 *------------------------------------------------*/
static long __opt_value_delete
  P2 (( struct optitem *, item  ),
      ( long            , index ))

/*------------------------------------------------*
 * __opt_value_free : ¥ª¥×¥·¥ç¥ó¤ËÃÍ¤òÌµ¸ú¤Ë¤¹¤ë
 *------------------------------------------------*/
static long __opt_value_free
  P2 (( struct optitem *, item  ),
      ( long            , index ))

/*------------------------------------------------*
 * __opt_value_insert : ¥ª¥×¥·¥ç¥ó¤ËÃÍ¤òÁÞÆþ¤¹¤ë
 *------------------------------------------------*/
static long __opt_value_insert
  P3 (( struct optitem *, item  ),
      ( long            , index ),
      ( char *          , value ))

/*------------------------------------------------*
 * __opt_value_let : ¥ª¥×¥·¥ç¥ó¤ËÃÍ¤òÂåÆþ¤¹¤ë
 *------------------------------------------------*/
static long __opt_value_let
  P3 (( struct optitem *, item  ),
      ( long            , index ),
      ( char *          , value ))

/*----------------------------------------------------------*
 * __opt_value_input : »ØÄê¤µ¤ì¤Æ¤¤¤Ê¤¤¥ª¥×¥·¥ç¥ó¤òÍ×µá¤¹¤ë
 *----------------------------------------------------------*/
static void __opt_value_input
  P1 (( struct optitem *, item ))

/*-----------------------------------------------------------------*
 * __opt_value_compact : °ú¿ô¤ò¤Þ¤È¤á¤ë
 *-----------------------------------------------------------------*/
static long __opt_value_compact
  P1 (( struct optitem *, item ))

/*----------------------------------------*
 * __optarg_append : Í¾¤Ã¤¿°ú¿ô¤òÄÉ²Ã¤¹¤ë
 *----------------------------------------*/
static void __optarg_append
  P1 (( char *, value ))



/*-----------------------------------------------------------*
 * __optresource_load : ¥ê¥½¡¼¥¹¥Õ¥¡¥¤¥ë¤òÆÉ¤ß¹þ¤à
 *-----------------------------------------------------------*/
static long __optresource_load
  P2 (( char *, resource_type            ),
      ( char *, resource_filename_return ))

/*------------------------------------------------------------*
 * __optresource_save : ¥ª¥×¥·¥ç¥ó¥ê¥½¡¼¥¹¤ò½ñ¤­¹þ¤à
 *------------------------------------------------------------*/
static long __optresource_save
  P2 (( char *, resource_type            ),
      ( char *, resource_filename_return ))

/*------------------------------------------------------------*
 * __optresource_print : ¥ê¥½¡¼¥¹¾õÂÖ¤òÉ½¼¨¤¹¤ë
 *------------------------------------------------------------*/
static void __optresource_print
  P1 (( char *, resource_type ))



/*-----------------------------------------------------*
 * __opt_unique_check : ¥ª¥×¥·¥ç¥ó¤¬°ì°Õ¤«¤É¤¦¤«Ä´¤Ù¤ë
 *-----------------------------------------------------*/
static void __opt_unique_check
  P0 (void)



/*-----------------------------------------------------------*
 * __opt_item : °ú¿ô¤¬¥ª¥×¥·¥ç¥ó¤«¤É¤¦¤«Ä´¤Ù¤Æ¡¢
 *              ¥ª¥×¥·¥ç¥ó¤Ê¤é¤Ð¥ª¥×¥·¥ç¥ó¤Î¥Ý¥¤¥ó¥¿¤òÊÖ¤¹¡£
 *              ¤½¤¦¤Ç¤Ê¤±¤ì¤Ð¡¢NULL ¤òÊÖ¤¹¡£
 *-----------------------------------------------------------*/
static struct optitem *__opt_item /* ¥ª¥×¥·¥ç¥ó¤Î¥Ý¥¤¥ó¥¿ */
  _P1 (( char *, spec )) /* °ú¿ô */
{
  int i;
  char specifier[ 128 ];

  for ( i = 0; i < optnum; i++ )
    {
      struct optitem *item = &optlist[ i ];

      switch ( item->type )
	{
	case TYPE_DESCRIPTION:
	  break;

	case TYPE_OPTION:
	  if ( strequal( spec, item->specifier ) ) return item;

	case TYPE_ARGUMENT:
	  sprintf( specifier, "-%s", item->name );
	  if ( strequal( spec, specifier ) ) return item;

	  sprintf( specifier, "--%s", item->name );
	  if ( strequal( spec, specifier ) ) return item;

	  break;
	}
    }

  return 0;
}



/*--------------------------------------------------------------------*
 * __opt_search : ¥ª¥×¥·¥ç¥óÌ¾ name ¤Î¥ª¥×¥·¥ç¥ó¥¢¥¤¥Æ¥à¤òÃµ¤·¤ÆÊÖ¤¹.
 *--------------------------------------------------------------------*/
static struct optitem *__opt_search /* ¥ª¥×¥·¥ç¥ó¥¢¥¤¥Æ¥à */
  _P1 (( char *, name )) /* ¥ª¥×¥·¥ç¥óÌ¾ */
{
  int i;
  char specifier[ 128 ];

  for ( i = 0; i < optnum; i++ )
    {
      struct optitem *item = &optlist[ i ];

      switch ( item->type )
	{
	case TYPE_DESCRIPTION:
	  break;

	case TYPE_OPTION:
	  if ( strequal( name, item->specifier ) ) return item;

	case TYPE_ARGUMENT:
	  if ( strequal( name, item->name ) ) return item;

	  sprintf( specifier, "-%s", item->name );
	  if ( strequal( name, specifier ) ) return item;

	  sprintf( specifier, "--%s", item->name );
	  if ( strequal( name, specifier ) ) return item;

	  break;
	}
    }

  return 0;
}



/*------------------------------------------------------------*
 * __opt_analize : ¥ª¥×¥·¥ç¥óÊ¸»úÎó orig ¤ò²òÀÏ¤·¤Æ¡¢
 *                 ¥ª¥×¥·¥ç¥ó¥¢¥¤¥Æ¥à item ¤ËÊÑ´¹¤¹¤ë¡£
 * <name>:<specifier>[:<number>][:<defaults>]*:<description>
 *------------------------------------------------------------*/
static void __opt_analize
  _P2 (( struct optitem *, item ), /* #1 output ¥ª¥×¥·¥ç¥ó¥¢¥¤¥Æ¥à */
       ( char *          , orig )) /*  ¥ª¥×¥·¥ç¥óÊ¸»úÎó   */
{
  long i, num;
  char *head[ 128 ];
  char spec[ 1024 ];

  item->type = 0;
  item->description = 0;
  item->state = 0;

  item->name = 0;
  item->specifier = 0;
  item->specified = 0;
  item->tab = 0;

  item->value_num = 0;
  item->default_value = 0;
  item->value = 0;
  item->value_type = "value";

  strcpy( spec, orig );
  num = strsplit( spec, head, ":" );
  if ( num < 3 )
    {
      item->type = TYPE_DESCRIPTION;
      if ( strequal( "<", head[ 0 ] ))
	{
	  item->description = strnew( &orig[ strlength( head[ 0 ] ) + 1 ] );
	  item->state = DESCRIPTION_FIRST;
	}
      else
      if ( strequal( "=", head[ 0 ] ))
	{
	  item->description = strnew( &orig[ strlength( head[ 0 ] ) + 1 ] );
	  item->state = DESCRIPTION_HERE;
	}
      else
      if ( strequal( ">", head[ 0 ] ))
	{
	  item->description = strnew( &orig[ strlength( head[ 0 ] ) + 1 ] );
	  item->state = DESCRIPTION_TAB;
	}
      else
      if ( '>' == head[ 0 ][ strlen( head[ 0 ] )-1 ] )
	{
	  item->description = strnew( &orig[ strlength( head[ 0 ] ) + 1 ] );
	  item->tab = atol( head[ 0 ] );
	  item->state = DESCRIPTION_TAB_SPECIFIED;
	}
      else
	{
	  item->description = strnew( orig );
	  item->state = DESCRIPTION_HERE;
	}
      return;
    }

  item->name = strnew( head[ 0 ] );

  if ( strequal( head[ 1 ], "*" ) )
    {
      item->type = TYPE_ARGUMENT;
      item->specifier = item->name;
    }
  else
    {
      item->type = TYPE_OPTION;
      item->specifier = strnew( head[ 1 ] );
    }

  {
    char *index_type = head[ num - 1 ];
    char *top_of_description;
    if ( *index_type == '(' )
      {
	while (( *index_type ) && ( *index_type != ')' )) index_type++;
	if ( *index_type )
	  {
	    int back_char = *( index_type );
	    *index_type = '\0';
	    item->value_type = strnew( head[ num - 1 ] + 1 );
	    *index_type = back_char;
	    index_type++;
	    while (( *index_type ) && (( *index_type == ' '  ) ||
				       ( *index_type == '\t' ))) index_type++;
	  }
	if ( *index_type == '\0' ) index_type = head[ num - 1 ];

	item->description = strnew( index_type );
      }
    else item->description = strnew( head[ num - 1 ] );
  }

  if (( num == 3 ) && ( item->type == TYPE_OPTION )) return;

  if ( strequal( head[ 2 ], "*" ) )
    item->value_num = OPT_INFINITE;
  else
    item->value_num = strtolong( head[ 2 ] );

  if ( item->value_num == 0 )
    item->value_num = 1;

  item->value = typenew1( item->value_num, char * );
  item->default_value = typenew1( item->value_num, char * );

  if ( num > 4 )
    {
      int default_value_num = num - 4;

      if ( default_value_num > item->value_num )
	{
	  fprintf( stderr, "PROGRAM-ERROR:__opt_analize:DEFAULT VALUE NUMBER (%d) IS WRONG.\n",
		   default_value_num);
	  fprintf( stderr, "    <%s> : CAPACITY IS %d.\n", item->name, item->value_num );
	  exit( -1 );
	}
      for ( i = 0; i < default_value_num; i++ )
	{
	  item->default_value[ i ] = strnew( head[ 3 + i ] );
	}
    }
}



/*-----------------------------------------------------------------*
 * __opt_print_system_help : ¥·¥¹¥Æ¥à¥ê¥¯¥¨¥¹¥È¤Î¥Ø¥ë¥×¤òÉ½¼¨¤¹¤ë
 *-----------------------------------------------------------------*/
static void __opt_print_system_help()
{
  printf( "\n");
  printf( "----------------------------------------------------------------------------\n");
  printf( "THE OPTIONS OF SYSTEM REQUESTS\n" );
  printf( "    *) --usage--     , --U            : display the usage.\n" );
  printf( "    *) --help--      , --H            : display the manual.\n" );
  printf( "    *) --check--     , --C            : check the options.\n" );
  printf( "    *) --parameter-- , --P            : input options and parameters.\n" );
  printf( "    *) --resource--  , --R <typename> : input <type> resources.\n" );
  printf( "    *) --noresource--, --NR           : not input option resources.\n" );
  printf( "    *) --prresource--, --PR           : print option resources.\n" );
  printf( "    *) --shell--     , --S            : go the option shell mode.\n" );
  printf( "    *) --output--    , --O <filename> : output a configure file.\n" );
  printf( "    *) --input--     , --I <filename> : input a configure file.\n" );
  printf( "----------------------------------------------------------------------------\n");
  printf( "\n");

  optmanual( 0 );
}



/*----------------------------------------------------------------------*
 * __str_side_skip_whitespace : Î¾Ã¼¤Î¥¹¥Ú¡¼¥¹¤òºï½ü¤·¤ÆÀèÆ¬¤òÊÖ¤¹
 *----------------------------------------------------------------------*/
static char *__str_side_skip_whitespace
  _P1 (( char *, buf ))
{
  int index;

  /* delete white space backward */
  index = strlen( buf ) - 1;
  while ( ( buf[ index ] == ' '  ) ||
	  ( buf[ index ] == '\t' ) ||
	  ( buf[ index ] == '\n' ) ) buf[ index-- ] = '\0';

  /* delete white space forward */
  index = 0;
  while ( ( buf[ index ] == ' '  ) ||
	  ( buf[ index ] == '\t' ) ||
	  ( buf[ index ] == '\n' ) ) buf[ index++ ] = '\0';

  return buf + index;
}



/*--------------------------------------------------*
 * __opt_getline : É¸½àÆþÎÏ¤«¤é°ì¹ÔÆÉ¤ß¹þ¤à
 *--------------------------------------------------*/
static char *__opt_getline
  _P1 (( char *, buf_ret ))
{
  char buf[ 1024 ];
  char *value;

  buf[ 0 ] = '\0';
  fgets( buf, 1024, stdin );

  if ( buf[ 0 ] == '\0' )
    {
      fprintf( stderr, "EOF\n" );
      exit( 0 );
    }

  value = __str_side_skip_whitespace( buf );
  strcpy( buf_ret, value );

  return buf_ret;
}



/*--------------------------------------------------------------------------*
 * __opt_complement_value : ¥ª¥×¥·¥ç¥ó¤ÎÃÍ¤ò½é´üÃÍ¤ò»È¤Ã¤ÆÊä´Ö¤¹¤ë
 *--------------------------------------------------------------------------*/
static void __opt_complement_value()
{
  int i, j;
  struct optitem *item;

  for ( i = 0; i < optnum; i++ )
    {
      item = &optlist[ i ];
      if ( item->type == TYPE_DESCRIPTION ) continue;
      if ( __opt_value_compact( item ) && ( item->value_num == OPT_INFINITE )) continue;
      for ( j = 0; j < item->value_num; j++ )
	if (( item->value[ j ] == 0 ) && ( item->default_value[ j ] ))
	  item->value[ j ] = strnew( item->default_value[ j ] );
    }
}



/*==========================================================*
 * optinit : ¥³¥Þ¥ó¥É¥é¥¤¥ó¤ò»ØÄê¤µ¤ì¤¿¥ª¥×¥·¥ç¥ó¤Ç²òÀÏ¤¹¤ë
 *==========================================================*/
void optinit
  _P4 (( int    ,op_argc ), /* ¥ª¥×¥·¥ç¥ó¤Î¿ô     */
       ( char **,op_argv ), /* ¥ª¥×¥·¥ç¥ó¤Î¥ê¥¹¥È */
       ( int    ,argc    ), /* °ú¿ô¤Î¿ô           */
       ( char **,argv    )) /* °ú¿ô¤Î¥ê¥¹¥È       */
{
  long i, j;
  char optcommandBuf[ 128 ];

  long configure = 0;
  char *configure_filename = "-";
  long output_configure = 0;
  char *output_configure_filename = "-";
  long input    = 0;
  long print    = 0;
  long shell    = 0;

  long  resource_print = 0;
  char *resource_type  = 0;
  long  resource_input = 1;

  if ( fn_filename( optcommandBuf, argv[ 0 ] ) == 0 )
    optcommand = strnew( optcommandBuf );
  else
    optcommand = argv[ 0 ];

  optnum     = op_argc;
  optlist    = typenew1( op_argc, struct optitem );
  optargc    = 0;
  optargv    = 0;

  for ( i = 0; i < op_argc; i++ )
    __opt_analize( &optlist[ i ], op_argv[ i ] );

  __opt_unique_check();

  for ( i = 1; i < argc; i++ )
    {
      struct optitem *item;

      /*-----------------------------*
       * CIL SYSTEM ¤«¤é¤Î¥ê¥¯¥¨¥¹¥È
       *-----------------------------*/
      if ( strequal( argv[ i ], "-help" ) ) optmanual( 1 );
      if ( strequal( argv[ i ], "-usage" ) ) optusage( 1 );

      if ( strequal( argv[ i ], "--" ) )
	{ continue; /* ¥ê¥¹¥È¤Î½ªÎ»µ­¹æ */}

      if (( strequal( argv[ i ], "--help--" ) ) || ( strequal( argv[ i ], "--H" ) ))
	__opt_print_system_help();

      if (( strequal( argv[ i ], "--usage--" ) ) || ( strequal( argv[ i ], "--U" )))
	optusage( 0 );

      if (( strequal( argv[ i ], "--shell--" ) ) || ( strequal( argv[ i ], "--S" ) ))
	{ shell = 1; continue; }

      if (( strequal( argv[ i ], "--check--" ) ) || ( strequal( argv[ i ], "--C" )))
	{ print = 1; continue; }

      if (( strequal( argv[ i ], "--parameter--" ) ) || ( strequal( argv[ i ], "--P" )))
	{ input = 1; continue; }

      if (( strequal( argv[ i ], "--prresource--" ) ) || ( strequal( argv[ i ], "--PR" )))
	{ resource_print = 1; continue; }

      if (( strequal( argv[ i ], "--noresource--" ) ) || ( strequal( argv[ i ], "--NR" )))
	{ resource_input = 0; continue; }

      if ( strequal( argv[ i ], "--resource--" ) || strequal( argv[ i ], "--R" ))
	{
	  if ( ++i < argc ) resource_type = argv[ i ];
	  else fprintf( stderr, "warning:--resource--:NOT SPECIFIED A TYPE NAME.\n" );
	  continue;
	}

      if ( strequal( argv[ i ], "--input--" ) || strequal( argv[ i ], "--I" ) ||
	   strequal( argv[ i ], "--load--" ) || strequal( argv[ i ], "--L" ))
	{
	  configure = 1;
	  if ( ++i < argc ) configure_filename = argv[ i ];
	  else fprintf( stderr, "warning:--input--:NOT SPECIFIED A CONFIGURE FILENAME.\n" );
	  continue;
	}

      if ( strequal( argv[ i ], "--output--" ) || strequal( argv[ i ], "--O" ) ||
	   strequal( argv[ i ], "--save--" ))
	{
	  output_configure = 1;
	  if ( ++i < argc ) output_configure_filename = argv[ i ];
	  else fprintf( stderr, "warning:--output--:NOT SPECIFIED AN CONFIGURE FILENAME.\n" );
	  continue;
	}

      /*--------------------------*
       * ¥æ¡¼¥¶»ØÄê¤Î¥ª¥×¥·¥ç¥ó
       *--------------------------*/
      item = __opt_item( argv[ i ] );

      if ( item )
	{
	  item->specified = 1;

	  if ( item->value_num != OPT_INFINITE )
	    {/* °ú¿ô¤¬Í­¸Â¤Î»þ */
	      for ( j = 0; ( j < item->value_num ) && ( ++i < argc ); j++ )
		__opt_value_append( item, argv[ i ] );
	    }
	  else
	    {/* °ú¿ô¤¬Ìµ¸Â¤Î»þ */
	      for ( ; ++i < argc; )
		{
		  if ( argv[ i ][ 0 ] == '-' ) { i--; break; }
		  __opt_value_append( item, argv[ i ] );
		}
	    }
	}
      else
	{/* °ú¿ô¤òÄÉ²Ã¤¹¤ë */
	  __opt_argument_append( argv[ i ] );
	}
    }

  __opt_complement_value();

  if ( resource_print ) __optresource_print( resource_type );

  if ( configure ) optinputconfigure( configure_filename );

  if ( resource_input )
    {
      int error_code;
      char resource_file[ 256 ];
      error_code = __optresource_load( resource_type, resource_file );
      if ( resource_type && ( error_code != 0 ))
	{
	  fprintf( stderr, "%s:cannot access resource file \"%s\".\n",
		   optcommand, resource_file );
	}
    }

  if ( input ) optinput( 0 );

  if ( shell ) optshellmode();

  if ( output_configure ) optoutputconfigure( output_configure_filename );

  if ( print ) optprint();
}



/*==============================================================*
 * optprint : ¥ª¥×¥·¥ç¥ó¤Î²òÀÏ·ë²Ì¤òÉ½¼¨¤·¤Þ¤¹¡Ê¼ç¤Ë¥Ç¥Ð¥Ã¥°ÍÑ¡Ë
 *==============================================================*/
void optprint
  _P0 ( void )
{
  int i, j, num;

  for ( i = 0; i < optnum; i++ )
    {
      struct optitem *item = &optlist[ i ];

      switch ( item->type )
	{
	case TYPE_DESCRIPTION:
	  fprintf( stderr, "#%d:DESCRIPTION={\"%s\"}\n", i, item->description );
	  continue;
	  break;

	case TYPE_OPTION:
	  fprintf( stderr, "#%d:OPTION=", i );
	  if ( item->value_num == OPT_INFINITE )
	    fprintf( stderr, "{<%s>,<%s>,#INF(%s),{", item->name, item->specifier, item->value_type );
	  if ( item->value_num == 0 )
	    fprintf( stderr, "{<%s>,<%s>,", item->name, item->specifier );
	  else
	    fprintf( stderr, "{<%s>,<%s>,#%d(%s),{", item->name, item->specifier, item->value_num, item->value_type );
	  break;

	case TYPE_ARGUMENT:
	  fprintf( stderr, "#%d:ARGUMENT=", i );
	  if ( item->value_num == OPT_INFINITE )
	    fprintf( stderr, "{<%s>,#INF(%s),{", item->name, item->value_type );
	  else
	  if ( item->value_num == 0 )
	    fprintf( stderr, "{<%s>,", item->name );
	  else
	    fprintf( stderr, "{<%s>,#%d(%s),{", item->name, item->value_num, item->value_type );
	  break;
	}

      if ( item->value_num )
	{
	  num = __opt_value_compact( item );

	  for ( j = 0; j < num; j++ )
	    {
	      fprintf( stderr, "\"%s\"", item->value[ j ] );
	      if ( j != ( num - 1 )) fprintf( stderr, "," );
	    }
	  fprintf( stderr, "}," );
	}

      if ( item->specified )
	fprintf( stderr, "[SPECIFIED]," );
      else
	fprintf( stderr, "[NOT SPECIFIED]," );
      fprintf( stderr, "\"%s\"}\n", item->description );
    }
}



/*-------------------------------------------------*
 * __opt_makeline : ¥Þ¥Ë¥å¥¢¥ëÉ½¼¨ÍÑ¤ÎÊ¸»úÎó¤òºî¤ë
 *-------------------------------------------------*/
static char *__opt_makeline /* Ê¸»úÎó */
  _P1 (( struct optitem *, item )) /* ¥ª¥×¥·¥ç¥ó¥¢¥¤¥Æ¥à */
{
  int i;
  char buf[ 512 ];
  char subbuf[ 64 ];

  switch ( item->type )
    {
    case TYPE_DESCRIPTION:
      buf[ 0 ] = '\0';
      break;

    case TYPE_OPTION:
      sprintf( buf, "[1m%s [0m", item->specifier );
      if ( item->value_num == OPT_INFINITE )
	{
	  sprintf( subbuf, "<[4m%s[0m>...", item->value_type );
	  strcat( buf, subbuf );
	}
      else
	for ( i = 0; i < item->value_num; i++ )
	  {
	    sprintf( subbuf, "<[4m%s[0m> ", item->value_type );
	    strcat( buf, subbuf );
	  }
      break;

    case TYPE_ARGUMENT:
      if ( strequal( item->value_type, "value" ) )
	sprintf( buf, "<[4m%s[0m>", item->name );
      else
	sprintf( buf, "[1m%s[0m <[4m%s[0m>", item->name, item->value_type );
      if ( item->value_num == OPT_INFINITE )
	strcat( buf, "... " );
      else
      if ( item->value_num == 1 )
	strcat( buf, " " );
      else
	{
	  sprintf( subbuf, "#%d ", item->value_num );
	  strcat( buf, subbuf );
	}
      break;
    }

  return strnew( buf );
}



/*------------------------------------------------------------*
 * __opt_print_default_value : ½é´üÃÍ¤òÉ½¼¨¤¹¤ë
 *------------------------------------------------------------*/
static void __opt_print_default_value
  _P2 (( FILE *          , fp   ),
       ( struct optitem *, item ))
{
  int i;
  int has_value_num;

  has_value_num = 0;
  for ( i = 0; i < item->value_num; i++ )
    if ( item->default_value[ i ] ) has_value_num++;

  if ( has_value_num )
    {
      fp_printf( fp, " (" );
      for ( i = 0; i < item->value_num; i++ )
	if ( item->default_value[ i ] )
	  {
	    fp_printf( fp, "[4m%s[0m", item->default_value[ i ] );
	    if ( i != has_value_num - 1 ) fp_printf( fp, "," );
	  }
      fp_printf( fp, ")" );
    }
}



/*--------------------------------------------------------*
 * __opt_printline : ¥Þ¥Ë¥å¥¢¥ë¤Î¥ª¥×¥·¥ç¥ó¤ò°ì¹ÔÉ½¼¨¤¹¤ë
 *--------------------------------------------------------*/
static void __opt_printline
  _P2 (( struct optitem *, item ), /* ¥ª¥×¥·¥ç¥ó¥¢¥¤¥Æ¥à */
       ( char *          , head )) /* ¥ª¥×¥·¥ç¥óÊ¸»úÎó   */
{
  int i;

  if ( item->type == TYPE_DESCRIPTION )
    {
      if ( item->state == DESCRIPTION_HERE )
	printf( "\r%s\n", item->description );
      else
      if ( item->state == DESCRIPTION_TAB )
	{
	  printf( "\r" );
	  for ( i = 0; i < item->tab + 4; i++ ) fputc( ' ', stdout );
	  printf( "  %s\n", item->description );
	}
      else
      if ( item->state == DESCRIPTION_TAB_SPECIFIED )
	{
	  printf( "\r" );
	  for ( i = 0; i < item->tab; i++ ) fputc( ' ', stdout );
	  printf( "%s\n", item->description );
	}
      return;
    }

  printf( "    %s", head );
  for ( i = strprintlength( head ); i < item->tab; i++ ) fputc( ' ', stdout );
  printf( ": %s", item->description );

  __opt_print_default_value( stdout, item );
  printf( ".\n" );

  strfree( head );
}



/*--------------------------------------------------*
 * __opt_print_first_line : °ì¹ÔÌÜ¤òÉ½¼¨¤¹¤ë
 *--------------------------------------------------*/
static void __opt_print_first_line()
{
  char buf[ 256 ];
  char subbuf[ 256 ];
  char argbuf[ 256 ];
  int i;
  int has_option = 0;
  int has_argument = 0;
  struct optitem *item;

  sprintf( buf, "Usage: [4m[1m %s [0m", optcommand );
  argbuf[ 0 ] = '\0';

  for ( i = 0; i < optnum; i++ )
    {
      item = &optlist[i];
      if ( item->type == TYPE_OPTION ) has_option++;
      if ( item->type == TYPE_ARGUMENT )
	{
	  has_argument++;
	  if ( item->value_num == OPT_INFINITE )
	    {
	      sprintf( subbuf, " [[4m%s[0m]...", item->name );
	      strcat( argbuf, subbuf );
	    }
	  else
	    {
	      int j;
	      for ( j = 0; j < item->value_num; j++ )
		{
		  if ( item->default_value[ 0 ] )
		    sprintf( subbuf, " [[4m%s[0m]", item->name );
		  else
		    sprintf( subbuf, " <[4m%s[0m>", item->name );
		  strcat( argbuf, subbuf );
		}
	    }
	}
    }
  if ( has_option == 1 ) strcat( buf, " [option]"  ); else
  if ( has_option > 1  ) strcat( buf, " [options]" );

  if ( has_argument )
    {
      strcat( buf, argbuf );
      if ( has_option == 1 ) strcat( buf, " [option]"  ); else
      if ( has_option > 1  ) strcat( buf, " [options]" );
    }

  printf( "%s\n", buf );
}



/*=========================================================================*
 * optmanual : ¥ª¥×¥·¥ç¥ó¤òÀ°·Á¤·¤ÆÉ½¼¨¤·¡¢½ªÎ»¥³¡¼¥É¤Ç½ªÎ»¤¹¤ë¡£
 *=========================================================================*/
void optmanual
  _P1 (( long, exit_code )) /* ½ªÎ»¥³¡¼¥É */
{
  long i, max, n;
  char *head[ 512 ];
  long maxlist[ 100 ];
  long maxindex;

  max = 0;
  maxindex = 0;
  for ( i = 0; i < optnum; i++ )
    {
      head[ i ] = __opt_makeline( &optlist[ i ] );
      n = strprintlength( head[ i ] );
      if ( max < n ) max = n;
      if (( optlist[ i ].type == TYPE_DESCRIPTION ) &&
	  ( optlist[ i ].state == DESCRIPTION_HERE ))
	{
	  maxlist[ maxindex++ ] = max;
	  max = 0;
	}
    }
  maxlist[ maxindex++ ] = max;

  maxindex = 0;
  for ( i = 0; i < optnum; i++ )
    {
      if (( optlist[ i ].type == TYPE_DESCRIPTION ) &&
	  ( optlist[ i ].state == DESCRIPTION_HERE ))
	maxindex ++;
      if (( optlist[ i ].type != TYPE_DESCRIPTION ) ||
	  ( optlist[ i ].state != DESCRIPTION_TAB_SPECIFIED ))
	  optlist[ i ].tab = maxlist[ maxindex ];
    }

  for ( i = 0; i < optnum; i++ )
    {
      if (( optlist[ i ].type  == TYPE_DESCRIPTION ) &&
	  ( optlist[ i ].state == DESCRIPTION_FIRST ))
	printf( "\r%s\n", optlist[ i ].description );
    }
  __opt_print_first_line();
  for ( i = 0; i < optnum; i++ )
    __opt_printline( &optlist[ i ], head[ i ] );

  exit( exit_code );
}



/*===============================================================*
 * optusage : ¥ª¥×¥·¥ç¥ó¤ò´ÊÃ±¤ËÉ½¼¨¤·¤Æ¡¢½ªÎ»¥³¡¼¥É¤Ç½ªÎ»¤¹¤ë¡£
 *===============================================================*/
void optusage
  _P1 (( long, exit_code )) /* ½ªÎ»¥³¡¼¥É */
{
  long i, j, n, col;
  char buf[ 512 ];
  char subbuf[ 64 ];

  __opt_print_first_line();
  printf( "   " ); col = 3;

  for ( i = 0; i < optnum; i++ )
    {
      struct optitem *item = &optlist[ i ];

      if ( item->type == TYPE_DESCRIPTION ) continue;
      if ( item->type == TYPE_ARGUMENT )
	{
	  if ( item->value_num == OPT_INFINITE )
	    sprintf( buf, " [ [1m%s [0m<[4m%s[0m>... ]", item->name, item->value_type );
	  else
	    {
	      sprintf( buf, " [ [1m%s [0m", item->name );
	      for ( j = 0; j < item->value_num; j++ )
	        {
		  strcat( buf, "<[4m" );
		  strcat( buf, item->value_type );
		  strcat( buf, "[0m> " );
		}
	      strcat( buf, "]" );
	    }
	  if ( ( col + strprintlength( buf ) ) > 80 )
	    { printf( "\n   " ); col = 3; }
	  printf( buf ); col += strprintlength( buf );
	  continue;
	}

      sprintf( buf, " [ %s [1m%s", item->name, item->specifier );
      if ( item->value_num != OPT_INFINITE )
	for ( j = 0; j < item->value_num; j++ )
	  {
	    sprintf( subbuf, " [0m<[4m%s[0m>", item->value_type );
	    strcat( buf, subbuf );
	  }
      else
	{
	  sprintf( subbuf, " [0m<[4m%s[0m>...", item->value_type );
	  strcat( buf, subbuf );
	}
      strcat( buf, " [0m]" );

      if ( ( col + strprintlength( buf ) ) > 80 )
	{ printf( "\n   " ); col = 3; }
      printf( buf ); col += strprintlength( buf );
    }
  printf( "\n" );

  exit( exit_code );
}



/*========================================*
 * optvaluenum : ¥ª¥×¥·¥ç¥ó¤ÎÃÍ¤Î¿ô¤òÊÖ¤¹
 *========================================*/
long optvaluenum /* ¥ª¥×¥·¥ç¥ó¤ÎÃÍ¤Î¿ô */
  _P1 (( char *, name )) /* ¥ª¥×¥·¥ç¥óÌ¾ */
{
  struct optitem *item;
  int i, num;

  item = __opt_search( name );

  if ( item == 0 )
    {
      fprintf( stderr, "PROGRAM-ERROR:optvaluenum:I DON'T HAVE THE OPTION \"%s\".\n", name );
      exit( -1 );
    }

  num = __opt_value_compact( item );

  return num;
}



/*===========================================*
 * optvaluelist : ¥ª¥×¥·¥ç¥ó¤ÎÃÍ¤Î¥ê¥¹¥ÈÊÖ¤¹
 *===========================================*/
char **optvaluelist /* ¥ª¥×¥·¥ç¥ó¤ÎÃÍ¤Î¥ê¥¹¥È */
  _P1 (( char *, name )) /* ¥ª¥×¥·¥ç¥óÌ¾ */
{
  struct optitem *item;

  item = __opt_search( name );

  if ( item == 0 )
    {
      fprintf( stderr, "PROGRAM-ERROR:optvaluelist:I DON'T HAVE THE OPTION \"%s\".\n", name );
      exit( -1 );
    }

  return item->value;
}



/*=================================*
 * optvalue : ¥ª¥×¥·¥ç¥ó¤ÎÃÍ¤òÊÖ¤¹
 *=================================*/
char *optvalue /* ¥ª¥×¥·¥ç¥ó¤ÎÃÍ */
  _P1 (( char *, name )) /* ¥ª¥×¥·¥ç¥óÌ¾ */
{
  return optnvalue( name, 0 );
}



/*====================================================*
 * optvalueint : ¥ª¥×¥·¥ç¥ó¤ÎÃÍ¤òÀ°¿ôÃÍ¤ËÊÑ´¹¤·¤ÆÊÖ¤¹
 *====================================================*/
long optvalueint /* À°¿ôÃÍ */
  _P1 (( char *, name )) /* ¥ª¥×¥·¥ç¥óÌ¾ */
{
  return atol( optvalue( name ) );
}



/*======================================================*
 * optvaluefloat : ¥ª¥×¥·¥ç¥ó¤ÎÃÍ¤ò¼Â¿ôÃÍ¤ËÊÑ´¹¤·¤ÆÊÖ¤¹
 *======================================================*/
double optvaluefloat /* ¼Â¿ôÃÍ */
  _P1 (( char *, name )) /* ¥ª¥×¥·¥ç¥óÌ¾ */
{
  return atof( optvalue( name ) );
}



/*=========================================*
 * optnvalue : ¥ª¥×¥·¥ç¥ó¤ÎiÈÖÌÜ¤ÎÃÍ¤òÊÖ¤¹
 *=========================================*/
char *optnvalue /* ¥ª¥×¥·¥ç¥ó¤ÎÃÍ */
  _P2 (( char *, name ), /* ¥ª¥×¥·¥ç¥óÌ¾ */
       ( long  , i    )) /* ¥¤¥ó¥Ç¥Ã¥¯¥¹ */
{
  struct optitem *item;

  item = __opt_search( name );

  if ( item == 0 )
    {
      fprintf( stderr, "PROGRAM-ERROR:optnvalue:I DON'T HAVE THE OPTION \"%s\".\n", name );
      exit( -1 );
    }

  if ( i < 0 || i >= item->value_num )
    {
      fprintf( stderr, "error:optnvalue:BAD INDEX %d\n", i );
      fprintf( stderr, "    YOU CAN SPECIFY 0 to %d\n", item->value_num );
      return 0;
    }

  while ( item->value[ i ] == 0 ) __opt_value_input( item );

  return item->value[ i ];
}



/*===========================================================*
 * optnvalueint : ¥ª¥×¥·¥ç¥ó¤ÎiÈÖÌÜ¤ÎÃÍ¤òÀ°¿ôÃÍ¤ËÊÑ´¹¤·¤ÆÊÖ¤¹
 *===========================================================*/
long optnvalueint /* À°¿ôÃÍ */
  _P2 (( char *, name ), /* ¥ª¥×¥·¥ç¥óÌ¾ */
       ( long  , i    )) /* ¥¤¥ó¥Ç¥Ã¥¯¥¹ */
{
  return atol( optnvalue( name, i ) );
}



/*==============================================================*
 * optnvaluefloat : ¥ª¥×¥·¥ç¥ó¤ÎiÈÖÌÜ¤ÎÃÍ¤ò¼Â¿ôÃÍ¤ËÊÑ´¹¤·¤ÆÊÖ¤¹
 *==============================================================*/
double optnvaluefloat /* ¼Â¿ôÃÍ */
  _P2 (( char *, name ), /* ¥ª¥×¥·¥ç¥óÌ¾ */
       ( long  , i    )) /* ¥¤¥ó¥Ç¥Ã¥¯¥¹ */
{
  return atof( optnvalue( name, i ) );
}



/*===================================================================*
 * optspecified : ¥ª¥×¥·¥ç¥ó¤¬»ØÄê¤µ¤ì¤¿¤«¤É¤¦¤«Ä´¤Ù¤ë
 *                »ØÄê¤µ¤ì¤Æ¤ë¤È¤­ 1 ¤ò¡¢»ØÄê¤µ¤ì¤Æ¤Ê¤¤¤È¤­ 0 ¤òÊÖ¤¹
 *===================================================================*/
long optspecified /* ¿¿ÍýÃÍ */
  _P1 (( char *, name )) /* ¥ª¥×¥·¥ç¥óÌ¾ */
{
  struct optitem *item;

  item = __opt_search( name );

  if ( item == 0 )
    {
      fprintf( stderr, "PROGRAM-ERROR:optspecfied:I DON'T HAVE THE OPTION \"%s\".\n", name );
      exit( -1 );
    }

  return item->specified;
}



/*===============================================*
 * optsetvalues : ¥ª¥×¥·¥ç¥ó¤ËÊ£¿ô¤ÎÃÍ¤òÀßÄê¤¹¤ë
 *================================================*/
void optsetvalues
  _P3 (( char * , name   ), /* ¥ª¥×¥·¥ç¥óÌ¾ */
       ( int    , num    ), /* ÃÍ¿ô         */
       ( char **, values )) /* ÃÍ¥ê¥¹¥È     */
{
  int i;
  struct optitem *item;

  item = __opt_search( name );

  if ( item == 0 )
    {
      fprintf( stderr, "PROGRAM-ERROR:optsetvalues:I DON'T HAVE THE OPTION \"%s\".\n", name );
      exit( -1 );
    }

  for ( i = 0; ( i < num ) && ( i < item->value_num ); i++ )
    {
      if ( item->value[ i ] ) strfree( item->value[ i ] );
      item->specified = 1;
      item->value[ i ] = strnew( values[ i ] );
    }
}



/*===============================================*
 * optaddvalues : ¥ª¥×¥·¥ç¥ó¤ËÊ£¿ô¤ÎÃÍ¤òÄÉ²Ã¤¹¤ë
 *================================================*/
void optaddvalues
  _P3 (( char * , name   ), /* ¥ª¥×¥·¥ç¥óÌ¾ */
       ( int    , num    ), /* ÃÍ¿ô         */
       ( char **, values )) /* ÃÍ¥ê¥¹¥È     */
{
  int i;
  struct optitem *item;

  item = __opt_search( name );

  if ( item == 0 )
    {
      fprintf( stderr, "PROGRAM-ERROR:optaddvalues:I DON'T HAVE THE OPTION \"%s\".\n", name );
      exit( -1 );
    }

  for ( i = 0; i < num; i++ )
    __opt_value_append( item, values[ i ] );
}



/*===================================*
 * optset : ¥ª¥×¥·¥ç¥ó¤ËÃÍ¤òÀßÄê¤¹¤ë
 *===================================*/
void optset
  _P2 (( char *, name  ), /* ¥ª¥×¥·¥ç¥óÌ¾ */
       ( char *, value )) /* ÃÍ           */
{
  struct optitem *item;

  item = __opt_search( name );

  if ( item == 0 )
    {
      fprintf( stderr, "PROGRAM-ERROR:optset:I DON'T HAVE THE OPTION \"%s\".\n", name );
      exit( -1 );
    }

  __opt_value_let( item, 0, value );
}



/*------------------------------------------------------------*
 * __opt_value_input_0 : ¥¹¥¤¥Ã¥Á¥ª¥×¥·¥ç¥ó¤òÆþÎÏ¤¹¤ë
 *------------------------------------------------------------*/
static void __opt_value_input_0
  _P1 (( struct optitem *, item ))
{
  char buf[64];

  while ( 1 )
    {
      if ( item->specified )
	fprintf( stderr, "    INPUT <%s> ([y]/n) ? ", item->name );
      else
	fprintf( stderr, "    INPUT <%s> (y/[n]) ? ", item->name );

      __opt_getline( buf );

      if ( buf[ 0 ] == '\0' ) break;

      if ( strxequal( buf, "n" ) || strxequal( buf, "no" ))
	{ item->specified = 0; break; }

      if ( strxequal( buf, "y" ) || strxequal( buf, "yes" ))
	{ item->specified = 1; break; }
    }
}



/*------------------------------------------------------------*
 * __opt_unspecified : ¥ª¥×¥·¥ç¥ó¤òÌ¤»ØÄê¤Ë¤¹¤ë
 *------------------------------------------------------------*/
static void __opt_unspecified
  _P1 (( struct optitem *, item ))
{
  int i;

  item->specified = 0;
  for ( i = 0; i < item->value_num; i++ )
    {
      if ( item->value[ i ] ) strfree( item->value[ i ] );
      item->value[ i ] = item->default_value[ i ];
    }
}



/*------------------------------------------------------------*
 * __opt_value_input_1 : °ú¿ô¤¬°ì¤Ä¤Î¥ª¥×¥·¥ç¥ó¤òÆþÎÏ¤¹¤ë
 *------------------------------------------------------------*/
static void __opt_value_input_1
  _P1 (( struct optitem *, item ))
{
  int i, num;
  char buf[ 256 ];

  while ( 1 )
    {
      if ( strequal( item->value_type, "value" ) )
	fprintf( stderr, "    INPUT <%s> = ", item->name );
      else
	fprintf( stderr, "    INPUT <%s> (%s) = ", item->name, item->value_type );

      __opt_getline( buf );

      if ( strequal( buf, "?" ) )
	{
	  fprintf( stderr, "---------------------------- HELP ----------------------------\n" );
	  fprintf( stderr, "    <value>  : SET VALUE TO THE OPTION \"%s\".\n", item->name );
	  fprintf( stderr, "    \"\"       : UNSET VALUE TO THE OPTION \"%s\".\n", item->name );
	  fprintf( stderr, "    .        : IGNORE THE OPTION \"%s\".\n", item->name );
	  fprintf( stderr, "    <return> : USE A CURRENT VALUE OF THE OPTION \"%s\".\n", item->name );
	  fprintf( stderr, "--------------------------------------------------------------\n" );
	}
      else
      if ( strequal( buf, "\"\"" ) )
	{
	  __opt_unspecified( item );
	  break;
	}
      else
      if ( strequal( buf, "." ) )
	{
	  break;
	}
      else
      if ( buf[ 0 ] != '\0' )
	{
	  __opt_value_let( item, 0, buf );
	  break;
	}
      else
      if ( item->value[ 0 ] )
	{
	  break;
	}
      else
	fprintf( stderr, "    I DON'T HAVE A DEFAULT VALUE OF THIS OPTION \"%s\".\n", item->name );
    }
}



/*------------------------------------------------------------*
 * __opt_item_print_value : ¥ª¥×¥·¥ç¥ó¤ÎÃÍ¤òÉ½¼¨¤¹¤ë
 *------------------------------------------------------------*/
static void __opt_item_print_value
  _P1 (( struct optitem *, item ))
{
  int has_value_num = 0;
  int i;

  for ( i = 0; i < item->value_num; i++ )
    {
      if ( item->value[ i ] ) has_value_num++;
    }

  if ( has_value_num == 0 )
    {
      fprintf( stderr, "YOU SPECIFY NO VALUES.\n" );
      return;
    }

  fprintf( stderr, "YOU SPECIFIED FOLLOWING VALUES.\n" );
  for ( i = 0; i < item->value_num; i++ )
    if ( item->value[ i ] )
      fprintf( stderr, "    #%d = \"%s\"\n", i, item->value[ i ] );
}



/*-----------------------------------------------------------------*
 * __opt_item_delete_all_values : ¥ª¥×¥·¥ç¥ó¤ÎÃÍ¤ò¤¹¤Ù¤Æºï½ü¤¹¤ë
 *-----------------------------------------------------------------*/
static void __opt_item_delete_all_values
  _P1 (( struct optitem *, item ))
{
  int i;

  for ( i = 0; i < item->value_num; i++ )
    {
      if ( item->value[ i ] ) strfree( item->value[ i ] );
      item->value[ i ] = 0;
    }
  item->specified = 0;
}



/*------------------------------------------------------------*
 * __opt_print_input_help : ÆþÎÏ¥Ø¥ë¥×¤òÉ½¼¨¤¹¤ë
 *------------------------------------------------------------*/
static void __opt_print_input_help()
{
  fprintf( stderr, "-----------------------------HELP-----------------------------\n" );
  fprintf( stderr, "    .                    : FINISH TO INPUT VALUES.\n" );
  fprintf( stderr, "    ?                    : PRINT THIS MESSAGES.\n" );
  fprintf( stderr, "    <value>              : CHANGE CURRENT TO <value>.\n" );
  fprintf( stderr, "    #*                   : PRINT ALL VALUES.\n" );
  fprintf( stderr, "    #*-                  : DELETE ALL VALUES.\n" );
  fprintf( stderr, "    #-<number[s]>        : DELETE #<number[s]> VALUES.\n" );
  fprintf( stderr, "    #<number>            : PRINT #<number> VALUE.\n" );
  fprintf( stderr, "    #<number>.           : CHANGE CURRENT NUMBER.\n" );
  fprintf( stderr, "    #<number>-           : DELETE #<number> VALUE.\n" );
  fprintf( stderr, "    #<number>+<value[s]> : INSERT <value[s]> BEFORE #<number>.\n" );
  fprintf( stderr, "    #<number>=<value[s]> : CHANGE #<number> VALUE[S] TO <value[s]>.\n" );
  fprintf( stderr, "--------------------------------------------------------------\n" );
}



/*-------------------------------------------------------------*
 * __opt_value_input_n : Ê£¿ô¤Î°ú¿ô¤ò»ý¤Ä¥ª¥×¥·¥ç¥ó¤òÆþÎÏ¤¹¤ë
 *-------------------------------------------------------------*/
static void __opt_value_input_n
  _P1 (( struct optitem *, item ))
{
  char buf[ 256 ];
  int current = 0;
  int index = 0;
  int command = 0;
  int number = 0;
  char *value;
  char *newvalue;
  char subbuf[ 256 ];

  for ( current = 0; current < item->value_num; current++ )
    if ( item->value[ current ] == 0 ) break;

  while ( 1 )
    {
      int last_num = __opt_value_compact( item );

      if ( last_num != 0 ) item->specified = 1;
      else		   item->specified = 0;

      if ( current > last_num ) current = last_num;

      if ( current >= item->value_num )
	sprintf( subbuf, "(END)" );
      else
      if ( item->value[ current ] )
	sprintf( subbuf, "(#%d:\"%s\")", current, item->value[ current ] );
      else
	{
	  if ( strequal( item->value_type, "value" ) )
	    sprintf( subbuf, "(#%d)", current );
	  else
	    sprintf( subbuf, "(#%d:%s)", current, item->value_type );
	}

      fprintf( stderr, "    INPUT <%s> %s = ", item->name, subbuf );

      __opt_getline( buf );

      if ( buf[ 0 ] == 0 )
	{
	  if ( current >= item->value_num ) break;
	  if ( item->value[ current ] ) current++;
	  continue;
	}

      if ( strequal( buf, "?" ) )
	{
	  __opt_print_input_help();
	  continue;
	}
      else
      if ( strequal( buf, "." ) )
	{
	  break;
	}
      else
      if ( strequal( buf, "#*" ) )
	{
	  __opt_item_print_value( item );
	  continue;
	}
      else
      if ( strequal( buf, "#*-" ) )
	{
	  __opt_item_delete_all_values( item );
	  fprintf( stderr, "ALL VALUES WERE DELETED.\n" );
	  current = 0;
	  continue;
	}
      else
      if ( strsubequal( buf, "#-", 2 ) )
	{
	  int j, num, *values[256];
	  num = strsplit( buf + 2, values, " \t" );
	  for ( j = 0; j < num; j++ )
	    {
	      if ( ! strisdigit( values[ j ] ) )
		{
		  fprintf( stderr, "\"%s\" IS NOT NUMBER .\n", values[ j ] );
		  continue;
		}

	      number = atol( values[ j ] );
	      if ( __opt_value_free( item, number ) )
		{
		  fprintf( stderr, "DELETED THE VALUE #%d \"%s\".\n", number, item->value[ number ] );
		}
	      else
		{
		  fprintf( stderr, "NOT SPECIFIED THE VALUE #%d.\n", number );
		}
	    }
	  continue;
	}
      else
      if ( buf[ 0 ] == '#' )
	{/* command */
	  value = buf + 1;
	  index = 0;
	  while ( isdigit( value[ index ] ) ) index++;
	  if ( index == 0 )
	    {/* bad command or filename */
	      if ( current < item->value_num )
		{
		  item->specified = 1;
		  __opt_value_let( item, current, buf );
		  current++;
		  continue;
		}
	      fprintf( stderr, "I CAN'T UNDERSTAND THIS LINE (\"%s\"). \n", buf );
	      continue;
	    }

	  command = value[ index ]; value[ index ] = '\0';
	  number = atol( value );
	  if (( number < 0 ) || ( number >= item->value_num ))
	    {/* bad number */
	      fprintf( stderr, "THE VALUE NUMBER (%d) IS WRONG.\n", number );
	      fprintf( stderr, "THE VALUE'S CAPACITY IS %d.\n", item->value_num );
	      continue;
	    }
	  newvalue = __str_side_skip_whitespace( value + index + 1 );
	  switch ( command )
	    {
	    case '+':
	      {
		int j, num, *values[256];
		num = strsplit( newvalue, values, " \t" );
		for ( j = num - 1; j >= 0; j-- )
		  {
		    __opt_value_insert( item, number, values[ j ] );
		    fprintf( stderr, "ADDED THE VALUE \"%s\" BEFORE #%d.\n", values[ j ], number );
		    current++;
		  }
	      }
	      break;
	    case '-':
	      if ( __opt_value_delete( item, number ) )
		{
		  fprintf( stderr, "DELETED THE VALUE #%d \"%s\".\n", number, item->value[ number ] );
		  current--;
		}
	      else
		{
		  fprintf( stderr, "NOT SPECIFIED THE VALUE #%d.\n", number );
		}
	      break;
	    case '=':
	      {
		int j, num, *values[256];
		num = strsplit( newvalue, values, " \t" );
		for ( j = 0; j < num; j++ )
		  {
		    if ( number >= item->value_num )
		      {
			fprintf( stderr, "CANNOT SET MORE VALUES \"%s\"... INTO THE OPTION \"%s\".\n", values[ j ],
				 item->name );
			break;
		      }
		    if ( item->value[ number ] )
		      fprintf( stderr, "CHANGE THE VALUE #%d \"%s\" TO \"%s\".\n",
			       number, item->value[ number ], values[ j ] );
		    else
		      fprintf( stderr, "SET THE VALUE \"%s\" IN #%d.\n", values[ j ], number );
		    __opt_value_let( item, number, values[ j ] );
		    number ++;
		  }
		current = number;
	      }
	      break;
	    case '.':
	      if ( current != number )
		{
		  current = number;
		  fprintf( stderr, "CHANGE THE CURRENT TO #%d.\n", number );
		}
	      break;
	    case '\0':
	    case '?':
	      if ( item->value[ number ] )
		fprintf( stderr, "THE VALUE #%d IS \"%s\".\n", number, item->value[ number ] );
	      else
		fprintf( stderr, "THE VALUE #%d IS NOT SPECIFIED.\n", number  );
	      break;
	    default:
	      {/* bad commnad */
		fprintf( stderr, "I CAN'T UNDERSTAND THIS COMMNAD (%c).\n", command );
		continue;
	      }
	      break;
	    }
	  continue;
	}
      else
      if ( current < item->value_num )
	{
	  int j, num, *values[256];

	  item->specified = 1;
	  num = strsplit( buf, values, " \t" );
	  for ( j = 0; j < num; j++ )
	    {
	      if ( current >= item->value_num )
		{
		  fprintf( stderr, "CANNOT SET MORE VALUE \"%s\"... INTO THE OPTION \"%s\".\n", values[ j ],
			  item->name );
		  break;
		}
	      if ( item->value[ current ] )
		fprintf( stderr, "CHENGE THE VALUE #%d \"%s\" TO \"%s\".\n",
			 current, item->value[ current ], values[ j ] );
	      else
		fprintf( stderr, "SET THE VALUE \"%s\" IN #%d.\n", values[ j ], current );
	      __opt_value_let( item, current, values[ j ] );
	      current ++;
	    }
	  continue;
	}
      else
	{
	  /* bad command */
	  fprintf( stderr, "I CAN'T UNDERSTAND THIS LINE (\"%s\").\n", buf );
	  continue;
	}
    }
}



/*------------------------------------------------------------*
 * __opt_value_input : »ØÄê¤µ¤ì¤Æ¤¤¤Ê¤¤¥ª¥×¥·¥ç¥ó¤òÍ×µá¤¹¤ë
 *------------------------------------------------------------*/
static void __opt_value_input
  _P1 (( struct optitem *, item ))
{
  long i;
  int has_value_num = 0;

  if ( strlength( item->name ) == 0 ) return;
  if ( strequal( item->name, "help" ) ) return;

  fprintf( stderr, "PLEASE SPECIFY <%s> : %s", item->specifier, item->description );

  for ( i = 0; i < item->value_num; i++ )
    if ( item->value[ i ] ) has_value_num++;

  if ( has_value_num > 0 )
    {
      fprintf( stderr, " (" );
      for ( i = 0; i < item->value_num; i++ )
	if ( item->value[ i ] )
	  {
	    fprintf( stderr, "%s", item->value[ i ] );
	    if ( i != ( has_value_num - 1 ) ) fprintf( stderr, "," );
	  }
      fprintf( stderr, ")" );
    }

  fprintf( stderr, ".\n" );

  switch ( item->value_num )
    {
    case 0:
      __opt_value_input_0( item );
      break;

    case 1:
      __opt_value_input_1( item );
      break;

    default:
      __opt_value_input_n( item );
      break;
    }
}



/*=======================================*
 * optinput : ¥ª¥×¥·¥ç¥ó¤ÎÆþÎÏÍ×µá¤ò¤¹¤ë
 *=======================================*/
void optinput
  _P1 (( char *, name ))
{
  int i;
  struct optitem *item;

  if ( name == 0 )
    {/* ²¿¤â¤«¤â */
      for ( i = 0; i < optnum; i++ )
	{
	  item = &optlist[ i ];
	  if ( item->type == TYPE_DESCRIPTION ) continue;
	  __opt_value_input( item );
	  if ( strequal( item->name, "help" ) ) continue;
	  if ( strlength( item->name ) == 0 ) continue;
	  fprintf( stderr, "\n" );
	}
    }
  else
    {/* »ØÄê¤µ¤ì¤¿Ì¾Á° */
      item = __opt_search( name );
      if ( item == 0 )
	{
	  fprintf( stderr, "I DON'T HAVE THIS OPTION \"%s\".\n", name );
	  return;
	}
      __opt_value_input( item );
      return;
    }
}



/*===================================================*
 * optinputconfigure : ¥³¥ó¥Õ¥£¥°¥Õ¥¡¥¤¥ë¤òÆÉ¤ß¹þ¤à
 *===================================================*/
long optinputconfigure
  _P1 (( char *, filename )) /* ¥³¥ó¥Õ¥£¥°¥Õ¥¡¥¤¥ëÌ¾ */
{
  int i, j, num;
  FILE *fp;
  char buf[ 1024 ];
  char *head[ 64 ];

  fp = fp_open( filename, "r" );
  if ( fp == 0 )
    {
      fprintf( stderr, "I CANNOT OPEN THE CONFIGURE FILE \"%s\".\n", filename );
      fprintf( stderr, "I DID NOT READ THIS FILE.\n" );
      return 0;
    }

  while ( fp_gets( fp, buf, sizeof( buf ) ) )
    {
      struct optitem *item;

      if ( buf[ 0 ] == '\n' ) continue;
      if ( buf[ 0 ] == '#' ) continue;

      num = strsplit( buf, head, " ()=\t\n" );
      if ( num == 0 ) continue;

      if ( num == 1 ) continue;

      if (( num >= 4 ) &&
	  strequal( head[ 0 ], "set"     ) &&
	  strequal( head[ 1 ], "option"  ) &&
	  strequal( head[ 2 ], "$option" ))
	{
	  item = __opt_search( head[ 3 ] );
	  if ( item == 0 ) continue;
	  if (( item->specified ) && ( item->value_num != OPT_INFINITE )) continue;
	  item->specified = 1;

	  if ( item->value_num == OPT_INFINITE )
	    {
	      for ( j = 4; ( j < num ) && (( j - 4 ) < item->value_num ); j++ )
		{
		  if ( strequal( head[ j ], "--" ) ) break;
		  if ( strequal( head[ j ], "\\" ) ) break;
		  if ( strequal( head[ j ], ")" ) ) break;
		  __opt_value_append( item, head[ j ] );
		}
	      if ( strequal( head[ j ], "--" ) ) continue;
	      if ( strequal( head[ j ], ")" ) ) continue;
	      if ( strequal( head[ j ], "\\" ) )
		{
		  while ( fp_gets( fp, buf, sizeof( buf ) ) )
		    {
		      num = strsplit( buf, head, " ()=\t\n" );
		      for ( j = 0; j < num; j++ )
			{
			  if ( strequal( head[ j ], "--" ) ) break;
			  if ( strequal( head[ j ], "\\" ) ) break;
			  if ( strequal( head[ j ], ")" ) ) break;
			  __opt_value_append( item, head[ j ] );
			}
		      if ( strequal( head[ j ], "--" ) ) break;
		      if ( strequal( head[ j ], ")" ) ) break;
		    }
		}
	    }
	  else
	    {
	      for ( j = 4; ( j < num ) && (( j - 4 ) < item->value_num ); j++ )
		{
		  if ( strequal( head[ j ], "--" ) ) break;
		  item->value[ j - 4 ] = strnew( head[ j ] );
		}
	    }
	}
    }

  fp_close( fp );
  return 1;
}



/*------------------------------------------------------------*
 * __opt_ignore_option : Ìµ»ë¤¹¤ë¥ª¥×¥·¥ç¥ó¤«¤É¤¦¤«
 *------------------------------------------------------------*/
static long __opt_ignore_option
  _P1 (( struct optitem *, item ))
{
  int i;
  static char *ignore[] = { "help", "configure",
			    "output-configure", "input-configure" };

  if ( item->type == TYPE_DESCRIPTION ) return 1;

  for ( i = 0; i < Number( ignore ); i++ )
    if ( strequal( item->name, ignore[ i ] ) ) return 1;

  return 0;
}



/*===================================================*
 * optoutputconfigure : ¥³¥ó¥Õ¥£¥°¥Õ¥¡¥¤¥ë¤ò½ñ¤­¹þ¤à
 *===================================================*/
long optoutputconfigure
  _P1 (( char *, filename )) /* ¥³¥ó¥Õ¥£¥°¥Õ¥¡¥¤¥ëÌ¾ */
{
  int i, j;
  FILE *fp;
  char *user;

  fp = fp_open( filename, "w" );
  if ( fp == 0 )
    {
      fprintf( stderr, "I CANNOT OPEN THE CONFIGURE FILE \"%s\".\n", filename );
      fprintf( stderr, "I DID NOT WRITE THIS FILE.\n" );
      return 0;
    }

  user = getenv( "USER" );
  if ( user == 0 ) user = "anonymous";

  fp_printf( fp, "#! /bin/csh -f\n" );
  fp_printf( fp, "#\n" );
  fp_printf( fp, "# This file is a configuration file of %s.\n", optcommand );
  fp_printf( fp, "#\n" );
  fp_printf( fp, "#     author   : %s\n", user );
  fp_printf( fp, "#     date     : %s\n", timedate( 0 ) );
  fp_printf( fp, "#     filename : %s\n", filename );
  fp_printf( fp, "#     title    : The configure file of %s.\n", optcommand );
  fp_printf( fp, "#\n" );

  fp_printf( fp, "\n" );
  fp_printf( fp, "\n" );
  fp_printf( fp, "\n" );

  fp_printf( fp, "#\n" );
  fp_printf( fp, "# BEGIN { SPECIFY PROGRAM }\n" );
  fp_printf( fp, "#\n" );
  fp_printf( fp, "\n" );
  fp_printf( fp, "set program = %s\n", optcommand );
  fp_printf( fp, "\n" );
  fp_printf( fp, "#\n" );
  fp_printf( fp, "# END { SPECIFY PROGRAM }\n" );
  fp_printf( fp, "#\n" );

  fp_printf( fp, "\n" );
  fp_printf( fp, "\n" );
  fp_printf( fp, "\n" );

  fp_printf( fp, "#\n" );
  fp_printf( fp, "# BEGIN { SPECIFY OPTIONS }\n" );
  fp_printf( fp, "#\n" );
  fp_printf( fp, "\n" );
  fp_printf( fp, "set option = \"\"\n" );
  fp_printf( fp, "\n" );

  for ( i = 0; i < optnum; i++ )
    {
      struct optitem *item = &optlist[i];

      if ( item->type == TYPE_DESCRIPTION )
	{
	  if ( strlength( item->description ) != 0 )
	    {
	      fp_printf( fp, "#\n" );
	      fp_printf( fp, "# %s\n", item->description );
	      fp_printf( fp, "#\n" );
	    }
	  continue;
	}
      if ( __opt_ignore_option( item ) ) continue;

      if ( strequal( item->value_type, "value" ) )
	fp_printf( fp, "# <%s> : %s", item->name, item->description );
      else
	fp_printf( fp, "# <%s> : (%s) %s", item->name, item->value_type, item->description );
      __opt_print_default_value( fp, item );
      fp_printf( fp, ".\n" );

      if ( ! item->specified ) fp_printf( fp, "#" );
      if ( item->type == TYPE_OPTION )
	fp_printf( fp, "set option = ( $option \"%s\"", item->specifier );
      else
	fp_printf( fp, "set option = ( $option \"--%s\"", item->name );

      for ( j = 0; j < item->value_num; j++ )
	if ( item->value[j] )
	  {
	    if ( item->value_num == OPT_INFINITE )
	      {
		fp_printf( fp, " \\\n\t\t       \"%s\"", item->value[j] );
	      }
	    else
	      fp_printf( fp, " \"%s\"", item->value[j] );
	  }
      if ( item->value_num == OPT_INFINITE )
	fp_printf( fp, " \\\n\t\t       \"--\"" );

      fp_printf( fp, " )\n\n" );
    }

  fp_printf( fp, "#\n" );
  fp_printf( fp, "# END { SPECIFY OPTIONS }\n" );
  fp_printf( fp, "#\n" );

  fp_printf( fp, "\n" );
  fp_printf( fp, "\n" );
  fp_printf( fp, "\n" );

  fp_printf( fp, "#\n" );
  fp_printf( fp, "# BEGIN { EXEC PROGRAM }\n" );
  fp_printf( fp, "#\n" );
  fp_printf( fp, "\n" );
  fp_printf( fp, "echo $program $option $argv\n" );
  fp_printf( fp, "$program $option $argv\n" );
  fp_printf( fp, "\n" );
  fp_printf( fp, "#\n" );
  fp_printf( fp, "# END { EXEC PROGRAM }\n" );
  fp_printf( fp, "#\n" );

  fp_close( fp );

  {
    char command[ 512 ];
    sprintf( command, "chmod +x %s", filename );
    system( command );
  }

  return 1;
}



/*------------------------------------------*
 * __optsh_help : ¥Ø¥ë¥×¤ò¥×¥ê¥ó¥È¤¹¤ë
 *------------------------------------------*/
static void __optsh_help()
{
  fprintf( stderr, "------------------------------ HELP MESSAGE ------------------------------\n" );
  fprintf( stderr, "    ?                            ... PRINT THIS MESSAGES.\n" );
  fprintf( stderr, "    quit                         ... FINISH THE OPTION SHELL.\n" );
  fprintf( stderr, "    usage                        ... PRINT THE USAGE OF THIS COMMAND.\n" );
  fprintf( stderr, "    help                         ... PRINT THE LONG HELP OF THIS COMMAND.\n" );
  fprintf( stderr, "    show *                       ... SHOW ALL OPTIONS.\n" );
  fprintf( stderr, "    show <options>               ... SHOW OPTIONS.\n" );
  fprintf( stderr, "    set *                        ... SET ALL OPTIONS.\n" );
  fprintf( stderr, "    set <option> = <value[s]>    ... SET OPTION IMMIDIATELY.\n" );
  fprintf( stderr, "    set <options>                ... SET OPTIONS.\n" );
  fprintf( stderr, "    unset *                      ... UNSET ALL OPTIONS.\n" );
  fprintf( stderr, "    unset <options>              ... UNSET OPTIONS.\n" );
  fprintf( stderr, "    input/read/load <filename>   ... INPUT CONFIGURE FILE.\n" );
  fprintf( stderr, "    output/write/save <filename> ... OUTPUT CONFIGURE FILE.\n" );
  fprintf( stderr, "    source [<type>]              ... INPUT RESOURCE FILE.\n" );
  fprintf( stderr, "    resource [<type>]            ... OUTPUT RESOURCE FILE.\n" );
  fprintf( stderr, "    run                          ... RUN THIS PROGRAM.\n" );
  fprintf( stderr, "    <UNIX command>               ... EXEC UNIX COMMAND.\n" );
  fprintf( stderr, "--------------------------------------------------------------------------\n" );
}



/*------------------------------------------------------------*
 * __opt_print_unit_short : ¥ª¥×¥·¥ç¥ó¤ò´ÊÎ¬·¿¤ÇÉ½¼¨¤¹¤ë
 *------------------------------------------------------------*/
static void __opt_print_unit_short
  _P2 (( struct optitem *, item ),
       ( char *          , line ))
{
  int i;

  if ( item->specified ) fprintf( stderr, "%s [   SPECIFIED   ]", line );
  else			 fprintf( stderr, "%s [ NOT SPECIFIED ]", line );

  if ( item->type == TYPE_OPTION )
    fprintf( stderr, " %s", item->specifier );

  if ( item->value_num == 0 )
    {
      if ( item->specified ) fprintf( stderr, " [TRUE]" );
      else		     fprintf( stderr, " [FALSE]" );
    }
  else
  if ( item->value_num == OPT_INFINITE )
    {
      if ( __opt_value_compact( item ) == 0 )
	{
	  fprintf( stderr, " <%s>...", item->value_type );
	}
      else
	{
	  for ( i = 0; i < item->value_num ; i++ )
	    if ( item->value[ i ] )
	      fprintf( stderr, " \"%s\"", item->value[ i ] );
	}
    }
  else
    {
      for ( i = 0; i < item->value_num ; i++ )
	if ( item->value[ i ] )
	  fprintf( stderr, " \"%s\"", item->value[ i ] );
	else
	  fprintf( stderr, " <%s>", item->value_type );
    }

  fprintf( stderr, "\n" );
}



/*------------------------------------------------------------*
 * __opt_print_unit_long : ¥ª¥×¥·¥ç¥ó¤òÉ½¼¨¤¹¤ë
 *------------------------------------------------------------*/
static void __opt_print_unit_long
  _P1 (( struct optitem *, item ))
{
  int i, tab;

  if ( strequal( item->value_type, "value" ) )
    fprintf( stderr, "    <%s> : %s", item->name, item->description );
  else
    fprintf( stderr, "    <%s> : (%s) %s", item->name, item->value_type, item->description );
  __opt_print_default_value( stderr, item );
  fprintf( stderr, ".\n" );

  if ( item->specified )
    {
      tab = strlen( "        [ SPECIFIED ]" );
      fprintf( stderr, "        [ SPECIFIED ]" );
    }
  else
    {
      tab = strlen( "        [ NOT SPECIFIED ]" );
      fprintf( stderr, "        [ NOT SPECIFIED ]" );
    }

  if ( item->type == TYPE_OPTION )
    {
      fprintf( stderr, " %s", item->specifier );
      tab += strlen( item->specifier ) + 1;
    }

  if ( item->value_num == 0 )
    {
      if ( item->specified ) fprintf( stderr, " [TRUE]" );
      else		     fprintf( stderr, " [FALSE]" );
      fprintf( stderr, "\n" );
    }
  else
  if ( item->value_num == OPT_INFINITE )
    {
      if ( __opt_value_compact( item ) == 0 )
	{
	  fprintf( stderr, " ...\n" );
	}
      else
	{
	  int j, first = 1;
	  for ( i = 0; i < item->value_num ; i++ )
	    if ( item->value[ i ] )
	      {
		if ( ! first )
		  for ( j = 0; j < tab; j++ ) fputc( ' ', stderr );
		fprintf( stderr, " [#%d] \"%s\"\n", i, item->value[ i ] );
		first = 0;
	      }
	  if ( first ) fprintf( stderr, "\n" );
	}
    }
  else
    {
      int j;
      for ( i = 0; i < item->value_num ; i++ )
	if ( item->value[ i ] )
	  fprintf( stderr, " \"%s\"", item->value[ i ] );
	else
	  fprintf( stderr, " \"\"", item->value[ i ] );
      fprintf( stderr, "\n" );
    }
}



/*-------------------------------------------------*
 * __optsh_print_all : ¥ª¥×¥·¥ç¥ó¤òÉ½¼¨¤¹¤ë
 *-------------------------------------------------*/
static void __optsh_print_all()
{
  int i, j, maxlen, oplen;
  char line[256];

  fprintf( stderr, "PRINT ALL OPTIONS ...\n" );

  maxlen = 0;
  for ( i = 0; i < optnum; i++ )
    {
      struct optitem *item = &optlist[ i ];
      int len;
      if ( strlength( item->name ) == 0 ) continue;
      if ( strequal( item->name, "help" ) ) continue;
      len = strlen( item->name );
      if ( len > maxlen ) maxlen = len;
    }

  oplen = strlen( "    <>" );
  for ( i = 0; i < optnum; i++ )
    {
      struct optitem *item = &optlist[ i ];

      if ( item->type == TYPE_DESCRIPTION )
	{
	  fprintf( stderr, "%s\n", item->description );
	  continue;
	}
      if ( strequal( item->name, "help" ) ) continue;

      __opt_print_unit_long( item );
    }
}



/*-------------------------------------------------*
 * __optsh_print_all_short : ¥ª¥×¥·¥ç¥ó¤òÉ½¼¨¤¹¤ë
 *-------------------------------------------------*/
static void __optsh_print_all_short()
{
  int i, j, maxlen, oplen;
  char line[256];
  int last_value;

  fprintf( stderr, "PRINT ALL OPTIONS IN SHORT...\n" );

  maxlen = 0;
  for ( i = 0; i < optnum; i++ )
    {
      struct optitem *item = &optlist[ i ];
      int len;
      if ( strlength( item->name ) == 0 ) continue;
      if ( strequal( item->name, "help" ) ) continue;
      len = strlen( item->name );
      if ( len > maxlen ) maxlen = len;
    }

  oplen = strlen( "    <>" );
  for ( i = 0; i < optnum; i++ )
    {
      struct optitem *item = &optlist[ i ];

      if ( item->type == TYPE_DESCRIPTION ) continue;
      if ( strequal( item->name, "help" ) ) continue;

      sprintf( line, "    <%s>", item->name );
      for ( j = strlen( line ); j < maxlen + oplen; j++ ) line[ j ] = ' ';
      line[ j ] = '\0';
      __opt_print_unit_short( item, line );
    }
}



/*-------------------------------------------------*
 * __optsh_print : ¥ª¥×¥·¥ç¥ó¤òÉ½¼¨¤¹¤ë
 *-------------------------------------------------*/
static void __optsh_print
  _P1 (( char *, name ))
{
  int i, j;
  struct optitem *item;
  int shorter = 0;
  char buf[ 256 ];
  int tab;

  if ( strequal( name, "*" ) )
    {
      __optsh_print_all();
      return;
    }

  if ( strequal( name, "?" ) )
    {
      __optsh_print_all_short();
      return;
    }

  item = __opt_search( name );
  if ( item == 0 )
    {/* BAD OPTION NAME */
      fprintf( stderr, "I DON'T HAVE THIS OPTION \"%s\".\n", name );
      return;
    }

  if ( name[ 0 ] == '-' )
    {
      sprintf( buf, "    <%s> ", item->name );
      __opt_print_unit_short( item, buf );
      return;
    }
  else
    {
      __opt_print_unit_long( item );
    }
}



/*-------------------------------------------------*
 * __optsh_input : ¥ª¥×¥·¥ç¥ó¤òÆþÎÏ¤¹¤ë
 *-------------------------------------------------*/
static void __optsh_input
  _P1 (( char *, name ))
{
  if ( strequal( name, "*" ) ) name = 0;
  optinput( name );
}




/*-------------------------------------------------*
 * __optsh_clear : ¥ª¥×¥·¥ç¥ó¤òÌ¤»ØÄê¤Ë¤¹¤ë
 *-------------------------------------------------*/
static void __optsh_clear
  _P1 (( char *, name ))
{
  int i, j;
  struct optitem *item;

  if ( strequal( name, "*" ) )
    {
      for ( i = 0; i < optnum; i++ )
	{
	  item = &optlist[ i ];
	  if ( item->type == TYPE_DESCRIPTION ) continue;
	  __opt_unspecified( item );
	}
      fprintf( stderr, "UNSET ALL OPTIONS.\n" );
      return;
    }

  item = __opt_search( name );
  if ( item == 0 )
    {/* BAD OPTION NAME */
      fprintf( stderr, "I DON'T HAVE THIS OPTION \"%s\"\n", name );
      return;
    }

  __opt_unspecified( item );
  fprintf( stderr, "UNSET THIS OPTION \"%s\".\n", name );
}



/*--------------------------------------------------*
 * __optsh_set_value : ¥ª¥×¥·¥ç¥ó¤ËÃÍ¤òÀßÄê¤¹¤ë
 *--------------------------------------------------*/
static void __optsh_set_value
  _P2 (( char *, name ),
       ( char *, buf  ))
{
  struct optitem *item;
  int i, num, value_num;
  char *head[ 512 ];

  item = __opt_search( name );
  if ( item == 0 )
    {
      fprintf( stderr, "I DON'T HAVE THIS OPTION \"%s\".\n", name );
      return;
    }

  num = strsplit( buf, head, " \t()\n" );
  value_num = num - 3;

  if ( value_num == 0 )
    {
      __optsh_input( name );
      return;
    }

  if (( value_num == 1 ) && ( item->value_num == 0 ))
    {
      char *value = head[ 3 ];
      if ( strxequal( value, "t" ) || strxequal( value, "true" ) ||
	   strxequal( value, "y" ) || strxequal( value, "yes" ) )
	{
	  fprintf( stderr, "SET THE OPTION \"%s\".\n", item->name );
	  item->specified = 1;
	}
      else
      if ( strxequal( value, "f" ) || strxequal( value, "false" ) ||
	   strxequal( value, "n" ) || strxequal( value, "no" ) )
	{
	  fprintf( stderr, "UNSET THE OPTION \"%s\".\n", item->name );
	  item->specified = 0;
	}
      else
	fprintf( stderr, "BAD VALUE \"%s\".\n", value );
      return;
    }

  if ( value_num > item->value_num )
    {
      fprintf( stderr, "TOO MANY VALUES (#%d). ", value_num );
      if ( item->value_num == 1 )
	fprintf( stderr, "THE OPTION \"%s\" NEEDS JUST ONE VALUE.\n", name );
      else
	fprintf( stderr, "THE OPTION \"%s\" NEEDS JUST #%d VALUES.\n", name, item->value_num );
      return;
    }

  for ( i = 0; i < value_num; i++ )
    {
      char *value = head[ 3 + i ];
      if ( value_num == 1 )
	fprintf( stderr, "SET A VALUE \"%s\" TO THE OPTION \"%s\".\n", value, item->name );
      else
	fprintf( stderr, "SET A VALUE #%d \"%s\" TO THE OPTION \"%s\".\n", i, value, item->name );
      __opt_value_let( item, i, value );
    }
}



/*--------------------------------------------------*
 * __optsh_add_set_value : ¥ª¥×¥·¥ç¥ó¤ËÃÍ¤òÀßÄê¤¹¤ë
 *--------------------------------------------------*/
static void __optsh_add_set_value
  _P2 (( char *, name ),
       ( char *, buf  ))
{
  struct optitem *item;
  int i, num, value_num;
  char *head[ 512 ];

  item = __opt_search( name );
  if ( item == 0 )
    {
      fprintf( stderr, "I DON'T HAVE THIS OPTION \"%s\".\n", name );
      return;
    }

  num = strsplit( buf, head, " \t()\n" );
  value_num = num - 3;

  if ( value_num == 0 )
    {
      __optsh_input( name );
      return;
    }

  if (( value_num == 1 ) && ( item->value_num == 0 ))
    {
      char *value = head[ 3 ];
      if ( strxequal( value, "t" ) || strxequal( value, "true" ) ||
	   strxequal( value, "y" ) || strxequal( value, "yes" ) )
	{
	  fprintf( stderr, "SET THE OPTION \"%s\".\n", item->name );
	  item->specified = 1;
	}
      else
      if ( strxequal( value, "f" ) || strxequal( value, "false" ) ||
	   strxequal( value, "n" ) || strxequal( value, "no" ) )
	{
	  fprintf( stderr, "UNSET THE OPTION \"%s\".\n", item->name );
	  item->specified = 0;
	}
      else
	fprintf( stderr, "BAD VALUE \"%s\".\n", value );
      return;
    }

  if ( value_num > item->value_num )
    {
      fprintf( stderr, "TOO MANY VALUES (#%d). ", value_num );
      if ( item->value_num == 1 )
	fprintf( stderr, "THE OPTION \"%s\" NEEDS JUST ONE VALUE.\n", name );
      else
	fprintf( stderr, "THE OPTION \"%s\" NEEDS JUST #%d VALUES.\n", name, item->value_num );
      return;
    }

  for ( i = 0; i < value_num; i++ )
    {
      char *value = head[ 3 + i ];
      if ( __opt_value_append( item, value ) == OPT_VALUE_ADDED )
	{
	  fprintf( stderr, "ADD A VALUE \"%s\" INTO THE OPTION \"%s\".\n", value, item->name );
	  item->specified = 1;
	}
      else
	{
	  fprintf( stderr, "CANNOT ADD MORE VALUE \"%s\"... INTO THE OPTION \"%s\".\n", value, item->name );
	  break;
	}
    }
}



/*============================================================*
 * optshellmode : ¥ª¥×¥·¥ç¥ó¥·¥§¥ë¥â¡¼¥É¤ËÆþ¤ë
 *============================================================*/
void optshellmode()
{
  int i, num;
  char command[ 1024 ];
  char buf[ 1024 ];
  char org[ 1024 ];
  char *head[64];

  fprintf( stderr, "WELCOME TO THE OPTION SHELL MODE.\n" );

  while ( 1 )
    {
      fprintf( stderr, "    %s) ", optcommand );

      __opt_getline( buf );

      strcpy( org, buf );
      num = strsplit( buf, head, " \t\n" );
      if ( num == 0 ) continue;

      if ( strcmp( head[ 0 ], "?" ) == 0 )
	{
	  __optsh_help();
	  continue;
	}

      if ( strcmp( head[ 0 ], "setenv" ) == 0 )
	{
	  if ( num >= 3 )
	    {
	      char envs[ 1024 ];
	      sprintf( envs, "%s=%s", head[ 1 ], head[ 2 ] );
	      putenv( envs );
	    }
	  continue;
	}

      if ( strxequal( head[ 0 ], "quit" ) )
	{
	  fprintf( stderr, "QUIT\n" );
	  exit( 0 );
	}

      if ( strxequal( head[ 0 ], "run" ) )
	{
	  break;
	}

      if ( strxequal( head[ 0 ], "usage" ) )
	{
	  sprintf( command, "%s -usage", optcommand );
	  system( command );
	  continue;
	}

      if ( strxequal( head[ 0 ], "help" ) )
	{
	  sprintf( command, "%s -help", optcommand );
	  system( command );
	  continue;
	}

      if ( strxequal( head[ 0 ], "show" ) )
	{
	  if ( num == 1 ) __optsh_print( "?" );
	  for ( i = 1; i < num; i++ ) __optsh_print( head[ i ] );
	  continue;
	}

      if ( strxequal( head[ 0 ], "set" ) )
	{
	  if ( num == 1 ) __optsh_print( "?" );
	  if (( num >= 3 ) && strequal( head[ 2 ], "=" ))
	    {/* immidiate value */
	      __optsh_set_value( head[ 1 ], org );
	    }
	  else
	  if (( num >= 3 ) && strequal( head[ 2 ], "+=" ))
	    {
	      __optsh_add_set_value( head[ 1 ], org );
	    }
	  else
	    for ( i = 1; i < num; i++ ) __optsh_input( head[ i ] );
	  continue;
	}

      if ( strxequal( head[ 0 ], "unset" ) )
	{
	  if ( num == 1 ) fprintf( stderr, "Usage: unset <options>\n" );
	  for ( i = 1; i < num; i++ ) __optsh_clear( head[ i ] );
	  continue;
	}

      if ( strxequal( head[ 0 ], "output" ) ||
	   strxequal( head[ 0 ], "write" ) ||
	   strxequal( head[ 0 ], "save" ) )
	{
	  if ( num == 1 ) fprintf( stderr, "Usage: output <filename>\n" );
	  else
	    {
	      optoutputconfigure( head[ 1 ] );
	      fprintf( stderr, "OUTPUT THE CONFIGURE FILE \"%s\".\n", head[ 1 ] );
	    }
	  continue;
	}

      if ( strxequal( head[ 0 ], "input" ) ||
	   strxequal( head[ 0 ], "read" ) ||
	   strxequal( head[ 0 ], "load" ))
	{
	  if ( num == 1 ) fprintf( stderr, "Usage: input <filename>\n" );
	  else
	    {
	      if ( optinputconfigure( head[ 1 ] ) )
		fprintf( stderr, "INPUT THE CONFIGURE FILE \"%s\".\n", head[ 1 ] );
	    }
	  continue;
	}

      if ( strxequal( head[ 0 ], "resource" ) )
	{
	  char *resource_type = 0;
	  char  resource_file[ 256 ];
	  int error_code;

	  if ( num >= 2 ) resource_type = head[ 1 ];

	  error_code = __optresource_save( resource_type, resource_file );

	  if ( error_code == 0 )
	    fprintf( stderr, "SAVE RESOURCE FILE \"%s\".\n", resource_file );

	  continue;
	}

      if ( strxequal( head[ 0 ], "source" ) )
	{
	  char *resource_type = 0;
	  char  resource_file[ 256 ];
	  int error_code;

	  if ( num >= 2 ) resource_type = head[ 1 ];

	  error_code = __optresource_load( resource_type, resource_file );

	  switch ( error_code )
	    {
	    case 0:
	      fprintf( stderr, "LOAD RESOURCE FILE \"%s\".\n", resource_file );
	      break;
	    case -1:
	      fprintf( stderr, "CANNOT ACCESS \"%s\".\n", resource_file );
	      break;
	    }
	  continue;
	}

      if ( strxsubequal( head[ 0 ], "rm", 2 ) )
	{
	  fprintf( stderr, "SORRY, I CANNOT EXEC \"rm*\" COMMAND.\n" );
	  continue;
	}

      system( org );
    }
}



/*-----------------------------------------------------------------*
 * __opt_value_compact : °ú¿ô¤ò¤Þ¤È¤á¤ë
 *-----------------------------------------------------------------*/
static long __opt_value_compact
  _P1 (( struct optitem *, item ))
{
  int i, j;

  for ( j = i = 0; i < item->value_num; i++ )
    {
      if ( item->value[ i ] )
	item->value[ j++ ] = item->value[ i ];
    }

  for ( i = j; i < item->value_num; i++ )
    item->value[ i ] = 0;

  return j;
}



/*--------------------------------------------------*
 * __opt_argument_append : ¥ª¥×¥·¥ç¥ó¤ËÃÍ¤òÄÉ²Ã¤¹¤ë
 *--------------------------------------------------*/
static long __opt_argument_append
  _P1 (( char *, value ))
{
  int i;
  struct optitem *item;

  for ( i = 0; i < optnum; i++ )
    {
      item = &optlist[ i ];
      if ( item->type != TYPE_ARGUMENT ) continue;
      if ( __opt_value_append( item, value ) == OPT_VALUE_ADDED ) break;
    }

  if ( i == optnum )
    {
      __optarg_append( value );
      return OPT_VALUE_NOT_ADDED;
    }

  return OPT_VALUE_ADDED;
}



/*------------------------------------------------*
 * __opt_value_append : ¥ª¥×¥·¥ç¥ó¤ËÃÍ¤òÄÉ²Ã¤¹¤ë
 *------------------------------------------------*/
static long __opt_value_append
  _P2 (( struct optitem *, item  ),
       ( char *          , value ))
{
  int index;

  index = __opt_value_compact( item );
  if ( index >= item->value_num ) return OPT_VALUE_NOT_ADDED;

  item->specified = 1;

  item->value[ index ] = strnew( value );

  return OPT_VALUE_ADDED;
}



/*------------------------------------------------*
 * __opt_value_delete : ¥ª¥×¥·¥ç¥ó¤ËÃÍ¤òºï½ü¤¹¤ë
 *------------------------------------------------*/
static long __opt_value_delete
  _P2 (( struct optitem *, item  ),
       ( long            , index ))
{
  int num;

  if ( __opt_value_free( item, index ) == 0 )
    return 0;

  num = __opt_value_compact( item );

  if ( num == 0 )
    item->specified = 0;
  else
    item->specified = 1;

  return 1;
}



/*------------------------------------------------*
 * __opt_value_free : ¥ª¥×¥·¥ç¥ó¤ËÃÍ¤òÌµ¸ú¤Ë¤¹¤ë
 *------------------------------------------------*/
static long __opt_value_free
  _P2 (( struct optitem *, item  ),
       ( long            , index ))
{
  if (( index < 0 ) || ( index >= item->value_num ))
    return 0;

  if ( item->value[ index ] )
    {
      strfree( item->value[ index ] );
      item->value[ index ] = 0;
      return 1;
    }

  return 0;
}



/*------------------------------------------------*
 * __opt_value_insert : ¥ª¥×¥·¥ç¥ó¤ËÃÍ¤òÁÞÆþ¤¹¤ë
 *------------------------------------------------*/
static long __opt_value_insert
  _P3 (( struct optitem *, item  ),
       ( long            , index ),
       ( char *          , value ))
{
  int i;
  int num;

  num = __opt_value_compact( item );

  if ( item->value[ item->value_num - 1 ] )
    strfree( item->value[ item->value_num - 1 ] );

  for ( i = item->value_num - 1; i > index; i-- )
    item->value[ i ] = item->value[ i - 1 ];

  item->specified = 1;

  if ( index < 0 || index >= item->value_num )
    {
      return 0;
    }

  item->value[ index ] = strnew( value );

  return 1;
}



/*------------------------------------------------*
 * __opt_value_let : ¥ª¥×¥·¥ç¥ó¤ËÃÍ¤òÂåÆþ¤¹¤ë
 *------------------------------------------------*/
static long __opt_value_let
  _P3 (( struct optitem *, item  ),
       ( long            , index ),
       ( char *          , value ))
{
  if ( index < 0 || index >= item->value_num )
    {
      return OPT_VALUE_NOT_ADDED;
    }

  if ( item->value[ index ] ) strfree( item->value[ index ] );
  item->specified = 1;
  item->value[ index ] = strnew( value );

  return OPT_VALUE_ADDED;
}




/*----------------------------------------*
 * __optarg_append : Í¾¤Ã¤¿°ú¿ô¤òÄÉ²Ã¤¹¤ë
 *----------------------------------------*/
static void __optarg_append
  _P1 (( char *, value ))
{
  int index;

  index = optargc++;
  optargv = typerenew1( optargv, optargc, char * );
  optargv[ index ] = strnew( value );
}



/*==============================================================*
 * optgetarg : °ú¿ô¤ÎiÈÖÌÜ¤ÎÃÍ¤òÊÖ¤¹
 *==============================================================*/
char *optgetarg
  _P2 (( char *, name ), /* Ì¾Á° */
       ( long  , i    )) /* ¥¤¥ó¥Ç¥Ã¥¯¥¹ */
{
  char buf[ 1024 ];

  if ( i < 0 )
    {
      fprintf( stderr, "error:optgetarg:You specified a bad index (%d)\n", i );
      exit( -1 );
    }

  for ( ; optargc <= i; ) __optarg_append( "" );

  while (( optargv[ i ] == 0 ) || ( optargv[ i ][ 0 ] == '\0' ))
    {
      if ( strlength( name ) )
	fprintf( stderr, "I NEED AN ARGUMENT #%d FOR \"%s\".\n", i, name );
      else
	fprintf( stderr, "I NEED AN ARGUMENT #%d.\n", i );
      fprintf( stderr, "    INPUT (#%d) = ", i );
      __opt_getline( buf );
      if ( buf[ 0 ] == 0 ) continue;
      if ( optargv[ i ] ) strfree( optargv[ i ] );
      optargv[ i ] = strnew( buf );
    }

  return optargv[ i ];
}



/* ===========================================================*
 * optgetargint : °ú¿ô¤ÎiÈÖÌÜ¤ÎÃÍ¤òÀ°¿ôÃÍ¤ËÊÑ´¹¤·¤ÆÊÖ¤¹
 * ===========================================================*/
long optgetargint
  _P2 (( char *, name ), /* Ì¾Á° */
       ( long  , i    )) /* ¥¤¥ó¥Ç¥Ã¥¯¥¹ */
{
  return atol( optgetarg( name, i ) );
}



/* ===========================================================*
 * optgetargfloat : °ú¿ô¤ÎiÈÖÌÜ¤ÎÃÍ¤ò¼Â¿ôÃÍ¤ËÊÑ´¹¤·¤ÆÊÖ¤¹
 * ===========================================================*/
double optgetargfloat
  _P2 (( char *, name ), /* Ì¾Á° */
       ( long  , i    )) /* ¥¤¥ó¥Ç¥Ã¥¯¥¹ */
{
  return atof( optgetarg( name, i ) );
}



/*
 * for access
 */
#include <unistd.h>

/*-----------------------------------------------------------*
 * __optresource_load : ¥ê¥½¡¼¥¹¥Õ¥¡¥¤¥ë¤òÆÉ¤ß¹þ¤à
 *-----------------------------------------------------------*/
static long __optresource_load
  _P2 (( char *, resource_type ),
       ( char *, resource_file ))
{
  char *resource_dir;
  char resource_fileRec[ 256 ];
  int error_code = 0;

  if ( resource_file == 0 ) resource_file = resource_fileRec;

  resource_dir = getenv( "CIL_RESOURCE_DIR" );
  if ( resource_dir == 0 ) resource_dir = ".";

  if (( resource_type == 0 ) || ( resource_type[ 0 ] == '\0' ))
    sprintf( resource_file, "%s/%s.rc", resource_dir, optcommand );
  else
    sprintf( resource_file, "%s/%s.%s.rc", resource_dir, optcommand, resource_type );

  if ( access( resource_file, R_OK ) == 0 )
    {
      error_code = optinputconfigure( resource_file );
    }

  return error_code - 1;
}



/*------------------------------------------------------------*
 * __optresource_save : ¥ª¥×¥·¥ç¥ó¥ê¥½¡¼¥¹¤ò½ñ¤­¹þ¤à
 *------------------------------------------------------------*/
static long __optresource_save
  _P2 (( char *, resource_type ),
       ( char *, resource_file ))
{
  char *resource_dir;
  char resource_fileRec[ 256 ];
  int error_code;

  if ( resource_file == 0 ) resource_file = resource_fileRec;

  resource_dir = getenv( "CIL_RESOURCE_DIR" );
  if ( resource_dir == 0 ) resource_dir = ".";

  if (( resource_type == 0 ) || ( resource_type[ 0 ] == '\0' ))
    sprintf( resource_file, "%s/%s.rc", resource_dir, optcommand );
  else
    sprintf( resource_file, "%s/%s.%s.rc",
	     resource_dir, optcommand, resource_type );
  if ( access( resource_file, R_OK ) == 0 )
    {
      char command[ 1024 ];
      fprintf( stderr, "THE RESOURCE FILE \"%s\" EXISTS.\n", resource_file );
      fprintf( stderr, "BACKUP IT TO THE FILE \"%s~\".\n", resource_file );
      sprintf( command, "/bin/mv %s %s~", resource_file, resource_file );
      system( command );
    }

  error_code = optoutputconfigure( resource_file );

  return error_code - 1;
}



/*------------------------------------------------------------*
 * __optresource_print : ¥ê¥½¡¼¥¹¾õÂÖ¤òÉ½¼¨¤¹¤ë
 *------------------------------------------------------------*/
static void __optresource_print
  _P1 (( char *, resource_type ))
{
  int i;
  struct optitem *item;
  char resource_file[ 256 ];

  for ( i = 0; i < optnum; i++ )
    {
      item = &optlist[ i ];
      if ( item->type == TYPE_DESCRIPTION ) continue;
      __opt_unspecified( item );
    }

  __optresource_load( resource_type, resource_file );
  fprintf( stderr, "THE RESOURCE FILE IS \"%s\"\n", resource_file );
  fprintf( stderr, "OPTION RESOURCES ARE FOLLOWINGS:\n" );
  __optsh_print_all_short();

  exit( 1 );
}



/*=====================================================*
 * optifspecifiedcopy : ¤â¤·»ØÄê¤µ¤ì¤Æ¤¤¤¿¤é¥³¥Ô¡¼¤¹¤ë
 *=====================================================*/
int optifspecifiedcopy /* ¥³¥Ô¡¼¤·¤¿¤«¤É¤¦¤« */
  _P2 (( char *, name         ),
       ( char *, return_value ))
{
  struct optitem *item;

  item = __opt_search( name );

  if ( item == 0 )
    {
      fprintf( stderr, "PROGRAM-ERROR:optifspecfiedcopy:I DON'T HAVE THE OPTION \"%s\".\n", name );
      exit( -1 );
    }

  if ( return_value == 0 )
    {
      fprintf( stderr, "PROGRAM-ERROR:optifspecfiedcopy:The return value pointer is null at \"%s\".\n", name );
      exit( -1 );
    }

  if ( item->specified )
    strcopy( return_value, item->value[ 0 ] );

  return item->specified;
}



/*================================================================*
 * optifspecifiedcopyint : ¤â¤·»ØÄê¤µ¤ì¤Æ¤¤¤¿¤éÀ°¿ôÃÍ¤ò¥³¥Ô¡¼¤¹¤ë
 *================================================================*/
int optifspecifiedcopyint /* ¥³¥Ô¡¼¤·¤¿¤«¤É¤¦¤« */
  _P2 (( char *, name         ),
       ( long *, return_value ))
{
  struct optitem *item;

  item = __opt_search( name );

  if ( item == 0 )
    {
      fprintf( stderr, "PROGRAM-ERROR:optifspecfiedcopyint:I DON'T HAVE THE OPTION \"%s\".\n", name );
      exit( -1 );
    }

  if ( return_value == 0 )
    {
      fprintf( stderr, "PROGRAM-ERROR:optifspecfiedcopyint:The return value pointer is null at \"%s\".\n", name );
      exit( -1 );
    }

  if ( item->specified )
    *return_value = atol( item->value[ 0 ] );

  return item->specified;
}



/*==================================================================*
 * optifspecifiedcopyfloat : ¤â¤·»ØÄê¤µ¤ì¤Æ¤¤¤¿¤é¼Â¿ôÃÍ¤ò¥³¥Ô¡¼¤¹¤ë
 *==================================================================*/
int optifspecifiedcopyfloat /* ¥³¥Ô¡¼¤·¤¿¤«¤É¤¦¤« */
  _P2 (( char   *, name         ),
       ( double *, return_value ))
{
  struct optitem *item;

  item = __opt_search( name );

  if ( item == 0 )
    {
      fprintf( stderr, "PROGRAM-ERROR:optifspecfiedcopyfloat:I DON'T HAVE THE OPTION \"%s\".\n", name );
      exit( -1 );
    }

  if ( return_value == 0 )
    {
      fprintf( stderr, "PROGRAM-ERROR:optifspecfiedcopyfloat:The return value pointer is null at \"%s\".\n", name );
      exit( -1 );
    }

  if ( item->specified )
    *return_value = atof( item->value[ 0 ] );

  return item->specified;
}



/*-----------------------------------------------------*
 * __opt_unique_check : ¥ª¥×¥·¥ç¥ó¤¬°ì°Õ¤«¤É¤¦¤«Ä´¤Ù¤ë
 *-----------------------------------------------------*/
static void __opt_unique_check
  _P0 (void)
{
  int find_same_name = 0;
  int i, j;

  for ( i = 0; i < optnum - 1; i++ )
    {
      struct optitem *item = &optlist[ i ];
      if ( item->type == TYPE_DESCRIPTION ) continue;
      for ( j = i + 1; j < optnum; j++ )
	if ( optlist[ j ].type == TYPE_DESCRIPTION )
	  {
	    continue;
	  }
	else
	if ( strequal( item->name, optlist[ j ].name ) )
	  {
	    find_same_name++;
	    fprintf( stderr,
		     "PROGRAM-ERROR:You use the same item name (%s).\n",
		     item->name );
	  }
	else
	if ( strequal( item->specifier, optlist[ j ].specifier ) )
	  {
	    find_same_name++;
	    fprintf( stderr,
		     "PROGRAM-ERROR:You use the same specifier (%s).\n",
		     item->specifier );
	    fprintf( stderr,
		     "    These option names are \"%s\" and \"%s\".\n",
		     item->name, optlist[ j ].name );
	  }
    }
  if ( find_same_name ) exit( -1 );
}
