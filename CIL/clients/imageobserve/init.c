/*
 * filename : initialize.c
 * author   : Takahiro Sugiyama
 * date     : Saturday, February 8 1997
 * describe : �����
 */


#include "observe.h"


/*------------------------------------------------------------------------*
 * ���ץ���������
 *------------------------------------------------------------------------*/
char *opt_item[] = {
  "����:*:*:(����) ���ϲ����Υե�����̾",
  "�إ��:-h:�إ�ץ�å�������ɽ������",
  "���ǥ�����:-s:1:8:(����) �ѻ�������ɥ���ɽ��������Ǥ��礭��",
  "�ե����:-f:1:a14:(�ե����̾) ��ɽ���ˤ�����ե���Ȥλ���",

  "*) ������ɥ���ɽ�����֤˴ؤ��륪�ץ����.",
  "�������:-T:������ɥ��򥿥�������¤٤�ʽ����",
  "�Ťͤ�:-O:������ɥ���Ťͤ�ɽ������",
  "�濴:-C:������ɥ����濴���¤٤�",
  "�ѻ��뱦:-R:�ѻ����ɽ��������ɥ��α����¤٤�ʽ����",
  "�ѻ��벼:-B:�ѻ����ɽ��������ɥ��β����¤٤�",

};


/*------------------------------------------------------------------------*
 * �����
 *------------------------------------------------------------------------*/
long MAX_WX;
long MAX_WY;
void initialize(ObserveRec *observe, long argc, char *argv[])
{
  long i;
  char **files;

  int xoff = 10, yoff = 10;
  int xstep = 25, ystep = 25;
  int xoffstart = xoff;
  int ymax;
  int scale;

  int right_ob, bottom_ob;

  OPTION_SETUP(opt_item);
  if (argc == 1) optusage(1);
  if (optspecified("�إ��")) optmanual(1);

  scale = optvalueint("���ǥ�����");
  observe->num = optvaluenum("����");
  files = optvaluelist("����");
  observe->list = typenew1(observe->num, ObItemRec);

  observe->display = XcilOpenDisplay(0);
  observe->context = XcilUniqueContext();
  item__make_pixel_pixmap(observe->display);
  set_value_font(observe->display, optvalue("�ե����"));

  XcilGetWindowSize(observe->display,Xcil_root_window,&MAX_WX,&MAX_WY);

  if (optspecified("�濴"))
    {
      xoff = MAX_WX;
      yoff = MAX_WY;
    }
  else
  if (optspecified("�Ťͤ�"))
    {
       xoff = 10; yoff = 10;
       xstep = 15; ystep = 25;
       xoffstart = xoff;
    }
  else
    {
       xoff = 5; yoff = 5;
       xstep = 5; ystep = 25;
       ymax = 0;
    }

  right_ob = 2;
  bottom_ob = 1;
  if (optspecified("�ѻ��벼"))
    {bottom_ob = 2; right_ob = 1;}

  for (i = 0; i < observe->num; i++)
    {
      ObItemRec *item = &observe->list[i];

      item__init(item, files[i], scale);
      if (item__load(item) == 0) continue;
      text__set_size(item, observe->display);
      item__fit_region_size(item);

      if (optspecified("�濴"))
	{
	  xoff = (MAX_WX - right_ob*item->xsize)/2;
	  yoff = (MAX_WY - bottom_ob*item->ysize)/2;
	}

      item__open_window(item, observe->display, observe->context, xoff, yoff);
      item__make_image(item);
      item__disp(item, observe->display);

      if (optspecified("�Ťͤ�"))
	{
	  xoff += xstep; yoff += ystep;
	  if ((xoff > (MAX_WX-item->xsize/2)) ||
	      (yoff > (MAX_WY-item->ysize/2)))
	    { xoffstart += 4*xstep; xoff = xoffstart; yoff = 10; }
	}
      else
      if (!optspecified("�濴"))
	{
	  xoff += right_ob * (xstep + item->xsize);
	  if (ymax < item->ysize) ymax = item->ysize;
	  if (xoff > (MAX_WX-item->xsize/bottom_ob))
	    {
	      yoff += bottom_ob * (ystep + ymax);
	      xoff = 5;
	      if (yoff > (MAX_WY-ymax/right_ob)) {yoff = 5;}
	      ymax = 0;
	    }
	}
    }
}
