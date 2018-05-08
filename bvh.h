#ifndef BVH_H
#define BVH_H
#include "ray.h"
#include "shape.h"

/* All code based on code by Peter Shirley from "Ray Tracing in One Weekend"*/

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

class bvh{
  public:
    bvh(){}
    bvh(const vec3& a, const vec3& b){
      min = a;
      max = b;
    }

    vec3 min;
    vec3 max;

    bool is_hit(const ray& r, float tmin, float tmax) const{
      for (int a = 0; a < 3; a++) {
          float t0 = ffmin((min[a] - r.origin()[a]) / r.direction()[a],
                          (max[a] - r.origin()[a]) / r.direction()[a]);
          float t1 = ffmax((min[a] - r.origin()[a]) / r.direction()[a],
                          (max[a] - r.origin()[a]) / r.direction()[a]);
          tmin = ffmax(t0, tmin);
          tmax = ffmin(t1, tmax);
          if (tmax <= tmin)
              return false;
      }
      return true;
    }
};

bvh surrounding_box(bvh box0, bvh box1){
  vec3 small(fmin(box0.min.x(), box1.min.x()), fmin(box0.min.y(), box1.min.y()), fmin(box0.min.z(), box1.min.z()));
  vec3 big(fmax(box0.max.x(), box1.max.x()), fmax(box0.max.y(), box1.max.y()), fmax(box0.max.z(), box1.max.z()));
  return bvh(small, big);
}

#endif
