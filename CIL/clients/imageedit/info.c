/*
 * info.c
 */



#include "Edit.h"



#define IX 48
#define IXW (48+4)
#define TX 264


static xw_panel panel_list[] = {
  {{"Bcc",(char *)&TitleBM,NoHandler,NoValue,&LabelV,&Box},
     {"title",NoName,NoName,FitSpace,FitSpace,TX,IXW}},
  {{"Bcc",(char *)&CloseBM,(xw_handler)info__close,NoValue,&CommandV,&Box},
     {"close",PrevName,NoName,StickSpace,SameSpace,IXW,IXW}}
};



void info__open(Edit e)
{
  e->info.menu = XwObject.create(e->world,XwHard.class,0,"InfoMenu");
  XwObject.panel(e->info.menu,XwNumber(panel_list),panel_list);
}



long info__close(Edit e)
{
  XwObject.unmap(e->info.menu);
  return 0;
}
