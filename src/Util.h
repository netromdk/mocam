#ifndef MOCAM_UTIL_H
#define MOCAM_UTIL_H

#include <QRect>
#include <QList>

#include <memory>

#include <opencv2/opencv.hpp>

#include "Face.h"
#include "Types.h"

class QImage;
class QString;
class QByteArray;

namespace mocam {
  class Util {
  public:
    static MatPtr imageToMat(const QString &file);
    static MatPtr imageToMat(const QImage &image, const char *fmt = "PNG");
    static MatPtr imageToMat(const QByteArray &data);
    static MatPtr imageToMat(const char *data, int len);

    static QRect toQRect(const cv::Rect &rect);

    static bool saveOverlays(const QString &outFile, QImage &image,
                             const QList<FacePtr> &faces, bool noFaces = false,
                             bool noEyes = false);
  };
}

#endif // MOCAM_UTIL_H
