/*
 * init.c
 */



#include "Edit.h"



static char *item_list[] = {
  "input:*:1:out.edit:(image) name of the input edit image",
  "others:*:*:(image) name of the other input",
  "help:-h:print this message",
  "size:-sz:2:256:256:(int) x,y size of the image",
  "type:-t:1:bit1:(type) type of the image [bit1,uchar,uchar3]",
  "scale:-s:1:(float) scale of the edit window",
  "viewscale:-vs:1:1:(float) scale of the view window",
  "part:-p:4:(int) x,y,xsize,ysize region of the image",
  "palet-save:-ps:1:(palet) name of the output palet when quit",
  "palet-load:-pl:1:(palet) name of the input palet"
};



static void type_check(Edit e);
static long auto_scale(Edit e);

void Edit__init(Edit e,int argc,char *argv[])
{
  image_item item;
  char *input;

  OPTION_SETUP( item_list );

  if ( optspecified( "help" ) ) optmanual( 1 );

  item = Edit__original(e);
  imageItem__initialize(item);
  imageItem__initialize(Edit__previous(e));
  e->drawn     = Image.create("drawn");
  e->gradation = Image.create("gradation");
  e->diffusion = Image.create("diffusion");
  e->zoom.state = 0;
  e->coord_on = 0;
  e->quit_flag = 0;

  input = optvalue("input");
  if (imageItem__load(item,input) == 0)
    {
      long type = string__to_metatype(optvalue("type"));
      long xsize = optnvalueint("size",0);
      long ysize = optnvalueint("size",1);
      fprintf(stderr,"This is new image !!\n");
      fprintf(stderr,"    type = %s, xsize = %d, xsize = %d\n",
	      typename(type),xsize,ysize);
      imageItem__make(item,type,xsize,ysize);
    }
  imageItem__set_filename(item,input);
  imageItem__set_comment(item,ImageFile.getComment());

  fprintf(stderr,"Target image ...\n");
  Image.print(Edit__image(e));

  type_check(e);

  if (optspecified("part"))
    {
      char **list = optvaluelist("part");
      e->x = strtolong(list[0]);
      e->y = strtolong(list[1]);
      e->xsize = strtolong(list[2]);
      e->ysize = strtolong(list[3]);
    }
  else
    {
      e->x = 0;
      e->y = 0;
      e->xsize = Edit__image_xsize(e);
      e->ysize = Edit__image_ysize(e);
    }

  if (optspecified("scale"))
    e->edit.scale = optvalueint("scale");
  else
    e->edit.scale = auto_scale(e);

  e->view_scale = atof(optvalue("viewscale"));

  imageItem__copy(Edit__previous(e),item);

  Edit__drawn_make(e);
  if (optspecified("palet-load"))
    sprintf(e->palet_load,optvalue("palet-load"));
  if (optspecified("palet-save"))
    sprintf(e->palet_save,optvalue("palet-save"));
}



static void type_check(Edit e)
{
  long type = Image.type(Edit__image(e));

  if (type == Bit1 || type == UChar || type == UChar3)
    return;
  fprintf(stderr,"Bad image type !!\n");
  fprintf(stderr,"only Bit1, UChar, UChar3\n");
  exit(-1);
}



#define XMAX 512
#define YMAX 512

static long auto_scale(Edit e)
{
  long i;

  for (i = 2; (i * e->xsize <= XMAX) && (i * e->ysize <= YMAX); i ++);

  return i - 1;
}
