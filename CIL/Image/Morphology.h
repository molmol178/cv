/*
 * Morphology.h
 */




#ifndef _morphology_h
#define _morphology_h



#include "Image.h"
#include "Funcs.h"



void image__dilation P ((image dest,image src,
			 image structuring_element,long spot_x,long spot_y));
void image__erosion  P ((image dest,image src,
			 image structuring_element,long spot_x,long spot_y));

void image__opening  P ((image dest,image src,
			 image structuring_element,long spot_x,long spot_y));
void image__closing  P ((image dest,image src,
			 image structuring_element,long spot_x,long spot_y));



#endif  _morphology_h
