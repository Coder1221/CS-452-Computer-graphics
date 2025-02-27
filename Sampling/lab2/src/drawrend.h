#ifndef CGL_DRAWREND_H
#define CGL_DRAWREND_H

#include "CGL/CGL.h"
#include "CGL/renderer.h"
#include "CGL/color.h"
#include <vector>
#include <cstring>
#include "GLFW/glfw3.h"
#include "svg.h"

using namespace std;

namespace CGL {

class DrawRend : public Renderer {
 public:
  DrawRend(std::vector<SVG*> svgs_):
  svgs(svgs_), current_svg(0)
  {}

  ~DrawRend( void );

  // inherited Renderer interface functions
  void init();
  void render();
  void resize( size_t w, size_t h );
  std::string name() { return "Draw"; }
  std::string info();
  void cursor_event( float x, float y );
  void scroll_event( float offset_x, float offset_y );
  void mouse_event( int key, int event, unsigned char mods );
  void keyboard_event( int key, int event, unsigned char mods );

  void set_gl(bool gl_) { gl = gl_; }

  // calculating dot product
  float dot_product(float x0, float y0,float x1, float y1, float xp, float yp);
  
  // custom helper function
  bool inside_tri(float x0, float y0,float x1, float y1, float x2, float y2, float xp, float yp);

  // write current pixel buffer to disk
  void write_screenshot();

  // write only framebuffer to disk
  void write_framebuffer();

  // drawing functions
  void redraw();
  void draw_pixels();
  void draw_zoom();

  // view transform functions
  void view_init();
  void set_view(float x, float y, float span);
  void move_view(float dx, float dy, float scale);

  // rasterize a point
  void rasterize_point( float x, float y, Color color );

  // rasterize a line
  void rasterize_line( float x0, float y0,
                       float x1, float y1,
                       Color color);

  void rasterize_fig( float x0, float y0,
                          float x1, float y1,
                          float x2, float y2,
                          Color color, Triangle *tri = NULL );
  // rasterize a triangle
  void rasterize_triangle( float x0, float y0,
                           float x1, float y1,
                           float x2, float y2,
                           Color color, Triangle *tri = NULL );

  void rasterize_scanline( float x0, float y0,
                          float x1, float y1,
                          float x2, float y2,
                          Color color, Triangle *tri = NULL );



private:
  // Global state variables for SVGs, pixels, and view transforms
  std::vector<SVG*> svgs; size_t current_svg;
  std::vector<Matrix3x3> svg_to_ndc;
  float view_x, view_y, view_span;
  bool toggle_scanline;

  Matrix3x3 ndc_to_screen;

  std::vector<unsigned char> framebuffer;
  size_t width, height;

  // UI state info
  float cursor_x; float cursor_y;
  bool left_clicked;
  int show_zoom;
  int sample_rate;

  PixelSampleMethod psm;
  LevelSampleMethod lsm;

  bool gl;

  typedef std::vector<unsigned char> PixelColorStorage;

  // Intuitively, a sample buffer instance is a pixel,
  // or (samples_per_side x samples_per_side) sub-pixels.
  struct SampleBuffer {
    std::vector<std::vector<PixelColorStorage> > sub_pixels;
    size_t samples_per_side;

    SampleBuffer(size_t sps): samples_per_side(sps) {
      clear();
    }

    void fill_color(int i, int j, Color c) {
      PixelColorStorage &pix = sub_pixels[i][j];
      pix[0]=(unsigned)((c.r)*255.0);
      pix[1]=(unsigned)((c.g)*255.0);
      pix[2]=(unsigned)((c.b)*255.0);
      pix[3]=(unsigned)((c.a)*255.0);

    }

    void fill_pixel(Color c) {
      for (int i = 0; i < samples_per_side; ++i)
        for (int j = 0; j < samples_per_side; ++j)
          fill_color(i, j, c);
    }


    //Use this function to retrieve subpixel color. i and j are the indices of the subpixel with reference to the pixel.
    Color get_subpixel_color(int i, int j){
    	return Color(sub_pixels[i][j].data());
    }


    Color get_pixel_color() { // find and return the average rgb value of the entire pixel.
      
      // average all sub-pixels' colors within a samplebuffer to get a pixel's color
      //data returns ptr to the first element of the vector
      float sp_ratio = 1.0f / (samples_per_side * samples_per_side); //2x2 (4 pixels total, then every pixel color has probability 1/4) when sampling_rate = 4
      Color avg_spcolor = Color(0,0,0); // has 4 floats, overloaded for + operator

      for (uint y = 0; y < samples_per_side; y++) { // iterating through all subpixels, one by one
        for (uint x = 0; x < samples_per_side; x++) {
          avg_spcolor += sp_ratio * get_subpixel_color(y, x); // every subpixel contributes to the avg_spcolor by a ratio of 1/total
        }
      }

      return avg_spcolor;
    }


    void clear() {
      if (sub_pixels.size() == samples_per_side) {
        for (int i = 0; i < samples_per_side; ++i)
          for (int j = 0; j < samples_per_side; ++j)
            sub_pixels[i][j].assign(4, (unsigned char)255);
        return;
      }

      sub_pixels.clear();
      PixelColorStorage white = std::vector<unsigned char>(4, 255);
      std::vector<PixelColorStorage> row;
      row.reserve(samples_per_side);
      for (int i = 0; i < samples_per_side; ++i)
        row.push_back(white);
      sub_pixels.reserve(samples_per_side);
      for (int i = 0; i < samples_per_side; ++i)
        sub_pixels.push_back(row);
    }
  };

  std::vector<std::vector<SampleBuffer> > samplebuffer;

  // This function takes the collected sub-pixel samples and
  // combines them together to fill in the framebuffer in preparation
  // for posting pixels to the screen.
  void resolve() {
    for (int x = 0; x < width; ++x) {
      for (int y = 0; y < height; ++y) {
        Color col = samplebuffer[y][x].get_pixel_color();
        for (int k = 0; k < 4; ++k) {
          framebuffer[4 * (y * width + x) + k] = (&col.r)[k] * 255;
        }
      }
    }
  }


};

} // namespace CGL

#endif // CGL_DRAWREND_H
