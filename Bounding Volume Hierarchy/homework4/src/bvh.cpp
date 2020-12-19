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
  
  // BBox centroid_box, bbox;
  // for (Primitive *p : prims) {   // for each primitive in 'prims'
  //   BBox bb = p->get_bbox();     // get the bounding box of the primitive
  //   bbox.expand(bb);             // expand bounding box to include bbox
  //   Vector3D c = bb.centroid();
  //   centroid_box.expand(c);
  // }
  // BVHNode *node = new BVHNode(bbox); // new node with bb initialized enclosing all the primitives
  // return node;
  
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
  int rec_axis;  // recursion axis
  Vector3D bboxdim = centroid_box.extent;

  double Max = max(bboxdim.x,max(bboxdim.y, bboxdim.z));  // longest recursion axis
  
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
  // cout<<"This one"<<endl;

  double  t0,t1;  
  if (node->bb.intersect(ray,t0,t1)){
    if(t1>ray.min_t && t1<ray.max_t){
     if (node->isLeaf()){
      for (Primitive *p : *(node->prims)) {
        total_isects++;
        if (p->intersect(ray)){
          return true;
        } 
      }
    }else{
      total_isects++;
      bool left_child  = intersect(ray , node->l);
      bool right_child = intersect(ray , node->r);
      return left_child || right_child;
    }
  }else{
      return false;
    }
  }
  return false;
}

bool BVHAccel::intersect(const Ray& ray, Intersection* i, BVHNode *node) const {
  
  // cout<<"thissssssssssssssssssssss";
  double  t1,t2;
  if (node->bb.intersect(ray,t1,t2)){
    if(t1>ray.min_t && t1<ray.max_t){
     if (node->isLeaf()){  // leaf node 
      // bool RE;
      bool temp ,prev=false;
      // min_t ,max_t;
      for (Primitive *p : *(node->prims)) {  // check for all premittives 
        total_isects++;
        // p->get_bbox().intersect(ray)
        temp  = p->intersect(ray,i) || prev; // if one of the premittive returns true then the whole  premittive it must retunrn true :TA;
        prev = temp;
        // p->get_bbox().intersect()
        // if (p->intersect(ray ,i)){ // intersection testing
          //  RE = true;
        // }
        // RE = p->intersect(ray,i) ; //it will store the max in its own intersect routine
      }
      return prev;
      // return true;
      // return RE;
    }else{
      total_isects++; // intersect with node
      bool left_child  = intersect(ray ,i, node->l);
      bool right_child = intersect(ray ,i, node->r);
      return left_child || right_child; 
      }
    }else{
      // cout<<"testeddd";
      return false;
    }
  }
  return false;
  
}

}  // namespace StaticScene
}  // namespace CGL