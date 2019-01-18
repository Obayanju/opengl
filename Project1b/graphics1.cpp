// OpenGL/GLUT starter kit for Windows 7 and Visual Studio 2010
// Created spring, 2011
//
// This is a starting point for OpenGl applications.
// Add code to the "display" function below, or otherwise
// modify this file to get your desired results.
//
// For the first assignment, add this file to an empty Windows Console project
//		and then compile and run it as is.
// NOTE: You should also have glut.h,
// glut32.dll, and glut32.lib in the directory of your project.
// OR, see GlutDirectories.txt for a better place to put them.

#include <cmath>
#include <cstring>
#include "glut.h"
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "Circle.h"
#include <math.h>
#include <time.h>

// Global Variables (Only what you need!)
double screen_x = 700;
double screen_y = 400;
std::vector<Circle> circles;
const double COLLISION_FRICTION = .999;

bool circleOverlap(Circle &circle, std::vector<Circle> &particles) {
	double radius = rand() % 50 + 10;
	for (int i = 0; i < particles.size(); i++) {
		double x1 = circle.getX();
		double y1 = circle.getY();
		double x2 = particles[i].getX();
		double y2 = particles[i].getY();
		double distance = sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));

		if (distance < circle.getRadius() + particles[i].getRadius()) {
			return true;
		}
	}
	return false;
}

// Your initialization code goes here.
void InitializeMyStuff()
{
	srand(time(0));
	for (int i = 0; i < 20; i++)
	{
		double radius = rand() % 50 + 10;
		double x = rand() % (int)(screen_x - (radius*2)) + radius;
		double y = rand() % (int)(screen_y - (radius*2)) + radius;
		double r = (double)rand() / RAND_MAX;
		double g = (double)rand() / RAND_MAX;
		double b = (double)rand() / RAND_MAX;
		Circle circle(x, y, radius, r, g, b);
		circle.setDx((double)rand() / RAND_MAX * .1);
		circle.setDy((double)rand() / RAND_MAX * .1);

		// check if created circle overlaps other circles
		while (circleOverlap(circle, circles)) {
			// reset the position of one of the overlapping circles
			double x = rand() % (int)(screen_x - (radius * 2)) + radius;
			double y = rand() % (int)(screen_y - (radius * 2)) + radius;
			circle.setX(x);
			circle.setY(y);
		}

		circles.push_back(circle);
	}
}

struct vectortype
{
	double x;
	double y;
};

void Collide(int p1, int p2, std::vector<Circle> &particles)
{
	vectortype en; // Center of Mass coordinate system, normal component
	vectortype et; // Center of Mass coordinate system, tangential component
	vectortype u[2]; // initial velocities of two particles
	vectortype um[2]; // initial velocities in Center of Mass coordinates
	double umt[2]; // initial velocities in Center of Mass coordinates, tangent component
	double umn[2]; // initial velocities in Center of Mass coordinates, normal component
	vectortype v[2]; // final velocities of two particles
	double m[2];	// mass of two particles
	double M; // mass of two particles together
	vectortype V; // velocity of two particles together
	double size;
	int i;

	double xdif = particles[p1].getNextX() - particles[p2].getNextX();
	double ydif = particles[p1].getNextY() - particles[p2].getNextY();

	// set Center of Mass coordinate system
	size = sqrt(xdif*xdif + ydif*ydif);
	xdif /= size; ydif /= size; // normalize
	en.x = xdif;
	en.y = ydif;
	et.x = ydif;
	et.y = -xdif;

	// set u values
	u[0].x = particles[p1].getDx();
	u[0].y = particles[p1].getDy();
	m[0] = particles[p1].getRadius()*particles[p1].getRadius();
	u[1].x = particles[p2].getDx();
	u[1].y = particles[p2].getDy();
	m[1] = particles[p2].getRadius()*particles[p2].getRadius();

	// set M and V
	M = m[0] + m[1];
	V.x = (u[0].x*m[0] + u[1].x*m[1]) / M;
	V.y = (u[0].y*m[0] + u[1].y*m[1]) / M;

	// set um values
	um[0].x = m[1] / M * (u[0].x - u[1].x);
	um[0].y = m[1] / M * (u[0].y - u[1].y);
	um[1].x = m[0] / M * (u[1].x - u[0].x);
	um[1].y = m[0] / M * (u[1].y - u[0].y);

	// set umt and umn values
	for (i = 0; i<2; i++)
	{
		umt[i] = um[i].x * et.x + um[i].y * et.y;
		umn[i] = um[i].x * en.x + um[i].y * en.y;
	}

	// set v values
	for (i = 0; i<2; i++)
	{
		v[i].x = umt[i] * et.x - umn[i] * en.x + V.x;
		v[i].y = umt[i] * et.y - umn[i] * en.y + V.y;
	}

	// reset particle values
	particles[p1].setDx(v[0].x*COLLISION_FRICTION);
	particles[p1].setDy(v[0].y*COLLISION_FRICTION);
	particles[p2].setDx(v[1].x*COLLISION_FRICTION);
	particles[p2].setDy(v[1].y*COLLISION_FRICTION);

} /* Collide */

void checkForCollision(int p1, int p2, std::vector<Circle> &All){
	double x1 = All[p1].getNextX();
	double y1 = All[p1].getNextY();
	double x2 = All[p2].getNextX();
	double y2 = All[p2].getNextY();

	double distance = sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
	if (distance < All[p1].getRadius() + All[p2].getRadius()) {
		Collide(p1, p2, All);
	}
}

// 
// Functions that draw basic primitives
//
void DrawCircle(double x1, double y1, double radius)
{
	glBegin(GL_POLYGON);
	for(int i=0; i<32; i++)
	{
		double theta = (double)i/32.0 * 2.0 * 3.1415926;
		double x = x1 + radius * cos(theta);
		double y = y1 + radius * sin(theta);
		glVertex2d(x, y);
	}
	glEnd();
}

void DrawRectangle(double x1, double y1, double x2, double y2)
{
	glBegin(GL_QUADS);
	glVertex2d(x1,y1);
	glVertex2d(x2,y1);
	glVertex2d(x2,y2);
	glVertex2d(x1,y2);
	glEnd();
}

void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3)
{
	glBegin(GL_TRIANGLES);
	glVertex2d(x1,y1);
	glVertex2d(x2,y2);
	glVertex2d(x3,y3);
	glEnd();
}

// Outputs a string of text at the specified location.
void DrawText(double x, double y, char *string)
{
	void *font = GLUT_BITMAP_9_BY_15;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
	
	int len, i;
	glRasterPos2d(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++) 
	{
		glutBitmapCharacter(font, string[i]);
	}

    glDisable(GL_BLEND);
}


//
// GLUT callback functions
//

// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	double G = .00002;
	for (size_t i = 0; i < circles.size(); i++) {
		circles[i].updatePosition(G, screen_x, screen_y);

		glColor3d(circles[i].getR(), circles[i].getG(), circles[i].getB());
		DrawCircle(circles[i].getX(), circles[i].getY(), circles[i].getRadius());
	}

	for (int i = 0; i < circles.size()-1; i++) {
		for (int j = i+1; j < circles.size(); j++) {
			checkForCollision(i, j, circles);
		}
	}

	glutSwapBuffers();
	glutPostRedisplay();
}


// This callback function gets called by the Glut
// system whenever a key is pressed.
void keyboard(unsigned char c, int x, int y)
{
	switch (c) 
	{
		case 27: // escape character means to quit the program
			exit(0);
			break;
		case 'b':
			// do something when 'b' character is hit.
			break;
		default:
			return; // if we don't care, return without glutPostRedisplay()
	}

	glutPostRedisplay();
}


// This callback function gets called by the Glut
// system whenever the window is resized by the user.
void reshape(int w, int h)
{
	// Reset our global variables to the new width and height.
	screen_x = w;
	screen_y = h;

	// Set the pixel resolution of the final picture (Screen coordinates).
	glViewport(0, 0, w, h);

	// Set the projection mode to 2D orthographic, and set the world coordinates:
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);

}

// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouse(int mouse_button, int state, int x, int y)
{
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
	{
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
	{
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) 
	{
	}
	glutPostRedisplay();
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screen_x, screen_y);
	glutInitWindowPosition(50, 50);

	int fullscreen = 0;
	if (fullscreen) 
	{
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	} 
	else 
	{
		glutCreateWindow("This appears in the title bar");
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);

	glColor3d(0,0,0); // forground color
	glClearColor(1, 1, 1, 0); // background color
	InitializeMyStuff();

	glutMainLoop();

	return 0;
}
