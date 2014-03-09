#ifndef MOCAM_CAPTURE_SESSION_IMPL_H
#define MOCAM_CAPTURE_SESSION_IMPL_H

#include <memory>

namespace mocam {
  class CaptureSessionImpl;
  typedef std::shared_ptr<CaptureSessionImpl> ImplCSPtr;
  
  class CaptureSessionImpl {
  public:
    CaptureSessionImpl();
    ~CaptureSessionImpl();

  private:
    void *handle; // AVCaptureSession*
  };
}

#endif // MOCAM_CAPTURE_SESSION_IMPL_H
