/*
 * XImage.c
 */


#include "XImage.h"

/* Drawable or Window のオプション */
int XDrawImage_SetColormap = 1;
void XDrawImage
  _P11 (( Display *, display   ),
	( Window   , window    ),
	( GC       , gc        ),
	( long     , win_x     ),
	( long     , win_y     ),
	( long     , win_xsize ),
	( long     , win_ysize ),
	( long     , type      ),
	( char **  , data      ),
	( long     , img_xsize ),
	( long     , img_ysize ))
{
  XImage *src;

  if (win_xsize <= 0 || win_ysize <= 0) return;
  if (img_xsize <= 0 || img_ysize <= 0) return;

  if (XDrawImage_SetColormap) XImageSetColormap(display, window);

  src = XCreateImageFromData(display, win_xsize, win_ysize,
			     type, data, img_xsize, img_ysize);
  if (src == 0) return;

  XPutImage(display, window, gc, src, 0, 0,
	    win_x, win_y, win_xsize, win_ysize);

  XDestroyImage(src);
}



XImage *XCreateImageFromData
  _P7 (( Display *, display   ),
       ( long     , win_xsize ),
       ( long     , win_ysize ),
       ( long     , type      ),
       ( char **  , data      ),
       ( long     , img_xsize ),
       ( long     , img_ysize ))
{
  XImage *src;
  long plane;

  plane = DisplayPlanes(display, DefaultScreen(display));

  XImageSetErrorPorpagate();
  XImageSetLookupTable();
  XImageSetRGBPlane(display);

  switch( plane )
    {
    case 1:
      src = XCreateImageFromData_1Plane(display, win_xsize, win_ysize,
					type, data, img_xsize, img_ysize);
      break;

    case 8:
      src = XCreateImageFromData_8Planes(display, win_xsize, win_ysize,
					 type, data, img_xsize, img_ysize);

      break;

    case 15:
      src = XCreateImageFromData_15Planes(display, win_xsize, win_ysize,
					  type, data, img_xsize, img_ysize);
      break;

    case 16:
      src = XCreateImageFromData_16Planes(display, win_xsize, win_ysize,
					  type, data, img_xsize, img_ysize);
      break;

    case 24:
      src = XCreateImageFromData_24Planes(display, win_xsize, win_ysize,
					  type, data, img_xsize, img_ysize);
      break;

    case 32:
      src = XCreateImageFromData_32Planes(display, win_xsize, win_ysize,
					  type, data, img_xsize, img_ysize);
      break;

    default:
      fprintf(stderr,
	      "error:XCreateImageFromData:I don't know this display type.\n" );
      return 0;
    }

  return src;
}


#include <stdlib.h>
#include <math.h>
/* for access */
#include <unistd.h>

/*==========================================*
 * 指定されたファイルからＬＵＴを読み込む。
 *==========================================*/
long Read_RGB_LUT
  _P2 ((uchar3 *, lut     ),
       (char *  , filename))
{
  int i;
  FILE *fp;
  long r, g, b;
  char buf[256];
  static int done_announce = 0;

  for (i = 0; i < 256; i++)
    {
      lut[i].at[0] = i;
      lut[i].at[1] = i;
      lut[i].at[2] = i;
    }

  if (access(filename,R_OK) != 0) return 0;
  fp = fopen(filename,"r");
  if (fp == 0) return 0;

  if (done_announce == 0)
    {
      done_announce = 1;
      fprintf(stderr,"read property file %s\n", filename);
    }

  i = 0;
  while (fgets(buf,sizeof(buf),fp))
    {
      if (buf[0] == '#') continue;
      sscanf(buf,"%d %d %d",&r,&g,&b);
      if (r < 0) r = 0; else if (r > 255) r = 255;
      if (g < 0) g = 0; else if (g > 255) g = 255;
      if (b < 0) b = 0; else if (b > 255) b = 255;
      lut[i].at[0] = r;
      lut[i].at[1] = g;
      lut[i].at[2] = b;
      i++;
      if (i > 255) break;
    }
  fclose(fp);

  return 1;
}

/*--------------------------------------------------------------*
 * ディスプレイ特性のルックアップテーブルをファイルから読み込む
 *--------------------------------------------------------------*/
static long get_display_property
  _P1 ((uchar3 *, lut))
{
  char *env = getenv(CIL_DP_DIR);
  char *dname = getenv("DISPLAY");
  int i, n;
  char filename[256];
  char buf[256];

  for (i = 0; i < 256; i++)
    {
      lut[i].at[0] = i;
      lut[i].at[1] = i;
      lut[i].at[2] = i;
    }

  if ((env == 0) || (dname == 0)) return 0;

  strcpy(buf,dname);
  n = strlen(buf);
  for (i = 0; i < n; i++)
    if (buf[i] == ':') buf[i] = '\0';
  if (strlen(buf) == 0)
    {
      char *hostname = getenv("HOSTNAME");
      if (hostname == 0) return 0;
      strcpy(buf, hostname);
    }
  sprintf(filename,"%s/%s",env,buf);

  return Read_RGB_LUT(lut,filename);
}

/*---------------------------------*
 * RGBのγを環境変数から読み込む。
 *---------------------------------*/
static long get_gamma
  _P1 ((double3 *, gamma_p))
{
  int i, n;
  double r_gamma, g_gamma, b_gamma;
  char *gamma_env = getenv(CIL_GAMMA);
  char *r_env, *g_env, *b_env;
  char gamma_buf[256];

  gamma_p->at[0] = gamma_p->at[1] = gamma_p->at[2] = 1.0;
  if (gamma_env == 0) return 0;

  r_env = g_env = b_env = 0;
  strcpy(gamma_buf, gamma_env);
  n = strlen(gamma_buf);
  r_env = g_env = b_env = gamma_buf;
  for (i = 0; i < n; i++)
    if (gamma_buf[i] == ',')
      {gamma_buf[i] = '\0'; g_env = gamma_buf + i + 1; break;}
  for ( ; i < n; i++)
    if (gamma_buf[i] == ',')
      {gamma_buf[i] = '\0'; b_env = gamma_buf + i + 1; break;}

  r_gamma = atof(r_env);
  g_gamma = atof(g_env);
  b_gamma = atof(b_env);
  if ((r_gamma <= 0) || (g_gamma <= 0) || (b_gamma <= 0))
    {
      fprintf(stderr,"ignore gamma value %s=%s\n",CIL_GAMMA,gamma_env);
      return 0;
    }

  gamma_p->at[0] = r_gamma;
  gamma_p->at[1] = g_gamma;
  gamma_p->at[2] = b_gamma;

  return 1;
}

/*--------------------------------*
 * すべての補正のテーブルを作る。
 *--------------------------------*/
long CIL_Setup_RGB_LUT = 0;
uchar3 CIL_RGB_LUT[256];
static long get_gamma_lut
  _P1 ((uchar3 *, lut))
{
  double3 gamma;
  uchar3 prop_lut[256];
  int i;
  long gamma_f, dp_f;

  gamma_f = get_gamma(&gamma);
  if (CIL_Setup_RGB_LUT)
    {
      dp_f = 1;
      for (i = 0; i < 256; i++)
	prop_lut[i] = CIL_RGB_LUT[i];
    }
  else
    dp_f = get_display_property(prop_lut);

  for (i = 0; i < 256; i++)
    {
      long r, g, b;
      r = 255 * pow(i/255.0, 1.0/gamma.at[0]);
      g = 255 * pow(i/255.0, 1.0/gamma.at[1]);
      b = 255 * pow(i/255.0, 1.0/gamma.at[2]);
      if (r > 255) r = 255; else if (r < 0) r = 0;
      if (g > 255) g = 255; else if (g < 0) g = 0;
      if (b > 255) b = 255; else if (b < 0) b = 0;
      lut[i].at[0] = prop_lut[r].at[0];
      lut[i].at[1] = prop_lut[g].at[1];
      lut[i].at[2] = prop_lut[b].at[2];
    }

  return (dp_f || gamma_f);
}

/*--------------------------*
 * 濃淡画像のγ補正をする。
 *--------------------------*/
static void gamma_correction_uchar
  _P2 (( image   , showing ),
       ( uchar3 *, lut     ))
{
  register int x, y;
  image original;

  original = Image.createFromImage("ORG",showing);
  Image.make(showing,UChar3,__XSIZE(showing),__YSIZE(showing));
  for (y = 0; y < __YSIZE(showing); y++)
    for (x = 0; x < __XSIZE(showing); x++)
      __PIXEL(showing, x, y, uchar3) = lut[__PIXEL(original, x, y, uchar)];
  Image.destroy(original);
}

/*----------------------------*
 * カラー画像のγ補正をする。
 *----------------------------*/
static void gamma_correction_uchar3
  _P2 (( image   , showing ),
       ( uchar3 *, lut     ))
{
  register int x, y;

  for (y = 0; y < __YSIZE(showing); y++)
    for (x = 0; x < __XSIZE(showing); x++)
      {
	__PIXEL0(showing, x, y, uchar3)
	  = lut[__PIXEL0(showing, x, y, uchar3)].at[0];
	__PIXEL1(showing, x, y, uchar3)
	  = lut[__PIXEL1(showing, x, y, uchar3)].at[1];
	__PIXEL2(showing, x, y, uchar3)
	  = lut[__PIXEL2(showing, x, y, uchar3)].at[2];
      }
}

/*------------------*
 * γ補正を行なう。
 *------------------*/
static void gamma_correction
  _P1 (( image , showing ))
{
  uchar3 lut[256];

  if (get_gamma_lut(lut) == 0) return;

  if (__TYPE(showing) == UChar)
    gamma_correction_uchar(showing, lut);
  else
  if (__TYPE(showing) == UChar3)
    gamma_correction_uchar3(showing, lut);
}

void XShowImage
  _P12 (( Display *, display   ),
	( Window   , window    ),
	( GC       , gc        ),
	( image    , img       ),
	( long     , win_x     ),
	( long     , win_y     ),
	( long     , win_xsize ),
	( long     , win_ysize ),
	( long     , img_x     ),
	( long     , img_y     ),
	( long     , img_xsize ),
	( long     , img_ysize ))
{
  image showing;

  showing = Image.create("Showing");
  ImageShowing(showing, img, img_x, img_y, img_xsize, img_ysize);
  if (__TYPE(showing) != Bit1) gamma_correction(showing);
  XDrawImage(display, window, gc, win_x, win_y, win_xsize, win_ysize,
	     showing->type, showing->data, img_xsize, img_ysize);
  Image.destroy(showing);
}



void XShowImagePartOfImage
  _P8 (( Display *, display   ),
       ( Window   , window    ),
       ( GC       , gc        ),
       ( image    , img       ),
       ( long     , img_x     ),
       ( long     , img_y     ),
       ( long     , img_xsize ),
       ( long     , img_ysize ))
{
  Window root;
  int x, y;
  unsigned int xsize, ysize, border, depth;

  XGetGeometry(display, window, &root,
	       &x, &y, &xsize, &ysize, &border, &depth );

  XShowImage(display, window, gc, img,
	     0, 0, xsize, ysize, img_x, img_y, img_xsize, img_ysize );
}



void XShowImagePartOfWindow
  _P8 (( Display *, display   ),
       ( Window   , window    ),
       ( GC       , gc        ),
       ( image    , img       ),
       ( long     , win_x     ),
       ( long     , win_y     ),
       ( long     , win_xsize ),
       ( long     , win_ysize ))
{
  XShowImage(display, window, gc, img,
	     win_x, win_y, win_xsize, win_ysize,
	     0, 0, img->xsize, img->ysize );
}



void XShowImageFull
  _P4 (( Display *, display   ),
       ( Window   , window    ),
       ( GC       , gc        ),
       ( image    , img       ))
{
  Window root;
  int x, y;
  unsigned int xsize, ysize, border, depth;

  XGetGeometry(display, window, &root,
	       &x, &y, &xsize, &ysize, &border, &depth );

  XShowImage(display, window, gc, img,
	     0, 0, xsize, ysize, 0, 0, img->xsize, img->ysize );
}



uchar __XEPT_90[ 256 ];
uchar __XEPT_45[ 256 ];

void XImageSetErrorPorpagate()
{
  int i;

  for ( i = 0; i < 256; i++ )
    {
      __XEPT_90[i] = 7 * i / 22; /* 右, 上 */
      __XEPT_45[i] = 2 * i / 22; /* 右下, 左下 */
    }
}



uchar XLUT_PC_UChar3[ RMAX ][ GMAX ][ BMAX ];

uchar XLUT_PC_UChar[ 256 ];
uchar XLUT_GS_UChar[ 256 ];
uchar XLUT_SG_UChar[ 256 ];

uchar XLUT_PCR[ 256 ];
uchar XLUT_PCG[ 256 ];
uchar XLUT_PCB[ 256 ];
uchar XLUT_PCRerr[ RMAX ];
uchar XLUT_PCGerr[ GMAX ];
uchar XLUT_PCBerr[ BMAX ];

void XImageSetLookupTable()
{
  static long XLookupTable_initialized = 0;
  int i, r, g, b;

  if ( XLookupTable_initialized ) return;

  for ( i = 0; i < 256; i++ )
    XLUT_PC_UChar[ i ] = MMAX * i / 256 + MBASE;

  for ( i = 0; i < 256; i++ )
    XLUT_GS_UChar[ i ] = GrayMax * i / 256 + GrayBase;

  for ( i = 0; i < 256; i++ )
    XLUT_SG_UChar[ i ] = i;

  for ( r = 0; r < RMAX; r++ )
    for ( g = 0; g < GMAX; g++ )
      for ( b = 0; b < BMAX; b++ )
	XLUT_PC_UChar3[ r ][ g ][ b ] = r * GMAX * BMAX + g * BMAX + b + CBASE;

  for ( i = 0; i < 256; i++ )
    {
      XLUT_PCR[ i ] = RMAX * i / 256;
      XLUT_PCG[ i ] = GMAX * i / 256;
      XLUT_PCB[ i ] = BMAX * i / 256;
    }

  for ( r = 0; r < RMAX; r++ )
    XLUT_PCRerr[ r ] = 256 * r / RMAX;

  for ( g = 0; g < GMAX; g++ )
    XLUT_PCGerr[ g ] = 256 * g / GMAX;

  for ( b = 0; b < BMAX; b++ )
    XLUT_PCBerr[ b ] = 256 * b / BMAX;

  XLookupTable_initialized = 1;
}


/*
 * Setting RGB Plane Info for making pixel value
 */
int R_Shift = 0, G_Shift = 0, B_Shift = 0;
int R_LowBit = 0, G_LowBit = 0, B_LowBit = 0;

void XImageSetRGBPlane(Display *display)
{
  unsigned long r_mask = DefaultVisual(display,DefaultScreen(display))->red_mask;
  unsigned long g_mask = DefaultVisual(display,DefaultScreen(display))->green_mask;
  unsigned long b_mask = DefaultVisual(display,DefaultScreen(display))->blue_mask;

  if (r_mask) {
    for (R_Shift = 0; (r_mask & 1) == 0; R_Shift++) r_mask >>= 1;
    for (R_LowBit = 8; (r_mask & 1); R_LowBit--) r_mask >>= 1;
  }
  if (g_mask) {
    for (G_Shift = 0; (g_mask & 1) == 0; G_Shift++) g_mask >>= 1;
    for (G_LowBit = 8; (g_mask & 1); G_LowBit--) g_mask >>= 1;
  }
  if (b_mask) {
    for (B_Shift = 0; (b_mask & 1) == 0; B_Shift++) b_mask >>= 1;
    for (B_LowBit = 8; (b_mask & 1); B_LowBit--) b_mask >>= 1;
  }

}
