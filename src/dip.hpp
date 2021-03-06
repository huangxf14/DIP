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

#define INFO(msg) {cerr << "[INFO] " << msg << endl;}
#define WARNING(msg) {cerr << "[WARNING] " << msg << endl;}
#define ERROR(msg) {cerr << "[ERROR] " << msg << endl; exit(-1);}
#define ASSERT(flag, msg) {if (!(flag)) ERROR(msg) }

class Frame {
public:
  Frame(Mat &src);
  ~Frame() {};

  void Boundary(bool &color_flag);
  void MatchKeypoints(vector<Point2i> &keypoints, vector<pair<Point2i, Point2i>> &match, double threshold, Point2i root);

  Mat img_;
  bool color_flag_;
  vector<Point2i> boundary_;
};

class LivenessDetector {
public:
  LivenessDetector(int device_id): 
    match_num_th1_(3), 
    match_num_th2_(6), 
    match_dis_th_(30)
    { vc_.open(device_id); }
  ~LivenessDetector() { vc_.release(); }
  
  void Detect();

  VideoCapture vc_;
  Mat display_;
  int last_palm_time_;
  int last_perfect_match_;
  bool is_living_;
  int match_stage_;
  bool pop_up_flag_;

  int match_num_th1_;
  int match_num_th2_;
  int match_dis_th_;
};

vector<Point2i> Match(vector<Point2i> &point1, vector<Point2i> &point2, double value = 20);
void KeypointsMask(Mat &img, vector<Point2i> &keypoints, Mat &mask);
void DrawKeypoints(Mat &img, vector<Point2i> &keypoints);
void DrawMatch(Mat &img, vector<pair<Point2i, Point2i>> &match);
void DrawInfo(Mat &img, bool is_palm, bool is_living, int stage);

#endif
