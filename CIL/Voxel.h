/*
 * filename : Voxel.h
 * author   : Takahiro Sugiyama
 * date     : 1993/01/29
 * redate   : 1993/05/19
 */



#ifndef _Voxel_h
#define _Voxel_h



#include "Image.h"



typedef struct voxelRec {
  char   *name;
  long    type;
  long    xsize;
  long    ysize;
  long    zsize;
  char ***data;
} *voxel;



extern struct VoxelRec {
  voxel (*create)  P ((char *name));
  void  (*destroy) P ((voxel self));
  void  (*print)   P ((voxel self));
  long  (*load)    P ((voxel self,char *filename));
  long  (*save)    P ((voxel self,char *filename,char *comment));

  char *  (*name)  P ((voxel self));
  long    (*type)  P ((voxel self));
  long    (*xsize) P ((voxel self));
  long    (*ysize) P ((voxel self));
  long    (*zsize) P ((voxel self));
  char ***(*data)  P ((voxel self));

  long    (*volume)  P ((voxel self));
  long    (*byte)    P ((voxel self));
  char  **(*data2D)  P ((voxel self,long z));
  char   *(*zraster) P ((voxel self,long z));

  void (*make)   P ((voxel self,long data_type,
		     long xsize,long ysize,long zsize));
  void (*free)   P ((voxel self));
  void (*copy)   P ((voxel self,voxel src));
  void (*clear)  P ((voxel self));
  void (*resize) P ((voxel self,voxel src,
		     long xsize,long ysize,long zsize));
  void (*sub)    P ((voxel self,voxel src,
		     long srcx,long srcy,long srcz,
		     long xsize,long ysize,long zsize));
  /* extend */
  voxel (*createFromFilename) P ((char *name,char *filename));
  voxel (*createMake) P ((char *name,long type,
			  long xsize,long ysize,long zsize));

  void  (*swap) P ((voxel src1,voxel src2));
} Voxel;



extern long DoCompressVoxelSave;/* = true */
extern char *C3DHeader;/* = 0 */



#endif //_Voxel_h
