#ifndef CAMERA_H
#define CAMERA_H
#include "ray.h"
/* lines 5-38 based on code by Peter Shirley from "Ray Tracing in One Weekend"*/
vec3 random_in_unit_disk() {
    vec3 p;
    do {
        p = 2.0*vec3(drand48(),drand48(),0) - vec3(1,1,0);
    } while (dot(p,p) >= 1.0);
    return p;
}
class camera{
  public:
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect) { // vfov is top to bottom in degrees
        //lens_radius = aperture / 2;
        float theta = vfov*M_PI/180;
        float half_height = tan(theta/2);
        float half_width = aspect * half_height;
        origin = lookfrom;
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);
        lower_left_corner = origin  - half_width*u -half_height*v - w;
        horizontal = 2*half_width*u;
        vertical = 2*half_height*v;
    }
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    //float lens_radius;
    //returns ray in orthographic projection
    ray getRay(float s, float t) {
        //vec3 rd = lens_radius*random_in_unit_disk();
        //vec3 offset = u * rd.x() + v * rd.y();
        return ray(origin, lower_left_corner + s*horizontal + t*vertical - origin);
    }
    //returns ray in perspective projection
    ray getRayP(vec3 eyepoint,float x, float y){
      vec3 o = eyepoint;
      vec3 p(x, y, 0);
      return ray(eyepoint, lower_left_corner + x*horizontal + y*vertical - eyepoint);
    }

};

#endif
