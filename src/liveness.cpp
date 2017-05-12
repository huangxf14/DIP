#include "dip.hpp"

shared_ptr<vector<Point2i>> init_keypoints(new vector<Point2i>);

shared_ptr<vector<Point2i>> GetInitKeypoints(int w, int h) {
  if (!init_keypoints->empty()) return init_keypoints;

  int w0 = 640, h0 = 480;
  double ratio = min(double(w) / double(w0), double(h) / double(h0));
  Point2i delta = Point2i((w - w0) / 2, (h - h0) / 2);
  init_keypoints->push_back(Point2i(307, 370) * ratio + delta); // palm root
  init_keypoints->push_back(Point2i(194, 309) * ratio + delta);
  init_keypoints->push_back(Point2i(152, 207) * ratio + delta); // finger 1
  init_keypoints->push_back(Point2i(232, 276) * ratio + delta);
  init_keypoints->push_back(Point2i(234, 83) * ratio + delta);  // finger 2
  init_keypoints->push_back(Point2i(273, 198) * ratio + delta);
  init_keypoints->push_back(Point2i(291, 57) * ratio + delta);  // finger 3
  init_keypoints->push_back(Point2i(313, 192) * ratio + delta);
  init_keypoints->push_back(Point2i(341, 73) * ratio + delta);  // finger 4
  init_keypoints->push_back(Point2i(350, 210) * ratio + delta);
  init_keypoints->push_back(Point2i(404, 122) * ratio + delta); // finger 5
  init_keypoints->push_back(Point2i(378, 236) * ratio + delta);


  for (int i = 0; i != init_keypoints->size(); ++i) {
    init_keypoints->at(i).x = init_keypoints->at(i).x;
    init_keypoints->at(i).y = init_keypoints->at(i).y;
  }

  return init_keypoints;
}

void LivenessDetector::Detect() {
  while (true) {
    // TODO
    Mat img;
    vc_ >> img;
    flip(img, img, 1);

    ASSERT(!img.empty(), "Empty frame!");

    int w = img.cols, h = img.rows;

    shared_ptr<vector<Point2i>> keypoints = GetInitKeypoints(w, h);
    
    if (!this->frames_.empty() && this->frames_.back().is_palm_) {
      keypoints = this->frames_.back().keypoints_;
    }

    frames_.emplace_back(img, keypoints);

    CheckLiveness();

    frames_.back().Display(is_living_);
    waitKey(20);
  }
}

void LivenessDetector::CheckLiveness() {
  // TODO
  this->is_living_ = false;
}