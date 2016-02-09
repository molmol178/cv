/*
 * Any Image file format to UChar or UChar3
 * any2j4
 * Usage : any2j4 [input] [output]
 * Bit1   -> Bit1
 * Bit4   -> Bit1
 * UChar  -> UChar
 * UChar3 -> UChar3
 * Float  -> UChar
 * Short  -> UChar
 * Short2 -> UChar3
 * Short3 -> UChar3
 * UShort -> Bit1 (Label Image)
 * Long   -> Bit1 (Label Image)
 */



#include "Image.h"
#include "ImageFile.h"



void float_to_j4  P ((image dest,image src));
void short_to_j4  P ((image dest,image src));
void short2_to_j4 P ((image dest,image src));
void short3_to_j4 P ((image dest,image src));
void ushort_to_j4 P ((image dest,image src));
void long_to_j4   P ((image dest,image src));



typedef void (*f_caller) P ((image dest,image src));

typedef struct {
  long type;
  long new_type;
  f_caller caller;
} tableRec;

tableRec table[] = {
  {Bit1,  Bit1,  0},
  {Bit4,  Bit4,  0},
  {UChar, UChar, 0},
  {UChar3,UChar3,0},

  {Float, UChar, float_to_j4},
  {Short, UChar, short_to_j4},
  {Short2,UChar3,short2_to_j4},
  {Short3,UChar3,short3_to_j4},
  {UShort,Bit1,  ushort_to_j4},
  {Long,  Bit1,  long_to_j4}
};



void main(argc,argv)
     int argc;
     char *argv[];
{
  image src;
  image dest;
  long type;
  char *input = "-";
  char *output = "-";
  tableRec *call;

  if (argc > 1) input  = argv[1];
  if (argc > 2) output = argv[2];

  src  = Image.createFromFilename(input,input);
  dest = Image.create("dest");
  if (src == 0)
    {
      fprintf(stderr,"any2j4:error:can't open file (%s)\n",input);
      exit(-1);
    }

  type = Image.type(src);
  call = typeselectconst( type, tableRec, table );
  if (call == 0)
    {
      fprintf(stderr,"any2j4:image type is wrong (%s)\n",typename(type));
      exit(-1);
    }

  ImageFile.setSaveFormat("J4");
  if (call->type == call->new_type)
    {
      Image.save(src,output,ImageFile.getComment());
    }
  else
    {
      Image.make(dest,call->new_type,Image.xsize(src),Image.ysize(src));
      call->caller(dest,src);
      Image.save(dest,output,ImageFile.getComment());
    }

  Image.destroy(src);
  Image.destroy(dest);
}



void float_to_j4(dest,src)
     image dest;
     image src;
{
  register long x,y;
  register uchar **d;
  register float **s;
  long xsize,ysize;
  double max,min;

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  d = (uchar **)Image.data(dest);
  s = (float **)Image.data(src);

  max = min = s[0][0];
  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      {
	if (max < s[y][x]) max = s[y][x];
	if (min > s[y][x]) min = s[y][x];
      }
  if (max == min) max += 1;
  for ( y = 0; y < ysize; y++ )
    for ( x = 0; x < xsize; x++ )
      d[y][x] = 256 * (s[y][x] - min) / (max - min);
}



void short_to_j4(dest,src)
     image dest;
     image src;
{
  register long x,y;
  register uchar **d;
  register short **s;
  long xsize,ysize;
  long max,min;

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  d = (uchar **)Image.data(dest);
  s = (short **)Image.data(src);

  max = min = s[0][0];
  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      if (max < s[y][x]) max = s[y][x];
      else
      if (min > s[y][x]) min = s[y][x];

  for ( y = 0; y < ysize; y++ )
    for ( x = 0; x < xsize; x++ )
      d[y][x] = 256 * (s[y][x] - min) / (max - min + 1);
}



void short2_to_j4(dest,src)
     image dest;
     image src;
{
  register long x,y;
  register uchar3 **d;
  register short2 **s;
  long xsize,ysize;
  long2 max,min,range;

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  d = (uchar3 **)Image.data(dest);
  s = (short2 **)Image.data(src);

  max.at[0] = min.at[0] = s[0][0].at[0];
  max.at[1] = min.at[1] = s[0][0].at[1];
  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      {
	if (max.at[0] < s[y][x].at[0]) max.at[0] = s[y][x].at[0];
	if (min.at[0] > s[y][x].at[0]) min.at[0] = s[y][x].at[0];
	
	if (max.at[1] < s[y][x].at[1]) max.at[1] = s[y][x].at[1];
	if (min.at[1] > s[y][x].at[1]) min.at[1] = s[y][x].at[1];
      }
  max.at[0] ++;
  max.at[1] ++;
  min.at[0] --;
  min.at[1] --;

  for ( y = 0; y < ysize; y++ )
    for ( x = 0; x < xsize; x++ )
      {
	if (s[y][x].at[0] == 0 && s[y][x].at[1] == 0)
	  {
	    d[y][x].at[0] = d[y][x].at[1] = d[y][x].at[2] = 255;
	  }
	else
	if (s[y][x].at[0] < 0 && s[y][x].at[1] < 0)
	  {
	    d[y][x].at[0] = 256 * s[y][x].at[0] / min.at[0];
	    d[y][x].at[1] = 256 * s[y][x].at[1] / min.at[1];
	    d[y][x].at[2] = 0;
	  }
	else
	if (s[y][x].at[0] < 0 && s[y][x].at[1] > 0)
	  {
	    d[y][x].at[0] = 256 * s[y][x].at[0] / min.at[0];
	    d[y][x].at[1] = 0;
	    d[y][x].at[2] = 256 * s[y][x].at[1] / max.at[1];
	  }
	else
	if (s[y][x].at[0] > 0 && s[y][x].at[1] < 0)
	  {
	    d[y][x].at[0] = 0;
	    d[y][x].at[1] = 256 * s[y][x].at[1] / min.at[1];
	    d[y][x].at[2] = 256 * s[y][x].at[0] / max.at[0];
	  }
	else
	if (s[y][x].at[0] > 0 && s[y][x].at[1] > 0)
	  {
	    d[y][x].at[0] = 256 * s[y][x].at[1] / max.at[1];
	    d[y][x].at[1] = 0;
	    d[y][x].at[2] = 256 * s[y][x].at[0] / max.at[0];
	  }
      }
}



void short3_to_j4(dest,src)
     image dest;
     image src;
{
  register long x,y;
  register uchar3 **d;
  register short3 **s;
  long xsize,ysize;
  long3 max,min,range;

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  d = (uchar3 **)Image.data(dest);
  s = (short3 **)Image.data(src);

  max.at[0] = min.at[0] = s[0][0].at[0];
  max.at[1] = min.at[1] = s[0][0].at[1];
  max.at[2] = min.at[2] = s[0][0].at[2];
  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      {
	if (max.at[0] < s[y][x].at[0]) max.at[0] = s[y][x].at[0];
	if (min.at[0] > s[y][x].at[0]) min.at[0] = s[y][x].at[0];

	if (max.at[1] < s[y][x].at[1]) max.at[1] = s[y][x].at[1];
	if (min.at[1] > s[y][x].at[1]) min.at[1] = s[y][x].at[1];

	if (max.at[2] < s[y][x].at[2]) max.at[2] = s[y][x].at[2];
	if (min.at[2] > s[y][x].at[2]) min.at[2] = s[y][x].at[2];
      }
  range.at[0] = max.at[0] - min.at[0] + 1;
  range.at[1] = max.at[1] - min.at[1] + 1;
  range.at[2] = max.at[2] - min.at[2] + 1;

  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      {
	d[y][x].at[0] = 256 * (s[y][x].at[0] - min.at[0]) / range.at[0];
	d[y][x].at[1] = 256 * (s[y][x].at[1] - min.at[1]) / range.at[1];
	d[y][x].at[2] = 256 * (s[y][x].at[2] - min.at[2]) / range.at[2];
      }
}



void ushort_to_j4(dest,src)
     image dest;
     image src;
{
  register ushort **s;
  register uchar **d;
  register int x,y;
  long xsize,ysize;

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  s = (ushort **)Image.data(src);
  d = (uchar **)Image.data(dest);

  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      {
	long l = s[y][x];
	if ((x == 0 || y == 0 || x == (xsize-1) || y == (ysize-1)) ||
	    (s[y][x-1] != l) || (s[y-1][x] != l) || (s[y-1][x-1] != l))
	  d[y][x] = 0;
	else
	  d[y][x] = 1;
      }
}



void long_to_j4(dest,src)
     image dest;
     image src;
{
  register long **s;
  register uchar **d;
  register int x,y;
  long xsize,ysize;

  xsize = Image.xsize(src);
  ysize = Image.ysize(src);
  s = (long **)Image.data(src);
  d = (uchar **)Image.data(dest);

  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      {
	long l = s[y][x];
	if ((x == 0 || y == 0 || x == (xsize-1) || y == (ysize-1))||
	    (s[y][x-1] != l) || (s[y-1][x] != l) || (s[y-1][x-1] != l))
	  d[y][x] = 0;
	else
	  d[y][x] = 1;
      }
}
