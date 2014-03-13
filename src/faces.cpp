#include <QFile>
#include <QDebug>
#include <QImage>
#include <QString>
#include <QPainter>
#include <QCoreApplication>

#include <vector>
#include <memory>

#include <opencv2/opencv.hpp>

#include "Util.h"
#include "Types.h"
using namespace mocam;

struct Arguments {
  QString filename, faceFile, eyesFile, overlayFile;
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
           << "  --overlay | -o <file>       Write image with overlays to file.";
}

std::unique_ptr<Arguments> parseArgs(int argc, char **argv) {
  if (argc < 4) {
    return nullptr;
  }

  auto args = std::unique_ptr<Arguments>(new Arguments);

  int lastOpt = 0;
  for (int i = 1; i < argc; i++) {
    QString arg = QString::fromUtf8(argv[i]).trimmed().toLower();
    if (arg == "--overlay" || arg == "-o") {
      if (i >= argc - 1) {
        qCritical() << "Specify the file to write overlays to!";
        return nullptr;
      }

      i++;
      args->overlayFile = QString::fromUtf8(argv[i]);
      if (i > lastOpt) lastOpt = i;
    }
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

  // Prepare for overlay drawing.
  QImage overlay;
  QPainter painter;
  bool doOverlay = !args->overlayFile.isEmpty();
  if (doOverlay) {
    overlay = QImage::fromData(imageData);
    painter.begin(&overlay);

    QPen pen = painter.pen();
    pen.setWidth(3);
    painter.setPen(pen);
  }

  cv::CascadeClassifier faceCas, eyesCas;
  if (!faceCas.load(args->faceFile.toStdString())) {
    qCritical() << "Could not load facial cascade!";
  }
  if (!eyesCas.load(args->eyesFile.toStdString())) {
    qCritical() << "Could not load eyes cascade!";
  }
  qDebug() << "Loaded cascade files..";

  // Make grayscaled version of the image, and equalize the histogram
  // which normalizes the brightness and increases the contrast in the
  // image.
  cv::Mat grayImg;
  cvtColor(*image.get(), grayImg, cv::COLOR_BGR2GRAY);
  equalizeHist(grayImg, grayImg);
  qDebug() << "Created grayscale image and equalized histogram..";

  // Detect faces with scale factor 1.1, minimum 3 neighbors and
  // minimum 80x80 face size.
  qDebug() << "Detect faces..";
  std::vector<cv::Rect> faces;
  faceCas.detectMultiScale(grayImg, faces, 1.1, 3, 0, cv::Size(80, 80));

  for (auto it = faces.begin(); it != faces.end(); ++it) {
    const auto &face = *it;
    QRect faceRect(face.x, face.y, face.width, face.height);

    QString msg = QString("  Face at (%1,%2) %3x%4")
      .arg(face.x) .arg(face.y).arg(face.width).arg(face.height);
    qDebug() << qPrintable(msg);
 
    painter.drawRect(faceRect);

    // Detect two eyes for each face with scale factor 1.1, 3
    // min. neighbors and min size of 30x30.
    cv::Mat facePart = grayImg(face);
    std::vector<cv::Rect> eyes;
    eyesCas.detectMultiScale(facePart, eyes, 1.1, 3, 0 | cv::CASCADE_SCALE_IMAGE,
                             cv::Size(25, 25));
    if (eyes.size() != 2) {
      continue;
    }
    QRect eye1(eyes[0].x + face.x, eyes[0].y + face.y, eyes[0].width, eyes[0].height),
      eye2(eyes[1].x + face.x, eyes[1].y + face.y, eyes[1].width, eyes[1].height);
    msg = QString("    Eyes at (%1,%2) %3x%4 and (%5,%6) %7x%8")
      .arg(eye1.x()).arg(eye1.y()).arg(eye1.width()).arg(eye1.height())
      .arg(eye2.x()).arg(eye2.y()).arg(eye2.width()).arg(eye2.height());
    qDebug() << qPrintable(msg);

    painter.drawRect(eye1);
    painter.drawRect(eye2);
  }

  // Save overlay to file.
  if (doOverlay) {
    painter.end();
    if (overlay.save(args->overlayFile)) {
      qDebug() << "Saved overlay to:" << args->overlayFile;
    }
    else {
      qCritical() << "Could not save overlay.";
    }
  }
  
  return 0;
}
