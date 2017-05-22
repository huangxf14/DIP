#include "dip.hpp"

Frame::Frame(Mat &src) {
  src.copyTo(img_);

  // ��ȡ�������������
  Boundary(color_flag_);
}

// ��ȡ�����������������
void Frame::Boundary(bool &color_flag) {
  Point2i center(img_.cols / 2, img_.rows / 2);

  // blur
  Mat tmp;
  img_.copyTo(tmp);

  // int blur_range = 5;
  // double blur_sigma = 3;

  // GaussianBlur(tmp, tmp, Size(blur_range, blur_range), blur_sigma);

  // �� flooding ȡ��ͼƬ������ɫ���������
  Scalar filling_color(255, 255, 255);
  Scalar ld(60, 60, 80);
  Scalar ud(60, 60, 80);
  floodFill(tmp, center, filling_color, NULL, ld, ud, 4 | FLOODFILL_FIXED_RANGE);
  Mat mask;
  inRange(tmp, filling_color, filling_color, mask);

  // ���ȡ���������ɫ�Ƿ����ɫ���
  Scalar mean_color = mean(img_, mask);
  Scalar palm_color_lb(160, 100, 100);
  Scalar palm_color_ub(255, 200, 200);

  color_flag = true;
  for (int c = 0; c != 3; ++c) {
    if (mean_color[c] < palm_color_lb[c] || mean_color[c] > palm_color_ub[c]) {
      color_flag = false;
    }
  }


  // ������̬ѧ���㣬ȥ��һЩ����
  Mat element = getStructuringElement(MORPH_RECT, Size(10, 10));
  morphologyEx(mask, mask, MORPH_OPEN, element);
  morphologyEx(mask, mask, MORPH_CLOSE, element);

  // ȡ����������ı߽�
  vector<vector<Point2i>> contours;
  findContours(mask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

  int max_ind = 0;
  for (int i = 0; i != contours.size(); ++i) {
    if (contours[i].size() > contours[max_ind].size()) {
      max_ind = i;
    }
  }

  boundary_ = contours[max_ind];
}


void Frame::Display(bool is_living, int match_stage) {
  // TODO
  Mat display;
  INFO("DRAW KEYPOINTS");
  INFO("DISPLAY");
  if (0) {
    putText(display, "Find Palm", Point2i(20, 30), FONT_HERSHEY_SCRIPT_SIMPLEX, 1, Scalar(0, 255, 0));
  } else {
    putText(display, "Can't Find Palm", Point2i(20, 30), FONT_HERSHEY_SCRIPT_SIMPLEX, 1, Scalar(0, 0, 255));
  }

  if (is_living) {
    putText(display, "Living", Point2i(20, 60), FONT_HERSHEY_SCRIPT_SIMPLEX, 1, Scalar(0, 255, 0));
  } else {
    putText(display, "Not Living", Point2i(20, 60), FONT_HERSHEY_SCRIPT_SIMPLEX, 1, Scalar(0, 0, 255));
  }

  if (0) {
    putText(display, "Place your hand ???", Point2i(20, 90), FONT_HERSHEY_SCRIPT_SIMPLEX, 1, Scalar(0, 0, 255));
  }

  imshow("display", display);
}


// ����׼�Ĺؼ�����������ƥ��
void Frame::MatchKeypoints(vector<Point2i>& keypoints, vector<Point2i>& match, double threshold) {
  // ����������ÿ���㵽���ؼ���ľ���
  Point2i root = keypoints[0];
  vector<double> distance;
  for (int i = 0; i != boundary_.size(); ++i) {
    Point2i p = boundary_[i];
    distance.push_back(norm(p - root));
  }

  // ȡ������ķ�/��ֵ��Ϊ�ؼ���
  vector<int> peaks;
  vector<int> valleys;
  int n = 5;  // ��Χ n ���㵥���������ж�Ϊ��/��ֵ

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

  // �������еķ�/��ֵ���׼�Ĺؼ�����ƥ��
  vector<Point2i> keypoints_detect;
  for (int i = 0; i != peaks.size(); ++i) {
    keypoints_detect.push_back(boundary_[peaks[i]]);
  }
  for (int i = 0; i != valleys.size(); ++i) {
    keypoints_detect.push_back(boundary_[valleys[i]]);
  }

  match = Match(keypoints, keypoints_detect, threshold);
}
