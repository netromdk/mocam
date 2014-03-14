#ifndef MOCAM_FACE_DETECTOR_H
#define MOCAM_FACE_DETECTOR_H

#include <QList>

#include <opencv2/opencv.hpp>

#include "Types.h"

class QString;

namespace mocam {
  class FaceDetector {
  public:
    FaceDetector(const QString &faceCascade, const QString &eyesCascade);

    bool isValid() const { return valid; }

    QList<FacePtr> detect(const MatPtr image);

  private:
    bool valid;
    cv::CascadeClassifier faceCas, eyesCas;
  };
}

#endif // MOCAM_FACE_DETECTOR_H
