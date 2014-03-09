#include <iostream>
using namespace std;

#include "VideoDevice.h"
using namespace mocam;

int main(int argc, char **argv) {
  auto defDev = VideoDevice::getDefaultDevice();

  cout << "Devices available on the system: (* = default)" << endl;
  auto devs = VideoDevice::getSystemDevices();
  for (auto it = devs.begin(); it != devs.end(); ++it) {
    cout << "Device: " << (*it)->toString();
    if (*it == defDev) {
      cout << " *";
    }
    cout << endl;
  }
  return 0;
}
