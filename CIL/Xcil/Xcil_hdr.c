/*
 * filename : Xcil_hdr.c
 * project  : Xcil Project
 * author   : Takahiro Sugiyama
 * date     : 1993/10/19
 */



#include "Xcil.h"



long Xcil_hdr_quit
  _P6 (( Display *, display      ),
       ( Window   , window       ),
       ( XContext , context      ),
       ( XEvent * , event        ),
       ( caddr_t  , client_value ),
       ( caddr_t  , server_value ))
{
  return 1;
}



long Xcil_hdr_map_popup
  _P6 (( Display *, display      ),
       ( Window   , window       ),
       ( XContext , context      ),
       ( XEvent * , event        ),
       ( caddr_t  , popup_window ),
       ( caddr_t  , server_value ))
{
  XMoveWindow(display,(Window)popup_window,
	      event->xbutton.x_root-10,event->xbutton.y_root-32);
  XMapRaised(display,(Window)popup_window);
  XFlush(display);

  return 0;
}



long Xcil_hdr_unmap_popup
  _P6 (( Display *, display      ),
       ( Window   , window       ),
       ( XContext , context      ),
       ( XEvent * , event        ),
       ( caddr_t  , popup_window ),
       ( caddr_t  , server_value ))
{
  XUnmapWindow(display,(Window)popup_window);
  return 0;
}
