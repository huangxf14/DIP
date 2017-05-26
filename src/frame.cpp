#include "dip.hpp"

Frame::Frame(Mat &src) {
  src.copyTo(img_);

  // 提取中心物体的轮廓
  Boundary(color_flag_);
}

// 提取画面中心物体的轮廓
void Frame::Boundary(bool &color_flag) {
  Point2i center(img_.cols / 2, img_.rows / 2 - 40);

  // blur
  Mat tmp;
  img_.copyTo(tmp);

  // 用 flooding 取出图片中心颜色相近的区域
  Scalar filling_color(255, 255, 255);
  Scalar ld(60, 60, 80);
  Scalar ud(60, 60, 80);
  floodFill(tmp, center, filling_color, NULL, ld, ud, 4 | FLOODFILL_FIXED_RANGE);
  Mat mask;
  inRange(tmp, filling_color, filling_color, mask);

  // 检查取出区域的颜色是否与肤色相符
  Scalar mean_color = mean(img_, mask);
  Scalar palm_color_lb(100, 100, 100);
  Scalar palm_color_ub(200, 200, 255);

  color_flag = true;
  for (int c = 0; c != 3; ++c) {
    if (mean_color[c] < palm_color_lb[c] || mean_color[c] > palm_color_ub[c]) {
      color_flag = false;
    }
  }


  // 进行形态学运算，去除一些噪声
  Mat element = getStructuringElement(MORPH_RECT, Size(10, 10));
  morphologyEx(mask, mask, MORPH_OPEN, element);
  morphologyEx(mask, mask, MORPH_CLOSE, element);

  // 取出中心物体的边界
  vector<vector<Point2i>> contours;
  findContours(mask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

  vector<Point2i> tmp_;
  tmp_.push_back(Point2i(0, 0));
  contours.push_back(tmp_);

  int max_ind = 0;
  for (int i = 0; i != contours.size(); ++i) {
    if (contours[i].size() > contours[max_ind].size()) {
      max_ind = i;
    }
  }

  boundary_ = contours[max_ind];
}

// 将标准的关键点与轮廓做匹配
void Frame::MatchKeypoints(vector<Point2i>& keypoints, vector<pair<Point2i, Point2i>>& match, double threshold, Point2i root) {
  // 计算轮廓上每个点到根关键点的距离
  if (keypoints.empty() || boundary_.empty()) {
    return;
  }
  vector<double> distance;
  for (int i = 0; i != boundary_.size(); ++i) {
    Point2i p = boundary_[i];
    distance.push_back(norm(p - root));
  }

  // 取出距离的峰/谷值作为关键点
  vector<int> peaks;
  vector<int> valleys;
  int n = 5;  // 周围 n 个点单调升降则判定为峰/谷值

  for (int i = 0; i != boundary_.size(); ++i) {
    if (boundary_[i].y < root.y) {
      auto d = [distance](int x) {return distance[(x + distance.size()) % distance.size()]; };
      bool is_peak = true;
      for (int j = 1; j != n; ++j) {
        if (d(i - j) >= d(i - j + 1) || d(i + j) >= d(i + j - 1)) {
          is_peak = false;
          break;
        }
      }
      if (is_peak) {
        peaks.push_back(i);
      }

      bool is_valley = true;
      for (int j = 1; j != n; ++j) {
        if (d(i - j) <= d(i - j + 1) || d(i + j) <= d(i + j - 1)) {
          is_valley = false;
          break;
        }
      }
      if (is_valley) {
        valleys.push_back(i);
      }
    }
  }

  if (peaks.empty() || valleys.empty()) {
    return;
  }

  // 将轮廓中的峰/谷值与标准的关键点做匹配
  vector<Point2i> keypoints_detect;
  for (int i = 0; i != peaks.size(); ++i) {
    keypoints_detect.push_back(boundary_[peaks[i]]);
  }
  for (int i = 0; i != valleys.size(); ++i) {
    keypoints_detect.push_back(boundary_[valleys[i]]);
  }


  vector<Point2i> keypoints_(keypoints.begin() + 2, keypoints.end() - 1);

  vector<Point2i> match_ind = Match(keypoints_, keypoints_detect, threshold);

  match.clear();
  for (int i = 0; i != match_ind.size(); ++i) {
    match.push_back(pair<Point2i, Point2i>(keypoints_[match_ind[i].x], keypoints_detect[match_ind[i].y]));
  }
}
