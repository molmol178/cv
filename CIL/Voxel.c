/*
 * filename : Voxel.c
 * author   : Takahiro Sugiyama
 * date     : 1993/01/29
 * redate   : 1993/05/20
 */



#include "Voxel.h"
#include "misc/memlib.h"
#include "misc/strlib.h"
#include "misc/filelib.h"


#include <stdio.h>
#include <stdlib.h>



static voxel voxel__create  P ((char *name));
static void  voxel__destroy P ((voxel self));
static void  voxel__print   P ((voxel self));
static long  voxel__load    P ((voxel self,char *filename));
static long  voxel__save    P ((voxel self,char *filename,char *comment));

static char   *voxel__name  P ((voxel self));
static long    voxel__type  P ((voxel self));
static long    voxel__xsize P ((voxel self));
static long    voxel__ysize P ((voxel self));
static long    voxel__zsize P ((voxel self));
static char ***voxel__data  P ((voxel self));

static long   voxel__volume  P ((voxel self));
static long   voxel__byte    P ((voxel self));
static char **voxel__data2D  P ((voxel self,long z));
static char  *voxel__zraster P ((voxel self,long z));

static void voxel__make   P ((voxel self,long data_type,
			      long xsize,long ysize,long zsize));
static void voxel__free   P ((voxel self));
static void voxel__copy   P ((voxel self,voxel src));
static void voxel__clear  P ((voxel self));
static void voxel__resize P ((voxel self,voxel src,
			      long xsize,long ysize,long zsize));
static void voxel__sub    P ((voxel self,voxel src,
			      long srcx,long srcy,long srcz,
			      long xsize,long ysize,long zsize));

static voxel voxel__createFromFilename P ((char *name,char *filename));
static voxel voxel__createMake P ((char *name,long type,
				   long xsize,long ysize,long zsize));

static void  voxel__swap P ((voxel src1,voxel src2));



struct VoxelRec Voxel = {
  voxel__create,
  voxel__destroy,
  voxel__print,
  voxel__load,
  voxel__save,

  voxel__name,
  voxel__type,
  voxel__xsize,
  voxel__ysize,
  voxel__zsize,
  voxel__data,

  voxel__volume,
  voxel__byte,
  voxel__data2D,
  voxel__zraster,

  voxel__make,
  voxel__free,
  voxel__copy,
  voxel__clear,
  voxel__resize,
  voxel__sub,
  voxel__createFromFilename,
  voxel__createMake,
  voxel__swap
};



long DoCompressVoxelSave = 1;
char *C3DHeader = 0;



static voxel voxel__create(name)
     char *name;
{
  voxel self;

  self        = typenew(struct voxelRec);
  self->name  = strnew(name);
  self->type  = 0;
  self->xsize = 0;
  self->ysize = 0;
  self->zsize = 0;
  self->data  = 0;

  return self;
}



static void voxel__destroy(self)
     voxel self;
{
  if (self == 0) return;

  voxel__free(self);
  strfree(self->name);
  typefree(self);
}



static void voxel__print(self)
     voxel self;
{
  //printf("Voxel %s={type=%s,xsize=%d,ysize=%d,zsize=%d,data=0x%08x}\n", self->name,typename(self->type), self->xsize,self->ysize,self->zsize,self->data);
  printf("Voxel %s={type=%d,xsize=%d,ysize=%d,zsize=%d,data=0x%08x}\n", self->name,self->type, self->xsize,self->ysize,self->zsize,self->data);
}



static char *voxel__name(self)
     voxel self;
{
  return self->name;
}



static long voxel__type(self)
     voxel self;
{
  return self->type;
}



static long voxel__xsize(self)
     voxel self;
{
  return self->xsize;
}



static long voxel__ysize(self)
     voxel self;
{
  return self->ysize;
}



static long voxel__zsize(self)
     voxel self;
{
  return self->zsize;
}



static char ***voxel__data(self)
     voxel self;
{
  return self->data;
}



static long voxel__volume(self)
     voxel self;
{
  return self->xsize * self->ysize * self->zsize;
}



static long voxel__byte(self)
     voxel self;
{
  return voxel__volume(self) * typesize(self->type);
}



static char **voxel__data2D(self,z)
     voxel self;
     long z;
{
  return self->data[z];
}



static char *voxel__zraster(self,z)
     voxel self;
     long z;
{
  return self->data[z][0];
}



static void voxel__make(self,type,xsize,ysize,zsize)
     voxel self;
     long type;
     long xsize;
     long ysize;
     long zsize;
{
  if (self->data) memfree3(self->data);
  self->type  = type;
  self->xsize = xsize;
  self->ysize = ysize;
  self->zsize = zsize;
  self->data  = memnew3(xsize,ysize,zsize,typesize(type));
}



static void voxel__free(self)
     voxel self;
{
  memfree3(self->data);
  self->xsize = 0;
  self->ysize = 0;
  self->zsize = 0;
  self->data  = 0;
}



static void voxel__copy(self,src)
     voxel self;
     voxel src;
{
  long z;

  voxel__free(self);
  self->type  = src->type;
  self->xsize = src->xsize;
  self->ysize = src->ysize;
  self->zsize = src->zsize;
  self->data  = memnew3(src->xsize,src->ysize,src->zsize,typesize(src->type));
  if (voxel__volume(src))
    {
      long z,size;
      size = self->xsize * self->ysize * typesize(self->type);
      for (z = 0; z < self->zsize; z++)
	memcpy(self->data[z][0],src->data[z][0],size);
    }
}



static void voxel__clear(self)
     voxel self;
{
  long i,size;
  size = self->xsize * self->ysize * typesize(self->type);
  for (i = 0; i < self->zsize; i++)
    memset(self->data[i][0],0,size);
}



static int compress;
static char compress_filename[1024];

static FILE *C3D_open(orig)
     char *orig;
{
  char filename[1024];
  char head[1024];
  char com[1024];
  char c;
  FILE *fp;
  long pid;

  char *tmp_dir = "/tmp";
  char *uncompress = "uncompress";
  char *suffix;

  compress = 0;
  pid = getpid();

  if (strcmp(filename,"-") == 0 || strcmp(filename,"stdin") == 0)
    return stdin;

  tmp_dir = getenv( "CIL_TMP" );
  if ( tmp_dir == 0 ) tmp_dir = "/tmp";

  uncompress = getenv( "CIL_UNCOMPRESS" );
  if ( uncompress == 0 ) uncompress = "uncompress";

  if ( strncmp( uncompress, "uncompress", strlen( "uncompress" ) ) == 0 )
    suffix = "Z";
  else
  if ( strncmp( uncompress, "gzip", strlen( "gzip" ) ) == 0 )
    suffix = "gz";
  else
    suffix = "Z";

  sprintf(filename,"%s",orig);
  fp = fopen(filename,"r");
  if (fp != 0)
    {
      c = getc(fp);
      ungetc(c,fp);
      if (c == 'C') return fp;
    }
  if ((fp) && (c == 0x1f))
    {
      fclose(fp);
      sprintf(com,"/bin/cp %s %s/cil%d.%s",filename,tmp_dir,pid,suffix);
      system(com);
      sprintf(com,"%s %s/cil%d.%s",uncompress,tmp_dir,pid,suffix);
      system(com);
      sprintf(compress_filename,"%s/cil%d",tmp_dir,pid);
      fp = fopen(compress_filename,"r");
      compress = 1;
      return fp;
    }

  suffix = "Z";
  uncompress = "uncompress";
  sprintf(filename,"%s.%s",orig,suffix);
  fp = fopen(filename,"r");
  if (fp != 0)
    {
      c = getc(fp);
      ungetc(c,fp);
      if (c == 'C') return fp;
    }
  if ((fp) && (c == 0x1f))
    {
      fclose(fp);
      sprintf(com,"/bin/cp %s %s/cil%d.%s",filename,tmp_dir,pid,suffix);
      system(com);
      sprintf(com,"%s %s/cil%d.%s",uncompress,tmp_dir,pid,suffix);
      system(com);
      sprintf(compress_filename,"%s/cil%d",tmp_dir,pid);
      fp = fopen(compress_filename,"r");
      compress = 1;
      return fp;
    }

  suffix = "gz";
  uncompress = "gzip -d";
  sprintf(filename,"%s.gz",orig);
  fp = fopen(filename,"r");
  if (fp != 0)
    {
      c = getc(fp);
      ungetc(c,fp);
      if (c == 'C') return fp;
    }
  if ((fp) && (c == 0x1f))
    {
      fclose(fp);
      sprintf(com,"/bin/cp %s %s/cil%d.%s",filename,tmp_dir,pid,suffix);
      system(com);
      sprintf(com,"%s %s/cil%d.%s",uncompress,tmp_dir,pid,suffix);
      system(com);
      sprintf(compress_filename,"%s/cil%d",tmp_dir,pid);
      fp = fopen(compress_filename,"r");
      compress = 1;
      return fp;
    }

  return 0;
}



static long voxel__load(self,filename)
     voxel self;
     char *filename;
{
  FILE *fp;
  long err = 0;
  char buf[1024];
  long n;
  char *head[512];
  long size,xsize,ysize,zsize;
  long type;
  long i,x,y,z;
  char comment[512];

  if (self == 0)
    {
      fprintf(stderr,"error:Voxel.load:voxel is wrong.\n");
      return 0;
    }

  fp = C3D_open(filename);
  if ( fp == 0 ) return 0;

  fgets(buf,1024,fp);
  if (C3DHeader) strfree(C3DHeader);
  C3DHeader = strnew(buf);

  n = strsplit(buf,head," \n");
  if (strcmp("C3D",head[0]) != 0)
    {
      fprintf(stderr,"error:Voxel.load(%s,%s):file magic word is wrong.\n",
	      self->name,filename);
      return 0;
    }

  type = typeget(head[1]);
  if (type == 0)
    {
      fprintf(stderr,"warning:Voxel.load(%s,%s):file type is unknown.\n",
	      self->name,filename,head[1]);
      size = atol(head[2]);
      type = typeenter(head[1],size);
    }
  xsize = atol(head[3]);
  ysize = atol(head[4]);
  zsize = atol(head[5]);

  if ( n > 6 )
    {
      strcpy(comment,C3DHeader+(head[6]-head[0]));
      if (comment[strlen(comment)-1] == '\n')
	comment[strlen(comment)-1] = 0;
    }
  else
    {
      comment[0] = '\0';
    }

  fprintf(stderr,"Voxel.load = (\n");
  fprintf(stderr,"    voxelname = %s\n",self->name);
  fprintf(stderr,"    filename  = %s\n",filename);
  fprintf(stderr,"        type  = ");ftypeprintf(stderr,type);fprintf(stderr,"\n");
  fprintf(stderr,"        xsize = %d\n",xsize);
  fprintf(stderr,"        ysize = %d\n",ysize);
  fprintf(stderr,"        zsize = %d\n",zsize);
  fprintf(stderr,"        comment ... \"%s\"\n",comment);
  fprintf(stderr,")\n");

  voxel__make(self,type,xsize,ysize,zsize);

  if (type == Bit1)
    {
      int xbyte;
      int amari;
      uchar *buf2;

      xbyte = (xsize + 7) / 8;
      size = xbyte*ysize;
      amari = xsize % 8;
      buf2   = (uchar *)malloc(size);

      for (z = 0; z < zsize; z++)
	{
	  register char *p;
	  register uchar *bp;

	  fp_readbyte(fp, buf2, size);
	  bp = buf2;
	  p = self->data[z][0];
	  for (y = 0; y < ysize; y++)
	    {
	      for (x = 0; x < xbyte; x++)
		{
		  register int i;
		  register ushort bits = *bp++;
		  if ((x == (xbyte - 1))&&(amari))
		    for (i = 7; i >= (8 - amari); i--)
		      {
			if ((bits >> i) & 1)
			  *p++ = 1;
			else
			  *p++ = 0;
		      }
		  else
		    for (i = 7; i >= 0; i--)
		      {
			if ((bits >> i) & 1)
			  *p++ = 1;
			else
			  *p++ = 0;
		      }
		}
	    }
	}
      free(buf2);
    }
  else
    {
      size = xsize * ysize * typesize(type);
      for (i = 0; i < zsize; i++)
	fp_readbyte(fp, self->data[i][0], size);
    }

  if (fp != stdin) fclose(fp);

  if (compress)
    {
      char com[1024];
      sprintf(com,"/bin/rm -f %s",compress_filename);
      system(com);
    }

  return 1;
}



static long voxel__save(self,original,comment)
     voxel self;
     char *original;
     char *comment;
{
  FILE *fp;
  long err = 0;
  long type;
  long i,x,y,z,size;
  char filename[1024];
  long pid;
  char com[1024];

  char *tmp_dir;
  char *compress_cmd;
  char *suffix;
  char *do_compress;

  if (self == 0)
    {
      fprintf(stderr,"error:Voxel.save(???,%s):voxel is wrong.\n",original);
      return 0;
    }

  tmp_dir = getenv( "CIL_TMP" );
  if ( tmp_dir == 0 ) tmp_dir = "/tmp";

  compress_cmd = getenv( "CIL_COMPRESS" );
  if ( compress_cmd == 0 ) compress_cmd = "compress";

  if ( strncmp( compress_cmd, "compress", strlen( "compress" ) ) == 0 )
    suffix = "Z";
  else
  if ( strncmp( compress_cmd, "gzip", strlen( "gzip" ) ) == 0 )
    suffix = "gz";
  else
    suffix = "Z";

  do_compress = getenv( "CIL_NOT_COMPRESS" );
  if ( do_compress ) DoCompressVoxelSave = 0;

  type = self->type;

  pid = getpid();
  if (DoCompressVoxelSave)
    {
      sprintf(filename,"%s/cil%d",tmp_dir,pid);
    }
  else
    {
      strcpy(filename,original);
    }

  if (strcmp(filename,"-") == 0 || strcmp(filename,"stdout") == 0)
    fp = stdout;
  else
    fp = fopen(filename,"w");

  if (fp == 0)
    {
      fprintf(stderr,"error:Voxel.save(%s,%s):file can't open.\n",
	      self->name,filename);
      return 0;
    }

  if (comment == 0 || *comment == 0) {
    //fprintf(fp,"C3D %s %d %d %d %d\n",typename(type),typesize(type), self->xsize,self->ysize,self->zsize);
    fprintf(fp,"C3D ");
    ftypeprintf(fp,type);
    fprintf(fp," %d %d %d %d\n",typesize(type), self->xsize,self->ysize,self->zsize);
  } else{ 
    //fprintf(fp,"C3D %s %d %d %d %d %s\n",typename(type),typesize(type), self->xsize,self->ysize,self->zsize,comment);
    fprintf(fp,"C3D ");
    ftypeprintf(fp,type);
    fprintf(fp," %d %d %d %d %s\n",typesize(type), self->xsize,self->ysize,self->zsize,comment);
  }

  if (type == Bit1)
    {
      int xbyte = (self->xsize + 7) / 8;
      uchar *buf;

      size = xbyte*self->ysize;
      buf = (uchar *)malloc(size);

      for (z = 0; z < self->zsize; z++)
	{
	  register uchar *bp = buf;
	  register char *p = self->data[z][0];
	  for (y = 0; y < self->ysize; y++)
	    for (x = 0; x < xbyte; x++)
	      {
		unsigned char bits = 0;
		if ((x == (xbyte-1))&&(self->xsize % 8))
		  {
		    for (i = 7; i >= (8 - (self->xsize % 8)); i--)
		      if (*p++) bits |= (1 << i);
		  }
		else
		  {
		    for (i = 7; i >= 0; i--)
		      if (*p++) bits |= (1 << i);
		  }
		*bp++ = bits;
	      }
	  fwrite(buf,1,size,fp);
	}
      free(buf);
    }
  else
    {
      size = self->xsize * self->ysize * typesize(type);
      for (i = 0; i < self->zsize; i++)
	{
	  char **data = (char **)self->data[i];
	  fwrite(data[0],1,size,fp);
	}
    }

  if (fp != stdout) fclose(fp);

  if (DoCompressVoxelSave)
    {
      sprintf(filename,"%s/cil%d",tmp_dir,pid);
      sprintf(com,"%s %s",compress_cmd,filename);
      system(com);
      sprintf(com,"mv %s.%s %s.%s",filename,suffix,original,suffix);
      system(com);
    }

  return 1;
}



static void voxel__resize(self,src,xsize,ysize,zsize)
     voxel self;
     voxel src;
     long xsize;
     long ysize;
     long zsize;
{
  long sx,sy,sz;
  long px,py,pz;
  long x,y,z;
  voxel temp;
  long size;

  if (src == self)
    temp = voxel__create(self->name);
  else
    temp = self;
  voxel__make(temp,src->type,xsize,ysize,zsize);
  voxel__clear(temp);

  size = typesize(self->type);
  for (sz = 0,z = 0; z < zsize; z++)
    {
      char **dpp,**spp;

      pz = sz / zsize;
      dpp = (char **)temp->data[z];
      spp = (char **)src->data[pz];

      for (sy = 0,y = 0; y < ysize; y++)
	{
	  char *dp,*sp;

	  py = sy / ysize;
	  dp = dpp[y];
	  sp = spp[py];

	  for (sx = 0,x = 0; x < xsize; x++)
	    {
	      char *d,*s;

	      px = sx / xsize;
	      d = dp + size * x;
	      s = sp + size * px;
	      memcpy(d,s,size);
	      sx += src->xsize;
	    }
	  sy += src->ysize;
	}
      sz += src->zsize;
    }

  if (src == self)
    {
      voxel__copy(self,temp);
      voxel__destroy(temp);
    }
}



static void voxel__sub(self,src,srcx,srcy,srcz,xsize,ysize,zsize)
     voxel self;
     voxel src;
     long srcx;
     long srcy;
     long srcz;
     long xsize;
     long ysize;
     long zsize;
{
  long x,y,z;
  long px,py,pz;
  voxel temp;
  long size;

  if (src == self)
    temp = voxel__create(self->name);
  else
    temp = self;
  voxel__make(temp,src->type,xsize,ysize,zsize);
  voxel__clear(temp);

  size = typesize(src->type);
  for (z = 0; z < zsize; z++)
    {
      pz = z + srcz;
      if (pz < 0 || pz >= src->zsize) continue;
      for (y = 0; y < ysize; y++)
	{
	  py = y + srcy;
	  if (py < 0 || py >= src->ysize) continue;
	  for (x = 0; x < xsize; x++)
	    {
	      px = x + srcx;
	      if (px < 0 || px >= src->xsize) continue;
	      COPY_3(temp->data,x,y,z,src->data,px,py,pz,size);
	    }
	}
    }

  if (src == self)
    {
      voxel__copy(self,temp);
      voxel__destroy(temp);
    }
}



static voxel voxel__createFromFilename(name,filename)
     char *name;
     char *filename;
{
  voxel self;

  self = voxel__create(name);
  if (voxel__load(self,filename) == 0)
    {
      fprintf(stderr,"error = {\n");
      fprintf(stderr,"    Voxel.createFromFilename(%s,%s)\n",name,filename);
      fprintf(stderr,"    can't open file(%s)\n",filename);
      fprintf(stderr,"}\n");
      voxel__destroy(self);
      return 0;
    }
  return self;
}



static voxel voxel__createMake(name,type,xsize,ysize,zsize)
     char *name;
     long type;
     long xsize;
     long ysize;
     long zsize;
{
  voxel self;

  self = voxel__create(name);
  voxel__make(self,type,xsize,ysize,zsize);

  return self;
}



static void voxel__swap(src1,src2)
     voxel src1;
     voxel src2;
{
  long mtype;
  long xsize,ysize,zsize;
  char ***data;

  mtype = src2->type;
  xsize = src2->xsize;
  ysize = src2->ysize;
  zsize = src2->zsize;
  data  = src2->data;

  src2->type  = src1->type;
  src2->xsize = src1->xsize;
  src2->ysize = src1->ysize;
  src2->zsize = src1->zsize;
  src2->data  = src1->data;

  src1->type  = mtype;
  src1->xsize = xsize;
  src1->ysize = ysize;
  src1->zsize = zsize;
  src1->data  = data;
}
