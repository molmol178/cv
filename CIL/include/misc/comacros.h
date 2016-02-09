/*
 * filename : comacros.h
 * author   : Takahiro Sugiyama
 * redate   : 1993/09/18
 */



#ifndef __comacros_h
#define __comacros_h



/*===========================================================================*

C プログラムの関数の書き方:

     関数の前宣言は、関数の型チェックなど重要です。
     しかし、ANSI と K&R の CC か共存する中でこれを行なうのは困難です。
     どちらかを殺すしかないでしょう。
     このマクロは、ANSI C のときプロトタイプ宣言を行ない、K&R C のとき、
     通常の宣言だけを行なうためのマクロです。
     また、関数宣言時における、ANSI C と K&R C の違いも吸収します。
     これで、関数宣言の統一化ができます。

書き方:

     関数前宣言:
     <type> <funcname> P<n> (( <type@1>, <argname@1> ),
			     ( <type@2>, <argname@2> ),
			     ...
			     ( <type@n>, <argname@n> ));

関数宣言:
     <type> <funcname> _P<n> (( <type@1>, <argname@1> ),
			      ( <type@2>, <argname@2> ),
			      ...
			      ( <type@n>, <argname@n> ))
     {
       <code>
     }

例:関数前宣言

     \*==========================================================*
      * optinit : コマンドラインを指定されたオプションで解析する
      *==========================================================*\
     void optinit
       P4 (( int    ,op_argc ), \* オプションの数     *\
           ( char **,op_argv ), \* オプションのリスト *\ 
	   ( int    ,argc    ), \* 引数の数           *\
	   ( char **,argv    )) \* 引数のリスト       *\

      *============================*
      * optaddarg : 引数を追加する
      *============================*
     void optaddarg
       P1 (( char *, arg )) \* 引数 *\

例:関数宣言

     \*==========================================================*
      * optinit : コマンドラインを指定されたオプションで解析する
      *==========================================================*\
     void optinit
       _P4 (( int    ,op_argc ), \* オプションの数     *\
            ( char **,op_argv ), \* オプションのリスト *\ 
	    ( int    ,argc    ), \* 引数の数           *\
	    ( char **,argv    )) \* 引数のリスト       *\
     {
       ...;
     }
 
     \*============================*
      * optaddarg : 引数を追加する
      *============================*\
     void optaddarg
       _P1 (( char *, arg )) \* 引数 *\
     {
       ...;
     }

     関数の引数でのコメントは、わかりやすい言葉で書きましょう。
     '=' で上と下を挟んたときは、グローバルに参照できるものとして、
     '-' で上と下を挟んたときは、ローカルに参照できるものとして、
     決めておくといいでしょう。コメントの書き方を規制するわけでは
     ありませんが、統一されていれば、混乱はなくなるでしょう。

     また、さらに詳しい使い方の説明が必要と思われるものは、これ自身のように、
     書くと良いでしょう。

     ヘッダファイルそのものがリファレンスマニュアルとなるといいですね。

注意:
     関数前宣言と関数宣言の違いは、ほとんどないので、
     まず、関数前宣言を作ってから、それをコピーして、
     P<n> を _P<n> に変更すれば、そのまま使えます。
     二度、同じことを書くのは、効率が悪いだけでなく、
     思わぬバグを作ることになりますよ。

注意:
     マクロは、引数が、0 〜 15 までしか用意してません。
     これ以上たくさんの引数を指定するような関数は作らない方がいいでしょう。
     また、可変引数に関してのマクロは用意してませんが、
     可変引数の書き方の詳細を知っていれば、書くのは難しくないでしょう。
     参考としては、<varargs.h> を見るか、man varargs を見ましょう。
 *===========================================================================*/




/*=======================*
 * Number : 配列の要素数
 *=======================*/
#define Number(arr) (sizeof(arr)/sizeof(arr[0]))



/*=========================================*
 * P : 昔のプロトタイプ宣言用のマクロ
 *     新しく作る人はこれを使わないように。
 *=========================================*/
#ifdef __STDC__
#define P(X) X
#else
#define P(X) ()
#endif //__STDC__



/*====================================================*
 * P#  : 関数の前宣言で用いるプロトタイプ宣言用マクロ
 * _P# : 関数の宣言で用いるプロトタイプ宣言用マクロ
 *====================================================*/

#ifdef __STDC__

#define _Z(type,value) type value
#define _Y(type,value)
#define _P0(X) (X)

#define P0(X) _P0(X);
#define P1(a) _P1(a);
#define P2(a,b) _P2(a,b);
#define P3(a,b,c) _P3(a,b,c);
#define P4(a,b,c,d) _P4(a,b,c,d);
#define P5(a,b,c,d,e) _P5(a,b,c,d,e);
#define P6(a,b,c,d,e,f) _P6(a,b,c,d,e,f);
#define P7(a,b,c,d,e,f,g) _P7(a,b,c,d,e,f,g);
#define P8(a,b,c,d,e,f,g,h) _P8(a,b,c,d,e,f,g,h);
#define P9(a,b,c,d,e,f,g,h,i) _P9(a,b,c,d,e,f,g,h,i);
#define P10(a,b,c,d,e,f,g,h,i,j) _P10(a,b,c,d,e,f,g,h,i,j);
#define P11(a,b,c,d,e,f,g,h,i,j,k) _P11(a,b,c,d,e,f,g,h,i,j,k);
#define P12(a,b,c,d,e,f,g,h,i,j,k,l) _P12(a,b,c,d,e,f,g,h,i,j,k,l);
#define P13(a,b,c,d,e,f,g,h,i,j,k,l,m) _P13(a,b,c,d,e,f,g,h,i,j,k,l,m);
#define P14(a,b,c,d,e,f,g,h,i,j,k,l,m,n) _P14(a,b,c,d,e,f,g,h,i,j,k,l,m,n);
#define P15(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o) _P15(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o);

#else

#define _Z(type,value) value
#define _Y(type,value) type value;
#define _P0(X) ()

#define P0(X) ();
#define P1(a) ();
#define P2(a,b) ();
#define P3(a,b,c) ();
#define P4(a,b,c,d) ();
#define P5(a,b,c,d,e) ();
#define P6(a,b,c,d,e,f) ();
#define P7(a,b,c,d,e,f,g) ();
#define P8(a,b,c,d,e,f,g,h) ();
#define P9(a,b,c,d,e,f,g,h,i) ();
#define P10(a,b,c,d,e,f,g,h,i,j) ();
#define P11(a,b,c,d,e,f,g,h,i,j,k) ();
#define P12(a,b,c,d,e,f,g,h,i,j,k,l) ();
#define P13(a,b,c,d,e,f,g,h,i,j,k,l,m) ();
#define P14(a,b,c,d,e,f,g,h,i,j,k,l,m,n) ();
#define P15(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o) ();

#endif /* __STDC__ */



#define _P1(a)\
  (_Z a)\
  _Y a
#define _P2(a,b)\
  (_Z a,_Z b)\
  _Y a _Y b
#define _P3(a,b,c)\
  (_Z a,_Z b,_Z c)\
  _Y a _Y b _Y c
#define _P4(a,b,c,d)\
  (_Z a,_Z b,_Z c,_Z d)\
  _Y a _Y b _Y c _Y d
#define _P5(a,b,c,d,e)\
  (_Z a,_Z b,_Z c,_Z d,_Z e)\
  _Y a _Y b _Y c _Y d _Y e
#define _P6(a,b,c,d,e,f)\
  (_Z a,_Z b,_Z c,_Z d,_Z e,_Z f)\
  _Y a _Y b _Y c _Y d _Y e _Y f
#define _P7(a,b,c,d,e,f,g)\
  (_Z a,_Z b,_Z c,_Z d,_Z e,_Z f,_Z g)\
  _Y a _Y b _Y c _Y d _Y e _Y f _Y g
#define _P8(a,b,c,d,e,f,g,h)\
  (_Z a,_Z b,_Z c,_Z d,_Z e,_Z f,_Z g,_Z h)\
  _Y a _Y b _Y c _Y d _Y e _Y f _Y g _Y h
#define _P9(a,b,c,d,e,f,g,h,i)\
  (_Z a,_Z b,_Z c,_Z d,_Z e,_Z f,_Z g,_Z h,_Z i)\
  _Y a _Y b _Y c _Y d _Y e _Y f _Y g _Y h _Y i
#define _P10(a,b,c,d,e,f,g,h,i,j)\
  (_Z a,_Z b,_Z c,_Z d,_Z e,_Z f,_Z g,_Z h,_Z i,_Z j)\
  _Y a _Y b _Y c _Y d _Y e _Y f _Y g _Y h _Y i _Y j
#define _P11(a,b,c,d,e,f,g,h,i,j,k)\
  (_Z a,_Z b,_Z c,_Z d,_Z e,_Z f,_Z g,_Z h,_Z i,_Z j,_Z k)\
  _Y a _Y b _Y c _Y d _Y e _Y f _Y g _Y h _Y i _Y j _Y k
#define _P12(a,b,c,d,e,f,g,h,i,j,k,l)\
  (_Z a,_Z b,_Z c,_Z d,_Z e,_Z f,_Z g,_Z h,_Z i,_Z j,_Z k,_Z l)\
  _Y a _Y b _Y c _Y d _Y e _Y f _Y g _Y h _Y i _Y j _Y k _Y l
#define _P13(a,b,c,d,e,f,g,h,i,j,k,l,m)\
  (_Z a,_Z b,_Z c,_Z d,_Z e,_Z f,_Z g,_Z h,_Z i,_Z j,_Z k,_Z l,_Z m)\
  _Y a _Y b _Y c _Y d _Y e _Y f _Y g _Y h _Y i _Y j _Y k _Y l _Y m
#define _P14(a,b,c,d,e,f,g,h,i,j,k,l,m,n)\
  (_Z a,_Z b,_Z c,_Z d,_Z e,_Z f,_Z g,_Z h,_Z i,_Z j,_Z k,_Z l,_Z m,_Z n)\
  _Y a _Y b _Y c _Y d _Y e _Y f _Y g _Y h _Y i _Y j _Y k _Y l _Y m _Y n
#define _P15(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o)\
  (_Z a,_Z b,_Z c,_Z d,_Z e,_Z f,_Z g,_Z h,_Z i,_Z j,_Z k,_Z l,_Z m,_Z n,_Z o)\
  _Y a _Y b _Y c _Y d _Y e _Y f _Y g _Y h _Y i _Y j _Y k _Y l _Y m _Y n _Y o



#endif //__comacros_h
