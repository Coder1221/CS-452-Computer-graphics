#include "triangle.h"

#include "CGL/CGL.h"
#include "GL/glew.h"

namespace CGL { namespace StaticScene {

Triangle::Triangle(const Mesh* mesh, size_t v1, size_t v2, size_t v3) :
    mesh(mesh), v1(v1), v2(v2), v3(v3) { }

BBox Triangle::get_bbox() const {

  Vector3D p1(mesh->positions[v1]), p2(mesh->positions[v2]), p3(mesh->positions[v3]);
  BBox bb(p1);
  bb.expand(p2); 
  bb.expand(p3);
  return bb;

}
bool Triangle::intersect(const Ray& r) const {

  // Task 2 Part 6: implement ray-triangle intersection
  Vector3D a(mesh->positions[v1]), b(mesh->positions[v2]), c(mesh->positions[v3]);
  
  Vector3D T  = r.o-a;
  auto e1 = b-a;
  auto e2 = c-a;
  float det_scalar = 1/dot(cross(r.d ,e2) ,e1);

  float t = det_scalar * dot(cross(T,e1) , e2);
  if (t < r.min_t || t > r.max_t) return false;
  float u = det_scalar * dot(cross(r.d , e2) ,T);
  if (u < 0 || u > 1) return false;
  float v = det_scalar * dot(cross(T,e1), r.d);
  if (v < 0 || u + v > 1) return false;

  r.max_t = t; // changing max value
  return true;
} 


bool Triangle::intersect(const Ray& r, Intersection *isect) const {

  // Task 2 Part 6:
  // implement ray-triangle intersection. When an intersection takes
  // place, the Intersection data should be updated accordingly

  Vector3D a(mesh->positions[v1]), b(mesh->positions[v2]), c(mesh->positions[v3]);
  Vector3D n1(mesh->normals[v1]), n2(mesh->normals[v2]), n3(mesh->normals[v3]);
  
  Vector3D T  = r.o-a;
  auto e1 = b-a;
  auto e2 = c-a;
  float det_scalar = 1/dot(cross(r.d ,e2) ,e1);

  float t = det_scalar * dot(cross(T,e1) , e2);
  if (t < r.min_t || t > r.max_t) return false;
  float u = det_scalar * dot(cross(r.d , e2) ,T);
  if (u < 0 || u > 1) return false;
  float v = det_scalar * dot(cross(T,e1), r.d);
  if (v < 0 || u + v > 1) return false;

  r.max_t = t;
  isect->t = t;
  isect->primitive = this;
  isect->n = n1 * u + n2 * v + n3 * (1-u-v); // bary centric interpolation
  isect->bsdf = get_bsdf();
  return true;
}

void Triangle::draw(const Color& c) const {
  glColor4f(c.r, c.g, c.b, c.a);
  glBegin(GL_TRIANGLES);
  glVertex3d(mesh->positions[v1].x,
             mesh->positions[v1].y,
             mesh->positions[v1].z);
  glVertex3d(mesh->positions[v2].x,
             mesh->positions[v2].y,
             mesh->positions[v2].z);
  glVertex3d(mesh->positions[v3].x,
             mesh->positions[v3].y,
             mesh->positions[v3].z);
  glEnd();
}

void Triangle::drawOutline(const Color& c) const {
  glColor4f(c.r, c.g, c.b, c.a);
  glBegin(GL_LINE_LOOP);
  glVertex3d(mesh->positions[v1].x,
             mesh->positions[v1].y,
             mesh->positions[v1].z);
  glVertex3d(mesh->positions[v2].x,
             mesh->positions[v2].y,
             mesh->positions[v2].z);
  glVertex3d(mesh->positions[v3].x,
             mesh->positions[v3].y,
             mesh->positions[v3].z);
  glEnd();
}



} // namespace StaticScene
} // namespace CGL
