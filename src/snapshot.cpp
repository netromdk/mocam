#include <QDebug>
#include <QImage>
#include <QString>
#include <QCoreApplication>

#include <memory>
#include <iostream>

#include "Util.h"
#include "VideoDevice.h"
#include "CaptureSession.h"
using namespace mocam;

struct Arguments {
  Arguments() : list(false) { }

  QString filename, device;
  bool list;
  // format
};

void usage(char **argv) {
  qDebug() << "Usage: " << argv[0] << " (options) <output filename>" << endl
           << "Options:" << endl
           << "  --help | -h            Shows this message."
           << endl
    /*
      << "  --format | -f <fmt>   Snapshot format: 'jpg' or 'png'. Default is 'jpg'."
      << endl
    */
           << "  --device | -d <id>    The device to take a snapshot from."
           << endl
           << "  --list | -l           List all available video devices on the system.";
}

std::unique_ptr<Arguments> parseArgs(int argc, char **argv) {
  if (argc < 2) {
    return nullptr;
  }

  auto args = std::unique_ptr<Arguments>(new Arguments);

  int lastOpt = 0;
  for (int i = 1; i < argc; i++) {
    QString arg = QString::fromUtf8(argv[i]).trimmed().toLower();
    if (arg.startsWith("--help") || arg.startsWith("-h")) {
      usage(argv);
      exit(0);
    }
    else if (arg.startsWith("--device") || arg.startsWith("-d")) {
      if (i >= argc - 1) {
        qCritical() << "Specify the device!";
        return nullptr;
      }

      i++;
      args->device = QString::fromUtf8(argv[i]);
      if (i > lastOpt) lastOpt = i;
    }
    else if (arg.startsWith("--list") || arg.startsWith("-l")) {
      args->list = true;
    }
  }

  // If last opt was after the filename then stop.
  if (lastOpt == argc - 1) {
    return nullptr;
  }

  args->filename = QString::fromUtf8(argv[argc - 1]);
  return args;
}

int main(int argc, char **argv) {
  QCoreApplication app(argc, argv);

  auto args = parseArgs(argc, argv);
  if (args == nullptr) {
    usage(argv);
    return -1;
  }

  if (args->list) {
    auto defDev = VideoDevice::getDefaultDevice();
    qDebug() << "Devices available on the system: (* = default)";
    foreach (const auto &dev, VideoDevice::getSystemDevices()) {
      qDebug() << "  " << dev << (dev == defDev ? "*" : "");
    }
    return 0;
  }

  VDPtr device = nullptr;
  if (args->device.isEmpty()) {
    device = VideoDevice::getDefaultDevice();
    qDebug() << "Using default device:" << device->getName();
  }
  else {
    foreach (const auto &dev, VideoDevice::getSystemDevices()) {
      if (dev->getUniqueId() == args->device) {
        device = dev;
        break;
      }
    }
    if (device == nullptr) {
      qCritical() << "Device not found:" << args->device << endl
                  << "Check list of available devices with the --list option.";
      return -1;
    }
    else {
      qDebug() << "Using device:" << device->getName();
    }
  }

  device->init();

  CaptureSession session;
  if (!session.setDevice(device)) {
    qCritical() << "Could not associate device with capture session.";
    return -1;
  }

  std::cout << "Getting snapshot..";
  std::cout.flush();
  
  QImage img = session.getSnapshot();
  if (img.isNull()) {
    qCritical() << endl << "Failed to get snapshot.";
    return -1;
  }

  std::cout << " done!" << std::endl;

  if (img.save(args->filename)) {
    qDebug() << "Saved to file:" << args->filename;
  }
  else {
    qDebug() << "Could not save to file:" << args->filename;
  }

  return 0;
}
