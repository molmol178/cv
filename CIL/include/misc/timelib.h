/*
 * filename : timelib.h
 * author   : Takahiro Sugiyama
 * date     : 1993/08/11
 */



#ifndef __timelib_h
#define __timelib_h



#include "comacros.h"



/*==================*
 * タイマーの最大数
 *==================*/
#define TIMER_MAX 8



/*====================================*
 * timestart   : 時間の計測の開始
 * timestop    : 時間の計測の終了
 * timepause   : 時間の計測の一時停止
 * timerestart : 時間の計測の再開
 *====================================*/
void timestart
  P1 (( long, timer_no )) /* タイマー番号 */

void timestop
  P1 (( long, timer_no )) /* タイマー番号 */

void timepause
  P1 (( long, timer_no )) /* タイマー番号 */

void timerestart
  P1 (( long, timer_no )) /* タイマー番号 */



/*=====================================*
 * timelapstart : ラップ時間の計測開始
 *=====================================*/
void timelapstart
  P1 (( long, timer_no )) /* タイマー番号 */



/*==============================================================*
 * timeuser      : 計測したユーザ時間を秒単位で返す。
 * timesystem    : 計測したシステム時間を秒単位で返す。
 * timelapuser   : 計測したラップのユーザ時間を秒単位で返す。
 * timelapsystem : 計測したラップのシステム時間を秒単位で返す。
 *
 * ポーズしていた時の時間は含まれてない。
 *==============================================================*/
double timeuser	/* 計測時間（秒） */
  P1 (( long, timer_no )) /* タイマー番号 */

double timesystem /* 計測時間（秒） */
  P1 (( long, timer_no )) /* タイマー番号 */

double timelapuser /* 計測時間（秒） */
  P1 (( long, timer_no )) /* タイマー番号 */

double timelapsystem /* 計測時間（秒） */
  P1 (( long, timer_no )) /* タイマー番号 */



/*===================================================================*
 * timeusermin      : 計測したユーザ時間を分秒単位で返す。
 * timesystemmin    : 計測したシステム時間を分秒単位で返す。
 * timelapusermin   : 計測したラップのユーザ時間を分秒単位で返す。
 * timelapsystemmin : 計測したラップのシステム時間を分秒単位で返す。
 *
 * ポーズしていた時の時間は含まれてない。
 *===================================================================*/
long timeusermin /* 計測時間（分） */
  P2 (( long    , timer_no ), /* タイマー番号   */
      ( double *, second   )) /* 計測時間（秒） */

long timesystemmin /* 計測時間（分） */
  P2 (( long    , timer_no ), /* タイマー番号   */
      ( double *, second   )) /* 計測時間（秒） */

long timelapusermin /* 計測時間（分） */
  P2 (( long    , timer_no ), /* タイマー番号   */
      ( double *, second   )) /* 計測時間（秒） */

long timelapsystemmin /* 計測時間（分） */
  P2 (( long    , timer_no ), /* タイマー番号   */
      ( double *, second   )) /* 計測時間（秒） */



/*======================*
 * dateRec : 日時の定義
 *======================*/
struct dateRec {
  long year, month, day, week;
  long hour, minute, second;
  char *s_month, *s_week;
  char str[ 64 ];
};



/*===================================*
 * timedate : 時間と日時を計測する。
 *===================================*/
char *timedate /* 日時の文字列 */
  P1 (( struct dateRec *, date )) /* 日時の型ポインタ */



#endif //__timelib_h
