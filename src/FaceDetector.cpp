#include <QDebug>

#include <vector>

#include "FaceDetector.h"

namespace mocam {
  FaceDetector::FaceDetector(const QString &faceCascade,
                             const QString &eyesCascade) {
    valid = true;
    if (!faceCas.load(faceCascade.toStdString())) {
      qCritical() << "Could not load facial cascade!";
      valid = false;
    }
    if (!eyesCas.load(eyesCascade.toStdString())) {
      qCritical() << "Could not load eyes cascade!";
      valid = false;
    }
  }

  QList<FacePtr> FaceDetector::detect(const MatPtr image) {
    // Make grayscaled version of the image, and equalize the histogram
    // which normalizes the brightness and increases the contrast in the
    // image.
    cv::Mat grayImg;
    cvtColor(*image.get(), grayImg, cv::COLOR_BGR2GRAY);
    equalizeHist(grayImg, grayImg);

    // Detect faces with scale factor 1.1, minimum 3 neighbors and
    // minimum 80x80 face size.
    std::vector<cv::Rect> faces;
    faceCas.detectMultiScale(grayImg, faces, 1.1, 3, 0, cv::Size(80, 80));

    QList<FacePtr> results;
    for (auto it = faces.begin(); it != faces.end(); ++it) {
      const auto &f = *it;
      auto face = FacePtr(new Face);
      face->setFace(f);

      // Detect two eyes for each face with scale factor 1.1, 3
      // min. neighbors and min size of 30x30.
      cv::Mat facePart = grayImg(f);
      std::vector<cv::Rect> eyes;
      eyesCas.detectMultiScale(facePart, eyes, 1.1, 3, 0, cv::Size(30, 30));
      if (eyes.size() >= 2) {
        // Take the two largest.
        auto &eye1 = eyes[0], &eye2 = eyes[1];
        int area1 = eye1.width * eye1.height,
          area2 = eye2.width * eye2.height;
        if (area1 > area2) {
          qSwap<cv::Rect>(eye1, eye2);
          qSwap<int>(area1, area2);
        }
        for (int i = 2; i < eyes.size(); i++) {
          const auto &eye = eyes[i];
          int area = eye.width * eye.height;
          if (area > area1 && area < area2) {
            eye1 = eye;
            area1 = area;
          }
          else if (area > area2 && area < area1) {
            eye2 = eye;
            area2 = area;
          }
          else if (area > area1 && area > area2) {
            eye1 = eye;
            area1 = area;
            qSwap<cv::Rect>(eye1, eye2);
            qSwap<int>(area1, area2);
          }
        }

        // Since we are only looking at the sub-region of the face we
        // need to convert into image coordinates.
        eye1.x += f.x; eye1.y += f.y;
        eye2.x += f.x; eye2.y += f.y;
        face->setEyes(eye1, eye2);
      }

      results << face;
    }
    
    return results;
  }
}
