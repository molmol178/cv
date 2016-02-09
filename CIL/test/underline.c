/*
 * filename : underline.c
 * author   : Takahiro Sugiyama
 * date     : Saturday, February 19 1994
 * describe : how to draw underline
 */

#include <stdio.h>


#define EMPHASIZE "\0x1b[1m"
#define UNDERLINE "\0x1b[4m"
#define REVERSE   "\0x1b[7m"


void strattr_reverse();
void strattr_normal();

int main( argc, argv )
     int argc;
     char *argv[];
{
  char *buf = "Sugiyama Takahiro";

  char dst[ 1024 ];

  sprintf( dst, "[1m%s[0m", buf );
  puts( dst );

  sprintf( dst, "[4m%s[0m", buf );
  puts( dst );

  sprintf( dst, "[7m%s[0m", buf );
  puts( dst );

  fputs( "[1m", stdout );
  fputs( buf, stdout );
  puts( "[0m" );

  fputs( "[4m", stdout );
  fputs( buf, stdout );
  puts( "[0m" );

  fputs( "[7m", stdout );
  fputs( buf, stdout );
  puts( "[0m" );

  return 0;
}
