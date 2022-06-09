/*==================================================================================
* COSC 363  Assignment 2
* Author: Rory Patterson
*
* A basic ray tracer
* See Lab06.pdf   for details.
*===================================================================================
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "SceneObject.h"
#include "Ray.h"
#include <GL/freeglut.h>
#include "Plane.h"
#include "TextureBMP.h"
#include "Cylinder.h"
#include "Cone.h"
using namespace std;

const float EDIST = 40.0;
const int NUMDIV = 500;
const int MAX_STEPS = 10;
const float XMIN = -10.0;
const float XMAX = 10.0;
const float YMIN = -10.0;
const float YMAX = 10.0;

vector<SceneObject*> sceneObjects;
TextureBMP texture;

//---The most important function in a ray tracer! ----------------------------------
//   Computes the colour value obtained by tracing a ray and finding its
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step)
{
	glm::vec3 backgroundCol(0);						//Background colour = (0,0,0)
	glm::vec3 lightPos(10, 40, -3);					//Light's position
	glm::vec3 color(0);
	SceneObject* obj;

    ray.closestPt(sceneObjects);					//Compare the ray with all objects in the scene
    if(ray.index == -1) return backgroundCol;		//no intersection
	obj = sceneObjects[ray.index];					//object on which the closest point of intersection is found

	if (ray.index == 3) {
 		//Stripe pattern
 		int stripeWidth = 5;
 		int iz = (ray.hit.z) / stripeWidth;
		int ix = (ray.hit.x) / stripeWidth;
 		int k = iz % 2; //2 colors
		int l = ix % 2;

		if (((k == 0 && l == 0) || (k == -1 && l == -1)) && (ray.hit.x <= 0)) {
			color = glm::vec3(0.5, 1, 0.37);
		} else if(((k == -1 && l == 0) || (k == 0 && l == 1)) && (ray.hit.x > 0)) {
			color = glm::vec3(0.5, 1, 0.37);
		} else {
			color = glm::vec3(0.7, 0, 0);
		}


 		obj->setColor(color);

	}


	color = obj->lighting(lightPos, -ray.dir, ray.hit);						//Object's colour

	glm::vec3 lightVec = lightPos - ray.hit;
	Ray shadowRay(ray.hit, lightVec);

	shadowRay.closestPt(sceneObjects);

	if ((shadowRay.index > -1) ) {
		if (shadowRay.index == 2 || shadowRay.index == 4) {
			color = 0.7f * obj->getColor();
		} else {
			color = 0.2f * obj->getColor(); //0.2 = ambient scale factor
		}

	}

	if (obj->isReflective() && step < MAX_STEPS)
	{
		float rho = obj->getReflectionCoeff();
		glm::vec3 normalVec = obj->normal(ray.hit);
		glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVec);
		Ray reflectedRay(ray.hit, reflectedDir);
		glm::vec3 reflectedColor = trace(reflectedRay, step + 1);
		color = color + (rho * reflectedColor);
	}

	if (obj->isRefractive() && step < MAX_STEPS) {
		float eta = obj->getRefractionCoeff();
		glm::vec3 normalVec = obj->normal(ray.hit);
        glm::vec3 refractedDir = glm::refract(ray.dir, normalVec, eta);
        Ray refractedRay1(ray.hit, refractedDir);
        refractedRay1.closestPt(sceneObjects);
        glm::vec3 m = obj->normal(refractedRay1.hit);
        glm::vec3 refractedDir2 = glm::refract(refractedDir, -1.0f*m, 1.0f/eta);

        Ray refractedRay2(refractedRay1.hit, refractedDir2);
        glm::vec3 refractedCol = trace(refractedRay2, step+1); // recursion

        color = color + refractedCol;
    }

	if (obj->isTransparent() && step < MAX_STEPS) {
        float transCo = obj->getTransparencyCoeff();
        Ray internalRay(ray.hit, ray.dir);
        internalRay.closestPt(sceneObjects);
        glm::vec3 newColor = trace(internalRay, step + 1);
        color = (color * 0.3f) + (transCo * newColor);
	}




	if (ray.index == 1 && step < MAX_STEPS) {
		glm::vec3 center(10, 10, -60.0);
		glm::vec3 d = glm::normalize(ray.hit-center);
		float u = (0.5-atan2(d.z,d.x)+M_PI)/(2*M_PI);
		float v = 0.5+asin(d.y)/M_PI;
		color = texture.getColorAt(u, v);
   }



	return color;
}

//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
void display()
{
	float xp, yp;  //grid point
	float cellX = (XMAX - XMIN) / NUMDIV;  //cell width
	float cellY = (YMAX - YMIN) / NUMDIV;  //cell height
	glm::vec3 eye(0., 0., 0.);

	glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glBegin(GL_QUADS);  //Each cell is a tiny quad.

	for (int i = 0; i < NUMDIV; i++)	//Scan every cell of the image plane
	{
		xp = XMIN + i * cellX;
		for (int j = 0; j < NUMDIV; j++)
		{
			yp = YMIN + j * cellY;

			glm::vec3 dir(xp + 0.5 * cellX, yp + 0.5 * cellY, -EDIST);	//direction of the primary ray

			Ray ray = Ray(eye, dir);

			glm::vec3 col = trace(ray, 1); //Trace the primary ray and get the colour value
			glColor3f(col.r, col.g, col.b);
			glVertex2f(xp, yp);				//Draw each cell with its color value
			glVertex2f(xp + cellX, yp);
			glVertex2f(xp + cellX, yp + cellY);
			glVertex2f(xp, yp + cellY);
		}
	}

    glEnd();
    glFlush();
}

void makeBox(float x, float y, float z, float length, float depth, float height, glm::vec3 colour) {
	// create a vector coordinate for each corner of the makeBox
	// if length width and height are the same then the box is a cube
	// Length is x-axis depth is z-axis height is y-axis
	float l = length;
	float d = depth;
	float h = height;
	glm::vec3 cornerA = glm::vec3(x,y,z);
	glm::vec3 cornerB = glm::vec3(x,y,z+d);
	glm::vec3 cornerC = glm::vec3(x,y+h,z+d);
	glm::vec3 cornerD = glm::vec3(x,y+h,z);
	glm::vec3 cornerE = glm::vec3(x+l,y+h,z);
	glm::vec3 cornerF = glm::vec3(x+l,y,z);
	glm::vec3 cornerG = glm::vec3(x+l,y,z+d);
	glm::vec3 cornerH = glm::vec3(x+l,y+h,z+d);


	Plane *planeFront = new Plane (cornerC, cornerB, cornerG, cornerH);
	Plane *planeRight = new Plane (cornerG, cornerF, cornerE, cornerH);
	Plane *planeBack = new Plane (cornerE, cornerF, cornerA, cornerD);
	Plane *planeLeft = new Plane (cornerA, cornerB, cornerC, cornerD);
	Plane *planeTop = new Plane (cornerD, cornerC, cornerE, cornerH);
	Plane *planeBottom = new Plane (cornerB, cornerA, cornerF, cornerG);

	planeFront->setColor(colour);
	planeRight->setColor(colour);
	planeBack->setColor(colour);
	planeLeft->setColor(colour);
	planeTop->setColor(colour);
	planeBottom->setColor(colour);

	sceneObjects.push_back(planeFront);
	sceneObjects.push_back(planeRight);
	sceneObjects.push_back(planeBack);
	sceneObjects.push_back(planeLeft);
	sceneObjects.push_back(planeTop);
	sceneObjects.push_back(planeBottom);

}

//---This function initializes the scene -------------------------------------------
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL 2D orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);

    glClearColor(0, 0, 0, 1);


	texture = TextureBMP("Earth.bmp");

//Reflective Sphere
	Sphere *sphere1 = new Sphere(glm::vec3(-5.0, 2.5, -90.0), 10.0);
	sphere1->setColor(glm::vec3(0, 0, 1));   //Set colour to blue
	sphere1->setReflectivity(true, 0.8);
	sceneObjects.push_back(sphere1);		 //0
	//Textured Sphere
	Sphere *sphere2 = new Sphere(glm::vec3(10.0, 10.0, -60.0), 3.0);
	sphere2->setColor(glm::vec3(0, 1, 0.5));
	sceneObjects.push_back(sphere2); //1
	//Refractive Sphere
	Sphere *sphere3 = new Sphere(glm::vec3(10.0, -5.0, -65.0), 5.0);
	sphere3->setColor(glm::vec3(0.5, 0.95, 1));
	sphere3->setRefractivity(true, 1.02, 1.02);
	sceneObjects.push_back(sphere3); //2

	Plane *floorPlane = new Plane (glm::vec3(-20., -15, -40), //Point A
 								glm::vec3(20., -15, -40), //Point B
 								glm::vec3(20., -15, -200), //Point C
 								glm::vec3(-20., -15, -200)); //Point D
	floorPlane->setColor(glm::vec3(0.8, 0.8, 0));
	sceneObjects.push_back(floorPlane); //3
	// Transparent Sphere
	Sphere *sphere4 = new Sphere(glm::vec3(0.0, -12.5, -60.0), 2.5);
	sphere4->setColor(glm::vec3(0.9, 1, 0.3));
	sphere4->setTransparency(true, 0.8);
	sceneObjects.push_back(sphere4); //4

	Plane *rearPlane = new Plane (glm::vec3(-20., -15, -120), //Point A
 								glm::vec3(20., -15, -120), //Point B
 								glm::vec3(20., 15, -120), //Point C
 								glm::vec3(-20., 15, -120)); //Point D
	rearPlane->setColor(glm::vec3(0.8, 0.8, 0));
	sceneObjects.push_back(rearPlane); // 5
	//Hollow Cylinder under large sphere
	Cylinder *cylinder1 = new Cylinder(glm::vec3(-5.0, -15, -90), 5, 5);
	cylinder1->setColor(glm::vec3(0.8, 0.4, 0));
	sceneObjects.push_back(cylinder1); //6

	//Cone

	Cone *cone1 = new Cone(glm::vec3(0.0, -15., -75), 2, 5);
	cone1->setColor(glm::vec3(0, 0.4, 1));
	sceneObjects.push_back(cone1); //7


	makeBox(7.5, -15, -70, 5, 5, 5, glm::vec3(1., 0., 0.));

}


int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Raytracing");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}
