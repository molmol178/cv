/*
 * filename : objlib.h
 * author   : Takahiro Sugiyama
 * date     : 1993/09/13
 */



#ifndef __objlib_h
#define __objlib_h



#include "comacros.h"



struct objectRec *Object;
struct classRec *Class;

/*
 * オブジェクト記述規則
 */
struct objectRec {
  char *name;

  long part_num;
  struct {
    Class class;
    char *instance;
  } *part_list;
};

struct classRec {
  char *name;
  long size;

  Class class;
  Class instance_class;

  void *(*constructor)(/* char *instance */);
  void  (*destructor) (/* char *instance */);

  long  method_num;
  struct {
    char *name;
    va_list (*call)(/* char *name,char *instance,va_list ap */);
  } *method_list;
};

Object objcreate(/* char *name, Class class */);
void objdestroy(/* Object object */);

void objadd(/* Object object, Class class */);
void objdel(/* Object object, Class class */);

void objcall(/* Object object,Class class,char *name,... */);


#endif  __objlib_h
