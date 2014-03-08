#include <iostream>

#include "video.h"

int main(int argc, char **argv) {
  using namespace std;

  cout << "Supported video devices:" << endl;
  dumpDevices();

  cout << endl << "Default device: ";
  dumpDefaultDevice();
  return 0;
}
