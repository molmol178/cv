/*
 * Xcil Test Program
 */



#include "Xcil/Xcil.h"
#include "misc/optlib.h"



long call_event_handler();
long expose_event_handler();
long quit_hdr();
long message_hdr();
long set_string_hdr();
long get_string_hdr();
long keyin_hdr();
long gets_hdr();
long gets_notify();
long modal_gets_hdr();
long scroll_hdr();
long popup_select();



typedef struct {
  long xsize, ysize;
  int Xsize, Ysize;
  Window w;
} rectangleRec;



char *option[] = {
  "geometry:-g:1:512x512-0-0:window geometry",
};


long Xcil_hdr_popupmenu();

int main(argc,argv)
     int argc;
     char **argv;
{
  Display *d;
  Window w, pw;
  long i, x, y;
  XContext c;
  rectangleRec rec;

  static XcilPopupMenuItem menu[] =
    {
      {"あいさつ"},
      {XcilPopupLINE},
      {"Hello",popup_select},
      {"Good morning",popup_select},
      {"Good bye",popup_select,1},
      {XcilPopupLINE},
      {"こんにちわ",popup_select},
      {"おはよう",popup_select},
      {"さよなら",popup_select,1},
      {XcilPopupEOM},
    };

  static XcilPanelItem item[] =
    {
      {{"a"   ,__TOP,__TOP},{XCIL_LABEL, "TEST PANEL WINDOW"}},
      {{"name",__PRE,__TOP},{XCIL_LABEL, " ACTION NAME "    }},
      {{"b"   ,__PRE,__TOP},{XCIL_3D_COMMAND,"QUIT"},{quit_hdr}},
      {{"c"   ,__PRE,__TOP},{XCIL_3D_COMMAND,"Close"},{Xcil_hdr_unmap_popup}},

      {{"s1",__TOP,__PRE},{XCIL_LABEL  ,"S LABEL"  },{get_string_hdr}},
      {{"s2",__PRE,__SAM},{XCIL_COMMAND,"S COMMAND"},{modal_gets_hdr}},
      {{"s3",__PRE,__SAM},{XCIL_BUTTON ,"S BUTTON" },{gets_hdr      }},

      {{"c1",__TOP,__PRE},{XCIL_COMMAND1,"COMMAND1"},{message_hdr   }},
      {{"c2",__PRE,__SAM},{XCIL_COMMAND2,"COMMAND2"},{set_string_hdr}},
      {{"c3",__PRE,__SAM},{XCIL_COMMAND3,"COMMAND3"},{set_string_hdr}},
      {{"c4",__PRE,__SAM},{XCIL_COMMAND4,"COMMAND4"},{set_string_hdr}},
      {{"c5",__PRE,__SAM},{XCIL_COMMAND5,"COMMAND5"},{set_string_hdr}},

      {{"b1",__TOP,__PRE},{XCIL_BUTTON1,"BUTTON1"},{keyin_hdr     }},
      {{"b2",__PRE,__SAM},{XCIL_BUTTON2,"BUTTON2"},{set_string_hdr}},
      {{"b3",__PRE,__SAM},{XCIL_BUTTON3,"BUTTON3"},{set_string_hdr}},
      {{"b4",__PRE,__SAM},{XCIL_BUTTON4,"BUTTON4"},{set_string_hdr}},
      {{"b5",__PRE,__SAM},{XCIL_BUTTON5,"BUTTON5"},{set_string_hdr}},

      {{"sb" ,__TOP,__PRE,400,  0},{XCIL_LABEL}},
      {{"sb3",__TOP,__PRE,400,  0},{XCIL_3D_LABEL}},
      {{"gx" ,__TOP,__PRE,400,  0},{XCIL_3D_LABEL}},
      {{"tx" ,__TOP,__PRE,400,200},{XCIL_3D_LABEL}},
      {{"sb4", "c5",__TOP, 20,400},{XCIL_LABEL}},
  };


  d = XcilOpenDisplay(0);
  c = XUniqueContext();

  optinit( Number( option ), option, argc, argv );
  XcilTransGeometry( optvalue("geometry"), &x, &y, &rec.xsize, &rec.ysize );

  rec.Xsize = DisplayWidth( d, 0 );
  rec.Ysize = DisplayHeight( d, 0 );
  if ( x == 0 ) x = ( rec.Xsize - rec.xsize ) / 2;
  if ( y == 0 ) y = ( rec.Ysize - rec.ysize ) / 2;
  w = XcilCreateWindow( d, 0, x, y, rec.xsize, rec.ysize, 1, "TEST WINDOW" );
  pw = XcilCreatePanelWindow( d, 0, c, 0, 0, item, XcilNumber(item), "TEST" );
  PI_value( item[ 2 ] ) = pw;
  PI_value( item[ 4 ] ) = pw;
  PI_deco( item[ 0 ] ) = DECO_FRAME2;
  PI_bodyGC( item[ 0 ] ) = Xcil_white_gc;
  PI_frameGC( item[ 0 ] ) = Xcil_black_gc;

  for (i = 3; i < XcilNumber(item); i++ )
    PI_value(item[i]) = pw;

  XcilMapWindow( d, w );
  XcilManagedWindow( d, w, c );
  rec.w = pw;
  XcilAddCallback( d, w, c, ButtonPress, call_event_handler, &rec );
  XcilAddCallback( d, w, c, Expose, expose_event_handler, &rec );
  XcilAddKeyCallback( d, w, c, 'q', Xcil_hdr_quit, 0 );

  w = XcilPanelItemWindow( d, pw, c, "gx" );
  XcilTextSet( d, w, c );
  XcilTextGetsReady( d, w, c, gets_notify, pw );

  w = XcilPanelItemWindow( d, pw, c, "tx" );
  XcilTextSet(d,w,c);

  w = XcilPanelItemWindow( d, pw, c, "sb" );
  XcilScrollSet(d,w,c,0,0);

  w = XcilPanelItemWindow( d, pw, c, "sb3" );
  XcilScrollSet(d,w,c,0,0);
  XcilScrollBarSetLength(d,w,c,0.0);

  w = XcilPanelItemWindow( d, pw, c, "sb4" );
  XcilScrollSet( d, w, c, scroll_hdr, pw );

  w = XcilPanelItemWindow( d, pw, c, "a" );
  for ( i = 0; i < Number( menu ); i++ ) menu[i].client_value = pw;
  XcilPopupSet( d, w, c, Button1, menu, XcilNumber( menu ) );

  XcilEventLoop( d, c );
}



long popup_select( d, w, c, e, p, index, list )
     Display *d;
     Window w;
     XContext c;
     XEvent *e;
     Window p;
     long index;
     XcilPopupMenuItem *list;
{
  Window tw;
  char buf[1024];

  tw = XcilPanelItemWindow( d, p, c, "tx" );

  XcilTextPrintf( d, tw, c, Xcil_black_gc,
		  "\nPOPUP(INDEX=%d,STRING=%s)",index,list[index].string );

  if (( strcmp( list[index].string, "Good bye" ) == 0 ) ||
      ( strcmp( list[index].string, "さよなら" ) == 0 ) ) return 1;

  return 0;
}


long call_event_handler( d, w, c, e, rec )
     Display *d;
     Window w;
     XContext c;
     XEvent *e;
     rectangleRec *rec;
{
  long x, y;

  switch ( e->xbutton.button )
    {
    case Button1:
      rec->xsize = rand() % ( rec->Xsize - rec->xsize );
      rec->ysize = rand() % ( rec->Ysize - rec->ysize );
      x = rand() % ( rec->Xsize - rec->xsize );
      y = rand() % ( rec->Ysize - rec->ysize );
      XMoveResizeWindow( d, w, x, y, rec->xsize, rec->ysize );
      break;
    case Button2:
      x = ( rec->Xsize - rec->xsize ) / 2;
      y = ( rec->Ysize - rec->ysize ) / 2;
      XMoveWindow( d, w, x, y );
      break;
    case Button3:
      XMoveWindow( d, rec->w, e->xbutton.x_root - 10, e->xbutton.y_root - 10 );
      XcilMapWindow( d, rec->w );
      XFlush( d );
      break;
    }

  XFlush( d );

  return 0;
}



long expose_event_handler( d, w, c, e, rec )
     Display *d;
     Window w;
     XContext c;
     XEvent *e;
     rectangleRec *rec;
{
  long x, y;
  char *string = "Hello World /Deco I";

  x = 10;
  y = 20;
  XcilPutStringDeco( d, w, Xcil_white_gc, Xcil_black_gc, x, y,
		     DECO_SHADOW1, string );
  y += 20;
  XcilPutStringDeco( d, w, Xcil_white_gc, Xcil_black_gc, x, y,
		     DECO_SHADOW2, string );
  y += 20;
  XcilPutStringDeco( d, w, Xcil_white_gc, Xcil_black_gc, x, y,
		     DECO_SHADOW3, string );
  y += 20;
  XcilPutStringDeco( d, w, Xcil_white_gc, Xcil_black_gc, x, y,
		     DECO_SHADOW4, string );
  y += 20;
  XcilPutStringDeco( d, w, Xcil_white_gc, Xcil_black_gc, x, y,
		     DECO_SHADOW5, string );
  y += 20;
  XcilPutStringDeco( d, w, Xcil_white_gc, Xcil_black_gc, x, y,
		     DECO_SHADOW6, string );
  y += 20;
  XcilPutStringDeco( d, w, Xcil_white_gc, Xcil_black_gc, x, y,
		     DECO_SHADOW7, string );
  y += 20;
  XcilPutStringDeco( d, w, Xcil_white_gc, Xcil_black_gc, x, y,
		     DECO_SHADOW8, string );
  y += 20;
  XcilPutStringDeco( d, w, Xcil_white_gc, Xcil_black_gc, x, y,
		     DECO_FRAME1, string );
  y += 20;
  XcilPutStringDeco( d, w, Xcil_white_gc, Xcil_black_gc, x, y,
		     DECO_FRAME2, string );
  y += 20;
  XcilPutStringDeco( d, w, Xcil_black_gc, Xcil_black_gc, x, y,
		     DECO_WIDE, string );

  return 0;
}



long quit_hdr( d, w, c, e )
     Display *d;
     Window w;
     XContext c;
     XEvent *e;
{
  return XcilConfirm( d, 0, c, -e->xbutton.x_root, -e->xbutton.y_root,
		      "QUIT CONFIRM", "Quit ... Are you sure ?");
}



long message_hdr( d, ow, c, e )
     Display *d;
     Window ow;
     XContext c;
     XEvent *e;
{
  static char *test_mess[] = {
    "This is test messages window test.",
    "  私はメッセージウインドウです。",
    "  メッセージを表示するだけが目的です。",
    0
  };

  XcilMessages( d, 0, c, 0, 0, "MESSAGE", test_mess );

  return 0;
}



long set_string_hdr( d, w, c, e, p, i )
     Display *d;
     Window w;
     XContext c;
     XEvent *e;
     Window p;
     XcilPanelItem *i;
{
  Window tw;

  XcilSetString( d, p, c, "name", PI_string(*i));

  tw = XcilPanelItemWindow( d, p, c, "tx" );

  XcilTextPrintf( d, tw, c, Xcil_black_gc,
		  "\nYou just setected %s", PI_string(*i) );

  return 0;
}



long keyin_hdr( d, w, c, e, p, i )
     Display *d;
     Window w;
     XContext c;
     XEvent *e;
     Window p;
     XcilPanelItem *i;
{
  Window tw;
  KeySym sym;

  tw = XcilPanelItemWindow( d, p, c, "tx" );

  XcilTextPrintf( d, tw, c, Xcil_black_gc, "\nPlease Input ... " );

  sym = XcilTextGetKeysym( d, tw, c );

  if ( sym < 0 ) return -sym;

  XcilTextPrintf( d, tw, c, Xcil_black_gc, " : KeySym = 0x%04x(%s)",
		  sym, XKeysymToString(sym));

  return 0;
}



long gets_hdr( d, w, c, e, p, i )
     Display *d;
     Window w;
     XContext c;
     XEvent *e;
     Window p;
     XcilPanelItem *i;
{
  long k;
  Window tw;
  static char buf[ 512 ] = "";

  tw = XcilPanelItemWindow( d, p, c, "tx" );

  XcilTextPrintf( d, tw, c, Xcil_black_gc, "\nGETS=" );

  k = XcilTextGets( d, tw, c, buf, sizeof( buf ) );

  if ( k < 0 ) return -k;

  XcilTextPrintf( d, tw, c, Xcil_black_gc, "\nYou just gets =\"%s\"", buf );

  return 0;
}



long modal_gets_hdr( d, w, c, e, p, i )
     Display *d;
     Window w;
     XContext c;
     XEvent *e;
     Window p;
     XcilPanelItem *i;
{
  long k;
  Window tw;
  static char buf[ 512 ] = "";

  tw = XcilPanelItemWindow( d, p, c, "tx" );

  XcilTextPrintf( d, tw, c, Xcil_black_gc, "\nMODAL GETS=" );

  k = XcilTextGrabGets( d, tw, c, buf, sizeof( buf ) );

  if ( k < 0 ) return -k;

  XcilTextPrintf( d, tw, c, Xcil_black_gc, "\nYou just modal gets =\"%s\"", buf );

  return 0;
}



long gets_notify( d, w, c, e, p, string )
     Display *d;
     Window w;
     XContext c;
     XEvent *e;
     Window p;
     char *string;
{
  Window tw;

  tw = XcilPanelItemWindow( d, p, c, "tx" );

  XcilTextPrintf( d, tw, c, Xcil_black_gc, "\nINPUTS=\"%s\"", string );

  return 0;
}



long get_string_hdr( d, w, c, e, p )
     Display *d;
     Window w;
     XContext c;
     XEvent *e;
     Window p;
{
  Window tw, gw;
  char buf[1024];

  tw = XcilPanelItemWindow( d, p, c, "tx" );
  gw = XcilPanelItemWindow( d, p, c, "gx" );

  XcilTextGetInput( d, gw, c, buf, sizeof(buf) );

  XcilTextPrintf( d, tw, c, Xcil_black_gc, "\nNOW INPUTS=\"%s\"", buf );

  return 0;
}



long scroll_hdr( d, w, c, e, p, offset )
     Display *d;
     Window w;
     XContext c;
     XEvent *e;
     Window p;
     double offset;
{
  Window tw;
  char buf[1024];

  tw = XcilPanelItemWindow( d, p, c, "tx" );

  XcilTextPrintf( d, tw, c, Xcil_black_gc, "\nNOW SCROLL=%.5f", offset );

  return 0;
}
