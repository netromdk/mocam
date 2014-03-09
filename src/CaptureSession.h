#ifndef MOCAM_CAPTURE_SESSION_H
#define MOCAM_CAPTURE_SESSION_H

#include <memory>

#include "VideoDevice.h"
#include "CaptureSessionImpl.h"

namespace mocam {
  class CaptureSession;
  typedef std::shared_ptr<CaptureSession> CSPtr;
  
  class CaptureSession {
  public:
    CaptureSession();

    void setDevice(VDPtr device);
    const unsigned char *getSnapshot(int &len);
    void close();

  private:
    ImplCSPtr impl;
  };
}

#endif // MOCAM_CAPTURE_SESSION_H
