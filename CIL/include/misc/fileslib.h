/*
 * filename : fileslib.h
 * author   : Takahiro Sugiyama
 * date     : 1993/05/19
 * date     : 1993/09/28
 */



#ifndef __fileslib_h
#define __fileslib_h



#include "typelib.h"
#include "comacros.h"
#include <stdio.h>



typedef struct _FILES_REC FILES;



FILES *filesopen
  P2 (( long   , filename_num  ),
      ( char **, filename_list ))

void filesclose
  P1 (( FILES *, fp ))

void filespush
  P2 (( FILES *, fp       ),
      ( char * , filename ))

char *filespop
  P1 (( FILES *, fp ))

char *filesname
  P1 (( FILES *, fp ))

long fileslines
  P1 (( FILES *, fp ))

char filesgetc
  P1 (( FILES *, fp ))

char filesungetc
  P2 (( FILES *, fp ),
      ( long   , c  ))

char *filesgets
  P3 (( FILES *, fp  ),
      ( char * , buf ),
      ( long   , n   ))

long filesread
  P4 (( FILES *, fp        ),
      ( char * , buf       ),
      ( long   , elem_num  ),
      ( long   , elem_size ))

long filesreadbyte
  P3 (( FILES *, fp    ),
      ( char * , buf   ),
      ( long   , bytes ))



#endif /* __fileslib_h */
