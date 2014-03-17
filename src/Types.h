#ifndef MOCAM_TYPES_H
#define MOCAM_TYPES_H

#include <QImage>

#include <memory>

#include <opencv2/opencv.hpp>

namespace mocam {
  typedef std::shared_ptr<cv::Mat> MatPtr;
  typedef std::shared_ptr<QImage> FramePtr;

  // Call this ones at application start to register custom types.
  void initTypes();
}

#endif // MOCAM_TYPES_H
