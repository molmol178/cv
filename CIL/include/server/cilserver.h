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
 * cil_openlist : $@%j%9%H$r%*!<%W%s$7$^$9!#(J
 *=======================================*/
cilRec *cil_openlist
  P0 (void)

/*==========================================*
 * cil_closelist : $@%j%9%H$r%/%m!<%:$7$^$9!#(J
 *==========================================*/
void cil_closelist
  P0 (void)

/*=============================================*
 * cil_create : $@L>A0(J name $@$r?7$7$/:n@.$7$^$9!#(J
 *=============================================*/
cilRec *cil_create
  P1 (( char *, name ))

/*===================================================*
 * cil_open : $@L>A0(J name $@$N(J cilRec $@$r%"%?%C%A$7$^$9!#(J
 *===================================================*/
cilRec *cil_open
  P1 (( char *, name ))

/*=======================================*
 * cil_unlink : $@L>A0(J name $@$r:o=|$7$^$9!#(J
 *=======================================*/
long cil_unlink
  P1 (( char *,name ))

/*============================*
 * cil_close : $@%/%m!<%:$9$k!#(J
 *============================*/
void cil_close
  P1 (( cilRec *, info ))

/*=============================================================*
 * cil_dopen : $@%$%s%G%C%/%9(J index $@$+$i(J cilRec $@$r%"%?%C%A$7$^$9!#(J
 *=============================================================*/
cilRec *cil_dopen
  P1 (( long, index ))

/*==========================================*
 * cil_xsend : imagedisp $@$K%$%Y%s%H$rAw$k!#(J
 *==========================================*/
long cil_xsend
  P3 (( char *, name ),
      ( long  , type ),
      ( long *, data ))



/*--------------------------------*
 * cil request : $@%5!<%P%j%/%(%9%H(J
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
