#ifndef MOCAM_WINDOW_H
#define MOCAM_WINDOW_H

#include <QWidget>

#include "VideoDevice.h"
#include "CaptureSession.h"

class QLabel;

namespace mocam {
  class Window : public QWidget {
  public:
    Window(QWidget *parent = nullptr);
    ~Window();

  private:
    void setupLayout();
    void setupVideo();
    
    QLabel *frameLbl;

    CaptureSession session;
    VDPtr device;
  };
}

#endif // MOCAM_WINDOW_H
