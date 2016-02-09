#include <Image.h>
#include <math.h>

void image_rotate(image input , image output){

	int ix ,iy;
	int ox ,oy;
	int cx ,cy;
	int iix,iiy,oox,ooy;
	int ixsize ,iysize;
	int oxsize ,oysize;
	int outx ,outy;
	int angle;
	double rad;
	uchar inverse_transformation;

	ixsize = Image.xsize(input);
	iysize = Image.ysize(input);

	oxsize = Image.xsize(input);
	oysize = Image.ysize(input);

	Image.make(output ,UChar ,oxsize ,oysize);
	
	cx = ixsize / 2;
	cy = iysize / 2;
	
	char filename[360];
	
	for(angle = 0; angle < 360; angle++){
		for (iy = 0; iy < iysize; iy++){
			for (ix = 0; ix < ixsize; ix++){
		//		for (oy = 0; oy < oysize + 1; oy++){
		//			for (ox = 0; ox < oxsize + 1; ox++){
					
				//		printf("angle");
				//		printf("%d\n",angle);
							
				//		printf("i(%d,%d)  ",ix ,iy);
				//		printf("ii(%d,%d)  ",iix ,iiy);

						//回転したoutputからinputの画素値を取得
						//逆変換処理
						iix = ix - cx;
						iiy = iy - cy;
					//	oox = ox - cx;
					//	ooy = oy - cy;
						ox = 0;
						oy = 0;						
				//		printf("iiharf(%d,%d)  ",iix ,iiy);

						
						//inputを回転させたoutputの座標を求める
						rad = -angle/180.0*M_PI;
						ox = cos(rad)*iix - sin(rad)*iiy + cx;
					    oy = sin(rad)*iix + cos(rad)*iiy + cy;	
					
			/*			printf("ix iy\n");
						printf("%d %d\n", ix,iy);
						printf("ox oy\n");
						printf("%d %d\n", ox,oy);
					
						printf("%f %f\n",sin(rad),cos(rad));	
						printf("%d %d\n" ,iix , iiy);
						printf("%d %d\n\n" ,oox , ooy);	
			*/		
				//		printf("o(%d,%d)\n",ox ,oy);
					//	printf("(%d,%d)\n",outx,outy);
					//	outx = oox + cx;
					//	outy = ooy + cy;
						
						if (ox >= 0 && ox < oxsize && oy >= 0 && oy < oysize){	
							inverse_transformation = __PIXEL(input ,ox ,oy ,uchar); 	
							__PIXEL(output, ix, iy, uchar) = inverse_transformation;
						}
					}
				}
		//	}
		//}
		//outputに書き出し処理
		sprintf(filename, "./templates_kaki/%03d.c2d", angle);
		Image.save(output,filename ,"output_write");
	}
}

int main(argc ,argv)
	int argc;
	char *argv[];
{
	image input ,output;
	input = Image.createFromFilename("input" ,argv[1]);
	output = Image.create("output");

	fprintf(stderr,"rotatein\n");
	image_rotate(input ,output);
	fprintf(stderr,"rotateout\n");
	Image.destroy(input);
	Image.destroy(output);
}

