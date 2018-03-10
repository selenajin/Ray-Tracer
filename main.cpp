#include <iostream>
#include <fstream>
#include "ray.h"
#include "sphere.h"
#include "shapelist.h"
#include "camera.h"
#include "triangle.h"
using namespace std;

//Used to fill the background with a gradient
vec3 interpolate(float t, vec3& start, vec3& end){
  return (1.0 - t)*start + t*end;
}

//Used to fill rays reflected off of objects
vec3 fill(const ray& r, shapelist* world){
  hit_record rec;
  if(world->is_hit(r, 0.0, 1000000000.0, rec)){
    ray light(vec3(-4,0,-1),vec3(4,0,-1));
    vec3 lm = unit_vector(unit_vector(rec.p)-unit_vector(light.A));
    vec3 ambient = (0.8*vec3(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1));
    //vec3 ambient(1,0.5,0.5);
    //The shading can be seen a little easier if the ambient vector is switched out for a flat color!
    vec3 diffuse = 0.3*(dot(lm, unit_vector(rec.normal))*unit_vector(vec3(1,1,1)));
    vec3 specular = 5*(dot(vec3(0,1,0), -light.B))*(dot(vec3(0,1,0), -light.B))*vec3(1,1,1);
    vec3 result = ambient + diffuse + specular;
    //Clamping values to fit in [0,1.0]
    float max = result.x();
    if(result.y()>max)max = result.y();
    if(result.z()>max)max = result.z();
    result /= max;
    return result;
  }
  else{
    //Using interpolate() to fill background
    vec3 unit = unit_vector(r.direction());
    float t = (unit.y() + 1)/2;
    vec3 start = vec3(0.96484375, 0.7890625, 0.78515625);
    vec3 end = vec3(0.56640625, 0.65625, 0.81640625);
    return interpolate(t , start, end);
  }
}
int main(){
  //Picture sectup
  int width = 400;
  int height = 200;
  int hres = 1;
  int vres = 1;
  int s = 100;

  //Compiling and outputting an orthographic perspective of the scene
  ofstream myfile;
  myfile.open("output.ppm");
  myfile<<"P3\n"<<width<<" "<<height<<"\n255\n";
  vec3 lower_left_corner(-4, -2, -1.0);
  vec3 horizontal(8, 0.0, 0.0);
  vec3 vertical(0.0, 4, 0.0);
  vec3 origin(0.0, 0.0, 0.0);
  camera cam(origin, lower_left_corner, horizontal, vertical);

  //Setting up objects
  vec3 center1 = vec3(-0.5, 0,-1);
  vec3 center2 = vec3(0.3,0,-1);
  vec3 pt1 = vec3(0.5, 0.7,-0.2);
  vec3 pt2 = vec3(-0.9, -0.8,-0.5);
  vec3 pt3 = vec3(-0.5, 0.7,-1);
  shape* list[3];
  list[0] = new sphere(center1, (float)0.6, 0);
  list[1] = new triangle(pt1, pt2, pt3, 1);
  list[2] = new sphere(center2, (float)0.5, 0);
  shapelist* world= new shapelist(list, 3);

  //Filling image pixel by pixel
  for(int i = height - 1; i >= 0; i--){
  for(int k = 0; k < width; k++){
      //using antialiasing to reduce uneven, jagged edges
      vec3 pixel(0,0,0);
      for(int n = 0; n < s; n++){
        for(int r = -hres; r < hres; r++){
          for(int c = -vres; c < vres; c++){
            float x = float(k + (c-hres/2 + (rand()%1000)/1000.0))/float(width);
            float y = float(i + (r*(c-vres/2) + (rand()%1000)/1000.0))/float(height);
            ray r = cam.getRay(x,y);
            pixel += fill(r, world);
          }
        }
      }
      //converting values to PPM format
      //Lines 90-94 by Peter Shirley from "Ray Tracing in One Weekend"
      pixel = pixel/(hres*2*vres*2*s);
      int ir = int(255.99*pixel[0]);
      int ig = int(255.99*pixel[1]);
      int ib = int(255.99*pixel[2]);
      myfile<<ir<<" "<<ig<<" "<<ib<<"\n";
    }
  }
  myfile.close();

  //Compiling and outputting a second image, in perspective view
  //Most of the code is the same from the first, with the exception of using a different camera ray method
  ofstream secondfile;
  secondfile.open("output-perspective.ppm");
  secondfile<<"P3\n"<<width<<" "<<height<<"\n255\n";
  for(int i = height - 1; i >= 0; i--){
  for(int k = 0; k < width; k++){

      vec3 pixel(0,0,0);
      for(int n = 0; n < s; n++){
        for(int r = -hres; r < hres; r++){
          for(int c = -vres; c < vres; c++){
            float x = float(k + (c-hres/2 + (rand()%1000)/1000.0))/float(width);
            float y = float(i + (r*(c-vres/2) + (rand()%1000)/1000.0))/float(height);
            ray r = cam.getRayP(vec3(0.5,0,0),x,y);
            pixel += fill(r, world);
          }
        }
      }

      pixel = pixel/(hres*2*vres*2*s);
      int ir = int(255.99*pixel[0]);
      int ig = int(255.99*pixel[1]);
      int ib = int(255.99*pixel[2]);
      secondfile<<ir<<" "<<ig<<" "<<ib<<"\n";
    }
  }
  secondfile.close();
}
