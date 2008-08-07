#include "cMenuItem.h"

cMenuItem::cMenuItem(string nname) {
	
	init(nname, NULL, "continue");

}

cMenuItem::cMenuItem(string nname, c3DSModel* nmodel, string ntarget) {

	init(nname, nmodel, ntarget);
	
}

cMenuItem::~cMenuItem(){}

void cMenuItem::draw() {
	/*GLfloat matrix[16] = {0};
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	PrintMatrix(matrix);*/
	glPushMatrix();
	glDisable(GL_TEXTURE_2D);
	if (active) {
		glColor3f(0.7,0.7,0.7);
		glScalef(1.1, 1.1, 1.1);
	} else {
		glColor3f(0.4,0.4,0.4);
	}
	
	if (clicked) {
		glScalef(0.9,0.9,0.9);
	}
	
	if (model == NULL) {
		glBegin(GL_QUADS);
			glVertex2f(-1, +0.5);
			glVertex2f(+1, +0.5);
			glVertex2f(+1, -0.5);
			glVertex2f(-1, -0.5);
		glEnd();
	} else {
		model->Draw();
	}
	glTranslatef(-0.5, -0.5, 0.1);
	glColor3f(1, 0, 0);
	glScalef(0.03,0.03,0.03);
	font->draw(name.c_str());
	glPopMatrix();
}

void cMenuItem::init(string nname, c3DSModel* nmodel, string ntarget) {
	
	active = clicked = false;
	name = nname;
	model = nmodel;
	if (ntarget == "") {
		target = "continue";
	} else {
		target = ntarget;
	}
	font = new cFont("data/font3.png", 14, 32, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]()^£""';:#");
}
