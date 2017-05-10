#include "dip.hpp"

// TODO

void LivenessDetector::PushFrame(const Mat &frame) {
  this->frames_.push_back(frame); // ?
}

void LivenessDetector::CheckLiveness() {
  // TODO
  this->is_living_ = false;
}
