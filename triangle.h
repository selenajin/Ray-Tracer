#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "shape.h"

class triangle:public shape{
  public:
    vec3 vertex1;
    vec3 vertex2;
    vec3 vertex3;
    int type;
    triangle(){};
    triangle(vec3 vertexone, vec3 vertextwo, vec3 vertexthree, int type_){
      vertex1 = vertexone;
      vertex2 = vertextwo;
      vertex3 = vertexthree;
      type = type_;
    }
    //Using Barycentric Coordinates to find intersection of plane triangle and Ray
    virtual bool is_hit(const ray& r, float tmin, float tmax, hit_record& rec) const{
      vec3 norm = cross(vertex1 - vertex3, vertex2 - vertex3);
      //Returns false if plane is parallel to ray
      if(float(dot(r.B, norm)) < 0.01)return false;
      float t = (dot(norm, r.A) + dot(norm, vertex3))/dot(norm, r.B);
      //Returns false if plane is behind ray
      if(t<0)return false;
      vec3 pos = r.A + t*r.B;
      //Checking if ray intersects within triangle bounds by checking each triangle edge
      vec3 C = cross(vertex1-vertex3, pos - vertex3);
      if(dot(norm, C) < 0)return false;
      C = cross(vertex2-vertex1, pos - vertex1);
      if(dot(norm, C) < 0)return false;
      C = cross(vertex3-vertex2, pos - vertex2);
      if(dot(norm, C) < 0)return false;
      //Returns hit_record values
      rec.t = t;
      rec.p = r.point_at_parameter(rec.t);
      rec.normal = unit_vector(norm);
      if(t < tmax && t > tmin)return true;
      return false;
    }
};

#endif
