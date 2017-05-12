#include "dip.hpp"

Frame::Frame(const Mat &src, shared_ptr<vector<Point2i>> kpts) {
  // param
  finger_thickness_ = 30;  // TODO

  src.copyTo(img_);

  ASSERT(!kpts->empty(), "keypoints empty!");
  this->init_keypoints_ = kpts;

  CheckPalm();

  if (is_palm_) {
    //Segment();
    //GetKeypoints();
    AffineTrans();
  } else { // not palm
    this->keypoints_ = kpts;
  }
}

void Frame::CheckPalm() {
  // TODO
  this->is_palm_ = false;

  Point2i center = (init_keypoints_->at(0) + init_keypoints_->at(3) + init_keypoints_->at(9));
  center.x = center.x / 3;
  center.y = center.y / 3;

  // blur
  Mat tmp;
  img_.copyTo(tmp);

  int blur_range = 5;
  double blur_sigma = 3;

  GaussianBlur(tmp, tmp, Size(blur_range, blur_range), blur_sigma);

  // flood fill
  Scalar filling_color(255, 255, 255);
  Scalar ld(40, 40, 40);
  Scalar ud(40, 40, 40);
  floodFill(tmp, center, filling_color, NULL, ld , ud, 8 | FLOODFILL_FIXED_RANGE);

  Mat mask;
  inRange(tmp, filling_color, filling_color, mask);

  // TODO: make it looks better
  Mat element = getStructuringElement(MORPH_RECT, Size(10, 10));
  morphologyEx(mask, mask, MORPH_OPEN, element);
  morphologyEx(mask, mask, MORPH_CLOSE, element);

  vector<vector<Point2i>> contours;
  findContours(mask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

  int max_ind = 0;
  for (int i = 0; i != contours.size(); ++i) {
    if (contours[i].size() > contours[max_ind].size()) {
      max_ind = i;
    }
  }

  Mat disp;
  img_.copyTo(disp, mask);
  drawContours(disp, contours, max_ind, Scalar(255, 0, 0));

  imshow("flood", disp);


  // Calc keypoints
  // TODO !!
  Point2i root = init_keypoints_->at(0);

  vector<Point2i> contour = contours[max_ind];
  vector<double> distance;
  for (int i = 0; i != contour.size(); ++i) {
    Point2i p = contour[i];

    distance.push_back(norm(p - root));
  }

  vector<int> peaks;
  vector<int> valleys;
  int n = 5;  // 周围 n 个点单调升降则判定为峰/谷值

  if (contour.size() < 2 * n + 1) {
    return;
  }
  for (int i = n; i != contour.size() - n; ++i) {
    if (contour[i].y < root.y) {
      bool is_peak = true;
      for (int j = 1; j != n; ++j) {
        if (distance[i - j] >= distance[i - j + 1] || distance[i + j] >= distance[i + j - 1]) {
          is_peak = false;
          break;
        }
      }
      if (is_peak) {
        peaks.push_back(i);
      }

      bool is_valley = true;
      for (int j = 1; j != n; ++j) {
        if (distance[i - j] <= distance[i - j + 1] || distance[i + j] <= distance[i + j - 1]) {
          is_valley = false;
          break;
        }
      }
      if (is_valley) {
        valleys.push_back(i);
      }
    }
  }

  // Draw peak valley
  if (!peaks.empty() && !valleys.empty()) {
    for (int i = 0; i != peaks.size(); ++i) {
      circle(disp, contour[peaks[i]], 2, Scalar(0, 0, 255), 2);
    }
    for (int i = 0; i != valleys.size(); ++i) {
      circle(disp, contour[valleys[i]], 2, Scalar(0, 255, 0), 2);
    }
  }
  imshow("keypoints", disp);

}

//void Frame::Segment() {
//  // TODO
//}
//
//void Frame::GetKeypoints() {
//  // TODO
//}

void Frame::AffineTrans() {
  // TODO
}

void Frame::Display(bool is_living) {
  // TODO
  Mat display;
  DrawKeypoints(img_, display);

  if (is_living) {
    imshow("display", display);
  } else { // not living
    imshow("display", display);
  }
}

void Frame::KeypointsMask(const shared_ptr<vector<Point2i>> keypoints, Mat &mask) {

  mask = Mat(img_.rows, img_.cols, CV_8UC1);

  Scalar color(255);
  int thickness = this->finger_thickness_;

  // palm
  vector<Point2i> palm_boundary = {
    keypoints->at(0),
    keypoints->at(1),
    keypoints->at(3),
    keypoints->at(5),
    keypoints->at(7),
    keypoints->at(9),
    keypoints->at(11)
  };
  for (int i = 0; i != palm_boundary.size(); ++i) {
    line(mask, palm_boundary.at(i), palm_boundary.at((i+1) % palm_boundary.size()), color, thickness);
  }
  fillConvexPoly(mask, palm_boundary, color);

  // finger 1
  Point2i finger1root(
    (keypoints->at(1).x + keypoints->at(3).x) / 2,
    (keypoints->at(1).y + keypoints->at(3).y) / 2);
  line(mask, finger1root, keypoints->at(2), color, thickness);

  // finger 2
  Point2i finger2root(
    (keypoints->at(3).x + keypoints->at(5).x) / 2,
    (keypoints->at(3).y + keypoints->at(5).y) / 2);
  line(mask, finger2root, keypoints->at(4), color, thickness);


  // finger 3
  Point2i finger3root(
    (keypoints->at(5).x + keypoints->at(7).x) / 2,
    (keypoints->at(5).y + keypoints->at(7).y) / 2);
  line(mask, finger3root, keypoints->at(6), color, thickness);

  // finger 4
  Point2i finger4root(
    (keypoints->at(7).x + keypoints->at(9).x) / 2,
    (keypoints->at(7).y + keypoints->at(9).y) / 2);
  line(mask, finger4root, keypoints->at(8), color, thickness);

  // finger 5
  Point2i finger5root(
    (keypoints->at(9).x + keypoints->at(11).x) / 2,
    (keypoints->at(9).y + keypoints->at(11).y) / 2);
  line(mask, finger5root, keypoints->at(10), color, thickness);

  threshold(mask, mask, 0, 255, THRESH_BINARY);
}

void Frame::DrawKeypoints(const Mat & src, Mat & dst) {
  ASSERT(!keypoints_->empty(), "Draw keypoints empty!");
  src.copyTo(dst);

  Mat mask;
  KeypointsMask(keypoints_, mask);
  dst.setTo(Scalar(0, 0, 255), mask);
  addWeighted(src, 0.8, dst, 0.2, 0., dst);
}
