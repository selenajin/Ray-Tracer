#ifndef SHAPE_H
#define SHAPE_H
#include "ray.h"

struct hit_record{
  float t;
  vec3 p;
  vec3 normal;
}

class shape{
  public:
    virtual bool is_hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif
