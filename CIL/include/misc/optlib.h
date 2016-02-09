/*
 * filename : optlib.h
 * author   : Takahiro Sugiyama
 * date     : 1993/09/17
 */



#ifndef __optlib_h
#define __optlib_h



#include "comacros.h"



/*==========================================================================*

     コマンドラインを指定されたオプションで解析する

     オプションは文字列で与えられ、その指定の仕方は、

     "名前:指示[:引数の数][:初期値]*:説明",

     である。


     --- 例 ---
     #include "optlib.h"

     char *options[] = {
       "images:*:*:( image ) name of the image."
       "xsize:-xs:1:xsize of the window",
       "ysize:-ys:1:ysize of the window",
       "size :-s:2:0:0:size of the window",
       "reverse:-r:Window reverse"
       };
     ...
     int main
     		_P2 (( int    , argc ),
		     ( char **, argv ))
     {
       ... ;
       OPTION_SETUP( options );
       ... ;
     }

 *==========================================================================*/



/*=======================================================*
 * optcommand : コマンド名 (argv[0])
 * optargc    : オプションで解析されなかった引数の数
 * optargv    : オプションで解析されなかった引数のリスト
 *=======================================================*/
extern char  *optcommand;
extern long   optargc;
extern char **optargv;



/*==========================================================*
 * optinit : コマンドラインを指定されたオプションで解析する
 *==========================================================*/
void optinit
  P4 (( int    ,op_argc ), /* オプションの数     */
      ( char **,op_argv ), /* オプションのリスト */
      ( int    ,argc    ), /* 引数の数           */
      ( char **,argv    )) /* 引数のリスト       */

#define OPTION_SETUP( option ) optinit( Number(option), option, argc, argv )



/*=========================================================================*
 * optmanual : オプションを整形して表示する
 * optusage  : オプションを簡単に表示する
 * optprint  : オプションの解析結果を表示します（主にデバッグ用）
 *=========================================================================*/
void optmanual
  P1 (( long, exit_code )) /* 終了コード */

void optusage
  P1 (( long, exit_code )) /* 終了コード */

void optprint
  P0  ( void )



/*=============================================================*
 * optvalue       : オプションの値を返す
 * optvalueint    : オプションの値を整数値に変換して返す
 * optvaluefloat  : オプションの値を実数値に変換して返す
 * optnvalue      : オプションのi番目の値を返す
 * optnvalueint   : オプションのi番目の値を整数値に変換して返す
 * optnvaluefloat : オプションのi番目の値を実数値に変換して返す
 * optvaluenum    : オプションの値の数を返す
 * optvaluelist   : オプションの値のリスト返す
 *=============================================================*/
char *optvalue /* オプションの値 */
  P1 (( char *, op_name )) /* オプション名 */

long optvalueint /* 整数値 */
  P1 (( char *, op_name )) /* オプション名 */

double optvaluefloat /* 実数値 */
  P1 (( char *, op_name )) /* オプション名 */


char *optnvalue /* オプションの値 */
  P2 (( char *, op_name ), /* オプション名 */
      ( long  , i       )) /* インデックス */

long optnvalueint /* 整数値 */
  P2 (( char *, op_name ), /* オプション名 */
      ( long  , i       )) /* インデックス */

double optnvaluefloat /* 実数値 */
  P2 (( char *, op_name ), /* オプション名 */
      ( long  , i       )) /* インデックス */


long optvaluenum /* オプションの値の数 */
  P1 (( char *, op_name )) /* オプション名 */

char **optvaluelist /* オプションの値のリスト */
  P1 (( char *, op_name )) /* オプション名 */



/*==============================================================*
 * optgetarg      : 引数のi番目の値を返す
 * optgetargint   : 引数のi番目の値を整数値に変換して返す
 * optgetargfloat : 引数のi番目の値を実数値に変換して返す
 *==============================================================*/
char *optgetarg
  P2 (( char *, name ), /* 名前 */
      ( long  , i    )) /* インデックス */

long optgetargint
  P2 (( char *, name ), /* 名前 */
      ( long  , i    )) /* インデックス */

double optgetargfloat
  P2 (( char *, name ), /* 名前 */
      ( long  , i    )) /* インデックス */



/*===================================================================*
 * optspecified : オプションが指定されたかどうか調べる
 *                指定されてるとき 1 を、指定されてないとき 0 を返す
 *===================================================================*/
long optspecified /* 真理値 */
  P1 (( char *, op_name )) /* オプション名 */



/*===============================================*
 * optset       : オプションに値を設定する
 * optsetvalues : オプションに複数の値を設定する
 *===============================================*/
void optset
  P2 (( char *, op_name ), /* オプション名 */
      ( char *, value   )) /* 値           */

void optsetvalues
  P3 (( char * , op_name ), /* オプション名 */
      ( int    , num     ), /* 値数         */
      ( char **, values  )) /* 値リスト     */



/*============================*
 * optaddarg : 引数を追加する
 *============================*/
void optaddarg
  P1 (( char *, arg )) /* 引数 */



/*=======================================*
 * optinput : オプションの入力要求をする
 *=======================================*/
void optinput
  P1 (( char *, name )) /* 0 のときはすべて */
			/* "NEED" のときは入力されてない必要なものだけ */
			/* "ALL" のときは入力されていないものすべて */
			/* その他 のときは入力されてない指定された名前 */



/*============================================*
 * optinputargs : 引数を追加を要求する
 *============================================*/
void optinputargs
  P1 (( long, num )) /* 引数の数 */
		     /* ゼロなら終了コードが入力されるまで */



/*===================================================*
 * optinputconfigure : コンフィグファイルを読み込む
 *===================================================*/
long optinputconfigure
  P1 (( char *, filename )) /* コンフィグファイル名 */



/*===================================================*
 * optoutputconfigure : コンフィグファイルを書き込む
 *===================================================*/
long optoutputconfigure
  P1 (( char *, filename )) /* コンフィグファイル名 */



/*==================================================================*
 * optifspecifiedcopy      : もし指定されていたらコピーする
 * optifspecifiedcopyint   : もし指定されていたら整数値をコピーする
 * optifspecifiedcopyfloat : もし指定されていたら実数値をコピーする
 *==================================================================*/
int optifspecifiedcopy /* コピーしたかどうか */
  P2 (( char *, name         ),
      ( char *, return_value ))

int optifspecifiedcopyint /* コピーしたかどうか */
  P2 (( char *, name         ),
      ( long *, return_value ))

int optifspecifiedcopyfloat /* コピーしたかどうか */
  P2 (( char   *, name         ),
      ( double *, return_value ))



/*============================================================*
 * optshellmode : オプションシェルモードに入る
 *============================================================*/
void optshellmode();



#endif  //__optlib_h
