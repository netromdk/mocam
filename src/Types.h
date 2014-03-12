#ifndef MOCAM_TYPES_H
#define MOCAM_TYPES_H

#include <memory>

#include <opencv2/opencv.hpp>

namespace mocam {
  typedef std::shared_ptr<cv::Mat> MatPtr;
}

#endif // MOCAM_TYPES_H
