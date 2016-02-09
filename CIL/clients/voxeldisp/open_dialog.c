/*
 * filename : open_dialog.c
 * author   : Takahiro Sugiyama
 * date     : Thursday, January 12 1995
 * describe : open dialog routines
 */



#include "voxeldisp.h"


/*------------------------------------------*
 * file_dialog_hdr : ファイルをオープンする。
 *------------------------------------------*/
static long file_dialog_hdr
  _P6 (( Display *                 , display     ),
       ( Window                    , window      ),
       ( XContext                  , context     ),
       ( XEvent *                  , event       ),
       ( t_environment *           , environment ),
       ( XcilFileDialogAttributes *, attribute   ))
{
  char *filename;

  filename = attribute->fullname;
  if ( Voxel.load( environment->object, filename ) == 0 )
    {
      static char *messages[4];
      static char buf[256];
      messages[0] = "Voxeldisp:error: bad file type";
      messages[1] = buf;
      sprintf( buf, "cannot open file ... \"%s\"", filename );
      XcilMessages( display, 0, context, 0, 0, "Error", messages );
      return 0;
    }
  strcopy( environment->name_of_input, filename );
  XUnmapWindow( display, window );
  XClearWindow( display, environment->window );
  XFlush( display );
  environment->scale_index = INITIAL_SCALE_INDEX;
  environment->scale = 5;

  all_draw_voxel_object( environment );

  return 0;
}




/*==========================================================*
 * add_open_dialog : ファイルオープンダイアログを設定する。
 *==========================================================*/
void add_open_dialog
  _P1 (( t_environment *, environment ))
{
  Display *display;
  XContext context;
  Window window;
  XcilFileDialogAttributes *attribute;

  display = environment->display;
  context = environment->context;

  attribute = &environment->attribute;

  attribute->title = "Please choose a voxel file:";
  attribute->mode = FDMOpen|FDMCheck;
  attribute->callback = (xcil_file_dialog_callback)file_dialog_hdr;
  attribute->client_value = (caddr_t)environment;
  attribute->filetype_num = 0;
  attribute->filetype_list = 0;
  attribute->filetype_index = -1;

  window = XcilCreateFileDialog( display, 0, context, 0, 0, attribute );
  XImageSetColormap( display, window );

  environment->open_dialog = window;
}



/*------------------------------------------*
 * save_dialog_hdr : ファイルをセーブする。
 *------------------------------------------*/
static long save_dialog_hdr
  _P6 (( Display *                 , display     ),
       ( Window                    , window      ),
       ( XContext                  , context     ),
       ( XEvent *                  , event       ),
       ( t_environment *           , environment ),
       ( XcilFileDialogAttributes *, attribute   ))
{
  char *filename;

  filename = attribute->fullname;

  strcopy( environment->name_of_output, filename );
  XUnmapWindow( display, window );
  write_postscript( environment );

  return 0;
}




/*==========================================================*
 * add_save_dialog : ファイルセーブダイアログを設定する。
 *==========================================================*/
void add_save_dialog
  _P1 (( t_environment *, environment ))
{
  Display *display;
  XContext context;
  Window window;
  XcilFileDialogAttributes *attribute;

  display = environment->display;
  context = environment->context;

  attribute = &environment->save_attribute;

  attribute->title = "Please specify a PostScript file:";
  attribute->mode = FDMSave|FDMCheck;
  attribute->callback = (xcil_file_dialog_callback)save_dialog_hdr;
  attribute->client_value = (caddr_t)environment;
  attribute->filetype_num = 0;
  attribute->filetype_list = 0;
  attribute->filetype_index = -1;

  window = XcilCreateFileDialog( display, 0, context, 0, 0, attribute );
  XImageSetColormap( display, window );

  environment->save_dialog = window;
}
