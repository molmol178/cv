/*
 * imageItem.c
 */



#include "Edit.h"



static void ushort_to_uchar(img)
     image img;
{
  register int i, n;
  register uchar *d;
  register ushort *s;
  image tmp;

  tmp = Image.createFromImage( "TMP", img );
  Image.make( img, UChar, __XSIZE( tmp ), __YSIZE( tmp ) );

  n = __AREA( img );
  d = __RASTER( img, uchar );
  s = __RASTER( tmp, ushort );
  for ( i = 0; i < n; i++ ) *d++ = 100 * *s++;

  Image.destroy( tmp );
}



static void uchar_to_ushort(img)
     image img;
{
  register int i, n;
  register uchar *s;
  register ushort *d;
  image tmp;

  tmp = Image.createFromImage( "TMP", img );
  Image.make( img, UShort, __XSIZE( tmp ), __YSIZE( tmp ) );

  n = __AREA( img );
  d = __RASTER( img, ushort );
  s = __RASTER( tmp, uchar );
  for (i = 0; i < n; i++ ) *d++ = *s++ / 100;

  Image.destroy( tmp );
}



void imageItem__set_filename(image_item item,char *filename)
{
  if (filename)
    strcpy(item->filename,filename);
  else
    item->filename[0] = '\0';
}



void imageItem__set_edit(image_item item)
{
  item->state |= ItemEdit;
}



void imageItem__set_comment(image_item item,char *comment)
{
  if (comment)
    {
      strcpy(item->comment,comment);
    }
  else
    item->comment[0] = '\0';
}



void imageItem__set_state(image_item item,long state)
{
  item->state = state;
}



void imageItem__initialize(image_item item)
{
  item->img = Image.create("BUFF");
  item->mask = Image.create("MASK");
  item->filename[0] = '\0';
  item->comment[0] = '\0';
  item->state = ItemNotOccupy;
}



void imageItem__free(image_item item)
{
  Image.free(item->img);
  Image.free(item->mask);
  item->comment[0] = 0;
  item->filename[0] = 0;
  item->state = ItemNotOccupy;
}



long imageItem__save(image_item item,char *filename)
{
  long err;

  if ( item->original_type == UShort )
    {
      uchar_to_ushort(item->img);
    }
  err = Image.save(item->img,filename,item->comment);
  if ( item->original_type == UShort )
    {
      ushort_to_uchar(item->img);
    }
  if (err) imageItem__reset_edit(item);

  return err;
}


void imageItem__reset_edit(image_item item)
{
  item->state = (item->state & (~1));
}



long imageItem__load(image_item item,char *filename)
{
  if (Image.load(item->img,filename) == 0)
    {
      fprintf(stderr,"can't open file (%s) ... ignore !\n",filename);
      return 0;
    }
  item->original_type = __TYPE( item->img );
  if ( __TYPE( item->img ) == UShort )
    {
      item->original_type = UShort;
      fprintf( stderr, "translate ushort image to uchar image.\n" );
      ushort_to_uchar(item->img);
    }
  imageItem__set_filename(item,filename);
  imageItem__set_comment(item,ImageFile.getComment());
  imageItem__set_state(item,ItemLoad);
  Image.make(imageItem__mask(item),Bit1,
	     Image.xsize(item->img),Image.ysize(item->img));
  return 1;
}



void imageItem__swap(image_item dest,image_item src)
{
  Image.swap(dest->img,src->img);
  Image.swap(dest->mask,src->mask);
  string__swap(dest->filename,src->filename);
  string__swap(dest->comment,src->comment);
  long__swap(dest->state,src->state);
}



void imageItem__copy(image_item dest,image_item src)
{
  Image.copy(dest->img,src->img);
  Image.copy(dest->mask,src->mask);
  strcpy(dest->filename,src->filename);
  strcpy(dest->comment,src->comment);
  dest->state = src->state;
}



void imageItem__copy_area(image_item dest,image_item src,
			  long x1,long y1,long x2,long y2)
{
  long xsize,ysize;
  char buf[512];

  if (x1 > x2) long__swap(x1,x2);
  if (y1 > y2) long__swap(y1,y2);

  xsize = x2 - x1 + 1;
  ysize = y2 - y1 + 1;

  Image.sub(dest->img,src->img,x1,y1,xsize,ysize);
  Image.sub(dest->mask,src->mask,x1,y1,xsize,ysize);
  sprintf(buf,"part (%d,%d)-(%d,%d) of %s",x1,y1,x2,y2,src->filename);
  imageItem__set_comment(dest,buf);
  imageItem__set_filename(dest,"noname");
  imageItem__set_state(dest,ItemStore);
}



void imageItem__make(image_item item,long type,long xsize,long ysize)
{
  Image.make(item->img,type,xsize,ysize);
  Image.make(item->mask,Bit1,xsize,ysize);
  if (type != Bit1) image__reverse(item->img,item->img);
  image__reverse(item->mask,item->mask);
  imageItem__set_state(item,ItemCreate);
}



void imageItem__set_mask(image_item item)
{
  Image.make(item->mask,Bit1,Image.xsize(item->img),Image.ysize(item->img));
  image__reverse(item->mask,item->mask);
}
