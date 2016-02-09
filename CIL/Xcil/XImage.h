/*
 * XImage
 */



#ifndef __XImage_h
#define __XImage_h



#include "../Image.h"
#include "../misc/typelib.h"
#include "../misc/memlib.h"
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>


/*
 * ��٥�����Τ���Υ�٥�θ³���
 * �����ۤ���ȿ����Ĥ�
 */
#define CIL_LABEL_LIMIT_MINUS -10000
#define CIL_LABEL_LIMIT_PLUS 10000


/*
 * �������Τ���δĶ��ѿ�
 */
#define CIL_GAMMA     "CIL_GAMMA"

/*
 * ����˾ܤ����ǥ����ץ쥤����������
 */
#define CIL_DP_DIR "CIL_DP_DIR"

/*
 * �̣գԤ�ľ�ܻ��ꤹ�롣
 */
extern long CIL_Setup_RGB_LUT;/* = 0 */
extern uchar3 CIL_RGB_LUT[256];

/*==========================================*
 * ���ꤵ�줿�ե����뤫��̣գԤ��ɤ߹��ࡣ
 *==========================================*/
long Read_RGB_LUT
  P2 ((uchar3, lut[]   ),
      (char *, filename))


/*
 * ���ӥåȥץ졼��Τ���Υ��顼�ޥå׾���
 *
 * PseudoColor
 */
#define SBASE 0
#define SMAX  16

#define MBASE (SBASE+SMAX)
#define MMAX  64

#define CBASE (MBASE+MMAX)
#define RMAX  6
#define GMAX  7
#define BMAX  4
#define CMAX  (RMAX * GMAX * BMAX)

#define FBASE (CBASE+CMAX)
#define FMAX  8



/*
 * For GrayScale
 */
#define GrayBase  SMAX
#define GrayMax   (256-SMAX-2)



extern uchar XLUT_PC_UChar3[ RMAX ][ GMAX ][ BMAX ];

extern uchar XLUT_PC_UChar[ 256 ];
extern uchar XLUT_GS_UChar[ 256 ];
extern uchar XLUT_SG_UChar[ 256 ];

extern uchar XLUT_PCR[ 256 ];
extern uchar XLUT_PCG[ 256 ];
extern uchar XLUT_PCB[ 256 ];
extern uchar XLUT_PCRerr[ RMAX ];
extern uchar XLUT_PCGerr[ GMAX ];
extern uchar XLUT_PCBerr[ BMAX ];

void XImageSetLookupTable();
void XImageSetErrorPorpagate();

extern uchar __XEPT_90[256];
extern uchar __XEPT_45[256];

/*
 * Setting RGB Plane Info for making pixel value
 */
void XImageSetRGBPlane(Display *display);
extern int R_Shift, G_Shift, B_Shift;
extern int R_LowBit, G_LowBit, B_LowBit;

/*
 * 65535 color setting
 */
#define R_SHIFT_16 11
#define G_SHIFT_16 5
#define B_SHIFT_16 0

#define R_NUM_16 32
#define G_NUM_16 64
#define B_NUM_16 32

#define R_MAX_16 31
#define G_MAX_16 63
#define B_MAX_16 31



void XImageSetColormap
(/*
   Display *display;
   Window w;
*/);



typedef XImage *(*XCreateImageFunc)
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);



/*
 * image ��¤�Τ�����Ȥ��ơ�
 */
void XShowImage
(/*
   Display *display,Window w,GC gc,image img,
   int win_x,int win_y,unsigned int win_xsize,unsigned int win_ysize,
   int img_x,int img_y,unsigned int img_xsize,unsigned int img_ysize
*/);

void XShowImagePartOfImage
(/*
   Display *display,Window w,GC gc,image img,
   int img_x,int img_y,unsigned int img_xsize,unsigned int img_ysize
*/);

void XShowImagePartOfWindow
(/*
   Display *display,Window w,GC gc,image img,
   int win_x,int win_y,unsigned int win_xsize,unsigned int win_ysize
*/);

void XShowImageFull
(/*
   Display *display,Window w,GC gc,image img
*/);



/*
 * ������ɥ��˲���������
 */
void XDrawImage
(/*
   Display *display,Window w,GC gc,long type,char **data,
   int win_x,int win_y,unsigned int win_xsize,unsigned int win_ysize,
   unsigned int img_xsize,unsigned int img_ysize
*/);



/*
 * XImage�����������������
 */
XImage *XCreateImageFromData
(/*
   Display *display,long win_xsize,long win_ysize,
   long type,char **data,long img_xsize,long img_ysize
*/);




/*
 * ���顼�ޥåפ�����
 */
void XImageSetColormap_8Planes_PseudoColor
(/*
   Display *display;
   Window w;
*/);



void XImageSetColormap_8Planes_GrayScale
(/*
   Display *display;
   Window w;
*/);



/*
 * ��٥�Υ��顼�ޥåפ�����
 */
uchar3 *XImageMakeLabelColormap
(/*
   long label_max;
   char *env;
*/);


/*
 * �ǥ����ץ쥤�Υץ졼�󤴤Ȥ�ʬ����
 */
XImage *XCreateImageFromData_32Planes
(/*
   Display *display,long win_xsize,long win_ysize,
    long type,char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_24Planes
(/*
   Display *display,long win_xsize,long win_ysize,
    long type,char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_16Planes
(/*
   Display *display,long win_xsize,long win_ysize,
    long type,char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_15Planes
(/*
   Display *display,long win_xsize,long win_ysize,
    long type,char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_8Planes
(/*
   Display *display,long win_xsize,long win_ysize,
   long type,char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_1Plane
(/*
   Display *display,long win_xsize,long win_ysize,
   long type,char **data,long img_xsize,long img_ysize
*/);



/*
 * ����˥ץ졼�󣸤ϥǥ����ץ쥤�Υӥ��奢�륿���פ�ʬ����
 */
XImage *XCreateImageFromData_8Planes_PseudoColor
(/*
   Display *display,long win_xsize,long win_ysize,
   long type,char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_8Planes_GrayScale
(/*
   Display *display,long win_xsize,long win_ysize,
   long type,char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_8Planes_StaticGray
(/*
   Display *display,long win_xsize,long win_ysize,
   long type,char **data,long img_xsize,long img_ysize
*/);



/*
 * �ץ졼��:�ǡ����η��ˤ�ä�ʬ����
 */
XImage *XCreateImageFromData_PackedBit1
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_Bit1
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_1Plane_Char
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_1Plane_Long
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_1Plane_UChar
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_1Plane_UChar3
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);



/*
 * �ץ졼��:�ǡ����η��ˤ�ä�ʬ����
 */
XImage *XCreateImageFromData_8Planes_PseudoColor_Char
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_8Planes_PseudoColor_Long
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_8Planes_PseudoColor_UChar
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_8Planes_PseudoColor_UChar3
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);



/*
 * �ץ졼��:�ǡ����η��ˤ�ä�ʬ����
 */
XImage *XCreateImageFromData_8Planes_GrayScale_Char
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_8Planes_GrayScale_Long
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_8Planes_GrayScale_UChar
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_8Planes_GrayScale_UChar3
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);



/*
 * �ץ졼��:�ǡ����η��ˤ�ä�ʬ����
 */
XImage *XCreateImageFromData_8Planes_StaticGray_Char
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_8Planes_StaticGray_Long
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_8Planes_StaticGray_UChar
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_8Planes_StaticGray_UChar3
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);



/*
 * �ץ졼�󣱣�:�ǡ����η��ˤ�ä�ʬ����
 */
XImage *XCreateImageFromData_16Planes_Char
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_16Planes_Long
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_16Planes_UChar
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_16Planes_UChar3
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);



/*
 * �ץ졼�󣱣�:�ǡ����η��ˤ�ä�ʬ����
 */
XImage *XCreateImageFromData_15Planes_Char
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_15Planes_Long
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_15Planes_UChar
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_15Planes_UChar3
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);



/*
 * �ץ졼�󣲣�:�ǡ����η��ˤ�ä�ʬ����
 */
XImage *XCreateImageFromData_24Planes_Char
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_24Planes_Long
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_24Planes_UChar
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);

XImage *XCreateImageFromData_24Planes_UChar3
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);


/*
 * �ץ졼��32:�ǡ����η��ˤ�ä�ʬ����
 */
XImage *XCreateImageFromData_32Planes_UChar3
(/*
   Display *display,long win_xsize,long win_ysize,
   char **data,long img_xsize,long img_ysize
*/);



/*
 * for data type char.
 */
char *XImageFontSet(/* char *name */);

long XImageFontGetXSize();
long XImageFontGetYSize();
long XImageFontGetXSpace();
long XImageFontGetYSpace();

void  XImageFontSetSpace(/* long xspace,long yspace */);

packedbit1 **XImageCharToPackedBit1
(/*
   char **src,long x,long y,long xsize,long ysize,
   			      long *ret_xsize,long *ret_ysize
*/);



#endif  __XImage_h
