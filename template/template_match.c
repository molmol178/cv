#include <Image.h>
#include <stdio.h>
#include <math.h>
 
float calc_similarity(image input,int x,int y, image template);
void draw_rectangle(image similarity ,int x, int y, image template);

void image_calc_similarity(image input,image template,image similarity)
{

	int x,y;
	int xsize,ysize;
	int xsizet,ysizet;
	float sim;
	
	xsize = Image.xsize(input);
	ysize = Image.ysize(input);
	
	xsizet = Image.xsize(template);
	ysizet = Image.ysize(template);

	Image.make(similarity,Float,xsize,ysize);
	
	for (y = 0; y < ysize - ysizet + 1; y++){
			for (x = 0; x < xsize - xsizet + 1; x++){
				   sim = calc_similarity(input, x, y, template);
				   __PIXEL(similarity, x, y, float) = sim;
		   }
	}
}

 
float calc_similarity(image input,int x,int y, image template){
	int tx,ty;
	double simi,simt;
	long sims;
	int xsizet,ysizet;
	float normalize_sim;	
		
	xsizet = Image.xsize(template);
	ysizet = Image.ysize(template);
	
	simi = 0;
	simt = 0;
	sims = 0;
	normalize_sim = 0;

	for(ty = 0; ty < ysizet; ty++){
			for(tx = 0; tx < xsizet; tx++){
				sims += __PIXEL(template , tx ,ty ,uchar) * __PIXEL(input ,x +tx ,y +ty ,uchar);
				simi += __PIXEL(input ,x + tx ,y + ty ,uchar) * __PIXEL(input ,x + tx , y + ty, uchar);
				simt += __PIXEL(template ,tx ,ty ,uchar) * __PIXEL(template ,tx ,ty ,uchar);
			}
	}
	normalize_sim = fabs( sims / ( sqrt (simi) * sqrt (simt) ));
	return normalize_sim;
}



void detect_peak(image peak ,image similarity , image template){

	int x,y;
	int xsize,ysize;
	int xsizet,ysizet;
	float sim;
	int tx,ty;
	
	xsize = Image.xsize(similarity);
	ysize = Image.ysize(similarity);
	float P0, P1, P2, P3, P4;
	int cx,cy;
	int rectangle;

	Image.make(peak , Bit1 ,xsize ,ysize);

	for (y = 1; y < ysize - 1; y++){
			for (x = 1; x < xsize - 1; x++){
					P0 = __PIXEL(similarity, x, y, float);
					P1 = __PIXEL(similarity, x+1, y, float);
					P2 = __PIXEL(similarity, x, y-1, float);
					P3 = __PIXEL(similarity, x-1, y, float);
					P4 = __PIXEL(similarity, x, y+1, float);
	
				if ( P0 > P1 && P0 > P1 && P0 >P2 && P0 > P3 && P0 >P4){
					__PIXEL(peak ,x ,y , bit1) = 1;
							draw_rectangle(peak,x,y,template);
		
				}
			}
	}
}
void draw_rectangle(image paper, int x, int y, image template){
	int xsize,ysize;
	int xsizet,ysizet;
	int sim;
	int tx,ty;
	
	xsize = Image.xsize(paper);
	ysize = Image.ysize(paper);
	xsizet = Image.xsize(template);
	ysizet = Image.ysize(template);
	
	for (ty = 0; ty < ysizet + 1; ty++){
			for (tx = 0; tx < xsizet + 1; tx++){
				  if (ty == 0 || ty == ysizet || tx == 0 || tx == xsizet){
					 	__PIXEL(paper, x + tx, y + ty ,bit1) = 1;
				  }
		 	}
	}
}

int main (argc,argv)
		int argc;
		char *argv[];
{
	image input,template,similarity,peak;
	input = Image.createFromFilename("input",argv[1]);
	template = Image.createFromFilename("template",argv[2]);	
	similarity = Image.create("similarity");		
	peak = Image.create("peak");

	image_calc_similarity(input, template, similarity);	
	detect_peak(peak,similarity,template);

	Image.save(similarity,"outsimi.c2d","outsimilarity");
	Image.save(peak , "outpeak.c2d" , "outpeak");

	Image.destroy(input);
	Image.destroy(template);
	Image.destroy(similarity);
	Image.destroy(peak);
}
