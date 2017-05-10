#include "dip.hpp"

void DisplayResult(const Mat &img, const Mat &boundary, const Mat &keypoints, const Mat &affine_mat, bool is_living) {
  // TODO
  Mat display;
  img.copyTo(display);

  string winname("liveness");
  imshow(winname, display);
}