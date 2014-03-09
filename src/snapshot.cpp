#include <iostream>
using namespace std;

#include "VideoDevice.h"
#include "CaptureSession.h"
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

  defDev->init();

  CaptureSession session;
  return 0;
}
