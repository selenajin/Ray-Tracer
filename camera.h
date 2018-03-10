#ifndef CAMERA_H
#define CAMERA_H
#include "ray.h"
/* lines 5-20 based on code by Peter Shirley from "Ray Tracing in One Weekend"*/
class camera{
  public:
    camera(vec3 o, vec3 l, vec3 h, vec3 v){
      origin = o;
      lower_left_corner = l;
      horizontal = h;
      vertical = v;
    }
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    //returns ray in orthographic projection
    ray getRay(float x, float y){
      return ray(origin, lower_left_corner + x*horizontal + y*vertical - origin);
    }
    //returns ray in perspective projection
    ray getRayP(vec3 eyepoint,float x, float y){
      vec3 o = eyepoint;
      vec3 p(x, y, 0);
      return ray(eyepoint, lower_left_corner + x*horizontal + y*vertical - eyepoint);
    }

};

#endif
