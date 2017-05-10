#include "dip.hpp"

int main(int argc, char *argv[]) {
  // open webcam
  int device_id = 0;
  VideoCapture vc;
  vc.open(device_id);

  // Liveness Detector
  LivenessDetector liveness;

  while (true) {
    Mat frame;
    vc >> frame;
    ASSERT(!frame.empty(), "Empty frame!");

    Mat seg, boundary;
    Segmentation(frame, seg, boundary);

    Mat align, keypoints, affine_mat;
    Alignment(seg, align, boundary, keypoints, affine_mat);

    liveness.PushFrame(align);
    bool is_living = liveness.IsLiving();

    DisplayResult(frame, boundary, keypoints, affine_mat, is_living);

    int wait_time = 20;
    waitKey(wait_time);
  }
}