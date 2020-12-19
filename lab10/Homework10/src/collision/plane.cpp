#include "iostream"
#include <nanogui/nanogui.h>
#include "../clothMesh.h"
#include "../clothSimulator.h"
#include "plane.h"
#include <iostream>
using namespace std;
using namespace CGL;
#define SURFACE_OFFSET 0.0001

void Plane::collide(PointMass &pm) {
  // hw part2
  // in last time step a,b two points   
  auto b = pm.last_position - this->point; // last point
  auto a = pm.position - this->point; // current point  
  auto x  =  dot(b, normal); //normal to check last point above plane or not
  auto y  =  dot(a, normal); //normal to check current point above plane or not
  if( (x>0) && (y<0) ){  //  last position point above the surface and one below the surface to bump it up       
    // auto dis_from_plane = (b - a).unit();
    // auto dd = (this->point ,b).norm()*this->normal;
    // auto xaa = (this->point - a);
    // auto xaa1 = (xaa[0]*this->normal[0] + xaa[1]*this->normal[1]+ xaa[2]*this->normal[2]);
    // auto sss = dot(dis_from_plane ,this->normal);
    // auto xaa2 = xaa1/sss;
    // auto tangent_point = a + dis_from_plane*xaa2;
    // auto correction = (tangent_point - pm.last_position) * (1 - SURFACE_OFFSET);
    // pm.position = pm.last_position + correction * (1 - this->friction);
    pm.position = pm.last_position;
  }
}

void Plane::render(GLShader &shader) {
  nanogui::Color color(0.7f, 0.7f, 0.7f, 1.0f);

  Vector3f sPoint(point.x, point.y, point.z);
  Vector3f sNormal(normal.x, normal.y, normal.z);
  Vector3f sParallel(normal.y - normal.z, normal.z - normal.x,
                     normal.x - normal.y);
  sParallel.normalize();
  Vector3f sCross = sNormal.cross(sParallel);

  MatrixXf positions(3, 4);
  MatrixXf normals(3, 4);

  positions.col(0) << sPoint + 2 * (sCross + sParallel);
  positions.col(1) << sPoint + 2 * (sCross - sParallel);
  positions.col(2) << sPoint + 2 * (-sCross + sParallel);
  positions.col(3) << sPoint + 2 * (-sCross - sParallel);

  normals.col(0) << sNormal;
  normals.col(1) << sNormal;
  normals.col(2) << sNormal;
  normals.col(3) << sNormal;
  if (shader.uniform("in_color", false) != -1) {
    shader.setUniform("in_color", color);
  }
  shader.uploadAttrib("in_position", positions);
  shader.uploadAttrib("in_normal", normals);
  shader.drawArray(GL_TRIANGLE_STRIP, 0, 4);
}
