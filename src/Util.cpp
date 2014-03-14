#include <QFile>
#include <QImage>
#include <QBuffer>
#include <QString>
#include <QPainter>
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

  QRect Util::toQRect(const cv::Rect &rect) {
    return QRect(rect.x, rect.y, rect.width, rect.height);
  }

  bool Util::saveOverlays(const QString &outFile, QImage &image,
                          const QList<FacePtr> &faces, bool noFaces,
                          bool noEyes) {
    QPainter painter;
    painter.begin(&image);

    QPen pen = painter.pen();
    pen.setWidth(3);
    painter.setPen(pen);

    foreach (const auto &face, faces) {
      if (!noFaces) {
        painter.drawRect(Util::toQRect(face->getFace()));
      }
      if (!noEyes && face->hasEyes()) {
        painter.drawRect(Util::toQRect(face->getEye1()));
        painter.drawRect(Util::toQRect(face->getEye2()));
      }
    }

    painter.end();
    return image.save(outFile);
  }
}
