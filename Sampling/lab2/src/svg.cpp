// Original file Copyright CMU462 Fall 2015:
// Kayvon Fatahalian, Keenan Crane,
// Sky Gao, Bryce Summers, Michael Choquette.
#include "svg.h"
//#include "CGL/lodepng.h"

#include "drawrend.h"
#include "transforms.h"
#include "triangulation.h"
#include <iostream>

#include "CGL/lodepng.h"

namespace CGL {

Group::~Group() {
  for (size_t i = 0; i < elements.size(); i++) {
    delete elements[i];
  } elements.clear();
}

SVG::~SVG() {
  for (size_t i = 0; i < elements.size(); i++) {
    delete elements[i];
  } elements.clear();
}

// Draw routines //

void Triangle::draw(DrawRend *dr, Matrix3x3 global_transform) {
  global_transform = global_transform * transform;

  Vector2D p0_scr = global_transform * p0_svg;
  Vector2D p1_scr = global_transform * p1_svg;
  Vector2D p2_scr = global_transform * p2_svg;

  // draw fill. Here the color field is empty, since children
  // export their own more sophisticated color() method.
  dr->rasterize_fig( p0_scr.x, p0_scr.y, p1_scr.x, p1_scr.y, p2_scr.x, p2_scr.y, Color(), this );

}

//Part1: Compute and return the barycentric coordinates of the point p; with reference to the given triangle (3 points)
Vector3D barycentric(float x0, float y0,float x1, float y1,float x2, float y2, float px, float py){
  
  Vector3D p_bary;

  // area
  float Aabc = (x1-x0)*(y2-y0) - (x2-x0)*(y1-y0);
  float Aapc = (px-x0)*(y2-y0) - (x2-x0)*(py-y0);
  float Aabp = (x1-x0)*(py-y0) - (px-x0)*(y1-y0);
  float beta = Aapc/Aabc;
  float gamma = Aabp/Aabc;

  // matrix
  // float k =y2-y0;
  // if(k==0){
  //   k=0.1;
  // }
  // float beta = ((x0*k) + ((py-y0)*(x2-x0)) -(px*k)) / (((y1-y0)*(x2-x0))- ((x1-x0)*k)); 
  // float gamma = ( py-y0 - (beta*(y1-y0)))/k;
  

  p_bary[0] = 1-(beta+gamma);
  p_bary[1] = beta;
  p_bary[2] = gamma;
  
  return p_bary;
}

/**
 * Returns the appropriate weighted combination of ColorTri's three colors.
 * Calculate the p_bary vector containing the barycentric coordinates of the point p.
 */
 Color ColorTri::color(float x0, float y0,float x1, float y1,float x2, float y2, float px, float py, SampleParams sp) {
    // Part 3
    // the following variables contain the colors at the 3 vertices of the triangle in action.
    p0_col; p1_col; p2_col;
    
    // calculating bary coordinates
    Vector3D bary = barycentric(x0,y0,x1,y1,x2,y2,px,py);

    // The following variable has been created to get you started to contain the weight point color to be returned.
    Color p_color;

    //Use the barycentric coordinates to interpolate color
    // p_color.r = p0_col.r*bary[0] + p1_col.r*bary[1] * p2_col.r*bary[2];
    // p_color.g = p0_col.g*bary[0] + p1_col.g*bary[1] * p2_col.g*bary[2];
    // p_color.b = p0_col.b*bary[0] + p1_col.b*bary[1] * p2_col.b*bary[2];
    // p_color.a = p0_col.a*bary[0] + p1_col.a*bary[1] * p2_col.a*bary[2];

    p_color = p0_col*bary[0] + p1_col*bary[1]+ p2_col*bary[2];

    return p_color;
 }

 /**
  * Returns the appropriate color from the TexTri's texture member.
  * Calculate the p_bary vector containing the barycentric coordinates of the point p.
  * p0_uv, p1_uv, p2_uv contain uv coordinate corresponding to points p0, p1, p2 respectively.
  * Use your calculated value of p_bary to calculate sp.p_uv
  */
 Color TexTri::color(float x0, float y0,float x1, float y1,float x2, float y2, float px, float py, SampleParams sp) {
 
    p0_uv; p1_uv; p2_uv; 
    sp.p_uv;
 
    return tex->sample(sp);
 }

void Group::draw(DrawRend *dr, Matrix3x3 global_transform) {
  global_transform = global_transform * transform;

  for (int i = 0; i < elements.size(); ++i)
    elements[i]->draw(dr, global_transform);
}

void Point::draw(DrawRend *dr, Matrix3x3 global_transform) {
  global_transform = global_transform * transform;
  Vector2D p = global_transform * position;
  dr->rasterize_point(p.x, p.y, style.fillColor);
}

void Line::draw(DrawRend *dr, Matrix3x3 global_transform) {
  global_transform = global_transform * transform;

  Vector2D f = global_transform * from, t = global_transform * to;
  dr->rasterize_line(f.x, f.y, t.x, t.y, style.strokeColor);
}

void Polyline::draw(DrawRend *dr, Matrix3x3 global_transform) {
  global_transform = global_transform * transform;

  Color c = style.strokeColor;

  if( c.a != 0 ) {
    int nPoints = points.size();
    for( int i = 0; i < nPoints - 1; i++ ) {
      Vector2D p0 = global_transform * points[(i+0) % nPoints];
      Vector2D p1 = global_transform * points[(i+1) % nPoints];
      dr->rasterize_line( p0.x, p0.y, p1.x, p1.y, c );
    }
  }
}

void Rect::draw(DrawRend *dr, Matrix3x3 global_transform) {
  global_transform = global_transform * transform;

  Color c;

  // draw as two triangles
  float x =  position.x, y =  position.y;
  float w = dimension.x, h = dimension.y;

  Vector2D p0 = global_transform * Vector2D(   x   ,   y   );
  Vector2D p1 = global_transform * Vector2D( x + w ,   y   );
  Vector2D p2 = global_transform * Vector2D(   x   , y + h );
  Vector2D p3 = global_transform * Vector2D( x + w , y + h );

  // draw fill
  c = style.fillColor;
  if (c.a != 0 ) {
    dr->rasterize_fig( p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, c );
    dr->rasterize_fig( p2.x, p2.y, p1.x, p1.y, p3.x, p3.y, c );
  }

  // draw outline
  c = style.strokeColor;
  if( c.a != 0 ) {
    dr->rasterize_line( p0.x, p0.y, p1.x, p1.y, c );
    dr->rasterize_line( p1.x, p1.y, p3.x, p3.y, c );
    dr->rasterize_line( p3.x, p3.y, p2.x, p2.y, c );
    dr->rasterize_line( p2.x, p2.y, p0.x, p0.y, c );
  }
}

void Polygon::draw(DrawRend *dr, Matrix3x3 global_transform) {
  global_transform = global_transform * transform;

  Color c;

  // draw fill
  c = style.fillColor;
  if( c.a != 0 ) {

    // triangulate
    std::vector<Vector2D> triangles;
    triangulate( *this, triangles );

    // draw as triangles
    for (size_t i = 0; i < triangles.size(); i += 3) {
      Vector2D p0 = global_transform * triangles[i + 0];
      Vector2D p1 = global_transform * triangles[i + 1];
      Vector2D p2 = global_transform * triangles[i + 2];
      dr->rasterize_fig( p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, c );
    }
  }

  // draw outline
  c = style.strokeColor;
  if( c.a != 0 ) {
    int nPoints = points.size();
    for( int i = 0; i < nPoints; i++ ) {
      Vector2D p0 = global_transform * points[(i+0) % nPoints];
      Vector2D p1 = global_transform * points[(i+1) % nPoints];
      dr->rasterize_line( p0.x, p0.y, p1.x, p1.y, c );
    }
  }
}

void Image::draw(DrawRend *dr, Matrix3x3 global_transform) {
  global_transform = global_transform * transform;
  Vector2D p0 = global_transform * position;
  Vector2D p1 = global_transform * (position + dimension);

  for (int x = floor(p0.x); x <= floor(p1.x); ++x) {
    for (int y = floor(p0.y); y <= floor(p1.y); ++y) {
      Color col = tex.sample_bilinear(Vector2D((x+.5-p0.x)/(p1.x-p0.x+1), (y+.5-p0.y)/(p1.y-p0.y+1)));
      dr->rasterize_point(x,y,col);
    }
  }
}

} // namespace CGL
