#ifndef SPHERE_H
#define SPHERE_H
#include "shape.h"

class sphere:public shape{
public:
  vec3 center;
  float radius;
  sphere(){};
  sphere(vec3& center_, float radius_){
    center = center_;
    radius = radius_;
  }
  
  virtual bool is_hit(const ray& r, float t_min, float t_max, hit_record& rec){
    vec3 oc = r.A - center;
    //y(x) = a*x^2 + b*x + c
    //y(x) = a*t^2 + b*t + c
    //y(x) = (B^2)*x^2 + (2*dot(B, A-C))*x + (dot(A-C, A-C)-R*R)
    float a = dot(r.B, r.B);
    float b = 2.0*dot(oc, r.B);
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - 4*a*c;
    return discriminant > 0;
  }

  float reflect(const ray&r){
    vec3 oc = r.A - center;
    //y(x) = a*x^2 + b*x + c
    //y(x) = a*t^2 + b*t + c
    //y(x) = (B^2)*x^2 + (2*dot(B, A-C))*x + (dot(A-C, A-C)-R*R)
    float a = dot(r.B, r.B);
    float b = 2.0*dot(oc, r.B);
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - 4*a*c;
    if(discriminant < 0){
      return -1;
    }
    else{
      return (-b - sqrt(discriminant))/(2*a);
    }
  }
};

#endif
