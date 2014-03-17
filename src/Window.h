#ifndef MOCAM_WINDOW_H
#define MOCAM_WINDOW_H

#include <QWidget>

#include "Types.h"
#include "VideoDevice.h"
#include "CaptureSession.h"

class QLabel;

namespace mocam {
  class Window : public QWidget {
    Q_OBJECT
    
  public:
    Window(QWidget *parent = nullptr);
    ~Window();

  private slots:
    void onFrameCaptured(FramePtr frame);

  private:
    void setupLayout();
    void setupVideo();
    void setTitle();
    
    QLabel *frameLbl;

    CaptureSession session;
    VDPtr device;
    int fps;
  };
}

#endif // MOCAM_WINDOW_H
