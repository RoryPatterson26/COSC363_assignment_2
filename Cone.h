//Defines the cone class for scene

#ifndef H_CONE
#define H_CONE

#include <glm/glm.hpp>
#include "SceneObject.h"


//Cylinder class

class Cone : public SceneObject
{
private:
    glm::vec3 center;
    float radius;
    float height;

public:
    Cone() {};

    Cone(glm::vec3 c, float r, float h) : center(c), radius(r), height(h) {}

    float intersect(glm::vec3 p0, glm::vec3 dir);

    glm::vec3 normal(glm::vec3 p);

};

#endif
