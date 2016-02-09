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
 * Graph__node_list_step : �Ρ��ɥꥹ�Ȥ����̤����䤹���ο�
 *----------------------------------------------------------*/
long Graph__node_list_step = 16;

/*----------------------------------------------------------*
 * Graph__edge_list_step : ���å��ꥹ�Ȥ����̤����䤹���ο�
 *----------------------------------------------------------*/
long Graph__edge_list_step = 32;



/*==========================================*
 * graph__create : ����դ򿷤����������롣
 *==========================================*/
t_graph graph__create /* ����դ��֤� */
  _P2 (( long, node_data_size ), /* �Ρ��ɤΥ桼���ǡ����Υ����� */
       ( long, edge_data_size )) /* ���å��Υ桼���ǡ����Υ����� */
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
 * graph__destroy : ����դ��˲����롣�Ρ��ɤ��˲�����롣
 *=========================================================*/
void graph__destroy
  _P1 (( t_graph, self )) /* ����� */
{
  if ( self == 0 ) return 0;
  graph__free( self );
  typefree( self );
}



/*======================================================*
 * graph__free : ����դ�������롣�Ρ��ɤ��˲�����롣
 *======================================================*/
void graph__free
  _P1 (( t_graph, self )) /* ����� */
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
 * graph__node : ����դΥΡ��ɤ��֤���
 *======================================*/
t_node graph__node /* �Ρ��ɤ��֤� */
  _P2 (( t_graph, self  ), /* ����� */
       ( long   , index )) /* ����ǥå��� */
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
 * graph__edge : ����դΥ��å����֤���
 *======================================*/
t_edge graph__edge /* ���å����֤� */
  _P2 (( t_graph, self  ), /* ����� */
       ( long   , index )) /* ����ǥå��� */
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
 * graph__node_num : ����դΥΡ��ɤο����֤���
 *==============================================*/
long graph__node_num /* �Ρ��ɿ����֤� */
  _P1 (( t_graph, self )) /* ����� */
{
  return self->node_num;
}



/*===================================================*
 * graph__node_list : ����դΥΡ��ɤΥꥹ�Ȥ��֤���
 *===================================================*/
t_node *graph__node_list /* �Ρ��ɥꥹ�Ȥ��֤� */
  _P1 (( t_graph, self )) /* ����� */
{
  return self->node_list;
}



/*==============================================*
 * graph__edge_num : ����դΥ��å��ο����֤���
 *==============================================*/
long graph__edge_num /* ���å������֤� */
  _P1 (( t_graph, self )) /* ����� */
{
  return self->edge_num;
}



/*===================================================*
 * graph__edge_list : ����դΥ��å��Υꥹ�Ȥ��֤���
 *===================================================*/
t_edge *graph__edge_list /* ���å��ꥹ�Ȥ��֤� */
  _P1 (( t_graph, self )) /* ����� */
{
  return self->edge_list;
}



/*================================================*
 * graph__add_node : ����դ˥Ρ��ɤ��դ��ä��롣
 *================================================*/
void graph__add_node
  _P2 (( t_graph, self ), /* ����� */
       ( t_node , node )) /* �Ρ��� */
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
 * graph__add_edge : ����դ˥��å����դ��ä��롣
 *================================================*/
void graph__add_edge
  P2 (( t_graph, self ), /* ����� */
      ( t_edge , edge )) /* ���å� */

/*==============================================================*
 * graph__del_node : ����դΥΡ��ɥꥹ�Ȥ���Ρ��ɤ������롣
 *==============================================================*/
void graph__del_node
  P2 (( t_graph, self ), /* ����� */
      ( t_node , node )) /* �Ρ��� */

/*==============================================================*
 * graph__del_edge : ����դΥ��å��ꥹ�Ȥ��饨�å��������롣
 *==============================================================*/
void graph__del_edge
  P2 (( t_graph, self ), /* ����� */
      ( t_edge , edge )) /* ���å� */

/*==================================================*
 * graph__bind_nodes : ����դΥΡ��ɴ֤���³���롣
 *==================================================*/
t_edge graph__bind_nodes /* ���å����֤� */
  P3 (( t_graph, self  ), /* ����� */
      ( t_node , node1 ), /* �Ρ��ɣ� */
      ( t_node , node2 )) /* �Ρ��ɣ� */

/*=========================================================*
 * graph__key_to_node : �Ρ��ɻ��ȥ�����Ρ��ɤ��Ѵ����롣
 *=========================================================*/
t_node graph__key_to_node /* �Ρ��ɤ��֤� */
  P2 (( t_graph, self ), /* ����� */
      ( long   , key  )) /* ���ȥ��� */

/*==================================================================*
 * graph__key_bind_nodes : ����դΥΡ��ɴ֤򥭡���Ȥä���³���롣
 *==================================================================*/
t_edge graph__key_bind_nodes /* ���å����֤� */
  P3 (( t_graph, self ), /* ����� */
      ( long   , key1 ), /* ���ȥ����� */
      ( long   , key2 )) /* ���ȥ����� */

/*========================================================================*
 * graph__keys_to_edge : ����դΥΡ��ɤ���³���Ƥ��륨�å��򥭡����֤���
 *========================================================================*/
t_edge graph__keys_to_edge /* ���å����֤� */
  P3 (( t_graph, self ), /* ����� */
      ( long   , key1 ), /* ���ȥ����� */
      ( long   , key2 )) /* ���ȥ����� */

/*===================================================================*
 * graph__nodes_to_edge : ����դΥΡ��ɤ���³���Ƥ��륨�å����֤���
 *===================================================================*/
t_edge graph__nodes_to_edge /* ���å����֤� */
  P3 (( t_graph, self  ), /* ����� */
      ( t_node , node1 ), /* �Ρ��ɣ� */
      ( t_node , node2 )) /* �Ρ��ɣ� */



static void __add_neighbor
  P2 (( t_node    , self     ),
      ( t_neighbor, neighbor ))




/*------------------------------------------------------*
 * edge__data_free : ���å��Υ桼���ǡ������������ؿ�
 *------------------------------------------------------*/
static f_edge__data_free edge__data_free = edge__data_free_default;

static void edge__data_free_default
  _P1 (( char *, data ))
{
  memfree( data );
}


/*======================================================================*
 * edge__set_data_free : ���å��Υ桼���ǡ������������ؿ������ꤹ�롣
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
 * node__data_free : �Ρ��ɤΥ桼���ǡ������������ؿ�
 *------------------------------------------------------*/
static f_node__data_free node__data_free = node__data_free_default;

/*------------------------------------------------------*
 * node__data_free : �Ρ��ɤΥ桼���ǡ������������ؿ�
 *------------------------------------------------------*/
static void node__data_free_default
  _P1 (( char *, data ))
{
  memfree( data );
}

/*======================================================================*
 * node__set_data_free : �Ρ��ɤΥ桼���ǡ������������ؿ������ꤹ�롣
 *======================================================================*/
f_node__data_free node__set_data_free
  _P1 (( f_node__data_free, func ))
{
  f_node__data_free back;
  back = node__data_free;
  node__data_free = func;
  return back;
}
