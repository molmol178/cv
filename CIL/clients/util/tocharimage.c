/*
  text file , char file -> ppm file
         by T.Hisajima    Sep.1.1993
	 £±£¹£¹£³Ç¯£±£°·î  £±Æü  ²þÄê
	 £±£¹£¹£´Ç¯  £²·î  £±Æü  ¥é¥¤¥Ö¥é¥êÊÑ¹¹
	 £±£¹£¹£´Ç¯  £²·î  £·Æü  ¥é¥¤¥Ö¥é¥êÊÑ¹¹
*/
#include "Image.h"
#include "Xcil/XImage.h"
#include "misc/optlib.h"
#include <stdio.h>
#include <stdlib.h>

#define MAXCHR 1024

typedef struct T2PRec {
  image src;
  image middle;
  image out;
  char *name_of_input;
  char *name_of_output;
  long xsize, ysize;
  char table[MAXCHR];
} *T2P;

char *opt_item[] =
{
  "input:*:1:(image) name of input text or character image",
  "output:-o:1:out.char:(image) name of output image",
  "font:-f:1:g5:(font) name of font g[2-8] or f[7-10]",
  "space:-s:1:1:(int) space between fonts",
  "xyspace:-xys:2:1:1:(int) x, y space between font",
  "help:-h:print this message",
};

void t2p_init
  P3(( T2P     , t2p ),
     ( int     , argc ),
     ( char ** , argv ))

void addchar
  P2(( char   , c ),
     ( char * , table))

void t2p_pass1 P1 (( T2P , t2p))
void t2p_pass_char P1 (( T2P , t2p ))
void t2p_bit1touchar P1 (( T2P , t2p ))
void t2p_exit P1 (( T2P , t2p ))

char fgets2
  P3 (( char * , s ),
      ( int    , n ),
      ( FILE * , iop ))

void main
  _P2 (( int    , argc ),
       (char ** , argv ))
{
  struct T2PRec t2p;

  t2p_init(&t2p,argc,argv);
  if (Image.load(t2p.src,t2p.name_of_input) == 0)
    {
      t2p_pass1(&t2p);
      t2p_pass_char(&t2p);
    }
  else
    if (typesize(Image.type(t2p.src)) == 1)
      t2p.src->type = Char;
  else
    {
      fprintf(stderr,"Image(%s) is wrong type(%s)\n",
	      t2p.name_of_input,typename(t2p.src));
      exit(-1);
    }
  t2p.middle->type = PackedBit1;
  t2p.middle->data = (char **)XImageCharToPackedBit1
    (t2p.src->data,0,0,t2p.src->xsize,t2p.src->ysize,
     &t2p.middle->xsize,&t2p.middle->ysize);
  t2p_bit1touchar(&t2p);
  t2p_exit(&t2p);
}

void t2p_init
  _P3(( T2P     , t2p ),
      ( int     , argc ),
      ( char ** , argv ))
{
  t2p->xsize = 0;
  t2p->ysize = 0;
  t2p->src = Image.create("SRC");
  t2p->middle = Image.create("MIDDLE");
  t2p->out = Image.create("OUT");

  optinit(Number(opt_item),opt_item,argc,argv);
  if (argc < 2) optusage(1);
  if (optspecified("help")) optmanual(1);

  t2p->name_of_input = optvalue("input");
  t2p->name_of_output = optvalue("output");

  XImageFontSet(optvalue("font"));

  if (optspecified("space"))
    XImageFontSetSpace(optvalueint("space"),optvalueint("space"));

  if (optspecified("xyspace"))
    XImageFontSetSpace(optnvalueint("xyspace",0),
		       optnvalueint("xyspace",1));
}

void t2p_pass1 _P1 (( T2P , t2p))
{
  int i,len;
  char s[MAXCHR];
  FILE *fp;

  fp = fopen(t2p->name_of_input, "r");
  if (fp == 0) {
    fprintf(stderr,"can't open file %s\n",t2p->name_of_input);
    exit(-1);
  }

  while (fgets2(s, MAXCHR, fp)) {
    t2p->ysize++;
    if (t2p->xsize < (len = strlen(s) - 1)) t2p->xsize = len;
    for (i = 0; i <= len; i++)
      if (s[i] != ' ') addchar(s[i], t2p->table);
  }
  addchar(' ', t2p->table);
  fclose(fp);
}

void t2p_pass_char _P1 (( T2P , t2p ))
{
  register char **data;
  int i,j,len;
  char s[MAXCHR];
  FILE *fp;

  fp = fopen(t2p->name_of_input,"r");
  Image.make(t2p->src, Char, t2p->xsize, t2p->ysize);

  data = (char **)Image.data(t2p->src);
  for (j = 0; j < t2p->ysize; j++) {
    fgets2(s, MAXCHR, fp);
    len = strlen(s) - 1;
    for (i = 0; i < t2p->xsize; i++) {
      if (i < len)
	data[j][i] = s[i];
      else
	data[j][i] = ' ';
    }
  }
  fclose(fp);
}

void t2p_bit1touchar _P1 (( T2P , t2p ))
{
  register bit1 **bdata;
  register uchar **udata;
  int x,y,xsize,ysize;
  static uchar bit1mask[] = {1<<7,1<<6,1<<5,1<<4,1<<3,1<<2,1<<1,1<<0};

  xsize = Image.xsize(t2p->middle);
  ysize = Image.ysize(t2p->middle);
  bdata = (bit1 **)Image.data(t2p->middle);
  Image.make(t2p->out, UChar, xsize, ysize);
  udata = (uchar **)Image.data(t2p->out);

  for (y = 0; y < ysize; y++)
    for (x = 0; x < xsize; x++)
      if (bdata[y][x/8] & bit1mask[x%8])
	udata[y][x] = 0;
      else
	udata[y][x] = 255;
}

void t2p_exit _P1 (( T2P , t2p ))
{
  Image.save(t2p->out, t2p->name_of_output, 0);
  Image.destroy(t2p->src);
  Image.destroy(t2p->middle);
  Image.destroy(t2p->out);
}

void addchar
  _P2(( char   , c ),
      ( char * , table))
{
  int i;

  for (i = 0; table[i]; i++)
    if (c == table[i]) return;
  table[i] = c;
}

char fgets2
  _P3 (( char * , s ),
       ( int    , n ),
       ( FILE * , iop ))
{
  register int c,i,tabpoint;
  register char *cs;
  static int remain;
  
  cs = s;
  for (i=1;i<remain;i++)
    {
      *cs++ = ' ';
      --n;
    }
  remain = 0;
  while(--n > 0 && (c = getc(iop)) != EOF)
    {
      if (c == '\t')
	{
	  tabpoint = 9 - (300 - n) % 8;
	  if (tabpoint == 9)
	    tabpoint = 1;
	  if ((remain = tabpoint - n) > 0)
	    tabpoint -= n;
	  for(i = 0; i < tabpoint; i++)
	    {
	      *cs++ = ' ';
	      --n;
	    }
	  n++;
	}
      else
	*cs++ = c;
      if (c == '\n') break;
    }
  *cs = '\0';
  return (c == EOF && cs == s) ? NULL : *s;
}
