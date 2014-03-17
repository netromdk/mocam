#ifndef MOCAM_CAPTURE_SESSION_IMPL_H
#define MOCAM_CAPTURE_SESSION_IMPL_H

#include <QImage>

#include <memory>

#include "../VideoDevice.h"

namespace mocam {
  class CaptureSessionImpl;
  typedef std::shared_ptr<CaptureSessionImpl> ImplCSPtr;
  
  class CaptureSessionImpl {
  public:
    CaptureSessionImpl();
    ~CaptureSessionImpl();

    bool setDevice(VDPtr device);
    QImage getSnapshot();
    void start();
    void stop();    
    void close();    

  private:
    void *handleSession, // AVCaptureSession*
      *handleInput, // AVCaptureDeviceInput*
      *handleOutput; // AVCaptureStillImageOutput* fx
    VDPtr device;
  };
}

#endif // MOCAM_CAPTURE_SESSION_IMPL_H
