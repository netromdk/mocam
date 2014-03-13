#include <QFile>
#include <QImage>
#include <QBuffer>
#include <QString>
#include <QByteArray>

#include "Util.h"

namespace mocam {
  MatPtr Util::imageToMat(const QString &file) {
    QFile f(file);
    if (!f.open(QIODevice::ReadOnly)) {
      return nullptr;
    }
    return imageToMat(f.readAll());
  }

  MatPtr Util::imageToMat(const QImage &image, const char *fmt) {
    QByteArray arr;
    QBuffer buf(&arr);
    buf.open(QIODevice::WriteOnly);
    image.save(&buf, fmt);
    return imageToMat(arr);
  }

  MatPtr Util::imageToMat(const QByteArray &data) {
    return imageToMat(data.constData(), data.size());
  }

  MatPtr Util::imageToMat(const char *data, int len) {
    if (!data || len <= 0) {
      return nullptr;
    }

    std::vector<char> vec(data, data + len);
    if (vec.size() == 0) {
      return nullptr;
    }

    auto mat = MatPtr(new cv::Mat);
    imdecode(cv::Mat(vec), CV_LOAD_IMAGE_COLOR, mat.get());
    return mat;
  }
}
