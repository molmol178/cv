/*
 * filename : open_dialog.c
 * author   : Takahiro Sugiyama
 * date     : Thursday, January 12 1995
 * describe : open dialog routines
 */



#include "voxeldisp.h"



#define odWidthList 200
#define odHeightFont 16
#define odTextLine 10
#define odHeightList (odHeightFont*odTextLine + 4)
#define odWidthPath (odWidthList + odWidthBar + 1 - 40 )
#define odWidthBar 20
#define odHeightBar (odHeightList - odWidthBar*2 - 2)



/*====================================*
 * get_path_list : パスリストを得る。
 *====================================*/
void get_path_list
  P1 (( t_environment *, environment ))

/*========================================*
 * set_path_list : パスリストを設定する。
 *========================================*/
void set_path_list
  P1 (( t_environment *, environment ))

/*=========================================*
 * popup_list : ポップアップリストが入る。
 *=========================================*/
static XcilPopupMenuItem popup_list[32];
long npopup_list;


/*-------------------------------------------------------*
 * cancel_hdr : キャンセルボタンが押された時のハンドラ。
 *-------------------------------------------------------*/
static long cancel_hdr
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( XEvent * , event   ),
       ( t_environment *, environment ))
{
  XUnmapWindow( display, environment->open_dialog );
  return 0;
}


static char *messages_bad_file[4];


/*------------------------------------------*
 * open_file_hdr : ファイルをオープンする。
 *------------------------------------------*/
static long open_file_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  long select_file;
  long nfile;
  char filename[512];
  struct stat sbuf;

  Display *display;
  Window open_dialog;
  XContext context;

  select_file = environment->select_file;
  nfile = environment->nfile;

  if ( select_file < 0 ) return 0;
  if ( select_file >= nfile ) return 0;

  display = environment->display;
  context = environment->context;
  open_dialog = environment->open_dialog;

  sprintf( filename, "%s/%s", environment->cwd, environment->file_list[select_file] );
  stat( filename, &sbuf );
  if (( sbuf.st_mode & S_IFMT ) == S_IFDIR )
    {/* 選択ファイルがディレクトリ */
      chdir( filename );
      environment->select_file = -1;
      get_path_list( environment );
      XcilSetString( display, open_dialog, context, "file",
		    environment->path_list[environment->npath-1] );
      set_path_list( environment );
    }
  else
    {/* 選択ファイルがファイル */
      if ( Voxel.load( environment->object, filename ) == 0 )
	{
	  static char buf[256];
	  messages_bad_file[0] = "Voxeldisp:error: bad file type";
	  messages_bad_file[1] = buf;
	  sprintf( buf, "cannot open file ... \"%s\"", filename );
	  XcilMessages( display, 0, context, 0, 0, "Error", messages_bad_file );
	  return 0;
	}
      strcopy( environment->name_of_input, filename );
      XUnmapWindow( display, open_dialog );
      XClearWindow( display, environment->window );
      XFlush( display );
      environment->scale_index = INITIAL_SCALE_INDEX;
      environment->scale = 5;
      all_draw_voxel_object( environment );
      return 0;
    }


  return 0;
}



/*------------------------------------------*
 * invert_filename : ファイル名を反転する。
 *------------------------------------------*/
static void invert_filename
  _P1 (( t_environment *, environment ))
{
  Display *display;
  XContext context;
  Window list_window;
  Window open_dialog;
  long index;
  long sb_offset;
  long x, y, xsize, ysize;

  if ( environment->select_file >= environment->nfile ) return;
  if ( environment->select_file < 0 ) return;

  sb_offset = environment->sb_offset;
  index = environment->select_file - sb_offset;
  if ((index < 0) || (index >= (sb_offset + 10))) return;

  display = environment->display;
  context = environment->context;
  open_dialog = environment->open_dialog;
  list_window = XcilPanelItemWindow( display, open_dialog, context, "list" );

  x = 0;
  y = (index * odHeightFont) + 1;
  xsize = odWidthList;
  ysize = odHeightFont + 2;
  XFillRectangle( display, list_window, Xcil_invert_gc, x, y, xsize, ysize );
  XFlush( display );
}

/*-------------------------------------*
 * select_hdr : ファイルの選択をする。
 *-------------------------------------*/
static long select_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  long yoffset;
  long new_select;

  yoffset = (e->xbutton.y - 2) / odHeightFont;
  new_select = environment->sb_offset + yoffset;
  if ( new_select == environment->select_file )
    return open_file_hdr( d, w, c, e, environment );

  invert_filename( environment );
  environment->select_file = new_select;
  if ( environment->select_file >= environment->nfile )
    environment->select_file = -1;
  if ( environment->select_file < 0 ) environment->select_file = -1;
  invert_filename( environment );

  return 0;
}





/*-------------------------------------------*
 * pr_file_list : ファイルリストを表示する。
 *-------------------------------------------*/
static void pr_file_list
  _P1 (( t_environment *,environment ))
{
  int i;
  long sb_offset;
  Window open_dialog;
  Window text_window;
  Display *display;
  XContext context;

  display = environment->display;
  context = environment->context;

  open_dialog = environment->open_dialog;
  text_window = XcilPanelItemWindow( display, open_dialog, context, "list" );
  sb_offset = environment->sb_offset;

  XcilTextClear( display, text_window, context );
  XClearWindow( display, text_window );
  for ( i = 0; (i < odTextLine)&&((i+sb_offset) < environment->nfile); i++ )
    {
      struct stat sbuf;
      char filename[512];
      sprintf( filename, "%s/%s", environment->cwd, environment->file_list[i+sb_offset] );
      stat( filename, &sbuf );
      if ( i ) XcilTextNextLine( display, text_window, context );
      if (( sbuf.st_mode & S_IFMT ) == S_IFDIR )
	XcilTextPrintf( display, text_window, context, Xcil_black_gc,
		        "%s/",environment->file_list[i+sb_offset]);
      else
	XcilTextPrintf( display, text_window, context, Xcil_black_gc,
		       "%s",environment->file_list[i+sb_offset]);
   }
  invert_filename( environment );
}



/*-------------------------------------------*
 * sb_shift_hdr : スクロールバーのハンドラ。
 *-------------------------------------------*/
static long sb_shift_hdr
  _P6 (( Display *, display ),
       ( Window   , window ),
       ( XContext , context ),
       ( XEvent * , event ),
       ( t_environment *, environment ),
       ( double, bar_position ))
{
  long sb_offset;
  int i;
  Window text_window;
  Window open_dialog;

  sb_offset = bar_position * environment->nfile - 10;
  if ( sb_offset < 0 ) sb_offset = 0;

  environment->sb_offset = sb_offset;
  pr_file_list( environment );

  return 0;
}



/*-------------------------------------------*
 * sb_shiftup_hdr : スクロールバーのハンドラ。
 *-------------------------------------------*/
static long sb_shiftup_hdr
  _P5 (( Display *, display ),
       ( Window   , window ),
       ( XContext , context ),
       ( XEvent * , event ),
       ( t_environment *, environment ))
{
  Window open_dialog;
  Window sb_window;
  long sb_offset;
  double position;
  double bar_size;

  open_dialog = environment->open_dialog;

  sb_offset = environment->sb_offset - 1;
  if ( sb_offset < 0 ) return 0;

  sb_window = XcilPanelItemWindow( display, open_dialog, context, "bar" );
  position = XcilScrollBarGetPosition( display, sb_window, context );

  position = (sb_offset + 10.0) / environment->nfile;
  XcilScrollBarSetPosition( display, sb_window, context, position );

  environment->sb_offset = sb_offset;
  pr_file_list( environment );

  return 0;
}



/*-------------------------------------------*
 * sb_shiftdown_hdr : スクロールバーのハンドラ。
 *-------------------------------------------*/
static long sb_shiftdown_hdr
  _P5 (( Display *, display ),
       ( Window   , window ),
       ( XContext , context ),
       ( XEvent * , event ),
       ( t_environment *, environment ))
{
  Window open_dialog;
  Window sb_window;
  long sb_offset;
  double position;
  double bar_size;

  open_dialog = environment->open_dialog;

  sb_offset = environment->sb_offset + 1;
  if ( sb_offset > (environment->nfile-10) ) return 0;

  sb_window = XcilPanelItemWindow( display, open_dialog, context, "bar" );
  position = XcilScrollBarGetPosition( display, sb_window, context );

  position = (sb_offset + 10.0) / environment->nfile;
  XcilScrollBarSetPosition( display, sb_window, context, position );

  environment->sb_offset = sb_offset;
  pr_file_list( environment );

  return 0;
}



/*-----------------------------------------------------*
 * change_dir_hdr : ディレクトリの移動をするハンドラ。
 *-----------------------------------------------------*/
static long change_dir_hdr
  _P7 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ),
       ( long, index ),
       ( XcilPopupMenuItem *, list ))
{
  char newdir[512];
  int i, n;

  newdir[0] = '\0';

  if ( index == 0 ) return 0;

  strcat( newdir, "/" );
  for ( i = npopup_list-2; i >= index; i-- )
    {
      strcat( newdir, list[i].string );
      strcat( newdir, "/" );
    }
  chdir( newdir );
  get_path_list( environment );
  if ( environment->npath > 0 )
    XcilSetString( environment->display, environment->open_dialog,
		  environment->context, "file",
		  environment->path_list[environment->npath-1] );
  else
    XcilSetString( environment->display, environment->open_dialog,
		  environment->context, "file","<root>");

  set_path_list( environment );

  return 0;
}



/*---------------------------------------------------------------*
 * down_dir_hdr : ディレクトリを一階層下に移動をするハンドラ。
 *---------------------------------------------------------------*/
static long down_dir_hdr
  _P5 (( Display *, d ),
       ( Window   , w ),
       ( XContext , c ),
       ( XEvent * , e ),
       ( t_environment *, environment ))
{
  char newdir[512];
  int i, n;
  char **list;
  long npath;

  strcopy( newdir, "/" );

  list = environment->path_list;
  npath = environment->npath;

  for ( i = 0; i < npath-1; i++ )
    {
      strcat( newdir, list[i] );
      strcat( newdir, "/" );
    }
  chdir( newdir );
  get_path_list( environment );
  if ( environment->npath > 0 )
    XcilSetString( environment->display, environment->open_dialog,
		  environment->context, "file",
		  environment->path_list[environment->npath-1] );
  else
    XcilSetString( environment->display, environment->open_dialog,
		  environment->context, "file","<root>");

  set_path_list( environment );

  return 0;
}



#include "bitmap/sb_up"
#include "bitmap/sb_down"

static XcilPanelItem open_dialog_list[] = {
  {{"file",__TOP,__TOP,odWidthPath,0,50,30,1},
     {XCIL_3D_LABEL,"PathLists"},{0,0}},

  {{"list",__TOP,__PRE,odWidthList,odHeightList,30,10,1},
     {XCIL_LABEL,""}, {(xcil_callback)select_hdr, 0}},

  {{"up"  ,__PRE,__SAM,odWidthBar,odWidthBar,-1,10,1},
     {XCIL_BUTTON,"","cc",(char *)sb_up_bits,sb_up_width,sb_up_height},
     {(xcil_callback)sb_shiftup_hdr,0}},
  {{"bar" ,__SAM,__PRE,odWidthBar,odHeightBar,-1,-1,1},{XCIL_LABEL,""},
     {0,0}},
  {{"down",__SAM,__PRE,odWidthBar,odWidthBar,-1,-1,1},
     {XCIL_BUTTON,"","cc",(char *)sb_down_bits,sb_down_width,sb_down_height},
     {(xcil_callback)sb_shiftdown_hdr,0}},

  {{"cd..",__PRE,__TOP,120,0,30,46,0},
     {XCIL_LABEL,"macintosh HD"},
     {(xcil_callback)down_dir_hdr, 0}},

  {{"skip",__SAM,__PRE,120,1,30,30,2},
     {XCIL_LABEL,"","cc"}, {0, 0}},

  {{"open",__SAM,__PRE,120,30,30,50,2},
     {XCIL_BUTTON,"Open","cc"}, {(xcil_callback)open_file_hdr, 0}},

  {{"canc",__SAM,__PRE,120,30,30,10,2},
     {XCIL_BUTTON,"Cancel","cc"}, {(xcil_callback)cancel_hdr, 0}},
};



/*==========================================================*
 * add_open_dialog : ファイルオープンダイアログを設定する。
 *==========================================================*/
void add_open_dialog
  _P1 (( t_environment *, environment ))
{
  int i;
  Display *display;
  XContext context;
  Window open_dialog;
  Window text_window;
  Window sb_window;
  long nopen_dialog;

  environment->all_file_list = 0;
  environment->nfile = 0;
  environment->select_file = -1;

  get_path_list( environment );
  nopen_dialog = XcilNumber( open_dialog_list );
  for ( i = 0; i < nopen_dialog; i++ )
    open_dialog_list[i].hdr.client_value = (caddr_t)environment;
  open_dialog_list[0].act.string = environment->path_list[environment->npath-1];

  display = environment->display;
  context = environment->context;

  open_dialog = XcilCreatePanelWindow( display, 0, context, 0, 0,
				      open_dialog_list, nopen_dialog,
				      "File Open Dialog");
  XImageSetColormap( display, open_dialog );

  environment->open_dialog = open_dialog;
  text_window = XcilPanelItemWindow( display, open_dialog, context, "list" );
  XcilTextSet( display, text_window, context );
  XDefineCursor( display, text_window, Xcil_pointer_cursor );

  sb_window = XcilPanelItemWindow( display, open_dialog, context, "bar" );
  XcilScrollSet( display, sb_window, context, sb_shift_hdr, environment );
  XcilScrollBarSetLength( display, sb_window, context, 10.0/environment->nfile );
  XcilScrollBarSetViewLength( display, sb_window, context, 1.0 );
  set_path_list( environment );
}



static int compare_str
  _P2 (( char **, a ),
       ( char **, b ))
{
  return strcmp( *a, *b );
}

/*========================================*
 * get_path_list : パスリストを設定する。
 *========================================*/
void get_path_list
  _P1 (( t_environment *, environment ))
{
  char **path_list;
  static char cwd[512];
  long i, npath;

  path_list = environment->path_list;

  if ( getcwd( cwd, 510 ) == 0 )
    {
      perror("pwd");
      exit(1);
    }
  strcopy( environment->cwd, cwd );
  npath = strsplit( cwd, path_list, "/" );
  environment->npath = npath;

  if ( environment->all_file_list ) fn_freelist( environment->all_file_list );
  environment->all_file_list = fn_list( environment->cwd, &(environment->nfile) );
  environment->file_list = environment->all_file_list + 2;
  environment->nfile -= 2;
  qsort( environment->file_list, environment->nfile, sizeof(char *), compare_str );
}



/*========================================*
 * set_path_list : パスリストを設定する。
 *========================================*/
void set_path_list
  _P1 (( t_environment *, environment ))
{
  char **path_list;
  long i, npath;

  Display *display;
  XContext context;
  Window panel;
  Window popup_window;
  Window text_window;
  Window sb_window;
  double bar_size;

  path_list = environment->path_list;
  npath = environment->npath;
  for ( i = 0; i < npath; i++ )
    {
      popup_list[i].string = path_list[npath - 1 - i];
      popup_list[i].callback = change_dir_hdr;
      popup_list[i].client_value = (caddr_t)environment;
    }
  popup_list[i].string = "<root>";
  popup_list[i].callback = change_dir_hdr;
  popup_list[i].client_value = (caddr_t)environment;
  npopup_list = npath + 1;

  display = environment->display;
  context = environment->context;
  panel = environment->open_dialog;
  popup_window = XcilPanelItemWindow( display, panel, context, "file" );

  XcilPopupSet( display, popup_window, context, Button1,
	        popup_list, npopup_list );

  /*------------------------*
   * スクローロバーの再設定
   *------------------------*/
  sb_window = XcilPanelItemWindow( display, panel, context, "bar" );
  bar_size = 10.0/environment->nfile;
  XcilScrollBarSetLength( display, sb_window, context, bar_size );
  XcilScrollBarSetViewLength( display, sb_window, context, 1.0 );
  XcilScrollBarSetPosition( display, sb_window, context, bar_size );
  environment->sb_offset = 0;
  pr_file_list( environment );
}
