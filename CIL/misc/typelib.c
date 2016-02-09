/*
 * filename : Type.c
 * author   : Takahiro Sugiyama
 * date     : 1993/01/29
 * redate   : 1993/05/19
 * redate   : 2015/10/26 for x86_64 64bit対応
 */



#include "typelib.h"
#include "memlib.h"
#include "strlib.h"



/*------------------------*
 * typeRec : 型情報の宣言
 *------------------------*/
struct typeRec { char *name;  long size; };



/*--------------------------------*
 * TypeNum   : 型テーブルの要素数
 * TypeTable : 型テーブルのリスト
 *--------------------------------*/
static long             TypeNum   = 0;
//static struct typeRec **TypeTable = NULL;
struct typeRec **TypeTable = NULL;


/*--------------------------------*
 * InitialType : 初期の型テーブル
 *--------------------------------*/
struct typeRec InitialType[] = {
  { "None"      , 0                    },
  { "PackedBit1", sizeof( packedbit1 ) },
  { "Bit1"      , sizeof( bit1       ) },
  { "bit4"      , sizeof( bit4       ) },

  { "Char"  , sizeof( char   ) },
  { "Short" , sizeof( short  ) },
  { "Int"   , sizeof( int    ) },
  { "Long"  , sizeof( long   ) },
  { "UChar" , sizeof( uchar  ) },
  { "UShort", sizeof( ushort ) },
  { "UInt"  , sizeof( uint   ) },
  { "ULong" , sizeof( ulong  ) },
  { "Float" , sizeof( float  ) },
  { "Double", sizeof( double ) },

  { "Char2"  , sizeof( char2   ) },
  { "Short2" , sizeof( short2  ) },
  { "Int2"   , sizeof( int2    ) },
  { "Long2"  , sizeof( long2   ) },
  { "UChar2" , sizeof( uchar2  ) },
  { "UShort2", sizeof( ushort2 ) },
  { "UInt2"  , sizeof( uint2   ) },
  { "ULong2" , sizeof( ulong2  ) },
  { "Float2" , sizeof( float2  ) },
  { "Double2", sizeof( double2 ) },

  { "Char3"  , sizeof( char3   ) },
  { "Short3" , sizeof( short3  ) },
  { "Int3"   , sizeof( int3    ) },
  { "Long3"  , sizeof( long3   ) },
  { "UChar3" , sizeof( uchar3  ) },
  { "UShort3", sizeof( ushort3 ) },
  { "UInt3"  , sizeof( uint3   ) },
  { "ULong3" , sizeof( ulong3  ) },
  { "Float3" , sizeof( float3  ) },
  { "Double3", sizeof( double3 ) }
};



/*----------------------------------------------*
 * __typeadd : 型ポインタを型テーブルに追加する。
 *----------------------------------------------*/
#include <stdio.h>
static long __typeadd /* 型識別子 */
  _P1 (( struct typeRec *, ptr )) /* 型ポインタ */
{
  long i;

  //  fprintf(stderr,"{%d,",TypeNum);
  i = TypeNum++;

  //  fprintf(stderr,".typerenew1(0x%08x,%d,%d);\n",TypeTable,TypeNum,sizeof(struct typeRec*));
  TypeTable = typerenew1( TypeTable, TypeNum, struct typeRec * );

  //  fprintf(stderr,"&(%08x[%d])->0x%08x,",TypeTable,i,&(TypeTable[i]));
  TypeTable[ i ] = ptr;

  //  fprintf(stderr,"}\n");

  return i;
}



/*===============================================*
 * typeinit : 型情報を初期化する。型の数を返す。
 *===============================================*/
#include <stdio.h>
long typeinit
  _P0 ( void )
{
  int i;

  TypeNum = Number( InitialType );
  TypeTable = typenew1( TypeNum, struct typeRec * );

  for ( i = 0; i < TypeNum; i++ )
    {
      //TypeTable[ i ] = &InitialType[ i ];
      TypeTable[ i ] = typenew(struct typeRec);
      TypeTable[ i ]->name = strnew(InitialType[ i ].name);
      TypeTable[ i ]->size = InitialType[ i ].size;
    }

  /*
  for ( i = 0; i < Number( InitialType ); i++ )
    {
      fprintf(stderr,"i=%d{..",i);
      __typeadd( &InitialType[ i ] );
      fprintf(stderr,"}\n");
    }
  */

  return Number( InitialType );
}



/*========================================================================*
 * typeenter : 型名 name, サイズ size の型を登録して、型識別子を返します。
 *========================================================================*/
long typeenter /* 型識別子 */
  _P2 (( char *, name ), /* 型名     */
       ( long  , size )) /* 型サイズ */
{
  long id;

  if ( TypeTable == 0 ) typeinit();

  id = typeget( name );

  if ( id == 0 )
    {
      struct typeRec *type;
      type       = typenew( struct typeRec );
      type->name = strnew( name );
      type->size = size;
      id = __typeadd( type );
    }

  return id;
}



/*===========================================*
 * typeget : 型名 name の型識別子を返します。
 *===========================================*/
#include <stdio.h>
long typeget /* 型識別子 */
  _P1 (( char *, name )) /* 型名 */
{
  int i;

  //  fprintf(stderr,"typeinit{");

  if ( TypeTable == 0 ) typeinit();

  //  fprintf(stderr,"..}");

  for ( i = 0; i < TypeNum; i++ )
    if ( strxequal( TypeTable[ i ]->name, name ) ) return i;

  return 0;
}



/*==========================================================*
 * typesize : 型識別子 type のサイズをバイト単位で返します。
 *==========================================================*/
long typesize /* 型サイズ */
  _P1 (( long, type )) /* 型識別子 */
{
  if ( TypeTable == 0 ) typeinit();

  if (( type < 0 ) || ( type >= TypeNum )) type = 0;

  return TypeTable[ type ]->size;
}



/*=================================================================*
 * typename : 型識別子 type の名前を文字列型のポインタで返します。
 *=================================================================*/
char *typename /* 型名 */
  _P1 (( long, type )) /* 型識別子 */
{
  // fprintf(stderr, "enter typename(type=%ld) = {\n",type);

  //  fprintf(stderr,"UChar3");

  //  return "UChar3";

  //  if ( TypeTable == NULL ) typeinit();
  if ( TypeNum == 0 ) typeinit();

  if (( type < 0 ) || ( type >= TypeNum )) type = 0;

  /*
  fprintf(stderr, "name = 0x%08x\n", (void*)TypeTable[ type ]->name);
  fprintf(stderr, "return [%s]\n", TypeTable[ type ]->name);
  fprintf(stderr, "}\n");
  */

  return TypeTable[ type ]->name;
}

/*=================================================================*
 * typeprint : 型識別子の名前を表示する
 *=================================================================*/
void typeprintf( long type )
{
  if (( type < 0 ) || ( type >= TypeNum )) type = 0;

  printf("%s", TypeTable[ type ]->name);
}

/*=================================================================*
 * ftypeprint : FILE*に型識別子の名前を表示する
 *=================================================================*/
void ftypeprintf( FILE *fp, long type )
{
  if (( type < 0 ) || ( type >= TypeNum )) type = 0;

  fprintf(fp, "%s", TypeTable[ type ]->name);
}


/*========================================================================*
 * typeselect : 型 type と同じテーブルの型を持つ要素のポインタを返します。
 *========================================================================*/
char *typeselect /* 要素ポインタ */
  _P4 (( long  ,type  ), /* 型識別子   */
       ( char *,table ), /* テーブル   */
       ( long  ,num   ), /* 要素数     */
       ( long  ,size  )) /* 要素サイズ */
{
  int i;

  if ( TypeTable == 0 ) typeinit();

  for ( i = 0; i < num; i++ )
    {
      struct elemRec { long type; } *elem;

      elem = ( struct elemRec * )( table + i * size );

      if ( type == elem->type ) return ( char * )elem;
    }

  return 0;
}


/*================================================*
 * typepeint : 型識別子 type の情報を表示します。
 *================================================*/
void typeprint
  _P1 (( long, type )) /* 型識別子 */
{
  if ( TypeTable == 0 ) typeinit();

  printf( "%s={id=%ld,size=%ld}\n", typename( type ), type, typesize( type ));
}
