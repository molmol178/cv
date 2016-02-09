/*
 * filename : Type.c
 * author   : Takahiro Sugiyama
 * date     : 1993/01/29
 * redate   : 1993/05/19
 * redate   : 2015/10/26 for x86_64 64bit$BBP1~(B
 */



#include "typelib.h"
#include "memlib.h"
#include "strlib.h"



/*------------------------*
 * typeRec : $B7?>pJs$N@k8@(B
 *------------------------*/
struct typeRec { char *name;  long size; };



/*--------------------------------*
 * TypeNum   : $B7?%F!<%V%k$NMWAG?t(B
 * TypeTable : $B7?%F!<%V%k$N%j%9%H(B
 *--------------------------------*/
static long             TypeNum   = 0;
//static struct typeRec **TypeTable = NULL;
struct typeRec **TypeTable = NULL;


/*--------------------------------*
 * InitialType : $B=i4|$N7?%F!<%V%k(B
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
 * __typeadd : $B7?%]%$%s%?$r7?%F!<%V%k$KDI2C$9$k!#(B
 *----------------------------------------------*/
#include <stdio.h>
static long __typeadd /* $B7?<1JL;R(B */
  _P1 (( struct typeRec *, ptr )) /* $B7?%]%$%s%?(B */
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
 * typeinit : $B7?>pJs$r=i4|2=$9$k!#7?$N?t$rJV$9!#(B
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
 * typeenter : $B7?L>(B name, $B%5%$%:(B size $B$N7?$rEPO?$7$F!"7?<1JL;R$rJV$7$^$9!#(B
 *========================================================================*/
long typeenter /* $B7?<1JL;R(B */
  _P2 (( char *, name ), /* $B7?L>(B     */
       ( long  , size )) /* $B7?%5%$%:(B */
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
 * typeget : $B7?L>(B name $B$N7?<1JL;R$rJV$7$^$9!#(B
 *===========================================*/
#include <stdio.h>
long typeget /* $B7?<1JL;R(B */
  _P1 (( char *, name )) /* $B7?L>(B */
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
 * typesize : $B7?<1JL;R(B type $B$N%5%$%:$r%P%$%HC10L$GJV$7$^$9!#(B
 *==========================================================*/
long typesize /* $B7?%5%$%:(B */
  _P1 (( long, type )) /* $B7?<1JL;R(B */
{
  if ( TypeTable == 0 ) typeinit();

  if (( type < 0 ) || ( type >= TypeNum )) type = 0;

  return TypeTable[ type ]->size;
}



/*=================================================================*
 * typename : $B7?<1JL;R(B type $B$NL>A0$rJ8;zNs7?$N%]%$%s%?$GJV$7$^$9!#(B
 *=================================================================*/
char *typename /* $B7?L>(B */
  _P1 (( long, type )) /* $B7?<1JL;R(B */
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
 * typeprint : $B7?<1JL;R$NL>A0$rI=<($9$k(B
 *=================================================================*/
void typeprintf( long type )
{
  if (( type < 0 ) || ( type >= TypeNum )) type = 0;

  printf("%s", TypeTable[ type ]->name);
}

/*=================================================================*
 * ftypeprint : FILE*$B$K7?<1JL;R$NL>A0$rI=<($9$k(B
 *=================================================================*/
void ftypeprintf( FILE *fp, long type )
{
  if (( type < 0 ) || ( type >= TypeNum )) type = 0;

  fprintf(fp, "%s", TypeTable[ type ]->name);
}


/*========================================================================*
 * typeselect : $B7?(B type $B$HF1$8%F!<%V%k$N7?$r;}$DMWAG$N%]%$%s%?$rJV$7$^$9!#(B
 *========================================================================*/
char *typeselect /* $BMWAG%]%$%s%?(B */
  _P4 (( long  ,type  ), /* $B7?<1JL;R(B   */
       ( char *,table ), /* $B%F!<%V%k(B   */
       ( long  ,num   ), /* $BMWAG?t(B     */
       ( long  ,size  )) /* $BMWAG%5%$%:(B */
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
 * typepeint : $B7?<1JL;R(B type $B$N>pJs$rI=<($7$^$9!#(B
 *================================================*/
void typeprint
  _P1 (( long, type )) /* $B7?<1JL;R(B */
{
  if ( TypeTable == 0 ) typeinit();

  printf( "%s={id=%ld,size=%ld}\n", typename( type ), type, typesize( type ));
}
