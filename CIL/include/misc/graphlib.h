/*
 * filename : graphlib.h
 * author   : Takahiro Sugiyama
 * date     : Wednesday, June 1 1994
 * describe : Graph Structure Library
 */



#ifndef __graphlib_h
#define __graphlib_h



#include "comacros.h"



/*========================*
 * ����չ�¤�ΤΥݥ���
 *========================*/
typedef struct _graphRec *t_graph;

/*========================*
 * �Ρ��ɹ�¤�ΤΥݥ���
 *========================*/
typedef struct _nodeRec *t_node;

/*========================*
 * ���å���¤�ΤΥݥ���
 *========================*/
typedef struct _edgeRec *t_edge;



/*=========================================================*
 * t_graph : �Ρ��ɤȥ��å��ν���򥰥�դȤ���������롣
 *=========================================================*/
typedef struct _graphRec {
  long    node_data_size; /* �Ρ��ɤΥǡ����Υ����� */
  long    edge_data_size; /* ���å��Υǡ����Υ����� */

  /* implement */
  long    node_capacity;  /* �Ρ��ɥꥹ�Ȥ����� */
  long    node_num;       /* �Ρ��ɤο� */
  t_node *node_list;      /* �Ρ��ɤΥꥹ�� */

  /* implement */
  long    edge_capacity;  /* ���å��ꥹ�Ȥ����� */
  long    edge_num;       /* ���å��ο� */
  t_edge *edge_list;      /* ���å��Υꥹ�� */
} t_graphRec;



/*------------------*
 * ���ܹ�¤�Τ����
 *------------------*/
typedef struct _neighborRec {
  /* implement */
  t_node node;  /* �Ρ��� */
  t_edge edge; /* ���å� */
} t_neighbor;

/*=================================================================*
 * t_node : ��ʬ�����ܤ��Ƥ���Ρ��ɤȥ��å��ν���Ȥ���������롣
 *=================================================================*/
typedef struct _nodeRec {
  char   *data; /* �桼���ǡ����Υݥ��� */
  long    key;  /* ���ȥ��� */

  /* implement */
  long        neighbor_capacity; /* ���ܥꥹ�Ȥ����� */
  long        neighbor_num;      /* ���ܤο� */
  t_neighbor *neighbor_list;     /* ���ܤΥꥹ�� */
} t_nodeRec;



/*============================================================*
 * t_edge : ��ʬ�����ܤ��Ƥ����������ΥΡ��ɤȤ���������롣
 *============================================================*/
typedef struct _edgeRec {
  char   *data;  /* �桼���ǡ����Υݥ��� */

  /* implement */
  t_node  node1; /* ���ܥΡ��ɣ� */
  t_node  node2; /* ���ܥΡ��ɣ� */
} t_edgeRec;



/*========================*
 * �����°�����ȥޥ�����
 *========================*/
#define GRAPH__EDGE(obj,index) (obj)->edge_list[index]
#define GRAPH__NODE(obj,index) (obj)->node_list[index]

#define GRAPH__EDGE_NUM(obj)  (obj)->edge_num
#define GRAPH__EDGE_LIST(obj) (obj)->edge_list
#define GRAPH__NODE_NUM(obj)  (obj)->node_num
#define GRAPH__NODE_LIST(obj) (obj)->node_list

/*==========================================*
 * graph__create : ����դ򿷤����������롣
 *==========================================*/
t_graph graph__create /* ����դ��֤� */
  P2 (( long, node_data_size ), /* �Ρ��ɤΥ桼���ǡ����Υ����� */
      ( long, edge_data_size )) /* ���å��Υ桼���ǡ����Υ����� */

/*=========================================================*
 * graph__destroy : ����դ��˲����롣�Ρ��ɤ��˲�����롣
 *=========================================================*/
void graph__destroy
  P1 (( t_graph, self )) /* ����� */

/*======================================================*
 * graph__free : ����դ�������롣�Ρ��ɤ��˲�����롣
 *======================================================*/
void graph__free
  P1 (( t_graph, self )) /* ����� */

/*======================================*
 * graph__node : ����դΥΡ��ɤ��֤���
 *======================================*/
t_node graph__node /* �Ρ��ɤ��֤� */
  P2 (( t_graph, self  ), /* ����� */
      ( long   , index )) /* ����ǥå��� */

/*======================================*
 * graph__edge : ����դΥ��å����֤���
 *======================================*/
t_edge graph__edge /* ���å����֤� */
  P2 (( t_graph, self  ), /* ����� */
      ( long   , index )) /* ����ǥå��� */

/*==============================================*
 * graph__node_num : ����դΥΡ��ɤο����֤���
 *==============================================*/
long graph__node_num /* �Ρ��ɿ����֤� */
  P1 (( t_graph, self )) /* ����� */

/*===================================================*
 * graph__node_list : ����դΥΡ��ɤΥꥹ�Ȥ��֤���
 *===================================================*/
t_node *graph__node_list /* �Ρ��ɥꥹ�Ȥ��֤� */
  P1 (( t_graph, self )) /* ����� */

/*==============================================*
 * graph__edge_num : ����դΥ��å��ο����֤���
 *==============================================*/
long graph__edge_num /* ���å������֤� */
  P1 (( t_graph, self )) /* ����� */

/*===================================================*
 * graph__edge_list : ����դΥ��å��Υꥹ�Ȥ��֤���
 *===================================================*/
t_edge *graph__edge_list /* ���å��ꥹ�Ȥ��֤� */
  P1 (( t_graph, self )) /* ����� */

/*================================================*
 * graph__add_node : ����դ˥Ρ��ɤ��դ��ä��롣
 *================================================*/
void graph__add_node
  P2 (( t_graph, self ), /* ����� */
      ( t_node , node )) /* �Ρ��� */

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



/*================================*
 * �Ρ��ɤ�°���򻲾Ȥ���ޥ�����
 *================================*/
#define NODE__DATA(node,type)      (type)((node)->data)
#define NODE__KEY(node)            (node)->key
#define NODE__NEIGHBOR(node,index) NODE__NEIGHBOR_LIST(node)[index]
#define NODE__nNODE(node,index)    NODE__NEIGHBOR(node,index).node
#define NODE__nEDGE(node,index)    NODE__NEIGHBOR(node,index).edge

#define NODE__NEIGHBOR_LIST(node)  (node)->neighbor_list
#define NODE__NEIGHBOR_NUM(node)   (node)->neighbor_num

/*=========================================*
 * node__create : �Ρ��ɤ򿷤����������롣
 *=========================================*/
t_node node__create /* �Ρ��ɤ��֤� */
  P2 (( long  , key  ), /* �Ρ��ɻ��ȥ��� */
      ( char *, data )) /* �桼���ǡ��� */

/*=======================================================*
 * node__destroy : �Ρ��ɤ�������롣���å��ϲ�������롣
 *=======================================================*/
void node__destroy
  P1 (( t_node, self )) /* �Ρ��� */

/*=====================================================*
 * node__free : �Ρ��ɤ�������롣���å��ϲ�������롣
 *=====================================================*/
void node__free
  P1 (( t_node, self )) /* �Ρ��� */

/*===========================================*
 * node__data : �Ρ��ɤΥ桼���ǡ������֤���
 *===========================================*/
char *node__data /* �桼���ǡ������֤� */
  P1 (( t_node, self )) /* �Ρ��� */

/*======================================*
 * node__key : �Ρ��ɤλ��ȥ������֤���
 *======================================*/
long node__key /* ���ȥ������֤� */
  P1 (( t_node, self ))  /* �Ρ��� */

/*==========================================*
 * node__neighbor_node : ���ܥΡ��ɤ��֤���
 *==========================================*/
t_node node__neighbor_node  /* �Ρ��ɤ��֤� */
  P2 (( t_node, self  ), /* �Ρ��� */
      ( long  , index )) /* ����ǥå��� */

/*==================================================*
 * node__neighbor_edge : ���ܥΡ��ɤΥ��å����֤���
 *==================================================*/
t_edge node__neighbor_edge /* ���å����֤� */
  P2 (( t_node, self  ), /* �Ρ��� */
      ( long  , index )) /* ����ǥå��� */

/*=============================================*
 * node__neighbor : ���ܹ�¤�Υݥ��󥿤��֤���
 *=============================================*/
t_neighbor *node__neighbor /* �ݥ��󥿤��֤� */
  P2 (( t_node, self  ), /* �Ρ��� */
      ( long  , index )) /* ����ǥå��� */

/*=====================================================*
 * node__neighbor_num : ���ܤ��Ƥ���Ρ��ɤο����֤���
 *=====================================================*/
long node__neighbor_num /* �Ρ��ɿ����֤� */
  P1 (( t_node, self )) /* �Ρ��� */

/*==================================================*
 * node__neighbor_list : ���ܤ��Ƥ���ꥹ�Ȥ��֤���
 *==================================================*/
t_neighbor *node__neighbor_list /* ���ܥꥹ�Ȥ��֤� */
  P1 (( t_node, self )) /* �Ρ��� */

/*=========================================*
 * node__add_node : ���ܥΡ��ɤ��ɲä��롣
 *=========================================*/
t_edge node__add_node /* ���å����֤� */
  P2 (( t_node, self ), /* �Ρ��� */
      ( t_node, node )) /* ���ܥΡ��� */

/*===================================================*
 * node__set_data : �桼���ǡ����򿷤������åȤ��롣
 *===================================================*/
char *node__set_data /* �Ť��桼���ǡ������֤� */
  P2 (( t_node, self ), /* �Ρ��� */
      ( char *, data )) /* �桼���ǡ��� */

/*==============================================*
 * node__set_key : �Ρ��ɤλ��ȥ������ѹ����롣
 *==============================================*/
long node__set_key /* �Ť����ȥ������֤� */
  P2 (( t_node, self ), /* �Ρ��� */
      ( long  , key  )) /* ���������ȥ��� */

/*===========================================================*
 * node__del_node : ���ܤ��Ƥ���Ρ��ɤ�ꥹ�Ȥ��������롣
 *===========================================================*/
long node__del_node /* ������줿�飱�����Ǥʤ���У� */
  P2 (( t_node, self ), /* �Ρ��� */
      ( t_node, node )) /* ����������Ρ��� */

/*========================================================================*
 * node__key_del_node : ���ȥ��������ܤ��Ƥ���Ρ��ɤ�ꥹ�Ȥ��������롣
 *========================================================================*/
long node__key_del_node /* ������줿�飱�����Ǥʤ���У� */
  P2 (( t_node, self ), /* �Ρ��� */
      ( long  , key  )) /* ������������ȥ��� */

/*==============================================================*
 * node__key_to_node : ���ܤ��Ƥ��뻲�ȥ�����Ρ��ɤ��Ѵ����롣
 *==============================================================*/
t_node node__key_to_node /* �Ρ��ɤ��֤� */
  P2 (( t_node, self ), /* �Ρ��� */
      ( long  , key  )) /* ���ȥ��� */

/*==============================================================*
 * node__key_to_edge : ���ܤ��Ƥ��뻲�ȥ����򥨥å����Ѵ����롣
 *==============================================================*/
t_edge node__key_to_edge /* ���å����֤� */
  P2 (( t_node, self ), /* �Ρ��� */
      ( long  , key  )) /* ���ȥ��� */

/*====================================================================*
 * node__key_to_neighbor : ���ܤ��Ƥ��뻲�ȥ��������ܹ�¤���Ѵ����롣
 *====================================================================*/
t_neighbor *node__key_to_neighbor  /* ���ܹ�¤�Υݥ��� */
  P2 (( t_node, self ), /* �Ρ��� */
      ( long  , key  )) /* ���ȥ��� */



/*======================*
 * ���å���°�����֤���
 *======================*/
#define EDGE__DATA(edge,type) (type)((edge)->data)
#define EDGE__NODE1(edge)     (edge)->node1
#define EDGE__NODE2(edge)     (edge)->node2

/*===================================================================*
 * edge__create : ���å��򿷤����������롣�Ρ��ɤ����Ƥ���Ѥ�롣
 *===================================================================*/
t_edge edge__create /* ���å����֤� */
  P3 (( t_node, node1 ), /* ���ܥΡ��ɣ� */
      ( t_node, node2 ), /* ���ܥΡ��ɣ� */
      ( char *, data  )) /* ���å�°��   */

/*=========================================================*
 * edge__destroy : ���å����˲����롣���ܥΡ��ɤ��Ѳ����롣
 *=========================================================*/
void edge__destroy
  P1 (( t_edge, self )) /* ���å� */

/*=======================================================*
 * edge__free : ���å���������롣���ܥΡ��ɤ��Ѳ����롣
 *=======================================================*/
void edge__free
  P1 (( t_edge, self )) /* ���å� */

/*====================================*
 * edge__node1 : ���ܥΡ��ɣ����֤���
 *====================================*/
t_node edge__node1 /* ���ܥΡ��ɣ����֤� */
  P1 (( t_edge, self )) /* ���å� */

/*====================================*
 * edge__node2 : ���ܥΡ��ɣ����֤���
 *====================================*/
t_node edge__node2 /* ���ܥΡ��ɣ����֤� */
  P1 (( t_edge, self )) /* ���å� */

/*===========================================*
 * edge__data : ���å��Υ桼���ǡ������֤���
 *===========================================*/
char *edge__data
  P1 (( t_edge, self )) /* ���å� */

/*=====================================================================*
 * edge__set_node : ���ܥΡ��ɤ��ѹ����롣���ܥΡ��ɤ����Ƥ��Ѳ����롣
 *=====================================================================*/
void edge__set_node
  P3 (( t_edge, self  ), /* ���å� */
      ( t_node, node1 ), /* �������Ρ��ɣ� */
      ( t_node, node2 )) /* �������Ρ��ɣ� */

/*=============================================*
 * edge__set_data : ���å��Υǡ������ѹ����롣
 *=============================================*/
char *edge__set_data /* ���Υǡ������֤� */
  P2 (( t_edge, self ), /* ���å� */
      ( char *, data )) /* ������°�� */



/*--------------------------------------------------------------*
 * f_edge__data_free : ���å��Υ桼���ǡ������������ؿ������
 *--------------------------------------------------------------*/
typedef void f_edge__data_free
  P1 (( char *, data ))

/*======================================================================*
 * edge__set_data_free : ���å��Υ桼���ǡ������������ؿ������ꤹ�롣
 *======================================================================*/
f_edge__data_free edge__set_data_free
  P1 (( f_edge__data_free, func ))



/*--------------------------------------------------------------*
 * f_node__data_free : �Ρ��ɤΥ桼���ǡ������������ؿ������
 *--------------------------------------------------------------*/
typedef void f_node__data_free
  P1 (( char *, data ))

/*======================================================================*
 * node__set_data_free : �Ρ��ɤΥ桼���ǡ������������ؿ������ꤹ�롣
 *======================================================================*/
f_node__data_free node__set_data_free
  P1 (( f_node__data_free, func ))



#endif  //__graphlib_h