#include "sphere.h"

#include <cmath>

#include  "../bsdf.h"
#include "../misc/sphere_drawing.h"

namespace CGL { namespace StaticScene {

//PART 4
bool Sphere::test(const Ray& r, double& t1, double& t2) const {


	const double A = dot(r.d, r.d);
	const double B = dot(r.d, r.o-o) * 2;
	const double C = dot(r.o-o , r.o-o)-r2;
	
	const double det = B*B -4*A*C;
	
	if(det<0)
		return false;

	t1 = (-B-sqrt(det))/(2*A);
	t2 = (-B+sqrt(det))/(2*A);

  return true;

}

//PART 5
bool Sphere::intersect(const Ray& r, Intersection *i) const {
  double t1,t2;

  if(!test(r,t1,t2))
  	return false;


  double t_int = min(t1,t2);

  const bool hit = ( t_int>r.min_t  && t_int<r.max_t);

  if(hit){
  	Vector3D normal = (r.o+t_int*r.d-o);
  	normal.normalize();
  	r.max_t = t_int;
  	i->t = t_int;
  	i->primitive = this;
  	i->n=normal;
  	i->bsdf = get_bsdf();
  	return true;
  }
  return false;

}

void Sphere::draw(const Color& c) const {
  Misc::draw_sphere_opengl(o, r, c);
}

bool Sphere::intersect(const Ray& r) const {}


void Sphere::drawOutline(const Color& c) const {
    //Misc::draw_sphere_opengl(o, r, c);
}


} // namespace StaticScene
} // namespace CGL
