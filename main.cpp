#include <iostream>
#include <fstream>
#include "ray.h"
#include "sphere.h"

vec3 interpolate(float t, vec3& start, vec3& end){
  return (1.0 - t)*start + t*end;
}

vec3 fill(const ray& r){
  vec3 center(0, 0, -1.0);
  float radius = 0.5;
  float diameter = radius*2;
  sphere test(center, radius);
  if(test.reflect(r) > 0){
    vec3 reflection = unit_vector(r.point_at_parameter(test.reflect(r)) - center);
    return (0.5)*(reflection + vec3(1,1,1));
  }
  else{
    float t = ((r.direction().y())/3 + 1)/2;
    vec3 start = vec3(0.96484375, 0.7890625, 0.78515625);
    vec3 end = vec3(0.56640625, 0.65625, 0.81640625);
    return interpolate(t , start, end);
  }
}

using namespace std;
int main(){
  int width = 600;
  int height = 300;
  ofstream myfile;
  myfile.open("output.ppm");
  myfile<<"P3\n"<<width<<" "<<height<<"\n255\n";
  vec3 lower_left_corner(-6, -3, -1.0);
  vec3 horizontal(12, 0.0, 0.0);
  vec3 vertical(0.0, 6, 0.0);
  vec3 origin(0.0, 0.0, 0.0);

  for(int i = height - 1; i >= 0; i--){
    for(int j = 0; j < width; j++){
      float x = float(j)/float(width);
      float y = float(i)/float(height);
      ray r(origin, lower_left_corner + x*horizontal + y*vertical);
      vec3 pixel = fill(r);
      int ir = int(255.99*pixel[0]);
      int ig = int(255.99*pixel[1]);
      int ib = int(255.99*pixel[2]);
      myfile<<ir<<" "<<ig<<" "<<ib<<"\n";
    }
  }
  myfile.close();
}
