#ifndef BVHNODE_
#define BVHNODE_
#include "shape.h"
#include <iostream>
#include <algorithm>
/* Lines 7-47 based on code by Peter Shirley from "Ray Tracing in One Weekend"*/
class bvh_node : public shape{
  public:
    bvh_node(){}
    bvh_node(shape** l, int n);
    //bvh_node(string file);

    virtual bool is_hit(const ray& r, float tmin, float tmax, hit_record& rec) const{
      if(box.is_hit(r, tmin, tmax)){
        hit_record left_rec;
        hit_record right_rec;
        bool hit_left = left->is_hit(r, tmin, tmax, left_rec);
        bool hit_right = right->is_hit(r, tmin, tmax, right_rec);
        if(hit_left && hit_right){
          if(left_rec.t < right_rec.t){
            rec = left_rec;
          }
          else rec = right_rec;
          return true;
        }
        else if(hit_left){
          rec = left_rec;
          return true;
        }
        else if(hit_right){
          rec = right_rec;
          return true;
        }
        else return false;
      }
      else return false;
    }


    virtual bool bounding_box(bvh& b) const{
      b = box;
      return true;
    }

    shape* left;
    shape* right;
    bvh box;
};

int partition(shape** l, int left, int right, int pivotIndex, int axis){
  shape* pivotValue = l[pivotIndex];
  std::swap(l[pivotIndex], l[right]);
  int storeIndex = left;
  for(int i = left; i < right; i++){
    if(l[i]->center[axis] < pivotValue->center[axis]){
      std::swap(l[storeIndex], l[i]);
      storeIndex ++;
    }
  }
  std::swap(l[right], l[storeIndex]);
  return storeIndex;
}

void select(shape** l, int left, int right, int k, int axis){
  if(left >= right)return;
  int pivotIndex = (left + right)/2;
  pivotIndex = partition(l, left, right, pivotIndex, axis);
  if(k = pivotIndex)return;
  else if(k < pivotIndex)return select(l, left, pivotIndex - 1, k, axis);
  else return select(l, pivotIndex + 1, right, k, axis);
}

bvh_node::bvh_node(shape** l, int n){
  select(l, 0, n, n/2, 0);
  if(n == 0)return;
  else if(n == 1){
    left = l[0];
    right = left;
  }
  else if(n == 2){
    left = l[0];
    right = l[1];
  }
  else{
    left = new bvh_node(l, n/2);
    right = new bvh_node(l + n/2, n - n/2);
  }
  bvh leftbox;
  bvh rightbox;
  left->bounding_box(leftbox);
  right->bounding_box(rightbox);
  box = surrounding_box(leftbox, rightbox);
}

#endif
