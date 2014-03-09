#ifndef MOCAM_CAPTURE_SESSION_IMPL_H
#define MOCAM_CAPTURE_SESSION_IMPL_H

#include <memory>

#include "../VideoDevice.h"

namespace mocam {
  class CaptureSessionImpl;
  typedef std::shared_ptr<CaptureSessionImpl> ImplCSPtr;
  
  class CaptureSessionImpl {
  public:
    CaptureSessionImpl();
    ~CaptureSessionImpl();

    void setDevice(VDPtr device);
    const unsigned char *getSnapshot(int &len);
    void close();    

  private:
    void *handleSession, // AVCaptureSession*
      *handleInput, // AVCaptureDeviceInput*
      *handleOutput; // AVCaptureStillImageOutput* fx
    VDPtr device;
  };
}

#endif // MOCAM_CAPTURE_SESSION_IMPL_H
