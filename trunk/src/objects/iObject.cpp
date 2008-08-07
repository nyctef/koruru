#include "iObject.h"

iObject::iObject(){
	deleteme = 0;
	modelviewmatrix = new GLfloat[16];
	for (int j=0;j<16;j++) modelviewmatrix[j] = 0;
}
iObject::~iObject(){
	delete modelviewmatrix;
}
