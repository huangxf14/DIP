#ifndef DIP_HPP_INCLUDE_
#define DIP_HPP_INCLUDE_

#include <algorithm>
#include <deque>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <cmath>
#include <cstdlib>

#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;



#define INFO(msg) {cerr << "[INFO] " << (msg) << endl;}
#define WARNING(msg) {cerr << "[WARNING] " << (msg) << endl;}
#define ERROR(msg) {cerr << "[ERROR] " << (msg) << endl; exit(-1);}
#define ASSERT(flag, msg) {if (!(flag)) ERROR(msg) }

class Frame {
public:
  Frame(const Mat &src, shared_ptr<vector<Point2i>> kpts);
  ~Frame() {};

  void CheckPalm();
  //void Segment();
  //void GetKeypoints();
  void AffineTrans();

  void Display(bool is_living_, int match_stage);
  void KeypointsMask(const shared_ptr<vector<Point2i>> keypoints, Mat &mask);
  void DrawKeypoints(const Mat &src, Mat &dst);

  double finger_thickness_;

  Mat img_;
  Mat palm_mask_;
  Mat palm_;  // after alignment
  bool is_palm_;

  shared_ptr<vector<Point2i>> init_keypoints_;
  shared_ptr<vector<Point2i>> keypoints_;
  Mat affine_mat_;
};

class LivenessDetector {
public:
  LivenessDetector(int device_id) { vc_.open(device_id); }
  ~LivenessDetector() { vc_.release(); }
  
  void Detect();

private:
  void CheckLiveness();

  VideoCapture vc_;
  deque<Frame> frames_;
  Mat display_;
  bool is_living_;
  int match_stage_;
};

vector<Point2i> Match(vector<Point2i> &point1, vector<Point2i> &point2);

#endif
