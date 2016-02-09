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
 * timeRec : �¹Ի��֤�׻����֤�׻����뷿
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
 * timestart   : ���֤η�¬�γ���
 *================================*/
void timestart
  _P1 (( long, no )) /* �����ޡ��ֹ� */
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
 * timestop : ���֤η�¬�ν�λ
 *=============================*/
void timestop
  _P1 (( long, no )) /* �����ޡ��ֹ� */
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
 * timepause : ���֤η�¬�ΰ�����
 *==================================*/
void timepause
  _P1 (( long, no )) /* �����ޡ��ֹ� */
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
 * timerestart : ���֤η�¬�κƳ�
 *================================*/
void timerestart
  _P1 (( long, no )) /* �����ޡ��ֹ� */
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
 * timelapstart : ��å׻��֤η�¬����
 *=====================================*/
void timelapstart
  _P1 (( long, no )) /* �����ޡ��ֹ� */
{
  Time[ no ].lap_pause_utime_sec  = 0;
  Time[ no ].lap_pause_utime_usec = 0;
  Time[ no ].lap_pause_stime_sec  = 0;
  Time[ no ].lap_pause_stime_usec = 0;
  getrusage( RUSAGE_SELF, &Time[ no ].lap );
}



/*===================================================*
 * timeuser : ��¬�����桼�����֤���ñ�̤��֤���
 *	      �ݡ������Ƥ������λ��֤ϴޤޤ�Ƥʤ���
 *===================================================*/
double timeuser /* ��¬���֡��á� */
  _P1 (( long, no )) /* �����ޡ��ֹ� */
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
 * timesystem : ��¬���������ƥ���֤���ñ�̤��֤���
 * 		�ݡ������Ƥ������λ��֤ϴޤޤ�Ƥʤ���
 *=====================================================*/
double timesystem /* ��¬���֡��á� */
  _P1 (( long, no )) /* �����ޡ��ֹ� */
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
 * timelapuser : ��¬������åפΥ桼�����֤���ñ�̤��֤���
 *		 �ݡ������Ƥ������λ��֤ϴޤޤ�Ƥʤ���
 *==========================================================*/
double timelapuser /* ��¬���֡��á� */
  _P1 (( long, no )) /* �����ޡ��ֹ� */
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
 * timelapsystem : ��¬������åפΥ����ƥ���֤���ñ�̤��֤���
 *		   �ݡ������Ƥ������λ��֤ϴޤޤ�Ƥʤ���
 *=============================================================*/
double timelapsystem /* ��¬���֡��á� */
  _P1 (( long, no )) /* �����ޡ��ֹ� */
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
 * timeusermin : ��¬�����桼�����֤�ʬ��ñ�̤��֤���
 *		 �ݡ������Ƥ������λ��֤ϴޤޤ�Ƥʤ���
 *======================================================*/
long timeusermin /* ��¬���֡�ʬ�� */
  _P2 (( long    , no     ), /* �����ޡ��ֹ�   */
       ( double *, second )) /* ��¬���֡��á� */
{
  double sec;
  long minute;

  sec = timeuser( no );

  minute = sec / 60;
  if ( second ) *second = sec - minute * 60;

  return minute;
}



/*========================================================*
 * timesystemmin : ��¬���������ƥ���֤�ʬ��ñ�̤��֤���
 *		   �ݡ������Ƥ������λ��֤ϴޤޤ�Ƥʤ���
 *========================================================*/
long timesystemmin /* ��¬���֡�ʬ�� */
  _P2 (( long    , no     ), /* �����ޡ��ֹ�   */
       ( double *, second )) /* ��¬���֡��á� */
{
  double sec;
  long minute;

  sec = timesystem( no );

  minute = sec / 60;
  if ( second ) *second = sec - minute * 60; 

  return minute;
}



/*===============================================================*
 * timelapusermin : ��¬������åפΥ桼�����֤�ʬ��ñ�̤��֤���
 * 		    �ݡ������Ƥ������λ��֤ϴޤޤ�Ƥʤ�
 *===============================================================*/
long timelapusermin /* ��¬���֡�ʬ�� */
  _P2 (( long    , no     ), /* �����ޡ��ֹ�   */
       ( double *, second )) /* ��¬���֡��á� */
{
  double sec;
  long minute;

  sec = timelapuser( no );

  minute = sec / 60;
  if ( second ) *second = sec - minute * 60; 

  return minute;
}



/*===================================================================*
 * timelapsystemmin : ��¬������åפΥ����ƥ���֤�ʬ��ñ�̤��֤���
 *		      �ݡ������Ƥ������λ��֤ϴޤޤ�Ƥʤ���
 *===================================================================*/
long timelapsystemmin /* ��¬���֡�ʬ�� */
  _P2 (( long    , no     ), /* �����ޡ��ֹ�   */
       ( double *, second )) /* ��¬���֡��á� */
{
  double sec;
  long minute;

  sec = timelapsystem( no );

  minute = sec / 60;
  if ( second ) *second = sec - minute * 60; 

  return minute;
}



/*===================================*
 * timedate : ���֤��������¬���롣
 *===================================*/
char *timedate /* ������ʸ���� */
  _P1 (( struct dateRec *, date )) /* �����η��ݥ��� */
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
