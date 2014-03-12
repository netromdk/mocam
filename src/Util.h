#ifndef MOCAM_UTIL_H
#define MOCAM_UTIL_H

#include <memory>

#include <opencv2/opencv.hpp>

#include "Types.h"

class QString;
class QByteArray;

namespace mocam {
  class Util {
  public:
    static MatPtr imageToMat(const QString &file);
    static MatPtr imageToMat(const QByteArray &data);
    static MatPtr imageToMat(const char *data, int len);
  };
}

#endif // MOCAM_UTIL_H
