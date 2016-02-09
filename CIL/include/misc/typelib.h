/*
 * filename : typelib.h
 * author   : Takahiro Sugiyama
 * redate   : 1993/09/14
 */



#ifndef __typelib_h
#define __typelib_h



#include "comacros.h"
//#include <malloc.h>
#include <memory.h>
#include <stdio.h>


/*==============================*
 * 画像処理で用いる画素型の宣言
 *==============================*/
typedef unsigned char packedbit1;
typedef unsigned char bit1;
typedef unsigned char bit4;


typedef unsigned char  uchar;

#include <sys/types.h>
/*
typedef unsigned short ushort;
typedef unsigned int   uint;
*/
//#ifdef __sys_types_h
typedef unsigned long  ulong;
//#endif __sys_types_h

typedef struct { char   at[ 2 ]; } char2;
typedef struct { short  at[ 2 ]; } short2;
typedef struct { int    at[ 2 ]; } int2;
typedef struct { long   at[ 2 ]; } long2;
typedef struct { uchar  at[ 2 ]; } uchar2;
typedef struct { ushort at[ 2 ]; } ushort2;
typedef struct { uint   at[ 2 ]; } uint2;
typedef struct { ulong  at[ 2 ]; } ulong2;
typedef struct { float  at[ 2 ]; } float2;
typedef struct { double at[ 2 ]; } double2;

typedef struct { char   at[ 3 ]; } char3;
typedef struct { short  at[ 3 ]; } short3;
typedef struct { int    at[ 3 ]; } int3;
typedef struct { long   at[ 3 ]; } long3;
typedef struct { uchar  at[ 3 ]; } uchar3;
typedef struct { ushort at[ 3 ]; } ushort3;
typedef struct { uint   at[ 3 ]; } uint3;
typedef struct { ulong  at[ 3 ]; } ulong3;
typedef struct { float  at[ 3 ]; } float3;
typedef struct { double at[ 3 ]; } double3;



/*================*
 * 型識別子の宣言
 *================*/
enum {
  _None = 0, PackedBit1, Bit1, Bit4,
  Char , Short , Int , Long , UChar , UShort , UInt , ULong , Float , Double ,
  Char2, Short2, Int2, Long2, UChar2, UShort2, UInt2, ULong2, Float2, Double2,
  Char3, Short3, Int3, Long3, UChar3, UShort3, UInt3, ULong3, Float3, Double3
};



/*===============================================*
 * typeinit : 型情報を初期化する。型の数を返す。
 *===============================================*/
long typeinit
  P0 ( void )



/*================================================*
 * typepeint : 型識別子 type の情報を表示します。
 *================================================*/
void typeprint
  P1 (( long, type )) /* 型識別子 */



/*========================================================================*
 * typeenter : 型名 name, サイズ size の型を登録して、型識別子を返します。
 *========================================================================*/
long typeenter /* 型識別子 */
  P2 (( char *, name ), /* 型名     */
      ( long  , size )) /* 型サイズ */



/*===========================================*
 * typeget : 型名 name の型識別子を返します。
 *===========================================*/
long typeget /* 型識別子 */
  P1 (( char *, name )) /* 型名 */



/*=================================================================*
 * typename : 型識別子 type の名前を文字列型のポインタで返します。
 * typesize : 型識別子 type のサイズをバイト単位で返します。
 *=================================================================*/
char *typename /* 型名 */
  P1 (( long, type )) /* 型識別子 */

long typesize /* 型サイズ */
  P1 (( long, type )) /* 型識別子 */

void typeprintf( long type ); /* 型識別子の名前を表示する */
void ftypeprintf( FILE *fp, long type ); /* FILE*に型識別子の名前を表示する */


/*========================================================================*
 * typeselect : 型 type と同じテーブルの型を持つ要素のポインタを返します。
 *		該当するものがない時、NULL(0) を返します。
 *========================================================================*/
char *typeselect /* 要素ポインタ */
  P4 (( long  ,type  ), /* 型識別子   */
      ( char *,table ), /* テーブル   */
      ( long  ,num   ), /* 要素数     */
      ( long  ,size  )) /* 要素サイズ */

/*=============================================================================*
 * typeselectconst : 型 type と同じテーブルの型を持つ要素のポインタを返します。
 *		     該当するものがない時、NULL(0) を返します。
 *		     table が const であるときに便利。
 *=============================================================================*/
#define typeselectconst( type, tbltype, table ) \
  ( tbltype * )typeselect( type, ( char* )table, Number( table ), sizeof( tbltype ) )



/*--------------------------------*
 * 配列にアクセスするためのマクロ
 *--------------------------------*/
#define PTR_1(data,x,size)     ((char *)(data)       + (x) * (size))
#define PTR_2(data,x,y,size)   ((char *)(data)[y]    + (x) * (size))
#define PTR_3(data,x,y,z,size) ((char *)(data)[z][y] + (x) * (size))

#define LET_0(dest,src,size)       memcpy(dest,src,size)
#define LET_1(data,x,ptr,size)     memcpy(PTR_1(data,x,size),ptr,size)
#define LET_2(data,x,y,ptr,size)   memcpy(PTR_2(data,x,y,size),ptr,size)
#define LET_3(data,x,y,z,ptr,size) memcpy(PTR_3(data,x,y,z,size),ptr,size)

#define COPY_1(dst,dx,src,sx,size) \
  LET_1(dst,dx,PTR_1(src,sx,size),size)

#define COPY_2(dst,dx,dy,src,sx,sy,size) \
  LET_2(dst,dx,dy,PTR_2(src,sx,sy,size),size)

#define COPY_3(dst,dx,dy,dz,src,sx,sy,sz,size) \
  LET_3(dst,dx,dy,dz,PTR_3(src,sx,sy,sz,size),size)



#endif //__typelib_h
