#include <memory>
#include <iostream>

#include "Util.h"
#include "VideoDevice.h"
#include "CaptureSession.h"
using namespace mocam;

struct Arguments {
  Arguments() : list(false) { }

  std::string filename, device;
  bool list;
  // format
};

void usage(char **argv) {
  using namespace std;
  cout << "Usage: " << argv[0] << " (options) <output filename>" << endl
       << "Options:" << endl
       << "  --help | h            Shows this message."
       << endl
    /*
       << "  --format | -f <fmt>   Snapshot format: 'jpg' or 'png'. Default is 'jpg'."
       << endl
    */
       << "  --device | -d <id>    The device to take a snapshot from."
       << endl
       << "  --list | -l           List all available video devices on the system."
       << endl;
}

std::unique_ptr<Arguments> parseArgs(int argc, char **argv) {
  if (argc < 2) {
    return nullptr;
  }

  auto args = std::unique_ptr<Arguments>(new Arguments);

  int lastOpt = 0;
  for (int i = 1; i < argc; i++) {
    std::string arg(argv[i]);
    Util::toLower(arg);
    if (arg.find("--help") == 0 || arg.find("-h") == 0) {
      usage(argv);
      exit(0);
    }
    else if (arg.find("--device") == 0 || arg.find("-d") == 0) {
      if (i >= argc - 1) {
        std::cout << "Specify the device!" << std::endl;
        return nullptr;
      }

      i++;
      args->device = std::string(argv[i]);
      if (i > lastOpt) lastOpt = i;
    }
    else if (arg.find("--list") == 0 || arg.find("-l") == 0) {
      args->list = true;
    }
  }

  // If last opt was after the filename then stop.
  if (lastOpt == argc - 1) {
    return nullptr;
  }

  args->filename = argv[argc - 1];
  return args;
}

int main(int argc, char **argv) {
  using namespace std;

  auto args = parseArgs(argc, argv);
  if (args == nullptr) {
    usage(argv);
    return -1;
  }

  if (args->list) {
    auto defDev = VideoDevice::getDefaultDevice();
    cout << "Devices available on the system: (* = default)" << endl;
    auto devs = VideoDevice::getSystemDevices();
    for (auto it = devs.begin(); it != devs.end(); ++it) {
      cout << "  " << (*it)->toString();
      if (*it == defDev) {
        cout << " *";
      }
      cout << endl;
    }
    return 0;
  }

  VDPtr device = nullptr;
  if (args->device.empty()) {
    device = VideoDevice::getDefaultDevice();
    cout << "Using default device: " << device->getName() << endl;
  }
  else {
    auto devs = VideoDevice::getSystemDevices();
    for (auto it = devs.begin(); it != devs.end(); ++it) {
      if ((*it)->getUniqueId() == args->device) {
        device = *it;
        break;
      }
    }
    if (device == nullptr) {
      cout << "Device not found: " << args->device << endl
           << "Check list of available devices with the --list option." << endl;
      return -1;
    }
    else {
      cout << "Using device: " << device->getName() << endl;
    }
  }

  device->init();

  CaptureSession session;
  if (!session.setDevice(device)) {
    cout << "Could not associate device with capture session." << endl;
    return -1;
  }

  cout << "Getting snapshot..";
  cout.flush();
  
  int len;
  const unsigned char *img = session.getSnapshot(len);
  if (!img) {
    cout << endl << "Failed to get snapshot." << endl;
    return -1;
  }

  cout << " done!" << " (" << len << " bytes)" << endl;

  if (Util::writeToFile(args->filename, (char*) img, len)) {
    cout << "Saved to file: " << args->filename << endl;
  }
  else {
    cout << "Could not save to file: " << args->filename << endl;
  }
  delete[] img;

  return 0;
}
