#include <iostream>

#include "video.h"

int main(int argc, char **argv) {
  using namespace std;

  cout << "Supported video devices:" << endl;
  dumpDevices();

  cout << endl;
  getSnapshot();
  return 0;
}
