/*
 * debug.c
 */



#include <stdio.h>
//#include <varargs.h>
#include <stdarg.h>
#include "debug.h"



extern vfprintf(/* FILE *stream,char *format, va_list ap */);
extern vprintf(/* char *format, va_list ap */);
extern fflush(/* FILE *stream */);



long Debug = 0;
long Verbose = 0;



/*
 * デバッグレベルを満たしてたらプリントする。
 */
//void debugf(level,fmt,va_alist)
void debugf(long level,char *fmt,...)
{
  va_list args;

  ifDebug(level)
    {
      va_start(args, fmt);
      vfprintf(stderr,fmt,args);
      va_end(args);
    }
}



/*
 * デバッグレベルのセット。
 */
void debugset(level)
     long level;
{
  Debug = level;
}



/*
 * バーボーズモードなら標準出力にプリントする。
 */
void verbosef(char *fmt,...)
{
  va_list args;

  if (Verbose)
    {
      va_start(args,fmt);
      vprintf(fmt,args);
      fflush(stdout);
      va_end(args);
    }
}



/*
 * バーボーズをセットする。
 */
void verboseset()
{
  Verbose = 1;
}
