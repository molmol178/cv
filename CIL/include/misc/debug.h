/*
 * debug.h
 *     by Takahiro Sugiyama (1993.8.9)
 * $B%G%P%C%0!u%P!<%\!<%:%b!<%I$N%i%$%V%i%j(B
 */



#ifndef __Debug_h
#define __Debug_h



#include "comacros.h"



/*
 * $B%G%P%C%0%l%Y%k(B
 */
extern long Debug;

/*
 * $B%P!<%\!<%:%b!<%I(B
 */
extern long Verbose;



/*
 * $B%G%P%C%0H=Dj%k!<%k(B
 */
#define ifDebug(level) if ((Debug == -level) || (Debug >= level))

/*
 * $B%G%P%C%0%b!<%I$G%l%Y%k$rK~$?$7$F$$$?$iI=<(%(%i!<=PNO$K%W%j%s%H$9$k!#(B
 */
void debugf(/* long level,char *fmt, ... */);

/*
 * $B%G%P%C%0%l%Y%k$r%;%C%H$9$k!#(B
 */
void debugset(/* long level */);



/*
 * $B%P!<%\!<%:%b!<%I$J$iI8=`=PNO$K%W%j%s%H$9$k!#(B
 */
void verbosef(/* char *fmt, ... */);

/*
 * $B%P!<%\!<%:$r%;%C%H$9$k!#(B
 */
void verboseset(/* void */);



#endif //__Debug_h
