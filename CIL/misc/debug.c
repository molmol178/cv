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
 * $@%G%P%C%0%l%Y%k$rK~$?$7$F$?$i%W%j%s%H$9$k!#(J
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
 * $@%G%P%C%0%l%Y%k$N%;%C%H!#(J
 */
void debugset(level)
     long level;
{
  Debug = level;
}



/*
 * $@%P!<%\!<%:%b!<%I$J$iI8=`=PNO$K%W%j%s%H$9$k!#(J
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
 * $@%P!<%\!<%:$r%;%C%H$9$k!#(J
 */
void verboseset()
{
  Verbose = 1;
}
