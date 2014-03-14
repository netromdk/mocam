#include "Face.h"

namespace mocam {
  Face::Face() : hasFace_(false), hasEye1_(false), hasEye2_(false) {
    
  }
  
  Face::Face(const cv::Rect &face)
    : face(face), hasFace_(true), hasEye1_(false), hasEye2_(false)
  {
    
  }
  
  Face::Face(const cv::Rect &face, const cv::Rect &eye1, const cv::Rect &eye2)
    : face(face), eye1(eye1), eye2(eye2), hasFace_(true), hasEye1_(true),
      hasEye2_(true)
  {
    
  }

  void Face::setEyes(const cv::Rect &eye1, const cv::Rect &eye2) {
    setEye1(eye1);
    setEye2(eye2);
  }
  
  QPair<cv::Rect, cv::Rect> Face::getEyes() const {
    return QPair<cv::Rect, cv::Rect>(eye1, eye2);
  }

  QDebug operator<<(QDebug dbg, FacePtr face) {
    QString msg("{ ");
    if (face->hasFace()) {
      auto f = face->getFace();
      msg.append(QString("face @ (%1,%2) %3x%4")
                 .arg(f.x).arg(f.y).arg(f.width).arg(f.height));
    }
    if (face->hasEyes()) {
      auto e1 = face->getEye1(), e2 = face->getEye2();
      msg.append(QString(", eyes @ (%1,%2) %3x%4 (%5,%6) %7x%8")
                 .arg(e1.x).arg(e1.y).arg(e1.width).arg(e1.height)
                 .arg(e2.x).arg(e2.y).arg(e2.width).arg(e2.height));
    }
    msg.append(" }");
    dbg << qPrintable(msg);
    return dbg;
  }
}
