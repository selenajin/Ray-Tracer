#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "shape.h"

class triangle:public shape{
  public:
    vec3 vertex1;
    vec3 vertex2;
    vec3 vertex3;
    vec3 center;
    int type;
    triangle(){};
    triangle(vec3 vertexone, vec3 vertextwo, vec3 vertexthree, int type_){
      vertex1 = vertexone;
      vertex2 = vertextwo;
      vertex3 = vertexthree;
      center = (vertexone + vertextwo + vertex3)/3;
      type = type_;
    }

    bool sign(int input) const{
      if (input > -1)return true;
      else return false;
    }
    //Using Barycentric Coordinates to find intersection of plane triangle and Ray
    virtual bool is_hit(const ray& r, float tmin, float tmax, hit_record& rec) const{
      vec3 edge1 = vertex2-vertex1;
      vec3 edge0 = vertex3-vertex1;
      vec3 norm = unit_vector(cross(edge1, edge0));
      //Returns false if plane is parallel to ray
      //if(float(dot(r.B, norm)) < 0.01)return false;
      float t = (dot(norm, vertex2 - r.A))/dot(norm, r.B);
      vec3 pos = r.A + t*r.B;
      //Checking if ray intersects within triangle bounds by checking each triangle edge
      bool signA = sign(dot(cross((vertex2- vertex1),(pos- vertex1)),norm));
      bool signB = sign(dot(cross((vertex3- vertex2),(pos- vertex2)),norm));
      bool signC = sign(dot(cross((vertex1- vertex3),(pos- vertex3)),norm));
      //Returns hit_record values
      rec.t = t;
      rec.type = type;
      //rec.p = r.point_at_parameter(rec.t);
      rec.p = r.point_at_parameter(t);
      //rec.p = norm;
      rec.normal = norm;
      return ((signA == signB) && (signA == signC) && t>=tmin &&t<=tmax);
      return false;

    }

    virtual bool bounding_box(bvh& box) const{
      //Finding max and min values for the axis-aligned bounding box
      float xmin = fmin(fmin(vertex1.x(), vertex2.x()), vertex3.x());
      float ymin = fmin(fmin(vertex1.y(), vertex2.y()), vertex3.y());
      float zmin = fmin(fmin(vertex1.z(), vertex2.z()), vertex3.z());
      float xmax = fmax(fmax(vertex1.x(), vertex2.x()), vertex3.x());
      float ymax = fmax(fmax(vertex1.y(), vertex2.y()), vertex3.y());
      float zmax = fmax(fmax(vertex1.z(), vertex2.z()), vertex3.z());

      vec3 min(xmin, ymin, zmin);
      vec3 max(xmax, ymax, zmax);
      box = bvh(min, max);
      return true;
    }
};

#endif
