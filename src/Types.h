#ifndef MOCAM_TYPES_H
#define MOCAM_TYPES_H

#include <memory>

#include <opencv2/opencv.hpp>

#include "Face.h"

namespace mocam {
  typedef std::shared_ptr<cv::Mat> MatPtr;
  typedef std::shared_ptr<Face> FacePtr;
}

#endif // MOCAM_TYPES_H
