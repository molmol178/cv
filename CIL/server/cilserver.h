/*
 * filename : cilserver.h
 * author   : Takahiro Sugiyama
 * date     : 1993/10/04
 */



#ifndef __cilserver_h
#define __cilserver_h



#include "misc/typelib.h"
#include <X11/Xlib.h>
#include "ImageDisp.h"



#define CIL_LIST_NUM 256

#define NAME_MAX 16
#define FNAME_MAX 128
#define DISPLAYNAME_MAX 32
#define CIL_DATA_NUM 12



typedef struct {
  char name[NAME_MAX];
  long key;
  char filename[FNAME_MAX];
  char displayname[DISPLAYNAME_MAX];
  char imagedispexec;
  char share;
  CILEvent event;
  long mask;
  Window id;
  long command;
  long data[CIL_DATA_NUM];
} cilRec;



/*==============================*
 * CIL SERVER USERS SYSTEM CALL
 *==============================*/

/*=======================================*
 * cil_openlist : リストをオープンします。
 *=======================================*/
cilRec *cil_openlist
  P0 (void)

/*==========================================*
 * cil_closelist : リストをクローズします。
 *==========================================*/
void cil_closelist
  P0 (void)

/*=============================================*
 * cil_create : 名前 name を新しく作成します。
 *=============================================*/
cilRec *cil_create
  P1 (( char *, name ))

/*===================================================*
 * cil_open : 名前 name の cilRec をアタッチします。
 *===================================================*/
cilRec *cil_open
  P1 (( char *, name ))

/*=======================================*
 * cil_unlink : 名前 name を削除します。
 *=======================================*/
long cil_unlink
  P1 (( char *,name ))

/*============================*
 * cil_close : クローズする。
 *============================*/
void cil_close
  P1 (( cilRec *, info ))

/*=============================================================*
 * cil_dopen : インデックス index から cilRec をアタッチします。
 *=============================================================*/
cilRec *cil_dopen
  P1 (( long, index ))

/*==========================================*
 * cil_xsend : imagedisp にイベントを送る。
 *==========================================*/
long cil_xsend
  P3 (( char *, name ),
      ( long  , type ),
      ( long *, data ))



/*--------------------------------*
 * cil request : サーバリクエスト
 *--------------------------------*/
enum {
  CIL_TEST = 1,
  CIL_EXIT,
  CIL_FREE,
  CIL_ADD,
  CIL_DEL,
  CIL_DRAW,
  CIL_DRAW_AREA,
  CIL_QUIT,
  CIL_OK
};



#endif /* __cilserver_h */
