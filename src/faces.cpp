#include <QDebug>
#include <QImage>
#include <QString>
#include <QCoreApplication>

#include <vector>
#include <memory>

#include <opencv2/opencv.hpp>

#include "Util.h"
#include "Types.h"
using namespace mocam;

struct Arguments {
  QString filename;
};

void usage(char **argv) {
  qDebug() << "Usage: " << argv[0] << " <input file>";
}

std::unique_ptr<Arguments> parseArgs(int argc, char **argv) {
  if (argc < 2) {
    return nullptr;
  }

  auto args = std::unique_ptr<Arguments>(new Arguments);

  /* No options yet.
  for (int i = 1; i < argc; i++) {
    QString arg = QString::fromUtf8(argv[i]).trimmed().toLower();
  }
  */

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

  MatPtr image = Util::imageToMat(args->filename);
  if (image == nullptr) {
    qCritical() << "Invalid image!";
    return -1;
  }
  qDebug() << "Loaded image..";

  cv::CascadeClassifier faceCas, eyesCas;
  if (!faceCas.load("lbpcascade_frontalface.xml")) {
    qCritical() << "Could not load facial cascade!";
  }
  if (!eyesCas.load("haarcascade_eye_tree_eyeglasses.xml")) {
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

    QString msg = QString("  Face at (%1,%2) %3x%4")
      .arg(face.x) .arg(face.y).arg(face.width).arg(face.height);
    qDebug() << qPrintable(msg);

    // Detect two eyes for each face with scale factor 1.1, 3
    // min. neighbors and min size of 30x30.
    cv::Mat facePart = grayImg(face);
    std::vector<cv::Rect> eyes;
    eyesCas.detectMultiScale(facePart, eyes, 1.1, 3, 0 | cv::CASCADE_SCALE_IMAGE,
                             cv::Size(30, 30));
    if (eyes.size() != 2) {
      continue;
    }
    msg = QString("    Eyes at (%1,%2) %3x%4 and (%5,%6) %7x%8")
      .arg(eyes[0].x).arg(eyes[0].y).arg(eyes[0].width).arg(eyes[0].height)
      .arg(eyes[1].x).arg(eyes[1].y).arg(eyes[1].width).arg(eyes[1].height);
    qDebug() << qPrintable(msg);
  }
  
  return 0;
}
