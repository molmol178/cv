/*
 * filename : ImageShowing.c
 * author   : Takahiro Sugiyama
 * date     : Tuesday, November 28 1995
 * describe : Translate any image into showable image
 */


#include "Image.h"
#include "misc/memlib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define GREEN_GAIN "CIL_GREEN_GAIN"

/*====================*
 * 特徴のカラーマップ
 *====================*/
static uchar3 *MakeFeatureColormap
  _P1 ((long, label_max))
{
  long max_color;
  uchar3 *cmap;
  long x;

  max_color = 1.5*label_max;

  cmap = typenew1(label_max + 3, uchar3);
  for (x = 0; x <= label_max; x++)
    {
      double theta;
      double r, g, b;

      theta = 2 * M_PI * x / max_color;

      if (theta <= 2*M_PI/3) /* R--G */
	{
	  b = 0;
	  r = cos(theta) + sin(theta) / tan(M_PI/3.0);
	  g = cos(2*M_PI/3.0-theta) + sin(2*M_PI/3.0-theta) / tan(M_PI/3.0);
	}
      else
      if (4*M_PI/3 <= theta) /* R--B */
	{
	  theta -= 4*M_PI/3;
	  g = 0;
	  b = cos(theta) + sin(theta) / tan(M_PI/3.0);
	  r = cos(2*M_PI/3.0-theta) + sin(2*M_PI/3.0-theta) / tan(M_PI/3.0);
	}
      else /* B--G */
	{
	  theta -= 2*M_PI/3;
	  r = 0;
	  g = cos(theta) + sin(theta) / tan(M_PI/3.0);
	  b = cos(2*M_PI/3.0-theta) + sin(2*M_PI/3.0-theta) / tan(M_PI/3.0);
	}
      r = 255 * r;
      g = 255 * g;
      b = 255 * b;

      if (r > 255) r = 255; else if (r < 0) r = 0;
      if (g > 255) g = 255; else if (g < 0) g = 0;
      if (b > 255) b = 255; else if (b < 0) b = 0;

      cmap[x].at[0] = b;
      cmap[x].at[1] = g;
      cmap[x].at[2] = r;
    }
  cmap[x].at[0] = cmap[x].at[1] = cmap[x].at[2] = 0; x++;
  cmap[x].at[0] = cmap[x].at[1] = cmap[x].at[2] = 255;

  return cmap;
}


/*
 * ラベルのカラーマップの設定
 */
static uchar3 *MakeLabelColormap
  _P2 ((long, label_max),
       (long, color_max))
{
  long smax;
  uchar3 *cmap;
  long x;

  cmap = typenew1(label_max + 1, uchar3);
  smax = label_max / color_max + 1;
  cmap[0].at[0] = cmap[0].at[1] = cmap[0].at[2] = 128;

  for (x = 1; x <= label_max; x++)
    {
      double theta;
      double s;
      int ti, si;
      double r,g,b;

      si = (x - 1) / color_max;
      ti = (x - 1) % color_max;

      s = 1 - (double)si / smax;
      theta = 2 * M_PI * (ti + (double)si/smax) / color_max;

      if (theta <= 2*M_PI/3) /* R--G */
	{
	  b = 0;
	  r = cos(theta) + sin(theta) / tan(M_PI/3.0);
	  g = cos(2*M_PI/3.0-theta) + sin(2*M_PI/3.0-theta) / tan(M_PI/3.0);
	  if (s < 0.5) { b += s; } else { r *= s; g *= s; }
	}
      else
      if (4*M_PI/3 <= theta) /* R--B */
	{
	  theta -= 4*M_PI/3;
	  g = 0;
	  b = cos(theta) + sin(theta) / tan(M_PI/3.0);
	  r = cos(2*M_PI/3.0-theta) + sin(2*M_PI/3.0-theta) / tan(M_PI/3.0);
	  if (s < 0.5) { g += s; } else { r *= s; b *= s; }
	}
      else /* B--G */
	{
	  theta -= 2*M_PI/3;
	  r = 0;
	  g = cos(theta) + sin(theta) / tan(M_PI/3.0);
	  b = cos(2*M_PI/3.0-theta) + sin(2*M_PI/3.0-theta) / tan(M_PI/3.0);
	  if (s < 0.5) { r += s; } else { b *= s; g *= s; }
	}
      r = 255 * r;
      g = 255 * g;
      b = 255 * b;

      if (r > 255) r = 255; else if (r < 0) r = 0;
      if (g > 255) g = 255; else if (g < 0) g = 0;
      if (b > 255) b = 255; else if (b < 0) b = 0;

      cmap[x].at[0] = r;
      cmap[x].at[1] = g;
      cmap[x].at[2] = b;
    }
  return cmap;
}



#define _DP_GRAY        0
#define _DP_LABEL       1
#define _DP_FEATURE     2
#define _DP_COLOR_LABEL 3
#define _DP_FONT        4

/*==============*
 * 変換テーブル
 *==============*/
typedef void (*t_imageShowing_type1) P7 (( image  , output    ),
					 ( image  , original  ),
					 ( long   , img_x     ),
					 ( long   , img_y     ),
					 ( long   , img_xsize ),
					 ( long   , img_ysize ),
					 ( double , max       ))

#define DEFINE_IMAGE_SHOWING_GRAY_TYPE1(TYPE) \
static void imageShowing_gray_##TYPE \
  _P7 (( image , output    ), \
       ( image , input     ), \
       ( long  , img_x     ), \
       ( long  , img_y     ), \
       ( long  , img_xsize ), \
       ( long  , img_ysize ), \
       ( double, max       )) \
{ \
  register TYPE **src; \
  register uchar **data; \
  register int x, y; \
 \
  long minus = 0; \
  double p, mag_max, mag; \
 \
  Image.make(output, UChar, img_xsize, img_ysize); \
  src = __DATA(input, TYPE); \
  data = __DATA(output, uchar); \
 \
  if (max == 0) \
    { \
      mag_max = src[img_y][img_x]; \
      minus = 0; \
      for ( y = 0; y < __YSIZE(input); y++ ) \
	for ( x = 0; x < __XSIZE(input); x++ ) \
	  { \
	    p = src[y][x]; \
	    if (p < 0) minus = 1; \
	    mag = fabs(p); \
	    if (mag_max < mag) mag_max = mag; \
	  } \
    } \
 else mag_max = max; \
 \
  for ( y = 0; y < img_ysize; y++ ) \
    for ( x = 0; x < img_xsize; x++ ) \
      { \
	p = src[img_y + y][img_x + x]; \
	if (minus) \
	  p = 127 * p / mag_max + 127; \
	else \
	  p = 255 * p / mag_max; \
	if (p < 0) p = 0; else if (p > 255) p = 255; \
	data[y][x] = p; \
      } \
}

DEFINE_IMAGE_SHOWING_GRAY_TYPE1(bit4)
DEFINE_IMAGE_SHOWING_GRAY_TYPE1(char)
DEFINE_IMAGE_SHOWING_GRAY_TYPE1(short)
DEFINE_IMAGE_SHOWING_GRAY_TYPE1(long)
DEFINE_IMAGE_SHOWING_GRAY_TYPE1(uchar)
DEFINE_IMAGE_SHOWING_GRAY_TYPE1(ushort)
DEFINE_IMAGE_SHOWING_GRAY_TYPE1(ulong)
DEFINE_IMAGE_SHOWING_GRAY_TYPE1(float)
DEFINE_IMAGE_SHOWING_GRAY_TYPE1(double)

#define DEFINE_IMAGE_SHOWING_FEATURE_PLUS_TYPE1(TYPE) \
static void imageShowing_feature_##TYPE \
  _P7 (( image , output    ), \
       ( image , input     ), \
       ( long  , img_x     ), \
       ( long  , img_y     ), \
       ( long  , img_xsize ), \
       ( long  , img_ysize ), \
       ( double, max       )) \
{ \
  register TYPE **src; \
  register uchar3 **data; \
  register int x, y; \
  register long label; \
  long label_max; \
  uchar3 *cmap; \
 \
  Image.make(output, UChar3, img_xsize, img_ysize); \
  src = __DATA(input, TYPE); \
  data = __DATA(output, uchar3); \
 \
  if (max == 0) \
    { \
      label_max = 0; \
      for (y = 0; y < __YSIZE(input); y++) \
	for (x = 0; x < __XSIZE(input); x++) \
	  if ((long)src[y][x] > label_max) label_max = src[y][x]; \
    } \
  else label_max = max; \
 \
  cmap = MakeFeatureColormap(label_max); \
 \
  for ( y = 0; y < img_ysize; y++) \
    for ( x = 0; x < img_xsize; x++ ) \
      { \
	label = src[img_y + y][img_x + x]; \
	if (label > label_max) label = label_max; \
	data[y][x] = cmap[label]; \
      } \
  typefree1(cmap); \
}

#define DEFINE_IMAGE_SHOWING_FEATURE_MINUS_TYPE1(TYPE) \
static void imageShowing_feature_##TYPE \
  _P7 (( image , output    ), \
       ( image , input     ), \
       ( long  , img_x     ), \
       ( long  , img_y     ), \
       ( long  , img_xsize ), \
       ( long  , img_ysize ), \
       ( double, max       )) \
{ \
  register TYPE **src; \
  register uchar3 **data; \
  register int x, y; \
  register uchar3 *cmap; \
 \
  long minus = 0; \
  long p, mag_max, mag, label_max; \
 \
  Image.make(output, UChar3, img_xsize, img_ysize); \
  src = __DATA(input, TYPE); \
  data = __DATA(output, uchar3); \
 \
   for ( y = 0; y < __YSIZE(input); y++ ) \
     for ( x = 0; x < __XSIZE(input); x++ ) \
       if (src[y][x] < 0) minus = 1; \
  if (max == 0) \
    { \
      mag_max = src[0][0]; \
      minus = 0; \
      for ( y = 0; y < __YSIZE(input); y++ ) \
	for ( x = 0; x < __XSIZE(input); x++ ) \
	  { \
	    p = src[y][x]; \
	    mag = abs(p); \
	    if (mag_max < mag) mag_max = mag; \
	  } \
    } \
  else mag_max = max; \
 \
  label_max = mag_max; \
  if (minus) label_max = mag_max * 2 + 1; \
  cmap = MakeFeatureColormap(label_max); \
 \
  for ( y = 0; y < img_ysize; y++ ) \
    for ( x = 0; x < img_xsize; x++ ) \
      { \
	p = src[img_y + y][img_x + x]; \
	if (minus) p += mag_max; \
	if (p < 0) p = 0; \
	else if (p > label_max) p = label_max; \
	data[y][x] = cmap[p]; \
      } \
}
/*
	if (p < 0) p = label_max + 2;\
	else if (p > label_max) p = label_max + 1; \
*/
#define REAL_FEATURE_MAX 1024
#define DEFINE_IMAGE_SHOWING_FEATURE_REAL_TYPE1(TYPE) \
static void imageShowing_feature_##TYPE \
  _P7 (( image , output    ), \
       ( image , input     ), \
       ( long  , img_x     ), \
       ( long  , img_y     ), \
       ( long  , img_xsize ), \
       ( long  , img_ysize ), \
       ( double, max       )) \
{ \
  register TYPE **src; \
  register uchar3 **data; \
  register int x, y; \
  register uchar3 *cmap; \
 \
  long minus = 0; \
  double p, mag_max, mag; \
  long idx, label_max; \
 \
  Image.make(output, UChar3, img_xsize, img_ysize); \
  src = __DATA(input, TYPE); \
  data = __DATA(output, uchar3); \
 \
   for ( y = 0; y < __YSIZE(input); y++ ) \
     for ( x = 0; x < __XSIZE(input); x++ ) \
       if (src[y][x] < 0) minus = 1; \
  if (max == 0) \
    { \
      mag_max = src[0][0]; \
      minus = 0; \
      for ( y = 0; y < __YSIZE(input); y++ ) \
	for ( x = 0; x < __XSIZE(input); x++ ) \
	  { \
	    p = src[y][x]; \
	    mag = fabs(p); \
	    if (mag_max < mag) mag_max = mag; \
	  } \
    } \
  else mag_max = max; \
 \
  if (mag_max == 0) mag_max = 1; \
  label_max = REAL_FEATURE_MAX; \
  if (minus) label_max = label_max * 2 + 1; \
  cmap = MakeFeatureColormap(label_max); \
 \
  for ( y = 0; y < img_ysize; y++ ) \
    for ( x = 0; x < img_xsize; x++ ) \
      { \
	p = src[img_y + y][img_x + x]; \
	if (minus) p += mag_max; \
	idx = REAL_FEATURE_MAX * p / mag_max; \
	if (idx < 0) idx = 0; \
	else if (idx > label_max) idx = label_max; \
	data[y][x] = cmap[idx]; \
      } \
}
/*
	if (idx < 0) idx = label_max + 2; \
	else if (idx > label_max) idx = label_max + 1; \
*/

DEFINE_IMAGE_SHOWING_FEATURE_PLUS_TYPE1(bit4)
DEFINE_IMAGE_SHOWING_FEATURE_MINUS_TYPE1(char)
DEFINE_IMAGE_SHOWING_FEATURE_MINUS_TYPE1(short)
DEFINE_IMAGE_SHOWING_FEATURE_MINUS_TYPE1(long)
DEFINE_IMAGE_SHOWING_FEATURE_PLUS_TYPE1(uchar)
DEFINE_IMAGE_SHOWING_FEATURE_PLUS_TYPE1(ushort)
DEFINE_IMAGE_SHOWING_FEATURE_PLUS_TYPE1(ulong)
DEFINE_IMAGE_SHOWING_FEATURE_REAL_TYPE1(float)
DEFINE_IMAGE_SHOWING_FEATURE_REAL_TYPE1(double)

#define DEFINE_IMAGE_SHOWING_LABEL_TYPE1(TYPE) \
static void imageShowing_label_##TYPE \
  _P7 (( image , output    ), \
       ( image , input     ), \
       ( long  , img_x     ), \
       ( long  , img_y     ), \
       ( long  , img_xsize ), \
       ( long  , img_ysize ), \
       ( double, max       )) \
{ \
  register TYPE **src; \
  register long **data; \
  register int x, y; \
 \
  Image.make(output, Long, img_xsize, img_ysize); \
  src = __DATA(input, TYPE); \
  data = __DATA(output, long); \
 \
  for ( y = 0; y < img_ysize; y++ ) \
    for ( x = 0; x < img_xsize; x++ ) \
      { \
	data[y][x] = (long)(src[img_y + y][img_x + x]); \
      } \
}

DEFINE_IMAGE_SHOWING_LABEL_TYPE1(bit4)
DEFINE_IMAGE_SHOWING_LABEL_TYPE1(char)
DEFINE_IMAGE_SHOWING_LABEL_TYPE1(short)
DEFINE_IMAGE_SHOWING_LABEL_TYPE1(long)
DEFINE_IMAGE_SHOWING_LABEL_TYPE1(uchar)
DEFINE_IMAGE_SHOWING_LABEL_TYPE1(ushort)
DEFINE_IMAGE_SHOWING_LABEL_TYPE1(ulong)
DEFINE_IMAGE_SHOWING_LABEL_TYPE1(float)
DEFINE_IMAGE_SHOWING_LABEL_TYPE1(double)

#define DEFINE_IMAGE_SHOWING_FONT_TYPE1(TYPE) \
static void imageShowing_font_##TYPE \
  _P7 (( image , output    ), \
       ( image , input     ), \
       ( long  , img_x     ), \
       ( long  , img_y     ), \
       ( long  , img_xsize ), \
       ( long  , img_ysize ), \
       ( double, max       )) \
{ \
  register TYPE **src; \
  register char **data; \
  register int x, y; \
 \
  Image.make(output, Char, img_xsize, img_ysize); \
  src = __DATA(input, TYPE); \
  data = __DATA(output, char); \
 \
  for ( y = 0; y < img_ysize; y++ ) \
    for ( x = 0; x < img_xsize; x++ ) \
      { \
	data[y][x] = (char)(src[img_y + y][img_x + x]); \
      } \
}

DEFINE_IMAGE_SHOWING_FONT_TYPE1(bit4)
DEFINE_IMAGE_SHOWING_FONT_TYPE1(char)
DEFINE_IMAGE_SHOWING_FONT_TYPE1(short)
DEFINE_IMAGE_SHOWING_FONT_TYPE1(long)
DEFINE_IMAGE_SHOWING_FONT_TYPE1(uchar)
DEFINE_IMAGE_SHOWING_FONT_TYPE1(ushort)
DEFINE_IMAGE_SHOWING_FONT_TYPE1(ulong)
DEFINE_IMAGE_SHOWING_FONT_TYPE1(float)
DEFINE_IMAGE_SHOWING_FONT_TYPE1(double)

#define DEFINE_IMAGE_SHOWING_COLOR_LABEL_TYPE1(TYPE) \
static void imageShowing_color_label_##TYPE \
  _P7 (( image , output    ), \
       ( image , input     ), \
       ( long  , img_x     ), \
       ( long  , img_y     ), \
       ( long  , img_xsize ), \
       ( long  , img_ysize ), \
       ( double, max       )) \
{ \
  register TYPE **src; \
  register uchar3 **data; \
  register int x, y; \
  register long label; \
  long label_max; \
  uchar3 *cmap; \
 \
  Image.make(output, UChar3, img_xsize, img_ysize); \
  src = __DATA(input, TYPE); \
  data = __DATA(output, uchar3); \
 \
  label_max = 0; \
  for (y = 0; y < __YSIZE(input); y++) \
    for (x = 0; x < __XSIZE(input); x++) \
      if ((long)src[y][x] > label_max) label_max = src[y][x]; \
  if (max == 0) max = label_max; \
 \
  if (max <= 0) max = 12; \
  cmap = MakeLabelColormap(label_max, (long)max); \
 \
  for ( y = 0; y < img_ysize; y++) \
    for ( x = 0; x < img_xsize; x++ ) \
      { \
	label = src[img_y + y][img_x + x]; \
 \
	if ( label < 0 ) \
	  { \
	    data[y][x].at[0] = 64; \
	    data[y][x].at[1] = 64; \
	    data[y][x].at[2] = 64; \
	  } \
	else \
	  { \
	    data[y][x] = cmap[label]; \
	  } \
      } \
  typefree1(cmap); \
}

DEFINE_IMAGE_SHOWING_COLOR_LABEL_TYPE1(bit4)
DEFINE_IMAGE_SHOWING_COLOR_LABEL_TYPE1(char)
DEFINE_IMAGE_SHOWING_COLOR_LABEL_TYPE1(short)
DEFINE_IMAGE_SHOWING_COLOR_LABEL_TYPE1(long)
DEFINE_IMAGE_SHOWING_COLOR_LABEL_TYPE1(uchar)
DEFINE_IMAGE_SHOWING_COLOR_LABEL_TYPE1(ushort)
DEFINE_IMAGE_SHOWING_COLOR_LABEL_TYPE1(ulong)
DEFINE_IMAGE_SHOWING_COLOR_LABEL_TYPE1(float)
DEFINE_IMAGE_SHOWING_COLOR_LABEL_TYPE1(double)

static t_imageShowing_type1 table_imageShowing_type1[][15] = {
  {/* gray */
    0, 0, 0,
    imageShowing_gray_bit4,
    imageShowing_gray_char,
    imageShowing_gray_short,
    imageShowing_gray_long,
    imageShowing_gray_long,
    imageShowing_gray_uchar,
    imageShowing_gray_ushort,
    imageShowing_gray_ulong,
    imageShowing_gray_ulong,
    imageShowing_gray_float,
    imageShowing_gray_double,
  },
  {/* label */
    0, 0, 0,
    imageShowing_label_bit4,
    imageShowing_label_char,
    imageShowing_label_short,
    imageShowing_label_long,
    imageShowing_label_long,
    imageShowing_label_uchar,
    imageShowing_label_ushort,
    imageShowing_label_ulong,
    imageShowing_label_ulong,
    imageShowing_label_float,
    imageShowing_label_double,
  },
  {/* feature */
    0, 0, 0,
    imageShowing_feature_bit4,
    imageShowing_feature_char,
    imageShowing_feature_short,
    imageShowing_feature_long,
    imageShowing_feature_long,
    imageShowing_feature_uchar,
    imageShowing_feature_ushort,
    imageShowing_feature_ulong,
    imageShowing_feature_ulong,
    imageShowing_feature_float,
    imageShowing_feature_double,
  },
  {/* color label */
    0, 0, 0,
    imageShowing_color_label_bit4,
    imageShowing_color_label_char,
    imageShowing_color_label_short,
    imageShowing_color_label_long,
    imageShowing_color_label_long,
    imageShowing_color_label_uchar,
    imageShowing_color_label_ushort,
    imageShowing_color_label_ulong,
    imageShowing_color_label_ulong,
    imageShowing_color_label_float,
    imageShowing_color_label_double,
  },
  {/* font */
    0, 0, 0,
    imageShowing_font_bit4,
    imageShowing_font_char,
    imageShowing_font_short,
    imageShowing_font_long,
    imageShowing_font_long,
    imageShowing_font_uchar,
    imageShowing_font_ushort,
    imageShowing_font_ulong,
    imageShowing_font_ulong,
    imageShowing_font_float,
    imageShowing_font_double,
  },
};

typedef struct _dp_rec {
  char *env;
  double max;
  long type;
} t_dp_rec;

static t_dp_rec dp_list[] = {
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {"CIL_DP_BIT4"  , 15 , _DP_GRAY },
  {"CIL_DP_CHAR"  , 127, _DP_FONT },
  {"CIL_DP_SHORT" , 0  , _DP_GRAY },
  {"CIL_DP_LONG"  , 0  , _DP_GRAY },
  {"CIL_DP_LONG"  , 0  , _DP_LABEL},
  {"CIL_DP_UCHAR" , 255, _DP_GRAY },
  {"CIL_DP_USHORT", 0  , _DP_LABEL},
  {"CIL_DP_ULONG" , 0  , _DP_GRAY },
  {"CIL_DP_ULONG" , 0  , _DP_GRAY },
  {"CIL_DP_FLOAT" , 0  , _DP_GRAY },
  {"CIL_DP_DOUBLE", 0  , _DP_GRAY },
};

static long imageShowingProperty
  _P2 (( long    , index ),
       ( double *, max_p ))
{
  char *env;
  char *buf;
  char str[1024];
  long type;
  int i, n;

  env    = dp_list[index].env;
  *max_p = dp_list[index].max;
  type   = dp_list[index].type;

  if (env == 0) return type;
  buf = getenv(env);
  if (buf == 0) return type;
  strcpy(str,buf);

  n = strlen(str);
  for (i = 0; i < n; i++)
    if (str[i] == '#') { str[i] = '\0'; *max_p = atof(str + i + 1); break; }

  if (strcmp("feature"    , str) == 0) return _DP_FEATURE;
  if (strcmp("gray"       , str) == 0) return _DP_GRAY;
  if (strcmp("font"       , str) == 0) return _DP_FONT;
  if (strcmp("label"      , str) == 0) return _DP_LABEL;
  if (strcmp("color-label", str) == 0) return _DP_COLOR_LABEL;

  fprintf(stderr, "no supported show property %s=%s\n", env, str);

  return type;
}



static void imageShowing_type1
  _P6 (( image, output    ),
       ( image, input     ),
       ( long , img_x     ),
       ( long , img_y     ),
       ( long , img_xsize ),
       ( long , img_ysize ))
{
  long type;
  double max;
  t_imageShowing_type1 caller;

  type = imageShowingProperty(__TYPE(input), &max);
  caller = table_imageShowing_type1[type][input->type];
  if (caller)
    (*caller)(output, input, img_x, img_y, img_xsize, img_ysize, max);
  else
    fprintf(stderr,"no supported image type %d\n", input->type);
}



#define DEFINE_IMAGE_SHOWING_TYPE3(TYPE) \
static void imageShowing_##TYPE \
  _P6 (( image, output    ), \
       ( image, input     ), \
       ( long , img_x     ), \
       ( long , img_y     ), \
       ( long , img_xsize ), \
       ( long , img_ysize )) \
{ \
  register TYPE **src; \
  register uchar3 **data; \
  register int x, y; \
 \
  long minus = 0; \
  double p0, p1, p2; \
  double mag_max, mag_min, mag_range, mag, mag_offset; \
 \
  Image.make(output, UChar3, img_xsize, img_ysize); \
 \
  src = __DATA(input, TYPE); \
  data = __DATA(output, uchar3); \
 \
  p0 = src[img_y][img_x].at[0]; \
  p1 = src[img_y][img_x].at[1]; \
  p2 = src[img_y][img_x].at[2]; \
  mag_max = mag_min = p0*p0 + p1*p1 + p2*p2; \
  minus = 0; \
  for ( y = 0; y < __YSIZE(input); y++ ) \
    for ( x = 0; x < __XSIZE(input); x++ ) \
      { \
	p0 = src[y][x].at[0]; \
	p1 = src[y][x].at[1]; \
	p2 = src[y][x].at[2]; \
	if ((p0 < 0) || (p1 < 0) || (p2 < 0)) minus = 1; \
	mag = p0*p0 + p1*p1 + p2*p2; \
	if (mag_max < mag) mag_max = mag; else \
	if (mag_min > mag) mag_min = mag; \
      } \
  mag_max = sqrt(mag_max); \
  mag_min = sqrt(mag_min); \
  mag_range = (mag_max - mag_min); \
 \
  if (minus) \
    { \
      mag_max = 127.0*sqrt(3.0)/mag_range; \
      mag_offset = 127; \
    } \
  else \
    { \
      mag_max = 255.0*sqrt(3.0)/mag_range; \
      mag_offset = 0; \
    } \
  mag_min = mag_max * mag_min; \
\
  for ( y = 0; y < img_ysize; y++ )\
    for ( x = 0; x < img_xsize; x++ )\
      {\
	p0 = src[img_y + y][img_x + x].at[0];\
	p1 = src[img_y + y][img_x + x].at[1];\
	p2 = src[img_y + y][img_x + x].at[2];\
\
	mag = sqrt(p0*p0 + p1*p1 + p2*p2);\
	if (mag != 0) mag = mag_max - mag_min/mag;\
	p0 = p0 * mag + mag_offset;\
	p1 = p1 * mag + mag_offset;\
	p2 = p2 * mag + mag_offset;\
\
	if (p0 > 255) p0 = 255; else if (p0 < 0) p0 = 0;\
	if (p1 > 255) p1 = 255; else if (p1 < 0) p1 = 0;\
	if (p2 > 255) p2 = 255; else if (p2 < 0) p2 = 0;\
\
	data[y][x].at[0] = p0;\
	data[y][x].at[1] = p1;\
	data[y][x].at[2] = p2;\
      }\
}



#define DEFINE_IMAGE_SHOWING_TYPE2(TYPE) \
static void imageShowing_##TYPE \
  _P6 (( image, output    ), \
       ( image, input     ), \
       ( long , img_x     ), \
       ( long , img_y     ), \
       ( long , img_xsize ), \
       ( long , img_ysize )) \
{ \
  register TYPE **src; \
  register uchar3 **data; \
  register int x, y; \
  register double p0, p1; \
  register uchar *u; \
 \
  double mag, mag_max; \
 \
  Image.make(output, UChar3, img_xsize, img_ysize); \
 \
  src = __DATA(input, TYPE); \
  data = __DATA(output, uchar3); \
 \
  mag_max = 0; \
  for ( y = 0; y < __YSIZE(input); y++ ) \
    for ( x = 0; x < __XSIZE(input); x++ ) \
      { \
	p0 = src[y][x].at[0]; \
	p1 = src[y][x].at[1]; \
	mag = p0*p0 + p1*p1; \
	if (mag_max < mag) mag_max = mag; \
      } \
  mag_max = sqrt(mag_max); \
 \
  for ( y = 0; y < img_ysize; y++ ) \
    for ( x = 0; x < img_xsize; x++ ) \
      { \
	double r, g, b; \
	double vx, vy; \
	double theta, radius; \
 \
	p0 = src[img_y + y][img_x + x].at[0]; \
	p1 = src[img_y + y][img_x + x].at[1]; \
	u = data[y][x].at; \
 \
	vx = - p0 / mag_max; \
	vy = p1 / mag_max; \
 \
	if ((vy == 0) && (vx == 0)) \
	  { \
	    u[0] = 255; \
	    u[1] = 255; \
	    u[2] = 255; \
	    continue; \
	  } \
	theta = (vy == 0) ? ((vx>0)?0:M_PI) : atan2(vy, vx); \
	if (theta < 0) theta = 2*M_PI + theta; \
	radius = sqrt(vx*vx+vy*vy); \
	if (theta <= 2*M_PI/3) \
	  { \
	    b = (1.0 - radius) / 1.0; \
	    r = radius*(cos(theta) + sin(theta) / tan(M_PI/3.0)) + b; \
	    g = radius*(cos(2*M_PI/3.0-theta) \
			+ sin(2*M_PI/3.0-theta) / tan(M_PI/3.0)) + b; \
	  } \
	else \
	if (4*M_PI/3 <= theta) /* R--B */ \
	  { \
	    theta -= 4*M_PI/3; \
	    g = (1.0 - radius) / 1.0; \
	    b = radius*(cos(theta) + sin(theta) / tan(M_PI/3.0)) + g; \
	    r = radius*(cos(2*M_PI/3.0-theta) \
			+ sin(2*M_PI/3.0-theta) / tan(M_PI/3.0)) + g; \
	  } \
	else /* B--G */ \
	  { \
	    theta -= 2*M_PI/3; \
	    r = (1.0 - radius) / 1.0; \
	    g = radius*(cos(theta) + sin(theta) / tan(M_PI/3.0)) + r; \
	    b = radius*(cos(2*M_PI/3.0-theta) \
			+ sin(2*M_PI/3.0-theta) / tan(M_PI/3.0)) + r; \
	  } \
 \
	r = 255 * r; if (r > 255) r = 255; else if (r < 0) r = 0; \
	g = 255 * g; if (g > 255) g = 255; else if (g < 0) g = 0; \
	b = 255 * b; if (b > 255) b = 255; else if (b < 0) b = 0; \
 \
	u[0] = r; \
	u[1] = g; \
	u[2] = b; \
      } \
}



#define DEFINE_IMAGE_SHOWING_COPY(TYPE) \
static void imageShowing_copy_##TYPE \
  _P6 (( image , output    ), \
       ( image , input     ), \
       ( long  , img_x     ), \
       ( long  , img_y     ), \
       ( long  , img_xsize ), \
       ( long  , img_ysize )) \
{ \
  register TYPE **src; \
  register TYPE **data; \
  register int x, y; \
 \
  Image.make(output, __TYPE(input), img_xsize, img_ysize); \
  src = __DATA(input, TYPE); \
  data = __DATA(output, TYPE); \
 \
  for ( y = 0; y < img_ysize; y++ ) \
    for ( x = 0; x < img_xsize; x++ ) \
      { \
	data[y][x] = src[img_y + y][img_x + x]; \
      } \
}



DEFINE_IMAGE_SHOWING_TYPE2(char2)
DEFINE_IMAGE_SHOWING_TYPE2(short2)
DEFINE_IMAGE_SHOWING_TYPE2(long2)
DEFINE_IMAGE_SHOWING_TYPE2(uchar2)
DEFINE_IMAGE_SHOWING_TYPE2(ushort2)
DEFINE_IMAGE_SHOWING_TYPE2(ulong2)
DEFINE_IMAGE_SHOWING_TYPE2(float2)
DEFINE_IMAGE_SHOWING_TYPE2(double2)

DEFINE_IMAGE_SHOWING_TYPE3(char3)
DEFINE_IMAGE_SHOWING_TYPE3(short3)
DEFINE_IMAGE_SHOWING_TYPE3(long3)
DEFINE_IMAGE_SHOWING_TYPE3(ushort3)
DEFINE_IMAGE_SHOWING_TYPE2(ulong3)
DEFINE_IMAGE_SHOWING_TYPE3(float3)
DEFINE_IMAGE_SHOWING_TYPE3(double3)

DEFINE_IMAGE_SHOWING_COPY(bit1)
DEFINE_IMAGE_SHOWING_COPY(uchar3)

/*==============*
 * 変換テーブル
 *==============*/
typedef void (*t_imageShowing) P6 (( image, output    ),
				   ( image, original  ),
				   ( long , img_x     ),
				   ( long , img_y     ),
				   ( long , img_xsize ),
				   ( long , img_ysize ))

static t_imageShowing table_imageShowing[] = {
  0,
  0/*imageShowing_packedbit1*/,
  imageShowing_copy_bit1,

  imageShowing_type1,
  imageShowing_type1,
  imageShowing_type1,
  imageShowing_type1,
  imageShowing_type1,
  imageShowing_type1,
  imageShowing_type1,
  imageShowing_type1,
  imageShowing_type1,
  imageShowing_type1,
  imageShowing_type1,

  imageShowing_char2,
  imageShowing_short2,
  imageShowing_long2,
  imageShowing_long2,
  imageShowing_uchar2,
  imageShowing_ushort2,
  imageShowing_ulong2,
  imageShowing_ulong2,
  imageShowing_float2,
  imageShowing_double2,

  imageShowing_char3,
  imageShowing_short3,
  imageShowing_long3,
  imageShowing_long3,
  imageShowing_copy_uchar3,
  imageShowing_ushort3,
  imageShowing_ulong3,
  imageShowing_ulong3,
  imageShowing_float3,
  imageShowing_double3
};

/*=========================================================*
 * 表示可能な型(Bit1, Char, UChar, UChar3, Long)に変換する。
 *=========================================================*/
void ImageShowing
  _P6 (( image, output    ),
       ( image, original  ),
       ( long , img_x     ),
       ( long , img_y     ),
       ( long , img_xsize ),
       ( long , img_ysize ))
{
  image input;
  long type;
  t_imageShowing caller;

  if (original == output)
    input = Image.createFromImage("TMP",original);
  else
    input = original;

  caller = table_imageShowing[input->type];
  if (caller)
    (*caller)(output, input, img_x, img_y, img_xsize, img_ysize);
  else
    fprintf(stderr,"no supported image type %d\n", input->type);

  if (original == output) Image.destroy(input);
}
