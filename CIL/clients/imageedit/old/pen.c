/*
 * pen.c
 */



#include "Edit.h"
#include <math.h>



void xw__child_fill_image(Edit e,char *name,image img);
void xw__fill_image(Edit e,xw_object w,image img);



#define MAXH 20

#define PF_XW (PF_X+4)
#define PF_YW (PF_Y+4)
#define CUBEW (CUBE + 4)

static xw_panel panel_list[] = {
  {{"Bcc",(char *)&ShapeBM,NoHandler,NoValue,&Label},
     {"l1",NoName,NoName,FitSpace,FitSpace}},
  {{"Bcc",(char *)&BoxShapeBM,(xw_handler)pen__shape_sel,SHAPE_BOX,&SwitchV},
     {"box",PrevName,SameName,NoSpace,SameSpace}},
  {{"Bcc",(char *)&CircleShapeBM,(xw_handler)pen__shape_sel,SHAPE_CIRCLE,&SwitchV},
     {"circle",PrevName,SameName,StickSpace,SameSpace}},

  {{"Bcc",(char *)&DownArrayBM,(xw_handler)pen__percent_step,-1,&CommandV},
     {"c-",PrevName,SameName,FitSpace,SameSpace}},
  {{"cc","100%",(xw_handler)pen__percent_step,0,&LabelV},
     {"pers",PrevName,SameName,StickSpace,SameSpace,0,MAXH}},
  {{"Bcc",(char *)&UpArrayBM,(xw_handler)pen__percent_step,1,&CommandV},
     {"c+",PrevName,SameName,SameSpace,SameSpace}},

  {{"Bcc",(char *)&SizeBM,NoHandler,NoValue,&Label},
     {"l11",PrevName,SameName,FitSpace,SameSpace}},
  {{"Bcc",(char *)&DownArrayBM,(xw_handler)pen__size_step,-1,&CommandV},
     {"p-",PrevName,SameName,NoSpace,SameSpace}},
  {{"cc"," 1",(xw_handler)pen__size_step,0,&CommandV},
     {"p0",PrevName,SameName,StickSpace,SameSpace,0,MAXH}},
  {{"Bcc",(char *)&UpArrayBM,(xw_handler)pen__size_step,1,&CommandV},
     {"p+",PrevName,SameName,StickSpace,SameSpace}},

#define FX 40
#define FXS 5
  {{"Bcc",(char *)&FunctionBM,NoHandler,NoValue,&Label},
     {"l3",NoName,PrevName,FitSpace,FitSpace}},
  {{"Bcc",(char *)&SetBM,(xw_handler)pen__func_sel,FUNC_SET,&SwitchV},
     {"set",PrevName,SameName,NoSpace,SameSpace}},
  {{"Bcc",(char *)&MaxBM,(xw_handler)pen__func_sel,FUNC_MAX,&SwitchV},
     {"max",PrevName,SameName,StickSpace,SameSpace}},
  {{"Bcc",(char *)&MinBM,(xw_handler)pen__func_sel,FUNC_MIN,&SwitchV},
     {"min",PrevName,SameName,SameSpace,SameSpace}},
  {{"Bcc",(char *)&MeanBM,(xw_handler)pen__func_sel,FUNC_MEAN,&SwitchV},
     {"mean",PrevName,SameName,SameSpace,SameSpace}},
  {{"Bcc",(char *)&AddBM,(xw_handler)pen__func_sel,FUNC_ADD,&SwitchV},
     {"add",PrevName,SameName,SameSpace,SameSpace}},
  {{"Bcc",(char *)&SubBM,(xw_handler)pen__func_sel,FUNC_SUB,&SwitchV},
     {"sub",PrevName,SameName,SameSpace,SameSpace}},

  {{"Bcc",(char *)&NotBM,(xw_handler)pen__func_sel,FUNC_NOT,&SwitchV},
     {"not","l3",PrevName,NoSpace,StickSpace}},
  {{"Bcc",(char *)&OrBM,(xw_handler)pen__func_sel,FUNC_OR,&SwitchV},
     {"or",PrevName,SameName,StickSpace,SameSpace}},
  {{"Bcc",(char *)&AndBM,(xw_handler)pen__func_sel,FUNC_AND,&SwitchV},
     {"and",PrevName,SameName,SameSpace,SameSpace}},
  {{"Bcc",(char *)&XorBM,(xw_handler)pen__func_sel,FUNC_XOR,&SwitchV},
     {"xor",PrevName,SameName,SameSpace,SameSpace}},
  {{"Bcc",(char *)&DiffBM,(xw_handler)pen__func_sel,FUNC_DIFF,&SwitchV},
     {"diff",PrevName,SameName,SameSpace,SameSpace}},

/* diffusion */
  {{"Bcc",(char *)&DiffusionBM,NoHandler,NoValue,&Label},
     {"diffusion",NoName,PrevName,FitSpace,FitSpace}},
  {{"Bcc",(char *)&DownArrayBM,(xw_handler)pen__diffusion_size_step,DXS_M,&CommandV},
     {"dx+",PrevName,SameName,NoSpace,SameSpace}},
  {{"cc","X:1",(xw_handler)pen__diffusion_size_step,DXS_O,&CommandV},
     {"dx0",PrevName,SameName,StickSpace,SameSpace,0,MAXH}},
  {{"Bcc",(char *)&UpArrayBM,(xw_handler)pen__diffusion_size_step,DXS_P,&CommandV},
     {"dx-",PrevName,SameName,SameSpace,SameSpace}},

  {{"Bcc",(char *)&DownArrayBM,(xw_handler)pen__diffusion_size_step,DYS_M,&CommandV},
     {"dy+","diffusion",PrevName,NoSpace,StickSpace}},
  {{"cc","Y:1",(xw_handler)pen__diffusion_size_step,DYS_O,&CommandV},
     {"dy0",PrevName,SameName,StickSpace,SameSpace,0,MAXH}},
  {{"Bcc",(char *)&UpArrayBM,(xw_handler)pen__diffusion_size_step,DYS_P,&CommandV},
     {"dy-",PrevName,SameName,SameSpace,SameSpace}},

  {{"Bcc",(char *)&StrengthBM,NoHandler,NoValue,&Label},
     {"strength","dx-","diff",FitSpace,FitSpace}},
  {{"Bcc",(char *)&DownArrayBM,(xw_handler)pen__diffusion_strength_step,DS_M,&CommandV},
     {"ds+",PrevName,SameName,NoSpace,SameSpace}},
  {{"cc"," 1",(xw_handler)pen__diffusion_strength_step,DS_O,&CommandV},
     {"ds0",PrevName,SameName,StickSpace,SameSpace,0,MAXH}},
  {{"Bcc",(char *)&UpArrayBM,(xw_handler)pen__diffusion_strength_step,DS_P,&CommandV},
     {"ds-",PrevName,SameName,SameSpace,SameSpace}},

/* select */
  {{"Bcc",(char *)&SelectBM,NoHandler,NoValue,&Label},
     {"select",NoName,"dy-",FitSpace,FitSpace}},
  {{"Bcc",(char *)&OnBM,(xw_handler)pen__select_onoff,1,&SwitchV},
     {"son",PrevName,SameName,NoSpace,SameSpace}},
  {{"Bcc",(char *)&OffBM,(xw_handler)pen__select_onoff,0,&SwitchV},
     {"soff",PrevName,SameName,StickSpace,SameSpace}},
  {{"Bcc",(char *)&PosiBM,(xw_handler)pen__negative_sel,0,&SwitchV},
     {"non",PrevName,SameName,FitSpace,SameSpace}},
  {{"Bcc",(char *)&NegaBM,(xw_handler)pen__negative_sel,1,&SwitchV},
     {"noff",PrevName,SameName,StickSpace,SameSpace}},
  {{"cc",0,(xw_handler)pen__change_select,GCH1,&LabelV},
     {"src0",PrevName,SameName,FitSpace,SameSpace,FX,MAXH}},
  {{"Bcc",(char *)&RightArrayBM,NoHandler,NoValue,&Label},
     {"src-l1",PrevName,SameName,NoSpace,SameSpace}},
  {{"cc",0,(xw_handler)pen__change_select,GCH2,&LabelV},
     {"src1",PrevName,SameName,NoSpace,SameSpace,FX,MAXH}},

  {{"cc",0,(xw_handler)pen__change_select,CCH1,&LabelV},
     {"src2","noff",PrevName,FitSpace,SameSpace,FX,MAXH}},
  {{"Bcc",(char *)&RightArrayBM,NoHandler,NoValue,&Label},
     {"src-l2",PrevName,SameName,NoSpace,SameSpace}},
  {{"cc",0,(xw_handler)pen__change_select,CCH2,&LabelV},
     {"src3",PrevName,SameName,NoSpace,SameSpace,FX,MAXH}},

#define GRA_X 128
  {{"Bcc",(char *)&GradationBM,NoHandler,NoValue,&Label},
     {"gradation",NoName,PrevName,FitSpace,FitSpace}},
  {{"Bcc",(char *)&OnBM,(xw_handler)pen__gradation_onoff,1,&SwitchV},
     {"gon",PrevName,SameName,NoSpace,SameSpace}},
  {{"Bcc",(char *)&OffBM,(xw_handler)pen__gradation_onoff,0,&SwitchV},
     {"goff",PrevName,SameName,StickSpace,SameSpace}},

  {{"Bcc",(char *)&LeftUpArrayBM,(xw_handler)pen__gradation_direction_sel,GRAD_LeftUp,&LabelV},
     {"glu",NoName,"gradation",30,FitSpace}},
  {{"Bcc",(char *)&UpArrayBM,(xw_handler)pen__gradation_direction_sel,GRAD_Up,&LabelV},
     {"gu",PrevName,SameName,StickSpace,SameSpace}},
  {{"Bcc",(char *)&RightUpArrayBM,(xw_handler)pen__gradation_direction_sel,GRAD_RightUp,&LabelV},
     {"gru",PrevName,SameName,StickSpace,SameSpace}},

  {{"Bcc",(char *)&LeftArrayBM,(xw_handler)pen__gradation_direction_sel,GRAD_Left,&LabelV},
     {"gl",NoName,PrevName,30,StickSpace}},
  {{"Bcc",(char *)&CrossBM,(xw_handler)pen__gradation_direction_sel,GRAD_Center,&LabelV},
     {"gc",PrevName,SameName,StickSpace,SameSpace}},
  {{"Bcc",(char *)&RightArrayBM,(xw_handler)pen__gradation_direction_sel,GRAD_Right,&LabelV},
     {"gr",PrevName,SameName,StickSpace,SameSpace}},

  {{"Bcc",(char *)&LeftDownArrayBM,(xw_handler)pen__gradation_direction_sel,GRAD_LeftDown,&LabelV},
     {"gld",NoName,PrevName,30,StickSpace}},
  {{"Bcc",(char *)&DownArrayBM,(xw_handler)pen__gradation_direction_sel,GRAD_Down,&LabelV},
     {"gd",PrevName,SameName,StickSpace,SameSpace}},
  {{"Bcc",(char *)&RightDownArrayBM,(xw_handler)pen__gradation_direction_sel,GRAD_RightDown,&LabelV},
     {"grd",PrevName,SameName,StickSpace,SameSpace}},

  {{"cc",0,(xw_handler)pen__change_gradation,GRAY_P,&LabelV},
     {"gg+","goff","src3",FitSpace,FitSpace,MAXH,MAXH}},
  {{"cc",0,(xw_handler)pen__gradation_gray_sel,NoValue,&LabelV},
     {"gg0",PrevName,SameName,StickSpace,SameSpace,GRA_X,MAXH}},
  {{"cc",0,(xw_handler)pen__change_gradation,GRAY_M,&LabelV},
     {"gg-",PrevName,SameName,SameSpace,SameSpace,MAXH,MAXH}},

  {{"cc",0,(xw_handler)pen__change_gradation,COLOR_P,&LabelV},
     {"gc+","goff",PrevName,FitSpace,FitSpace,MAXH,MAXH}},
  {{"cc",0,(xw_handler)pen__gradation_color_sel,NoValue,&LabelV},
     {"gc0",PrevName,SameName,StickSpace,SameSpace,GRA_X,MAXH}},
  {{"cc",0,(xw_handler)pen__change_gradation,COLOR_M,&LabelV},
     {"gc-",PrevName,SameName,SameSpace,SameSpace,MAXH,MAXH}},


#define CRX MAXH
#define CRY MAXH
#define CCRY (20 * 3 + 4 + 10)
#define BARX 128

  {{"Bcc",(char *)&Bit1BM,NoHandler,NoValue,&Label},
     {"l4",NoName,"gd",FitSpace,FitSpace+MAXH+MAXH+8}},
  {{"cc",NoLabel,NoHandler,NoValue,&LabelV},
     {"BR",PrevName,SameName,NoSpace,SameSpace,CRX,CRY}},
  {{"Bcc",(char *)&FalseBM,(xw_handler)pen__binary_sel,BIN_0,&SwitchV},
     {"false",PrevName,SameName,FitSpace,SameSpace}},
  {{"Bcc",(char *)&TrueBM,(xw_handler)pen__binary_sel,BIN_1,&SwitchV},
     {"true",PrevName,SameName,StickSpace,SameSpace}},

  {{"Bcc",(char *)&UCharBM,NoHandler,NoValue,&Label},
     {"l5",NoName,PrevName,FitSpace,FitSpace}},
  {{"cc",NoLabel,NoHandler,NoValue,&LabelV},
     {"GR",PrevName,SameName,NoSpace,SameSpace,CRX,CRY}},
  {{"Bcc",(char *)&LeftArrayBM,(xw_handler)pen__gray_step,-1,&CommandV},
     {"g-",PrevName,SameName,FitSpace,SameSpace}},
  {{"cc","M:255",NoHandler,NoValue,&LabelV},
     {"g0",PrevName,SameName,StickSpace,SameSpace,0,MAXH}},
  {{"Bcc",(char *)&RightArrayBM,(xw_handler)pen__gray_step,1,&CommandV},
     {"g+",PrevName,SameName,SameSpace,SameSpace}},
  {{"cc",NoLabel,(xw_handler)pen__gray_sel,NoValue,&LabelV},
     {"graysel",PrevName,SameName,FitSpace,SameSpace,BARX,MAXH}},

  {{"Bcc",(char *)&UChar3BM,NoHandler,NoValue,&Label},
     {"l6",NoName,PrevName,FitSpace,FitSpace}},
  {{"cc",NoLabel,NoHandler,NoValue,&LabelV},
     {"CR",PrevName,SameName,NoSpace,SameSpace,CRX,CCRY}},

  {{"Bcc",(char *)&LeftArrayBM,(xw_handler)pen__color_step,-1,&CommandV},
     {"R-",PrevName,SameName,FitSpace,SameSpace}},
  {{"cc","R:255",NoHandler,NoValue,&LabelV},
     {"R0",PrevName,SameName,StickSpace,SameSpace,0,MAXH}},
  {{"Bcc",(char *)&RightArrayBM,(xw_handler)pen__color_step,1,&CommandV},
     {"R+",PrevName,SameName,StickSpace,SameSpace}},
  {{"cc",NoLabel,(xw_handler)pen__red_sel,NoValue,&LabelV},
     {"RSEL",PrevName,SameName,FitSpace,SameSpace,BARX,MAXH}},

  {{"Bcc",(char *)&LeftArrayBM,(xw_handler)pen__color_step,-2,&CommandV},
     {"G-","CR",PrevName,FitSpace,FitSpace}},
  {{"cc","G:255",NoHandler,NoValue,&LabelV},
     {"G0",PrevName,SameName,StickSpace,SameSpace,0,MAXH}},
  {{"Bcc",(char *)&RightArrayBM,(xw_handler)pen__color_step,2,&CommandV},
     {"G+",PrevName,SameName,SameSpace,SameSpace}},
  {{"cc",NoLabel,(xw_handler)pen__green_sel,NoValue,&LabelV},
     {"GSEL",PrevName,SameName,FitSpace,SameSpace,BARX,MAXH}},

  {{"Bcc",(char *)&LeftArrayBM,(xw_handler)pen__color_step,-3,&CommandV},
     {"B-","CR",PrevName,FitSpace,FitSpace}},
  {{"cc","B:255",NoHandler,NoValue,&LabelV},
     {"B0",PrevName,SameName,StickSpace,SameSpace,0,MAXH}},
  {{"Bcc",(char *)&RightArrayBM,(xw_handler)pen__color_step,3,&CommandV},
     {"B+",PrevName,SameName,SameSpace,SameSpace}},
  {{"cc",NoLabel,(xw_handler)pen__blue_sel,NoValue,&LabelV},
     {"BSEL",PrevName,SameName,FitSpace,SameSpace,BARX,MAXH}},

  {{"Bcc",(char *)&CloseBM,(xw_handler)pen__close,NoValue,&CommandV},
     {"close","sub","l1",FitSpace,FitSpace}},

#define LPAL 32
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,0,&LabelV},
     {"p00","false","gc-",FitSpace+5,FitSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,1,&LabelV},
     {"p01",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,2,&LabelV},
     {"p02",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,3,&LabelV},
     {"p03",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,4,&LabelV},
     {"p04",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,5,&LabelV},
     {"p05",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,6,&LabelV},
     {"p06",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,7,&LabelV},
     {"p07",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"Bcc",(char *)&UpArrayBM,(xw_handler)pen__palet_step,-1,&CommandV},
     {"pal-",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},

#define PALSC (2*MAXH+1)
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,8,&LabelV},
     {"p08","false",PrevName,FitSpace+5,StickSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,9,&LabelV},
     {"p09",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,10,&LabelV},
     {"p10",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,11,&LabelV},
     {"p11",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,12,&LabelV},
     {"p12",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,13,&LabelV},
     {"p13",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,14,&LabelV},
     {"p14",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,15,&LabelV},
     {"p15",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"Bcc",NoLabel,(xw_handler)pen__palet_step,0,&LabelV},
     {"pal0",PrevName,SameName,StickSpace,SameSpace,MAXH,PALSC}},

  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,16,&LabelV},
     {"p16","false","p15",FitSpace+5,StickSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,17,&LabelV},
     {"p17",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,18,&LabelV},
     {"p18",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,19,&LabelV},
     {"p19",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,20,&LabelV},
     {"p20",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,21,&LabelV},
     {"p21",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,22,&LabelV},
     {"p22",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,23,&LabelV},
     {"p23",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},

  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,24,&LabelV},
     {"p24","false",PrevName,FitSpace+5,StickSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,25,&LabelV},
     {"p25",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,26,&LabelV},
     {"p26",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,27,&LabelV},
     {"p27",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,28,&LabelV},
     {"p28",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,29,&LabelV},
     {"p29",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,30,&LabelV},
     {"p30",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"cc",NoLabel,(xw_handler)pen__palet_set_put,31,&LabelV},
     {"p31",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}},
  {{"Bcc",(char *)&DownArrayBM,(xw_handler)pen__palet_step,1,&CommandV},
     {"pal+",PrevName,SameName,StickSpace,SameSpace,MAXH,MAXH}}
};



static xw_event event_list[] = {
  {Expose,(xw_handler)pen__expose}
};



static xw_event key_list[] = {
  {'r',(xw_handler)pen__expose}
};




Edit ed;
static image bar_gray,bar_red,bar_green,bar_blue;
static image ch1_g,ch1_c;
static image ch2_g,ch2_c;
static image sam_gr,sam_br,sam_cr;
static image gra_g1,gra_g,gra_g2;
static image gra_c1,gra_c,gra_c2;
static image palet_image;



static uchar3 BlackColor = {0,0,0};
static uchar3 WhiteColor = {255,255,255};

void pen__open(Edit e)
{
  e->pen.menu = XwObject.create(e->world,XwHard.class,0,"PenMenu");
  XwObject.panel(e->pen.menu,XwNumber(panel_list),panel_list);
  XwObject.addEvents(e->pen.menu,XwNumber(event_list),event_list);
  XwObject.addKeyEvents(e->pen.menu,XwNumber(key_list),key_list);

  ed = e;
  pen__initialize_pen(e);
  pen__initialize_images(e);
  pen__initialize_palet(e);

  pen__set_size(e,1);
  pen__set_shape(e,SHAPE_BOX);
  pen__set_func(e,FUNC_SET);
  pen__set_binary(e,BIN_1);
  pen__set_select(e,0);
  pen__set_negative(e,0);
  pen__set_gradation(e,0);
  pen__set_gradation_direction(e,GRAD_Right);
  pen__set_diffusion_size(e,1,1);
  pen__set_diffusion_strength(e,1);
  pen__set_gray(e,0);
  pen__set_color(e,BlackColor);
  pen__set_reverse(e,0);
  pen__set_select_first_gray(e,0);
  pen__set_select_last_gray(e,255);
  pen__set_select_first_color(e,BlackColor);
  pen__set_select_last_color(e,WhiteColor);
  pen__set_gradation_first_gray(e,0);
  pen__set_gradation_last_gray(e,255);
  pen__set_gradation_first_color(e,BlackColor);
  pen__set_gradation_last_color(e,WhiteColor);
}



long pen__size_step(Edit e,xw_object obj,XEvent *event,long value)
{
  long step = 1;
  long size = e->pen.size;

  switch (event->xbutton.button)
    {
    case Button1: step = 1; break;
    case Button2: step = 5; break;
    case Button3: step = 10; break;
    }

  switch(value)
    {
    case -1: size = step * (size / step - 1); break;
    case  0: size = step; break;
    case  1: size = step * (size / step + 1); break;
    }

  pen__set_size(e,size);

  return 0;
}



long pen__shape_sel(Edit e,xw_object obj,XEvent *event,long value)
{
  pen__set_shape(e,value);

  return 0;
}



long pen__func_sel(Edit e,xw_object obj,XEvent *event,long value)
{
  pen__set_func(e,value);

  return 0;
}



long pen__select_onoff(Edit e,xw_object obj,XEvent *event,long value)
{
  pen__set_select(e,value);

  return 0;
}



long pen__negative_sel(Edit e,xw_object obj,XEvent *event,long value)
{
  pen__set_negative(e,value);

  return 0;
}



long pen__binary_sel(Edit e,xw_object obj,XEvent *event,long value)
{
  pen__set_binary(e,value);
  pen__redraw_binary(e);

  return 0;
}



void pen__set_bar_point(Edit e,xw_object w,long oldx,long newx)
{
  if (Pen__id(e) == 0) return;

  e->world->gc = e->whiteGC;
  UpTriangle.fill(e->pen.menu,w->x+1+BARX*oldx/256-7/2,w->y+1+MAXH,7,6);

  e->world->gc = e->blackGC;
  UpTriangle.fill(e->pen.menu,w->x+1+BARX*newx/256-7/2,w->y+1+MAXH,7,6);
}



void pen__redraw_binary(Edit e)
{
  image__fill(sam_br,(char *)&e->pen.binary);
  xw__child_fill_image(e,"BR",sam_br);
}



void pen__redraw_gray(Edit e)
{
  image__fill(sam_gr,(char *)&e->pen.gray);
  xw__child_fill_image(e,"GR",sam_gr);
}



void pen__redraw_color(Edit e)
{
  image__fill(sam_cr,(char *)&e->pen.color);
  xw__child_fill_image(e,"CR",sam_cr);
}



void pen__set_red(Edit e,long x)
{
  xw_object bar;

  if (x < 0) x = 0;
  if (x > 255) x = 255;

  bar = XwObject.search(e->pen.menu,"RSEL");
  pen__set_bar_point(e,bar,e->pen.color.at[0],x);
  e->pen.color.at[0] = x;
  XwObject.child.setLabel(e->pen.menu,"R0","R:%d",x);
  pen__redraw_color(e);
}



void pen__set_green(Edit e,long x)
{
  xw_object bar;

  if (x < 0) x = 0;
  if (x > 255) x = 255;

  bar = XwObject.search(e->pen.menu,"GSEL");
  pen__set_bar_point(e,bar,e->pen.color.at[1],x);
  e->pen.color.at[1] = x;
  XwObject.child.setLabel(e->pen.menu,"G0","G:%d",x);
  pen__redraw_color(e);
}



void pen__set_blue(Edit e,long x)
{
  xw_object bar;

  if (x < 0) x = 0;
  if (x > 255) x = 255;

  bar = XwObject.search(e->pen.menu,"BSEL");
  pen__set_bar_point(e,bar,e->pen.color.at[2],x);
  e->pen.color.at[2] = x;
  XwObject.child.setLabel(e->pen.menu,"B0","B:%d",x);
  pen__redraw_color(e);
}



long pen__gray_step(Edit e,xw_object obj,XEvent *event,long value)
{
  long step = 1;
  long gray;

  gray = e->pen.gray;

  switch (event->xbutton.button)
    {
    case Button1: step = 1; break;
    case Button2: step = 5; break;
    case Button3: step = 10; break;
    }

  switch(value)
    {
    case -1: gray = step * (gray / step - 1);  break;
    case  0: gray = step; break;
    case  1: gray = step * (gray / step + 1); break;
    }

  pen__set_gray(e,gray);

  return 0;
}



long pen__color_step(Edit e,xw_object obj,XEvent *event,long value)
{
  long step = 1;
  long r,g,b;
  long c;

  c = abs(value) - 1;

  r = e->pen.color.at[0];
  g = e->pen.color.at[1];
  b = e->pen.color.at[2];

  switch (event->xbutton.button)
    {
    case Button1: step = 1; break;
    case Button2: step = 5; break;
    case Button3: step = 10; break;
    }

  switch(value)
    {
    case -1: r = step * (r / step - 1); break;
    case -2: g = step * (g / step - 1); break;
    case -3: b = step * (b / step - 1); break;
    case  1: r = step * (r / step + 1); break;
    case  2: g = step * (g / step + 1); break;
    case  3: b = step * (b / step + 1); break;
    }

  pen__set_rgb(e,r,g,b);

  return 0;
}



long pen__gray_sel(Edit e,xw_object obj,XEvent *event,long value)
{
  pen__set_gray(e,255 * event->xbutton.x / (BARX - 1));
  return 0;
}



long pen__red_sel(Edit e,xw_object obj,XEvent *event,long value)
{
  long r = 255 * event->xbutton.x / (BARX - 1);
  long g = e->pen.color.at[1];
  long b = e->pen.color.at[2];
  pen__set_rgb(e,r,g,b);

  return 0;
}



long pen__green_sel(Edit e,xw_object obj,XEvent *event,long value)
{
  long r = e->pen.color.at[0];
  long g = 255 * event->xbutton.x / (BARX - 1);
  long b = e->pen.color.at[2];
  pen__set_rgb(e,r,g,b);

  return 0;
}



long pen__blue_sel(Edit e,xw_object obj,XEvent *event,long value)
{
  long r = e->pen.color.at[0];
  long g = e->pen.color.at[1];
  long b = 255 * event->xbutton.x / (BARX - 1);
  pen__set_rgb(e,r,g,b);

  return 0;
}



long pen__gradation_gray_sel(Edit e,xw_object obj,XEvent *event,long value)
{
  long v = (e->pen.sh2.gray - e->pen.sh1.gray) * event->xbutton.x
    / (GRA_X - 1) + e->pen.sh1.gray;
  switch(event->xbutton.button)
    {
    case Button1:
    case Button2:
      XwObject.setLabel(e->message,"gradation -> (gray = %d)\n",v);
      break;
    case Button3:
      pen__set_gray(e,v);
      break;
    }
  return 0;
}



long pen__gradation_color_sel(Edit e,xw_object obj,XEvent *event,long value)
{
  long v0,v1,v2;
  uchar3 p1,p2;

  p1 = e->pen.sh1.color;
  p2 = e->pen.sh2.color;
  v0 = (p2.at[0] - p1.at[0]) * event->xbutton.x / (GRA_X - 1) + p1.at[0];
  v1 = (p2.at[1] - p1.at[1]) * event->xbutton.x / (GRA_X - 1) + p1.at[1];
  v2 = (p2.at[2] - p1.at[2]) * event->xbutton.x / (GRA_X - 1) + p1.at[2];
  switch(event->xbutton.button)
    {
    case Button1:
    case Button2:
      XwObject.setLabel(e->message,"gradation -> (r=%d,g=%d,b=%d)",v0,v1,v2);
      break;
    case Button3:
      pen__set_rgb(e,v0,v1,v2);
    }
  return 0;
}



long pen__percent_step(Edit e,xw_object obj,XEvent *event,long value)
{
  long step = 1;
  long dots = e->pen.dots;

  switch (event->xbutton.button)
    {
    case Button1: step = 1;  break;
    case Button2: step = 5;  break;
    case Button3: step = 10; break;
    }

  switch (value)
    {
    case -1: dots = step * (dots / step - 1); break;
    case  0:
      switch (event->xbutton.button)
	{
	case Button1:dots = 1;break;
	case Button2:dots = 50;break;
	case Button3:dots = 100;break;
	}
      break;
    case  1: dots = step * (dots / step + 1); break;
    }

  pen__set_percent(e,dots);

  return 0;
}



void xw__fill_image(Edit e,xw_object w,image img)
{
  if (Pen__id(e) == 0) return;
  XShowImagePartOfWindow(Edit__display(e),Pen__id(e),Edit__gc(e),
			 img,w->x+1,w->y+1,w->width,w->height);
}



void xw__child_fill_image(Edit e,char *name,image img)
{
  xw_object w;

  if (Pen__id(e) == 0) return;
  w = XwObject.search(e->pen.menu,name);
  XShowImagePartOfWindow(Edit__display(e),Pen__id(e),Edit__gc(e),
			 img,w->x+1,w->y+1,w->width,w->height);
}



void pen__redraw_gray_bar(Edit e)
{
  xw_object w = XwObject.search(e->pen.menu,"graysel");
  xw__fill_image(e,w,bar_gray);
  pen__set_bar_point(e,w,e->pen.gray,e->pen.gray);
}



void pen__redraw_color_bar(Edit e)
{
  xw_object r = XwObject.search(e->pen.menu,"RSEL");
  xw_object g = XwObject.search(e->pen.menu,"GSEL");
  xw_object b = XwObject.search(e->pen.menu,"BSEL");

  xw__fill_image(e,r,bar_red);
  xw__fill_image(e,g,bar_green);
  xw__fill_image(e,b,bar_blue);

  pen__set_bar_point(e,r,e->pen.color.at[0],e->pen.color.at[0]);
  pen__set_bar_point(e,g,e->pen.color.at[1],e->pen.color.at[1]);
  pen__set_bar_point(e,b,e->pen.color.at[2],e->pen.color.at[2]);
}



void pen__redraw_select_gray(Edit e)
{
  xw__child_fill_image(e,"src0",ch1_g);
  xw__child_fill_image(e,"src1",ch2_g);
}



void pen__redraw_select_color(Edit e)
{
  xw__child_fill_image(e,"src2",ch1_c);
  xw__child_fill_image(e,"src3",ch2_c);
}



void pen__redraw_gradation_gray(Edit e)
{
  pen__make_gradation_gray(e);
  xw__child_fill_image(e,"gg0",gra_g);
  xw__child_fill_image(e,"gg+",gra_g1);
  xw__child_fill_image(e,"gg-",gra_g2);
}



void pen__redraw_gradation_color(Edit e)
{
  pen__make_gradation_color(e);
  xw__child_fill_image(e,"gc0",gra_c);
  xw__child_fill_image(e,"gc+",gra_c1);
  xw__child_fill_image(e,"gc-",gra_c2);
}



long pen__expose(Edit e,xw_object obj,XEvent *event,long value)
{
  XwObject.map(obj);

  pen__redraw_gray(e);
  pen__redraw_gray_bar(e);
  pen__redraw_color_bar(e);
  pen__redraw_color(e);

  xw__child_fill_image(e,"BR",sam_br);
  xw__child_fill_image(e,"GR",sam_gr);
  xw__child_fill_image(e,"CR",sam_cr);

  xw__child_fill_image(e,"src0",ch1_g);
  xw__child_fill_image(e,"src1",ch2_g);
  xw__child_fill_image(e,"src2",ch1_c);
  xw__child_fill_image(e,"src3",ch2_c);

  pen__redraw_gradation_gray(e);
  pen__redraw_gradation_color(e);

  pen__redraw_palet(e);

  return 0;
}



long pen__close(Edit e,xw_object obj)
{
  XwObject.unmap(e->pen.menu);
  return 0;
}



long pen__change_select(Edit e,xw_object obj,XEvent *event,long value)
{
  switch(value)
    {
    case GCH1:
      pen__set_select_first_gray(e,e->pen.gray);
      break;
    case GCH2:
      pen__set_select_last_gray(e,e->pen.gray);
      break;
    case CCH1:
      pen__set_select_first_color(e,e->pen.color);
      break;
    case CCH2:
      pen__set_select_last_color(e,e->pen.color);
      break;
    }

  return 0;
}



long pen__gradation_onoff(Edit e,xw_object obj,XEvent *event,long value)
{
  pen__set_gradation(e,value);

  return 0;
}



void pen__make_gradation_gray(Edit e)
{
  long i,n;
  uchar *data;
  long start,end;

  n     = Image.xsize(gra_g);
  data  = (uchar *)Image.raster(gra_g);
  start = e->pen.sh1.gray;
  end   = e->pen.sh2.gray;

  for (i = 0; i < n; i++) data[i] = (end - start) * i / (n - 1) + start;
}



void pen__make_gradation_color(Edit e)
{
  long i,n;
  uchar3 *data;
  long startR,endR;
  long startG,endG;
  long startB,endB;

  n      = Image.xsize(gra_c);
  data   = (uchar3 *)Image.raster(gra_c);
  startR = e->pen.sh1.color.at[0];
  endR   = e->pen.sh2.color.at[0];
  startG = e->pen.sh1.color.at[1];
  endG   = e->pen.sh2.color.at[1];
  startB = e->pen.sh1.color.at[2];
  endB   = e->pen.sh2.color.at[2];

  for (i = 0; i < n; i++)
    {
      data[i].at[0] = (endR - startR) * i / (n - 1) + startR;
      data[i].at[1] = (endG - startG) * i / (n - 1) + startG;
      data[i].at[2] = (endB - startB) * i / (n - 1) + startB;
    }
}



long pen__change_gradation(Edit e,xw_object obj,XEvent *event,long value)
{
  if (event->xbutton.button != Button1) return 0;

  switch (value)
    {
    case GRAY_P:
      pen__set_gradation_first_gray(e,e->pen.gray);
      break;
    case GRAY_S:
      break;
    case GRAY_M:
      pen__set_gradation_last_gray(e,e->pen.gray);
      break;
    case COLOR_P:
      pen__set_gradation_first_color(e,e->pen.color);
      break;
    case COLOR_S:
      break;
    case COLOR_M:
      pen__set_gradation_last_color(e,e->pen.color);
      break;
    }
  return 0;
}



long pen__gradation_direction_sel(Edit e,xw_object obj,XEvent *event,long value)
{
  pen__set_gradation_direction(e,value);

  return 0;
}



void pen__set_palet(Edit e,xw_object obj,long i_offset,uchar3 color)
{
  long i = e->palet_offset + i_offset;

  e->palet[i] = color;
  image__fill(palet_image,(char *)&color);
  xw__fill_image(e,obj,palet_image);
}



void pen__print_palet(Edit e,long i_offset)
{
  long i;

  i = e->palet_offset + i_offset;
  XwObject.setLabel(e->message," palet#%d -> (r=%d,g=%d,b=%d)",i,
		    e->palet[i].at[0],e->palet[i].at[1],e->palet[i].at[2]);
}



void pen__put_palet(Edit e,long i_offset)
{
  long i;

  i = e->palet_offset + i_offset;
  pen__set_color(e,e->palet[i]);
}



long pen__palet_set_put(Edit e,xw_object obj,XEvent *event,long i_offset)
{
  switch (event->xbutton.button)
    {
    case Button3:
      pen__set_palet(e,obj,i_offset,e->pen.color);
      break;

    case Button2:
      pen__print_palet(e,i_offset);
      break;

    case Button1:
      pen__put_palet(e,i_offset);
      break;
    }

  return 0;
}



long pen__palet_step(Edit e,xw_object obj,XEvent *event,long value)
{
  long step = 1;
  long offset = e->palet_offset;

  switch(event->xbutton.button)
    {
    case Button1:step = 1; break;
    case Button2:step = LPAL/4;break;
    case Button3:step = LPAL;break;
    }

  switch (value)
    {
    case -1:offset -= step;break;
    case  0:offset = (256 - LPAL) * event->xbutton.y / PALSC;
    case  1:offset += step;break;
    }

  pen__set_palet_offset(e,offset);

  return 0;
}



void pen__set_palet_offset(Edit e,long offset)
{
  if (offset < 0) offset = 0;
  if (offset > (256 - LPAL)) offset = 256 - LPAL;
  e->palet_offset = offset;

  pen__redraw_palet(e);
}



void pen__redraw_palet(Edit e)
{
  xw_object obj;
  char buf[100];
  long i,y;

  if (Pen__id(e) == 0) return;

  for (i = 0; i < LPAL; i++)
    {
      image__fill(palet_image,(char *)&e->palet[e->palet_offset+i]);
      sprintf(buf,"p%02d",i);
      xw__child_fill_image(e,buf,palet_image);
    }

  y = PALSC * e->palet_offset / (256 - LPAL+1);
  obj = XwObject.search(e->pen.menu,"pal0");
  XwObject.map(obj);
  XDrawLine(Edit__display(e),Pen__id(e),Edit__gc(e),
	    obj->x+1,obj->y+y+1,obj->x+MAXH,obj->y+y+1);
}



void pen__palet_load(Edit e)
{
  char *filename;
  FILE *fp;
  char buf[256];
  int i,n;
  char *head[100];

  filename = e->palet_load;
  fp = fopen(filename,"r");
  if (fp == 0)
    {
      fprintf(stderr,"can't open palet file(%s).\n",filename);
      return;
    }

  i = 0;
  while (fgets(buf,256,fp) && (i < 256))
    {
      n = strsplit(buf,head," \n,");
      if (n < 3) continue;
      e->palet[i].at[0] = atol(head[0]);
      e->palet[i].at[1] = atol(head[1]);
      e->palet[i].at[2] = atol(head[2]);
      i++;
    }
}



void pen__palet_save(Edit e)
{
  char *filename;
  FILE *fp;
  int i;

  filename = e->palet_save;
  fp = fopen(filename,"w");
  if (fp == 0)
    {
      fprintf(stderr,"can't open palet file(%s).\n",filename);
      return;
    }

  for (i = 0; i < 256; i++)
    {
      fprintf(fp,"%3d %3d %3d\n",
	      e->palet[i].at[0],e->palet[i].at[1],e->palet[i].at[2]);
    }
}



long pen__diffusion_size_step(Edit e,xw_object obj,XEvent *event,long value)
{
  long step = 1;
  long xsize,ysize;

  switch (event->xbutton.button)
    {
    case Button1: step = 1; break;
    case Button2: step = 2; break;
    case Button3: step = 5; break;
    }

  xsize = e->pen.diff_xsize;
  ysize = e->pen.diff_ysize;
  switch(value)
    {
    case DXS_P: xsize = step * (xsize / step + 1); break;
    case DXS_O: xsize = step; break;
    case DXS_M: xsize = step * (xsize / step - 1); break;

    case DYS_P: ysize = step * (ysize / step + 1); break;
    case DYS_O: ysize = step; break;
    case DYS_M: ysize = step * (ysize / step - 1); break;
    }

  pen__set_diffusion_size(e,xsize,ysize);

  return 0;
}



long pen__diffusion_strength_step(Edit e,xw_object obj,XEvent *event,long value)
{
  long step = 1;
  long strength = e->pen.diff_strength;

  switch (event->xbutton.button)
    {
    case Button1: step = 1; break;
    case Button2: step = 2; break;
    case Button3: step = 5; break;
    }

  switch (value)
    {
    case DS_M: strength = step * (strength / step - 1); break;
    case DS_O: strength = step; break;
    case DS_P: strength = step * (strength / step + 1); break;
    }

  pen__set_diffusion_strength(e,strength);

  return 0;
}



static uchar3 StandardPalet[256] = {
{  0,   0,   0},{  0,  36,   0},{  0,  72,   0},{  0, 109,   0},
{  0, 145,   0},{  0, 182,   0},{  0, 218,   0},{  0, 255,   0},
{  0,   0,  85},{  0,  36,  85},{  0,  72,  85},{  0, 109,  85},
{  0, 145,  85},{  0, 182,  85},{  0, 218,  85},{  0, 255,  85},
{  0,   0, 170},{  0,  36, 170},{  0,  72, 170},{  0, 109, 170},
{  0, 145, 170},{  0, 182, 170},{  0, 218, 170},{  0, 255, 170},
{  0,   0, 255},{  0,  36, 255},{  0,  72, 255},{  0, 109, 255},
{  0, 145, 255},{  0, 182, 255},{  0, 218, 255},{  0, 255, 255},
{ 36,   0,   0},{ 36,  36,   0},{ 36,  72,   0},{ 36, 109,   0},
{ 36, 145,   0},{ 36, 182,   0},{ 36, 218,   0},{ 36, 255,   0},
{ 36,   0,  85},{ 36,  36,  85},{ 36,  72,  85},{ 36, 109,  85},
{ 36, 145,  85},{ 36, 182,  85},{ 36, 218,  85},{ 36, 255,  85},
{ 36,   0, 170},{ 36,  36, 170},{ 36,  72, 170},{ 36, 109, 170},
{ 36, 145, 170},{ 36, 182, 170},{ 36, 218, 170},{ 36, 255, 170},
{ 36,   0, 255},{ 36,  36, 255},{ 36,  72, 255},{ 36, 109, 255},
{ 36, 145, 255},{ 36, 182, 255},{ 36, 218, 255},{ 36, 255, 255},
{ 72,   0,   0},{ 72,  36,   0},{ 72,  72,   0},{ 72, 109,   0},
{ 72, 145,   0},{ 72, 182,   0},{ 72, 218,   0},{ 72, 255,   0},
{ 72,   0,  85},{ 72,  36,  85},{ 72,  72,  85},{ 72, 109,  85},
{ 72, 145,  85},{ 72, 182,  85},{ 72, 218,  85},{ 72, 255,  85},
{ 72,   0, 170},{ 72,  36, 170},{ 72,  72, 170},{ 72, 109, 170},
{ 72, 145, 170},{ 72, 182, 170},{ 72, 218, 170},{ 72, 255, 170},
{ 72,   0, 255},{ 72,  36, 255},{ 72,  72, 255},{ 72, 109, 255},
{ 72, 145, 255},{ 72, 182, 255},{ 72, 218, 255},{ 72, 255, 255},
{109,   0,   0},{109,  36,   0},{109,  72,   0},{109, 109,   0},
{109, 145,   0},{109, 182,   0},{109, 218,   0},{109, 255,   0},
{109,   0,  85},{109,  36,  85},{109,  72,  85},{109, 109,  85},
{109, 145,  85},{109, 182,  85},{109, 218,  85},{109, 255,  85},
{109,   0, 170},{109,  36, 170},{109,  72, 170},{109, 109, 170},
{109, 145, 170},{109, 182, 170},{109, 218, 170},{109, 255, 170},
{109,   0, 255},{109,  36, 255},{109,  72, 255},{109, 109, 255},
{109, 145, 255},{109, 182, 255},{109, 218, 255},{109, 255, 255},
{145,   0,   0},{145,  36,   0},{145,  72,   0},{145, 109,   0},
{145, 145,   0},{145, 182,   0},{145, 218,   0},{145, 255,   0},
{145,   0,  85},{145,  36,  85},{145,  72,  85},{145, 109,  85},
{145, 145,  85},{145, 182,  85},{145, 218,  85},{145, 255,  85},
{145,   0, 170},{145,  36, 170},{145,  72, 170},{145, 109, 170},
{145, 145, 170},{145, 182, 170},{145, 218, 170},{145, 255, 170},
{145,   0, 255},{145,  36, 255},{145,  72, 255},{145, 109, 255},
{145, 145, 255},{145, 182, 255},{145, 218, 255},{145, 255, 255},
{182,   0,   0},{182,  36,   0},{182,  72,   0},{182, 109,   0},
{182, 145,   0},{182, 182,   0},{182, 218,   0},{182, 255,   0},
{182,   0,  85},{182,  36,  85},{182,  72,  85},{182, 109,  85},
{182, 145,  85},{182, 182,  85},{182, 218,  85},{182, 255,  85},
{182,   0, 170},{182,  36, 170},{182,  72, 170},{182, 109, 170},
{182, 145, 170},{182, 182, 170},{182, 218, 170},{182, 255, 170},
{182,   0, 255},{182,  36, 255},{182,  72, 255},{182, 109, 255},
{182, 145, 255},{182, 182, 255},{182, 218, 255},{182, 255, 255},
{218,   0,   0},{218,  36,   0},{218,  72,   0},{218, 109,   0},
{218, 145,   0},{218, 182,   0},{218, 218,   0},{218, 255,   0},
{218,   0,  85},{218,  36,  85},{218,  72,  85},{218, 109,  85},
{218, 145,  85},{218, 182,  85},{218, 218,  85},{218, 255,  85},
{218,   0, 170},{218,  36, 170},{218,  72, 170},{218, 109, 170},
{218, 145, 170},{218, 182, 170},{218, 218, 170},{218, 255, 170},
{218,   0, 255},{218,  36, 255},{218,  72, 255},{218, 109, 255},
{218, 145, 255},{218, 182, 255},{218, 218, 255},{218, 255, 255},
{255,   0,   0},{255,  36,   0},{255,  72,   0},{255, 109,   0},
{255, 145,   0},{255, 182,   0},{255, 218,   0},{255, 255,   0},
{255,   0,  85},{255,  36,  85},{255,  72,  85},{255, 109,  85},
{255, 145,  85},{255, 182,  85},{255, 218,  85},{255, 255,  85},
{255,   0, 170},{255,  36, 170},{255,  72, 170},{255, 109, 170},
{255, 145, 170},{255, 182, 170},{255, 218, 170},{255, 255, 170},
{255,   0, 255},{255,  36, 255},{255,  72, 255},{255, 109, 255},
{255, 145, 255},{255, 182, 255},{255, 218, 255},{255, 255, 255}
};



void pen__initialize_palet(Edit e)
{
  int i;

  e->palet_offset = 0;

  for (i = 0; i < 256; i++)
    e->palet[i] = StandardPalet[i];

  if (optspecified("palet-load")) pen__palet_load(e);
}



void pen__initialize_pen(Edit e)
{
  e->pen.pen = CGPen.create("pen",Edit__image_type(e));
  CGPen.setClip(e->pen.pen,0,0,Edit__image_xsize(e),Edit__image_ysize(e));
  CGPen.setFunction(e->pen.pen,(cg_function)pen__draw);
  e->pen.bit1_pen = CGPen.create("Bit1Pen",Bit1);
  CGPen.setFunction(e->pen.bit1_pen,(cg_function)pen__bit1_draw);

  e->pen.pat  = Image.create("PenPattern");
  pen__make(e);
  e->pen.fill = Image.createMake("PenFillPattern",Bit1,1,1);
  Image.raster(e->pen.fill)[0] = 1;
  e->pen.dots = 100;
/* srandom(e->pen.fill); */
  srand(e->pen.fill);
}



void pen__initialize_images(Edit e)
{
  int i;
  uchar *m;
  uchar3 *r,*g,*b;

  bar_gray  = Image.createMake("bar_gray",UChar,BARX,1);
  bar_red   = Image.createMake("bar_red",UChar3,BARX,1);
  bar_green = Image.createMake("bar_green",UChar3,BARX,1);
  bar_blue  = Image.createMake("bar_blue",UChar3,BARX,1);
  sam_br    = Image.createMake("sam_br",Bit1,1,1);
  sam_gr    = Image.createMake("sam_gr",UChar,1,1);
  sam_cr    = Image.createMake("sam_cr",UChar3,1,1);
  palet_image = Image.createMake("palet",UChar3,1,1);
  ch1_g = Image.createMake("ch1_g",UChar,1,1);
  ch1_c = Image.createMake("ch1_c",UChar3,1,1);
  ch2_g = Image.createMake("ch2_g",UChar,1,1);
  ch2_c = Image.createMake("ch2_c",UChar3,1,1);
  gra_g1 = Image.createMake("gra_g1",UChar,1,1);
  gra_g2 = Image.createMake("gra_g2",UChar,1,1);
  gra_g  = Image.createMake("gra_g", UChar,GRA_X,1);
  gra_c1 = Image.createMake("gra_c1",UChar3,1,1);
  gra_c2 = Image.createMake("gra_c2",UChar3,1,1);
  gra_c  = Image.createMake("gra_c", UChar3,GRA_X,1);

  m = (uchar  *)Image.raster(bar_gray);
  r = (uchar3 *)Image.raster(bar_red);
  g = (uchar3 *)Image.raster(bar_green);
  b = (uchar3 *)Image.raster(bar_blue);
  for (i = 0; i < BARX; i++)
    m[i] = r[i].at[0] = g[i].at[1] = b[i].at[2] = 255 * i / (BARX - 1);

  image__fill(sam_br,(char *)&e->pen.binary);
  image__fill(sam_gr,(char *)&e->pen.gray);
  image__fill(sam_cr,(char *)&e->pen.color);
  image__fill(ch1_g,(char *)&e->pen.ch1.gray);
  image__fill(ch1_c,(char *)&e->pen.ch1.color);
  image__fill(ch2_g,(char *)&e->pen.ch2.gray);
  image__fill(ch2_c,(char *)&e->pen.ch2.color);
  image__reverse(gra_g2,gra_g2);
  image__reverse(gra_c2,gra_c2);
}



void pen__set_size(Edit e,long size)
{
  if (size <= 0) size = 1;
  e->pen.size = size;
  XwObject.child.setLabel(e->pen.menu,"p0","%d",size);

  pen__make(e);
}


void pen__set_shape(Edit e,long shape)
{
  static char *name[] = {"box","circle"};

  XwObject.child.setState(e->pen.menu,name[e->pen.shape],XW_FRAME);
  e->pen.shape = shape;
  XwObject.child.setState(e->pen.menu,name[e->pen.shape],XW_REVERSE);

  pen__make(e);
}



void pen__set_func(Edit e,long func)
{
  static char *name[] = {"set","max","min","mean",
			    "not","or","and","xor","add","sub","diff"};
  XwObject.child.setState(e->pen.menu,name[e->pen.func],XW_FRAME);
  e->pen.func = func;
  XwObject.child.setState(e->pen.menu,name[e->pen.func],XW_REVERSE);
}



void pen__set_binary(Edit e,long binary)
{
  static char *name[] = {"false","true"};

  XwObject.child.setState(e->pen.menu,name[e->pen.binary],XW_FRAME);
  e->pen.binary = binary;
  XwObject.child.setState(e->pen.menu,name[e->pen.binary],XW_REVERSE);
}



void pen__set_select(Edit e,long select)
{
  static char *name[] = {"soff","son"};

  XwObject.child.setState(e->pen.menu,name[e->pen.select],XW_FRAME);
  e->pen.select = select;
  XwObject.child.setState(e->pen.menu,name[e->pen.select],XW_REVERSE);
}



void pen__set_negative(Edit e,long negative)
{
  static char *name[] = {"non","noff"};

  XwObject.child.setState(e->pen.menu,name[e->pen.negative],XW_FRAME);
  e->pen.negative = negative;
  XwObject.child.setState(e->pen.menu,name[e->pen.negative],XW_REVERSE);
}



void pen__set_gradation(Edit e,long gradation)
{
  static char *name[] = {"goff","gon"};

  XwObject.child.setState(e->pen.menu,name[e->pen.gradation],XW_FRAME);
  e->pen.gradation = gradation;
  XwObject.child.setState(e->pen.menu,name[e->pen.gradation],XW_REVERSE);
}



void pen__set_gradation_direction(Edit e,long dir)
{
  static char *name[] = {"glu","gld","gru","grd","gl","gr","gu","gd","gc"};

  if ((dir < GRAD_Left) || (dir > GRAD_Down)) return;

  XwObject.child.setState(e->pen.menu,name[e->pen.grad_dir],XW_FRAME);
  e->pen.grad_dir = dir;
  XwObject.child.setState(e->pen.menu,name[e->pen.grad_dir],XW_REVERSE);
}



void pen__set_diffusion_size(Edit e,long xsize,long ysize)
{
  if (xsize <= 0) xsize = 1;
  if (ysize <= 0) ysize = 1;

  if (xsize != e->pen.diff_xsize)
    XwObject.child.setLabel(e->pen.menu,"dx0","X:%d",xsize);
  if (ysize != e->pen.diff_ysize)
    XwObject.child.setLabel(e->pen.menu,"dy0","Y:%d",ysize);

  e->pen.diff_xsize = xsize;
  e->pen.diff_ysize = ysize;
}



void pen__set_diffusion_strength(Edit e,long strength)
{
  if (strength <= 0) strength = 1;

  e->pen.diff_strength = strength;
  XwObject.child.setLabel(e->pen.menu,"ds0","%d",strength);
}



void pen__set_gray(Edit e,long gray)
{
  xw_object bar;

  if (gray < 0) gray = 0;
  if (gray > 255) gray = 255;

  bar = XwObject.search(e->pen.menu,"graysel");
  pen__set_bar_point(e,bar,e->pen.gray,gray);
  e->pen.gray = gray;
  XwObject.child.setLabel(e->pen.menu,"g0","M:%d",gray);

  pen__redraw_gray(e);
}



void pen__set_rgb(Edit e,long r,long g,long b)
{
  if (r != e->pen.color.at[0]) pen__set_red(e,r);
  if (g != e->pen.color.at[1]) pen__set_green(e,g);
  if (b != e->pen.color.at[2]) pen__set_blue(e,b);
  XwObject.child.setLabel(e->pen.menu,"R0","R:%d",r);
  XwObject.child.setLabel(e->pen.menu,"G0","G:%d",g);
  XwObject.child.setLabel(e->pen.menu,"B0","B:%d",b);
}



void pen__set_color(Edit e,uchar3 color)
{
  if (color.at[0] != e->pen.color.at[0]) pen__set_red(e,color.at[0]);
  if (color.at[1] != e->pen.color.at[1]) pen__set_green(e,color.at[1]);
  if (color.at[2] != e->pen.color.at[2]) pen__set_blue(e,color.at[2]);
  XwObject.child.setLabel(e->pen.menu,"R0","R:%d",color.at[0]);
  XwObject.child.setLabel(e->pen.menu,"G0","G:%d",color.at[1]);
  XwObject.child.setLabel(e->pen.menu,"B0","B:%d",color.at[2]);
}



void pen__set_reverse(Edit e,long reverse)
{
  e->pen.reverse = reverse;
}



void pen__set_percent(Edit e,long dots)
{
  if (dots <= 0) dots = 1;
  if (dots >= 100) dots = 100;
  e->pen.dots = dots;
  XwObject.child.setLabel(e->pen.menu,"pers","%d%%",dots);
}



void pen__set_select_first_gray(Edit e,uchar gray)
{
  e->pen.ch1.gray = gray;
  image__fill(ch1_g,(char *)&gray);
  pen__redraw_select_gray(e);
}



void pen__set_select_last_gray(Edit e,uchar gray)
{
  e->pen.ch2.gray = gray;
  image__fill(ch2_g,(char *)&gray);
  pen__redraw_select_gray(e);
}



void pen__set_select_first_color(Edit e,uchar3 color)
{
  e->pen.ch1.color = color;
  image__fill(ch1_c,(char *)&color);
  pen__redraw_select_color(e);
}



void pen__set_select_last_color(Edit e,uchar3 color)
{
  e->pen.ch2.color = color;
  image__fill(ch2_c,(char *)&color);
  pen__redraw_select_color(e);
}



void pen__set_gradation_first_gray(Edit e,uchar gray)
{
  e->pen.sh1.gray = gray;
  image__fill(gra_g1,(char *)&gray);
  pen__redraw_gradation_gray(e);
}



void pen__set_gradation_last_gray(Edit e,uchar gray)
{
  e->pen.sh2.gray = gray;
  image__fill(gra_g2,(char *)&gray);
  pen__redraw_gradation_gray(e);
}



void pen__set_gradation_first_color(Edit e,uchar3 color)
{
  e->pen.sh1.color = color;
  image__fill(gra_c1,(char *)&color);
  pen__redraw_gradation_color(e);
}



void pen__set_gradation_last_color(Edit e,uchar3 color)
{
  e->pen.sh2.color = color;
  image__fill(gra_c2,(char *)&color);
  pen__redraw_gradation_color(e);
}



void pen__get_color(Edit e,long px,long py)
{
  if (Edit__image_type(e) == UChar)
    {
      uchar **data = (uchar **)Image.data(Edit__image(e));
      pen__set_gray(e,data[py][px]);
    }
  else
  if (Edit__image_type(e) == UChar3)
    {
      uchar3 **data = (uchar3 **)Image.data(Edit__image(e));
      pen__set_color(e,data[py][px]);
    }
}
