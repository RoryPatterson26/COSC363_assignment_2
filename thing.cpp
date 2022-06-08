float texcoords = (ray.hit.x - (-15))/(5-(-15));
float texcoordt = (ray.hit.z - (-60))/((-90)-(-60));
if(texcoords > 0 && texcoords < 1 && texcoordt > 0 && texcoordt < 1) {
    color=texture.getColorAt(texcoords, texcoordt);
    obj->setColor(color);
