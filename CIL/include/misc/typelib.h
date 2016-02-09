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
 * $B2hA|=hM}$GMQ$$$k2hAG7?$N@k8@(B
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
 * $B7?<1JL;R$N@k8@(B
 *================*/
enum {
  _None = 0, PackedBit1, Bit1, Bit4,
  Char , Short , Int , Long , UChar , UShort , UInt , ULong , Float , Double ,
  Char2, Short2, Int2, Long2, UChar2, UShort2, UInt2, ULong2, Float2, Double2,
  Char3, Short3, Int3, Long3, UChar3, UShort3, UInt3, ULong3, Float3, Double3
};



/*===============================================*
 * typeinit : $B7?>pJs$r=i4|2=$9$k!#7?$N?t$rJV$9!#(B
 *===============================================*/
long typeinit
  P0 ( void )



/*================================================*
 * typepeint : $B7?<1JL;R(B type $B$N>pJs$rI=<($7$^$9!#(B
 *================================================*/
void typeprint
  P1 (( long, type )) /* $B7?<1JL;R(B */



/*========================================================================*
 * typeenter : $B7?L>(B name, $B%5%$%:(B size $B$N7?$rEPO?$7$F!"7?<1JL;R$rJV$7$^$9!#(B
 *========================================================================*/
long typeenter /* $B7?<1JL;R(B */
  P2 (( char *, name ), /* $B7?L>(B     */
      ( long  , size )) /* $B7?%5%$%:(B */



/*===========================================*
 * typeget : $B7?L>(B name $B$N7?<1JL;R$rJV$7$^$9!#(B
 *===========================================*/
long typeget /* $B7?<1JL;R(B */
  P1 (( char *, name )) /* $B7?L>(B */



/*=================================================================*
 * typename : $B7?<1JL;R(B type $B$NL>A0$rJ8;zNs7?$N%]%$%s%?$GJV$7$^$9!#(B
 * typesize : $B7?<1JL;R(B type $B$N%5%$%:$r%P%$%HC10L$GJV$7$^$9!#(B
 *=================================================================*/
char *typename /* $B7?L>(B */
  P1 (( long, type )) /* $B7?<1JL;R(B */

long typesize /* $B7?%5%$%:(B */
  P1 (( long, type )) /* $B7?<1JL;R(B */

void typeprintf( long type ); /* $B7?<1JL;R$NL>A0$rI=<($9$k(B */
void ftypeprintf( FILE *fp, long type ); /* FILE*$B$K7?<1JL;R$NL>A0$rI=<($9$k(B */


/*========================================================================*
 * typeselect : $B7?(B type $B$HF1$8%F!<%V%k$N7?$r;}$DMWAG$N%]%$%s%?$rJV$7$^$9!#(B
 *		$B3:Ev$9$k$b$N$,$J$$;~!"(BNULL(0) $B$rJV$7$^$9!#(B
 *========================================================================*/
char *typeselect /* $BMWAG%]%$%s%?(B */
  P4 (( long  ,type  ), /* $B7?<1JL;R(B   */
      ( char *,table ), /* $B%F!<%V%k(B   */
      ( long  ,num   ), /* $BMWAG?t(B     */
      ( long  ,size  )) /* $BMWAG%5%$%:(B */

/*=============================================================================*
 * typeselectconst : $B7?(B type $B$HF1$8%F!<%V%k$N7?$r;}$DMWAG$N%]%$%s%?$rJV$7$^$9!#(B
 *		     $B3:Ev$9$k$b$N$,$J$$;~!"(BNULL(0) $B$rJV$7$^$9!#(B
 *		     table $B$,(B const $B$G$"$k$H$-$KJXMx!#(B
 *=============================================================================*/
#define typeselectconst( type, tbltype, table ) \
  ( tbltype * )typeselect( type, ( char* )table, Number( table ), sizeof( tbltype ) )



/*--------------------------------*
 * $BG[Ns$K%"%/%;%9$9$k$?$a$N%^%/%m(B
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
