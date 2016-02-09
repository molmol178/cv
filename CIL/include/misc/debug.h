/*
 * debug.h
 *     by Takahiro Sugiyama (1993.8.9)
 * デバッグ＆バーボーズモードのライブラリ
 */



#ifndef __Debug_h
#define __Debug_h



#include "comacros.h"



/*
 * デバッグレベル
 */
extern long Debug;

/*
 * バーボーズモード
 */
extern long Verbose;



/*
 * デバッグ判定ルール
 */
#define ifDebug(level) if ((Debug == -level) || (Debug >= level))

/*
 * デバッグモードでレベルを満たしていたら表示エラー出力にプリントする。
 */
void debugf(/* long level,char *fmt, ... */);

/*
 * デバッグレベルをセットする。
 */
void debugset(/* long level */);



/*
 * バーボーズモードなら標準出力にプリントする。
 */
void verbosef(/* char *fmt, ... */);

/*
 * バーボーズをセットする。
 */
void verboseset(/* void */);



#endif //__Debug_h
