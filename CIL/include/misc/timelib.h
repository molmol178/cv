/*
 * filename : timelib.h
 * author   : Takahiro Sugiyama
 * date     : 1993/08/11
 */



#ifndef __timelib_h
#define __timelib_h



#include "comacros.h"



/*==================*
 * $B%?%$%^!<$N:GBg?t(B
 *==================*/
#define TIMER_MAX 8



/*====================================*
 * timestart   : $B;~4V$N7WB,$N3+;O(B
 * timestop    : $B;~4V$N7WB,$N=*N;(B
 * timepause   : $B;~4V$N7WB,$N0l;~Dd;_(B
 * timerestart : $B;~4V$N7WB,$N:F3+(B
 *====================================*/
void timestart
  P1 (( long, timer_no )) /* $B%?%$%^!<HV9f(B */

void timestop
  P1 (( long, timer_no )) /* $B%?%$%^!<HV9f(B */

void timepause
  P1 (( long, timer_no )) /* $B%?%$%^!<HV9f(B */

void timerestart
  P1 (( long, timer_no )) /* $B%?%$%^!<HV9f(B */



/*=====================================*
 * timelapstart : $B%i%C%W;~4V$N7WB,3+;O(B
 *=====================================*/
void timelapstart
  P1 (( long, timer_no )) /* $B%?%$%^!<HV9f(B */



/*==============================================================*
 * timeuser      : $B7WB,$7$?%f!<%6;~4V$rICC10L$GJV$9!#(B
 * timesystem    : $B7WB,$7$?%7%9%F%`;~4V$rICC10L$GJV$9!#(B
 * timelapuser   : $B7WB,$7$?%i%C%W$N%f!<%6;~4V$rICC10L$GJV$9!#(B
 * timelapsystem : $B7WB,$7$?%i%C%W$N%7%9%F%`;~4V$rICC10L$GJV$9!#(B
 *
 * $B%]!<%:$7$F$$$?;~$N;~4V$O4^$^$l$F$J$$!#(B
 *==============================================================*/
double timeuser	/* $B7WB,;~4V!JIC!K(B */
  P1 (( long, timer_no )) /* $B%?%$%^!<HV9f(B */

double timesystem /* $B7WB,;~4V!JIC!K(B */
  P1 (( long, timer_no )) /* $B%?%$%^!<HV9f(B */

double timelapuser /* $B7WB,;~4V!JIC!K(B */
  P1 (( long, timer_no )) /* $B%?%$%^!<HV9f(B */

double timelapsystem /* $B7WB,;~4V!JIC!K(B */
  P1 (( long, timer_no )) /* $B%?%$%^!<HV9f(B */



/*===================================================================*
 * timeusermin      : $B7WB,$7$?%f!<%6;~4V$rJ,ICC10L$GJV$9!#(B
 * timesystemmin    : $B7WB,$7$?%7%9%F%`;~4V$rJ,ICC10L$GJV$9!#(B
 * timelapusermin   : $B7WB,$7$?%i%C%W$N%f!<%6;~4V$rJ,ICC10L$GJV$9!#(B
 * timelapsystemmin : $B7WB,$7$?%i%C%W$N%7%9%F%`;~4V$rJ,ICC10L$GJV$9!#(B
 *
 * $B%]!<%:$7$F$$$?;~$N;~4V$O4^$^$l$F$J$$!#(B
 *===================================================================*/
long timeusermin /* $B7WB,;~4V!JJ,!K(B */
  P2 (( long    , timer_no ), /* $B%?%$%^!<HV9f(B   */
      ( double *, second   )) /* $B7WB,;~4V!JIC!K(B */

long timesystemmin /* $B7WB,;~4V!JJ,!K(B */
  P2 (( long    , timer_no ), /* $B%?%$%^!<HV9f(B   */
      ( double *, second   )) /* $B7WB,;~4V!JIC!K(B */

long timelapusermin /* $B7WB,;~4V!JJ,!K(B */
  P2 (( long    , timer_no ), /* $B%?%$%^!<HV9f(B   */
      ( double *, second   )) /* $B7WB,;~4V!JIC!K(B */

long timelapsystemmin /* $B7WB,;~4V!JJ,!K(B */
  P2 (( long    , timer_no ), /* $B%?%$%^!<HV9f(B   */
      ( double *, second   )) /* $B7WB,;~4V!JIC!K(B */



/*======================*
 * dateRec : $BF|;~$NDj5A(B
 *======================*/
struct dateRec {
  long year, month, day, week;
  long hour, minute, second;
  char *s_month, *s_week;
  char str[ 64 ];
};



/*===================================*
 * timedate : $B;~4V$HF|;~$r7WB,$9$k!#(B
 *===================================*/
char *timedate /* $BF|;~$NJ8;zNs(B */
  P1 (( struct dateRec *, date )) /* $BF|;~$N7?%]%$%s%?(B */



#endif //__timelib_h
