/*
 * eventtest.c
 */



#include "Image.h"
#include "ImageDisp.h"
#include <stdio.h>
#include "misc/memlib.h"
#include "server/cilserver.h"



void labelinfo_calc
  P1 (( image, src ))



typedef struct labelTABLE {
  long area;
  short len;
  long x,y;
  long neg_num;
  long *neg_list;
} LabelTABLE;


long table_num = 0;
LabelTABLE *table_list = 0;
void labelinfo_calc P ((image src));


void addneg(l,p)
     LabelTABLE *l;
     long p;
{
  int i;

  for (i = 0; i < l->neg_num; i++)
    if (l->neg_list[i] == p) return;

  l->neg_num++;

  l->neg_list = typerenew1( l->neg_list, l->neg_num, long );

  l->neg_list[i] = p;
}



void print_label
  _P3 (( CILEvent *    , event ),
       ( register image, label ),
       ( register image, shm   ))
{
  int x,y;
  long pix;

  Image.clear( shm );

  pix = __PIXEL( label, event->mouse.x, event->mouse.y, long );
  for ( y = 0; y < label->ysize; y++ )
    for ( x = 0; x < label->xsize; x++ )
      if ( __PIXEL( label, x, y, long ) == pix ) __PIXEL_PBIT1_SET(shm,x,y);

  Image.display( shm, 0 );
}



void print_info
  _P3 (( CILEvent *, event ),
       ( image     , label ),
       ( image     , shm   ))
{
  long x,y,dy;
  long pix;
  packedbit1 **bits;
  LabelTABLE *l;
  char buf[256];

  Image.clear( shm );
  pix = ((long **)label->data)[event->mouse.y][event->mouse.x];
  l = &table_list[pix];
  bits = (packedbit1 **)shm->data;

  x = 10;
  y = 30;
  dy = 20;

  sprintf( buf, "  ID       : %d / %d\n", pix, table_num );
  ImageDrawString( shm, x, y, buf, strlen( buf ), 1 ); y += dy;

  sprintf( buf, "  Area     : %d\n", l->area );
  ImageDrawString( shm, x, y, buf, strlen( buf ), 1 ); y += dy;

  sprintf( buf, "  Length   : %d\n", l->len );
  ImageDrawString( shm, x, y, buf, strlen( buf ), 1 ); y += dy;

  sprintf( buf, "  Gx,Gy    : (%d,%d)\n", l->x, l->y );
  ImageDrawString( shm, x, y, buf, strlen( buf ), 1 ); y += dy;

  sprintf( buf, "  Neighbor : %d\n", l->neg_num );
  ImageDrawString( shm, x, y, buf, strlen( buf ), 1 ); y += dy;

  Image.display( shm,0 );
}



int main(argc,argv)
     int argc;
     char **argv;
{
  image src,shm,info;
  char *input = "-";
  CILEvent event;
  long px,py;

  px = py = 0;

  if ( ( argc == 2 ) && ( strcmp( "-h", argv[ 1 ] ) == 0 ) )
    {
      fprintf(stderr,"Usage : xlabelinfo [input]\n");
      return 1;
    }

  if ( argc > 1 ) input = argv[1];

  src = Image.createFromFilename(input,input);
  if (src == 0)
    {
      fprintf(stderr,"xlabelinfo:can't open (%s)\n",input);
      return -1;
    }

  if (src->type != Long)
    {
      fprintf(stderr,"xlabelinfo:wrong type (%s)\n",input);
      return -1;
    }

  Image.display(src,0);

  shm = Image.createMake("@xlabelinfo",PackedBit1,src->xsize,src->ysize);
  info = Image.createMake("@onlyinfo",PackedBit1,256,128);
  labelinfo_calc(src);

  ImageDispSelectEvent( src, MousePressMask );
  while ( 1 )
    {
      if ( ImageDispNextEvent( src, &event ) == -1 )
	{
	  cil_unlink( src->name );
	  cil_unlink( shm->name );
	  cil_unlink( info->name );
	  break;
	}

      if ( event.type != MousePress ) continue;

      if (( px == event.mouse.x ) && ( py == event.mouse.y )) continue;

      px = event.mouse.x;
      py = event.mouse.y;

      switch ( event.mouse.button )
	{
	case 1:
	  print_label(&event,src,shm);
	  print_info(&event,src,info);
	  break;

	case 2:
	  return 0;
	  break;

	case 3:
	  Image.undisplay( src );
	  Image.undisplay( shm );
	  Image.undisplay( info );
	  cil_unlink( shm->name );
	  cil_unlink( src->name );
	  cil_unlink( info->name );
	  return 0;
	  break;
	}
    }
}




void labelinfo_calc(src)
     register image src;
{
  register long i,x,y;

  table_num = 0;
  for (y = 0; y < src->ysize; y++)
    for (x = 0; x < src->xsize; x++)
      {
	long i = __PIXEL( src, x, y, long );
	if ( i > table_num ) table_num = i;
      }

  table_list = typenew1(table_num + 1, LabelTABLE);
  if (table_list == 0)
    {
      fprintf(stderr,"can't alloc memory. It's too large.\n");
      exit(-1);
    }

  for (i = 1; i <= table_num; i++) {
    table_list[i].area = 0;
    table_list[i].len = 0;
    table_list[i].x = 0;
    table_list[i].y = 0;
    table_list[i].neg_num = 0;
    table_list[i].neg_list = 0;
  };

  for (y = 0; y < src->ysize; y++)
    for (x = 0; x < src->xsize; x++)
      {
	long i, up, down, right, left;
	LabelTABLE *l;

	i = __PIXEL( src, x, y, long );

	l = &table_list[i];
	l->area ++;
	l->x += x;
	l->y += y;

	up = down = right = left = i;
	if (y == 0 || x == 0 || x == (src->xsize-1) || y == (src->ysize-1))
	  l->len++;

	if ( y != 0 )		       up    = __PIXEL( src, x, y - 1, long );
	if ( x != 0 )		       left  = __PIXEL( src, x - 1, y, long );
	if ( y != ( src->ysize - 1 ) ) down  = __PIXEL( src, x, y + 1, long );
	if ( x != ( src->xsize - 1 ) ) right = __PIXEL( src, x + 1, y, long );

	if ( up    != i ) { l->len++; addneg( l, up    ); }
	if ( left  != i ) { l->len++; addneg( l, left  ); }
	if ( right != i ) { l->len++; addneg( l, right ); }
	if ( down  != i ) { l->len++; addneg( l, down  ); }
      }

  for (i = 1; i <= table_num; i++)
    if (table_list[i].area)
      {
	table_list[i].x /= table_list[i].area;
	table_list[i].y /= table_list[i].area;
      }
}
