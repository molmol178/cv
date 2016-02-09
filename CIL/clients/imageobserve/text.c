/*
 * filename : text.c
 * author   : Takahiro Sugiyama
 * date     : Sunday, February 9 1997
 * describe : Text Routines
 */

#include "observe.h"

enum {
  is_bit1, is_char,
  is_long, is_long2, is_long3,
  is_ulong, is_ulong2, is_ulong3,
  is_float, is_float2, is_float3
};

struct print_valueRec {
  long type;
  long type_is;
  char fmt[30];
  char blank[30];
  long n;
  union {
    char  c[3];
    long  l[3];
    ulong u[3];
    float f[3];
  } v;
} print_value_list[] = {
  {Bit1, is_bit1, "%c", " *"},
  {Bit4, is_long, "%2u", " **"},

  {Char, is_char, "[%c:%02x]", " ******"},

  {UChar, is_ulong, "%3u",          " ***"},
  {UChar2, is_ulong2, "[%3u,%3u]",    " *********"},
  {UChar3, is_ulong3, "[%3u,%3u,%3u]", " *************"},

  {Short, is_long, "%5d",          " *****"},
  {Short2, is_long2, "[%4d,%4d]",    " ***********"},
  {Short3, is_long3, "[%6d,%6d,%6d]", " **********************"},

  {UShort, is_ulong, "%5u",          " *****"},
  {UShort2, is_ulong2, "[%6u,%6u]",    " ***************"},
  {UShort3, is_ulong3, "[%6u,%6u,%6u]", " **********************"},

  {Long, is_long, "%5d",          " *****"},
  {Long2, is_long2, "[%6d,%6d]",    " ***************"},
  {Long3, is_long3, "[%6d,%6d,%6d]", " **********************"},

  {ULong, is_ulong, "%6u",          " ******"},
  {ULong2, is_ulong2, "[%6u,%6u]",    " ***************"},
  {ULong3, is_ulong3, "[%6u,%6u,%6u]", " **********************"},

  {Float, is_float, "%7.3f",              " *******"},
  {Float2, is_float2, "[%3.0f,%3.0f]",      " *********"},
  {Float3, is_float3, "[%3.0f,%3.0f,%3.0f]", " *************"}
};



/*------------------------------------------------------------------------*
 * フォントの設定
 *------------------------------------------------------------------------*/
XFontStruct *value_font_struct;
XFontStruct *original_font_struct;
void set_value_font(Display *display, char *fontname)
{
  original_font_struct = Xcil_font_struct;
  value_font_struct = XLoadQueryFont(display, fontname);
  if (value_font_struct == 0)
    {
      fprintf(stderr,"フォント(%s)がロードできませんでした。\n", fontname);
      value_font_struct = Xcil_font_struct;
      return;
    }
}


static struct print_valueRec *set_print_values(type)
     long type;
{
  int i;

  for (i = 0; i < Number(print_value_list); i++)
    if (print_value_list[i].type == type) break;
  if (i == Number(print_value_list)) return 0;

  print_value_list[i].n = 80/(strlen(print_value_list[i].blank)+1);

  return &print_value_list[i];
}



static void let_in_value(pvdata, data, xx, yy)
     struct print_valueRec *pvdata;
     char **data;
     long xx, yy;
{
  if (pvdata->type == Bit1)
    pvdata->v.l[0] = ((bit1 **)data)[yy][xx];

  if (pvdata->type == Bit4)
    pvdata->v.l[0] = ((bit4 **)data)[yy][xx];

  if (pvdata->type == Char)
    pvdata->v.c[0] = ((char **)data)[yy][xx];

  if (pvdata->type == UChar)
    pvdata->v.u[0] = ((uchar **)data)[yy][xx];

  if (pvdata->type == UChar2)
    {
      pvdata->v.u[0] = ((uchar2 **)data)[yy][xx].at[0];
      pvdata->v.u[1] = ((uchar2 **)data)[yy][xx].at[1];
    }

  if (pvdata->type == UChar3)
    {
      pvdata->v.u[0] = ((uchar3 **)data)[yy][xx].at[0];
      pvdata->v.u[1] = ((uchar3 **)data)[yy][xx].at[1];
      pvdata->v.u[2] = ((uchar3 **)data)[yy][xx].at[2];
    }

  if (pvdata->type == Short)
    pvdata->v.l[0] = ((short **)data)[yy][xx];

  if (pvdata->type == Short2)
    {
      pvdata->v.l[0] = ((short2 **)data)[yy][xx].at[0];
      pvdata->v.l[1] = ((short2 **)data)[yy][xx].at[1];
    }

  if (pvdata->type == Short3)
    {
      pvdata->v.l[0] = ((short3 **)data)[yy][xx].at[0];
      pvdata->v.l[1] = ((short3 **)data)[yy][xx].at[1];
      pvdata->v.l[2] = ((short3 **)data)[yy][xx].at[2];
    }

  if (pvdata->type == UShort)
    pvdata->v.u[0] = ((ushort **)data)[yy][xx];

  if (pvdata->type == UShort2)
    {
      pvdata->v.u[0] = ((ushort2 **)data)[yy][xx].at[0];
      pvdata->v.u[1] = ((ushort2 **)data)[yy][xx].at[1];
    }

  if (pvdata->type == UShort3)
    {
      pvdata->v.u[0] = ((ushort3 **)data)[yy][xx].at[0];
      pvdata->v.u[1] = ((ushort3 **)data)[yy][xx].at[1];
      pvdata->v.u[2] = ((ushort3 **)data)[yy][xx].at[2];
    }

  if (pvdata->type == Long)
    pvdata->v.l[0] = ((long **)data)[yy][xx];

  if (pvdata->type == Long2)
    {
      pvdata->v.l[0] = ((long2 **)data)[yy][xx].at[0];
      pvdata->v.l[1] = ((long2 **)data)[yy][xx].at[1];
    }

  if (pvdata->type == Long3)
    {
      pvdata->v.l[0] = ((long3 **)data)[yy][xx].at[0];
      pvdata->v.l[1] = ((long3 **)data)[yy][xx].at[1];
      pvdata->v.l[2] = ((long3 **)data)[yy][xx].at[2];
    }

  if (pvdata->type == ULong)
    pvdata->v.u[0] = ((ulong **)data)[yy][xx];

  if (pvdata->type == ULong2)
    {
      pvdata->v.u[0] = ((ulong2 **)data)[yy][xx].at[0];
      pvdata->v.u[1] = ((ulong2 **)data)[yy][xx].at[1];
    }

  if (pvdata->type == ULong3)
    {
      pvdata->v.u[0] = ((ulong3 **)data)[yy][xx].at[0];
      pvdata->v.u[1] = ((ulong3 **)data)[yy][xx].at[1];
      pvdata->v.u[2] = ((ulong3 **)data)[yy][xx].at[2];
    }

  if (pvdata->type == Float)
    pvdata->v.f[0] = ((float **)data)[yy][xx];

  if (pvdata->type == Float2)
    {
      pvdata->v.f[0] = ((float2 **)data)[yy][xx].at[0];
      pvdata->v.f[1] = ((float2 **)data)[yy][xx].at[1];
    }

  if (pvdata->type == Float3)
    {
      pvdata->v.f[0] = ((float3 **)data)[yy][xx].at[0];
      pvdata->v.f[1] = ((float3 **)data)[yy][xx].at[1];
      pvdata->v.f[2] = ((float3 **)data)[yy][xx].at[2];
    }
}



static void sprint_each_value(buf, pvdata, data, xx, yy)
     char buf[];
     struct print_valueRec *pvdata;
     char **data;
     long xx, yy;
{
  let_in_value(pvdata, data, xx, yy);

  switch(pvdata->type_is)
    {
    case is_char:
      {
	if (isprint(pvdata->v.c[0]))
	  sprintf(buf,pvdata->fmt, pvdata->v.c[0], pvdata->v.c[0]);
	else
	  sprintf(buf,pvdata->fmt, ' ', pvdata->v.c[0]);
	break;
      }

    case is_bit1:
      {
	if (pvdata->v.l[0])
	  sprintf(buf,"1");
	else
	  sprintf(buf,"0");
	break;
      }

    case is_long:
      sprintf(buf,pvdata->fmt, pvdata->v.l[0]);break;
    case is_long2:
      sprintf(buf,pvdata->fmt, pvdata->v.l[0], pvdata->v.l[1]);break;
    case is_long3:
      sprintf(buf,pvdata->fmt, pvdata->v.l[0], pvdata->v.l[1], pvdata->v.l[2]);break;

    case is_ulong:
      sprintf(buf,pvdata->fmt, pvdata->v.u[0]);break;
    case is_ulong2:
      sprintf(buf,pvdata->fmt, pvdata->v.u[0], pvdata->v.u[1]);break;
    case is_ulong3:
      sprintf(buf,pvdata->fmt, pvdata->v.u[0], pvdata->v.u[1], pvdata->v.u[2]);break;

    case is_float:
      sprintf(buf,pvdata->fmt, pvdata->v.f[0]);break;
    case is_float2:
      sprintf(buf,pvdata->fmt, pvdata->v.f[0], pvdata->v.f[1]);break;
    case is_float3:
      sprintf(buf,pvdata->fmt, pvdata->v.f[0], pvdata->v.f[1], pvdata->v.f[2]);break;
    }
}


void text__set_size(ObItemRec *item, Display *display)
{
  struct print_valueRec *val = set_print_values(__TYPE(item->org));

  Xcil_font_struct = value_font_struct;
  XSetFont(display, Xcil_black_gc, Xcil_font_struct->fid);
  item->text_xsize = XcilTextWidth(val->blank);
  item->text_ysize = XcilTextHeight(val->blank);
  Xcil_font_struct = original_font_struct;
  XSetFont(display, Xcil_black_gc, Xcil_font_struct->fid);
}


void item__draw_eye_buffer_text(ObItemRec *item, Display *display,
				int l, int r, int t, int b, int wx0, int wy0)
{
  register int x, y, wx, wy;
  register int text_xsize = item->text_xsize;
  register int text_ysize = item->text_ysize;
  char buf[256];
  struct print_valueRec *val = set_print_values(__TYPE(item->org));

  XFillRectangle(display, item->buffer, Xcil_white_gc, wx0, wy0,
		 (r-l+1)*text_xsize, (b-t+1)*text_ysize);
  Xcil_font_struct = value_font_struct;
  XSetFont(display, Xcil_black_gc, Xcil_font_struct->fid);
  for (wy = wy0, y = t; y <= b; y++, wy += text_ysize)
    for (wx = wx0, x = l; x <= r; x++, wx += text_xsize)
      {
	sprint_each_value(buf, val, item->org->data, x, y);
	XcilDrawStringBox(display, item->buffer, wx, wy, text_xsize, text_ysize,
			  "cc", buf);
      }
  Xcil_font_struct = original_font_struct;
  XSetFont(display, Xcil_black_gc, Xcil_font_struct->fid);
}
