/*
 * draw.c
 */



#include "Edit.h"
#include <math.h>



#define MAXX 300
#define MAXH 20

static xw_panel panel_list[] = {
  {{"Bcc",(char *)&PointBM,(xw_handler)draw__sel,D_POINT,&SwitchV,&Box},
     {"dp",0,0,5,5}},
  {{"Bcc",(char *)&PointsBM,(xw_handler)draw__sel,D_POINTS,&SwitchV,&Box},
     {"dps","dp",0,-1,5}},
  {{"Bcc",(char *)&LineBM,(xw_handler)draw__sel,D_SEGMENT,&SwitchV,&Box},
     {"ds","dps",0,-1,5}},
  {{"Bcc",(char *)&LinesBM,(xw_handler)draw__sel,D_LINES,&SwitchV,&Box},
     {"dl","ds",0,-1,5}},
  {{"Bcc",(char *)&BoxBM,(xw_handler)draw__sel,D_BOX,&SwitchV,&Box},
     {"db","dl",0,-1,5}},
  {{"Bcc",(char *)&FillBoxBM,(xw_handler)draw__sel,F_BOX,&SwitchV,&Box},
     {"fb","db",0,-1,5}},

  {{"Bcc",(char *)&Circle1BM,(xw_handler)draw__sel,D_CIRCLE1,&SwitchV,&Box},
     {"dc1",0,"dp",5,-1}},
  {{"Bcc",(char *)&Circle2BM,(xw_handler)draw__sel,D_CIRCLE2,&SwitchV,&Box},
     {"dc2","dc1","dp",-1,-1}},
  {{"Bcc",(char *)&Circle3BM,(xw_handler)draw__sel,D_CIRCLE3,&SwitchV,&Box},
     {"dc3","dc2","dp",-1,-1}},
  {{"Bcc",(char *)&Circle4BM,(xw_handler)draw__sel,D_CIRCLE4,&SwitchV,&Box},
     {"dc4","dc3","dp",-1,-1}},
  {{"Bcc",(char *)&Ellipse1BM,(xw_handler)draw__sel,D_ELLIPSE1,&SwitchV,&Box},
     {"de1","dc4","dp",-1,-1}},
  {{"Bcc",(char *)&Ellipse2BM,(xw_handler)draw__sel,D_ELLIPSE2,&SwitchV,&Box},
     {"de2","de1","dp",-1,-1}},

  {{"Bcc",(char *)&FillCircle1BM,(xw_handler)draw__sel,F_CIRCLE1,&SwitchV,&Box},
     {"fc1",0,"dc1",5,-1}},
  {{"Bcc",(char *)&FillCircle2BM,(xw_handler)draw__sel,F_CIRCLE2,&SwitchV,&Box},
     {"fc2","fc1","dc1",-1,-1}},
  {{"Bcc",(char *)&FillCircle3BM,(xw_handler)draw__sel,F_CIRCLE3,&SwitchV,&Box},
     {"fc3","fc2","dc1",-1,-1}},
  {{"Bcc",(char *)&FillCircle4BM,(xw_handler)draw__sel,F_CIRCLE4,&SwitchV,&Box},
     {"fc4","fc3","dc1",-1,-1}},
  {{"Bcc",(char *)&FillEllipse1BM,(xw_handler)draw__sel,F_ELLIPSE1,&SwitchV,&Box},
     {"fe1","fc4","dc1",-1,-1}},
  {{"Bcc",(char *)&FillEllipse2BM,(xw_handler)draw__sel,F_ELLIPSE2,&SwitchV,&Box},
     {"fe2","fe1","dc1",-1,-1}},


  {{"Bcc",(char *)&PolyBM,(xw_handler)draw__sel,D_POLY,&SwitchV,&Box},
     {"dpo",0,"fc1",5,-1}},
  {{"Bcc",(char *)&FillPolyBM,(xw_handler)draw__sel,F_POLY,&SwitchV,&Box},
     {"fp","dpo","fc1",-1,-1}},
  {{"Bcc",(char *)&StoreAreaBM,(xw_handler)draw__sel,STORE_AREA,&SwitchV,&Box},
     {"sa","fp","fc1",-1,-1}},
  {{"Bcc",(char *)&CopyBM,(xw_handler)draw__sel,RESTORE_AREA,&SwitchV,&Box},
     {"ra","sa","fc1",-1,-1}},
  {{"Bcc",(char *)&ZoomBM,(xw_handler)draw__sel,ZOOM_AREA,&SwitchV,&Box},
     {"zo","ra","fc1",-1,-1}},
  {{"Bcc",(char *)&CloseBM,(xw_handler)draw__close,0,&CommandV,&Box},
     {"close","fe1","fc1",-1,-1}}
};



void draw__open(Edit e)
{
  e->draw.menu = XwObject.create(e->world,XwHard.class,0,"DrawMenu");
  XwObject.panel(e->draw.menu,XwNumber(panel_list),panel_list);

  e->draw.func = D_POINT;
  e->draw.on_object = 0;
  e->draw.state = 0;

  XwObject.child.setState(e->draw.menu,"dp",XW_REVERSE);
}



long draw__sel(Edit e,xw_object obj,XEvent *event,long value)
{
  char *name[] = {"dp","dps","ds","dl",
		    "db","dpo","dc1","dc2","dc3","dc4","de1","de2",
		    "fb","fp","fc1","fc2","fc3","fc4","fe1","fe2",
		    "sa","ra","zo"};

  if (value == e->draw.func) return 0;

  XwObject.child.setState(e->draw.menu,name[e->draw.func],XW_FRAME);

  e->draw.func = value;
  e->draw.on_object = 0;
  e->draw.state = 0;

  XwObject.child.setState(e->draw.menu,name[e->draw.func],XW_REVERSE);

  return 0;
}



long draw__close(Edit e)
{
  XwObject.unmap(e->draw.menu);
  return 0;
}
