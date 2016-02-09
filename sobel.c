#include "Image.h"
#include <stdio.h>
void sobel 
	_P2 ((image,output),
		(image,input))
{
		int x,y,i,j,xsize,ysize;
		uchar **i_data;
		float **o_data;
		int filter [3][3] = {{-1,0,1},
							{-2,0,2},
							{-1,0,1}};

		xsize = Image.xsize(input);
		ysize = Image.ysize(input);
		Image.make(output,Float,xsize,ysize);
		i_data = (uchar **)Image.data(input);
		o_data = (float **)Image.data(output);

		for(y = 1;y < ysize-1;y++){
			for(x = 1;x < xsize-1; x++)
			{
					o_data[y][x] = 0;
				for(j = -1;j <= 1; j++){
					for(i = -1;i <=1;i++){
						o_data[y][x] +=
							filter[j+1][i+1] * i_data[y+j][x+i];
					}}}}
					}


int main(argc,argv)
		int argc;
		char *argv[];
{
		image input ,output;
		input = Image.createFromFilename("input",argv[1]);
		output = Image.create("output");
		fprintf(stderr,"sobelin");
		
		sobel(output,input);
		fprintf(stderr,"sobelout");
		Image.save(output,argv[2],"sobelFilter");
		Image.destroy(input);
		Image.destroy(output);
}

