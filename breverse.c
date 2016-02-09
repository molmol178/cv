#include "Image.h"

void binary__reverse
	_P2 ((image,output),
		(image,input))
{
		int i , n;
		bit1 *i_ptr,*o_ptr;

		Image.make(output,Bit1,__XSIZE(input),__YSIZE(input));
		n = __AREA(input);
		i_ptr = __RASTER(input,bit1);
		o_ptr = __RASTER(output,bit1);
		for( i = 0; i<n; i++, i_ptr++, o_ptr++)
				*o_ptr = (*i_ptr) ? 0 : 1;
}

void main(argc,argv)

		int argc;
		char *argv[];
	{
			image input ,output;

			input = Image.createFromFilename("input",argv[1]);
			output = Image.create("output");
			binary__reverse(output,input);
			Image.save(output,argv[2],"Reverse Image");
			Image.destroy(input);
			Image.destroy(output);
	}
			

