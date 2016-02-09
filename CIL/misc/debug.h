/*
 * debug.h
 *     by Takahiro Sugiyama (1993.8.9)
 * $@%G%P%C%0!u%P!<%\!<%:%b!<%I$N%i%$%V%i%j(J
 */



#ifndef __Debug_h
#define __Debug_h



#include "comacros.h"



/*
 * $@%G%P%C%0%l%Y%k(J
 */
extern long Debug;

/*
 * $@%P!<%\!<%:%b!<%I(J
 */
extern long Verbose;



/*
 * $@%G%P%C%0H=Dj%k!<%k(J
 */
#define ifDebug(level) if ((Debug == -level) || (Debug >= level))

/*
 * $@%G%P%C%0%b!<%I$G%l%Y%k$rK~$?$7$F$$$?$iI=<(%(%i!<=PNO$K%W%j%s%H$9$k!#(J
 */
void debugf(long level,char *fmt, ...);

/*
 * $@%G%P%C%0%l%Y%k$r%;%C%H$9$k!#(J
 */
void debugset(/* long level */);



/*
 * $@%P!<%\!<%:%b!<%I$J$iI8=`=PNO$K%W%j%s%H$9$k!#(J
 */
void verbosef(char *fmt, ...);

/*
 * $@%P!<%\!<%:$r%;%C%H$9$k!#(J
 */
void verboseset(/* void */);



#endif __Debug_h
