#include "bvh.h"

#include "CGL/CGL.h"
#include "static_scene/triangle.h"

#include <iostream>
#include <stack>

using namespace std;

namespace CGL { namespace StaticScene {

BVHAccel::BVHAccel(const std::vector<Primitive *> &_primitives,
                   size_t max_leaf_size) {

  root = construct_bvh(_primitives, max_leaf_size);

}

BVHAccel::~BVHAccel() {
  if (root) delete root;
}

BBox BVHAccel::get_bbox() const {
  return root->bb;
}

void BVHAccel::draw(BVHNode *node, const Color& c) const {
  if (node->isLeaf()) {
    for (Primitive *p : *(node->prims))
      p->draw(c);
  } else {
    draw(node->l, c);
    draw(node->r, c);
  }
}

void BVHAccel::drawOutline(BVHNode *node, const Color& c) const {
  if (node->isLeaf()) {
    for (Primitive *p : *(node->prims))
      p->drawOutline(c);
  } else {
    drawOutline(node->l, c);
    drawOutline(node->r, c);
  }
}

BVHNode *BVHAccel::construct_bvh(const std::vector<Primitive*>& prims, size_t max_leaf_size) {
  
  BBox centroid_box, bbox;

  for (Primitive *p : prims) {
    BBox bb = p->get_bbox();
    bbox.expand(bb);
    Vector3D c = bb.centroid();
    centroid_box.expand(c);
  }

  BVHNode *node = new BVHNode(bbox);

  node->prims = new vector<Primitive *>(prims);
  
  if(prims.size() <=max_leaf_size){
    return node;
  }
  int rec_axis;
  Vector3D bboxdim = centroid_box.extent;
  double Max = max(bboxdim.x,max(bboxdim.y, bboxdim.z));
  if(Max == bboxdim.x){
    rec_axis=0;
  }else if (Max ==bboxdim.y){
    rec_axis=1;
  }else{
    rec_axis=2;
  }

  Vector3D  splitpoint = centroid_box.centroid();
  vector<Primitive*> letftprims ,rightprims;

  for (Primitive *p:prims){
    Vector3D temp = p->get_bbox().centroid();
    if(temp[rec_axis] <= splitpoint[rec_axis]){
      letftprims.push_back(p);
    }else
        rightprims.push_back(p);
  }
  node->l = construct_bvh(letftprims ,max_leaf_size);
  node->r = construct_bvh(rightprims,max_leaf_size);
  return node;
}


bool BVHAccel::intersect(const Ray& ray, BVHNode *node) const {

  for (Primitive *p : *(root->prims)) {
    total_isects++;
    if (p->intersect(ray)) 
      return true;
  }
  return false;

}

bool BVHAccel::intersect(const Ray& ray, Intersection* i, BVHNode *node) const {

  bool hit = false;
  for (Primitive *p : *(root->prims)) {
    total_isects++;
    if (p->intersect(ray, i)) 
      hit = true;
  }
  return hit;
  
}

}  // namespace StaticScene
}  // namespace CGL
