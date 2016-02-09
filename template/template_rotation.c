#include <Image.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double calc_similarity(image input,int x,int y, image template);
void draw_rectangle(image peak ,int x, int y, image template);

void image_calc_similarity(image input,image template,image similarity)
{

	int x,y,tx,ty;
	int xsize,ysize;
	int xsizet,ysizet;
	double sim;
	int angle;

	xsize = Image.xsize(input);
	ysize = Image.ysize(input);
	xsizet = Image.xsize(template);
	ysizet = Image.ysize(template);

	for (y = 0; y < ysize - ysizet + 1; y++){
		for (x = 0; x < xsize - xsizet + 1; x++){
			//	printf("imagecalc %d\n",i);
			sim = calc_similarity(input, x, y, template);
	   		__PIXEL(similarity, x, y, float) += sim;
		}
	}
}

 
double calc_similarity(image input,int x,int y, image template){
	int tx,ty,angle;
	double simit,simii,simtt ,simi,simt;
	int xsizet,ysizet;
	double normalize_sim;	

	simi = 0;
	simt = 0;
	simit = 0;
	simii = 0;
	simtt = 0;
	normalize_sim = 0;

	xsizet = Image.xsize(template);
	ysizet = Image.ysize(template);

	for(ty = 0; ty < ysizet; ty++){
		for(tx = 0; tx < xsizet; tx++){
		//	printf("calcsimilarity %d\n",ty);
			
			if (__PIXEL(template, tx ,ty ,uchar) < 255){
				simit += __PIXEL(template , tx ,ty ,uchar) * __PIXEL(input ,x +tx ,y +ty ,uchar);
				simii += __PIXEL(input ,x + tx ,y + ty ,uchar) * __PIXEL(input ,x + tx , y + ty, uchar);
				simtt += __PIXEL(template ,tx ,ty ,uchar) * __PIXEL(template ,tx ,ty ,uchar);
				simi  += __PIXEL(input , x + tx ,y + ty ,uchar);
				simt  += __PIXEL(template ,tx ,ty ,uchar);		
				//printf("simit =%f ,simii = %f ,simtt = %f ,simi = %f ,simt = %f\n",simit,simii,simtt,simi,simt);
			}
		}
	}
	normalize_sim =  (tx * ty * simit - simi * simt) /  sqrt (((tx * ty * simii) - (simi * simi)) * ((tx * ty * simtt) - (simt * simt)));
	//printf("normalize_sim = %f\n" ,normalize_sim);
	return normalize_sim;	
}
/*

void detect_peak(image peak ,image similarity , image template){

	int x,y;
	int xsize,ysize;
	int xsizet,ysizet;
	double sim;
	int tx,ty;
	int angle;

	xsize = Image.xsize(similarity);
	ysize = Image.ysize(similarity);
	double P0, P1, P2, P3, P4;
	int cx,cy;
	int rectangle;

	for (y = 1; y < ysize - 1; y++){
		for (x = 1; x < xsize - 1; x++){
					P0 = __PIXEL(similarity, x, y, double);
					P1 = __PIXEL(similarity, x+1, y, double);
					P2 = __PIXEL(similarity, x, y-1, double);
					P3 = __PIXEL(similarity, x-1, y, double);
					P4 = __PIXEL(similarity, x, y+1, double);
				if ( P0 > P1 && P0 > P2 && P0 > P3 && P0 > P4 ){
					//printf("detectpeak %d\n",y);	
					__PIXEL(peak ,x ,y , bit1) = 1;
					draw_rectangle(peak,x,y,template);
				}
			
		}
	}
}
*/
void detect_peak_3d(image peak ,image *similarity , image template){

	int x,y;
	int xsize,ysize;
	int xsizet,ysizet;
	float sim;
	int tx,ty;
	int angle;

	xsize = Image.xsize(similarity[1]);
	ysize = Image.ysize(similarity[1]);
	float P0, P1, P2, P3, P4,P5 ,P6;
	int cx,cy;
	int rectangle;

	for(angle = 91; angle < 360 - 90; angle += 90){
		for (y = 1; y < ysize - 1; y++){
			for (x = 1; x < xsize - 1; x++){
					P0 = __PIXEL(similarity[angle], x, y, float);
					P1 = __PIXEL(similarity[angle], x+1, y, float);
					P2 = __PIXEL(similarity[angle], x, y-1, float);
					P3 = __PIXEL(similarity[angle], x-1, y, float);
					P4 = __PIXEL(similarity[angle], x, y+1, float);
					P5 = __PIXEL(similarity[angle+90],x ,y ,float);
					P6 = __PIXEL(similarity[angle-90],x ,y ,float);
				if ( P0 > P1 && P0 > P2 && P0 > P3 && P0 > P4 && P0 > P5 && P0 > P6){
					__PIXEL(peak ,x ,y , bit1) = 1;
					draw_rectangle(peak,x,y,template);
				}
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
					 	if ( x + tx < xsize && y + ty < ysize){
							// printf("drawrectangle\n");
							__PIXEL(paper, x + tx, y + ty ,bit1) = 1;
				  		}
				  }
		 	}
	}
}

int main (argc,argv)
		int argc;
		char *argv[];
{
	image input,peak ,template;	
	image similarity[360];
	input = Image.createFromFilename("input",argv[1]);
	peak = Image.create("peak");
	
	int angle;
	char filename[360];	
	char simifiles[360];
	int xsize ,ysize;

	xsize = Image.xsize(input);
	ysize = Image.ysize(input);
	
	Image.make(peak , Bit1 ,xsize ,ysize);

	for (angle = 1; angle < 360; angle += 90){
		
		similarity[angle] = Image.create("similarity");
		Image.make(similarity[angle],Float,xsize,ysize);
	
		sprintf(filename,"./templates_kaki/%03d.c2d",angle);
		printf("in loop %d\n" , angle);
		template = Image.createFromFilename("template",filename);
		
		if (template == NULL){
			fprintf(stderr,"cannot open file:%s",filename);
			exit(-1);
		}
		image_calc_similarity(input, template, similarity[angle]);
	
		sprintf(simifiles,"./outsimi/outsimi%d.c2d",angle);
		Image.save(similarity[angle],simifiles,"outsimilarity");
		fprintf(stderr,"out loop");
	}

	detect_peak_3d(peak,similarity,template);
	
	Image.destroy(template);
	
	for (angle = 1;angle < 360; angle += 90){
		Image.destroy(similarity[angle]);
	}
	
	Image.save(peak, "outpeak.c2d" , "outpeak");
	Image.destroy(input);
	Image.destroy(peak);
}
