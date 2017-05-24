#include "dip.hpp"

int max_stage_num = 5;
vector<vector<Point2i>> init_keypoints;

vector<Point2i> &GetStandardKeypoints(int w, int h, int stage) {
  // 设置标准的关键点模板

  if (stage >= max_stage_num) {
    stage = max_stage_num - 1;
  }
  if (!init_keypoints.empty()) return init_keypoints[stage];

  for (int i = 0; i != max_stage_num; ++i) {
    init_keypoints.push_back(vector<Point2i>());
  }

  int w0 = 640, h0 = 480;
  double ratio = min(double(w) / double(w0), double(h) / double(h0));
  Point2i delta = Point2i((w - w0) / 2, (h - h0) / 2);
  INFO("GET KEYPOINTS START");
  // stage 0
  init_keypoints[0].push_back(Point2i(307, 390) * ratio + delta); // palm root
  init_keypoints[0].push_back(Point2i(194, 329) * ratio + delta);
  init_keypoints[0].push_back(Point2i(152, 227) * ratio + delta); // finger 1
  init_keypoints[0].push_back(Point2i(232, 296) * ratio + delta);
  init_keypoints[0].push_back(Point2i(234, 103) * ratio + delta);  // finger 2
  init_keypoints[0].push_back(Point2i(273, 218) * ratio + delta);
  init_keypoints[0].push_back(Point2i(291, 77) * ratio + delta);  // finger 3
  init_keypoints[0].push_back(Point2i(313, 212) * ratio + delta);
  init_keypoints[0].push_back(Point2i(341, 93) * ratio + delta);  // finger 4
  init_keypoints[0].push_back(Point2i(350, 230) * ratio + delta);
  init_keypoints[0].push_back(Point2i(404, 142) * ratio + delta); // finger 5
  init_keypoints[0].push_back(Point2i(378, 256) * ratio + delta);
  INFO("GET KEYPOINTS 0");

  // stage 1
  init_keypoints[1].push_back(Point2i(307, 390) * ratio + delta); // palm root
  init_keypoints[1].push_back(Point2i(194, 329) * ratio + delta);
  init_keypoints[1].push_back(Point2i(120, 267) * ratio + delta); // finger 1
  init_keypoints[1].push_back(Point2i(232, 296) * ratio + delta);
  init_keypoints[1].push_back(Point2i(234, 103) * ratio + delta);  // finger 2
  init_keypoints[1].push_back(Point2i(273, 218) * ratio + delta);
  init_keypoints[1].push_back(Point2i(291, 77) * ratio + delta);  // finger 3
  init_keypoints[1].push_back(Point2i(313, 212) * ratio + delta);
  init_keypoints[1].push_back(Point2i(341, 93) * ratio + delta);  // finger 4
  init_keypoints[1].push_back(Point2i(350, 230) * ratio + delta);
  init_keypoints[1].push_back(Point2i(404, 142) * ratio + delta); // finger 5
  init_keypoints[1].push_back(Point2i(378, 256) * ratio + delta);
  INFO("GET KEYPOINTS 1");

  // stage 2
  init_keypoints[2].push_back(Point2i(307, 390) * ratio + delta); // palm root
  init_keypoints[2].push_back(Point2i(194, 329) * ratio + delta);
  init_keypoints[2].push_back(Point2i(152, 227) * ratio + delta); // finger 1
  init_keypoints[2].push_back(Point2i(232, 296) * ratio + delta);
  init_keypoints[2].push_back(Point2i(234, 103) * ratio + delta);  // finger 2
  init_keypoints[2].push_back(Point2i(273, 218) * ratio + delta);
  init_keypoints[2].push_back(Point2i(291, 77) * ratio + delta);  // finger 3
  init_keypoints[2].push_back(Point2i(313, 212) * ratio + delta);
  init_keypoints[2].push_back(Point2i(341, 93) * ratio + delta);  // finger 4
  init_keypoints[2].push_back(Point2i(350, 230) * ratio + delta);
  init_keypoints[2].push_back(Point2i(404, 142) * ratio + delta); // finger 5
  init_keypoints[2].push_back(Point2i(378, 256) * ratio + delta);

  INFO("GET KEYPOINTS 2");


  // stage 3
  init_keypoints[3].push_back(Point2i(307, 390) * ratio + delta); // palm root
  init_keypoints[3].push_back(Point2i(194, 329) * ratio + delta);
  init_keypoints[3].push_back(Point2i(152, 227) * ratio + delta); // finger 1
  init_keypoints[3].push_back(Point2i(232, 296) * ratio + delta);
  init_keypoints[3].push_back(Point2i(190, 120) * ratio + delta);  // finger 2
  init_keypoints[3].push_back(Point2i(273, 218) * ratio + delta);
  init_keypoints[3].push_back(Point2i(291, 77) * ratio + delta);  // finger 3
  init_keypoints[3].push_back(Point2i(313, 212) * ratio + delta);
  init_keypoints[3].push_back(Point2i(341, 93) * ratio + delta);  // finger 4
  init_keypoints[3].push_back(Point2i(350, 230) * ratio + delta);
  init_keypoints[3].push_back(Point2i(404, 142) * ratio + delta); // finger 5
  init_keypoints[3].push_back(Point2i(378, 256) * ratio + delta);
  INFO("GET KEYPOINTS 3");

  // stage 4
  init_keypoints[4].push_back(Point2i(307, 390) * ratio + delta); // palm root
  init_keypoints[4].push_back(Point2i(194, 329) * ratio + delta);
  init_keypoints[4].push_back(Point2i(152, 227) * ratio + delta); // finger 1
  init_keypoints[4].push_back(Point2i(232, 296) * ratio + delta);
  init_keypoints[4].push_back(Point2i(234, 103) * ratio + delta);  // finger 2
  init_keypoints[4].push_back(Point2i(273, 218) * ratio + delta);
  init_keypoints[4].push_back(Point2i(291, 77) * ratio + delta);  // finger 3
  init_keypoints[4].push_back(Point2i(313, 212) * ratio + delta);
  init_keypoints[4].push_back(Point2i(341, 93) * ratio + delta);  // finger 4
  init_keypoints[4].push_back(Point2i(350, 230) * ratio + delta);
  init_keypoints[4].push_back(Point2i(404, 142) * ratio + delta); // finger 5
  init_keypoints[4].push_back(Point2i(378, 256) * ratio + delta);

  INFO("GET KEYPOINTS 4");

  return init_keypoints[stage];
}



void LivenessDetector::Detect() {
  match_stage_ = 0;
  last_palm_time_ = 0;
  is_living_ = false;
  while (true) {
    Mat img;
    vc_ >> img;
    flip(img, img, 1);

    INFO("TRACKING");
    ASSERT(!img.empty(), "Empty frame!");

    int w = img.cols, h = img.rows;
    Frame frame(img);

    vector<Point2i> keypoints = GetStandardKeypoints(w, h, match_stage_);
    vector<pair<Point2i, Point2i>> match;
    
    // 对每一帧提取出的轮廓，判断其轮廓上的关键点是否与模板匹配
    frame.MatchKeypoints(keypoints, match, 30);
    INFO("match num: " << match.size());

    ++last_palm_time_;
    ++last_perfect_match_;
    bool is_palm = false;
    bool is_perfect_match = false;
    if (match.size() >= 5 && frame.color_flag_) {
      is_palm = true;
      last_palm_time_ = 0;
    }
    if (is_palm && match.size() >= 7) {
      is_perfect_match = true;
    }


    // 在关键点基本匹配的情况下判断几个重要的关键点是否匹配
    if (is_perfect_match && last_perfect_match_ > 10 && match_stage_ < max_stage_num) {
      bool essential_keypoint_match = true;
      vector <Point2i> essential_keypoints;
      vector <pair<Point2i, Point2i> > essential_match;
      switch (match_stage_) {
      case 1:
      case 2:
        essential_keypoints.push_back(keypoints[2]); break;
      case 3:
      case 4:
        essential_keypoints.push_back(keypoints[4]); break;
      }
      frame.MatchKeypoints(essential_keypoints, essential_match, 35);
      if (essential_match.size() == essential_keypoints.size()) {
        last_perfect_match_ = 0;
        ++match_stage_;  // 如果匹配成功的话则指示用户做下一个动作
      }
    }

    if (match_stage_ == max_stage_num) {
      is_living_ = true;
    }

    if (last_palm_time_ >= 10) {
      is_living_ = false;
      match_stage_ = 0;
    }

    img.copyTo(display_);
    DrawKeypoints(display_, keypoints);
    DrawMatch(display_, match);
    DrawInfo(display_, is_palm || last_palm_time_ < 8, is_living_, match_stage_);
    imshow("display", display_);
    waitKey(25);
  }
}
