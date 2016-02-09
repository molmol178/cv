#include "Image.h"



void main(argc,argv)
     int argc;
     char *argv[];
{
  image src,ss,ks;

  if (argc < 2)
    {
      fprintf(stderr,"share image test : put filename\n");
      exit(0);
    }

  src = Image.createFromFilename(argv[1],argv[1]);

  ss = Image.createShare("AAAB",Image.type(src),
			 Image.xsize(src),Image.ysize(src));

  Image.save(ss,"aaa",0);

  Image.copy(ss,src);
  Image.save(ss,"aab",0);

  ks = Image.createFromKey("AAAB");
  Image.save(ks,"aac",0);

  Image.destroyShare(ss);
}
