#include <QFile>
#include <QImage>
#include <QBuffer>
#include <QString>
#include <QPainter>
#include <QByteArray>
#include <QXmlStreamWriter>

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
        painter.drawRect(toQRect(face->getFace()));
      }
      if (!noEyes && face->hasEyes()) {
        painter.drawRect(toQRect(face->getEye1()));
        painter.drawRect(toQRect(face->getEye2()));
      }
    }

    painter.end();
    return image.save(outFile);
  }

  bool Util::exportFacesXml(const QString &outFile,
                            const QList<FacePtr> &faces) {
    QFile file(outFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
      return false;
    }

    QXmlStreamWriter writer(&file);
    writer.setAutoFormattingIndent(2);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("Faces");
    
    foreach (const auto &face, faces) {
      if (!face->hasFace()) continue;

      const auto f = face->getFace();
      writer.writeStartElement("Face");
      writer.writeAttribute("x", QString::number(f.x));
      writer.writeAttribute("y", QString::number(f.y));
      writer.writeAttribute("width", QString::number(f.width));
      writer.writeAttribute("height", QString::number(f.height));

      if (face->hasEyes()) {
        const auto e1 = face->getEye1();
        writer.writeStartElement("Eye1");
        writer.writeAttribute("x", QString::number(e1.x));
        writer.writeAttribute("y", QString::number(e1.y));
        writer.writeAttribute("width", QString::number(e1.width));
        writer.writeAttribute("height", QString::number(e1.height));
        writer.writeEndElement();

        const auto e2 = face->getEye2();        
        writer.writeStartElement("Eye2");
        writer.writeAttribute("x", QString::number(e2.x));
        writer.writeAttribute("y", QString::number(e2.y));
        writer.writeAttribute("width", QString::number(e2.width));
        writer.writeAttribute("height", QString::number(e2.height));
        writer.writeEndElement();        
      }
      
      writer.writeEndElement();
    }
    
    writer.writeEndElement();
    writer.writeEndDocument();
    
    file.flush();    
    file.close();
    return true;
  }
}
