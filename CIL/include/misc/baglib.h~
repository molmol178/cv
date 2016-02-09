/*
 * filename : baglib.h
 * author   : Takahiro Sugiyama
 * date     : Thursday, June 2 1994
 * describe : Bag Structure Library
 */



#ifndef __baglib_h
#define __baglib_h



#include "comacros.h"



typedef struct bagRec {
  short elem_size;
  short capacity_skip;
  long  capacity;
  long  length;
  char *data;
} *t_bag;



#define BAG__LENGTH(bag)          (bag)->length
#define BAG__DATA(bag,type)       ((type *)(bag)->data)
#define BAG__ELEM(bag,type,index) BAG__DATA(bag,type)[index]



t_bag bag__create
  P2 (( long, elem_size     ),
      ( long, capacity_skip ))

void bag__destroy
  P1 (( t_bag, self ))

void bag__free
  P1 (( t_bag, self ))





#endif  __baglib_h
