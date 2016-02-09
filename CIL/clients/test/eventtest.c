/*
 * eventtest.c
 */




#include "Image.h"
#include "ImageDisp.h"
#include <stdio.h>



void print_mouse
  _P1 (( CILEvent *, event ))
{
  fprintf(stderr,"MouseEvent = {\n");
  fprintf(stderr,"    type     : %d\n",event->mouse.type);
  fprintf(stderr,"    button   : %d\n",event->mouse.button);
  fprintf(stderr,"    x        : %d\n",event->mouse.x);
  fprintf(stderr,"    y        : %d\n",event->mouse.y);
  fprintf(stderr,"    window_x : %d\n",event->mouse.window_x);
  fprintf(stderr,"    window_y : %d\n",event->mouse.window_y);
  fprintf(stderr,"}\n");
}



void print_keyboard
  _P1 (( CILEvent *, event ))
{
  fprintf(stderr,"KeyboardEvent = {\n");
  fprintf(stderr,"    type     : %d\n",event->keyboard.type);
  fprintf(stderr,"    ascii    : 0x%02x`%c`\n",event->keyboard.ascii,event->keyboard.ascii);
  fprintf(stderr,"    x        : %d\n",event->keyboard.x);
  fprintf(stderr,"    y        : %d\n",event->keyboard.y);
  fprintf(stderr,"    window_x : %d\n",event->keyboard.window_x);
  fprintf(stderr,"    window_y : %d\n",event->keyboard.window_y);
  fprintf(stderr,"    keysym   : 0x%04x\n",event->keyboard.keysym);
  fprintf(stderr,"}\n");
}



char *eventname[] = {
  "NoEvent",
  "ImageExpose",
  "MouseMove",
  "MousePress",
  "MouseRelease",
  "MouseEnter",
  "MouseLeave",
  "KeyboardPress",
  "KeyboardRelease"
};



int main(argc,argv)
     int argc;
     char **argv;
{
  image src;
  char *input = "-";
  CILEvent event;

  if ( ( argc == 2 ) && ( strcmp( "-h", argv[ 1 ] ) == 0 ) )
    {
      fprintf(stderr,"Usage : cilview [input]\n");
      return 1;
    }

  if ( argc > 1 ) input = argv[1];

  src = Image.createFromFilename(input,input);
  if (src == 0)
    {
      fprintf(stderr,"cilview:can't open (%s)\n",input);
      return -1;
    }
  Image.display(src,0);

  ImageDispSelectEvent( src, (ImageExposeMask|
			      MousePressMask|MouseMoveMask|MouseReleaseMask|
			      MouseEnterMask|MouseLeaveMask|
			      KeyboardPressMask|KeyboardReleaseMask) );
  while ( 1 )
    {
      if ( ImageDispNextEvent( src, &event ) == -1 ) break;
      fprintf(stderr,"%s:",eventname[event.type]);
      switch ( event.type )
	{
	case NoEvent:
	  fprintf(stderr,"\n");
	  break;
	case ImageExpose:
	  fprintf(stderr,"\n");
	  break;
	case MouseMove:
	case MouseRelease:
	case MousePress:
	case MouseEnter:
	case MouseLeave:
	  print_mouse(&event);
	  break;
	case KeyboardPress:
	case KeyboardRelease:
	  print_keyboard(&event);
	  if ((event.keyboard.ascii == 'q')||(event.keyboard.ascii == 'Q'))
	    exit(0);
	  break;
	}
    }
  Image.undisplay( src );
  return 0;
}
