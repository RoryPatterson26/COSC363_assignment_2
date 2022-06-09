//Methods for Cone subclass

#include "Cone.h"
#include <math.h>


float Cone::intersect(glm::vec3 p0, glm::vec3 dir) {
    glm::vec3 d = p0 - center;
    float yd = height - p0.y + center.y;
    float tanPiece = powf((radius / height), 2);

    float a = powf(dir.x, 2) + powf(dir.z, 2) - (tanPiece * powf(dir.y, 2));
    float b = 2*(d.x * dir.x + d.z * dir.z + tanPiece * yd * dir.y);
    float c = powf(d.x, 2) + powf(d.z, 2) - (tanPiece * powf(yd, 2));
    float delta = powf(b, 2) - 4*(a * c);

	if(delta < 0.001) return -1.0;

    float t1 = (-b - sqrt(delta)) / (2*a);
    float t2 = (-b + sqrt(delta)) / (2*a);

    if(t1 < 0.01) t1 = -1;
	if(t2 < 0.01) t2 = -1;

	if (t1 < t2){
		float temp = t1;
        t1 = t2;
        t2 = temp;
    }

    float ypos = p0.y + dir.y * t2;
    if ((ypos >= center.y) && (ypos <= center.y + height)){
		return t2;
	} else {
       float ypos = p0.y + dir.y * t1;
       if ((ypos >= center.y) && (ypos <= center.y + height)){
			return t1;
       } else {
			return -1.0;
       }
    }


}

glm::vec3 Cone::normal(glm::vec3 p) {
    glm::vec3 d = p - center;
    float r = sqrt(powf(d.x, 2) + powf(d.z, 2));
    glm::vec3 normal = glm::vec3(d.x, r * (radius / height), d.z);
    return glm::normalize(normal);
}
