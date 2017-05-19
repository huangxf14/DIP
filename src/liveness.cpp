#include "dip.hpp"

int max_stage_num = 3;
vector<shared_ptr<vector<Point2i>>> init_keypoints;

shared_ptr<vector<Point2i>> GetStandardKeypoints(int w, int h, int stage) {
  if (!init_keypoints.empty()) return init_keypoints[stage];

  for (int i = 0; i != max_stage_num; ++i) {
    init_keypoints.emplace_back(new vector<Point2i>);
  }

  int w0 = 640, h0 = 480;
  double ratio = min(double(w) / double(w0), double(h) / double(h0));
  Point2i delta = Point2i((w - w0) / 2, (h - h0) / 2);
  INFO("GET KEYPOINTS START");
  // stage 0
  init_keypoints[0]->push_back(Point2i(307, 370) * ratio + delta); // palm root
  init_keypoints[0]->push_back(Point2i(194, 309) * ratio + delta);
  init_keypoints[0]->push_back(Point2i(152, 207) * ratio + delta); // finger 1
  init_keypoints[0]->push_back(Point2i(232, 276) * ratio + delta);
  init_keypoints[0]->push_back(Point2i(234, 83) * ratio + delta);  // finger 2
  init_keypoints[0]->push_back(Point2i(273, 198) * ratio + delta);
  init_keypoints[0]->push_back(Point2i(291, 57) * ratio + delta);  // finger 3
  init_keypoints[0]->push_back(Point2i(313, 192) * ratio + delta);
  init_keypoints[0]->push_back(Point2i(341, 73) * ratio + delta);  // finger 4
  init_keypoints[0]->push_back(Point2i(350, 210) * ratio + delta);
  init_keypoints[0]->push_back(Point2i(404, 122) * ratio + delta); // finger 5
  init_keypoints[0]->push_back(Point2i(378, 236) * ratio + delta);
  INFO("GET KEYPOINTS 0");

  // stage 1
  init_keypoints[1]->push_back(Point2i(307, 370) * ratio + delta); // palm root
  init_keypoints[1]->push_back(Point2i(194, 309) * ratio + delta);
  init_keypoints[1]->push_back(Point2i(152, 207) * ratio + delta); // finger 1
  init_keypoints[1]->push_back(Point2i(232, 276) * ratio + delta);
  init_keypoints[1]->push_back(Point2i(234, 83) * ratio + delta);  // finger 2
  init_keypoints[1]->push_back(Point2i(273, 198) * ratio + delta);
  init_keypoints[1]->push_back(Point2i(291, 57) * ratio + delta);  // finger 3
  init_keypoints[1]->push_back(Point2i(313, 192) * ratio + delta);
  init_keypoints[1]->push_back(Point2i(341, 73) * ratio + delta);  // finger 4
  init_keypoints[1]->push_back(Point2i(350, 210) * ratio + delta);
  init_keypoints[1]->push_back(Point2i(404, 122) * ratio + delta); // finger 5
  init_keypoints[1]->push_back(Point2i(378, 236) * ratio + delta);
  INFO("GET KEYPOINTS 1");
  // stage 2
  init_keypoints[2]->push_back(Point2i(307, 370) * ratio + delta); // palm root
  init_keypoints[2]->push_back(Point2i(194, 309) * ratio + delta);
  init_keypoints[2]->push_back(Point2i(152, 207) * ratio + delta); // finger 1
  init_keypoints[2]->push_back(Point2i(232, 276) * ratio + delta);
  init_keypoints[2]->push_back(Point2i(234, 83) * ratio + delta);  // finger 2
  init_keypoints[2]->push_back(Point2i(273, 198) * ratio + delta);
  init_keypoints[2]->push_back(Point2i(291, 57) * ratio + delta);  // finger 3
  init_keypoints[2]->push_back(Point2i(313, 192) * ratio + delta);
  init_keypoints[2]->push_back(Point2i(341, 73) * ratio + delta);  // finger 4
  init_keypoints[2]->push_back(Point2i(350, 210) * ratio + delta);
  init_keypoints[2]->push_back(Point2i(404, 122) * ratio + delta); // finger 5
  init_keypoints[2]->push_back(Point2i(378, 236) * ratio + delta);

  INFO("GET KEYPOINTS 2");
  return init_keypoints[stage];
}



void LivenessDetector::Detect() {
  match_stage_ = 0;
  while (true) {
    // TODO
    Mat img;
    vc_ >> img;
    flip(img, img, 1);

    INFO("TRACKING");
    ASSERT(!img.empty(), "Empty frame!");

    int w = img.cols, h = img.rows;

    shared_ptr<vector<Point2i>> keypoints = GetStandardKeypoints(w, h, match_stage_);
    
    if (!this->frames_.empty() && this->frames_.back().is_palm_) {
      keypoints = this->frames_.back().keypoints_;
    }

    frames_.emplace_back(img, keypoints);

    CheckLiveness();

    frames_.back().Display(is_living_, match_stage_);

    frames_.pop_front();

    waitKey(20);
  }
}

void LivenessDetector::CheckLiveness() {
  // TODO
  this->is_living_ = false;
}