#include <QFile>
#include <QDebug>
#include <QImage>
#include <QString>
#include <QCoreApplication>

#include <memory>

#include <opencv2/opencv.hpp>

#include "Util.h"
#include "Types.h"
#include "FaceDetector.h"
using namespace mocam;

struct Arguments {
  Arguments() : noFaces(false), noEyes(false) { }

  QString filename, faceFile, eyesFile, overlayFile, dataFile;
  bool noFaces, noEyes;
};

void usage(char **argv) {
  qDebug() << "Detects faces and eyes in an image."
           << endl << endl
           << "Usage: " << argv[0] << " (<options>) <face file> <eyes file> <image file>"
           << endl << endl
           << "<face file> and <eyes file> must to be cascade files in XML that"
           << endl
           << "are either LBP or HAAR types."
           << endl << endl
           << "Options:"
           << endl
           << "  --help | -h              Shows this message."
           << endl
           << "  --overlay | -o <file>    Write image with overlays to file."
           << endl
           << "  --no-faces | -nf         Don't draw faces to overlay."
           << endl
           << "  --no-eyes | -ne          Don't draw eyes to overlay."
           << endl
           << "  --out-data | -od <file>  Write faces to an XML file.";
}

std::unique_ptr<Arguments> parseArgs(int argc, char **argv) {
  if (argc < 4) {
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
    else if (arg == "--overlay" || arg == "-o") {
      if (i >= argc - 1) {
        qCritical() << "Specify the file to write overlays to!";
        return nullptr;
      }

      i++;
      args->overlayFile = QString::fromUtf8(argv[i]);
      if (i > lastOpt) lastOpt = i;
    }
    else if (arg == "--no-faces" || arg == "-nf") {
      args->noFaces = true;
      if (i > lastOpt) lastOpt = i;
    }
    else if (arg == "--no-eyes" || arg == "-ne") {
      args->noEyes = true;
      if (i > lastOpt) lastOpt = i;
    }
    else if (arg == "--out-data" || arg == "-od") {
      if (i >= argc - 1) {
        qCritical() << "Specify the file to write faces to!";
        return nullptr;
      }

      i++;
      args->dataFile = QString::fromUtf8(argv[i]);
      if (i > lastOpt) lastOpt = i;
    }
  }

  if (!args->overlayFile.isEmpty() && args->noFaces && args->noEyes) {
    qCritical() << "You can't disable the drawing of both faces and eyes!";
    return nullptr;
  }

  // If last opt was after the filename then stop.
  if (lastOpt == argc - 1) {
    return nullptr;
  }

  args->faceFile = QString::fromUtf8(argv[argc - 3]);
  args->eyesFile = QString::fromUtf8(argv[argc - 2]);
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

  QFile f(args->filename);
  if (!f.open(QIODevice::ReadOnly)) {
    qCritical() << "Could not read from image file!";
    return -1;
  }
  QByteArray imageData = f.readAll();
  f.close();

  MatPtr image = Util::imageToMat(imageData);
  if (image == nullptr) {
    qCritical() << "Invalid image!";
    return -1;
  }
  qDebug() << "Loaded image..";

  FaceDetector detector(args->faceFile, args->eyesFile);
  if (!detector.isValid()) {
    qCritical() << "Could not setup facial detector.";
    return -1;
  }

  qDebug() << "Detecting faces..";
  QList<FacePtr> faces = detector.detect(image);
  if (faces.isEmpty()) {
    qDebug() << "Did not find any faces!";
    return 0;
  }

  qDebug() << "Found" << faces.size() << "face(s).";
  qDebug() << faces;

  // Write overlays to image file.
  if (!args->overlayFile.isEmpty()) {
    QImage overlay = QImage::fromData(imageData);
    if (Util::saveOverlays(args->overlayFile, overlay, faces, args->noFaces,
                           args->noEyes)) {
      qDebug() << "Saved overlays to:" << args->overlayFile;
    }
    else {
      qCritical() << "Could not save overlays";
    }
  }

  // Write overlays to XML file.
  if (!args->dataFile.isEmpty()) {
    if (Util::exportFacesXml(args->dataFile, faces)) {
      qDebug() << "Exported faces to:" << args->dataFile;
    }
    else {
      qCritical() << "Could not export faces to XML";
    }
  }
  
  return 0;
}
