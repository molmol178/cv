/*
 * imageItem.h
 */



#define ItemNotOccupy     0
#define ItemEdit          1
#define ItemCreate        (1<<1)
#define ItemLoad          (1<<2)
#define ItemStore         (1<<3)
#define ItemCreateAndEdit (ItemCreate|ItemEdit)
#define ItemLoadAndEdit   (ItemLoad|ItemEdit)
#define ItemStoreAndEdit  (ItemStore|ItemEdit)



typedef struct image_itemRec {
  image img;
  image mask;
  char filename[256];
  char comment[256];
  long state;
} *image_item;


#define imageItem__image(item)    (item)->img
#define imageItem__mask(item)     (item)->mask
#define imageItem__filename(item) (item)->filename
#define imageItem__comment(item)  (item)->comment
#define imageItem__state(item)    (item)->state

void  imageItem__initialize(image_item item);
void  imageItem__free(image_item item);
long  imageItem__load(image_item item,char *filename);
long  imageItem__save(image_item item,char *filename);
void  imageItem__swap(image_item dest,image_item src);
void  imageItem__copy(image_item dest,image_item src);
void  imageItem__copy_area(image_item dest,image_item src,
			   long x1,long y1,long x2,long y2);
void imageItem__make(image_item item,long type,long xsize,long ysize);

void  imageItem__set_filename(image_item item,char *filename);
void  imageItem__set_comment(image_item item,char *comment);
void  imageItem__set_state(image_item item,long state);
void  imageItem__set_edit(image_item item);
void  imageItem__set_mask(image_item item);
void  imageItem__reset_edit(image_item item);

#define imageItem__occupy(item)     (imageItem__state(item) != ItemNotOccupy)
#define imageItem__not_occupy(item) (imageItem__state(item) == ItemNotOccupy)
#define imageItem__need_save(item)  (imageItem__state(item) & ItemEdit)
#define imageItem__not_need_save(item) (!(imageItem__state(item) & ItemEdit))
