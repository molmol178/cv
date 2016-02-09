#include "Image.h"
#include <stdio.h>

void main ( argc , argv )
		int argc;
		char *argv[];
{
		image foo = NULL;
		Image.print(foo);
//		printf(foo);

		foo = Image.create("foo");
//		Image.print(foo);
		printf("%s",foo->name);
		printf("%d",foo->type);
		printf("%d",foo->xsize);
		printf("%d",foo->ysize);

		Image.make(foo,UChar,256,256);
		Image.print(foo);
		//printf("%s",foo);

		Image.free(foo);
	Image.print(foo);
//	printf("%s",foo);

		Image.destroy(foo);
	Image.print(foo);
//		printf("%s",foo);
}
