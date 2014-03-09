#ifndef MOCAM_CAPTURE_SESSION_H
#define MOCAM_CAPTURE_SESSION_H

#include <memory>

#include "CaptureSessionImpl.h"

namespace mocam {
  class CaptureSession;
  typedef std::shared_ptr<CaptureSession> CSPtr;
  
  class CaptureSession {
  public:
    CaptureSession();

  private:
    ImplCSPtr impl;
  };
}

#endif // MOCAM_CAPTURE_SESSION_H
