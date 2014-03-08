#include <iostream>
using namespace std;

#include "VideoDevice.h"
using namespace mocam;

int main(int argc, char **argv) {
  auto dev = VideoDevice::getDefaultDevice();
  cout << "Default device: " << dev->toString() << endl;
  return 0;
}
