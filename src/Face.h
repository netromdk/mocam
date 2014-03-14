#ifndef MOCAM_FACE_H
#define MOCAM_FACE_H

#include <QPair>

#include <opencv2/opencv.hpp>

namespace mocam {
  class Face {
  public:
    Face();
    Face(const cv::Rect &face);
    Face(const cv::Rect &face, const cv::Rect &eye1, const cv::Rect &eye2);

    void setFace(const cv::Rect &face) { this->face = face; hasFace_ = true; }
    cv::Rect getFace() const { return face; }
    bool hasFace() const { return hasFace_; }

    void setEyes(const cv::Rect &eye1, const cv::Rect &eye2);
    QPair<cv::Rect, cv::Rect> getEyes() const;
    bool hasEyes() const { return hasEye1_ && hasEye2_; }

    void setEye1(const cv::Rect &eye1) { this->eye1 = eye1; hasEye1_ = true; }
    cv::Rect getEye1() const { return eye1; }
    bool hasEye1() const { return hasEye1_; }

    void setEye2(const cv::Rect &eye2) { this->eye2 = eye2; hasEye2_ = true; }
    cv::Rect getEye2() const { return eye2; }
    bool hasEye2() const { return hasEye2_; }    

  private:
    cv::Rect face, eye1, eye2;
    bool hasFace_, hasEye1_, hasEye2_;
  };
}

#endif // MOCAM_FACE_H
