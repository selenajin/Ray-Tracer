#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <bits/stdc++.h>
#include "ray.h"
#include "sphere.h"
#include "shapelist.h"
#include "camera.h"
#include "triangle.h"
#include "bvhnode.h"


using namespace std;

//Used to fill the background with a gradient
vec3 interpolate(long double t, vec3& start, vec3& end){
  return (1.0 - t)*start + t*end;
}
//Lines 21-30 based on code by Peter Shirley from "Ray Tracing in One Weekend
vec3 reflect(const vec3&v, const vec3&n){
  return v-2*dot(v,n)*n;
}

bool scatter(const ray& r_in, const hit_record& rec, ray& scattered){
  vec3 reflected = reflect(r_in.B, unit_vector(rec.normal));
  scattered = ray(rec.p, reflected);
  return dot(scattered.direction(), rec.normal) > 0;
}

//Used to fill rays reflected off of objects
vec3 fill(const ray& r, shape* world, int depth){
  hit_record rec;
  if(world->is_hit(r, 0.0, 100000000000000.0, rec)){
    if(rec.type == 0){
      ray light(vec3(-4,0,-1),vec3(4,0,-1));
      vec3 lm = unit_vector(unit_vector(rec.p)-unit_vector(light.A));
      //vec3 ambient = (0.8*vec3(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1));
      vec3 ambient(0.8,0.3,0.3);
      //The shading can be seen a little easier if the ambient vector is switched out for a flat color!
      vec3 diffuse = 0.7*(dot(lm, unit_vector(rec.normal))*unit_vector(vec3(1,1,1)));
      vec3 specular = 10*(dot(vec3(0,1,0), -light.B))*(dot(vec3(0,1,0), -light.B))*vec3(1,1,1);
      vec3 result = ambient + diffuse + specular;
      //Clamping values to fit in [0,1.0]

      for(int i = 0; i < 3; i++){
        result[i] = abs(result[i]);
      }
      long double max = result.x();
      if(result.y()>max)max = result.y();
      if(result.z()>max)max = result.z();
      result = result/max;
      return result;
    }
    else{
      ray scattered;
      if(depth < 500 && scatter(r, rec, scattered)){
        return vec3(0.9,0.9,0.9)*fill(scattered, world, depth + 1);
      }
      else{
        vec3 unit = unit_vector(r.direction());
        long double t = (unit.y() + 1)/2;
        vec3 start = vec3(0.96484375, 0.7890625, 0.78515625);
        vec3 end = vec3(0.56640625, 0.65625, 0.81640625);
        return interpolate(t, end, start);
      }
    }
  }
  else{
    //return vec3(0,0,0);

    //Using interpolate() to fill background
    vec3 unit = unit_vector(r.direction());
    long double t = (unit.y() + 1)/2;
    vec3 start = vec3(0.96484375, 0.7890625, 0.78515625);
    vec3 end = vec3(0.56640625, 0.65625, 0.81640625);
    return interpolate(t, end, start);
  }
}

vector<string> split(const string& s){
   vector<string> tokens;
   string token;
   istringstream tokenStream(s);
   while(getline(tokenStream, token, ' '))
   {
      tokens.push_back(token);
   }
   return tokens;
}

bvh_node* objParser(string file){
  vector<vec3> points;
  shape** list = new shape*[1000000000];
  int count = 0;

  string line;
  ifstream myfile;
  myfile.open(file.c_str());
  //myfile.open(file.c_str());
  if(myfile.is_open()){
    while(!myfile.eof()){
      getline(myfile, line);
      if(line.size() == 0)continue;
      else if(line[0] == 'v'){
        vector<string> lines = split(line.substr(2));
        char i[9];
        char j[9];
        char k[9];
        strcpy(i, lines[0].c_str());
        strcpy(j, lines[1].c_str());
        strcpy(k, lines[2].c_str());
        long double x = atof(i);
        long double y = atof(j);
        long double z = atof(k);
        vec3 temp = vec3(x*1000, y*1000, z*1000);
        //cout<<x*500<<' '<<y*500<<' '<<z*500<<endl;
        points.push_back(temp);
      }
      else if(line[0] == 'f'){
        vector<string> lines = split(line.substr(2));
        char i[9];
        char j[9];
        char k[9];
        strcpy(i, lines[0].c_str());
        strcpy(j, lines[1].c_str());
        strcpy(k, lines[2].c_str());
        int x = atoi(i);
        int y = atoi(j);
        int z = atoi(k);
        list[count++] = new triangle(points[x-1], points[y-1], points[z-1], 0);
      }
    }
  }
  //list[count++] = new sphere(vec3(-1000,-1500,-1000), 1000.0, 1);
  if(count != 0){
    bvh_node* world = new bvh_node(list, count-1);
    //shapelist* world= new shapelist(list, count-1);
    return world;
  }else return NULL;
}

int main(){
  //Picture sectup
  int width = 600;
  int height = 400;
  int hres = 1;
  int vres = 1;
  int s = 100;

  //Compiling and outputting an orthographic perspective of the scene
  //ofstream myfile;
  //myfile.open("output.ppm");
  //myfile<<"P3\n"<<width<<" "<<height<<"\n255\n";

  vec3 lookfrom(4000,0,4000);
  vec3 lookat(700,-30,0);
  camera cam(lookfrom, lookat, vec3(0,1,0), 20, (long double)(width)/(long double)(height));
  bvh_node* world = objParser("dragon.obj");
  if(world == NULL)return -1;
  /*
  //Setting up objects without BVH
  int n = 50000;
  shape** list = new shape*[n + 1];
  //list[0] = new sphere(vec3(0, -1000, 0), 1000, 0);
  int i = 0;
  for(int a = -10; a < 10; a++){
    for(int b = -10; b < 10; b++){
      vec3 center((long double)a/(long double)5, (long double)b/(long double)5, -1);
      list[i++] = new sphere(center, 0.1, -100);
    }
  }
  bvh_node* world = new bvh_node(list, i-1);
  //shapelist* world= new shapelist(list, i);


  //Filling image pixel by pixel
  for(int i = height - 1; i >= 0; i--){
  for(int k = 0; k < width; k++){
      //using antialiasing to reduce uneven, jagged edges
      vec3 pixel(0,0,0);
      for(int n = 0; n < s; n++){
        for(int r = -hres; r < hres; r++){
          for(int c = -vres; c < vres; c++){
            long double x = long double(k + (c-hres/2 + (rand()%1000)/1000.0))/long double(width);
            long double y = long double(i + (r*(c-vres/2) + (rand()%1000)/1000.0))/long double(height);
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
  myfile.close();*/

  //Compiling and outputting a second image, in perspective view
  //Most of the code is the same from the first, with the exception of using a different camera ray method
  ofstream secondfile;
  secondfile.open("output-perspective.ppm");
  secondfile<<"P3\n"<<width<<" "<<height<<"\n255\n";
  for(int i = height - 1; i >= 0; i--){
    for(int k = 0; k < width; k++){
      //Temporarily commented out antialiasing to test render times of BVH/No BVH implementation
      /*
      vec3 pixel(0,0,0);
      for(int n = 0; n < s; n++){
        for(int r = -hres; r < hres; r++){
          for(int c = -vres; c < vres; c++){
            long double x = long double(k + (c-hres/2 + (rand()%1000)/1000.0))/long double(width);
            long double y = long double(i + (r*(c-vres/2) + (rand()%1000)/1000.0))/long double(height);
            ray r = cam.getRayP(vec3(0.5,0,0),x,y);
            pixel += fill(r, world);
          }
        }
      }*/
      ray r = cam.getRay((long double)(i)/(long double)(width),(long double)(k)/(long double)(height));
      vec3 pixel = fill(r, world, 0);
      //pixel = pixel/(hres*2*vres*2*s);
      int ir = int(255.99*pixel[0]);
      int ig = int(255.99*pixel[1]);
      int ib = int(255.99*pixel[2]);
      secondfile<<ir<<" "<<ig<<" "<<ib<<"\n";
    }
  }
  secondfile.close();
}
