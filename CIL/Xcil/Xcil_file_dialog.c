/*
 * filename : Xcil_file_dialog.c
 * author   : Takahiro Sugiyama
 * date     : Thursday, January 12 1995
 * describe : file dialog routines like Macintosh
 */



#include <stdlib.h>
#include "Xcil/Xcil.h"
#include "misc/filelib.h"
#include "misc/memlib.h"
#include "misc/strlib.h"



#define odWidthList 200
#define odHeightFont 16
#define odTextLine 10
#define odHeightList (odHeightFont*odTextLine + 4)
#define odWidthPath (odWidthList + odWidthBar + 1 - 40 )
#define odWidthBar 20
#define odHeightBar (odHeightList - odWidthBar*2 - 2)

#define OD_X_BUTTON 20
#define OD_Y_BUTTON 10
#define OD_W_BUTTON 120
#define OD_H_BUTTON 25

#define odWidthTitle (odWidthList+2 + odWidthBar + OD_W_BUTTON+4 +OD_X_BUTTON)



/*====================================*
 * get_path_list : パスリストを得る。
 *====================================*/
static void get_path_list
  P1 (( XcilFileDialogAttributes *, attribute ))

/*========================================*
 * set_path_list : パスリストを設定する。
 *========================================*/
static void set_path_list
  P3 (( Display *                 , display   ),
      ( XContext                  , context   ),
      ( XcilFileDialogAttributes *, attribute ))



/*-------------------------------------------------------*
 * cancel_hdr : キャンセルボタンが押された時のハンドラ。
 *-------------------------------------------------------*/
static long cancel_hdr
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( XEvent * , event   ),
       ( XcilFileDialogAttributes *, attribute ))
{
  XUnmapWindow( display, attribute->window );

  if ( attribute->cancel_callback )
    {
      return attribute->cancel_callback( display, attribute->window, context,
					 event,
					 attribute->client_value, attribute );
    }

  return 0;
}



/*------------------------------------------------*
 * error_message : エラー・メッセージを表示する。
 *------------------------------------------------*/
static void error_message
  _P5 (( Display *, display  ),
       ( XContext , context  ),
       ( char *   , message  ),
       ( char *   , filename ),
       ( long     , icn32    ))
{
  static char *messages[4];
  static char buf0[1024], buf1[1024];

  if (Xcil_Japanese_message == 0)
    {
      sprintf( buf0, " FileDialog:error:%s: ", message );
      sprintf( buf1, "            ... \"%s\" ", filename );
    }
  else
    {
      sprintf( buf0, " ファイル・ダイアログ:エラー:%s: ", message );
      sprintf( buf1, "            ... \"%s\" ", filename );
    }
  messages[0] = buf0;
  messages[1] = buf1;
  messages[2] = 0;
  XcilErrorMessages( display, 0, context, 0, 0, "FileDialogError", messages,
		     icn32 );
}



/*------------------------------------------*
 * open_file_hdr : ファイルをオープンする。
 *------------------------------------------*/
static long open_file_hdr
  _P6 (( Display *                 , display   ),
       ( Window                    , bwindow   ),
       ( XContext                  , context   ),
       ( XEvent *                  , event     ),
       ( XcilFileDialogAttributes *, attribute ),
       ( caddr_t                   , dummy     ))
{
  long select_file;
  long file_num;
  char filename[ 512 ];
  struct stat sbuf;

  Window window;
  caddr_t client_value = attribute->client_value;
  xcil_file_dialog_callback callback = attribute->callback;
  char *current_dir;
  char *file_name;
  long exist_flag, read_ok, write_ok;

  /* 選択ファイルのチェック */
  select_file = attribute->select_file;
  file_num = attribute->file_num;
  if (( select_file == -1 ) && (( attribute->mode & FDMText ) == 0 ))
    return 0;

  /* フルパスでファイル名を構成する */
  window = attribute->window;
  current_dir = attribute->current_dir;
  file_name   = attribute->filename;
  if (( select_file == -1 ) && ( attribute->mode & FDMText ))
    {
      Window text;
      text = XcilPanelItemWindow( display, window, context, "name" );
      XcilTextGetInput( display, text, context, attribute->filename, 512 );
    }

  if ( strequal( current_dir, "/" ) )
    sprintf( filename, "/%s", file_name );
  else
    sprintf( filename, "%s/%s", current_dir, file_name );

  exist_flag = access( filename, F_OK );
  stat( filename, &sbuf );
  if (( exist_flag == 0 ) && (( sbuf.st_mode & S_IFMT ) == S_IFDIR ))
    {/* 選択ファイルがディレクトリ */
      long  path_num;
      char *path_name;
      read_ok = access( filename, R_OK );
      if ( read_ok == -1 )
	{
	  if (Xcil_Japanese_message == 0)
	    error_message( display, context,
			  "cannot open this directory", filename, ICN32Stop );
	  else
	    error_message( display, context,
			  "このディレクトリは開くことができません。",
			  filename, ICN32Stop );
	  return 0;
	}
      chdir( filename );
      attribute->select_file = -1;
      get_path_list( attribute );
      path_num  = attribute->path_num;
      path_name = attribute->path_list[ path_num - 1 ];
      XcilSetString( display, window, context, "file", path_name );
      set_path_list( display, context, attribute );
      return 0;
    }

  /* 選択ファイルがファイル */

  /* ファイルの存在をチェックする */
  if ( attribute->mode & FDMCheck )
    if ( attribute->mode & FDMSave )
      {/* セーブする時はファイルが書き込めるか／存在するかを確認をする */
	long confirm;
	if ( exist_flag == 0 )
	  {/* ファイルが存在している */
	    if (Xcil_Japanese_message == 0)
	      confirm = XcilConfirm( display, 0, context, 0, 0,
				    "Confirm Overwrite",
				    "File exists, Overwrite OK ?" );
	    else
	      confirm = XcilConfirm( display, 0, context, 0, 0,
				    "Confirm Overwrite",
				    "ファイルがすでに存在しています。上書きしても言いですか？" );
	    if ( confirm == XCIL_CANCEL ) return 0;
	    /* 存在しているファイルに書き込みができない */
	    write_ok = access( filename, W_OK );
	    if ( write_ok == -1 ) /* 書き込めない */
	      {
		if (Xcil_Japanese_message == 0)
		  error_message( display, context,
				"cannot write this file",
				filename, ICN32Caution );
		else
		  error_message( display, context,
				"このファイルは書き込むことができません",
				filename, ICN32Caution );
		return 0;
	      }
	  }
	else
	  {/* ディレクトリの書き込みチェック */
	    write_ok = access( current_dir, W_OK );
	    if ( write_ok == -1 ) /* 書き込めない */
	      {
		if (Xcil_Japanese_message == 0)
		  error_message( display, context,
				"cannot write this directory",
				current_dir, ICN32Caution );
		else
		  error_message( display, context,
				"このディレクトリは書き込みできません",
				current_dir, ICN32Caution );
		return 0;
	      }
	  }
      }
    else
      {/* オープンする時はファイルが存在しない時に警告を出す */
	if ( exist_flag == -1 )
	  {
	    if (Xcil_Japanese_message == 0)
	      error_message( display, context,
			    "not exist this file", filename, ICN32Caution );
	    else
	      error_message( display, context,
			    "このファイルは存在しません",
			    filename, ICN32Caution );
	    return 0;
	  }
	read_ok = access( filename, R_OK );
	if ( read_ok == -1 ) /* 読み込めない */
	  {
	    if (Xcil_Japanese_message == 0)
	      error_message( display, context,
			    "cannot read this file", filename, ICN32Caution );
	    else
	      error_message( display, context,
			    "このファイルを読むことができません",
			    filename, ICN32Caution );
	    return 0;
	  }
      }
  strcopy( attribute->fullname, filename );

  return callback( display, window, context, event, client_value, attribute );
}



/*------------------------------------------------------------*
 * return_callback : リターンキーが押された時のコールバック。
 *------------------------------------------------------------*/
static long return_callback
  _P5 (( Display *                 , display   ),
       ( Window                    , window    ),
       ( XContext                  , context   ),
       ( XEvent *                  , event     ),
       ( XcilFileDialogAttributes *, attribute ))
{
  char filename[512];

  /* 入力されたファイルを読み込む */
  XcilTextGetInput( display, window, context, filename, 512 );
  strcopy( attribute->filename, filename );

  return open_file_hdr( display, window, context, event, attribute, 0 );
}



/*----------------------------------------------*
 * desktop_hdr : デスクトップに移動するハンドラ
 *----------------------------------------------*/
static long desktop_hdr
  _P5 (( Display *, display ),
       ( Window   , bwindow ),
       ( XContext , context ),
       ( XEvent * , event   ),
       ( XcilFileDialogAttributes *, attribute ))
{
  Window window;

  if ( attribute->desktop_callback )
    {
      return attribute->desktop_callback( display, attribute->window, context,
					 event,
					 attribute->client_value, attribute );
    }

  chdir( "/" );
  get_path_list( attribute );

  window = attribute->window;
  if ( attribute->path_num > 0 )
    XcilSetString( display, window, context, "file",
		   attribute->path_list[attribute->path_num-1] );
  else
    XcilSetString( display, window, context, "file","<root>");
  attribute->select_file = -1;
  set_path_list( display, context, attribute );

  return 0;
}



/*----------------------------------------------*
 * home_hdr : ホームに移動するハンドラ
 *----------------------------------------------*/
static long home_hdr
  _P5 (( Display *, display ),
       ( Window   , bwindow ),
       ( XContext , context ),
       ( XEvent * , event   ),
       ( XcilFileDialogAttributes *, attribute ))
{
  char *pathname;
  Window window;

  if ( attribute->home_callback )
    {
      return attribute->home_callback( display, attribute->window, context,
				      event,
				      attribute->client_value, attribute );
    }

  pathname = getenv( "HOME" );

  chdir( pathname );
  get_path_list( attribute );

  window = attribute->window;
  if ( attribute->path_num > 0 )
    XcilSetString( display, window, context, "file",
		   attribute->path_list[attribute->path_num-1] );
  else
    XcilSetString( display, window, context, "file","<root>");
  attribute->select_file = -1;
  set_path_list( display, context, attribute );

  return 0;
}



/*------------------------------------------*
 * invert_filename : ファイル名を反転する。
 *------------------------------------------*/
static void invert_filename
  _P3 (( Display *                 , display   ),
       ( XContext                  , context   ),
       ( XcilFileDialogAttributes *, attribute ))
{
  Window window;
  Window list_window;
  long index;
  long select_offset;
  long select_file;
  long file_num;
  long x, y, xsize, ysize;

  /* 選択ファイルのチェック */
  select_file = attribute->select_file;
  file_num = attribute->file_num;
  if (( select_file < 0 ) || ( select_file >= file_num )) return;

  /* 表示のインデックスのチェック */
  select_offset = attribute->select_offset;
  index = select_file - select_offset;
  if (( index < 0 ) || ( index >= ( select_offset + 10 ))) return;

  window = attribute->window;
  list_window = XcilPanelItemWindow( display, window, context, "list" );

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
  _P5 (( Display *                 , display   ),
       ( Window                    , window    ),
       ( XContext                  , context   ),
       ( XEvent *                  , event     ),
       ( XcilFileDialogAttributes *, attribute ))
{
  long offset;
  long select_file;
  long file_num;

  /* ウィンドウ座標からインデックスに変換する */
  offset = (event->xbutton.y - 2) / odHeightFont;
  select_file = attribute->select_offset + offset;

  /* もし選択されているファイルを選択したらオープンする */
  if ( select_file == attribute->select_file )
    return open_file_hdr( display, window, context, event, attribute, 0 );

  /* 選択ファイルのチェック */
  file_num = attribute->file_num;
  if (( select_file < 0 ) || ( select_file >= file_num )) select_file = -1;

  /* 選択ファイルの書き直し */
  invert_filename( display, context, attribute );
  attribute->select_file = select_file;
  invert_filename( display, context, attribute );

  if ( select_file == -1 )
    {
      strcopy( attribute->filename, "" );
    }
  else
    {
      char *fullname[512];
      char *current_dir = attribute->current_dir;
      char *filename    = attribute->filename;
      struct stat sbuf;

      strcopy( attribute->filename, attribute->file_list[ select_file ] );
      sprintf( fullname, "%s/%s", current_dir, filename );
      stat( fullname, &sbuf );
      if (( attribute->mode & FDMText ) &&
	  (( sbuf.st_mode & S_IFMT ) != S_IFDIR ))
	{
	  Window panel = attribute->window;
	  Window text = XcilPanelItemWindow( display, panel, context, "name" );
	  XcilTextClear( display, text, context );
	  XClearWindow( display, text );
	  XcilTextSetInput( display, text, context, filename );
	}
    }

  return 0;
}



/*---------------------------------------------------------------*
 * key_release_hdr : テキストウィンドウでキーボードが押された。
 *---------------------------------------------------------------*/
static long key_release_hdr
  _P5 (( Display *, display ),
       ( Window   , window  ),
       ( XContext , context ),
       ( XEvent * , event   ),
       ( XcilFileDialogAttributes *, attribute ))
{
  /* 選択ファイルの書き直し */
  invert_filename( display, context, attribute );
  attribute->select_file = -1;

  return 0;
}




/*-------------------------------------------*
 * pr_file_list : ファイルリストを表示する。
 *-------------------------------------------*/
static void pr_file_list
  _P3 (( Display *                 , display  ),
       ( XContext                  , context  ),
       ( XcilFileDialogAttributes *,attribute ))
{
  int i;
  long select_offset;
  long file_num;
  Window window;
  Window text_window;
  GC gc;

  window = attribute->window;
  gc = Xcil_black_gc;
  text_window = XcilPanelItemWindow( display, window, context, "list" );

  select_offset = attribute->select_offset;
  file_num = attribute->file_num;

  XcilTextClear( display, text_window, context );
  XClearWindow( display, text_window );
  for ( i = 0; ( i < odTextLine ) && (( i + select_offset ) < file_num ); i++ )
    {
      struct stat sbuf;
      char *current_dir = attribute->current_dir;
      char *filename = attribute->file_list[ i + select_offset ];
      char pathname[512];

      sprintf( pathname, "%s/%s", current_dir, filename );
      stat( pathname, &sbuf );
      if ( i ) XcilTextNextLine( display, text_window, context );
      if (( sbuf.st_mode & S_IFMT ) == S_IFDIR )
	{
	  XcilTextPutIcon16( display, text_window, context, gc, ICN16Folder);
	}
      else
      if (( sbuf.st_mode & S_IFMT ) == S_IFLNK )
	{
	  XcilTextPutIcon16( display, text_window, context, gc, ICN16SymLnk);
	}
      else
      if ( sbuf.st_mode & S_IEXEC )
	{
	  XcilTextPutIcon16( display, text_window, context, gc, ICN16App);
	}
      else
	{
	  XcilTextPutIcon16( display, text_window, context, gc, ICN16Doc);
	}
      XcilTextPrintf( display, text_window, context, gc, filename);
   }
  invert_filename( display, context, attribute );
}



/*----------------------------------------*
 * expose_list_hdr : リストを再表示する。
 *----------------------------------------*/
static long expose_list_hdr
  _P5 (( Display *, display ),
       ( Window   , bwindow ),
       ( XContext , context ),
       ( XEvent * , event   ),
       ( XcilFileDialogAttributes *, attribute ))
{
  pr_file_list( display, context, attribute );

  return 0;
}



/*----------------------------------------*
 * map_notify_hdr : リストを再表示する。
 *----------------------------------------*/
static long map_notify_hdr
  _P5 (( Display *, display ),
       ( Window   , bwindow ),
       ( XContext , context ),
       ( XEvent * , event   ),
       ( XcilFileDialogAttributes *, attribute ))
{
  Window window;

  chdir(attribute->current_dir);
  get_path_list( attribute );

  window = attribute->window;
  if ( attribute->path_num > 0 )
    XcilSetString( display, window, context, "file",
		   attribute->path_list[attribute->path_num-1] );
  else
    XcilSetString( display, window, context, "file","<root>");
  set_path_list( display, context, attribute );

  if (attribute->mode & FDMText)
    {
      Window text = XcilPanelItemWindow( display, bwindow, context, "name" );
      XcilTextClear( display, text, context );
      XClearWindow( display, text );
      XcilTextSetInput( display, text, context, attribute->filename );
    }

  {/* スクロールバーの再表示 */
    Window sb_window = XcilPanelItemWindow( display, bwindow, context, "bar" );
    double position = XcilScrollBarGetPosition( display, sb_window, context );
    XcilScrollBarSetPosition( display, sb_window, context, position );
  }

  return 0;
}



/*-------------------------------------------*
 * sb_shift_hdr : スクロールバーのハンドラ。
 *-------------------------------------------*/
static long sb_shift_hdr
  _P6 (( Display *                 , display      ),
       ( Window                    , window       ),
       ( XContext                  , context      ),
       ( XEvent *                  , event        ),
       ( XcilFileDialogAttributes *, attribute    ),
       ( double                    , bar_position ))
{
  int i;
  long select_offset;

  select_offset = bar_position * attribute->file_num - 5;
  if ( select_offset < 0 ) select_offset = 0;
  attribute->select_offset = select_offset;
  pr_file_list( display, context, attribute );

  return 0;
}



/*-------------------------------------------*
 * sb_shiftup_hdr : スクロールバーのハンドラ。
 *-------------------------------------------*/
static long sb_shiftup_hdr
  _P5 (( Display *                 , display   ),
       ( Window                    , bwindow   ),
       ( XContext                  , context   ),
       ( XEvent *                  , event     ),
       ( XcilFileDialogAttributes *, attribute ))
{
  Window window;
  Window sb_window;
  long select_offset;
  double position;
  double bar_size;

  window = attribute->window;

  select_offset = attribute->select_offset - 1;
  if ( select_offset < 0 ) return 0;

  sb_window = XcilPanelItemWindow( display, window, context, "bar" );
  position = XcilScrollBarGetPosition( display, sb_window, context );

  position = ( select_offset + 10.0 ) / attribute->file_num;
  XcilScrollBarSetPosition( display, sb_window, context, position );

  attribute->select_offset = select_offset;
  pr_file_list( display, context, attribute );

  return 0;
}



/*-------------------------------------------*
 * sb_shiftdown_hdr : スクロールバーのハンドラ。
 *-------------------------------------------*/
static long sb_shiftdown_hdr
  _P5 (( Display *                 , display   ),
       ( Window                    , bwindow   ),
       ( XContext                  , context   ),
       ( XEvent *                  , event     ),
       ( XcilFileDialogAttributes *, attribute ))
{
  Window window;
  Window sb_window;
  long select_offset;
  double position;
  double bar_size;

  window = attribute->window;

  select_offset = attribute->select_offset + 1;
  if ( select_offset > ( attribute->file_num - 5 ) ) return 0;

  sb_window = XcilPanelItemWindow( display, window, context, "bar" );
  position = XcilScrollBarGetPosition( display, sb_window, context );

  position = ( select_offset + 10.0 ) / attribute->file_num;
  XcilScrollBarSetPosition( display, sb_window, context, position );

  attribute->select_offset = select_offset;
  pr_file_list( display, context, attribute );

  return 0;
}



/*-----------------------------------------------------*
 * change_dir_hdr : ディレクトリの移動をするハンドラ。
 *-----------------------------------------------------*/
static long change_dir_hdr
  _P7 (( Display *                 , display   ),
       ( Window                    , bwindow   ),
       ( XContext                  , context   ),
       ( XEvent *                  , event     ),
       ( XcilFileDialogAttributes *, attribute ),
       ( long                      , index     ),
       ( XcilPopupMenuItem *       , list      ))
{
  char newdir[512];
  int i, n;
  Window window;
  long popup_num;

  if ( index == 0 ) return 0;

  window = attribute->window;
  popup_num = attribute->popup_num;

  newdir[0] = '\0';
  strcat( newdir, "/" );
  for ( i = popup_num - 2; i >= index; i-- )
    {
      strcat( newdir, list[i].string );
      strcat( newdir, "/" );
    }
  chdir( newdir );
  attribute->select_file = -1;
  get_path_list( attribute );

  if ( attribute->path_num > 0 )
    XcilSetString( display, window, context, "file",
		   attribute->path_list[attribute->path_num-1] );
  else
    XcilSetString( display, window, context, "file","<root>");

  set_path_list( display, context, attribute );

  return 0;
}



/*---------------------------------------------------------------*
 * down_dir_hdr : ディレクトリを一階層下に移動をするハンドラ。
 *---------------------------------------------------------------*/
static long down_dir_hdr
  _P5 (( Display *                 , display   ),
       ( Window                    , bwindow   ),
       ( XContext                  , context   ),
       ( XEvent *                  , event     ),
       ( XcilFileDialogAttributes *, attribute ))
{
  char newdir[512];
  int i, n;
  char **list;
  long path_num;
  Window window;

  if ( attribute->macintosh_callback )
    {
      return attribute->macintosh_callback(display, attribute->window,
					   context,
					   event,
					   attribute->client_value,
					   attribute );
    }

  list = attribute->path_list;
  path_num = attribute->path_num;
  strcopy( newdir, "/" );
  for ( i = 0; i < path_num - 1; i++ )
    {
      strcat( newdir, list[ i ] );
      strcat( newdir, "/" );
    }
  chdir( newdir );
  get_path_list( attribute );

  window = attribute->window;
  list = attribute->path_list;
  path_num = attribute->path_num;
  if ( path_num > 0 )
    XcilSetString( display, window, context, "file", list[ path_num - 1 ] );
  else
    XcilSetString( display, window, context, "file","<root>");
  attribute->select_file = -1;
  set_path_list( display, context, attribute );

  return 0;
}



/*----------------------------------------------------------*
 * change_filetype_hdr : ファイルタイプの変更をするハンドラ。
 *----------------------------------------------------------*/
static long change_filetype_hdr
  _P7 (( Display *                 , display   ),
       ( Window                    , bwindow   ),
       ( XContext                  , context   ),
       ( XEvent *                  , event     ),
       ( XcilFileDialogAttributes *, attribute ),
       ( long                      , index     ),
       ( XcilPopupMenuItem *       , list      ))
{
  Window window;

  attribute->filetype_index = index;
  window = attribute->window;
  XcilSetString( display, window, context, "type",
		 attribute->filetype_list[ index ] );

  return 0;
}



/*---------------------------------------------------------------*
 * set_filetype_popup : ファイルタイプのポップアップを設定する。
 *---------------------------------------------------------------*/
static void set_filetype_popup
  _P3 (( Display *, display ),
       ( XContext , context ),
       ( XcilFileDialogAttributes *, attribute ))
{
  XcilPopupMenuItem *filetype_popup_list;
  Window window, popup;
  long filetype_popup_num;
  char **filetype_list;
  long i, filetype_num;

  filetype_list = attribute->filetype_list;
  filetype_num = attribute->filetype_num;
  filetype_popup_list = typenew1( filetype_num, XcilPopupMenuItem );
  for ( i = 0; i < filetype_num; i++ )
    {
      filetype_popup_list[ i ].string = filetype_list[ i ];
      filetype_popup_list[ i ].callback = change_filetype_hdr;
      filetype_popup_list[ i ].client_value = (caddr_t)attribute;
    }
  window = attribute->window;
  popup = XcilPanelItemWindow( display, window, context, "type" );
  XcilPopupSet( display, popup, context, Button1, filetype_popup_list, filetype_num );
  XcilSetString( display, window, context, "type", filetype_list[ 0 ] );

  attribute->filetype_index = 0;
  attribute->filetype_popup_list = filetype_popup_list;
}



#include "bitmap/sb_up"
#include "bitmap/sb_down"

static XcilPanelItem file_dialog_list_static[] = {
  {{"file",__TOP,__TOP,odWidthPath,0,40,20,1},
     {XCIL_3D_LABEL,"PathLists"},{0,0}},
  {{"list",__TOP,__PRE,odWidthList,odHeightList,20,10,1},
     {XCIL_LABEL,""}, {(xcil_callback)select_hdr, 0}},

  {{"up"  ,__PRE,__SAM,odWidthBar,odWidthBar,-1,10,1},
     {XCIL_BUTTON,"","cc",(char *)sb_up_bits,sb_up_width,sb_up_height},
     {(xcil_callback)sb_shiftup_hdr,0}},
  {{"bar" ,__SAM,__PRE,odWidthBar,odHeightBar,-1,-1,1},{XCIL_LABEL,""},
     {0,0}},
  {{"down",__SAM,__PRE,odWidthBar,odWidthBar,-1,-1,1},
     {XCIL_BUTTON,"","cc",(char *)sb_down_bits,sb_down_width,sb_down_height},
     {(xcil_callback)sb_shiftdown_hdr,0}},

#define MACINTOSH_INDEX 5
  {{"cd..",__PRE,__TOP,OD_W_BUTTON,0,OD_X_BUTTON,27,-1},
     {XCIL_LABEL,"macintosh HD"},
     {(xcil_callback)down_dir_hdr, 0}},
  {{"skip",__SAM,__PRE,OD_W_BUTTON,1,OD_X_BUTTON,OD_Y_BUTTON,2},
     {XCIL_LABEL,"","cc"}, {0, 0}},
#define DESKTOP_INDEX 7
  {{"desk",__SAM,__PRE,OD_W_BUTTON,OD_H_BUTTON,OD_X_BUTTON,OD_Y_BUTTON,2},
     {XCIL_BUTTON,"Desktop","cc"}, {(xcil_callback)desktop_hdr, 0}},
#define HOME_INDEX 8
  {{"home",__SAM,__PRE,OD_W_BUTTON,OD_H_BUTTON,OD_X_BUTTON,OD_Y_BUTTON,2},
     {XCIL_BUTTON,"Home","cc"}, {(xcil_callback)home_hdr, 0}},
#define OPEN_INDEX 9
  {{"open",__SAM,__PRE,OD_W_BUTTON,OD_H_BUTTON,OD_X_BUTTON,OD_Y_BUTTON,2},
     {XCIL_BUTTON,"Open","cc"}, {(xcil_callback)open_file_hdr, 0}},
#define CANCEL_INDEX 10
  {{"canc",__SAM,__PRE,OD_W_BUTTON,OD_H_BUTTON,OD_X_BUTTON,OD_Y_BUTTON,2},
     {XCIL_BUTTON,"Cancel","cc"}, {(xcil_callback)cancel_hdr, 0}},
#define TITLE_INDEX 11
  {{"titl",__TOP,"list",odWidthTitle,0,20,10,-1},
     {XCIL_LABEL,"Choose a file:","l"},{0,0}},

  {{"name",__TOP,__PRE,odWidthTitle,20,20,5,1},
     {XCIL_LABEL,"","l"},{0,0}},

  {{"ltyp",__TOP,__PRE,0,20,20,10,-1},
     {XCIL_LABEL,"File Type:","l"},{0,0}},
  {{"type",__PRE,__SAM,150,20,0,10,1},
     {XCIL_3D_LABEL,"PICT","c"},{0,0}}
};



/*=========================================================*
 * XcilCreateFileDialog : ファイル・ダイアログを作成する。
 *=========================================================*/
Window XcilCreateFileDialog
  _P6 (( Display *                 , display   ),
       ( Window	                   , parent    ),
       ( XContext                  , context   ),
       ( long                      , x         ),
       ( long                      , y         ),
       ( XcilFileDialogAttributes *, attribute ))
{
  int i;
  Window file_dialog;
  Window text;
  Window scrollbar;
  long file_dialog_num;
  XcilPanelItem *file_dialog_list;
  char *dialog_name;

  attribute->all_file_list = 0;
  attribute->select_file = -1;

  if ( attribute->mode & FDMSave )
    {
      attribute->mode |= FDMText;
      dialog_name = "Save Dialog";
    }
  else
    {
      dialog_name = "Open Dialog";
    }

  if ( attribute->current_dir[0] )
    {
      int read_ok = access( attribute->current_dir, F_OK | R_OK );
      if ( read_ok == 0 ) chdir( attribute->current_dir );
    }

  get_path_list( attribute );
  file_dialog_num = XcilNumber( file_dialog_list_static );
  file_dialog_list = typenew1( file_dialog_num, XcilPanelItem );
  for ( i = 0; i < file_dialog_num; i++ )
    file_dialog_list[i] = file_dialog_list_static[i];

  if ( attribute->filetype_num == 0 )
    {
      file_dialog_num -= 2;
      if (( attribute->mode & FDMText ) == 0 ) file_dialog_num -= 1;
    }
  else
    {
      attribute->mode |= FDMText;
    }
  for ( i = 0; i < file_dialog_num; i++ )
    file_dialog_list[i].hdr.client_value = (caddr_t)attribute;
  if ( attribute->path_num == 0 )
    file_dialog_list[0].act.string = "<root>";
  else
    file_dialog_list[0].act.string = attribute->path_list[attribute->path_num-1];
  file_dialog_list[TITLE_INDEX].act.string = attribute->title;
  if ( attribute->mode & FDMSave )
    file_dialog_list[OPEN_INDEX].act.string = "Save";

  if ( attribute->macintosh_label )
    file_dialog_list[MACINTOSH_INDEX].act.string = attribute->macintosh_label;
  if ( attribute->desktop_label )
    file_dialog_list[DESKTOP_INDEX].act.string = attribute->desktop_label;
  if ( attribute->home_label )
    file_dialog_list[HOME_INDEX].act.string = attribute->home_label;
  if ( attribute->open_or_save_label )
    file_dialog_list[OPEN_INDEX].act.string = attribute->open_or_save_label;
  if ( attribute->cancel_label )
    file_dialog_list[CANCEL_INDEX].act.string = attribute->cancel_label;

  file_dialog = XcilCreatePanelWindow( display, 0, context, x, y,
				      file_dialog_list, file_dialog_num,
				      dialog_name);

  attribute->window = file_dialog;
  text = XcilPanelItemWindow( display, file_dialog, context, "list" );
  XcilTextSet( display, text, context );
  XDefineCursor( display, text, Xcil_pointer_cursor );
  XcilAddCallback( display, text, context, Expose, expose_list_hdr, attribute );
  XcilAddCallback( display, file_dialog, context, MapNotify,
		   map_notify_hdr, attribute );

  XcilAddKeyCallback( display, file_dialog, context, 0x0d,
		      open_file_hdr, attribute );

  if ( attribute->filetype_num )
    {
      /* ファイルタイプポップアップを設定する */
      set_filetype_popup( display, context, attribute );
    }

  /* テキストウィンドウを設定する */
  if ( attribute->mode & FDMText )
    {
      text = XcilPanelItemWindow( display, file_dialog, context, "name" );
      XcilTextSet( display, text, context );
      XcilTextGetsReady( display, text, context, return_callback, attribute );
      XcilAddCallback( display, text, context, KeyRelease,
		       key_release_hdr, attribute );
      XcilTextSetInput( display, text, context, attribute->filename );
    }

  /* スクロールバーの設定をする */
  scrollbar = XcilPanelItemWindow( display, file_dialog, context, "bar" );
  XcilScrollSet( display, scrollbar, context, sb_shift_hdr, attribute );
  XcilScrollBarSetLength( display, scrollbar, context, 10.0/attribute->file_num );
  XcilScrollBarSetViewLength( display, scrollbar, context, 1.0 );
  set_path_list( display, context, attribute );

  return file_dialog;
}



/*------------------------------------------*
 * compare_str : qsort 用の文字列の比較関数
 *------------------------------------------*/
static int compare_str
  _P2 (( char **, a ),
       ( char **, b ))
{
  return strcmp( *a, *b );
}

/*========================================*
 * get_path_list : パスリストを設定する。
 *========================================*/
static void get_path_list
  _P1 (( XcilFileDialogAttributes *, attribute ))
{
  char **path_list;
  char *static_current_dir;
  long path_num;

  long file_num;
  char **file_list;

  path_list = attribute->path_list;
  static_current_dir = attribute->static_current_dir;

  if ( getcwd( static_current_dir, 510 ) == 0 )
    {
      perror("pwd");
      exit(1);
    }
  strcopy( attribute->current_dir, static_current_dir );
  path_num = strsplit( static_current_dir, path_list, "/" );
  attribute->path_num = path_num;

  file_list = attribute->all_file_list;
  if ( file_list ) fn_freelist( file_list );
  if (attribute->dotfile_ignore)
    {
      fn_set_dotfile_ignore();
      file_list = fn_list( attribute->current_dir, &file_num );
      fn_reset_dotfile_ignore();
    }
  else
    {
      file_list = fn_list( attribute->current_dir, &file_num );
    }

  attribute->all_file_list = file_list;
  attribute->file_list = file_list + 2;
  attribute->file_num = file_num - 2;
  qsort( file_list + 2, file_num - 2, sizeof(char *), compare_str );
}



/*========================================*
 * set_path_list : パスリストを設定する。
 *========================================*/
static void set_path_list
  _P3 (( Display *                 , display   ),
       ( XContext                  , context   ),
       ( XcilFileDialogAttributes *, attribute ))
{
  char **path_list;
  long i, path_num;

  long popup_num;
  XcilPopupMenuItem *popup_list;

  Window window;
  Window popup;
  Window scrollbar;
  double bar_size;

  path_list = attribute->path_list;
  path_num = attribute->path_num;
  popup_list = attribute->popup_list;

  for ( i = 0; i < path_num; i++ )
    {
      popup_list[i].string = path_list[path_num - 1 - i];
      popup_list[i].callback = change_dir_hdr;
      popup_list[i].client_value = (caddr_t)attribute;
    }
  popup_list[i].string = "<root>";
  popup_list[i].callback = change_dir_hdr;
  popup_list[i].client_value = (caddr_t)attribute;
  popup_num = path_num + 1;
  attribute->popup_num = popup_num;

  window = attribute->window;
  popup = XcilPanelItemWindow( display, window, context, "file" );
  XcilPopupSet( display, popup, context, Button1, popup_list, popup_num );

  /*------------------------*
   * スクローロバーの再設定
   *------------------------*/
  scrollbar = XcilPanelItemWindow( display, window, context, "bar" );
  bar_size = 10.0 / attribute->file_num;
  XcilScrollBarSetLength( display, scrollbar, context, bar_size );
  XcilScrollBarSetViewLength( display, scrollbar, context, 1.0 );
  XcilScrollBarSetPosition( display, scrollbar, context, bar_size );
  attribute->select_offset = 0;
  pr_file_list( display, context, attribute );
}
