#include "Image.h"
#include "Image/Morphology.h"
#include <math.h>
#include <stdio.h>



void image__add
  P3 (( image, dest ),
      ( image, src1 ),
      ( image, src2 ))

void image__sub
  P3 (( image, dest ),
      ( image, src1 ),
      ( image, src2 ))



void main(argc,argv)
     int argc;
     char **argv;
{
  char *out = "-";
  image src1,src2,dest;
  char comment[512];
  long xspot,yspot;

  if (argc < 4)
    {
      fprintf(stderr,"Usage : op <input> <+> <input> <output> : uchar\n");
      fprintf(stderr,"Usage : op <input> <-> <input> <output> : uchar\n");
      fprintf(stderr,"Usage : op <input> '<' <input> <output> : uchar\n");
      fprintf(stderr,"Usage : op <input> '>' <input> <output> : uchar\n");
      fprintf(stderr,"Usage : op <input> 'x' <input> <output> : float\n");
      fprintf(stderr,"Usage : op <input> '/' <input> <output> : float\n");
      fprintf(stderr,"Usage : op <input> <o> <input> <output> [xspot] [yspot]\n");
      fprintf(stderr,"Usage : op <input> <c> <input> <output> [xspot] [yspot]\n");
      fprintf(stderr,"Usage : op <input> <d> <input> <output> [xspot] [yspot]\n");
      fprintf(stderr,"Usage : op <input> <e> <input> <output> [xspot] [yspot]\n");
      exit(-1);
    }

  src1 = Image.createFromFilename("SRC1",argv[1]);
  src2 = Image.createFromFilename("SRC2",argv[3]);
  dest = Image.createMake("DEST",Image.type(src1),Image.xsize(src1),Image.ysize(src2));
  if ( argc > 4 ) out = argv[4];

  xspot = Image.xsize(src2) / 2;
  yspot = Image.ysize(src2) / 2;

  if (argc > 5) xspot = atol(argv[5]);
  if (argc > 6) yspot = atol(argv[6]);

  switch (argv[2][0])
    {
    case '+':
      image__add(dest,src1,src2);
      sprintf(comment,"add(image = %s, image = %s)",argv[1],argv[2]);
      Image.save(dest,out,comment);
      break;

    case '-':
      image__sub(dest,src1,src2);
      sprintf(comment,"sub(image = %s, image = %s)",argv[1],argv[2]);
      Image.save(dest,out,comment);
      break;

    case '>':
      {
	register int i;
	register uchar *s1,*s2,*d;
	long n;

	s1 = Image.raster(src1);
	s2 = Image.raster(src2);
	d  = Image.raster(dest);
	n  = Image.area(src1);
	for (i = 0; i < n; i++,s1++,s2++,d++)
	  *d = ( *s1 < *s2 ) ? *s1 : *s2;

	sprintf(comment,"min(image = %s, image = %s)",argv[1],argv[2]);
	Image.save(dest,out,comment);
      }
      break;

    case '<':
      {
	register int i;
	register uchar *s1,*s2,*d;
	long n;

	s1 = Image.raster(src1);
	s2 = Image.raster(src2);
	d  = Image.raster(dest);
	n  = Image.area(src1);
	for (i = 0; i < n; i++,s1++,s2++,d++)
	  *d = ( *s1 > *s2 ) ? *s1 : *s2;

	sprintf(comment,"max(image = %s, image = %s)",argv[1],argv[2]);
	Image.save(dest,out,comment);
      }
      break;

    case 'x':
      {
	register int i;
	register float *s1,*s2,*d;
	long n;

	s1 = Image.raster(src1);
	s2 = Image.raster(src2);
	d  = Image.raster(dest);
	n  = Image.area(src1);
	for (i = 0; i < n; i++) *d++ = *s1++ * *s2++;

	sprintf(comment,"mul(image = %s, image = %s)",argv[1],argv[2]);
	Image.save(dest,out,comment);
      }
      break;

    case '/':
      {
	register int i;
	register float *s1,*s2,*d;
	long n;

	s1 = Image.raster(src1);
	s2 = Image.raster(src2);
	d  = Image.raster(dest);
	n  = Image.area(src1);
	for (i = 0; i < n; i++,d++,s1++,s2++)
	  if ( *s2 ) *d = *s1 / *s2; else *d = 0;

	sprintf(comment,"div(image = %s, image = %s)",argv[1],argv[2]);
	Image.save(dest,out,comment);
      }
      break;

    case 'o':
      image__opening(dest,src1,src2,xspot,yspot);
      sprintf(comment,"opening(image = %s, image = %s, xspot = %d, yspot = %d)",
	      argv[1],argv[2],xspot,yspot);
      Image.save(dest,out,comment);
      break;

    case 'c':
      image__closing(dest,src1,src2,xspot,yspot);
      sprintf(comment,"closing(image = %s, image = %s, xspot = %d, yspot = %d)",
	      argv[1],argv[2],xspot,yspot);
      Image.save(dest,out,comment);
      break;

    case 'd':
      image__dilation(dest,src1,src2,xspot,yspot);
      sprintf(comment,"dilation(image = %s, image = %s, xspot = %d, yspot = %d)",
	      argv[1],argv[2],xspot,yspot);
      Image.save(dest,out,comment);
      break;

    case 'e':
      image__erosion(dest,src1,src2,xspot,yspot);
      sprintf(comment,"erosion(image = %s, image = %s, xspot = %d, yspot = %d)",
	      argv[1],argv[2],xspot,yspot);
      Image.save(dest,out,comment);
      break;
    }
}



void image__add
  _P3 (( image, dest ),
       ( image, src1 ),
       ( image, src2 ))
{
  register int i;
  register long n;

  n = Image.area(src1);
  switch ( __TYPE(src1) )
    {
    case UChar:
      {
	register uchar *s1,*s2,*ds;
	s1 = Image.raster(src1);
	s2 = Image.raster(src2);
	ds  = Image.raster(dest);
	for ( i = 0; i < n; i++ ) *ds++ = *s1++ + *s2++;
      }
      break;

    case Float:
      {
	register float *s1,*s2,*ds;
	s1 = Image.raster(src1);
	s2 = Image.raster(src2);
	ds  = Image.raster(dest);
	for ( i = 0; i < n; i++ ) *ds++ = *s1++ + *s2++;
      }
      break;
    }
}



void image__sub
  _P3 (( image, dest ),
       ( image, src1 ),
       ( image, src2 ))
{
  register int i;
  register long n;

  n = Image.area(src1);
  switch ( __TYPE(src1) )
    {
    case UChar:
      {
	register uchar *s1,*s2,*ds;
	s1 = Image.raster(src1);
	s2 = Image.raster(src2);
	ds  = Image.raster(dest);
	for ( i = 0; i < n; i++ ) *ds++ = abs( (long)*s1++ - (long)*s2++ );
      }
      break;

    case Float:
      {
	register float *s1,*s2,*ds;
	s1 = Image.raster(src1);
	s2 = Image.raster(src2);
	ds  = Image.raster(dest);
	for ( i = 0; i < n; i++ ) *ds++ = *s1++ - *s2++;
      }
      break;
    }
}
