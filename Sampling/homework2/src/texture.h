#ifndef CGL_TEXTURE_H
#define CGL_TEXTURE_H

#include <vector>
#include "CGL/CGL.h"
#include "CGL/color.h"
#include "CGL/vector2D.h"

#include <iostream>
using namespace std;

namespace CGL {

typedef enum PixelSampleMethod { P_NEAREST = 0, P_LINEAR = 1 } PixelSampleMethod;
typedef enum LevelSampleMethod { L_ZERO = 0, L_NEAREST = 1, L_LINEAR = 2 } LevelSampleMethod;

struct SampleParams {
  Vector2D p_uv;
  Vector2D p_dx_uv, p_dy_uv;
  PixelSampleMethod psm;
  LevelSampleMethod lsm;
};

static const int kMaxMipLevels = 14;

struct MipLevel {
	size_t width;
	size_t height;
  std::vector<unsigned char> texels;
};

struct Texture {
  size_t width;
  size_t height;
  std::vector<MipLevel> mipmap;

  void init(const vector<unsigned char>& pixels, const size_t& w, const size_t& h) {
    width = w; height = h;

    // A fancy C++11 feature. emplace_back constructs the element in place,
    // and in this case it uses the new {} list constructor syntax.
    mipmap.emplace_back(MipLevel{width, height, pixels});

    generate_mips();
  }

  // Generates up to kMaxMipLevels of mip maps. Level 0 contains
  // the unfiltered original pixels.
  void generate_mips(int startLevel = 0);

  Color sample(const SampleParams &sp);

  Color sample_nearest(Vector2D uv);

  Color sample_bilinear(Vector2D uv);
  Color get_color(int level , int x0 ,int y0, int index);
  Color linear_interpolation(Color a, Color b ,int x1,int x2,float x);
};

}

#endif // CGL_TEXTURE_H
