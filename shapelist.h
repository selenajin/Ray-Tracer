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
      double closest_so_far = tmax;
      for (int i = 0; i < size; i++) {
          if (list[i]->is_hit(r, tmin, closest_so_far, temp_rec)) {
              hit_anything = true;
              closest_so_far = temp_rec.t;
              rec = temp_rec;
          }
      }
      return hit_anything;
    }


    virtual bool bounding_box(bvh& box) const{
      if(size < 1)return false;
      bvh temp_box;
      bool first_true = list[0]->bounding_box(temp_box);
      if(!first_true)return false;
      else box = temp_box;
      for(int i = 0; i < size; i++){
        if(list[0]->bounding_box(temp_box)){
          box = surrounding_box(box, temp_box);
        }
        else return false;
      }
      return true;
    }
};

#endif
