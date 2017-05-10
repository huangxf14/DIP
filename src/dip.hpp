#ifndef DIP_HPP_INCLUDE_
#define DIP_HPP_INCLUDE_

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <cstdlib>

#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

#define INFO(msg) {cerr << "[INFO] " << (msg) << endl;}
#define WARNING(msg) {cerr << "[WARNING] " << (msg) << endl;}
#define ERROR(msg) {cerr << "[ERROR] " << (msg) << endl; exit(-1);}
#define ASSERT(flag, msg) {if (!(flag)) ERROR(msg) }

void Segmentation(const Mat &src, Mat &dst,  Mat &boundary);
void Alignment(const Mat &src, Mat &dst, const Mat &boundary, Mat &keypoints, Mat &affine_mat);

class LivenessDetector {
public:
  LivenessDetector() : is_living_(false) {}

  void PushFrame(const Mat &frame);
  bool IsLiving() { return is_living_; }

private:
  void CheckLiveness();

  vector<Mat> frames_;
  bool is_living_;
};

void DisplayResult(const Mat &img, const Mat &boundary, const Mat &keypoints, const Mat &affine_mat, bool is_living);

#endif
