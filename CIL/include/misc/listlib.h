/*
 * filename : listlib.h
 * author   : Takahiro Sugiyama
 * date     : Thursday, June 2 1994
 * describe : List Structure Library
 */



#ifndef __listlib_h
#define __listlib_h



#include "comacros.h"



typedef struct listRec {
  short elem_size;
  short capacity_skip;
  long  capacity;
  long  length;
  char *data;
} *t_list;

#define LIST__LENGTH(list)          (list)->length
#define LIST__DATA(list,type)       ((type *)(list)->data)
#define LIST__ELEM(list,type,index) LIST__DATA(list,type)[index]

t_list list__create
  P2 (( long, elem_size     ),
      ( long, capacity_skip ))

void list__destroy
  P1 (( t_list, self ))

long list__length
  P1 (( t_list, self ))

char *list__data
  P1 (( t_list, self ))

char *list__elem
  P2 (( t_list, self  ),
      ( long  , index ))

void list__enter
  P2 (( t_list, self     ),
      ( char *, elem_ptr ))



#endif  //__listlib_h
