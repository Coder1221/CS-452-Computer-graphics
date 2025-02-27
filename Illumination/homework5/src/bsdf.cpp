#include "bsdf.h"

#include <iostream>
#include <algorithm>
#include <utility>

using std::min;
using std::max;
using std::swap;

namespace CGL {

void make_coord_space(Matrix3x3& o2w, const Vector3D& n) {

    Vector3D z = Vector3D(n.x, n.y, n.z);
    Vector3D h = z;
    if (fabs(h.x) <= fabs(h.y) && fabs(h.x) <= fabs(h.z)) h.x = 1.0;
    else if (fabs(h.y) <= fabs(h.x) && fabs(h.y) <= fabs(h.z)) h.y = 1.0;
    else h.z = 1.0;

    z.normalize();
    Vector3D y = cross(h, z);
    y.normalize();
    Vector3D x = cross(z, y);
    x.normalize();

    o2w[0] = x;
    o2w[1] = y;
    o2w[2] = z;
}


// Diffuse BSDF //

Spectrum DiffuseBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  // Part 3, Task 1:
  // This function takes in both wo and wi and returns the evaluation of
  // the BSDF for those two directions.

  return reflectance / PI;

}

Spectrum DiffuseBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {
  // Part 3, Task 1:
  // This function takes in only wo and provides pointers for wi and pdf,
  // which should be assigned by this function.
  // After sampling a value for wi, it returns the evaluation of the BSDF
  // at (wo, *wi).  

  *wi = sampler.get_sample(pdf);
  return f(wo, *wi);

}


// Mirror BSDF //

Spectrum MirrorBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  return Spectrum();
}

Spectrum MirrorBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {
  // TODO: 3-2 Part 1 Task 2
  // Implement MirrorBSDF

  // return Spectrum();
  *pdf = 1.0;
  reflect(wo, wi);
  // *wi = sampler.get_sample(pdf);
  return reflectance / abs_cos_theta(*wi);
}


// Glossy BSDF //

Spectrum GlossyBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  return Spectrum();
}

Spectrum GlossyBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {
  return Spectrum();
}



// Refraction BSDF //

Spectrum RefractionBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  return Spectrum();
}

Spectrum RefractionBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {
  return Spectrum();
}

// Glass BSDF //

Spectrum GlassBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  return Spectrum();
}

Spectrum GlassBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {
  if (!refract(wo, wi, ior)) {
   reflect(wo, wi);
   *pdf = 1.0;
   return reflectance / abs_cos_theta(*wi);
 }
 else {
   float R_o = pow(((1.0 - ior) / (1.0 + ior)), 2);
   float R = R_o + (1.0 - R_o) * pow((1.0 - abs_cos_theta(wo)), 5);
   if (coin_flip(R)) {
     reflect(wo, wi);
     *pdf = R;
     return R * reflectance / abs_cos_theta(*wi);
   }
   else {
     refract(wo, wi, ior);
     *pdf = 1.0 - R;
     float eta;
     if (wo.z > 0) {
       eta = 1.0/ior;
     }
     if (wo.z < 0) {
       eta = ior;
     }
     return (1.0 - R) * transmittance / abs_cos_theta(*wi) / pow(eta, 2);
   }
 }
}

void BSDF::reflect(const Vector3D& wo, Vector3D* wi) {
   *wi =Vector3D(-wo.x ,-wo.y ,wo.z); // (-x,-y)
}

bool BSDF::refract(const Vector3D& wo, Vector3D* wi, float ior) {
  float eta = ior;
  float sign_indicator  = 1.0 ;
  if(wo.z>0){ // entering the non-air material
    eta = 1.0/ior;
    sign_indicator   = -1.0; // z axis negative when light starts from non -air material
  }
  float temp  = 1 - (eta*eta)*(sin_theta2(wo));
  if(temp<0) return false; // total reflection
  float  Z = sign_indicator * sqrt(temp);
  *wi = Vector3D (-1*eta*wo.x , -1*eta*wo.y , Z);
  return true;
}

// Emission BSDF // 

Spectrum EmissionBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  return Spectrum();
}

Spectrum EmissionBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {
  *pdf = 1.0 / PI;
  *wi  = sampler.get_sample(pdf);
  return Spectrum();
}

} // namespace CGL
