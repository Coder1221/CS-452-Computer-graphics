#include <nanogui/nanogui.h>
#include "../clothMesh.h"
#include "../misc/sphere_drawing.h"
#include "sphere.h"

using namespace nanogui;
using namespace CGL;

void Sphere::collide(PointMass &pm) {
  // hw part1
  // printf("hellllo thererrrr");
  auto distance  = (pm.position - this->origin).norm(); // distance between the origon and the point mass
  
  if(distance <= this->radius){ // check for collosion if the differnece is less  than     
   
    auto tagent_point = this->origin + (pm.position - this->origin).unit() * this->radius; // p = o + rd (ray from origin in the direction of pm
    
    pm.position = pm.last_position + (1-this->friction) * (tagent_point -pm.last_position);
  }

}

void Sphere::render(GLShader &shader) {
  // We decrease the radius here so flat triangles don't behave strangely
  // and intersect with the sphere when rendered
  Misc::draw_sphere(shader, origin, radius * 0.92);
}