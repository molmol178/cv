/*
 * filename : mkepsf.c
 * author   : Takahiro Sugiyama
 * date     : February 1994
 * update   : Tuesday February 15 1994 by Takahiro Sugiyama
 *
 */



#include "Image.h"
#include "ImageFile.h"
#include "Xcil/XImage.h"
#include "Image/Funcs.h"
#include "misc/optlib.h"
#include "misc/strlib.h"
#include <stdio.h>
#include <stdlib.h>



void make_frame P ((image dest));



static char *option[] = {
  "<:Transform an image in other formats to EPSF file.",

  "input:*:1:(image) name of the input image",
  "output:*:1:(epsf) name of the output epsf file",

  "help:-h:print this message",
  "size:-s:1:(size/scale) image size [<XSIZE>x<YSIZE>] / [<SCALE>]",
  "threshold:-t:1:128:(uchar) threshold and output a binary image",
  "reverse:-R:reverse image",
  "frame:-f:write frame",
  "frame-intensity:-F:1:0:(uchar) intensity of the frames",

  "Followings are used by the long label image.",
  "border-superior:-b:borders superior to uncertain regions",
  "border:-B:1:0:(uchar) intensity of the borders between regions",
  "certain:-C:1:255:(uchar) intensity of the certain regions [L>0]",
  "uncertain:-U:1:150:(uchar) intensity of the uncertain regions [L=0]",
  "previous:-P:1:230:(uchar) intensity of the previous regions [L<0]",

  "Followings are used by the ushort or short image.",
  "ushort-is-bindary:-ub:ushort image is binary image",
  "ushort-is-long:-ul:ushort image is long-label image",
  "short-is-bindary:-sb:short image is binary image",
  "short-is-long:-sl:short image is long-label image",

  "Followings are used by the char image.",
  "is-text:-txt:bit1, bit4, char and uchar image is text image",
  "char-font:-cf:1:g5:(font) font of char image",
  "12>:Gothic .................. g2, g3, g4, g5, g6, g7, g8",
  "12>:Feature ................. f7, f8, f9, 10",
  "12>:Four Directions ......... dir4-3, dir4-5, dir4-7",
  "12>:Eight Directions ........ dir8-5, dir8-7, dir8-9",
  "12>:Sixteen Directions ...... dir16-9",
  "12>:Thirty-Two Directions ... dir32-17",
  "char-font-space:-cfs:2:1:1:(int) x,y space of char font",
};



void to_epsf P1 ((image, dest))



void main( argc, argv )
     int argc;
     char *argv[];
{
  image epsf, share;
  char *input = "-";
  char *output = "-";

  OPTION_SETUP( option );

  if ( optspecified( "help" ) ) optmanual( 1 );
  if ( argc == 1 ) optusage( 1 );

  input  = optvalue( "input"  );
  output = optvalue( "output" );

  epsf = Image.create( "EPSF" );
  share = Image.createFromFilename( "SHARE", input );
  if ( share == 0 )
    {
      fprintf( stderr, "error:can't open file (%s)\n", input );
      exit( -1 );
    }
  Image.copy( epsf, share );
  Image.destroy( share );

  if ((__TYPE(epsf) == UShort) && optspecified("ushort-is-long"))
    image__long_type(epsf,epsf);
  if ((__TYPE(epsf) == Short) && optspecified("short-is-long"))
    image__long_type(epsf,epsf);
  to_epsf( epsf );
  if ( optspecified( "threshold" ) )
    {
      image original = Image.createFromImage("ORG",epsf);
      double threshold = optvaluefloat( "threshold" );
      if ( __TYPE( original ) != Bit1 )
	{
	  image__thresholding( epsf, original, threshold );
	}
      Image.destroy( original );
    }

  if ( optspecified( "reverse" )  ) image__reverse( epsf, epsf );
  if ( optspecified( "frame" )    ) make_frame( epsf );

  ImageFile.setSaveFormat( "PS" );
  Image.save( epsf, output, 0 );

  Image.destroy( epsf );
}



void bit1_to_epsf   P ((image dest,image src));
void bit4_to_epsf   P ((image dest,image src));
void uchar_to_epsf  P ((image dest,image src));
void uchar3_to_epsf P ((image dest,image src));

void char_to_epsf   P ((image dest,image src));
void short_to_epsf  P ((image dest,image src));
void short2_to_epsf P ((image dest,image src));

void ushort_to_epsf P ((image dest,image src));
void long_to_epsf   P ((image dest,image src));
void float_to_epsf  P ((image dest,image src));



typedef void (*f_caller) P ((image dest,image src));

struct {
  long type;
  f_caller caller;
} table[] = {
  {Bit1,bit1_to_epsf},
  {Bit4,bit4_to_epsf},
  {UChar,uchar_to_epsf},
  {Char,char_to_epsf},
  {UChar3,uchar3_to_epsf},
  {Short,short_to_epsf},
  {Short2,short2_to_epsf},
  {Float,float_to_epsf},
  {UShort,ushort_to_epsf},
  {Long,long_to_epsf}
};



int Xsize, Ysize;



static void __check_size( img )
     image img;
{
  long n;
  char buf[ 256 ];
  char *head[4];

  Xsize = __XSIZE( img );
  Ysize = __YSIZE( img );

  if ( optspecified( "size" ) )
    {
      strcopy( buf, optvalue( "size" ) );
      n = strsplit( buf, head, "x" );
      if ( n == 1 )
	{
	  double scale = atof( head[ 0 ] );
	  Xsize = Xsize * scale;
	  Ysize = Ysize * scale;
	}
      else
      if ( n == 2 )
	{
	  Xsize = atol( head[ 0 ] );
	  Ysize = atol( head[ 1 ] );
	}
      else
	{
	  fprintf( stderr, "waring:bad option value (%s) of \"size\".\n",
		   optvalue( "size" ) );
	}
    }
}



void to_epsf( dest )
     image dest;
{
  long type;
  image src;
  long i;

  type = __TYPE( dest );
  for ( i = 0; i < Number( table ); i++ )
    if ( table[ i ].type == type ) break;

  if ( i == Number( table ) )
    {
      fprintf( stderr, "error:image type is wrong (%s)\n", typename( type ) );
      exit(-1);
    }

  __check_size( dest );
  src = Image.createFromImage( "src", dest );
  table[ i ].caller( dest, src );
  Image.destroy( src );
}


void char_to_text_epsf(dest, src)
     image dest;
     image src;
{
  long xsize, ysize;
  long xsize_dest, ysize_dest;
  long xspace, yspace;

  XImageFontSet(optvalue("char-font"));
  xspace = optnvalueint("char-font-space",0);
  yspace = optnvalueint("char-font-space",1);
  XImageFontSetSpace(xspace, yspace);

  Image.free(dest);
  dest->type = PackedBit1;
  dest->data = (char**)XImageCharToPackedBit1(src->data,
					      src->xsize,
					      src->ysize,
					      &xsize_dest,
					      &ysize_dest);
  dest->xsize = xsize_dest;
  dest->ysize = ysize_dest;
}


void bit1_to_epsf(dest,src)
     image dest;
     image src;
{
  register long x, y;
  register uchar **s,**d;
  long xsize,ysize;
  long sx,sy;
  long xx,yy;

  if (optspecified("is-text"))
    {
      char_to_text_epsf(dest, src);
      return;
    }

  xsize = __XSIZE( src );
  ysize = __YSIZE( src );
  Image.make( dest, Bit1, Xsize, Ysize );
  d = __DATA( dest, bit1 );
  s = __DATA( src , bit1 );

  for ( yy = sy = y = 0; y < Ysize; y++ )
    {
      yy = sy / Ysize;
      for ( xx = sx = x = 0; x < Xsize; x++ )
	{
	  xx = sx / Xsize;
	  d[ y ][ x ] = s[ yy ][ xx ];
	  sx += xsize;
	}
      sy += ysize;
    }
}



void bit4_to_epsf(dest,src)
     image dest;
     image src;
{
  register long x, y;
  register uchar **s, **d;
  long xsize, ysize;
  long sx, sy;
  long xx, yy;

  if (optspecified("is-text"))
    {
      char_to_text_epsf(dest, src);
      return;
    }

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  Image.make( dest, UChar, Xsize, Ysize );
  d = __DATA( dest, uchar );
  s = __DATA( src , uchar );

  for ( yy = sy = y = 0; y < Ysize; y++ )
    {
      yy = sy / Ysize;
      for ( xx = sx = x = 0; x < Xsize; x++ )
	{
	  xx = sx / Xsize;
	  d[ y ][ x ] = ((~s[yy][xx]) & 0x3) << 6;
	  sx += xsize;
	}
      sy += ysize;
    }
}



void uchar_to_epsf(dest,src)
     image dest;
     image src;
{
  register long x,y;
  register uchar **s,**d;
  long xsize,ysize;
  long sx,sy;
  long xx,yy;

  if (optspecified("is-text"))
    {
      char_to_text_epsf(dest, src);
      return;
    }

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  Image.make( dest, UChar, Xsize, Ysize );
  d = __DATA( dest, uchar );
  s = __DATA( src , uchar );

  for ( yy = sy = y = 0; y < Ysize; y++ )
    {
      yy = sy / Ysize;
      for ( xx = sx = x = 0; x < Xsize; x++ )
	{
	  xx = sx / Xsize;
	  d[ y ][ x ] = s[ yy ][ xx ];
	  sx += xsize;
	}
      sy += ysize;
    }
}



void uchar3_to_epsf( dest, src )
     image dest;
     image src;
{
  register long x,y;
  register uchar3 **s;
  register uchar **d;
  long xsize,ysize;
  long sx,sy;
  long xx,yy;

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  Image.make( dest, UChar, Xsize, Ysize );
  d = __DATA( dest, uchar );
  s = __DATA( src , uchar3 );

  for ( yy = sy = y = 0; y < Ysize; y++ )
    {
      yy = sy / Ysize;
      for ( xx = sx = x = 0; x < Xsize; x++ )
	{
	  xx = sx / Xsize;
	  d[ y ][ x ] = (0.299*s[yy][xx].at[0]+
			 0.587*s[yy][xx].at[1]+
			 0.114*s[yy][xx].at[2]);
	  sx += xsize;
	}
      sy += ysize;
    }
}



void short_to_epsf(dest,src)
     image dest;
     image src;
{
  register long x,y;
  register uchar **d;
  register short **s;
  long xsize,ysize;
  long sx,sy;
  long xx,yy;
  long max,min,range;

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  Image.make( dest, UChar, Xsize, Ysize );
  d = __DATA( dest, uchar );
  s = __DATA( src , short );

  max = min = s[0][0];
  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      if (max < s[y][x]) max = s[y][x];
      else
      if (min > s[y][x]) min = s[y][x];
  range = max - min + 1;

  for (yy = sy = y = 0; y < Ysize; y++)
    {
      yy = sy / Ysize;
      for (xx = sx = x = 0; x < Xsize; x++)
	{
	  xx = sx / Xsize;
	  d[y][x] = 256 * (s[yy][xx] - min) / range;
	  sx += xsize;
	}
      sy += ysize;
    }
}



void ushort_binary_to_epsf();

void ushort_to_epsf(dest,src)
     image dest;
     image src;
{
  register ushort **s;
  register bit1   **d;
  register int x, y;
  long px, py;
  long sx, sy;
  long xx, yy;
  long xsize, ysize;

  if ( optspecified( "ushort-is-bindary" ) )
    {
      ushort_binary_to_epsf(dest,src);
      return;
    }

  xsize = __XSIZE( src );
  ysize = __YSIZE( src );
  Image.make( dest, Bit1, Xsize, Ysize );
  d = __DATA( dest, bit1 );
  s = __DATA( src , ushort );

  for ( yy = sy = y = 0; y < Ysize; y++ )
    {
      py = yy;
      yy = sy / Ysize;
      for ( xx = sx = x = 0; x < Xsize; x++ )
	{
	  px = xx;
	  xx = sx / Xsize;
	  if ( s[ yy ][ px ] != s[ yy ][ xx ] ||
	       s[ py ][ xx ] != s[ yy ][ xx ] ||
	       s[ py ][ px ] != s[ yy ][ xx ] )
	    d[ y ][ x ] = 1;
	  else
	    d[ y ][ x ] = 0;
	  sx += xsize;
	}
      sy += ysize;
    }
}



void ushort_binary_to_epsf(dest,src)
     image dest;
     image src;
{
  register ushort **s;
  register bit1   **d;
  register int x, y;
  long sx, sy;
  long xx, yy;
  long xsize, ysize;

  xsize = __XSIZE( src );
  ysize = __YSIZE( src );
  Image.make( dest, Bit1, Xsize, Ysize );
  d = __DATA( dest, bit1 );
  s = __DATA( src , ushort );

  for ( yy = sy = y = 0; y < Ysize; y++ )
    {
      yy = sy / Ysize;
      for ( xx = sx = x = 0; x < Xsize; x++ )
	{
	  xx = sx / Xsize;
	  d[ y ][ x ] = ( s[ yy ][ xx ] ) ? 1 : 0;
	  sx += xsize;
	}
      sy += ysize;
    }
}



void long_to_epsf(dest,src)
     image dest;
     image src;
{
  register long  **s;
  register uchar **d;
  register int x,y;
  long px,py;
  long sx,sy;
  long xx,yy;
  long xsize,ysize;

  long uncertain_intensity;
  long certain_intensity;
  long border_intensity;
  long previous_intensity = -1;
  long border_superior = 0;

  xsize = __XSIZE( src );
  ysize = __YSIZE( src );
  Image.make( dest, UChar, Xsize, Ysize );
  d = __DATA( dest, uchar );
  s = __DATA( src , long );

  uncertain_intensity = optvalueint( "uncertain" );
  certain_intensity   = optvalueint( "certain" );
  border_intensity    = optvalueint( "border" );
  previous_intensity  = optvalueint( "previous" );
  border_superior     = optspecified( "border-superior" );

  for (yy = sy = y = 0; y < Ysize; y++)
    {
      py = yy;
      yy = sy / Ysize;
      for ( xx = sx = x = 0; x < Xsize; x++ )
	{
	  px = xx;
	  xx = sx / Xsize;

	  if ( border_superior == 0 )
	    {
	      if (s[yy][xx] == 0)
		{
		  d[y][x] = uncertain_intensity;
		}
	      else
	      if ((( s[ yy ][ px ] != 0 ) && ( s[ yy ][ px ] != s[ yy ][ xx ] )) ||
		  (( s[ py ][ xx ] != 0 ) && ( s[ py ][ xx ] != s[ yy ][ xx ] )) ||
		  (( s[ py ][ px ] != 0 ) && ( s[ py ][ px ] != s[ yy ][ xx ] )))
		{
		  d[y][x] = border_intensity;
		}
	      else
	      if ( s[yy][xx] > 0 )
		{
		  d[y][x] = certain_intensity;
		}
	      else
	      if ( s[yy][xx] < 0 )
		{
		  d[y][x] = previous_intensity;
		}
	    }
	  else
	    {
	      if ( ( s[ yy ][ px ] != s[ yy ][ xx ] ) ||
		   ( s[ py ][ xx ] != s[ yy ][ xx ] ) ||
		   ( s[ py ][ px ] != s[ yy ][ xx ] ) )
		{
		  d[y][x] = border_intensity;
		}
	      else
	      if ( s[yy][xx] == 0 )
		{
		  d[y][x] = uncertain_intensity;
		}
	      else
	      if ( s[yy][xx] > 0 )
		{
		  d[y][x] = certain_intensity;
		}
	      else
	      if ( s[yy][xx] < 0 )
		{
		  d[y][x] = previous_intensity;
		}
	    }
	  sx += xsize;
	}
      sy += ysize;
    }
}



void float_to_epsf(dest,src)
     image dest;
     image src;
{
  register long x,y;
  register uchar **d;
  register float **s;
  long xsize,ysize;
  long sx,sy;
  long xx,yy;
  float max,min,range;

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  Image.make( dest, UChar, Xsize, Ysize );
  d = (uchar **)Image.data(dest);
  s = (float **)Image.data(src);

  max = min = s[0][0];
  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      if (max < s[y][x]) max = s[y][x];
      else
      if (min > s[y][x]) min = s[y][x];
  range = max - min;

  for (yy = sy = y = 0; y < Ysize; y++)
    {
      yy = sy / Ysize;
      for (xx = sx = x = 0; x < Xsize; x++)
	{
	  xx = sx / Xsize;
	  d[y][x] = 255 * (s[yy][xx] - min) / range;
	  sx += xsize;
	}
      sy += ysize;
    }
}



void short2_to_epsf(dest,src)
     image dest;
     image src;
{
  register long x,y;
  register uchar **d;
  register short2 **s;
  long xsize,ysize;
  long Xsize,Ysize;
  long sx,sy;
  long xx,yy;
  float max,min,range;

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  Xsize = Image.xsize(dest);
  Ysize = Image.ysize(dest);
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

  for (yy = sy = y = 0; y < Ysize; y++)
    {
      yy = sy / Ysize;
      for (xx = sx = x = 0; x < Xsize; x++)
	{
	  xx = sx / Xsize;
	  if (s[yy][xx].at[0] >= 0)
	    d[y][x] = 128 * s[yy][xx].at[0] / max + 127;
	  else
	    d[y][x] = 128 - 128 * s[yy][xx].at[0] / min;
	  sx += xsize;
	}
      sy += ysize;
    }
}


void char_to_epsf(dest,src)
     image dest;
     image src;
{
  register long x,y;
  register uchar **d;
  register uchar **s;
  long xsize,ysize;
  long Xsize,Ysize;
  long sx,sy;
  long xx,yy;
  long max,min,range;

  if (optspecified("is-text"))
    {
      char_to_text_epsf(dest, src);
      return;
    }

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  Xsize = Image.xsize(dest);
  Ysize = Image.ysize(dest);
  d = (uchar **)Image.data(dest);
  s = (uchar **)Image.data(src);

  max = min = s[0][0];
  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      if (max < (long)s[y][x]) max = s[y][x];
      else
      if (min > (long)s[y][x]) min = s[y][x];
  range = max - min + 1;

  for (yy = sy = y = 0; y < Ysize; y++)
    {
      yy = sy / Ysize;
      for (xx = sx = x = 0; x < Xsize; x++)
	{
	  xx = sx / Xsize;
	  d[y][x] = 256 * ((long)s[yy][xx] - min) / range;
	  sx += xsize;
	}
      sy += ysize;
    }
}



void make_frame( img )
     image img;
{
  register long x, y, xsize, ysize;
  int frame_intensity;
  register uchar **d;

  xsize = Image.xsize(img);
  ysize = Image.ysize(img);
  d = (uchar **)Image.data(img);

  frame_intensity = optvalueint( "frame-intensity" );
  fprintf( stderr, "mkepsf:write frame (%d)\n", frame_intensity );

  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      if ((x == 0) || (x == (xsize-1)) || (y == 0) || (y == (ysize-1)))
	d[y][x] = frame_intensity;
}
