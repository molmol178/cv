/*
 * FImage.c
 */



#include "FImage.h"
#include "../misc/strlib.h"
#include "../misc/typelib.h"
#include "../misc/filelib.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * for access
 */
#include <unistd.h>



static struct format {
  char *name;
  long (*load)(/* image self, char *filename */);
  long (*save)(/* image self, char *filename, char *comment */);
  long (*magic)(/* FILE *fp */);
} FImageList[] = {
  {FORMAT_C2D, FImageLoad_C2D, FImageSave_C2D, FImageMagic_C2D},
  {FORMAT_PNM, FImageLoad_PNM, FImageSave_PNM, FImageMagic_PNM},
  {FORMAT_J4, FImageLoad_J4, FImageSave_J4, FImageMagic_J4},
  {FORMAT_TIFF, FImageLoad_TIFF, FImageSave_TIFF, FImageMagic_TIFF},
  {FORMAT_VIFF, FImageLoad_VIFF, FImageSave_VIFF, FImageMagic_VIFF},
  {FORMAT_GIF, FImageLoad_GIF, FImageSave_GIF, FImageMagic_GIF},
  {FORMAT_XBM, FImageLoad_XBM, FImageSave_XBM, FImageMagic_XBM},
  {FORMAT_S3D, FImageLoad_S3D, FImageSave_S3D, FImageMagic_S3D},
  {FORMAT_XWD, FImageLoad_XWD, FImageSave_XWD, FImageMagic_XWD},
  /*  {FORMAT_DIB, FImageLoad_DIB, FImageSave_DIB, FImageMagic_DIB},*/
  {FORMAT_BMP, FImageLoad_BMP, FImageSave_BMP, FImageMagic_BMP},
  {FORMAT_XwBitmap, FImageLoad_XwBitmap, FImageSave_XwBitmap, FImageMagic_XwBitmap},
  {FORMAT_PS, FImageLoad_PS, FImageSave_PS, FImageMagic_PS},
  {FORMAT_JPEG, FImageLoad_JPEG, FImageSave_JPEG, FImageMagic_JPEG}
};
static long FImageNum = Number(FImageList);

static char *FImageSaveFormat = FORMAT_C2D;
static long  FImageQuality = 100;
static char *FImageFormat = 0;
static char *FImageComment = 0;
static char *FImageHeader = 0;
static long  FImageBit1Type = Bit1;
static char  FImageLoadDirectory[ 512 ];



void FImageSetSaveFormat(format)
     char *format;
{
  FImageSaveFormat = format;
}



void FImageSetQuality(quality)
     long quality;
{
  FImageQuality = quality;
}



void FImageSetFormat(format)
     char *format;
{
  FImageFormat = format;
}



void FImageSetComment(comment)
     char *comment;
{
  strfree(FImageComment);
  FImageComment = strnew(comment);
}



void FImageSetHeader(header)
     char *header;
{
  strfree(FImageHeader);
  FImageHeader = strnew(header);
}



char *FImageGetSaveFormat()
{
  return FImageSaveFormat;
}



long FImageGetQuality()
{
  return FImageQuality;
}



char *FImageGetFormat()
{
  return FImageFormat;
}



char *FImageGetComment()
{
  return FImageComment;
}



char *FImageGetHeader()
{
  return FImageHeader;
}



long FImageGetBit1Type()
{
  return FImageBit1Type;
}



char *FImageGetLoadDirectory()
{
  return FImageLoadDirectory;
}



static struct format *FImage__search(name_org)
     char *name_org;
{
  int i;
  char name[ 64 ];

  if ( name_org )
    {
      strcpy( name, name_org );
      if ( strxequal( "pbm" , name_org ) ) strcpy( name, "pnm" );
      if ( strxequal( "pgm" , name_org ) ) strcpy( name, "pnm" );
      if ( strxequal( "ppm" , name_org ) ) strcpy( name, "pnm" );
      if ( strxequal( "eps" , name_org ) ) strcpy( name, "ps"  );
      if ( strxequal( "epsf", name_org ) ) strcpy( name, "ps"  );
    }
  else
    {
      strcpy( name, "c2d" );
    }

  for (i = 0; i < FImageNum; i++)
    if (strxequal(name,FImageList[i].name))
      {
	return &FImageList[i];
      }
  fprintf(stderr,"not find format %s\n",name_org);

  return &FImageList[0];
}



static char *ext_list[] = {
  "","",".j4","j4",".r","r",
  ".pbm",".pgm",".ppm","pbm","pgm","ppm",
  ".c2d","c2d",
  ".tiff",".tif",".viff",".vif",".gif","gif",
  ".Z","Z",".gz","gz",
  ".c2d.Z","c2d.Z",".c2d.gz","c2d.gz",
  ".j4.Z","j4.Z",".j4.gz","j4.gz",
  ".pbm.Z","pbm.Z",".pbm.gz","pbm.gz",
  ".pgm.Z","pgm.Z",".pgm.gz","pgm.gz",
  ".ppm.Z","ppm.Z",".ppm.gz","ppm.gz",
  ".r.Z","r.Z",".r.gz","r.gz",
  ".m.Z","m.Z",".m.gz","m.gz",
  ".m","bmp"
};

static FILE *FImage__magic(filename,load_filename)
     char *filename;
     char *load_filename;
{
  int i;
  FILE *fp = 0;
  char newfilename[256];

  load_filename[0] = '\0';

  if (strequal(filename,"-") || strequal(filename,"stdin"))
    return stdin;

  /*
   * ファイル名に拡張子をつけたものをオープンする。
   */
  for (i = 0; i < Number(ext_list); i++)
    {
      sprintf(newfilename,"%s%s",filename,ext_list[i]);

      if (access(newfilename,R_OK) == 0)
	{
	  if (fp = fp_open(newfilename,"r"))
	    {
	      if ((!strequal(ext_list[i],"r")) && (!strequal(ext_list[i],".r")))
		strcpy(load_filename,newfilename);
	      return fp;
	    }
	}
    }

  /*
   * ディレクトリを省略した時にディレクトリを付ける。
   */
  {
    long  j, dir_num;
    char *dir_buf;
    char *dir_env;
    char *dir_list[256];

    dir_env = getenv( CIL_IMAGE_DIR );
    if ( dir_env == 0 )
      {
	dir_env = getenv( IMAGE_DIR );
	if ( dir_env == 0 ) return 0;
      }

    dir_buf = strnew( dir_env );
    dir_num = strsplit( dir_buf, dir_list, " ;:" );

    for ( j = 0; j < dir_num; j++ )
      for ( i = 0; i < Number( ext_list ); i++ )
	{
	  sprintf( newfilename, "%s/%s%s", dir_list[ j ], filename, ext_list[ i ] );
	  if ( access( newfilename, R_OK ) == 0 )
	    {
	      if ( fp = fp_open( newfilename, "r" ) )
		{
		  if ( strequal( ext_list[ i ], "r" ) || strequal( ext_list[ i ], ".r" ))
		    sprintf( load_filename, "%s/%s", dir_list[ j ], filename );
		  else
		    sprintf( load_filename, "%s", newfilename );
		  strcpy( FImageLoadDirectory, dir_list[ j ] );
		  if ( getenv( "CIL_VERBOSE" ) )
		    fprintf( stderr, "CIL:LOADING FROM \"%s\"\n", load_filename );
		  return fp;
		}
	    }
	}
    strfree( dir_buf );
  }

  return 0;
}



static long exec_uncompress(filename,ext,ext2)
  char *filename;
  char *ext;
  char *ext2;
{
  long pid = getpid();
  char command[512];
  char compressed_filename[256];
  char *uncompress;

  uncompress = getenv( "CIL_UNCOMPRESS" );
  if ( uncompress == 0 ) uncompress = "uncompress";
  if (access(filename,R_OK) == 0)
    {
      if ( filename[strlen(filename)-1] != 'Z' ) uncompress = "gunzip";
      sprintf(command,"cp %s /tmp/cil%d.Z",filename,pid);
      system(command);
      sprintf(command,"%s /tmp/cil%d.Z",uncompress,pid);
      system(command);
    }
  else
    {
      int i;
      static char *comp_ext[] = { ".Z", "Z", ".gz", "gz" };
      for ( i = 0; i < Number( comp_ext ); i++ )
	{
	  sprintf(compressed_filename,"%s%s%s",filename,ext,comp_ext[i]);
	  if (access(compressed_filename,R_OK) == 0) break;
	}
      if ( i == Number( comp_ext ) ) return -1;

      if ( i >= 2 ) uncompress = "gunzip";

      sprintf(command,"cp %s /tmp/cil%d%s.Z",compressed_filename,pid,ext2);
      system(command);
      sprintf(command,"%s /tmp/cil%d%s.Z",uncompress,pid,ext2);
      system(command);
    }

  return 0;
}



static long do_uncompress(filename,newfilename,ext)
     char *filename;
     char *newfilename;
     char *ext;
{
  long pid = getpid();

  if (ext == 0) ext = "";

  if (strequal(ext,".r"))
    {
      sprintf(newfilename,"/tmp/cil%d.r",pid);
      if (exec_uncompress(filename,".r",".r") != 0) return -1;
      if (exec_uncompress(filename,".g",".g") != 0) return -1;
      if (exec_uncompress(filename,".b",".b") != 0) return -1;
    }
  else
  if (strequal(ext,"r"))
    {
      sprintf(newfilename,"/tmp/cil%d.r",pid);
      if (exec_uncompress(filename,"r",".r") != 0) return -1;
      if (exec_uncompress(filename,"g",".g") != 0) return -1;
      if (exec_uncompress(filename,"b",".b") != 0) return -1;
    }
  else
    {
      sprintf(newfilename,"/tmp/cil%d",pid);
      if (exec_uncompress(filename,ext,"") != 0) return -1;
    }

  return 0;
}



static FILE *FImage__compressMagic(org_filename,newfilename)
     char *org_filename;
     char *newfilename;
{
  int i;
  FILE *fp = 0;
  char magic_filename[256];
  char filename[256];

  if ( newfilename[0] ) strcpy( filename, newfilename );
  else			strcpy( filename, org_filename );

  sprintf(newfilename,"/tmp/cil%d",getpid());

  for (i = 0; i < Number(ext_list); i++)
    {
      if (do_uncompress(filename,magic_filename,ext_list[i]) == 0)
	{
	  fp = fp_open(magic_filename,"r");
	  if (fp) return fp;
	}
    }

  return 0;
}



long FImageLoad(self, filename)
     image self;
     char *filename;
{
  char com[512];
  char newfilename[512];
  FILE *fp;
  long compress = 0;
  long err = 0;
  long compc;
  long i;

  //  fprintf(stderr,"1.");

  newfilename[0] = '\0';

  //  fprintf(stderr,"2.");

  fp = FImage__magic(filename,newfilename);

  //  fprintf(stderr,"3.");

  if (fp) {compc = getc(fp);ungetc(compc,fp);}

  //  fprintf(stderr,"4.");

  if (fp == 0 || compc == 0x1f)
    {
      if (fp) fp_close(fp);
      compress = 1;
      fp = FImage__compressMagic(filename,newfilename);
      if (fp == 0) return 0;
/*      fprintf(stderr,"This file %s is compressed.\n",filename);*/
    }

  //  fprintf(stderr,"5.");

  if (newfilename[0]) filename = newfilename;

  //  fprintf(stderr,"6.");

  for (i = 0; i < FImageNum; i++)
    {
      if (FImageList[i].magic(fp)) break;
      if (feof(fp)) {i = -1;break;}
    }
  fp_close(fp);
 
  //  fprintf(stderr,"7(%d).",i);

  if (i >= 0)
    {
      err = FImageList[i].load(self,filename);
      FImageFormat = FImageList[i].name;
    }

  //  fprintf(stderr,"8.");

  if (compress)
    {
      sprintf(com,"/bin/rm -f %s %s.r %s.g %s.b",
	      filename,filename,filename,filename);
      system(com);
    }

  //  fprintf(stderr,"9.");

  return err;
}



long FImageSave(self, filename, comment)
     image self;
     char *filename;
     char *comment;
{
  struct format *fmt;

  fmt = FImage__search(FImageSaveFormat);
  return fmt->save(self,filename,comment);
}
