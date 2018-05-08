#ifndef SHAPE_H
#define SHAPE_H

#include "bvh.h"
/* All code by Peter Shirley from "Ray Tracing in One Weekend"*/
class material;

struct hit_record{
  float t;
  int type;
  //0 = metal, 1 = diffuse, 2 = experimental
  //Material type hasn't been implemented yet
  vec3 p;
  vec3 normal;
};

class shape{
  public:
    vec3 center;
    virtual bool is_hit(const ray& r, float tmin, float tmax, hit_record& rec) const = 0;
    virtual bool bounding_box(bvh& box) const = 0;
};

#endif
