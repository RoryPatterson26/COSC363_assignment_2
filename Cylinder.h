/*
COSC363 Ray Tracer
Cylinder class Subclass of SceneObject
*/

#ifndef H_CYLINDER
#define H_CYLINDER

#include <glm/glm.hpp>
#include "SceneObject.h"


//Cylinder class

class Cylinder : public SceneObject
{
private:
    glm::vec3 center;
    float radius;
    float height;

public:
    Cylinder() {};

    Cylinder(glm::vec3 c, float r, float h) : center(c), radius(r), height(h) {}

    float intersect(glm::vec3 p0, glm::vec3 dir);

    glm::vec3 normal(glm::vec3 p);

};

#endif
