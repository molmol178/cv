/*
 * filename : initialize.c
 * author   : Takahiro Sugiyama
 * date     : from 1993
 * describe : initialize application attributes
 */


#include "Xdisp.h"


char *opt_item[] = {
  "input:*:*:(image) name of the input image",
  "help:-h:print help message",
  "debug:-d:debug mode for shared memory",
  "verbose:-v:verbose mode",
  "display:-D:1:(display) name of the X window's display",
  "key:-k:1:(name) key of the imagedisp",
  "reverse:-r:reverse image",
  "window-size:-ws:2:(int) x,y size of the window",
  "pixel-size:-p:1:1.0:(float) size of the pixel",
  "pixel-xsize:-px:1:1.0:(float) X size of the pixel",
  "pixel-ysize:-py:1:1.0:(float) Y size of the pixel",
  "image-offset:-o:2:0:0:(int) x,y offset of the top of image",
  "image-size:-s:2:(int) x,y size of the view region",

  "*) display gamma correction setting",
  "gamma:-g:1:1.0:(float) gamma of display for correction [#r,#g,#b]",
  "lookup-table:-lut:1:(file) name of lookup table file",

  "*) display property setting",
  "gray-property:-gp:1:(float) maximum of gray level [all,#]",
  "feature-property:-fp:1:(float) maximum of feature level [all,#]",
  "label-property:-lp:label image",
  "color-label-property:-cp:1:(float) maximum of color label level [all,#]",

  "*) For char image.",
  "font:-f:1:g5:(font) name of the font",
  "12>:Gothic .................. g2, g3, g4, g5, g6, g7, g8",
  "12>:Feature ................. f7, f8, f9, 10",
  "12>:Four Directions ......... dir4-3, dir4-5, dir4-7",
  "12>:Eight Directions ........ dir8-5, dir8-7, dir8-9",
  "12>:Sixteen Directions ...... dir16-9",
  "12>:Thirty-Two Directions ... dir32-17",
  "space:-spc:1:1:(int) space of the font",
  "uchar:-u:change Char to UChar",
  "char:-c:change UChar or Bit1 to Char",
  "xyspace:-xys:2:1:1:(int) x,y space of char font",

  "*) other options.",
  "tile:-T:set tile window position",
  "center:-C:set center window position",
};


void initialize(XdispApp app, long argc, char *argv[])
{
  int i;
  char **files;
  char property_env[512];

  OPTION_SETUP(opt_item);

  if (argc == 1) optusage(1);
  if (optspecified("help")) optmanual(1);

  app->debug   = 0;
  app->wx = 0;
  app->wy = 0;
  app->ox = 0;
  app->oy = 0;
  app->rx = 0;
  app->ry = 0;
  app->px = 0;
  app->py = 0;

  app->debug = optspecified("debug");

  if (optspecified("key"))
    {
      char *head[256];
      char keys[256];
      long n;
      strcpy(keys, optvalue("key"));
      n = strsplit(keys, head, " ");
      optaddvalues("input", n, head);
    }

  if (optspecified("gamma"))
    {
      static char envbuf[512];
      sprintf(envbuf,"%s=%s", CIL_GAMMA,optvalue("gamma"));
      putenv(envbuf);
    }

  if (optspecified("lookup-table"))
    {
      if (Read_RGB_LUT(CIL_RGB_LUT, optvalue("lookup-table")) == 0)
	{
	  fprintf(stderr,"cannot open lookup table file\n\t%s ... ignored\n",
		  optvalue("lookup-table"));
	}
      else
	CIL_Setup_RGB_LUT = 1;
    }

  property_env[0] = '\0';
  if (optspecified("gray-property"))
    {
      if (optvaluefloat("gray-property") > 0)
	sprintf(property_env,"gray#%s", optvalue("gray-property"));
      else
	sprintf(property_env,"gray");
    }
  else
  if (optspecified("feature-property"))
    {
      if (optvaluefloat("feature-property") > 0)
	sprintf(property_env,"feature#%s", optvalue("feature-property"));
      else
	sprintf(property_env,"feature");
    }
  else
  if (optspecified("label-property"))
    {
      sprintf(property_env,"label");
    }
  else
  if (optspecified("color-label-property"))
    {
      if (optvaluefloat("color-label-property") > 0)
	sprintf(property_env,"color-label#%s", optvalue("color-label-property"));
      else
	sprintf(property_env,"color-label");
    }
  if (property_env[0])
    {
      static char env[12][512];
      static char *dp_list[] = {
	"CIL_DP_BIT4", "CIL_DP_CHAR", "CIL_DP_SHORT", "CIL_DP_LONG",
	"CIL_DP_LONG", "CIL_DP_UCHAR", "CIL_DP_USHORT", "CIL_DP_ULONG",
	"CIL_DP_ULONG",	"CIL_DP_FLOAT", "CIL_DP_DOUBLE"};
      for (i = 0; i < 11; i++)
	{
	  sprintf(env[i],"%s=%s",dp_list[i],property_env);
	  putenv(env[i]);
	}
    }

  app->num = optvaluenum("input");
  while (app->num == 0)
    {
      optinput("input");
      app->num = optvaluenum("input");
    }
  files = optvaluelist("input");

  if (optspecified("window-size"))
    {
      app->wx = optnvalueint("window-size", 0);
      app->wy = optnvalueint("window-size", 1);
    }

  app->ox = optnvalueint("image-offset", 0);
  app->oy = optnvalueint("image-offset", 1);

  if (optspecified("image-size"))
    {
      app->rx = optnvalueint("image-size", 0);
      app->ry = optnvalueint("image-size", 1);
    }

  app->px = optvaluefloat("pixel-size");
  app->py = optvaluefloat("pixel-size");
  if (optspecified("pixel-xsize"))
    app->px = optvaluefloat("pixel-xsize");
  if (optspecified("pixel-ysize"))
    app->py = optvaluefloat("pixel-ysize");

  if (optspecified("font")) XImageFontSet(optvalue("font"));
  if (optspecified("space"))
    XImageFontSetSpace(optvalueint("space"), optvalueint("space"));
  if (optspecified("xyspace"))
    XImageFontSetSpace(optnvalueint("xyspace", 0),
		        optnvalueint("xyspace", 1));

  if (optspecified("display"))
    {
      app->display = XcilOpenDisplay(optvalue("display"));
      if (app->debug) fprintf(stderr, "SetDisplay = \"%s\"\n",
			      optvalue("display"));
    }
  else
    app->display = XcilOpenDisplay(0);

  app->context = XcilUniqueContext();

  app->list = typenew1(app->num, struct ITEM);
  for (i = 0; i < app->num; i++)
    {
      struct ITEM *item = &app->list[i];
      strcpy(item->name, files[i]);
      strcpy(item->filename, files[i]);
      item->transed = 0;
      item->histflat = 0;
      item->reveresed = 0;
      item->org = Image.create(item->name);
      item->img = Image.create("DISP");
      item->win = 0;
      item->wx = app->wx;
      item->wy = app->wy;
      item->ox = app->ox;
      item->oy = app->oy;
      item->rx = app->rx;
      item->ry = app->ry;
      item->px = app->px;
      item->py = app->py;
    }
}
