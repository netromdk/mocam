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
}
