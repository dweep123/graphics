#include <GL/glut.h>
#include <assert.h>
#include <fstream>

#include "robot.h"

using namespace std;
#define diam 10
#define world_len -5
#define world_wid 3
#define tile_width 3
#define tile_len 3
#define tilesp 0.5
#define PI 3.14159265
#define p 100
#define q 100
#define myh 0.5
#define sp 0.5*myh
#define scale 5
void drawhemi(float,float,float,float);
void drawcyl(float,float,float,float,float);
void drawsphere(float,float,float,float);
robot::robot(){
		head=3*0.5;body_r=3*0.5;body_h=5*0.5;arm_r=0.5*0.5;arm_h=2*0.5;	leg_r=0.5*0.5;leg_h=2.0*0.5;leg_sp=1*0.5;ant_r=0.125*0.5;ant_h=1*0.5;eye_r=0.25*0.5;dir=0;
block_i=-1;block_j=-1;
 rob_x=-5*scale;rob_y=2;rob_z=-5*scale;

	}
void robot::drawrobo()
	{
extern float theta[6];
	glPushMatrix();
	glTranslatef(rob_x,rob_y,rob_z);
	if(dir==1)
		glRotatef(90,0.0,1.0,0.0);
	else if(dir==2)
		glRotatef(180,0.0,1.0,0.0);
	else if(dir==3)
		glRotatef(270,0.0,1.0,0.0);
	glPushMatrix();
	glRotatef(theta[0],0.0,1.0,0.0);
	//draw head
	drawhemi(head,0,body_h+sp,0);

	//draw right antena
	glPushMatrix();
	glTranslatef(body_r/4+2*sp,body_h+head,0);
	glRotatef(-45,0.0,0.0,1.0);
	drawcyl(ant_r,ant_h,0,0,0);
	glPopMatrix();
	
	//draw left antena
	glPushMatrix();
	glTranslatef(-body_r/4-2*sp,body_h+head,0);
	glRotatef(45,0.0,0.0,1.0);
	drawcyl(ant_r,ant_h,0,0,0);
	glPopMatrix();
	
	//draw right eye
	glColor3f(1.0,1.0,1.0);
	drawsphere(eye_r,body_r/2.5,body_h+body_r/1.5,head-eye_r/1.5);
	//draw left eye
	drawsphere(eye_r,-body_r/2.5,body_h+body_r/1.5,head-eye_r/1.5);
	glColor3f(0.0,1.0,0.0);

	
	glPopMatrix();

	//draw body
	glPushMatrix();
	glRotatef(theta[1],0.0,1.0,0.0);
	drawcyl(body_r,body_h,0,0,0);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(body_r+sp+arm_r,body_h,0);
	glRotatef(-theta[2],1.0,0.0,0.0);
	//draw arm right
	drawcyl(arm_r,arm_h,0,-body_h/2-arm_r,0);
	// draw right arm upper hemi
	drawhemi(arm_r,0,-arm_r-sp,0);
	// draw right arm lower hemi
	drawhemi(-arm_r,0,-3*body_h/4+arm_r+sp,0);
	glPopMatrix();
	
	
	glPushMatrix();
	glTranslatef(-body_r-sp-arm_r,body_h,0);
	glRotatef(-theta[3],1.0,0.0,0.0);
	//draw arm left
	drawcyl(arm_r,arm_h,0,-body_h/2-arm_r,0);
	// draw left arm upper hemi
	drawhemi(arm_r,0,-arm_r-sp,0);
	// draw left arm lower hemi
	drawhemi(-arm_r,0,-3*body_h/4+arm_r+sp,0);
	glPopMatrix();

	glPushMatrix();
	glRotatef(theta[4],1.0,0.0,0.0);
	//draw right leg
	drawcyl(leg_r,leg_h,leg_sp,-leg_h,0);
	// draw right leg hemi
	drawhemi(-leg_r,leg_sp,-leg_h-leg_r/2+sp,0);
	glPopMatrix();

	glPushMatrix();
	glRotatef(theta[5],1.0,0.0,0.0);
	//draw left leg
	drawcyl(leg_r,leg_h,-leg_sp,-leg_h,0);
	// draw left leg hemi
	drawhemi(-leg_r,-leg_sp,-leg_h-leg_r/2+sp,0);
	glPopMatrix();

	glPopMatrix();
	
}
