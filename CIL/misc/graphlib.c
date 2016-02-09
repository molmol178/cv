/*
 * filename : graphlib.c
 * author   : Takahiro Sugiyama
 * date     : Thursday, June 2 1994
 * describe : Graph Structure Code
 */




#include "graphlib.h"
#include "memlib.h"
#include <stdio.h>



/*----------------------------------------------------------*
 * Graph__node_list_step : ノードリストの容量を増やす時の数
 *----------------------------------------------------------*/
long Graph__node_list_step = 16;

/*----------------------------------------------------------*
 * Graph__edge_list_step : エッジリストの容量を増やす時の数
 *----------------------------------------------------------*/
long Graph__edge_list_step = 32;



/*==========================================*
 * graph__create : グラフを新しく作成する。
 *==========================================*/
t_graph graph__create /* グラフを返す */
  _P2 (( long, node_data_size ), /* ノードのユーザデータのサイズ */
       ( long, edge_data_size )) /* エッジのユーザデータのサイズ */
{
  t_graph self;

  self = typenew( t_graphRec );

  self->node_data_size = node_data_size;
  self->edge_data_size = edge_data_size;

  self->node_capacity = 0;
  self->node_num = 0;
  self->node_list = 0;

  self->edge_capacity = 0;
  self->edge_num = 0;
  self->edge_list = 0;

  return self;
}



/*=========================================================*
 * graph__destroy : グラフを破壊する。ノードも破壊される。
 *=========================================================*/
void graph__destroy
  _P1 (( t_graph, self )) /* グラフ */
{
  if ( self == 0 ) return 0;
  graph__free( self );
  typefree( self );
}



/*======================================================*
 * graph__free : グラフを解放する。ノードも破壊される。
 *======================================================*/
void graph__free
  _P1 (( t_graph, self )) /* グラフ */
{
  int i;

  for ( i = 0; i < self->node_num; i++ )
    {
      t_node node = self->node_list[i];
      memfree( node->data );
      typefree1( node->neighbor_list );
    }

  for ( i = 0; i < self->edge_num; i++ )
    {
      t_edge edge = self->edge_list[i];
      memfree( edge->data );
    }

  typefree1( self->node_list );
  self->node_capacity = 0;
  self->node_num = 0;

  typefree1( self->edge_list );
  self->edge_capacity = 0;
  self->edge_num = 0;
}



/*======================================*
 * graph__node : グラフのノードを返す。
 *======================================*/
t_node graph__node /* ノードを返す */
  _P2 (( t_graph, self  ), /* グラフ */
       ( long   , index )) /* インデックス */
{
  if ( self->node_num >= index )
    {
      fprintf( stderr, "waring:graph__node:index (%d !< %d) is wrong.",
	       index, self->node_num );
      return 0;
    }

  return self->node_list[ index ];
}



/*======================================*
 * graph__edge : グラフのエッジを返す。
 *======================================*/
t_edge graph__edge /* エッジを返す */
  _P2 (( t_graph, self  ), /* グラフ */
       ( long   , index )) /* インデックス */
{
  if ( self->edge_num >= index )
    {
      fprintf( stderr, "waring:graph__edge:index (%d !< %d) is wrong.",
	       index, self->edge_num );
      return 0;
    }

  return self->edge_list[ index ];
}



/*==============================================*
 * graph__node_num : グラフのノードの数を返す。
 *==============================================*/
long graph__node_num /* ノード数を返す */
  _P1 (( t_graph, self )) /* グラフ */
{
  return self->node_num;
}



/*===================================================*
 * graph__node_list : グラフのノードのリストを返す。
 *===================================================*/
t_node *graph__node_list /* ノードリストを返す */
  _P1 (( t_graph, self )) /* グラフ */
{
  return self->node_list;
}



/*==============================================*
 * graph__edge_num : グラフのエッジの数を返す。
 *==============================================*/
long graph__edge_num /* エッジ数を返す */
  _P1 (( t_graph, self )) /* グラフ */
{
  return self->edge_num;
}



/*===================================================*
 * graph__edge_list : グラフのエッジのリストを返す。
 *===================================================*/
t_edge *graph__edge_list /* エッジリストを返す */
  _P1 (( t_graph, self )) /* グラフ */
{
  return self->edge_list;
}



/*================================================*
 * graph__add_node : グラフにノードを付け加える。
 *================================================*/
void graph__add_node
  _P2 (( t_graph, self ), /* グラフ */
       ( t_node , node )) /* ノード */
{
  long num;
  long capacity;
  t_node *list;

  capacity = self->node_capacity;
  list = self->node_list;
  num = self->node_num;
  if ( capacity >= num )
    {
    }
}



/*================================================*
 * graph__add_edge : グラフにエッジを付け加える。
 *================================================*/
void graph__add_edge
  P2 (( t_graph, self ), /* グラフ */
      ( t_edge , edge )) /* エッジ */

/*==============================================================*
 * graph__del_node : グラフのノードリストからノードを削除する。
 *==============================================================*/
void graph__del_node
  P2 (( t_graph, self ), /* グラフ */
      ( t_node , node )) /* ノード */

/*==============================================================*
 * graph__del_edge : グラフのエッジリストからエッジを削除する。
 *==============================================================*/
void graph__del_edge
  P2 (( t_graph, self ), /* グラフ */
      ( t_edge , edge )) /* エッジ */

/*==================================================*
 * graph__bind_nodes : グラフのノード間を接続する。
 *==================================================*/
t_edge graph__bind_nodes /* エッジを返す */
  P3 (( t_graph, self  ), /* グラフ */
      ( t_node , node1 ), /* ノード１ */
      ( t_node , node2 )) /* ノード２ */

/*=========================================================*
 * graph__key_to_node : ノード参照キーをノードに変換する。
 *=========================================================*/
t_node graph__key_to_node /* ノードを返す */
  P2 (( t_graph, self ), /* グラフ */
      ( long   , key  )) /* 参照キー */

/*==================================================================*
 * graph__key_bind_nodes : グラフのノード間をキーを使って接続する。
 *==================================================================*/
t_edge graph__key_bind_nodes /* エッジを返す */
  P3 (( t_graph, self ), /* グラフ */
      ( long   , key1 ), /* 参照キー１ */
      ( long   , key2 )) /* 参照キー２ */

/*========================================================================*
 * graph__keys_to_edge : グラフのノードの接続しているエッジをキーで返す。
 *========================================================================*/
t_edge graph__keys_to_edge /* エッジを返す */
  P3 (( t_graph, self ), /* グラフ */
      ( long   , key1 ), /* 参照キー１ */
      ( long   , key2 )) /* 参照キー２ */

/*===================================================================*
 * graph__nodes_to_edge : グラフのノードの接続しているエッジを返す。
 *===================================================================*/
t_edge graph__nodes_to_edge /* エッジを返す */
  P3 (( t_graph, self  ), /* グラフ */
      ( t_node , node1 ), /* ノード１ */
      ( t_node , node2 )) /* ノード２ */



static void __add_neighbor
  P2 (( t_node    , self     ),
      ( t_neighbor, neighbor ))




/*------------------------------------------------------*
 * edge__data_free : エッジのユーザデータを解放する関数
 *------------------------------------------------------*/
static f_edge__data_free edge__data_free = edge__data_free_default;

static void edge__data_free_default
  _P1 (( char *, data ))
{
  memfree( data );
}


/*======================================================================*
 * edge__set_data_free : エッジのユーザデータを解放する関数を設定する。
 *======================================================================*/
f_edge__data_free edge__set_data_free
  _P1 (( f_edge__data_free, func ))
{
  f_edge__data_free back;
  back = edge__data_free;
  edge__data_free = func;
  return back;
}



/*------------------------------------------------------*
 * node__data_free : ノードのユーザデータを解放する関数
 *------------------------------------------------------*/
static f_node__data_free node__data_free = node__data_free_default;

/*------------------------------------------------------*
 * node__data_free : ノードのユーザデータを解放する関数
 *------------------------------------------------------*/
static void node__data_free_default
  _P1 (( char *, data ))
{
  memfree( data );
}

/*======================================================================*
 * node__set_data_free : ノードのユーザデータを解放する関数を設定する。
 *======================================================================*/
f_node__data_free node__set_data_free
  _P1 (( f_node__data_free, func ))
{
  f_node__data_free back;
  back = node__data_free;
  node__data_free = func;
  return back;
}
