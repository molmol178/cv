/*
 * filename : filelib.h
 * author   : Takahiro Sugiyama
 * date     : 1993/09/13
 *
 * update   : Friday, February 18 1994 by Takahiro Sugiyama
 * describe : ファイル名に関する関数を追加。
 * 	      fn_filename, fn_makefilelist, fn_freefilelist
 */



#include "filelib.h"
#include "misc/strlib.h"
#include "misc/memlib.h"
#include <dirent.h>



/*===================================*
 * fp_open : ファイルをオープンする。
 *===================================*/
FILE *fp_open /* ファイルポインタ */
  _P2 (( char *, name ), /* ファイル名     */
       ( char *, type )) /* ファイルタイプ */
{
  if (( name == 0 ) || ( strcmp( name, "-" ) == 0 ))
    {
      if ( strcmp( type, "r" ) == 0 ) return stdin;
      if ( strcmp( type, "w" ) == 0 ) return stdout;
      if ( strcmp( type, "a" ) == 0 ) return stdout;
      return 0;
    }

  if ( strcmp( name, "stdin" ) == 0 ) return stdin;

  if ( strcmp( name, "stdout" ) == 0 ) return stdout;

  if ( strcmp( name, "stderr" ) == 0 ) return stdout;

  return fopen( name, type );
}



/*===================================*
 * fp_open : ファイルをオープンする。
 *===================================*/
long fd_open /* ファイル記述子 */
  _P3 (( char *, path  ), /* ファイル名     */
       ( long  , flags ), /* ファイル属性   */
       ( long  , mode  )) /* ファイルモード */
{
  if (( path == 0 ) || ( strcmp( path, "-" ) == 0 ))
    {
      if ( flags & O_RDONLY ) return fd_stdin;
      if ( flags & O_WRONLY ) return fd_stdout;
      if ( flags & O_APPEND ) return fd_stdout;
      if ( flags & O_RDWR   ) return fd_stdout;
      return 0;
    }

  if ( strcmp( path, "stdin" ) == 0 ) return fd_stdin;

  if ( strcmp( path, "stdout" ) == 0 ) return fd_stdout;

  if ( strcmp( path, "stderr" ) == 0 ) return fd_stderr;

  return open( path, flags, mode );
}



#define DEBUG 0

/*==============================================*
 * fp_read : ファイルから大量のデータを読み込む
 *==============================================*/
long fp_read
  _P4 (( FILE *, fp   ),
       ( char *, ptr  ),
       ( long  , num  ),
       ( long  , size ))
{
  long totalbyte;
  long loadbyte = 0;
  long byte;

  totalbyte = num * size;
  while ( loadbyte != totalbyte )
    {
      byte = fread( ptr + loadbyte, 1, totalbyte - loadbyte, fp );
      loadbyte += byte;
      if ( byte == 0 )
	{
	  return loadbyte / size;
	}
    }

  return num;
}



/*===============================================*
 * fp_write : ファイルから大量のデータを書き込む
 *===============================================*/
long fp_write
  _P4 (( FILE *, fp   ),
       ( char *, ptr  ),
       ( long  , num  ),
       ( long  , size ))
{
  long totalbyte;
  long savebyte = 0;
  long byte;

  totalbyte = num * size;
  while ( savebyte != totalbyte )
    {
      byte = fwrite( ptr + savebyte, 1, totalbyte - savebyte, fp );
      savebyte += byte;
      if ( byte == 0 )
	{
	  return savebyte / size;
	}
    }

  return num;
}



#include "misc/strlib.h"
#include "misc/memlib.h"

/*==========================================*
 * fn_filename : パス名からファイル名を得る
 *==========================================*/
int fn_filename /* 0:うまくいった  -1:ファイル名がない */
  _P2 (( char *, return_filename ),
       ( char *, input_pathname  ))
{
  char *head[ 64 ];
  int num;
  char pathname[ 1024 ];

  strcopy( pathname, input_pathname );
  num = strsplit( pathname, head, "/" );
  if ( num == 0 ) return -1;

  strcopy( return_filename, head[ num - 1 ] );

  return 0;
}



/*==================================================================*
 * fn_basename : パス名からファイル名のベースを得る(file.c -> file)
 *==================================================================*/
int fn_basename /* 0:うまくいった  -1:ファイル名がない */
  _P2 (( char *, return_basename ),
       ( char *, input_pathname  ))
{
  char *head[ 64 ];
  int i, num;
  char pathname[ 1024 ];
  char filename[ 256 ];

  strcopy( pathname, input_pathname );
  num = strsplit( pathname, head, "/" );
  if ( num == 0 ) return -1;

  strcopy( filename, head[ num - 1 ] );
  i = strlength( filename ) - 1;
  while (( i > 0 ) && ( filename[ i ] != '.' )) i--;
  if ( i > 0 ) filename[ i ] = '\0';

  strcopy( return_basename, filename );

  return 0;
}



/*==================================================================*
 * fn_setsuffix : サフィックスを変更する(file.c -> file.o)
 *==================================================================*/
int fn_setsuffix /* 0:うまくいった  -1:ファイル名がない */
  _P3 (( char *, return_newname ),
       ( char *, input_pathname ),
       ( char *, new_suffix     ))
{
  char basename[ 1024 ];

  if ( fn_basename( basename, input_pathname ) == -1 ) return -1;

  sprintf( return_newname, "%s.%s", basename, new_suffix );

  return 0;
}



/*==================================*
 * fn_size : ファイルのサイズを返す
 *==================================*/
long fn_size
  _P1 (( char *, pathname ))
{
  struct stat statbuf;

  if ( stat( pathname, &statbuf ) < 0 )
    {
      fprintf( stderr, "fn_size:couldn't stat \"%s\"\n", pathname );
      return -1;
    }

  return statbuf.st_size;
}



/*============================================*
 * fn_mode : ファイルのパーミッションを調べる
 *============================================*/
long fn_mode
  _P1 (( char *, pathname ))
{
  struct stat statbuf;

  if ( stat( pathname, &statbuf ) < 0 )
    {
      fprintf( stderr,"fn_mode:couldn't stat \"%s\"\n", pathname );
      return -1;
    }

  return statbuf.st_mode;
}



/*=========================================*
 * fn_uid : ファイルのユーザIDを調べる
 *=========================================*/
long fn_uid
  _P1 (( char *, pathname ))
{
  struct stat statbuf;

  if ( stat( pathname, &statbuf ) < 0 )
    {
      fprintf( stderr,"fn_uid:couldn't stat \"%s\"\n", pathname );
      return -1;
    }

  return statbuf.st_uid;
}



/*=========================================*
 * fn_gid : ファイルのユーザIDを調べる
 *=========================================*/
long fn_gid
  _P1 (( char *, pathname ))
{
  struct stat statbuf;

  if ( stat( pathname, &statbuf ) < 0 )
    {
      fprintf( stderr,"fn_gid:couldn't stat \"%s\"\n", pathname );
      return -1;
    }

  return statbuf.st_gid;
}



/*======================================*
 * fn_list : ディレクトリのリストをとる
 *======================================*/
static long fn_list_dotfile_ignore = 0;
void fn_set_dotfile_ignore(){fn_list_dotfile_ignore = 1;}
void fn_reset_dotfile_ignore(){fn_list_dotfile_ignore = 0;}
char **fn_list
  _P2 (( char *, pathname      ),
       ( long *, return_number ))
{
  char **list;
  char *list_max[ 1024 ];
  int i, num;

  struct dirent *dp;
  DIR *dirp;

  if ( return_number ) *return_number = 0;

  dirp = opendir( pathname );
  if ( dirp == 0 ) return 0;
  num = 0;
  for ( dp = readdir( dirp ); dp != NULL; dp = readdir( dirp ) )
    {
      if ((fn_list_dotfile_ignore) &&
	  (dp->d_name[0] == '.') &&
	  (strcmp(dp->d_name, ".") != 0) &&
	  (strcmp(dp->d_name, "..") != 0)) continue;

      list_max[ num++ ] = strnew( dp->d_name );
    }
  closedir( dirp );

  if ( num == 0 ) return 0;
  list = typenew1( num + 1, char * );
  for ( i = 0; i < num; i++ ) list[ i ] = list_max[ i ];
  list[ i ] = 0;

  if ( return_number ) *return_number = num;

  return list;
}



/*===========================================================*
 * fn_regexplist : 正規表現のファイル名のリストをとってくる
 *===========================================================*/
char **fn_regexplist /* ファイルのリスト */
  _P2 (( char *, filename_pattern ),
       ( int * , return_number    ))
{
  char **filename_list;
  char *big_filename_list[ 1024 ];
  char command[ 256 ];
  int i, num;

  FILE *fp;
  char buf[ 256 ];

  if ( return_number ) *return_number = 0;

  sprintf( command, "/usr/bin/csh -f -c \"%s\"", filename_pattern );

  num = 0;
  fp = popen( command, "r" );
  while ( fgets( buf, sizeof( buf ), fp ) )
    big_filename_list[ num++ ] = strnew( buf );
  pclose( fp );

  if ( num == 0 ) return 0;
  filename_list = typenew1( num + 1, char * );
  for ( i = 0; i < num; i++ )
    filename_list[ i ] = big_filename_list[ i ];
  filename_list[ i ] = 0;

  *return_number = num;

  return filename_list;
}



/*============================================*
 * fn_freelist : ファイルリストを解放する
 *============================================*/
void fn_freelist
  _P1 (( char **, filelist ))
{
  int i;

  i = 0;

  while ( filelist[ i ] ) strfree( filelist[ i++ ] );

  typefree1( filelist );
}
