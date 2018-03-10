#ifndef SPHERE_H
#define SPHERE_H
#include "shape.h"
/* All code based on code by Peter Shirley from "Ray Tracing in One Weekend"*/
class sphere:public shape{
  public:
    vec3 center;
    float radius;
    int type;
    sphere(){};
    sphere(vec3 center_, float radius_, int type_){
      center = center_;
      radius = radius_;
      type = type_;
    }

    virtual bool is_hit(const ray& r, float tmin, float tmax, hit_record& rec) const{
      vec3 oc = r.A - center;
      //y(x) = a*x^2 + b*x + c
      //y(x) = a*t^2 + b*t + c
      //y(x) = (B^2)*x^2 + (2*dot(B, A-C))*x + (dot(A-C, A-C)-R*R)
      float a = dot(r.B, r.B);
      float b = 2.0*dot(oc, r.B);
      float c = dot(oc, oc) - radius*radius;
      float discriminant = b*b - 4*a*c;
      if(discriminant > 0){
        float temp = (-b - sqrt(b*b-4*a*c))/(2*a);
        if(temp < tmax && temp > tmin){
          rec.t = temp;
          rec.p = r.point_at_parameter(rec.t);
          rec.normal = (rec.p - center)/radius;
          return true;
        }
        temp = (-b + sqrt(b*b-4*a*c))/(2*a);
        if(temp < tmax && temp > tmin){
          rec.t = temp;
          rec.p = r.point_at_parameter(rec.t);
          rec.normal = (rec.p - center)/radius;
          return true;
        }
      }
      return false;
    }
};

#endif
