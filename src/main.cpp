#include "dip.hpp"

int main(int argc, char *argv[]) {
  int device_id = 0;
  LivenessDetector detector(device_id);
  detector.Detect();
}