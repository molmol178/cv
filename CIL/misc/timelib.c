/*
 * filename : timelib.c
 * author   : Takahiro Sugiyama
 * date     : 1993/08/11
 */



#include "timelib.h"
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#ifdef __SOLARIS__
#include <sys/rusage.h>
#endif



extern int getrusage(/* int who, struct rusage *rusage */);
extern time_t time(/* time_t *tloc */);

#ifdef __SOLARIS__
int getrusage(int who,struct rusage *ru)
{
  return 0;
}
#endif



/*==========================================*
 * timeRec : 実行時間や計算時間を計算する型
 *==========================================*/
#define TIMER_STOP  0
#define TIMER_RUN   1
#define TIMER_PAUSE 2

static struct timeRec {
  struct rusage start;
  struct rusage end;

  long   state;
  struct rusage pause;
  double pause_utime_sec;
  double pause_utime_usec;
  double pause_stime_sec;
  double pause_stime_usec;

  struct rusage lap;
  double lap_pause_utime_sec;
  double lap_pause_utime_usec;
  double lap_pause_stime_sec;
  double lap_pause_stime_usec;
} Time[ TIMER_MAX ];



/*================================*
 * timestart   : 時間の計測の開始
 *================================*/
void timestart
  _P1 (( long, no )) /* タイマー番号 */
{
  Time[ no ].state = TIMER_RUN;
  Time[ no ].pause_utime_sec  = 0;
  Time[ no ].pause_utime_usec = 0;
  Time[ no ].pause_stime_sec  = 0;
  Time[ no ].pause_stime_usec = 0;
  getrusage( RUSAGE_SELF, &Time[ no ].start );
  timelapstart( no );
}



/*=============================*
 * timestop : 時間の計測の終了
 *=============================*/
void timestop
  _P1 (( long, no )) /* タイマー番号 */
{
  switch ( Time[ no ].state )
    {
    case TIMER_PAUSE:
      timerestart( no );
    case TIMER_RUN:
      getrusage( RUSAGE_SELF, &Time[ no ].end );
    case TIMER_STOP:
      break;
    }
  Time[ no ].state = TIMER_STOP;
}



/*==================================*
 * timepause : 時間の計測の一時停止
 *==================================*/
void timepause
  _P1 (( long, no )) /* タイマー番号 */
{
  switch ( Time[ no ].state )
    {
    case TIMER_RUN:
    case TIMER_STOP:
      getrusage( RUSAGE_SELF, &Time[ no ].pause );
    case TIMER_PAUSE:
      break;
    }
  Time[ no ].state = TIMER_PAUSE;
}



/*================================*
 * timerestart : 時間の計測の再開
 *================================*/
void timerestart
  _P1 (( long, no )) /* タイマー番号 */
{
  double sec,usec;
  struct rusage now;

  switch ( Time[ no ].state )
    {
    case TIMER_PAUSE:
      getrusage( RUSAGE_SELF, &now );

      sec  = now.ru_utime.tv_sec  - Time[ no ].pause.ru_utime.tv_sec;
      usec = now.ru_utime.tv_usec - Time[ no ].pause.ru_utime.tv_usec;
      Time[ no ].pause_utime_sec  += sec;
      Time[ no ].pause_utime_usec += usec;
      Time[ no ].lap_pause_utime_sec  += sec;
      Time[ no ].lap_pause_utime_usec += usec;

      sec  = now.ru_stime.tv_sec  - Time[ no ].pause.ru_stime.tv_sec;
      usec = now.ru_stime.tv_usec - Time[ no ].pause.ru_stime.tv_usec;
      Time[ no ].pause_stime_sec  += sec;
      Time[ no ].pause_stime_usec += usec;
      Time[ no ].lap_pause_stime_sec  += sec;
      Time[ no ].lap_pause_stime_usec += usec;

    case TIMER_STOP:
    case TIMER_RUN:
      break;
    }
  Time[ no ].state = TIMER_RUN;
}



/*=====================================*
 * timelapstart : ラップ時間の計測開始
 *=====================================*/
void timelapstart
  _P1 (( long, no )) /* タイマー番号 */
{
  Time[ no ].lap_pause_utime_sec  = 0;
  Time[ no ].lap_pause_utime_usec = 0;
  Time[ no ].lap_pause_stime_sec  = 0;
  Time[ no ].lap_pause_stime_usec = 0;
  getrusage( RUSAGE_SELF, &Time[ no ].lap );
}



/*===================================================*
 * timeuser : 計測したユーザ時間を秒単位で返す。
 *	      ポーズしていた時の時間は含まれてない。
 *===================================================*/
double timeuser /* 計測時間（秒） */
  _P1 (( long, no )) /* タイマー番号 */
{
  double sec, usec;

  switch ( Time[ no ].state )
    {
    case TIMER_PAUSE:
      timerestart( no );
      timestop( no );
      timepause( no );
      break;

    case TIMER_RUN:
      timestop( no );
      timerestart( no );
      break;

    case TIMER_STOP:
      break;
    }

  sec  = Time[ no ].end.ru_utime.tv_sec  - Time[ no ].start.ru_utime.tv_sec;
  usec = Time[ no ].end.ru_utime.tv_usec - Time[ no ].start.ru_utime.tv_usec;
  sec  -= Time[ no ].pause_utime_sec;
  usec -= Time[ no ].pause_utime_usec;

  return sec + usec / 1000000.0;
}



/*=====================================================*
 * timesystem : 計測したシステム時間を秒単位で返す。
 * 		ポーズしていた時の時間は含まれてない。
 *=====================================================*/
double timesystem /* 計測時間（秒） */
  _P1 (( long, no )) /* タイマー番号 */
{
  double sec,usec;

  switch ( Time[ no ].state )
    {
    case TIMER_PAUSE:
      timerestart( no );
      timestop( no );
      timepause( no );
      break;

    case TIMER_RUN:
      timestop( no );
      timerestart( no );
      break;

    case TIMER_STOP:
      break;
    }

  sec  = Time[ no ].end.ru_stime.tv_sec  - Time[ no ].start.ru_stime.tv_sec;
  usec = Time[ no ].end.ru_stime.tv_usec - Time[ no ].start.ru_stime.tv_usec;
  sec  -= Time[ no ].pause_stime_sec;
  usec -= Time[ no ].pause_stime_usec;

  return sec + usec / 1000000.0;
}



/*==========================================================*
 * timelapuser : 計測したラップのユーザ時間を秒単位で返す。
 *		 ポーズしていた時の時間は含まれてない。
 *==========================================================*/
double timelapuser /* 計測時間（秒） */
  _P1 (( long, no )) /* タイマー番号 */
{
  double sec,usec;

  switch ( Time[ no ].state )
    {
    case TIMER_PAUSE:
      timerestart( no );
      timestop( no );
      timepause( no );
      break;

    case TIMER_RUN:
      timestop( no );
      timerestart( no );
      break;

    case TIMER_STOP:
      break;
    }

  sec  = Time[ no ].end.ru_utime.tv_sec  - Time[ no ].lap.ru_utime.tv_sec;
  usec = Time[ no ].end.ru_utime.tv_usec - Time[ no ].lap.ru_utime.tv_usec;
  sec  -= Time[ no ].lap_pause_utime_sec;
  usec -= Time[ no ].lap_pause_utime_usec;

  return sec + usec / 1000000.0;
}



/*=============================================================*
 * timelapsystem : 計測したラップのシステム時間を秒単位で返す。
 *		   ポーズしていた時の時間は含まれてない。
 *=============================================================*/
double timelapsystem /* 計測時間（秒） */
  _P1 (( long, no )) /* タイマー番号 */
{
  double sec,usec;

  switch ( Time[ no ].state )
    {
    case TIMER_PAUSE:
      timerestart( no );
      timestop( no );
      timepause( no );
      break;

    case TIMER_RUN:
      timestop( no );
      timerestart( no );
      break;

    case TIMER_STOP:
      break;
    }

  sec  = Time[ no ].end.ru_stime.tv_sec  - Time[ no ].lap.ru_stime.tv_sec;
  usec = Time[ no ].end.ru_stime.tv_usec - Time[ no ].lap.ru_stime.tv_usec;
  sec  -= Time[ no ].lap_pause_stime_sec;
  usec -= Time[ no ].lap_pause_stime_usec;

  return sec + usec / 1000000.0;
}



/*======================================================*
 * timeusermin : 計測したユーザ時間を分秒単位で返す。
 *		 ポーズしていた時の時間は含まれてない。
 *======================================================*/
long timeusermin /* 計測時間（分） */
  _P2 (( long    , no     ), /* タイマー番号   */
       ( double *, second )) /* 計測時間（秒） */
{
  double sec;
  long minute;

  sec = timeuser( no );

  minute = sec / 60;
  if ( second ) *second = sec - minute * 60;

  return minute;
}



/*========================================================*
 * timesystemmin : 計測したシステム時間を分秒単位で返す。
 *		   ポーズしていた時の時間は含まれてない。
 *========================================================*/
long timesystemmin /* 計測時間（分） */
  _P2 (( long    , no     ), /* タイマー番号   */
       ( double *, second )) /* 計測時間（秒） */
{
  double sec;
  long minute;

  sec = timesystem( no );

  minute = sec / 60;
  if ( second ) *second = sec - minute * 60; 

  return minute;
}



/*===============================================================*
 * timelapusermin : 計測したラップのユーザ時間を分秒単位で返す。
 * 		    ポーズしていた時の時間は含まれてない
 *===============================================================*/
long timelapusermin /* 計測時間（分） */
  _P2 (( long    , no     ), /* タイマー番号   */
       ( double *, second )) /* 計測時間（秒） */
{
  double sec;
  long minute;

  sec = timelapuser( no );

  minute = sec / 60;
  if ( second ) *second = sec - minute * 60; 

  return minute;
}



/*===================================================================*
 * timelapsystemmin : 計測したラップのシステム時間を分秒単位で返す。
 *		      ポーズしていた時の時間は含まれてない。
 *===================================================================*/
long timelapsystemmin /* 計測時間（分） */
  _P2 (( long    , no     ), /* タイマー番号   */
       ( double *, second )) /* 計測時間（秒） */
{
  double sec;
  long minute;

  sec = timelapsystem( no );

  minute = sec / 60;
  if ( second ) *second = sec - minute * 60; 

  return minute;
}



/*===================================*
 * timedate : 時間と日時を計測する。
 *===================================*/
char *timedate /* 日時の文字列 */
  _P1 (( struct dateRec *, date )) /* 日時の型ポインタ */
{
  time_t ct = time( 0 );
  struct tm *loctm = localtime( &ct );
  static struct dateRec dateR;

  static char *week[ 7 ] = {  "Sunday",
			      "Monday",
			      "Tuesday",
			      "Wednesday",
			      "Thursday",
			      "Friday",
			      "Saturday"};

  static char *month[ 12 ] = {  "January",
				"February",
				"March",
				"April",
				"May",
				"June",
				"July",
				"August",
				"September",
				"October",
				"November",
				"December"};

  if ( date == 0 ) date = &dateR;

  date->year    = loctm->tm_year;
  date->month   = loctm->tm_mon + 1;
  date->day     = loctm->tm_mday;
  date->week    = loctm->tm_wday;
  date->hour    = loctm->tm_hour;
  date->minute  = loctm->tm_min;
  date->second  = loctm->tm_sec;
  date->s_month = month[ loctm->tm_mon ];
  date->s_week  = week[ loctm->tm_wday ];

  sprintf( date->str, "%s %s %d 19%d %d:%d:%d",
	   date->s_week, date->s_month, date->day, date->year,
	   date->hour, date->minute, date->second );

  return date->str;
}
