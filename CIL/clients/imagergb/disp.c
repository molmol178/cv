#include "XRGB.h"
#include "Xcil/XImage.h"



static void get_size P ((XRGB *rgb));



void xrgb_disp(rgb)
     XRGB *rgb;
{
  long i,x,y;
  uchar3 *p;
  image img;
  uchar3 **data,*q;
  uchar3 bg;

  get_size(rgb);

  img = Image.createMake("TMP",UChar3,256,256);

  data = (uchar3 **)Image.data(img);
  bg.at[0] = (rgb->bg & 0x0000ff);
  bg.at[1] = (rgb->bg & 0x00ff00) >> 8;
  bg.at[2] = (rgb->bg & 0xff0000) >> 16;

  p = (uchar3 *)Image.raster(rgb->img);
  q = (uchar3 *)Image.raster(img);
  for (i = 0; i < Image.area(img); i++) *q++ = bg;
  for (i = 0; i < Image.area(rgb->img); i++,p++)
    data[255-p->at[1]][p->at[0]] = *p;
  XShowImagePartOfWindow(rgb->display,rgb->win,rgb->gc,img,
			 rgb->rx,rgb->ry,rgb->rxsize,rgb->rysize);

  p = (uchar3 *)Image.raster(rgb->img);
  q = (uchar3 *)Image.raster(img);
  for (i = 0; i < Image.area(img); i++) *q++ = bg;
  for (i = 0; i < Image.area(rgb->img); i++,p++)
    data[255-p->at[2]][p->at[0]] = *p;
  XShowImagePartOfWindow(rgb->display,rgb->win,rgb->gc,img,
			 rgb->gx,rgb->gy,rgb->gxsize,rgb->gysize);

  p = (uchar3 *)Image.raster(rgb->img);
  q = (uchar3 *)Image.raster(img);
  for (i = 0; i < Image.area(img); i++) *q++ = bg;
  for (i = 0; i < Image.area(rgb->img); i++,p++)
    data[255-p->at[2]][p->at[1]] = *p;
  XShowImagePartOfWindow(rgb->display,rgb->win,rgb->gc,img,
			 rgb->bx,rgb->by,rgb->bxsize,rgb->bysize);

  p = (uchar3 *)Image.raster(rgb->img);
  q = (uchar3 *)Image.raster(img);
  for (i = 0; i < Image.area(img); i++) *q++ = bg;
  for (i = 0; i < Image.area(rgb->img); i++,p++) {
    long xx,yy;
    xx = (255 + ((int)p->at[0] - ((int)p->at[1] + (int)p->at[2]) / 2))/2;
    yy = (255 + (1732 * ((int)p->at[1] - (int)p->at[2]) / 2000))/2;
    if (yy > 255 || yy < 0) {
      fprintf(stderr,"warning:yy = %d.\n");
      if (yy > 255) yy = 255;
      if (yy < 0) yy = 0;
    }
    data[255-yy][xx] = *p;
  }
  XShowImagePartOfWindow(rgb->display,rgb->win,rgb->gc,img,
			 rgb->mx,rgb->my,rgb->mxsize,rgb->mysize);
  Image.destroy(img);
  XFlush(rgb->display);


  x = rgb->mxsize / 2;
  y = rgb->mysize / 2;
  XSetLineAttributes(rgb->display,rgb->gc,0,LineOnOffDash,CapRound,JoinMiter);
  XDrawLine(rgb->display,rgb->win,rgb->gc,
	    rgb->mx+0,
	    rgb->my+y,
	    rgb->mx+rgb->mxsize,
	    rgb->my+y);
  XDrawLine(rgb->display,rgb->win,rgb->gc,
	    rgb->mx+(int)(x+x*cos((double)(M_PI/3))),
	    rgb->my+(int)(y+y*sin((double)(M_PI/3))),
	    rgb->mx+(int)(x-x*cos((double)(M_PI/3))),
	    rgb->my+(int)(y-y*sin((double)(M_PI/3))));
  XDrawLine(rgb->display,rgb->win,rgb->gc,
	    rgb->mx+(int)(x+x*cos((double)(M_PI/3))),
	    rgb->my+(int)(y-y*sin((double)(M_PI/3))),
	    rgb->mx+(int)(x-x*cos((double)(M_PI/3))),
	    rgb->my+(int)(y+y*sin((double)(M_PI/3))));

  XSetLineAttributes(rgb->display,rgb->gc,0,LineSolid,CapRound,JoinMiter);
  XDrawLine(rgb->display,rgb->win,rgb->gc,
	    rgb->mx+0,
	    rgb->my+y,
	    rgb->mx+(int)(x-x*cos((double)(M_PI/3))),
	    rgb->my+(int)(y+y*sin((double)(M_PI/3))));
  XDrawLine(rgb->display,rgb->win,rgb->gc,
	    rgb->mx+0,
	    rgb->my+y,
	    rgb->mx+(int)(x-x*cos((double)(M_PI/3))),
	    rgb->my+(int)(y-y*sin((double)(M_PI/3))));
  XDrawLine(rgb->display,rgb->win,rgb->gc,
	    rgb->mx+rgb->mxsize,
	    rgb->my+y,
	    rgb->mx+(int)(x+x*cos((double)(M_PI/3))),
	    rgb->my+(int)(y-y*sin((double)(M_PI/3))));
  XDrawLine(rgb->display,rgb->win,rgb->gc,
	    rgb->mx+rgb->mxsize,
	    rgb->my+y,
	    rgb->mx+(int)(x+x*cos((double)(M_PI/3))),
	    rgb->my+(int)(y+y*sin((double)(M_PI/3))));
  XDrawLine(rgb->display,rgb->win,rgb->gc,
	    rgb->mx+(int)(x-x*cos((double)(M_PI/3))),
	    rgb->my+(int)(y-y*sin((double)(M_PI/3))),
	    rgb->mx+(int)(x+x*cos((double)(M_PI/3))),
	    rgb->my+(int)(y-y*sin((double)(M_PI/3))));
  XDrawLine(rgb->display,rgb->win,rgb->gc,
	    rgb->mx+(int)(x-x*cos((double)(M_PI/3))),
	    rgb->my+(int)(y+y*sin((double)(M_PI/3))),
	    rgb->mx+(int)(x+x*cos((double)(M_PI/3))),
	    rgb->my+(int)(y+y*sin((double)(M_PI/3))));

  x = rgb->mxsize / 2;
  y = rgb->mysize / 2;
  x = rgb->mx - rgb->fheight;
  y = rgb->my+y - rgb->fheight / 2;
  XcilPutStringBox(rgb->display,rgb->win,rgb->gc,
		   x,y,rgb->fheight,rgb->fheight,"cc","C");

  x = rgb->mxsize / 2;
  y = rgb->mysize / 2;
  x = rgb->mx+(int)(x-x*cos((double)(M_PI/3))) - rgb->fheight;
  y = rgb->my+(int)(y+y*sin((double)(M_PI/3)));
  XcilPutStringBox(rgb->display,rgb->win,rgb->gc,
		   x,y,rgb->fheight,rgb->fheight,"cc","B");

  x = rgb->mxsize / 2;
  y = rgb->mysize / 2;
  x = rgb->mx+(int)(x-x*cos((double)(M_PI/3))) - rgb->fheight;
  y = rgb->my+(int)(y-y*sin((double)(M_PI/3))) - rgb->fheight;
  XcilPutStringBox(rgb->display,rgb->win,rgb->gc,
		   x,y,rgb->fheight,rgb->fheight,"cc","G");

  x = rgb->mxsize / 2;
  y = rgb->mysize / 2;
  x = rgb->mx+rgb->mxsize;
  y = rgb->my+y - rgb->fheight / 2;
  XcilPutStringBox(rgb->display,rgb->win,rgb->gc,
		   x,y,rgb->fheight,rgb->fheight,"cc","R");

  x = rgb->mxsize / 2;
  y = rgb->mysize / 2;
  x = rgb->mx+(int)(x+x*cos((double)(M_PI/3)));
  y = rgb->my+(int)(y-y*sin((double)(M_PI/3))) - rgb->fheight;
  XcilPutStringBox(rgb->display,rgb->win,rgb->gc,
		   x,y,rgb->fheight,rgb->fheight,"cc","Y");

  x = rgb->mxsize / 2;
  y = rgb->mysize / 2;
  x = rgb->mx+(int)(x+x*cos((double)(M_PI/3)));
  y = rgb->my+(int)(y+y*sin((double)(M_PI/3)));
  XcilPutStringBox(rgb->display,rgb->win,rgb->gc,
		   x,y,rgb->fheight,rgb->fheight,"cc","M");

  XFlush(rgb->display);
}



static void get_size(rgb)
     XRGB *rgb;
{
  long x, y, width,height;
  XWindowAttributes xwa;

  XClearWindow(rgb->display,rgb->win);
  XGetWindowAttributes(rgb->display,rgb->win,&xwa);
  width = xwa.width;
  height = xwa.height;

  rgb->rxsize = (width  - 2 * rgb->fheight - 3) / 2;
  rgb->rysize = (height - 2 * rgb->fheight - 3) / 2;

  rgb->gxsize = rgb->rxsize;
  rgb->gysize = (height - 2 * rgb->fheight - 3) - rgb->rysize;

  rgb->bxsize = (width  - 2 * rgb->fheight - 3) - rgb->rxsize;
  rgb->bysize = rgb->gysize;

  rgb->rx = rgb->fheight + 1;
  rgb->ry = 0;

  rgb->gx = rgb->fheight + 1;
  rgb->gy = rgb->fheight + rgb->rysize + 2;

  rgb->bx = 2 * rgb->fheight + rgb->rxsize + 3;
  rgb->by = rgb->fheight + rgb->rysize + 2;

  rgb->mxsize = width - rgb->rxsize - 3*rgb->fheight - 3;
  rgb->mysize = height - rgb->bysize - 3*rgb->fheight - 3;

  rgb->mx = rgb->fheight + rgb->fheight + rgb->rxsize + 3;
  rgb->my = rgb->fheight + 1;

  y = rgb->rysize + rgb->fheight + 1;
  XDrawLine(rgb->display,rgb->win,rgb->gc,0,y,width,y);
  x = rgb->rxsize + rgb->fheight + 1;
  XDrawLine(rgb->display,rgb->win,rgb->gc,x,0,x,height);

  x = rgb->fheight;
  XDrawLine(rgb->display,rgb->win,rgb->gc,x,0,x,height);
  x = rgb->fheight + 1 + rgb->rxsize + 1 + rgb->fheight;
  XDrawLine(rgb->display,rgb->win,rgb->gc,x,y,x,height);

  y = rgb->rysize + 1 + rgb->fheight + 1 + rgb->gysize;
  XDrawLine(rgb->display,rgb->win,rgb->gc,0,y,width,y);
  y = rgb->rysize;
  XDrawLine(rgb->display,rgb->win,rgb->gc,0,y,x-1-rgb->fheight,y);



  x = rgb->fheight + 1 + rgb->rxsize - rgb->fheight;
  y = rgb->rysize + 1;
  XcilPutStringBox(rgb->display,rgb->win,rgb->gc,x,y,rgb->fheight,rgb->fheight,"cc","R");

  x = 0;
  y = rgb->rysize + 1;
  XcilPutStringBox(rgb->display,rgb->win,rgb->gc,x,y,rgb->fheight,rgb->fheight,"cc","0");

  x = 0;
  y = 0;
  XcilPutStringBox(rgb->display,rgb->win,rgb->gc,x,y,rgb->fheight,rgb->fheight,"cc","G");


  x = rgb->fheight + 1 + rgb->rxsize - rgb->fheight;
  y = rgb->rysize + 1 + rgb->fheight + 1 + rgb->gysize + 1;
  XcilPutStringBox(rgb->display,rgb->win,rgb->gc,x,y,rgb->fheight,rgb->fheight,"cc","R");

  x = 0;
  y = rgb->rysize + 1 + rgb->fheight + 1 + rgb->gysize + 1;
  XcilPutStringBox(rgb->display,rgb->win,rgb->gc,x,y,rgb->fheight,rgb->fheight,"cc","0");

  x = 0;
  y = rgb->rysize + 1 + rgb->fheight + 1;
  XcilPutStringBox(rgb->display,rgb->win,rgb->gc,x,y,rgb->fheight,rgb->fheight,"cc","B");

  x = rgb->fheight + 1 + rgb->rxsize + 1;
  y = rgb->rysize + 1 + rgb->fheight + 1;
  XcilPutStringBox(rgb->display,rgb->win,rgb->gc,x,y,rgb->fheight,rgb->fheight,"cc","B");

  x = rgb->fheight + 1 + rgb->rxsize + 1;
  y = rgb->rysize + 1 + rgb->fheight + 1 + rgb->gysize + 1;
  XcilPutStringBox(rgb->display,rgb->win,rgb->gc,x,y,rgb->fheight,rgb->fheight,"cc","0");

  x = (rgb->fheight + 1 + rgb->rxsize + 1 + rgb->fheight + 1 + rgb->bxsize
       - rgb->fheight);
  y = rgb->rysize + 1 + rgb->fheight + 1 + rgb->bysize + 1;
  XcilPutStringBox(rgb->display,rgb->win,rgb->gc,x,y,rgb->fheight,rgb->fheight,"cc","G");
}
