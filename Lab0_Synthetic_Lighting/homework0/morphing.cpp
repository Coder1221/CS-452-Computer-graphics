#include <opencv2/highgui.hpp>
#include "filters.cpp"
#include <iostream>

using namespace cv;
using namespace std;

// orientaion of images = top ,left ,bottom,right
//Assignment Part 2
Mat morphing4(Mat inp1, Mat inp2, Mat inp3, Mat inp4, float wv, float wh){
  
  // sir soultion
  Mat morph = (wh*inp1)+((1-wh)*inp2);
  Mat morph2 = (wh*inp3)+((1-wh)*inp4);
  Mat res =  (wv*morph)+ ((1-wv)*morph2);
  // return res;

  Mat F = (1-wh)*(1-wv)*inp1 + (inp2*(1-wh)*wv);
  Mat F2 = (wh*(1-wv)*inp3) + (wh*wv*inp4);
  
  return (F+F2)/1;
}

// Assignment Part 3
Mat colored_morphing4(Mat inp1, Mat inp2, Mat inp3, Mat inp4, float wv, float wh){
  Mat d = colored_light(inp1 ,0);//b
  Mat c = colored_light(inp2 ,1);//g
  Mat b = colored_light(inp3 ,2);//r
  Mat a = colored_light(inp4 ,3); //same image
  // inp1=d;
  // inp2=c;
  // inp3=b;
  // inp4=a;
  // Mat morph =  ((1-wh)*inp1) + (wh*inp2) + ((1-wv)*inp1) + (wv*inp3)-inp1;
  // if (wh>0.5 && wv>0.5){
  //   morph += ((1- (wh*wv))*inp3) + (wv*wh*inp4) - (-inp1+inp2+inp3);  
  // }
  // Mat res = morph;
  // return res;

  return morphing4(d,c,b,a,wv,wh);
}