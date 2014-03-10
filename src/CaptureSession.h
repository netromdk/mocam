#ifndef MOCAM_CAPTURE_SESSION_H
#define MOCAM_CAPTURE_SESSION_H

#include <QImage>

#include <memory>

#include "VideoDevice.h"
#include "CaptureSessionImpl.h"

namespace mocam {
  class CaptureSession;
  typedef std::shared_ptr<CaptureSession> CSPtr;
  
  class CaptureSession {
  public:
    CaptureSession();

    bool setDevice(VDPtr device);
    QImage getSnapshot();
    void close();

  private:
    ImplCSPtr impl;
  };
}

#endif // MOCAM_CAPTURE_SESSION_H
