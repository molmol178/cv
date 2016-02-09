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
 * mode �Υޥ���
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
 * fn_* : �ե�����̾�ؿ�
 * fp_* : �ե�����ݥ��󥿴ؿ�
 * fd_* : �ե����뵭�һҴؿ�
 *=============================*/



/*====================*
 * �ե�����̾����ؿ�
 *====================*/
#define fn_rename( oldname, newname ) rename( oldname, newname )
#define fn_remove( name )             unlink( name )
#define fn_stat( name, buf )          stat( name, buf )
#define fn_access( name, mode )       access( name, mode )



/*============================================*
 * fn_size : �ե�����Υ��������֤�
 * fn_mode : �ե�����Υѡ��ߥå�����Ĵ�٤�
 * fn_uid  : �ե�����Υ桼��ID��Ĵ�٤�
 * fn_gid  : �ե�����Υ桼��ID��Ĵ�٤�
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
 * fn_filename : �ѥ�̾����ե�����̾������
 *==========================================*/
int fn_filename /* 0:���ޤ����ä�  -1:�ե�����̾���ʤ� */
  P2 (( char *, return_filename ),
      ( char *, input_pathname  ))

/*==================================================================*
 * fn_basename : �ѥ�̾����ե�����̾�Υ١���������(file.c -> file)
 *==================================================================*/
int fn_basename /* 0:���ޤ����ä�  -1:�ե�����̾���ʤ� */
  P2 (( char *, return_basename ),
      ( char *, input_pathname  ))

/*==================================================================*
 * fn_setsuffix : �١���̾�Υ��ե��å������ѹ�����(file.c -> file.o)
 *==================================================================*/
int fn_setsuffix /* 0:���ޤ����ä�  -1:�ե�����̾���ʤ� */
  P3 (( char *, return_newname ),
      ( char *, input_pathname ),
      ( char *, new_suffix     ))

/*===========================================================*
 * fn_list       : �ǥ��쥯�ȥ�Υꥹ�Ȥ�Ȥ�
 * fn_regexplist : ����ɽ���Υե�����̾�Υꥹ�Ȥ�ȤäƤ���
 * fn_freelist   : �ե�����ꥹ�Ȥ��������
 *===========================================================*/
void fn_set_dotfile_ignore();/* �ɥåȥե������̵�� */
void fn_reset_dotfile_ignore();/* �ɥåȥե�����ͭ�� */
char **fn_list /* �ե�����Υꥹ�� */
  P2 (( char *, pathname      ),
      ( long *, return_number ))

char **fn_regexplist /* �ե�����Υꥹ�� */
  P2 (( char *, filename_pattern ),
      ( int * , return_number    ))

void fn_freelist
  P1 (( char **, filelist ))



/*==========================*
 * �ե�����ݥ��󥿻���ؿ�
 *==========================*/
FILE *fp_open /* �ե�����ݥ��� */
  P2 (( char *, name ), /* �ե�����̾     */
      ( char *, type )) /* �ե����륿���� */

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
 * �ե����뵭�һһ���ؿ�
 *========================*/
long fd_open /* �ե����뵭�һ� */
  P3 (( char *, path  ), /* �ե�����̾     */
      ( long  , flags ), /* �ե�����°��   */
      ( long  , mode  )) /* �ե�����⡼�� */

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
 * �ե����뵭�һҤȥե�����ݥ��󥿤��Ѵ�
 *========================================*/
#define fd_to_fp( fd, type ) fdopen( fd, type )
#define fp_to_fd( fp )       fileno( fp )



#endif  //__filelib_h
