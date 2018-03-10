#ifndef SHAPELIST_H
#define SHAPELIST_H
#include "shape.h"
/* All code by Peter Shirley from "Ray Tracing in One Weekend"*/
class shapelist: public shape{
  public:
    shape** list;
    int size;
    shapelist(){}
    shapelist(shape** objs, int n){
      list = objs;
      size = n;
    }
    virtual bool is_hit(const ray&r, float tmin, float tmax, hit_record& rec) const{
      hit_record temp_rec;
      bool hit_anything = false;
      double shortest_distance = tmax;
      for(int i = 0; i < size; i++){
        if(list[i]->is_hit(r, tmin, shortest_distance, temp_rec)){
          hit_anything = true;
          shortest_distance = temp_rec.t;
          rec = temp_rec;
          rec.type = list[i]->type;
        }
      }
      return hit_anything;
    }
};
#endif
