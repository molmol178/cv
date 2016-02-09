/*
 * filename : filelib.h
 * author   : Takahiro Sugiyama
 * date     : 1993/09/13
 */



#ifndef __filelib_h
#define __filelib_h



#include "comacros.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>



/*===============*
 * mode のマスク
 *===============*/
#define OWNER_READ     0400
#define OWNER_WRITE    0200
#define OWNER_EXECUTE  0100

#define GROUP_READ     0040
#define GROUP_WRITE    0020
#define GROUP_EXECUTE  0010

#define OTHERS_READ    0004
#define OTHERS_WRITE   0002
#define OTHERS_EXECUTE 0001



/*=============================*
 * fn_* : ファイル名関数
 * fp_* : ファイルポインタ関数
 * fd_* : ファイル記述子関数
 *=============================*/



/*====================*
 * ファイル名指定関数
 *====================*/
#define fn_rename( oldname, newname ) rename( oldname, newname )
#define fn_remove( name )             unlink( name )
#define fn_stat( name, buf )          stat( name, buf )
#define fn_access( name, mode )       access( name, mode )



/*============================================*
 * fn_size : ファイルのサイズを返す
 * fn_mode : ファイルのパーミッションを調べる
 * fn_uid  : ファイルのユーザIDを調べる
 * fn_gid  : ファイルのユーザIDを調べる
 *============================================*/
long fn_size
  P1 (( char *, name ))

long fn_mode
  P1 (( char *, name ))

long fn_uid
  P1 (( char *, name ))

long fn_gid
  P1 (( char *, name ))




/*==========================================*
 * fn_filename : パス名からファイル名を得る
 *==========================================*/
int fn_filename /* 0:うまくいった  -1:ファイル名がない */
  P2 (( char *, return_filename ),
      ( char *, input_pathname  ))

/*==================================================================*
 * fn_basename : パス名からファイル名のベースを得る(file.c -> file)
 *==================================================================*/
int fn_basename /* 0:うまくいった  -1:ファイル名がない */
  P2 (( char *, return_basename ),
      ( char *, input_pathname  ))

/*==================================================================*
 * fn_setsuffix : ベース名のサフィックスを変更する(file.c -> file.o)
 *==================================================================*/
int fn_setsuffix /* 0:うまくいった  -1:ファイル名がない */
  P3 (( char *, return_newname ),
      ( char *, input_pathname ),
      ( char *, new_suffix     ))

/*===========================================================*
 * fn_list       : ディレクトリのリストをとる
 * fn_regexplist : 正規表現のファイル名のリストをとってくる
 * fn_freelist   : ファイルリストを解放する
 *===========================================================*/
void fn_set_dotfile_ignore();/* ドットファイルの無視 */
void fn_reset_dotfile_ignore();/* ドットファイル有効 */
char **fn_list /* ファイルのリスト */
  P2 (( char *, pathname      ),
      ( long *, return_number ))

char **fn_regexplist /* ファイルのリスト */
  P2 (( char *, filename_pattern ),
      ( int * , return_number    ))

void fn_freelist
  P1 (( char **, filelist ))



/*==========================*
 * ファイルポインタ指定関数
 *==========================*/
FILE *fp_open /* ファイルポインタ */
  P2 (( char *, name ), /* ファイル名     */
      ( char *, type )) /* ファイルタイプ */

#define fp_close( fp ) \
  if (( fp != 0 ) && ( fp != stdin ) && ( fp != stdout )) fclose( fp )

#define fp_flush( fp ) fflush( fp )

#define fp_seek( fp, offset, ptrname ) fseek( fp, offset, ptrname )
#define fp_tell( fp )                  ftell( fp )
#define fp_rewind( fp )                rewind( fp )

#define fp_printf          fprintf
#define fp_getc( fp )      getc( fp )
#define fp_ungetc( fp, c ) ungetc( c, fp )
#define fp_putc( fp, c )   putc( c, fp )

#define fp_gets( fp, ptr, n ) fgets( ptr, n, fp )
#define fp_puts( fp, ptr )    fputs( ptr, fp )

long fp_read
  P4 (( FILE *, fp   ),
      ( char *, ptr  ),
      ( long  , num  ),
      ( long  , size ))

long fp_write
  P4 (( FILE *, fp   ),
      ( char *, ptr  ),
      ( long  , num  ),
      ( long  , size ))

#define fp_readbyte( fp, ptr, nbytes )  fp_read( fp, ptr, nbytes, 1 )
#define fp_writebyte( fp, ptr, nbytes ) fp_write( fp, ptr, nbytes, 1 )



/*========================*
 * ファイル記述子指定関数
 *========================*/
long fd_open /* ファイル記述子 */
  P3 (( char *, path  ), /* ファイル名     */
      ( long  , flags ), /* ファイル属性   */
      ( long  , mode  )) /* ファイルモード */

#define fd_close( fd ) if ( fd > 2 ) close( fd )

#define fd_seek( fd, offset, whence ) lseek( fd, offset, whence )
#define fd_tell( fd )                 tell( fd )
#define fd_stat( fd, buf )             fstat( fd, buf )

#define fd_read( fd, dest, num )      read( fd, dest, num )
#define fd_write( fd, src, num )      write( fd, src, num )

#define fd_stdin  0
#define fd_stdout 1
#define fd_stderr 2

#define fd_bindstdin( fd )  close( fd_stdin  ),dup( fd ),close( fd )
#define fd_bindstdout( fd ) close( fd_stdout ),dup( fd ),close( fd )
#define fd_bindstderr( fd ) close( fd_stderr ),dup( fd ),close( fd )



/*========================================*
 * ファイル記述子とファイルポインタの変換
 *========================================*/
#define fd_to_fp( fd, type ) fdopen( fd, type )
#define fp_to_fd( fp )       fileno( fp )



#endif  //__filelib_h
