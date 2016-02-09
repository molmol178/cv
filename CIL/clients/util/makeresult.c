/*
 * makeresult.c
 */



#include "Image.h"
#include "ImageFile.h"
#include "Image/Funcs.h"
#include "misc/optlib.h"



#define IX(n) ((XSize + XSpace) * (n % Column) + XSpace)
#define IY(n) ((YSize + YSpace) * (n / Column) + YSpace)



void add_result P ((image dest,long n,char *fname,long rev,long frame,long t));
void make_frame P ((image dest));
void image_rotation P ((image dest));



char *Output;
long Num;
long Row,Column;
long XSpace = 0,YSpace = 0;
long XSize = 256,YSize = 256;
long Portlate = 1;
long Frame = 0;
long Frame_intensity = 0;
long Reverse = 0;
long Theshold = 128;



static char *oplist[] = {
  "Paste result images.",
  "input:*:*:(image) name of the input image",
  "help:-h:print this message",
  "output:-o:1:(image) name of the output image",
  "space:-spc:1:0x0:(int*2) size of the image space <XSPACE>x<YSPACE>",
  "size:-sz:1:256x256:(int*2) size of the image box <XSIZE>x<XSIZE>",
  "column:-c:1:(int) number of the image column",
  "row:-r:1:(int) number of the image row",
  "reverse:-R:reverse image",
  "theshold:-t:1:128:(uchar) theshold and output binary image",
  "frame:-f:write frame",
  "frame-intensity:-F:1:0:(uchar) intensity of the frame",
  "landscape:-l:set landscape",
  "Followings are used by a long label image.",
  "certain:-C:1:255:(uchar) intensity of the certain regions [L>0]",
  "border:-B:1:0:(uchar) intensity of the borders between regions",
  "uncertain:-U:1:0:(uchar) intensity of the uncertain regions [L=0]",
  "previous:-P:1:(uchar) intensity of the previous regions [L<0]",
};



void initialize(argc,argv)
     int argc;
     char *argv[];
{
  long n;
  char *head[4];

  OPTION_SETUP( oplist );

  if ( optspecified("help") ) optmanual( 1 );
  if ( argc == 1 ) optusage( 1 );

  Num = optvaluenum( "input" );
  if ( optspecified("space") )
    {
      n = strsplit( optvalue("space"), head, "x" );
      XSpace = atol( head[ 0 ] );
      YSpace = atol( head[ 1 ] );
    }
  if ( optspecified("size") )
    {
      n = strsplit( optvalue("size"), head, "x" );
      XSize = atol( head[ 0 ] );
      YSize = atol( head[ 1 ] );
    }
  else
    {
      if ( Num == 1 )
	{
	  image tmp = Image.createFromFilename( "IMAGE", optvalue("input") );
	  XSize = __XSIZE( tmp );
	  YSize = __YSIZE( tmp );
	  Image.destroy( tmp );
	}
    }

  Reverse  = optspecified( "reverse" );
  Portlate = ( ! optspecified( "landscape" ) );
  Frame  = optspecified( "frame" );
  Theshold = optvalueint( "theshold" );
  Frame_intensity = optvalueint( "frame-intensity" );
  if ( optspecified( "column" ) ) Column = optvalueint("column");
  if ( optspecified( "row"    ) ) Row = optvalueint("row");

  if ( Num == 1 )
    {
      Column = 1;
    }
  else
    {
      while ( ( Column == 0 ) && ( Row == 0 ) )
	{
	  optinput( "column" );
	  if ( optspecified( "column" ) ) Column = optvalueint("column");
	  if ( Column == 0 )
	    {
	      optinput( "row" );
	      if ( optspecified( "row" ) ) Row = optvalueint("row");
	    }
	}
    }

  if ( Column == 0 ) Column = ( Num + Row    - 1 ) / Row;
  if ( Row    == 0 ) Row    = ( Num + Column - 1 ) / Column;

  fprintf( stderr, "Column = %d, Row = %d\n", Column, Row );

  Output = optvalue("output");
}



void main(argc,argv)
     int argc;
     char *argv[];
{
  image result;
  int i;
  long xsize, ysize;
  char **filelist;

  initialize( argc, argv );

  filelist = optvaluelist("input");
  xsize  = (XSize + XSpace) * Column + XSpace;
  ysize  = (YSize + YSpace) * Row    + YSpace;
  result = Image.createMake( "RES", UChar, xsize, ysize );
  image__reverse(result,result);

  for ( i = 0; i < Num; i++ )
    {
      add_result( result, i, filelist[i], Reverse, Frame, 0 );
    }

  if ( Portlate == 0 ) image_rotation( result );

  if ( optspecified( "theshold" ) )
    {
      image org = Image.createFromImage("ORG",result);
      image__thresholding( result, org, Theshold );
      image__reverse( result, result );
      Image.destroy( org );
    }

  ImageFile.setSaveFormat( "PNM" );
  Image.save( result, Output, 0 );

  Image.destroy( result );
}



void to_uchar P1 ((image, dest))

void add_result(dest,n,fname,rev,frame,t)
     image dest;
     long n;
     char *fname;
     long rev,frame,t;
{
  image img, org;

  img = Image.create("SRC");
  org = Image.createFromFilename("ORG",fname);
  if (org == 0)
    {
      fprintf(stderr,"error:can't open file (%s)\n",fname);
      exit(-1);
    }
  Image.copy( img, org );
  Image.destroy(org);

  if (Image.type(img) != UChar) to_uchar(img);
  if (rev)   image__reverse(img,img);
  if (t)     image__liner(img,img);
  if (frame) make_frame(img);
  fprintf(stderr,"%s -> (%d,%d,%d,%d)\n",
	  fname,IX(n),IY(n),Image.xsize(img),Image.ysize(img));
  image__copyarea(dest,img,IX(n),IY(n),0,0,Image.xsize(img),Image.ysize(img));
  Image.destroy(img);
}



void bit1_to_gif   P ((image dest,image src));
void bit4_to_gif   P ((image dest,image src));
void uchar_to_gif  P ((image dest,image src));
void char_to_gif   P ((image dest,image src));
void uchar3_to_gif P ((image dest,image src));
void short_to_gif  P ((image dest,image src));
void short2_to_gif P ((image dest,image src));
void ushort_to_gif P ((image dest,image src));
void long_to_gif   P ((image dest,image src));
void float_to_gif  P ((image dest,image src));



typedef void (*f_caller) P ((image dest,image src));

struct {
  long type;
  f_caller caller;
} table[] = {
  {Bit1,bit1_to_gif},
  {Bit4,bit4_to_gif},
  {UChar,uchar_to_gif},
  {Char,char_to_gif},
  {UChar3,uchar3_to_gif},
  {Short,short_to_gif},
  {Short2,short2_to_gif},
  {Float,float_to_gif},
  {UShort,ushort_to_gif},
  {Long,long_to_gif}
};



void to_uchar(dest)
     image dest;
{
  long type;
  image src;
  long i;

  type = Image.type(dest);
  for (i = 0; i < Number(table); i++)
    if (table[i].type == type) break;

  if ( i == Number( table ) )
    {
      fprintf(stderr,"error:image type is wrong (%s)\n",typename(type));
      exit(-1);
    }

  src = Image.createFromImage("src",dest);
  Image.make(dest,UChar,Image.xsize(src),Image.ysize(src));
  table[i].caller(dest,src);
  Image.destroy(src);
}



void bit1_to_gif(dest,src)
     image dest;
     image src;
{
  register long x,y;
  register uchar **s,**d;
  long xsize,ysize;
  long dest_xsize,dest_ysize;
  long sx,sy;
  long xx,yy;

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  dest_xsize = Image.xsize(dest);
  dest_ysize = Image.ysize(dest);
  d = (uchar **)Image.data(dest);
  s = (uchar **)Image.data(src);

  for (yy = sy = y = 0; y < dest_ysize; y++)
    {
      yy = sy / dest_ysize;
      for (xx = sx = x = 0; x < dest_xsize; x++)
	{
	  xx = sx / dest_xsize;
	  d[y][x] = (s[yy][xx] == 0) ? 255 : 0;
	  sx += xsize;
	}
      sy += ysize;
    }
}



void bit4_to_gif(dest,src)
     image dest;
     image src;
{
  register long x,y;
  register uchar **s,**d;
  long xsize,ysize;
  long dest_xsize,dest_ysize;
  long sx,sy;
  long xx,yy;

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  dest_xsize = Image.xsize(dest);
  dest_ysize = Image.ysize(dest);
  d = (uchar **)Image.data(dest);
  s = (uchar **)Image.data(src);

  for (yy = sy = y = 0; y < dest_ysize; y++)
    {
      yy = sy / dest_ysize;
      for (xx = sx = x = 0; x < dest_xsize; x++)
	{
	  xx = sx / dest_xsize;
	  d[y][x] = ((~s[yy][xx]) & 0x3) << 6;
	  sx += xsize;
	}
      sy += ysize;
    }
}



void uchar_to_gif(dest,src)
     image dest;
     image src;
{
  register long x,y;
  register uchar **s,**d;
  long xsize,ysize;
  long dest_xsize,dest_ysize;
  long sx,sy;
  long xx,yy;

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  dest_xsize = Image.xsize(dest);
  dest_ysize = Image.ysize(dest);
  d = (uchar **)Image.data(dest);
  s = (uchar **)Image.data(src);

  for (yy = sy = y = 0; y < dest_ysize; y++)
    {
      yy = sy / dest_ysize;
      for (xx = sx = x = 0; x < dest_xsize; x++)
	{
	  xx = sx / dest_xsize;
	  d[y][x] = s[yy][xx];
	  sx += xsize;
	}
      sy += ysize;
    }
}



void uchar3_to_gif(dest,src)
     image dest;
     image src;
{
  register long x,y;
  register uchar3 **s;
  register uchar  **d;
  long xsize,ysize;
  long dest_xsize,dest_ysize;
  long sx,sy;
  long xx,yy;

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  dest_xsize = Image.xsize(dest);
  dest_ysize = Image.ysize(dest);
  d = (uchar  **)Image.data(dest);
  s = (uchar3 **)Image.data(src);

  for (yy = sy = y = 0; y < dest_ysize; y++)
    {
      yy = sy / dest_ysize;
      for (xx = sx = x = 0; x < dest_xsize; x++)
	{
	  xx = sx / dest_xsize;
	  d[y][x] = (0.299 * s[yy][xx].at[0] +
		     0.587 * s[yy][xx].at[1] +
		     0.114 * s[yy][xx].at[2]);
	  sx += xsize;
	}
      sy += ysize;
    }
}



void short_to_gif(dest,src)
     image dest;
     image src;
{
  register long x,y;
  register uchar **d;
  register short **s;
  long xsize,ysize;
  long dest_xsize,dest_ysize;
  long sx,sy;
  long xx,yy;
  long max,min,range;

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  dest_xsize = Image.xsize(dest);
  dest_ysize = Image.ysize(dest);
  d = (uchar **)Image.data(dest);
  s = (short **)Image.data(src);

  max = min = s[0][0];
  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      if (max < s[y][x]) max = s[y][x];
      else
      if (min > s[y][x]) min = s[y][x];
  range = max - min + 1;

  for (yy = sy = y = 0; y < dest_ysize; y++)
    {
      yy = sy / dest_ysize;
      for (xx = sx = x = 0; x < dest_xsize; x++)
	{
	  xx = sx / dest_xsize;
	  d[y][x] = 256 * (s[yy][xx] - min) / range;
	  sx += xsize;
	}
      sy += ysize;
    }
}



void ushort_to_gif(dest,src)
     image dest;
     image src;
{
  register ushort **s;
  register uchar **d;
  register int x,y;
  long dest_xsize,dest_ysize;
  long px,py;
  long sx,sy;
  long xx,yy;
  long xsize,ysize;

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  dest_xsize = Image.xsize(dest);
  dest_ysize = Image.ysize(dest);
  s = (ushort **)Image.data(src);
  d = (uchar **)Image.data(dest);

  for (yy = sy = y = 0; y < dest_ysize; y++)
    {
      py = yy;
      yy = sy / dest_ysize;
      for (xx = sx = x = 0; x < dest_xsize; x++)
	{
	  px = xx;
	  xx = sx / dest_xsize;
	  if (s[yy][px] != s[yy][xx] || s[py][xx] != s[yy][xx] ||
	      s[py][px] != s[yy][xx] ||
	      (x == 0 || y == 0 || x == (dest_xsize-1) || y == (dest_ysize-1)))
	    d[y][x] = 0;
	  else
	    d[y][x] = 255;
	  sx += xsize;
	}
      sy += ysize;
    }
}



void long_to_gif(dest,src)
     image dest;
     image src;
{
  register long **s;
  register uchar **d;
  register int x,y;
  long dest_xsize,dest_ysize;
  long px,py;
  long sx,sy;
  long xx,yy;
  long xsize,ysize;
  long uncertain_intensity;
  long certain_intensity;
  long border_intensity;
  long previous_intensity = -1;

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  dest_xsize = Image.xsize(dest);
  dest_ysize = Image.ysize(dest);
  s = (long **)Image.data(src);
  d = (uchar **)Image.data(dest);

  uncertain_intensity = optvalueint( "uncertain" );
  certain_intensity   = optvalueint( "certain" );
  border_intensity    = optvalueint( "border" );
  if ( optspecified( "previous" ) )
    previous_intensity = optvalueint( "previous" );

  for (yy = sy = y = 0; y < dest_ysize; y++)
    {
      py = yy;
      yy = sy / dest_ysize;
      for (xx = sx = x = 0; x < dest_xsize; x++)
	{
	  px = xx;
	  xx = sx / dest_xsize;

	  if ((s[yy][px] != s[yy][xx]) ||
	      (s[py][xx] != s[yy][xx]) ||
	      (s[py][px] != s[yy][xx]) ||
	      (x == 0 || y == 0 || x == (dest_xsize-1) || y == (dest_ysize-1)))
	    {
	      d[y][x] = border_intensity;
	    }
	  else
	  if (s[yy][xx] == 0)
	    {
	      d[y][x] = uncertain_intensity;
	    }
	  else
	  if ( s[yy][xx] > 0 )
	    {
	      d[y][x] = certain_intensity;
	    }
	  if ( s[yy][xx] < 0 )
	    {
	      if ( previous_intensity == -1 )
		{
		  if ((( y + x ) % 2 ) == 0 )
		    d[y][x] = 0;
		  else
		    d[y][x] = 255;
		}
	      else
		d[y][x] = previous_intensity;
	    }
	  sx += xsize;
	}
      sy += ysize;
    }
}



void float_to_gif(dest,src)
     image dest;
     image src;
{
  register long x,y;
  register uchar **d;
  register float **s;
  long xsize,ysize;
  long dest_xsize,dest_ysize;
  long sx,sy;
  long xx,yy;
  float max,min,range;

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  dest_xsize = Image.xsize(dest);
  dest_ysize = Image.ysize(dest);
  d = (uchar **)Image.data(dest);
  s = (float **)Image.data(src);

  max = min = s[0][0];
  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      if (max < s[y][x]) max = s[y][x];
      else
      if (min > s[y][x]) min = s[y][x];
  range = max - min;

  for (yy = sy = y = 0; y < dest_ysize; y++)
    {
      yy = sy / dest_ysize;
      for (xx = sx = x = 0; x < dest_xsize; x++)
	{
	  xx = sx / dest_xsize;
	  d[y][x] = 255 * (s[yy][xx] - min) / range;
	  sx += xsize;
	}
      sy += ysize;
    }
}



void short2_to_gif(dest,src)
     image dest;
     image src;
{
  register long x,y;
  register uchar **d;
  register short2 **s;
  long xsize,ysize;
  long dest_xsize,dest_ysize;
  long sx,sy;
  long xx,yy;
  float max,min,range;

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  dest_xsize = Image.xsize(dest);
  dest_ysize = Image.ysize(dest);
  d = (uchar  **)Image.data(dest);
  s = (short2 **)Image.data(src);

  max = min = s[0][0].at[0];
  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      {
	long v;
	if (abs(s[y][x].at[0]) > abs(s[y][x].at[1]))
	  v = s[y][x].at[0];
	else v = s[y][x].at[1];
	if (max < v) max = v;
	else
	if (min > v) min = v;
	s[y][x].at[0] = v;
      }
  range = max - min + 1;

  for (yy = sy = y = 0; y < dest_ysize; y++)
    {
      yy = sy / dest_ysize;
      for (xx = sx = x = 0; x < dest_xsize; x++)
	{
	  xx = sx / dest_xsize;
	  if (s[yy][xx].at[0] >= 0)
	    d[y][x] = 128 * s[yy][xx].at[0] / max + 127;
	  else
	    d[y][x] = 128 - 128 * s[yy][xx].at[0] / min;
	  sx += xsize;
	}
      sy += ysize;
    }
}




void char_to_gif(dest,src)
     image dest;
     image src;
{
  register long x,y;
  register uchar **d;
  register uchar **s;
  long xsize,ysize;
  long dest_xsize,dest_ysize;
  long sx,sy;
  long xx,yy;
  long max,min,range;

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  dest_xsize = Image.xsize(dest);
  dest_ysize = Image.ysize(dest);
  d = (uchar **)Image.data(dest);
  s = (uchar **)Image.data(src);

  max = min = s[0][0];
  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      if (max < s[y][x]) max = s[y][x];
      else
      if (min > s[y][x]) min = s[y][x];
  range = max - min + 1;

  for (yy = sy = y = 0; y < dest_ysize; y++)
    {
      yy = sy / dest_ysize;
      for (xx = sx = x = 0; x < dest_xsize; x++)
	{
	  xx = sx / dest_xsize;
	  d[y][x] = 256 * (s[yy][xx] - min) / range;
	  sx += xsize;
	}
      sy += ysize;
    }
}



void make_frame(img)
     image img;
{
  register long x,y;
  register uchar **d;
  long xsize,ysize;

  xsize = Image.xsize(img);
  ysize = Image.ysize(img);
  d = (uchar **)Image.data(img);

  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      if ((x == 0) || (x == (xsize-1)) || (y == 0) || (y == (ysize-1)))
	d[y][x] = Frame_intensity;
}



void image_rotation(img)
     image img;
{
  register long x,y;
  register uchar **d;
  register uchar **s;
  long xsize,ysize;
  image src;

  src = Image.createFromImage("src",img);
  xsize = Image.xsize(img);
  ysize = Image.ysize(img);
  Image.make(img,UChar,ysize,xsize);

  d = (uchar **)Image.data(img);
  s = (uchar **)Image.data(src);

  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      d[xsize-1 - x][y] = s[y][x];

  Image.destroy(src);
}
