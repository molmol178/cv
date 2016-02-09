/*
 * filename : socketlib.h	
 * author   : Takahiro Sugiyama
 * date     : 1993/10/03
 */



#ifndef __socketlib_h
#define __socketlib_h



#include "comacros.h"
#include <sys/types.h>



/****************************************
 * サーバとクライアントが用いるルーチン *
 ****************************************/

/*=============================================*
 * socketread : ソケットからデータを読み込む。
 *=============================================*/
long socketread /* 読み込みバイト数 */
  P3 (( long  , fd    ), /* ソケットＦＤ   */
      ( char *, buf   ), /* データポインタ */
      ( long  , nbyte )) /* データバイト数 */

/*============================================*
 * socketwrite : ソケットにデータを書き込む。
 *============================================*/
long socketwrite /* 書き込みバイト数 */
  P3 (( long  , fd    ), /* ソケットＦＤ   */
      ( char *, buf   ), /* データポインタ */
      ( long  , nbyte )) /* データバイト数 */

/*==================================*
 * socketclose : ソケットを閉じる。
 *==================================*/
void socketclose
  P1 (( long, fd )) /* ソケットＦＤ */



/**************************
 * サーバが用いるルーチン *
 **************************/

/*=====================================*
 * socketinit : ソケットを初期化する。
 *=====================================*/
long socketinit /* ソケットＦＤ */
  P1 (( char *, name )) /* ソケットファイル名 */

/*==================================*
 * socketexit : ソケットを終了する。
 *==================================*/
void socketexit
  P2 (( char *, name ), /* ソケットファイル名 */
      ( long  , fd   )) /* ソケットＦＤ       */

/*================================================*
 * socektselect : 読み込み可能なソケットを調べる。
 *================================================*/
long socketselect /* 読み込みＯＫのソケットＦＤ数  */
  P3 (( long    , max_fd  ), /* 最大のソケットＦＤの番号   */
      ( fd_set *, mask ), /* ソケットＦＤのビットマスク */
      ( double  , waitsec )) /* 待ち時間（秒）0は無限に待つ */

#define socketready( mask, fd )	FD_ISSET( fd, &mask )
#define socketfdisset( mask, fd )	FD_ISSET( fd, &mask )
#define socketfdclear( mask, fd )	FD_CLR( fd, &mask )
#define socketfdset( mask, fd )	FD_SET( fd, &mask )
#define socketfdzero( mask )		FD_ZERO( &mask )

/*===================================*
 * socketaccept : 接続を受け入れる。
 *===================================*/
long socketaccept /* 相手のソケットＦＤ */
  P1 (( int, fd )) /* ソケットＦＤ */



/********************************
 * クライアントが用いるルーチン *
 ********************************/

/*==================================*
 * socketconnect : 接続を要求する。
 *==================================*/
long socketconnect /* 相手のソケットＦＤ */
  P1 (( char *, name )) /* ソケットファイル名 */



#endif /* __socketlib_h */
