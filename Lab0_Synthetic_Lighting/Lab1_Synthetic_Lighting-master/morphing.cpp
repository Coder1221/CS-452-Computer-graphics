#include <opencv2/highgui.hpp>
#include "filters.cpp"
#include<iostream>
using namespace cv;
using namespace std;
//Part 1: 
Mat morphing(Mat inp1, Mat inp2, float w){
  Mat masked  = (w*inp1) + ((1-w)*inp2);
  return masked;
}

// 0->blue
// 1->green
// 2-> red

//Part 4:
Mat filtered_morphing(Mat inp1, Mat inp2, float w){
  Mat blue = colored_light(inp1, 0); // extracting that filter from image
  Mat red = colored_light(inp2 ,2);
  
  // cout<<w<<endl;
  Mat masked  = (w*red) + ((1-w)*blue);
  return masked;

}


