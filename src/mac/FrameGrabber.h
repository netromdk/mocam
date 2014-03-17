#ifndef MOCAM_FRAME_GRABBER_H
#define MOCAM_FRAME_GRABBER_H

#include <QThread>

#include "../Types.h"

namespace mocam {
  class CaptureSessionImpl;
  
  class FrameGrabber : public QThread {
    Q_OBJECT

  public:
    FrameGrabber(CaptureSessionImpl *session);

  protected:
    void run();

  signals:
    void frameCaptured(FramePtr frame);

  private:
    CaptureSessionImpl *session;
  };
}

#endif // MOCAM_FRAME_GRABBER_H
