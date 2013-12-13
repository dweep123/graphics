#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<unistd.h>
#include <GL/glut.h>
#include<math.h>
#include "imageloader.h"
#include "robot.h"
#include "text3d.h"

#define p 100
#define q 100
#define PI 3.14159265
#define myh 0.4
#define sp 0.5*myh
#define scale 5 
#define t_world 10
#define x_world -5
#define y_world -0.5
#define z_world -5
#define zlim 13.5
#define xlim 13.5
using namespace std;
typedef float point[3];
float jr=0.5;
int score=0;
int mousex,mousey=10,mousez,helx,helz;
float theta[6] = {0.0,0.0,0.0,0.0,0.0,0.0}, vel; /* initial joint angles */
static GLint angle = 2;
float height[4]={0,0,0,0};
int done[4]={0,0,0,0}, firstPerson=0, tower=0, jump=0, base_i=-1, base_j=-1,tile=0, tile_i=9, tile_j=9, glow=0, thirdPerson=0, helicopter=0,spot_light=0,teleport=0;
float diff_heights[4]={4,5,3.7,2.7}, base;
float rate[4]={0.43, 0.2, 0.5, 0.07};
float winw,winh;
int level_no=1;
float beans[100][4];
float rottheta=0.0;
int step;
void myinit();
void update(int v);
void drawworld();
void drawcyl(float r, float h,float x, float y , float z);
void drawsphere(float r,float x,float y , float z);
void drawhemi(float R,float x,float y,float z);
void drawCuboid(float l, float b, float h, int i, int j);
void assignBlocks ( int i, int j, float x, float y, float z, int v, int s, float vel, int m1, int m2);
void levels (int which);
void drawwater();
void texture();
void mark();
int check();
void updateWorld();
void drawblock(float l, float b, float h,int tele,int tramp);
void handleKeypress1(unsigned char key, int x, int y);
void handleKeypress2(int key, int x, int y);
GLUquadricObj *t, *h;
	GLUquadricObj *disk ;// Create pointer for storage space for object

		//example
	GLUquadricObj *Cylinder; // Create pointer for our cylinder
char buf[10];

double size=1.0;
int timer, keyPress=0, moveup=0;
float vely=0.0;
int ntiles[10][10];
int call=0;

// for score
float _angle = -30.0f;
float _scale;
const char* STRS[1] = {"player score"};
void cleanup() {
	t3dCleanup();
}
float computeScale(const char* strs[1]) {
	float maxWidth = 0;
	float width = t3dDrawWidth(strs[0]);
	if (width > maxWidth) {
		maxWidth = width;
	}   
	return 2.6f / maxWidth;
}

static double yVal=1.0;

int spot(double a, double b, double c, double d, double e, double f){

	/*

	   a, b and c -- x, y and z co-ordinates for light position

	   d, e and f -- x, y and z co-ordinates for spot light position

	 */

	//GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
	//GLfloat mat_specular[] = { 0.3, 1.0, 0.3, 1.0 };

//	GLfloat mat_shininess[] = { 50.0 };

	GLfloat light_position[] = { a,b,c, 1.0 };

	GLfloat spotDir[] = { d,e,f };

//	glClearColor (0.5, 0.5, 0.5, 0.0);

	glShadeModel (GL_SMOOTH);

	//glLightfv(GL_LIGHT0,GL_SPECULAR,mat_specular);

	glLightfv(GL_LIGHT2,GL_POSITION,light_position);

//	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);

//	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	// Definig spotlight attributes

	glLightf(GL_LIGHT2,GL_SPOT_CUTOFF,100.0);

	glLightf(GL_LIGHT2,GL_SPOT_EXPONENT,7);

	glLightfv(GL_LIGHT2,GL_SPOT_DIRECTION,spotDir);

	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_DEPTH_TEST);

	return 0;

}

GLuint _textureId[10]; //The id of the texture


//remaining code

GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
			0,                            //0 for now
			GL_RGB,                       //Format OpenGL uses for image
			image->width, image->height,  //Width and height
			0,                            //The border of the image
			GL_RGB, //GL_RGB, because pixels are stored in RGB format
			GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
			//as unsigned numbers
			image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}
void draw_jell(float x, float y,float z)
{

	Cylinder = gluNewQuadric(); // Create our new quadric object
	disk = gluNewQuadric(); // Create our new quadric object
	gluQuadricDrawStyle( Cylinder, GLU_FILL); //FILL also can be line(wire)
	gluQuadricNormals( Cylinder, GLU_SMOOTH); // For if lighting is to be used.
	gluQuadricNormals( disk, GLU_SMOOTH); // For if lighting is to be used.
	gluQuadricOrientation( Cylinder,GLU_OUTSIDE);
	gluQuadricTexture( Cylinder, GL_TRUE);// if you want to map a texture to it.
	gluQuadricTexture( disk, GL_TRUE);// if you want to map a texture to it.
	glPushMatrix();
	glTranslatef(x,y,z);
	glRotatef(-90,0,1,0);
	glColor3f( 1.0,1, 1.0 ); 
	glEnable(GL_TEXTURE_2D);
	//	glBindTexture(GL_TEXTURE_2D, _textureId[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, _textureId[2]); 
	glRotatef(rottheta,0,1,0);

	gluCylinder( Cylinder,.5, .5, .2,20,20 );
	glPushMatrix();
	glTranslatef(0,0,.1);
	gluDisk( disk,0,.5, 20,20);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0,0,0);
	gluDisk( disk,0,.5, 20,20);
	glPopMatrix();

	//gluSphere(h,jr,20,20);
	glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();


}
void drawcoins()
{
	for(int i=0;i<level_no*10;i++)
	{
		if(beans[i][3]==1)
		draw_jell(beans[i][0],beans[i][1],beans[i][2]);
	}
}

//float zoomfactor=1;

robot a=robot();

class block
{
	public:
	float block_x,block_y,block_z,l,b,h,velocity,x,y,z;
	int visible;
	int state;
	int tele;
	int tramp;
	int tox,toz;
	int min, max;
	float low, high;

	block( )
	{
		l=1.0f*scale;
		b=1.0f*scale;
		h=1.0f*scale;
	}
};

block blocks[10][10];

void display(void)
{
	mark();
	keyPress=0;
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(0.0, 0.0, 0.0);

	glPushMatrix();
	glTranslatef(0.0,0.0,-15.0);
	
	//move
	if ((int)(1000*vel)==0)
	{
		theta[5]=0;
		theta[4]=0;
		theta[2]=0;
		theta[3]=0;
	}
	else
	{
		if(timer%4<2)
		{
			theta[4]=60;
			theta[5]=0;
			theta[3]=60;
			theta[2]=0;
		}
		if(timer%4>2)
		{
			theta[5]=60;
			theta[4]=0;
			theta[2]=60;
			theta[3]=0;
		}
	}
/*	if(spot_light==1)
	{
		glClearColor (0.5, 0.5, 0.5, 0.0);
		GLfloat mat_specular[] = { 0.3, 0.3, 1.0, 1.0 };
		GLfloat mat_shininess[] = { 100.0 };
		glShadeModel (GL_SMOOTH);
		spot( a.rob_x-3,a.rob_y+5,a.rob_z,a.rob_x,0,a.rob_z);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		glEnable(GL_LIGHTING);


	}
	if(spot_light==0)
	{
		glDisable(GL_LIGHTING);
	}*/

	if (firstPerson==1)
	{
		if (a.dir==0)
			gluLookAt(a.rob_x, a.rob_y,a.rob_z+9, a.rob_x,0,a.rob_z+10, 0,1,0);
		else if (a.dir==1)
			gluLookAt(a.rob_x + 9, a.rob_y,a.rob_z, a.rob_x+10,0,a.rob_z, 0,1,0);
		else if (a.dir==2)
			gluLookAt(a.rob_x, a.rob_y,a.rob_z-9, a.rob_x,0,a.rob_z-10, 0,1,0);
		else if (a.dir==3)
			gluLookAt(a.rob_x-9, a.rob_y,a.rob_z, a.rob_x-10,0,a.rob_z, 0,1,0);
	}
	else if (tower ==1 )
	{
		//gluLookAt(3,3,3,a.rob_x,a.rob_y,a.rob_z,0,1,0);
		gluLookAt(40,40,40,0,0,0,0,1,0);
	}
	else if (tile==1)
	{
		gluLookAt(blocks[tile_i][tile_j].x*0.5,blocks[tile_i][tile_j].y+2.0,blocks[tile_i][tile_j].z*0.5,0.0,0.0,0.0,0,1,0);
	}
	else if(thirdPerson==1)
	{
                    if (a.dir==0)
                          gluLookAt(a.rob_x, a.rob_y,a.rob_z, a.rob_x,0,a.rob_z+10, 0,1,0);
                   else if (a.dir==1)
                       gluLookAt(a.rob_x , a.rob_y,a.rob_z, a.rob_x+10,0,a.rob_z, 0,1,0);
                   else if (a.dir==2)
                         gluLookAt(a.rob_x, a.rob_y,a.rob_z, a.rob_x,0,a.rob_z-10, 0,1,0);
                    else if (a.dir==3)
                             gluLookAt(a.rob_x, a.rob_y,a.rob_z, a.rob_x-10,0,a.rob_z, 0,1,0);
    	}
   	else if(helicopter==1)
    	{
        	//helx=(float)helx/winw;
        //helz=1-(float)helz/winh;
        	//printf("*** %f %f\n", helx, helz);
                gluLookAt(helx*0.1, mousey, helz*0.1, a.rob_x, a.rob_y, a.rob_z, 0,1,0);
	}
	else
	{
		gluLookAt(0,8,12,0,0,0,0,1,0);
	}
	drawcoins();
	drawworld();
	a.drawrobo();

	if(spot_light==1)
	{
		glPushMatrix();
		printf("%d %d\n",a.block_i,a.block_j);
		printf("%f %f %f\n",blocks[a.block_i][a.block_j].x,blocks[a.block_i][a.block_j].y,blocks[a.block_i][a.block_j].z);

		glTranslatef(blocks[a.block_i][a.block_j].x,blocks[a.block_i][a.block_j].y+15,blocks[a.block_i][a.block_j].z);
		glColor3f(1.0, 0.0, 0.0);
		gluSphere(h,1.5,20,20);
		glPopMatrix();


//		glClearColor (0.5, 0.5, 0.5, 0.0);
		//GLfloat mat_specular[] = { 0.3, 0.3, 1.0, 1.0 };
		//GLfloat mat_shininess[] = { 100.0 };
		glShadeModel (GL_SMOOTH);
		GLfloat ambient[]= {0.2, 0.2, 0.3};
		glLightfv( GL_LIGHT2, GL_AMBIENT, ambient);
		//GLfloat diffuse[]= {1, 1, 1};
		GLfloat diffuse[]= {0, 0, 0};
		glLightfv( GL_LIGHT2, GL_DIFFUSE, diffuse);

		//GLfloat specular[]= {1,1,1};
		GLfloat specular[]= {0,0,0};
		glLightfv(GL_LIGHT2, GL_SPECULAR, specular);
		//GLfloat position []= {blocks[a.block_i][a.block_j].x,blocks[a.block_i][a.block_j].y+7,blocks[a.block_i][a.block_j].z,1};
		spot(blocks[a.block_i][a.block_j].x,blocks[a.block_i][a.block_j].y+15,blocks[a.block_i][a.block_j].z, 0,-1,0);
		//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		//glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		glEnable(GL_LIGHTING);






		/*glLightfv( GL_LIGHT2, GL_POSITION, position);

		GLfloat spotdir []= {0,-1,0};
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotdir);
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 25); 
		glLightf(GL_LIGHT2, GL_SPOT_EXPONENT,7.0);      */ //set focusing strength*/
	}    

	if(spot_light==0)
	{
		glDisable(GL_LIGHTING);
	}

	glPopMatrix();
	glPushMatrix();

	glTranslatef(0.0f, 0.0f, -8.0f);

	GLfloat ambientColor[] = {0.4f, 0.4f, 0.4f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	GLfloat lightColor0[] = {0.6f, 0.6f, 0.6f, 1.0f};
	GLfloat lightPos0[] = {-0.5f, 0.5f, 1.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);


	glScalef(_scale, _scale, _scale);
	glColor3f(0.3f, 1.0f, 0.3f);
	glPushMatrix();
//	glTranslatef(5,5, 1.5f / _scale);
	glTranslatef(15,11,-5);
	t3dDraw3D(STRS[0],0, 0, 0.2f);
	glTranslatef(5,0,0);
	t3dDraw3D(buf,0, 0, 0.2f);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}



void mouse(int btn, int state, int x, int z)
{
if(btn==GLUT_LEFT_BUTTON)
	{
  mousex=x;
  mousez=z;
        
	}

    if(btn == '3'){
        mousey--;
    }
    if(btn=='4'){
        mousey++;
    }
}
void mouse1( int x1, int z1)
{
    helx=x1-mousex;
    helz=z1-mousez;
}
void menu(int id)
{
	if(id <11 ) angle=id;
	if(id ==11 ) exit(0);
}

	void
myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	winw=w;
    	winh=h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(float)w/(float)h,0.1f,200.0f);
/*	if (w <= h)
		glOrtho(-10.0, 10.0, -10.0 * (GLfloat) h / (GLfloat) w,
				10.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
	else
		glOrtho(-10.0 * (GLfloat) w / (GLfloat) h,
				10.0 * (GLfloat) w / (GLfloat) h, 0.0, 10.0, -10.0, 10.0);*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void make_beans()
{
	for(int i=0;i<level_no*10;i++)
	{
		int x=rand()%6;
		int z=rand()%6;
		int negx=rand()%2;
		int negz=rand()%2;
		beans[i][0]=1.0*x*scale;
		if(negx==1)
		beans[i][0]=-1.0*x*scale;
		beans[i][1]=4.0;
		beans[i][2]=1.0*z*scale;
		if(negz==1)
		beans[i][2]=-1.0*z*scale;
		beans[i][3]=1;
	}
}


int  main(int argc, char **argv)
{

strcpy(buf,"0");
	keyPress=0;
	levels(level_no);
	make_beans();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("robot");
	myinit();
	  _scale = computeScale(STRS);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(mouse1);
    	glutKeyboardFunc(handleKeypress1);
	glutSpecialFunc(handleKeypress2);
	glutReshapeFunc(myReshape);
	glutTimerFunc(1,update , 1);
	glutMainLoop();
	return 0;
}
void myinit()
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHT0);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	t3dInit();
	h=gluNewQuadric();
	gluQuadricDrawStyle(h, GLU_FILL);
	t=gluNewQuadric();
	gluQuadricDrawStyle(t, GLU_FILL);
	Image* image = loadBMP("tile3.bmp");
	_textureId[0] = loadTexture(image);
	delete image;
	Image* image1 = loadBMP("space.bmp");
	_textureId[1] = loadTexture(image1);
	delete image1;
	Image* image2 = loadBMP("jelly.bmp");
	_textureId[2] = loadTexture(image2);
	delete image2;
	Image* image3 = loadBMP("tramp.bmp");
	_textureId[3] = loadTexture(image3);
	delete image3;
}
void update(int v)
{
	int i,j,flag1=0,flag=0;
	float x,y,z;
	x=a.rob_x;
	y=a.rob_y;
	z=a.rob_z;
	timer++;
	rottheta=rottheta+7;
	updateWorld();
	for(int k=0;k<level_no*10;k++)
	{
		if(beans[k][3]==1 && abs(a.rob_x-beans[k][0])<=2 && abs(a.rob_y-beans[k][1])<=2 && abs(a.rob_z-beans[k][2])<=2)
		{
			score=score+10;
			beans[k][3]=0;
			//printf("%d",score);
			 // convert 123 to string [buf]
		sprintf(buf,"%d",score);
			//char *s;
			//strcpy(STRS[0],);
			break;
		}

	}
	if (a.block_i!=-1 && a.block_j!=-1 && jump==0)
	{
		//printf("%f\n",blocks[a.block_i][a.block_j].y);
		if (blocks[a.block_i][a.block_j].state==1)
			a.rob_x=blocks[a.block_i][a.block_j].x;
		if (blocks[a.block_i][a.block_j].state==2)
			a.rob_z=blocks[a.block_i][a.block_j].z;
		if (blocks[a.block_i][a.block_j].state==3)
			a.rob_y=blocks[a.block_i][a.block_j].y+0.45+blocks[a.block_i][a.block_j].h/2;
	}
	for (i=0;i<10;i++)
	{
		for(j=0;j<10;j++)
		{
			if(a.rob_x>blocks[i][j].x-blocks[i][j].l/2 && a.rob_x<=blocks[i][j].x+blocks[i][j].l/2 && a.rob_z>blocks[i][j].z-blocks[i][j].b/2 && a.rob_z<=blocks[i][j].z+blocks[i][j].b/2 && blocks[i][j].visible==1)
			{
				flag=1;
				if ( (int)(10*(a.rob_y -(blocks[i][j].y+ blocks[i][j].h/2 + a.leg_h + a.leg_r/2 -sp +a.leg_r)))<0.05 &&  (int)(10*(a.rob_y -(blocks[i][j].y+ blocks[i][j].h/2 + a.leg_h + a.leg_r/2 -sp +a.leg_r)))>-0.05)
				//if ( (int)(1000*a.rob_y) == (int)(1000*(blocks[i][j].y+ blocks[i][j].h/2 + a.leg_h + a.leg_r/2 -sp +a.leg_r)))
				{
					a.block_i=i;
					a.block_j=j;
					flag1=1;
					if (jump==1 && (i!=base_i || j!=base_j))
					{
						vel=0.0;
						a.rob_x=blocks[i][j].x;
						a.rob_z=blocks[i][j].z;
						jump=0;
						base_i=-1;
						base_j=-1;
					}
					break;
				}
			}
		}
		if (flag1==1)
			break;
	}
	if (flag1==0)
	{
		a.block_i=-1;
		a.block_j=-1;
	}
	
	if (a.block_i!=-1 && a.block_j!=-1)
	{
		//printf("robot coods %f %f %f\n",a.rob_x,a.rob_y,a.rob_z);
		//printf("%f\n",blocks[a.block_i][a.block_j].y);
		if (blocks[a.block_i][a.block_j].state==1)
			a.rob_x=blocks[a.block_i][a.block_j].x;
		if (blocks[a.block_i][a.block_j].state==2)
			a.rob_z=blocks[a.block_i][a.block_j].z;
		if (blocks[a.block_i][a.block_j].state==3)
			a.rob_y=blocks[a.block_i][a.block_j].y+0.45+blocks[a.block_i][a.block_j].h/2;
	}
	else if(flag1==0)
	{
		if (/*a.rob_y>0.45 &&*/ jump==0)
		{
			a.rob_y-=0.1;
		}
	}
	if (vel>=0.1 && keyPress==0 && jump==0)
	{
		vel-=0.1;
	}
	if (jump==1)
	{
		if (moveup>=1 && moveup<20)
		{
			moveup++;
			a.rob_y+=0.1;
		}
		if (moveup==20)
		{
			moveup=-1;
		}
		if (moveup<=-1 && moveup>=-20 && flag1==0)
		{
			moveup--;
			a.rob_y-=0.1;
		}
		if(moveup==-20)
		{
			moveup=0;
			jump=0;
			vel=0.0;
			a.rob_y=base;
		}
	}
//	if (jump==0)
//	{
		if (a.dir==0)
			a.rob_z+=vel;
		else if (a.dir==1)
			a.rob_x+=vel;
		else if (a.dir==2)
			a.rob_z-=vel;
		else if (a.dir==3)
			a.rob_x-=vel;
//	}
		
	/*	if (a.rob_z<=-zlim && a.dir==0)
		a.dir=2;
	else if (a.rob_z>=zlim && a.dir==2)
		a.dir=0;
	else if (a.rob_x<=-xlim && a.dir==3)
		a.dir=1;
	else if (a.rob_x>=xlim && a.dir==1)
		a.dir=3;*/

//	glutPostRedisplay();
	if (check()==0 && flag==1)
	{
		/*a.rob_x=x;
		a.rob_y=y;
		a.rob_z=z;*/
		if (a.dir==0)
			a.dir=2;
		else if(a.dir==2)
			a.dir=0;
		else if(a.dir==1)
			a.dir=3;
		else if(a.dir==3)
			a.dir=1;
		if(vel==0.0)
			vel=0.1;
	}
	if(blocks[a.block_i][a.block_j].tele==1 && teleport==1)
	{
		int h,g;
		h=a.block_i;
		g=a.block_j;
		a.block_i=blocks[h][g].tox;
		a.block_j=blocks[h][g].toz;
		a.rob_x=blocks[a.block_i][a.block_j].x;
		a.rob_z=blocks[a.block_i][a.block_j].z;
		a.rob_y=blocks[a.block_i][a.block_j].y+scale;
		teleport=0;
	}
//	printf("%d %d\n",a.block_i,a.block_j);
	glutTimerFunc(1,update, 1);
}
void drawCuboid(float l, float b, float h, int i, int j)
{

	float ll=l/2;
	float bb=b/2;
	float hh=h/2;
	float bs=1.5;	

	
	if(i==2 && j==3){
		bs=-1.0*height[0];
		//drawblock(l,b,h,ll,bb,hh,bs);
	}
	if(i==5 && j==9){
		bs=-1.0*height[1];
		//drawblock(l,b,h,ll,bb,hh,bs);
	}
	if(i==8 && j==7){
		bs=-1.0*height[2];
		//drawblock(l,b,h,ll,bb,hh,bs);
	}
	if(i==6 && j==1){
		bs=-1.0*height[3];
		//drawblock(l,b,h,ll,bb,hh,bs);
	}
	
	

	bs=1.5;
	//drawblock(l,b,h,ll,bb,hh,bs);
	


}
void drawhemi(float R,float x,float y,float z)
{
    	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(x,y,z);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(0.0, 1.0, 0.0);

	// Array of latitudinal triangle strips, each parallel to the equator, stacked one
	// above the other from the equator to the north pole.
	for(int j = 0; j < q; j++)
	{
		// One latitudinal triangle strip.
		glBegin(GL_TRIANGLE_STRIP);
		for(int i = 0; i <= p; i++)
		{
			glVertex3f( R * cos( (float)(j+1)/q * PI/2.0 ) * cos( 2.0 * (float)i/p * PI ),
					R * sin( (float)(j+1)/q * PI/2.0 ),
					R * cos( (float)(j+1)/q * PI/2.0 ) * sin( 2.0 * (float)i/p * PI ) );
			glVertex3f( R * cos( (float)j/q * PI/2.0 ) * cos( 2.0 * (float)i/p * PI ),
					R * sin( (float)j/q * PI/2.0 ),
					R * cos( (float)j/q * PI/2.0 ) * sin( 2.0 * (float)i/p * PI ) );         
		}
		glEnd();
	}
	glPopMatrix();
	glColor3f(0.0, 1.0, 0.0);

}
void drawcyl(float r, float h,float x, float y , float z)
{
	// draws body //right hand //left hand// right leg // left leg
	glPushMatrix();
	glTranslatef(x,y,z);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(t,r,r,h,100,100);
	glPopMatrix();
}
void drawsphere(float r,float x,float y , float z)
{
	glPushMatrix();
	glTranslatef(x,y,z);
	//glColor3f(1.0,0.0,0.0);
	gluSphere(h,r,10,10);
	glPopMatrix();
}
void drawworld()
{
/*	float gl=-5,gb=-2;
	int i,j;
	for(i=0;i<10;i++)
	{
		for(j=0;j<10;j++)
		{
			if(ntiles[i][j]==0)
			{
			}
			else
			{
			glPushMatrix();
			glTranslatef(gl,0.0,- gb);
			drawCuboid(1.0f, 0.0f, 1.0f,i,j);
			glPopMatrix();
			gl=gl+1.02;
			}
		}
		gb=gb-1.02;
		gl=-5;

	}*/
	int i,j;
	for (i=0; i<10; i++)
	{
		for (j=0; j<10; j++)
		{
			if (blocks[i][j].visible==1)
			{
			
				glPushMatrix();
				glTranslatef(blocks[i][j].x,blocks[i][j].y,blocks[i][j].z);
				/*if (tile==1 && i==0 && j==0)
				{
					printf("enablong glow\n");
					glow=1;
					//glMaterialfv(gl_emission, glow_color);
					glColorMaterial ( GL_FRONT_AND_BACK,GL_EMISSION ) ;
					glEnable ( GL_COLOR_MATERIAL ) ;
				}
				else if(glow==1)
				{
					glow=0;
					glDisable(GL_COLOR_MATERIAL);
				}*/
				drawblock(blocks[i][j].l,blocks[i][j].b,blocks[i][j].h,blocks[i][j].tele,blocks[i][j].tramp);
				glPopMatrix();
			}
		}
	}
	drawwater();

}

void drawblock(float l, float b, float h,int tele,int tramp)
{
	//Bottom face
//	if(glow==0)
	glColor3f(0.0,0.0,0.0);
	glBegin(GL_POLYGON);
	glVertex3f( -l/2 , -h/2, -b/2 );
	glVertex3f(  l/2, -h/2,  -b/2 );
	glVertex3f( l/2, -h/2,  b/2 );
	glVertex3f( -l/2, -h/2, b/2 );
	glEnd();

	glPushMatrix();
	glBegin(GL_POLYGON);

	// back face
//	if(glow==0)
	//	glColor3f(0.0,1.0,1.0);
	glBegin(GL_POLYGON);
	glVertex3f( -l/2, -h/2, -b/2 );
	glVertex3f( l/2,  -h/2, -b/2 );
	glVertex3f( l/2,  h/2, -b/2 );
	glVertex3f( -l/2,  h/2, -b/2 );
	glEnd();

//	if(glow==0)
//	{
	if(tele==0)
	glColor3f( 0.0, 1.0, 1.0 ); 
	if(tramp==1)
	glColor3f( 1.0, 1.0, 1.0 ); 
	else if(tele==1)
	glColor3f( 1.0, 1.0, 0.0 ); 

		glEnable(GL_TEXTURE_2D);
		if(tramp!=1)
		glBindTexture(GL_TEXTURE_2D, _textureId[0]);
		else
		glBindTexture(GL_TEXTURE_2D, _textureId[3]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	}
	glBegin(GL_POLYGON);

	glNormal3f(0.0, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-l/2, h/2, -b/2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(  l/2, h/2,  -b/2 );
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f( l/2, h/2,  b/2 );
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f( -l/2, h/2, b/2 );

	glEnd();

//	if(glow==0)
		glDisable(GL_TEXTURE_2D);

//	if(glow==0)
		glColor3f(0.0,0.0,0.0);

	glEnd();
	glPopMatrix();

	// Front face
//	if(glow==0)
		glColor3f(0.0,0.0,0.0);
	glBegin(GL_POLYGON);
	glVertex3f(  -l/2, -h/2, b/2 );
	glVertex3f(  l/2,  -h/2, b/2 );
	glVertex3f( l/2,  h/2, b/2 );
	glVertex3f( -l/2, h/2, b/2 );
	glEnd();

	// Left face
//	if(glow==0)
		glColor3f(0.0,0.0,0.0);
	glBegin(GL_POLYGON);
	//	glColor3f( 0.0, 1.0, 1.0 ); 
	glVertex3f( -l/2, -h/2, -b/2 );
	glVertex3f( -l/2,  -h/2,  b/2 );
	glVertex3f( -l/2,  h/2, b/2 );
	glVertex3f( -l/2, h/2, -b/2 );
	glEnd();

	// Right face
//	if(glow==0)
		glColor3f(0.0,0.0,0.0);
	glBegin(GL_POLYGON);
	//	glColor3f( 1.0, 0.0, 1.0 ); 
	glVertex3f( l/2, -h/2, -b/2 );
	glVertex3f( l/2,  -h/2,  b/2 );
	glVertex3f( l/2,  h/2, b/2 );
	glVertex3f( l/2, h/2, -b/2 );
	glEnd();


}
int check()
{
	int i,j,flag=1;
	for (i=0;i<10;i++)
	{
		for(j=0;j<10;j++)
		{
			//printf("check %f %f\n",a.rob_y-(a.leg_h+a.leg_r/2-sp+a.leg_r),blocks[i][j].y+blocks[i][j].h/2);
			if( !( (int)(1000*(a.rob_x-a.body_r))>=(int)(1000*(blocks[i][j].x+blocks[i][j].l/2)) || (int)(1000*((a.rob_x+a.body_r))<=(int)(1000*(blocks[i][j].x-blocks[i][j].l/2))) || (int)(1000*(a.rob_y+(a.body_h+sp+a.head)))<=(int)(1000*(blocks[i][j].y-blocks[i][j].h/2)) || (int)(1000*(a.rob_y-(a.leg_h+a.leg_r/2-sp+a.leg_r))>=(int)(1000*blocks[i][j].y+blocks[i][j].h/2))  ||  (int)(1000*(a.rob_z-a.body_r))>=(int)(1000*(blocks[i][j].z+blocks[i][j].b/2)) || (int)(1000*(a.rob_z+a.body_r))<=(int)(1000*(blocks[i][j].z-blocks[i][j].b/2)) ) )
			{
				flag=0;
			}
		}
	}
	return flag;
}
void mark()
{
	if(call==0)
	{
		int i,j;
		int rand1,rand2;
		for(i=0;i<10;i++)
			for(j=0;j<10;j++)
				ntiles[i][j]=1;
		for(i=0;i<10;i++)
		{
			rand1=rand()%10;
			rand2=rand()%10;
			ntiles[rand1][rand2]=0;
		}
		call=1;
	}

}
void handleKeypress2(int key, int x, int y)
{
	keyPress=1;
	//printf("%d\n",a.dir);
	if (key== GLUT_KEY_UP)
	{
		if (jump==0)
		{
			step++;
			if (vel==0.0)
				vel=0.2;
			else if(vel<0.6)
				vel+=0.1;
		}
	}
	else if (key== GLUT_KEY_DOWN)
	{
		if (jump==0)
		{
			if (vel>=0.1)
				vel-=0.1;
		}
	}
	else if (key== GLUT_KEY_RIGHT)
	{
		if (a.dir==0)
			a.dir=3;
		else
			a.dir--;
	}
	else if (key== GLUT_KEY_LEFT)
	{
		if (a.dir==3)
			a.dir=0;
		else
			a.dir++;
	}
}
void handleKeypress1(unsigned char key, int x, int y)
{
	if (key=='j'/* && moveup==0*/)
	{
		if (jump==0)
		{
			jump=1;
			vely=0.1;
			if (vel>0)
				vel=0.125;
			moveup=1;
			base=a.rob_y;
			base_i=a.block_i;
			base_j=a.block_j;
		}
	}
	if (key== 'f' )
	{
		firstPerson=(firstPerson+1)%2;
		tower=0;
		tile=0;
		thirdPerson=0;
		helicopter=0;
	}
	if (key== 't' )
	{
		tower=(tower+1)%2;
		firstPerson=0;
		tile=0;
		thirdPerson=0;
		helicopter=0;
	}
	if (key == 'i')
	{
		tile=(tile+1)%2;
		tower=0;
		firstPerson=0;
		thirdPerson=0;
		helicopter=0;
	}
	if(key == 'b')
	{
	        thirdPerson=(thirdPerson+1)%2;   
		tower=0;
		firstPerson=0;
		tile=0;
		helicopter=0;
    	}
   	 if(key=='h')
    	{
        	 helicopter=(helicopter+1)%2;
		tower=0;
		firstPerson=0;
		tile=0;
		thirdPerson=0;
    	}
	if (tile==1)
	{
		if(key == 'a')
		{
			if (tile_j>0)
				tile_j--;
		}
		if(key == 'd')
		{
			if (tile_j<9)
				tile_j++;
		}
		if (key == 's')
		{
			if (tile_i<9)
				tile_i++;
		}
		if (key == 'w')
		{
			if (tile_i>0)
				tile_i--;
		}
	}
	if(key=='s')
	{
		spot_light=(spot_light+1)%2;
	}
	if(key=='w')
	{
		teleport=(teleport+1)%2;
	}
	//printf("tile %d %d\n",tile_i,tile_j);
}

void levels (int which)
{
	float xdir,ydir,zdir;
	float velocity=0.125,min=which,max=which;
	for(int i=0;i<t_world;i++)
	{
		xdir=x_world;
		ydir=y_world;
		zdir=z_world;
		int vis_arr[which*2];
		

		for (int j=0;j<which*2;j++)
		{
			vis_arr[j]=rand()%10;
		}
		for(int j=0;j<t_world;j++)
		{
			int visible=1;
			/*for(int k=0;k<which*2;k++)
			{
				if(j==vis_arr[k])
				{
					visible=0;
					break;
				}
			}*/
			assignBlocks(i,j,xdir+j,ydir,zdir+i,visible,0,0,0,0);
		}
	}
	for(int i=0;i<t_world;i++)
	{
		xdir=x_world;
		ydir=y_world;
		zdir=z_world;
		int state_arr[which*2];
		int state1[t_world];
         	for(int k=0;k<t_world;k++)
		{
			state1[k]=0;
		}
		for (int j=0;j<which*2;j++)
		{
			state_arr[j]=rand()%10;
			state1[state_arr[j]]=rand()%3+1;
		}
		for(int j=0;j<t_world;j++)
		{
			int state=0;
			for(int k=0;k<which*2;k++)
			{
				if(j==state_arr[k])
				{
					state=state1[state_arr[k]];
					for(int m=-which;m<=which;m++)
					{
						if(state==1)
						{
							if(m!=0 && j+m>=0 && j+m<t_world)
							assignBlocks(i,j+m,xdir+j,ydir,zdir+i+m,0,blocks[i][j+m].state,0,0,0);
						}
						if(state==2)
						{
							if(m!=0 && i+m>=0 && i+m<t_world)
							assignBlocks(i+m,j,xdir+j+m,ydir,zdir+i,0,blocks[i+m][j].state,0,0,0);
						}

					}
				}
			}
			assignBlocks(i,j,xdir+j,ydir,zdir+i,blocks[i][j].visible,state,velocity,min,max);
		}
	}
	for(int i=0;i<level_no;i++)
	{
		int from_x;
		int from_z;
		int to_x;
		int to_z;
		while(1)
		{
			from_x=rand()%10;
			from_z=rand()%10;
			if(blocks[from_x][from_z].visible==1)
			{
				to_x=rand()%10;
				to_z=rand()%10;
				if(blocks[to_x][to_z].visible==1 && to_x!=from_x && to_x!=from_z)
					break;
			}
		}
		blocks[from_x][from_z].tele=1;
		blocks[from_x][from_z].tox=to_x;
		blocks[from_x][from_z].toz=to_z;
		blocks[to_x][to_z].tele=1;
		blocks[to_x][to_z].tox=from_x;
		blocks[to_x][to_z].toz=from_z;

	}
	for(int i=0;i<level_no;i++)
	{
		int x,y;
		while(1)
		{
			x=rand()%10;
			y=rand()%10;
			if(blocks[x][y].tele!=1 && blocks[x][y].visible==1)
			{
				blocks[x][y].tramp=1;
				break;
			}
		}
	}
	int i,j;
	for (i=0;i<10;i++)
	{
		for (j=0;j<10;j++)
		{
			blocks[i][j].block_x*=scale;
			blocks[i][j].block_y*=scale;
			blocks[i][j].block_z*=scale;

			blocks[i][j].x=blocks[i][j].block_x;
			blocks[i][j].y=blocks[i][j].block_y;
			blocks[i][j].z=blocks[i][j].block_z;
			if (blocks[i][j].state==3)
			{
				blocks[i][j].low=blocks[i][j].block_y-blocks[i][j].min*blocks[i][j].h;
				blocks[i][j].high=blocks[i][j].block_y+blocks[i][j].max*blocks[i][j].h;
			}
			else if (blocks[i][j].state==2)
			{
				blocks[i][j].low=blocks[i][j].block_z-blocks[i][j].min*blocks[i][j].h;
				blocks[i][j].high=blocks[i][j].block_z+blocks[i][j].max*blocks[i][j].h;
			}
			else if (blocks[i][j].state==1)
			{
				blocks[i][j].low=blocks[i][j].block_x-blocks[i][j].min*blocks[i][j].h;
				blocks[i][j].high=blocks[i][j].block_x+blocks[i][j].max*blocks[i][j].h;
			}
		}
	}	
}

void assignBlocks ( int i, int j, float x, float y, float z, int v, int s, float vel, int m1, int m2)
{
	blocks[i][j].block_x=x;
	blocks[i][j].block_y=y;
	blocks[i][j].block_z=z;
	blocks[i][j].visible=v;
	blocks[i][j].state=s;
	blocks[i][j].velocity=vel;
	blocks[i][j].min=m1;
	blocks[i][j].max=m2;
}
void updateWorld()
{
	int i,j;
	for (i=0;i<10;i++)
	{
		for(j=0;j<10;j++)
		{
			if(blocks[i][j].state==3)
			{
				if( blocks[i][j].y<=blocks[i][j].low*1.2 || blocks[i][j].y>=blocks[i][j].high*1.2)
				{
					blocks[i][j].velocity*=-1;
				}
				blocks[i][j].y+=blocks[i][j].velocity;
			}
			if(blocks[i][j].state==2)
			{
				if( blocks[i][j].z==blocks[i][j].low || blocks[i][j].z==blocks[i][j].high)
				{
					blocks[i][j].velocity*=-1;
				}
				blocks[i][j].z+=blocks[i][j].velocity;
			}
			if(blocks[i][j].state==1)
			{
				if( blocks[i][j].x==blocks[i][j].low || blocks[i][j].x==blocks[i][j].high)
				{
					blocks[i][j].velocity*=-1;
				}
				blocks[i][j].x+=blocks[i][j].velocity;
			}
		}
	}
}
void drawwater()
{
	glColor3f(1.0,1.0,1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_POLYGON);

	glNormal3f(0.0, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-100, -250, -100);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-100, 250, -100);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(100, 250, -100);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(100, -250, -100);

	glEnd();
	glBegin(GL_POLYGON);

	glNormal3f(0.0, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-100, -250, 100);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-100, 250, 100);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(100, 250, 100);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(100, -250, 100);

	glEnd();
	glBegin(GL_POLYGON);

	glNormal3f(0.0, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-100,-10, -100);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-100, -10, 100);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(100, -10, 100);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(100, -10, -100);

	glEnd();


glDisable(GL_TEXTURE_2D);

}
void texture()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f, 1.0f, -6.0f);

	GLfloat ambientLight[] = {0.2f, 0.2f, 0.2f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	GLfloat directedLight[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat directedLightPos[] = {-10.0f, 15.0f, 20.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
	glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId[0]);
	//printf("this is id ->%d\n",_textureId);

	//Bottom
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor3f(1.0f, 0.2f, 0.2f);
	glBegin(GL_QUADS);

	glNormal3f(0.0, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.5f, -2.5f, 2.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.5f, -2.5f, 2.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(2.5f, -2.5f, -2.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.5f, -2.5f, -2.5f);

	glEnd();

	//Back
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLES);

	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.5f, -2.5f, -2.5f);
	glTexCoord2f(5.0f, 5.0f);
	glVertex3f(0.0f, 2.5f, -2.5f);
	glTexCoord2f(10.0f, 0.0f);
	glVertex3f(2.5f, -2.5f, -2.5f);

	glEnd();

	//Left
	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0f, 0.7f, 0.3f);
	glBegin(GL_QUADS);

	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-2.5f, -2.5f, 2.5f);
	glVertex3f(-2.5f, -2.5f, -2.5f);
	glVertex3f(-2.5f, 2.5f, -2.5f);
	glVertex3f(-2.5f, 2.5f, 2.5f);

	glEnd();

	glutSwapBuffers();
}
