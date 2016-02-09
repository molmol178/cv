/*
 * imagefuncs.h
 */


/* shift direction */
enum {
  SHIFT_UP,
  SHIFT_DOWN,
  SHIFT_LEFT,
  SHIFT_RIGHT
};

/* gravity */
enum {
  GRAVITY_MM,
  GRAVITY_LU,
  GRAVITY_MU,

  GRAVITY_RU,
  GRAVITY_RM,
  GRAVITY_RD,

  GRAVITY_MD,
  GRAVITY_LD,
  GRAVITY_LM
};



void image__rotation_right_90(image dest,image src);
void image__rotation_left_90(image dest,image src);
void image__rotation_any(image dest,image src,long angle);
void image__x_reverse(image dest,image src);
void image__y_reverse(image dest,image src);
void image__xy_reverse(image dest,image src);
void image__shift(image src,image dest,long dir,long step);
void image__region(image dest,image src,long xsize,long ysize,long gravity);
void image__resize(image dest,image src,long xsize,long ysize);
void image__clear(image src);
void image__fill(image dest,char *value);
