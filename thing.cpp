float texcoords = (ray.hit.x - (-15))/(5-(-15));
float texcoordt = (ray.hit.z - (-60))/((-90)-(-60));
if(texcoords > 0 && texcoords < 1 && texcoordt > 0 && texcoordt < 1) {
    color=texture.getColorAt(texcoords, texcoordt);
    obj->setColor(color);



    glm::vec3 d = p0 - center;
    float yd = height - p0.y + center.y;
    float rh = radius / height;
    float tan = powf(rh, 2);
    // Implements equation given in lecture notes
    float a = powf(dir.x, 2) + powf(dir.z, 2) - (tan * powf(dir.y, 2));
    float b = 2*(d.x * dir.x + d.z * dir.z + tan * yd * dir.y);
    float c = powf(d.x, 2) + powf(d.z, 2) - (tan * powf(yd, 2));
    float delta = powf(b, 2) - 4 * (a * c);


    // Same as in cylinder
    if(delta < 0.001) return -1.0;

    float t1 = (-b - sqrt(delta)/(2*a));
    float t2 = (-b + sqrt(delta)/(2*a));

    if (t1 < 0.01) t1 = -1;
    if (t2 < 0.01) t2 = -1;

    if (t1 < t2) {
        float temp = t1;
        t1 = t2;
        t2 = temp;
    }

    float ypos = p0.y + dir.y * t2;
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
// Potential floor colour chooser
    if (k == 0) {
        if (l == 0) {
            color = glm::vec3(0, 1, 0);
        } else if (l == 1) {
            color = glm::vec3(1, 1, 0.5);
        }

    } else {
        if (l == 0) {
            color = glm::vec3(1, 1, 0.5);
        } else {
            color = glm::vec3(0, 1, 0);
        }
    }
