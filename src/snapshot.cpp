#include <QDebug>
#include <QImage>
#include <QString>
#include <QFileInfo>
#include <QCoreApplication>

#include <memory>
#include <iostream>

#include "Util.h"
#include "VideoDevice.h"
#include "CaptureSession.h"
using namespace mocam;

struct Arguments {
  Arguments() : list(false), quality(-1), width(-1), height(-1) { }

  QString filename, device;
  bool list;
  int quality, // -1 means default with QImage
    width, height;
};

void usage(char **argv) {
  qDebug() << "Usage: " << argv[0] << " (<options>) <output filename>" << endl
           << endl
           << "The format of the snapshot is dictated by the extension of <output filename>."
           << endl
           << "Supported extensions: jpg, jpeg, and png." << endl << endl
           << "Options:" << endl
           << "  --help | -h          Shows this message."
           << endl
           << "  --list | -l          List all available video devices on the system."
           << endl
           << "  --device | -d <str>  The device to take a snapshot from."
           << endl
           << "  --quality | -q <n>   Quality of the snapshot in percentage (1-100)."
           << endl
           << "  --scale | -s <str>   Scales to the defined size given 'WidthxHeight'" << endl
           << "                       in pixels. Or scale keeping the size-ratio by" << endl
           << "                       'Xw' or 'Xh', where 'X' is the size to scale to" << endl
           << "                       in width or height respectively.";
}

std::unique_ptr<Arguments> parseArgs(int argc, char **argv) {
  if (argc < 2) {
    return nullptr;
  }

  auto args = std::unique_ptr<Arguments>(new Arguments);

  int lastOpt = 0;
  for (int i = 1; i < argc; i++) {
    QString arg = QString::fromUtf8(argv[i]).trimmed().toLower();
    if (arg == "--help" || arg == "-h") {
      usage(argv);
      exit(0);
    }
    else if (arg == "--device" || arg == "-d") {
      if (i >= argc - 1) {
        qCritical() << "Specify the device!";
        return nullptr;
      }

      i++;
      args->device = QString::fromUtf8(argv[i]);
      if (i > lastOpt) lastOpt = i;
    }
    else if (arg == "--list" || arg == "-l") {
      args->list = true;
    }
    else if (arg == "--quality" || arg == "-q") {
      if (i >= argc - 1) {
        qCritical() << "Specify the quality in percentage!";
        return nullptr;
      }

      i++;
      bool ok;
      unsigned int q = QString::fromUtf8(argv[i]).toUInt(&ok);
      if (!ok || q == 0) {
        qCritical() << "Invalid quality!";
        return nullptr;
      }

      args->quality = q;
      if (i > lastOpt) lastOpt = i;
    }
    else if (arg == "--scale" || arg == "-s") {
      if (i >= argc - 1) {
        qCritical() << "Specify the scaling!";
        return nullptr;
      }

      i++;
      QString scale = QString::fromUtf8(argv[i]).toLower();
      if (scale.contains("x")) {
        QStringList elms = scale.split("x", QString::SkipEmptyParts);
        if (elms.size() != 2) {
          qCritical() << "Invalid scaling!";
          return nullptr;
        }
        bool ok1, ok2;
        unsigned int w = elms[0].toUInt(&ok1), h = elms[1].toUInt(&ok2);
        if (!ok1 || !ok2 || w == 0 || h == 0) {
          qCritical() << "Invalid scaling!";
          return nullptr;
        }
        args->width = w;
        args->height = h;
        if (i > lastOpt) lastOpt = i;
      }
      else if (scale.endsWith("w")) {
        bool ok;
        unsigned int w = scale.mid(0, scale.size() - 1).toUInt(&ok);
        if (!ok || w == 0) {
          qCritical() << "Invalid scaling!";
          return nullptr;
        }
        args->width = w;
        if (i > lastOpt) lastOpt = i;
      }
      else if (scale.endsWith("h")) {
        bool ok;
        unsigned int h = scale.mid(0, scale.size() - 1).toUInt(&ok);
        if (!ok || h == 0) {
          qCritical() << "Invalid scaling!";
          return nullptr;
        }
        args->height = h;
        if (i > lastOpt) lastOpt = i;
      }
      else {
        qCritical() << "Invalid scaling!";
        return nullptr;
      }
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

  QString ext = QFileInfo(args->filename).suffix().toLower();
  if (ext.isEmpty()) {
    qCritical() << "You have to specify a valid output extension.";
    return -1;
  }

  QStringList exts = QStringList() << "jpg" << "jpeg" << "png";
  if (!exts.contains(ext)) {
    qCritical() << "Invalid output extension:" << ext;
    return -1;
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
  
  QImage img = session.getSnapshot(args->width, args->height);
  if (img.isNull()) {
    qCritical() << endl << "Failed to get snapshot.";
    return -1;
  }

  std::cout << " done!" << std::endl;

  QString qStr = QString("(%1% quality)").arg(args->quality);
  if (img.save(args->filename, 0, args->quality)) {
    qDebug() << "Saved to file:" << args->filename
             << (args->quality != -1 ? qPrintable(qStr) : "");
  }
  else {
    qDebug() << "Could not save to file:" << args->filename;
  }

  return 0;
}
