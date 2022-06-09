/* Cylinder Object extends sceneobject
*  creates a cylinder in the scene
*/

#include <math.h>
#include "Cylinder.h"

float Cylinder::intersect(glm::vec3 p0, glm::vec3 dir) {
    //Using quadratic formula to find intersect points with ray
    glm::vec3 d = p0 - center;
    float a = powf(dir.x, 2) + powf(dir.z, 2);
    float b = 2 * (dir.x * d.x + dir.z * d.z);
    float c = powf(d.x, 2) + powf(d.z, 2) - powf(radius, 2);
    float delta = powf(b, 2) - 4*(a * c);

    if(delta < 0.001) return -1.0;

    float t1 = (-b - sqrt(delta))/(2*a);
    float t2 = (-b + sqrt(delta))/(2*a);

    if (t1 < 0.01) t1 = -1;
    if (t2 < 0.01) t2 = -1;

    if (t1 < t2) {
        float temp = t1;
        t1 = t2;
        t2 = temp;
    }

    float ypos = p0.y + dir.y * t2;
    //Checks to see if the ray is within the height of the cylinder so it isn't infinitely tall
    if ((ypos >= center.y) && (ypos <= center.y + height)) {
        return t2;
    } else {
        ypos = p0.y + dir.y * t1;
        if ((ypos >= center.y) && (ypos <= center.y + height)) {
            return t1;
        } else {
            return -1.0;
        }
    }
}


glm::vec3 Cylinder::normal(glm::vec3 p) {
    glm::vec3 d = p - center;
    glm::vec3 n = glm::vec3 (d.x, 0, d.z);
    n = glm::normalize(n);
    return n;
}
